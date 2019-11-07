package org.designsafe.ci.simcenter;

import java.io.*;
import java.lang.reflect.*;
import java.nio.file.Paths;
import java.util.*;

import javax.swing.JOptionPane;

import org.apache.commons.lang3.ArrayUtils;
import org.opensha.commons.data.*;
import org.opensha.commons.data.siteData.*;
import org.opensha.commons.exceptions.ParameterException;
import org.opensha.commons.data.function.*;
import org.opensha.commons.geo.*;
import org.opensha.commons.param.*;
import org.opensha.commons.param.Parameter;
import org.opensha.commons.param.constraint.ParameterConstraint;
import org.opensha.commons.param.event.*;
import org.opensha.commons.util.*;
import org.opensha.nshmp2.imr.NSHMP08_SUB_Slab;
import org.opensha.nshmp2.imr.impl.AB2003_AttenRel;
import org.opensha.nshmp2.imr.impl.YoungsEtAl_1997_AttenRel;
import org.opensha.sha.earthquake.*;
import org.opensha.sha.earthquake.param.*;
import org.opensha.sha.earthquake.rupForecastImpl.Frankel02.*;
import org.opensha.sha.earthquake.rupForecastImpl.GEM1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.MeanUCERF2.*;
import org.opensha.sha.faultSurface.*;
import org.opensha.sha.gui.infoTools.IMT_Info;
import org.opensha.sha.imr.*;
import org.opensha.sha.imr.attenRelImpl.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.NGAW2_Wrappers.*;
import org.opensha.sha.imr.param.IntensityMeasureParams.*;
import org.opensha.sha.imr.param.OtherParams.*;
import org.opensha.sha.imr.param.SiteParams.Vs30_Param;
import org.opensha.sha.util.*;
import org.opensha.sha.calc.*;

import com.google.common.base.*;
import com.google.common.io.*;
import com.google.gson.*;

import scratch.UCERF3.erf.mean.MeanUCERF3;

public class EQHazardCalc implements ParameterChangeWarningListener {
	private HashMap<String, String> imrMap;
	private SHAOutput output;
	private OrderedSiteDataProviderList siteDataProviders;
	public static final double dTol = 1e-3;
	
	public EQHazardCalc()
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
		imrMap.put(AB2003_AttenRel.NAME, AB2003_AttenRel.class.getName());
		imrMap.put(YoungsEtAl_1997_AttenRel.NAME, YoungsEtAl_1997_AttenRel.class.getName());
		imrMap.put("Zhao Et Al. (2006) - Intraslab", ZhaoEtAl_2006_AttenRel.class.getName());

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
	

		EQHazardCalc calc = new EQHazardCalc();
		String jsonCfgPath = args[0];
		File cfgFile = new File(jsonCfgPath);
		
		String cfg = "";
		try {
			cfg = Files.toString(cfgFile, Charsets.UTF_8);
		} catch (IOException e) {
			System.out.print(e.getMessage());
			System.exit(-1);
		}
		
		Gson gson = new GsonBuilder().create();
		EQHazardConfig config = gson.fromJson(cfg, EQHazardConfig.class);
		try {
				
			if(config.getType() == ConfigType.Scenario)
				calc.PerformSHA(config);
			else if(config.getType() == ConfigType.UHS)
				calc.ComputeUHS(config);
			else if(config.getType() == ConfigType.ERFEXPORTER)
			{
				SiteConfig siteConfig = config.GetSiteConfig();
				if (!siteConfig.Type().equalsIgnoreCase("SingleLocation"))
				{
					System.err.print("Only single location sites are supported for ERF export!");
					System.exit(-10);
				}
				EqRuptureConfig rupconfig = config.GetRuptureConfig();
				ERF erf = calc.getERF(rupconfig.RuptureForecast());
				SiteLocation siteLocation = siteConfig.Location();
				Location location = new Location(siteLocation.Latitude(), siteLocation.Longitude());
				ERFExporter.ExportToGeoJson(erf, args[1], rupconfig.MaxDistance(), location, rupconfig.MaxSources());
				
				System.exit(0);
			}
		}
		catch (Exception e) {
			System.err.print(e.getMessage());
			System.exit(-2);
		}
		
		String outFilePath = args[1];
		File outFile = new File(outFilePath);
		String directory = outFile.getAbsoluteFile().getParent();
		String file = outFile.getName();

