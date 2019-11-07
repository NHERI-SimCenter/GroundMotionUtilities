package org.designsafe.ci.simcenter;

import java.io.*;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.opensha.commons.geo.LocationList;
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

		int nSources = erf.getNumSources();
		int count = 0;
		for (int i = 0; i < nSources; i++)
		{
			if (count >= maxSources)
				break;
				
			ProbEqkSource rupSource = erf.getSource(i);
			
			RuptureSurface sourceSurface = rupSource.getSourceSurface();
			double distanceToRup = sourceSurface.getDistanceRup(siteLocation);

			System.out.print("\rProcessing Source " + i + " of "+ nSources + " [Found "+ count +" Sources]");

			if(distanceToRup <= maxDistance)
			{				
				count = count + 1;
				List<ProbEqkRupture> rupList = rupSource.getRuptureList();
				for(int j = 0; j <  rupList.size(); j ++)
				{
					
					ProbEqkRupture rupture = rupList.get(j);
					System.out.print("\rProcessing rupture " + j+1 + " of Source " + i);

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
					rupturePropretiesJson.add("Rupture", new JsonPrimitive(j));
					rupturePropretiesJson.add("Source", new JsonPrimitive(i));
					rupturePropretiesJson.add("Distance", new JsonPrimitive(distanceToRup));
					rupturePropretiesJson.add("Magnitude", new JsonPrimitive(rupture.getMag()));
										
					ruptureJson.add("type", new JsonPrimitive("Feature"));				
					ruptureJson.add("geometry", ruptureGeometryJson);
					ruptureJson.add("properties", rupturePropretiesJson);

					featuresJson.add(ruptureJson);
				}
				
//				JsonObject sourceJson = new JsonObject();
//				JsonObject sourceGeometryJson = new JsonObject();
//				JsonObject sourcePropretiesJson = new JsonObject();
	

				
	
				//Reading ruptures magnitude
//				List<ProbEqkRupture> rupList = rupSource.getRuptureList();				
//				JsonArray rupturesMagnitudes = new JsonArray();
//				for(int j = 0; j < rupList.size(); j++)
//				{
//					sourceJson.add("type", new JsonPrimitive("Feature"));				
//					ProbEqkRupture rupture = rupList.get(j);				
//					rupturesMagnitudes.add(rupture.getMag());	
//				}
				
				


				//sourceJson.add("geometry", sourceGeometryJson);
				//sourceJson.add("properties", sourcePropretiesJson);
				//featuresJson.add(sourceJson);
			}
		}
		
		erfGeoJson.add("features", featuresJson);

		
		//Now we can export
		Gson gson = new GsonBuilder().setPrettyPrinting().create();
		String jsonString = gson.toJson(erfGeoJson);
		
		FileUtils.save(filename, jsonString);
	}
}
