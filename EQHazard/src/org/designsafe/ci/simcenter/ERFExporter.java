package org.designsafe.ci.simcenter;

import java.io.*;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import org.opensha.commons.geo.LocationList;
import org.apache.commons.math3.util.Pair;
import org.opensha.commons.geo.Location;
import org.opensha.commons.util.FileUtils;
import org.opensha.sha.cybershake.calc.RuptureProbabilityModifier;
import org.opensha.sha.earthquake.ERF;
import org.opensha.sha.earthquake.EqkRupture;
import org.opensha.sha.earthquake.ProbEqkRupture;
import org.opensha.sha.earthquake.ProbEqkSource;
import org.opensha.sha.faultSurface.FaultTrace;
import org.opensha.sha.faultSurface.PointSurface;
import org.opensha.sha.faultSurface.RuptureSurface;

import com.google.common.io.*;
import com.google.gson.*;


public class ERFExporter {
	
	public static void main(String[] args) {
		EQHazardCalc eqCalc = new EQHazardCalc();
		
		List<String> erfNames = Arrays.asList("USGS/CGS 2002 Adj. Cal. ERF", "GEM1 CEUS ERF", "Mean UCERF3 FM3.1", "Mean UCERF3 FM3.2", "Mean UCERF3", "WGCEP (2007) UCERF2 - Single Branch", "GEM1 WEUS ERF");
		
		for (String erfName: erfNames)
		{
			ERF erf = eqCalc.getERF(erfName);
			ExportToJson(erf);
		}
	}
	
