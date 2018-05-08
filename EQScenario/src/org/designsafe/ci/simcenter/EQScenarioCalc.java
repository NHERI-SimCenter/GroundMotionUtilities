package org.designsafe.ci.simcenter;

import java.io.*;
import java.lang.reflect.*;
import java.nio.file.Paths;
import java.util.*;

import org.opensha.commons.data.*;
import org.opensha.commons.data.siteData.*;
import org.opensha.commons.geo.*;
import org.opensha.commons.param.*;
import org.opensha.commons.param.Parameter;
import org.opensha.commons.param.event.*;
import org.opensha.commons.util.*;
import org.opensha.sha.earthquake.*;
import org.opensha.sha.earthquake.param.*;
import org.opensha.sha.earthquake.rupForecastImpl.Frankel02.*;
import org.opensha.sha.earthquake.rupForecastImpl.GEM1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.MeanUCERF2.*;
import org.opensha.sha.faultSurface.*;
import org.opensha.sha.imr.*;
import org.opensha.sha.imr.attenRelImpl.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.NGAW2_Wrappers.*;
import org.opensha.sha.imr.param.IntensityMeasureParams.*;
import org.opensha.sha.imr.param.OtherParams.*;
import org.opensha.sha.util.*;

import com.google.common.base.*;
import com.google.common.io.*;
import com.google.gson.*;

import scratch.UCERF3.erf.mean.MeanUCERF3;

public class EQScenarioCalc implements ParameterChangeWarningListener {
	private HashMap<String, String> imrMap;
	private SHAOutput output;
	private OrderedSiteDataProviderList siteDataProviders;
	public static final double dTol = 1e-3;
	
	public EQScenarioCalc()
	{
		MapIMRs();
		InitSiteDataProviders();
	}
	
	@SuppressWarnings("deprecation")
	private void MapIMRs()
	{
		imrMap = new HashMap<String, String>();
		
		//NGA-West2 IMRs
		imrMap.put(ASK_2014.NAME, ASK_2014_Wrapper.class.getName());//Abrahamson, Silva & Kamai (2014)
		imrMap.put(BSSA_2014.NAME, BSSA_2014_Wrapper.class.getName());//Boore, Stewart, Seyhan & Atkinson (2014)
		imrMap.put(CB_2014.NAME, CB_2014_Wrapper.class.getName());//Campbell & Bozorgnia (2014)
		imrMap.put(CY_2014.NAME, CY_2014_Wrapper.class.getName());//Chiou & Youngs (2014)
		imrMap.put(Idriss_2014.NAME, Idriss_2014_Wrapper.class.getName());//Idriss (2014)
		
		//NGA-West IMRs
		imrMap.put(CB_2008_AttenRel.NAME, CB_2008_AttenRel.class.getName());
		imrMap.put(BA_2008_AttenRel.NAME, BA_2008_AttenRel.class.getName());
		imrMap.put(AS_2008_AttenRel.NAME, AS_2008_AttenRel.class.getName());
		imrMap.put(CY_2008_AttenRel.NAME, CY_2008_AttenRel.class.getName());
		
		imrMap.put(BA_2006_AttenRel.NAME, BA_2006_AttenRel.class.getName());
		imrMap.put(CY_2006_AttenRel.NAME, CY_2006_AttenRel.class.getName());
		imrMap.put(CB_2006_AttenRel.NAME, CB_2006_AttenRel.class.getName());
		
		imrMap.put(AS_1997_AttenRel.NAME, AS_1997_AttenRel.class.getName());
		imrMap.put(BJF_1997_AttenRel.NAME, BJF_1997_AttenRel.class.getName());
		imrMap.put(Campbell_1997_AttenRel.NAME, Campbell_1997_AttenRel.class.getName());
		imrMap.put(CB_2003_AttenRel.NAME, CB_2003_AttenRel.class.getName());
		imrMap.put(Field_2000_AttenRel.NAME, Field_2000_AttenRel.class.getName());
		
		//TODO add the rest of IMRs

	}
	
	void InitSiteDataProviders()
	{
		siteDataProviders = OrderedSiteDataProviderList.createSiteDataProviderDefaults();
	}
	
	public OrderedSiteDataProviderList GetSiteDataProviders()
	{
		return siteDataProviders;
	}
	