		calc.WriteOutputs(config, directory, file);
	}

	@Override
	public void parameterChangeWarning(ParameterChangeWarningEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
	public SHAOutput GetOutput()
	{
		return output;		
	}
	
	public void PerformSHA(EQHazardConfig scenarioConfig)
	{
		GMPEConfig GMPECfg = scenarioConfig.GetGMPEConfig();
		ScalarIMR imr = CreateIMRInstance(GMPECfg.Type());
		ParameterList ims =imr.getSupportedIntensityMeasures();
		SA_Param saParam = (SA_Param)ims.getParameter(SA_Param.NAME);
		double[] supportedPeriods = saParam.getPeriodParam().getPeriods();
		Arrays.sort(supportedPeriods);
		
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
		List<SiteSpec> siteSpecs = new ArrayList<SiteSpec>();
		if(siteCfg.Type().equalsIgnoreCase("SingleLocation"))
		{
			siteSpecs.add(new SiteSpec(siteCfg.Location()));
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
					siteSpecs.add(new SiteSpec(latitude, longitude));
				}
			}
		}
		else if(siteCfg.Type().equalsIgnoreCase("SiteList"))
		{
			siteSpecs.addAll(siteCfg.SiteList());
		}
		else
		{
			System.out.print("Invalid Site Type: " + siteCfg.Type() + " !");
			return;
		}
				
		IMConfig imConfig = scenarioConfig.GetIMConfig();
		//check if periods are specified
		if(!imConfig.hasPeriods())
		{
			imConfig.setPeriods(supportedPeriods);
		}
		
		//First we need to find the type of IM
		boolean isSANeeded = false;
		boolean isPGANeeded = false;

		if(imConfig.Type().equalsIgnoreCase("SA"))
		{
			isSANeeded = true;
			isPGANeeded = false;

			output = new SHAOutput(siteSpecs.size(), imConfig.Periods(), scenarioConfig.GetRuptureConfig());
		}
		else if(imConfig.Type().equalsIgnoreCase("PGA"))
		{
			output = new SHAOutput(siteSpecs.size(), null, scenarioConfig.GetRuptureConfig());
			isSANeeded = false;
			isPGANeeded = true;
		}
		else if(imConfig.Type().equalsIgnoreCase("All"))
		{
			output = new SHAOutput(siteSpecs.size(), imConfig.Periods(), scenarioConfig.GetRuptureConfig());
			isSANeeded = true;
			isPGANeeded = true;
		}
		else
		{
			System.out.print("Invalid IM Type: " + imConfig.Type() + " !");
			return;
		}
		
		
		ArrayList<Site> sites = new ArrayList<Site>();
		for(SiteSpec siteSpec: siteSpecs)
		{
			Location location = new Location (siteSpec.Location().Latitude(), siteSpec.Location().Longitude());
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

		for(int i = 0; i < siteSpecs.size(); i++)
		{

			System.out.print("\rProcessing Site " + (i+1));
			SiteSpec siteSpec = siteSpecs.get(i);
			SiteLocation siteLocation = siteSpec.Location();
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

				if(newParam.getName().equalsIgnoreCase("Vs30") && siteSpec.hasVs30())
				{
					newParam.setValue(siteSpec.Vs30().doubleValue());
					siteDataResults.add(new SiteDataResult(newParam.getName(),
									newParam.getValue(), "User Defined"));
				}
				else if(newParam.getName().equalsIgnoreCase("Vs30 Type") && siteSpec.hasVs30())
				{
					newParam.setValue("Measured");
					siteDataResults.add(new SiteDataResult(newParam.getName(),
									newParam.getValue(), "User Defined"));
				}
				else if(siteDataFound)
				{
					String provider = "Unknown"; 

					provider = getDataSource(newParam.getName(), siteDataValues);
					
					siteDataResults.add(new SiteDataResult(newParam.getName(),
								newParam.getValue(), provider));
				}
				else if(newParam.getName().equalsIgnoreCase(ZhaoEtAl_2006_AttenRel.SITE_TYPE_NAME))
				{
					Vs30_Param vs30param = new Vs30_Param();
					boolean vs30Found = siteTrans.setParameterValue(vs30param, siteDataValues);
					if(vs30Found)
					{
						double vs30 = vs30param.getValue();
						if(vs30 > 1100)
							newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_HARD_ROCK);
						
						else if(vs30 > 600)
							newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_ROCK);
						
						else if(vs30 > 300)
							newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_HARD_SOIL);
						
						else if(vs30 > 200)
							newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_MEDIUM_SOIL);
						
						else
							newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_SOFT_SOIL);

					}
					else
						newParam.setValue(ZhaoEtAl_2006_AttenRel.SITE_TYPE_DEFAULT);
					
					String provider = "Unknown"; 

					provider = getDataSource(vs30param.getName(), siteDataValues);
					
					siteDataResults.add(new SiteDataResult(vs30param.getName(),
							vs30param.getValue(), provider));
					
					siteDataResults.add(new SiteDataResult(newParam.getName(),
							newParam.getValue(), "Mapped from Vs30"));
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
			
			boolean hasIEStats = false;
			StdDevTypeParam stdDevParam = null;
			try
			{
				stdDevParam = (StdDevTypeParam)imr.getParameter(StdDevTypeParam.NAME);
				
				hasIEStats = stdDevParam.isAllowed(StdDevTypeParam.STD_DEV_TYPE_INTER) &&
						stdDevParam.isAllowed(StdDevTypeParam.STD_DEV_TYPE_INTRA);
			}
			catch(ParameterException e)
			{
				hasIEStats = false;
			}
			double[] periods = imConfig.Periods();
			
			PGAResult pgaResult = null;
			SAResult saResult = null;
			if(isSANeeded)
			{	
				imr.setIntensityMeasure("SA");

				Parameter imtParam = (Parameter)imr.getIntensityMeasure();
				saResult = new SAResult(periods.length, hasIEStats);	

				for(int j=0; j < imConfig.Periods().length; j++)
				{
					imtParam.getIndependentParameter(PeriodParam.NAME).setValue(periods[j]);
					double mean = imr.getMean();
					if(null != stdDevParam)
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
			}

			if(isPGANeeded)
			{
				imr.setIntensityMeasure("PGA");

				double mean = imr.getMean();
				if(null != stdDevParam)
					stdDevParam.setValue(StdDevTypeParam.STD_DEV_TYPE_TOTAL);
				double stdDev = imr.getStdDev();					
				
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
			}
			
			if(siteDataValues.isEmpty())
				result = new SiteResult(siteLocation, null, pgaResult, saResult);
			else
				result = new SiteResult(siteLocation, siteDataResults, pgaResult, saResult);
			
			output.SetResult(i, result);
		}
		long stopTime = System.currentTimeMillis();
	    long elapsedTime = stopTime - startTime;
	    System.out.println(", Done! Time Elapsed: " + elapsedTime/1000.0 + " Sec.");
	}
	
	
	public void ComputeUHS(EQHazardConfig config)
	{
		GMPEConfig GMPECfg = config.GetGMPEConfig();
		ScalarIMR imr = CreateIMRInstance(GMPECfg.Type());
		ParameterList ims =imr.getSupportedIntensityMeasures();
		SA_Param saParam = (SA_Param)ims.getParameter(SA_Param.NAME);
		List<Double> supportedPeriods = saParam.getPeriodParam().getAllowedDoubles();
		Collections.sort(supportedPeriods);	


		SiteLocation siteLocation = config.GetSiteConfig().Location();
		Location location = new Location (siteLocation.Latitude(), siteLocation.Longitude());
		Site site = new Site(location);
		
		System.out.print("Obtaining Site Data...");
		long sdStartTime = System.currentTimeMillis();
		ArrayList<SiteDataValue<?>> availableSiteData = null;
		try {
			availableSiteData = siteDataProviders.getAllAvailableData(location);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
		long sdStopTime = System.currentTimeMillis();
	    long sdElapsedTime = sdStopTime - sdStartTime;
	    System.out.println("[Time Elapsed: " + sdElapsedTime/1000.0 + " Sec.]");
		
	    System.out.print("Processing Rupture Forecast...");
		long erfStartTime = System.currentTimeMillis();
		EqRuptureConfig eqRupCfg = config.GetRuptureConfig();
		ERF erf = getERF(eqRupCfg.RuptureForecast());
		ParameterList erfParams = erf.getAdjustableParameterList();
		erfParams.setValue("Background Seismicity", "Exclude");
		long erfStopTime = System.currentTimeMillis();
	    long erfElapsedTime = erfStopTime - erfStartTime;
		System.out.println("[Time Elapsed: " + erfElapsedTime/1000.0 + " Sec.]");
		
		ParameterList imrSiteParams = imr.getSiteParams();
		ArrayList<SiteDataResult> siteDataResults = new ArrayList<SiteDataResult>();
		SiteTranslator siteTrans = new SiteTranslator();
		
		SiteSpec siteSpec = new SiteSpec(siteLocation);
		for(Parameter siteParam:imrSiteParams)
		{	
			Parameter newParam = (Parameter)siteParam.clone();
			//checking if a provider has the value, otherwise set the default
			boolean siteDataFound = siteTrans.setParameterValue(newParam, availableSiteData);

			if(newParam.getName().equalsIgnoreCase("Vs30") && siteSpec.hasVs30())
			{
				newParam.setValue(siteSpec.Vs30().doubleValue());
				siteDataResults.add(new SiteDataResult(newParam.getName(),
								newParam.getValue(), "User Defined"));
			}
			else if(newParam.getName().equalsIgnoreCase("Vs30 Type") && siteSpec.hasVs30())
			{
				newParam.setValue("Measured");
				siteDataResults.add(new SiteDataResult(newParam.getName(),
								newParam.getValue(), "User Defined"));
			}
			else if(siteDataFound)
			{
				String provider = "Unknown";

				provider = getDataSource(newParam.getName(), availableSiteData);
				
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
		
		SpectrumCalculatorAPI spectrumCalc = new SpectrumCalculator();
		IMT_Info imtInfo = new IMT_Info();
		
		DiscretizedFunc hazFunction = imtInfo.getDefaultHazardCurve(SA_Param.NAME);
		imr.setIntensityMeasure("SA");
		double[] periods = new double [supportedPeriods.size()];
		for (int i = 0; i < supportedPeriods.size(); i++)
			periods[i] = supportedPeriods.get(i);
		output = new SHAOutput(1, periods, config.GetRuptureConfig());
		
		Thread calcThread = new Thread(new Runnable(){
			
			public void run()
			{
				DiscretizedFunc spectrum = null;

				try
				{
					spectrum = spectrumCalc.getIML_SpectrumCurve(hazFunction, site, imr, erf, 0.05, supportedPeriods);
				}
				catch (RuntimeException e) {
					e.printStackTrace();
					return;
				}
				catch(Exception e)
				{
					System.out.print(e.getMessage());
				}
				
				synchronized (output) {
					SAResult saResult = new SAResult();
					double[] UHS = new double [spectrum.size()];
					for (int i = 0; i < spectrum.size(); i++)
						UHS[i] = Math.log(spectrum.getY(i));
					saResult.SetUHS(UHS);
					SiteResult siteResult = new SiteResult(siteLocation, null, null, saResult);
					output.SetResult(0, siteResult);
				}
			}});
		
		calcThread.setName("PSHA-thread");
		calcThread.start();
		
		long shaStartTime = System.currentTimeMillis();
		while(calcThread.isAlive())
		{
			try
			{
				Thread.sleep(200);
			}
			catch (Exception e)
			{
				
			}
			int current = Math.max(0, spectrumCalc.getCurrRuptures());
			double percent = (double)current/spectrumCalc.getTotRuptures() * 100;
			System.out.printf("\rProcessing Rupture %d of %d [%.1f %% Done]", current, spectrumCalc.getTotRuptures(), percent);
		}
		long shaStopTime = System.currentTimeMillis();
	    long shaElapsedTime = shaStopTime - shaStartTime;
		System.out.println("...[Time Elapsed: " + shaElapsedTime/1000.0 + " Sec.]");		
	}
	
	private void WriteOutputs(EQHazardConfig scenarioConfig, String directory, String file)
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

			Class imrClass = Class.forName(imrClassName);
			
			Constructor ctor;
			AttenuationRelationship imr;
			
			if(!type.startsWith("NSHMP"))
			{
				Class listenerClass = ParameterChangeWarningListener.class;
				Object[] paramObjects = new Object[] {this};
				Class[] params = new Class[] {listenerClass};
				ctor = imrClass.getConstructor(params);
				imr = (AttenuationRelationship) ctor.newInstance(paramObjects);
			}
			else
			{
				ctor = imrClass.getConstructor();
				imr = (AttenuationRelationship) ctor.newInstance();
			}
			
			
			//setting the Attenuation with the default parameters
			imr.setParamDefaults();
			
			if(type.endsWith("Intraslab"))
				imr.getParameter(TectonicRegionTypeParam.NAME).setValue(TectonicRegionType.SUBDUCTION_SLAB.toString());

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
	
	ERF getERF(String erfName)
	{
		ERF erf = null;
		switch (erfName) {
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
			
		case "GEM1 WEUS ERF":
			erf = new GEM1_WEUS_ERF();
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
			return erf;
		}
		
		return erf;//We failed to find the earthquake rupture forecast
	}
	
	EqkRupture getEqRuptureFromERF(EqRuptureConfig eqRupConfig)
	{
		ERF erf = getERF(eqRupConfig.RuptureForecast());		
		
		if(null != erf)
		{
			EqkSource eqSource = erf.getSource(eqRupConfig.SourceIndex());
			eqSource.getName();
			if(null != eqSource)
				return eqSource.getRupture(eqRupConfig.RuptureIndex());
		}
		return null;//We failed to find the earthquake rupture
	}	
}


