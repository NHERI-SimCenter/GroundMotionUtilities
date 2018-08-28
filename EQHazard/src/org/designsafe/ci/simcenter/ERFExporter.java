package org.designsafe.ci.simcenter;

import java.io.*;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.opensha.commons.geo.LocationList;
import org.opensha.commons.util.FileUtils;
import org.opensha.sha.earthquake.ERF;
import org.opensha.sha.earthquake.EqkRupture;
import org.opensha.sha.earthquake.ProbEqkRupture;
import org.opensha.sha.earthquake.ProbEqkSource;

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

}