	public static void main(String[] args) {
		
		if (args.length != 2)
		{
			System.out.println("Usage:");
			System.out.println("\tEQScenario ScenarioConfig.json ScenarioOutput.json");
			return;
		}
		
		EQScenarioCalc calc = new EQScenarioCalc();
		String jsonCfgPath = args[0];
		File cfgFile = new File(jsonCfgPath);
		
		String cfg = "";
		try {
			cfg = Files.toString(cfgFile, Charsets.UTF_8);
		} catch (IOException e) {
			System.out.print(e.getMessage());
			return;
		}
		
		Gson gson = new GsonBuilder().create();
		EQScenarioConfig scenarioConfig = gson.fromJson(cfg, EQScenarioConfig.class);
		calc.PerformSHA(scenarioConfig);
		
		String outFilePath = args[1];
		File outFile = new File(outFilePath);
		String directory = outFile.getAbsoluteFile().getParent();
		String file = outFile.getName();

		calc.WriteOutputs(scenarioConfig, directory, file);
	}

	@Override
	public void parameterChangeWarning(ParameterChangeWarningEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
	public SHAOutput GetOutput()
	{
		return output;		
	}
	
	public void PerformSHA(EQScenarioConfig scenarioConfig)
	{
		GMPEConfig GMPECfg = scenarioConfig.GetGMPEConfig();
		ScalarIMR imr = CreateIMRInstance(GMPECfg.Type());
		
		EqkRupture eqRup = new EqkRupture();
		EqRuptureConfig eqRupCfg = scenarioConfig.GetRuptureConfig();
		
		if(eqRupCfg.Type().equalsIgnoreCase("PointSource"))
		{
			//Set Magnitude
			eqRup.setMag(eqRupCfg.Magnitude());
			//Set Location and Dip
			RuptureLocation rupLocation = eqRupCfg.Location();
			Location eqRupLocation = new Location(rupLocation.Latitude(), rupLocation.Longitude(), rupLocation.Depth());
			eqRup.setPointSurface(eqRupLocation, eqRupCfg.AverageDip());
			eqRup.setAveRake(eqRupCfg.AverageRake());
		}
		else if(eqRupCfg.Type().equalsIgnoreCase("ERF"))
		{
		    System.out.print("Creating ERF...");
			long erfStartTime = System.currentTimeMillis();
			eqRup = getEqRuptureFromERF(eqRupCfg);
			long erfStopTime = System.currentTimeMillis();
		    long erfElapsedTime = erfStopTime - erfStartTime;
		    System.out.println("[Time Elapsed: " + erfElapsedTime/1000.0 + " Sec.]");

			double magnitude = eqRup.getMag();
			double averageDip = eqRup.getRuptureSurface().getAveDip();
			double averageRake = eqRup.getAveRake();
			
			ArrayList<RuptureLocation> eqRupSurface = new ArrayList<RuptureLocation>();
			RuptureSurface surface = eqRup.getRuptureSurface();
			if(surface  instanceof EvenlyGriddedSurface) 
			{				
				EvenlyGriddedSurface griddedSurface = (EvenlyGriddedSurface) surface;
				
				Location corner1 = griddedSurface.get(0, 0);
				eqRupSurface.add(new RuptureLocation(corner1));

				Location corner2 = griddedSurface.get(0, griddedSurface.getNumCols() - 1);
				eqRupSurface.add(new RuptureLocation(corner2));
				
				Location corner3 = griddedSurface.get(griddedSurface.getNumRows() - 1, 0);
				eqRupSurface.add(new RuptureLocation(corner3));
				
				Location corner4 = griddedSurface.get(griddedSurface.getNumRows() - 1, griddedSurface.getNumCols() - 1);
				eqRupSurface.add(new RuptureLocation(corner4));
			}
			else
			{
				for(Location location: eqRup.getRuptureSurface().getPerimeter())
				{
					eqRupSurface.add(new RuptureLocation(location));
				}
			}
			
			if(eqRupSurface.isEmpty())
				eqRupCfg.SetRupture(magnitude, averageDip, averageRake, null);
			else
				eqRupCfg.SetRupture(magnitude, averageDip, averageRake, eqRupSurface);

		}
		
		imr.setEqkRupture(eqRup);
		
		//Get a list of sites locations ready
		SiteConfig siteCfg = scenarioConfig.GetSiteConfig();
		List<SiteLocation> siteLocations = new ArrayList<SiteLocation>();
		if(siteCfg.Type().equalsIgnoreCase("SingleLocation"))
		{
			siteLocations.add(siteCfg.Location());
		}
		else if(siteCfg.Type().equalsIgnoreCase("Grid"))
		{
			Division latitudeCfg = siteCfg.Grid().Latitude();
			double latitudeStep = latitudeCfg.GetStep();
			
			Division longitudeCfg = siteCfg.Grid().Longitude();
			double longitudeStep = longitudeCfg.GetStep();
			
			for(double latitude = latitudeCfg.Min(); latitude <= latitudeCfg.Max() + dTol; latitude += latitudeStep)
			{
				for(double longitude = longitudeCfg.Min(); longitude <= longitudeCfg.Max() + dTol; longitude += longitudeStep)
				{
					siteLocations.add(new SiteLocation(latitude, longitude));
				}
			}
		}
		else
		{
			System.out.print("Invalid Site Type: " + siteCfg.Type() + " !");
			return;
		}
				
		IMConfig imConfig = scenarioConfig.GetIMConfig();

		//First we need to find the type of IM
		boolean isSA = false;
		if(imConfig.Type().equalsIgnoreCase("SA"))
		{
			isSA = true;
			output = new SHAOutput(siteLocations.size(), imConfig.Periods(), scenarioConfig.GetRuptureConfig());
		}
		else if(imConfig.Type().equalsIgnoreCase("PGA"))
		{
			output = new SHAOutput(siteLocations.size(), null, scenarioConfig.GetRuptureConfig());
			isSA = false;
		}
		else
		{
			System.out.print("Invalid IM Type: " + imConfig.Type() + " !");
			return;
		}	
		
		ArrayList<Site> sites = new ArrayList<Site>();
		for(SiteLocation siteLocation: siteLocations)
		{
			Location location = new Location (siteLocation.Latitude(), siteLocation.Longitude());
			sites.add(new Site(location));
		}
		
	    System.out.print("Obtaining Site Data...");
		long sdStartTime = System.currentTimeMillis();
		ArrayList<SiteDataValueList<?>> availableSiteData = null;
		try {
			availableSiteData = siteDataProviders.getAllAvailableData(sites);
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}
		long sdStopTime = System.currentTimeMillis();
	    long sdElapsedTime = sdStopTime - sdStartTime;
	    System.out.println("[Time Elapsed: " + sdElapsedTime/1000.0 + " Sec.]");
	    
		SiteTranslator siteTrans = new SiteTranslator();
		long startTime = System.currentTimeMillis();

		for(int i = 0; i < siteLocations.size(); i++)
		{

			System.out.print("\rProcessing Site " + (i+1));
			SiteLocation siteLocation = siteLocations.get(i);
			SiteResult result;
			
			//set site
			Site site = sites.get(i);
			ArrayList<SiteDataValue<?>> siteDataValues = new ArrayList<SiteDataValue<?>>();
			for(SiteDataValueList<?> providerData:availableSiteData)
			{
				siteDataValues.add(providerData.getValue(i));				
			}
			
			ParameterList imrSiteParams = imr.getSiteParams();
			ArrayList<SiteDataResult> siteDataResults = new ArrayList<SiteDataResult>();

			for(Parameter siteParam:imrSiteParams)
			{	
				Parameter newParam = (Parameter)siteParam.clone();
				//checking if a provider has the value, otherwise set the default
				boolean siteDataFound = siteTrans.setParameterValue(newParam, siteDataValues);

				if(siteDataFound)
				{
					String provider = "Unknown"; 

					provider = getDataSource(newParam.getName(), siteDataValues);
					
					siteDataResults.add(new SiteDataResult(newParam.getName(),
								newParam.getValue(), provider));
				}
				else 
				{
					newParam.setValue(siteParam.getDefaultValue());
					siteDataResults.add(new SiteDataResult(siteParam.getName(),
							siteParam.getDefaultValue(), "Default"));
				}
				site.addParameter(newParam);
			}

			imr.setSite(site);
		      
			StdDevTypeParam stdDevParam = (StdDevTypeParam)imr.getParameter(StdDevTypeParam.NAME);
			
			boolean hasIEStats = stdDevParam.isAllowed(StdDevTypeParam.STD_DEV_TYPE_INTER) &&
					stdDevParam.isAllowed(StdDevTypeParam.STD_DEV_TYPE_INTRA);
			
			imr.setIntensityMeasure(imConfig.Type());
			double[] periods = imConfig.Periods();
			if(isSA)
			{			
				Parameter imtParam = (Parameter)imr.getIntensityMeasure();
				SAResult saResult = new SAResult(periods.length, hasIEStats);	

				for(int j=0; j < imConfig.Periods().length; j++)
				{
					imtParam.getIndependentParameter(PeriodParam.NAME).setValue(periods[j]);
					double mean = imr.getMean();
					stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_TOTAL);
					double stdDev = imr.getStdDev();
					
					if(hasIEStats)
					{
						stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_INTER);
						double interEvStdDev = imr.getStdDev();
						stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_INTRA);
						double intraEvStdDev = imr.getStdDev();
						saResult.SetResult(j, mean, stdDev, interEvStdDev, intraEvStdDev);
					}
					else
					{
						saResult.SetResult(j, mean, stdDev);
					}
				}
				
				if(siteDataValues.isEmpty())
					result = new SiteResult(siteLocation, null, saResult);
				else
					result = new SiteResult(siteLocation, siteDataResults, saResult);

			}
			else
			{
				double mean = imr.getMean();
				stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_TOTAL);
				double stdDev = imr.getStdDev();
				PGAResult pgaResult;	