	private static void ExportToCSV(ERF erf)
	{
		try
		{
			Writer outWriter = new BufferedWriter(new OutputStreamWriter(
		              new FileOutputStream("D:\\TestERF.csv"), "utf-8"));
			outWriter.write("SourceIndex, RuptureIndex, Magnitude\n");
			
			int nSources = erf.getNumSources();
			for (int i = 0; i < nSources; i++)
			{
				ProbEqkSource rupSource = erf.getSource(i);

				List<ProbEqkRupture> rupList = rupSource.getRuptureList();
				for(int j = 0; j < rupList.size(); j++)
				{
					ProbEqkRupture rupture = rupList.get(j);
					outWriter.write(String.valueOf(i));
					outWriter.write(',');
					outWriter.write(String.valueOf(j));
					outWriter.write(',');
					outWriter.write(String.valueOf(rupture.getMag()));
					outWriter.write("\n");
				}
			}
			
			outWriter.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	private static void ExportToJson(ERF erf)
	{
		JsonObject erfJson = new JsonObject();
		JsonArray sourcesJson = new JsonArray();
		erfJson.add("Sources", sourcesJson);


		int nSources = erf.getNumSources();
		for (int i = 0; i < nSources; i++)
		{
			JsonObject sourceJson = new JsonObject();

			ProbEqkSource rupSource = erf.getSource(i);
			sourceJson.addProperty("Name", rupSource.getName());
			
			JsonArray locationsJson = new JsonArray();
			
			
			JsonObject rupturesJson = new JsonObject();
			sourceJson.add("Ruptures", rupturesJson);
			JsonArray magnitudesJson = new JsonArray();
			rupturesJson.add("Magnitudes", magnitudesJson);
			
			List<ProbEqkRupture> rupList = rupSource.getRuptureList();
			for(int j = 0; j < rupList.size(); j++)
			{
				ProbEqkRupture rupture = rupList.get(j);
				magnitudesJson.add(rupture.getMag());
			}

			sourcesJson.add(sourceJson);
		}
		
		//Now we can export
		Gson gson = new GsonBuilder().setPrettyPrinting().create();
		String jsonString = gson.toJson(erfJson);
		
		FileUtils.save(erf.getName().replace('/', '-') + ".json", jsonString);
	}

	public static void ExportToGeoJson(ERF erf, String filename, double maxDistance, Location siteLocation, int maxSources)
	{
		JsonObject erfGeoJson = new JsonObject();
		JsonArray featuresJson = new JsonArray();
		erfGeoJson.add("type", new JsonPrimitive("FeatureCollection"));
		
		System.out.print("Calculating distances...");
		long erfStartTime = System.currentTimeMillis();
		int nSources = erf.getNumSources();
		ArrayList<Pair<Integer, Double>> distances = new ArrayList<Pair<Integer, Double>>();
		
		for (int i = 0; i < nSources; i++)
		{
			ProbEqkSource rupSource = erf.getSource(i);
			
			RuptureSurface sourceSurface = rupSource.getSourceSurface();
			double distanceToSource = sourceSurface.getDistanceRup(siteLocation);
			distances.add(new Pair<Integer, Double>(i, distanceToSource));			
		}
		
		long erfStopTime = System.currentTimeMillis();
	    long erfElapsedTime = erfStopTime - erfStartTime;
	    System.out.println("[Time Elapsed: " + erfElapsedTime/1000.0 + " Sec.]");
	    
	    System.out.print("Sorting sources...");
		long sortStartTime = System.currentTimeMillis();
		
		distances.sort(new Comparator<Pair<Integer, Double>>()
				{
					@Override
				    public int compare(final Pair<Integer, Double> p1, final Pair<Integer, Double> p2) {
						if(p1.getValue() > p2.getValue())
							return 1;
						
						if (p1.getValue().equals(p2.getValue()) && p1.getKey() > p2.getKey())
								return 1;				

						return -1;
			    }});
		
		long sortStopTime = System.currentTimeMillis();
	    long sortElapsedTime = sortStopTime - sortStartTime;
	    System.out.println("[Time Elapsed: " + sortElapsedTime/1000.0 + " Sec.]");

	    if (maxSources > nSources)
	    	maxSources = nSources;
	    
		for (int i = 0; i < maxSources; i++)
		{
			Pair<Integer, Double> pair = distances.get(i);
			int sourceIndex = pair.getKey();
			double distanceToSource = pair.getValue();

			ProbEqkSource rupSource = erf.getSource(sourceIndex);
			
			if(distanceToSource > maxDistance)
				break;
			
			List<ProbEqkRupture> rupList = rupSource.getRuptureList();
			ArrayList<Pair<Integer, Double>> distancesToRups = new ArrayList<Pair<Integer, Double>>();
			for(int j = 0; j <  rupList.size(); j ++)
			{		
				ProbEqkRupture rupture = rupList.get(j);
				double distanceToRup = rupture.getRuptureSurface().getDistanceRup(siteLocation);
				distancesToRups.add(new Pair<Integer, Double>(j, distanceToRup));
			}
			
			distancesToRups.sort(new Comparator<Pair<Integer, Double>>()
			{
				@Override
			    public int compare(final Pair<Integer, Double> p1, final Pair<Integer, Double> p2) {
					if(p1.getValue() > p2.getValue())
						return 1;
					
					if (p1.getValue().equals(p2.getValue()) && p1.getKey() > p2.getKey())
						return 1;	
					
					return -1;
		    }});
			
			for(int j = 0; j <  rupList.size(); j ++)
			{
				pair = distancesToRups.get(j);
				int ruptureIndex = pair.getKey();
				double distanceToRup = pair.getValue();
				
				ProbEqkRupture rupture = rupList.get(ruptureIndex);
				System.out.print("\rProcessing rupture " + ruptureIndex + " of Source " + sourceIndex);

				JsonObject ruptureJson = new JsonObject();
				JsonObject ruptureGeometryJson = new JsonObject();
				JsonObject rupturePropretiesJson = new JsonObject();
				
				RuptureSurface ruptureSurface = rupture.getRuptureSurface();

				if (ruptureSurface.isPointSurface())
				{
					PointSurface pointSurface = (PointSurface)ruptureSurface;
					Location location = pointSurface.getLocation();
					ruptureGeometryJson.add("type", new JsonPrimitive("Point"));
					JsonArray coordinates = new JsonArray();
					coordinates.add(location.getLongitude());
					coordinates.add(location.getLatitude());
					ruptureGeometryJson.add("coordinates", coordinates);
				}
				else
				{
					ruptureGeometryJson.add("type", new JsonPrimitive("LineString"));
					JsonArray coordinates = new JsonArray();
					
					FaultTrace trace;
					try
					{
						trace = ruptureSurface.getUpperEdge();
					}
					catch(Exception e)
					{
						trace = ruptureSurface.getEvenlyDiscritizedUpperEdge();
					}
					
					for (Location location : trace)
					{
						//TODO: change to corners
						JsonArray pointCoordinates = new JsonArray();
						pointCoordinates.add(location.getLongitude());
						pointCoordinates.add(location.getLatitude());
						coordinates.add(pointCoordinates);
					}
	
					ruptureGeometryJson.add("coordinates", coordinates);
				}
				
				rupturePropretiesJson.add("Name", new JsonPrimitive(rupSource.getName()));
				rupturePropretiesJson.add("Rupture", new JsonPrimitive(ruptureIndex));
				rupturePropretiesJson.add("Source", new JsonPrimitive(sourceIndex));
				rupturePropretiesJson.add("Distance", new JsonPrimitive(distanceToRup));

				double distanceRup = rupture.getRuptureSurface().getDistanceRup(siteLocation);
				rupturePropretiesJson.add("DistanceRup", new JsonPrimitive(distanceRup));

				double distanceSeis = rupture.getRuptureSurface().getDistanceSeis(siteLocation);
				rupturePropretiesJson.add("DistanceSeis", new JsonPrimitive(distanceSeis));
				
				double distanceJB = rupture.getRuptureSurface().getDistanceJB(siteLocation);
				rupturePropretiesJson.add("DistanceJB", new JsonPrimitive(distanceJB));
				
				double distanceX = rupture.getRuptureSurface().getDistanceX(siteLocation);
				rupturePropretiesJson.add("DistanceX", new JsonPrimitive(distanceX));

				rupturePropretiesJson.add("Magnitude", new JsonPrimitive(rupture.getMag()));
				rupturePropretiesJson.add("Probability", new JsonPrimitive(rupture.getProbability()));
				double meanAnnualRate = rupture.getMeanAnnualRate(erf.getTimeSpan().getDuration());
				rupturePropretiesJson.add("MeanAnnualRate", new JsonPrimitive(meanAnnualRate));
		
									
				ruptureJson.add("type", new JsonPrimitive("Feature"));				
				ruptureJson.add("geometry", ruptureGeometryJson);
				ruptureJson.add("properties", rupturePropretiesJson);

				featuresJson.add(ruptureJson);
			}
		}
		
		erfGeoJson.add("features", featuresJson);

		
		//Now we can export
		Gson gson = new GsonBuilder().setPrettyPrinting().create();
		String jsonString = gson.toJson(erfGeoJson);
		
		FileUtils.save(filename, jsonString);
	}
}