				if(hasIEStats)
				{
					stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_INTER);
					double interEvStdDev = imr.getStdDev();
					stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_INTRA);
					double intraEvStdDev = imr.getStdDev();
					pgaResult = new PGAResult(mean, stdDev, interEvStdDev, intraEvStdDev);
				}
				else
				{
					pgaResult = new PGAResult(mean, stdDev);
				}
				
				if(siteDataValues.isEmpty())
					result = new SiteResult(siteLocation, null, pgaResult);
				else
					result = new SiteResult(siteLocation, siteDataResults, pgaResult);				
			}
			
			output.SetResult(i, result);
		}
		long stopTime = System.currentTimeMillis();
	    long elapsedTime = stopTime - startTime;
	    System.out.println(", Done! Time Elapsed: " + elapsedTime/1000.0 + " Sec.");
	}
	
	private void WriteOutputs(EQScenarioConfig scenarioConfig, String directory, String file)
	{
		System.out.print("Saving Outputs...");
		long startTime = System.currentTimeMillis();
		//Output
		IMConfig imCfg = scenarioConfig.GetIMConfig();
		if(imCfg.hasJsonOutput())
		{
			Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
			String outputFile = file;
			FileUtils.save(Paths.get(directory, outputFile).toAbsolutePath().toString(), gson.toJson(output));
		}
		
		if(imCfg.hasCsvOutput())
		{
			String outputFile = Files.getNameWithoutExtension(file) + "_Output.csv";
			//TODO
		}
		
		if(imCfg.hasGeoJsonOutput())
		{
			GeoJsonOutput geoOutput = new GeoJsonOutput();
			GeoJsonFeature feature;
			for(SiteResult result: output.Results())
			{
				GeoJsonGeometry geometry = new GeoJsonGeometry(result.Location().Latitude(), result.Location().Longitude());
				feature = new GeoJsonFeature(geometry);

				if(null != result.PGA())
					feature.AddProperty("MeanPGA", result.PGA().Mean());
				
				if(null != result.SA())
				{
					double[] periods = output.Periods();
					for(int i = 0; i< result.SA().Size(); i++)
					{
						String propName = "MeanSA" + periods[i] + "S";
						feature.AddProperty(propName, result.SA().Mean(i));
					}
				}
				
				if(null != result.SiteData())
				{
					for(SiteDataResult data:result.SiteData())
					{
						feature.AddProperty(data.Type(), data.Value());
					}
				}
				geoOutput.AddFeature(feature);
			}
			
			Gson gson = new GsonBuilder().setPrettyPrinting().serializeSpecialFloatingPointValues().create();
			String outputFile = Files.getNameWithoutExtension(file) + "_Geo.json";		    
			FileUtils.save(Paths.get(directory, outputFile).toAbsolutePath().toString(), gson.toJson(geoOutput));
		}
		long stopTime = System.currentTimeMillis();
	    long elapsedTime = stopTime - startTime;
	    System.out.println("[Time Elapsed: " + elapsedTime/1000.0 + " Sec.]");
	}
	
	//This method will create the imr object using reflection
	public AttenuationRelationship CreateIMRInstance(String type)
	{
		//These classes will be instantiated using reflection
		try 
		{	
			String imrClassName = imrMap.get(type);
			Class listenerClass = ParameterChangeWarningListener.class;
			Object[] paramObjects = new Object[] {this};
			Class[] params = new Class[] {listenerClass};
			Class imrClass = Class.forName(imrClassName);
			Constructor ctor = imrClass.getConstructor(params);
			AttenuationRelationship imr = (AttenuationRelationship) ctor.newInstance(paramObjects);
			
			//setting the Attenuation with the default parameters
			imr.setParamDefaults();
			return imr;
		}
		catch (ClassCastException e) {
			e.printStackTrace();
		}
		catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
		catch (InvocationTargetException e) {
			e.printStackTrace();
		}
		catch (IllegalAccessException e) {
			e.printStackTrace();
		}
		catch (InstantiationException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	private String getDataSource(String paramName, ArrayList<SiteDataValue<?>> siteData) 
	{
		SiteDataTypeParameterNameMap typeMap = SiteTranslator.DATA_TYPE_PARAM_NAME_MAP;
		for(String dataType: typeMap.getTypesForParameterName(paramName))
		{
			if(dataType == SiteData.TYPE_VS30)
			{					
				for(SiteDataValue<?> dataValue:siteData)
				{
					if(dataValue.getDataType() != dataType)
						continue;
						
					Double vs30 = (Double)dataValue.getValue();
					if(vs30!= null && !vs30.isNaN() && vs30 > 0.0)
						return dataValue.getSourceName();				
				}
			}
			else if(dataType == SiteData.TYPE_DEPTH_TO_1_0 || dataType == SiteData.TYPE_DEPTH_TO_2_5)
			{
				for(SiteDataValue<?> dataValue:siteData)
				{
					if(dataValue.getDataType() != dataType)
						continue;
					
					Double depth = (Double)dataValue.getValue();
					if(depth!= null && !depth.isNaN())
						return dataValue.getSourceName();				
				}
			}
		}
		return "Provider not detected!";
	}
	
	EqkRupture getEqRuptureFromERF(EqRuptureConfig eqRupConfig)
	{
		ERF erf = null;
		switch (eqRupConfig.RuptureForecast()) {
		case "WGCEP (2007) UCERF2 - Single Branch":
			erf = new MeanUCERF2();
			erf.getAdjustableParameterList().getParameter(
					UCERF2.PROB_MODEL_PARAM_NAME).setValue(UCERF2.PROB_MODEL_POISSON);
			break;

		case "USGS/CGS 2002 Adj. Cal. ERF":
			erf = new Frankel02_AdjustableEqkRupForecast();
			break;
			
		case "WGCEP UCERF 1.0 (2005)":
			erf = new WGCEP_UCERF1_EqkRupForecast();
			erf.getAdjustableParameterList().getParameter(
					WGCEP_UCERF1_EqkRupForecast.TIME_DEPENDENT_PARAM_NAME).setValue(new Boolean(false));
			break;
			
		case "GEM1 CEUS ERF":
			erf = new GEM1_CEUS_ERF();
			break;
			
		case "Mean UCERF3":
			MeanUCERF3 forecast = new MeanUCERF3();
			forecast.setPreset(MeanUCERF3.Presets.BOTH_FM_BRANCH_AVG);
			forecast.setParameter(ProbabilityModelParam.NAME, ProbabilityModelOptions.POISSON);
			erf = forecast;
			break;
			
		case "Mean UCERF3 FM3.1":
			MeanUCERF3 forecast31 = new MeanUCERF3();
			forecast31.setPreset(MeanUCERF3.Presets.FM3_1_BRANCH_AVG);
			forecast31.setParameter(ProbabilityModelParam.NAME, ProbabilityModelOptions.POISSON);
			erf = forecast31;
			break;
			
		case "Mean UCERF3 FM3.2":
			MeanUCERF3 forecast32 = new MeanUCERF3();
			forecast32.setPreset(MeanUCERF3.Presets.FM3_2_BRANCH_AVG);
			forecast32.setParameter(ProbabilityModelParam.NAME, ProbabilityModelOptions.POISSON);
			erf = forecast32;
			break;
			
		default:
			break;
		}
		
		erf.getTimeSpan().setDuration(1.0);
		
		if(null != erf)
		{
			erf.updateForecast();
			EqkSource eqSource = erf.getSource(eqRupConfig.SourceIndex());
			eqSource.getName();
			if(null != eqSource)
				return eqSource.getRupture(eqRupConfig.RuptureIndex());
		}
		return null;//We failed to find the earthquake rupture
	}	
}


