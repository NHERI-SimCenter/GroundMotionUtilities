package org.designsafe.ci.simcenter;

import java.awt.geom.Point2D;
import java.io.*;
import java.lang.reflect.*;
import java.nio.file.Paths;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

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
import org.opensha.refFaultParamDb.vo.FaultSectionPrefData;
import org.opensha.sha.earthquake.*;
import org.opensha.sha.earthquake.param.*;
import org.opensha.sha.earthquake.rupForecastImpl.Frankel02.*;
import org.opensha.sha.earthquake.rupForecastImpl.GEM1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF1.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.*;
import org.opensha.sha.earthquake.rupForecastImpl.WGCEP_UCERF_2_Final.MeanUCERF2.*;
import org.opensha.sha.faultSurface.*;
import org.opensha.sha.faultSurface.cache.SurfaceCachingPolicy;
import org.opensha.sha.gui.infoTools.IMT_Info;
import org.opensha.sha.imr.*;
import org.opensha.sha.imr.attenRelImpl.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.*;
import org.opensha.sha.imr.attenRelImpl.ngaw2.NGAW2_Wrappers.*;
import org.opensha.sha.imr.param.EqkRuptureParams.DipParam;
import org.opensha.sha.imr.param.EqkRuptureParams.MagParam;
import org.opensha.sha.imr.param.EqkRuptureParams.RakeParam;
import org.opensha.sha.imr.param.EqkRuptureParams.RupTopDepthParam;
import org.opensha.sha.imr.param.EqkRuptureParams.RupWidthParam;
import org.opensha.sha.imr.param.IntensityMeasureParams.*;
import org.opensha.sha.imr.param.OtherParams.*;
import org.opensha.sha.imr.param.PropagationEffectParams.DistanceJBParameter;
import org.opensha.sha.imr.param.PropagationEffectParams.DistanceRupParameter;
import org.opensha.sha.imr.param.SiteParams.Vs30_Param;
import org.opensha.sha.util.*;
import org.opensha.sha.calc.*;

import com.google.common.base.*;
import com.google.common.io.*;
import com.google.gson.*;

import scratch.UCERF3.FaultSystemRupSet;
import scratch.UCERF3.FaultSystemSolution;
import scratch.UCERF3.erf.mean.MeanUCERF3;
import scratch.kevin.ucerf3.QuadToEvenlyGridded;

public class RegionalProcessor implements ParameterChangeWarningListener
{
	ERF[] erfs;
	EQHazardCalc calc;
	ProbEqkRupture[] currentRupture = null;
	NGAWestAverage[] attenRels;
	double[] means = null;
	double[] intraEventStdDevs = null;
	double[] interEventStdDevs = null;
	double[] distanceJB = null;
	double[] distanceToRup = null;
	Location[] locations = null;
	double maxDistance = 50.0;

	double[] Vs30;
	int numThreads = 1;
	RuptureSurface lastDistanceSurface = null;
	
	public RegionalProcessor(int numThreads)
	{
		System.setProperty(SurfaceCachingPolicy.FORCE_TYPE, SurfaceCachingPolicy.CacheTypes.SINGLE.toString());

		if (numThreads < 1)
			this.numThreads = 1;
		
		this.numThreads = numThreads;
		calc = new EQHazardCalc();
		initERF();
		initGMM();
	}
	
	public RegionalProcessor()
	{
		this(1);
	}
	
	public void initERF()
	{		
		erfs = new ERF[numThreads];
		currentRupture = new ProbEqkRupture[numThreads];
		
		List<Future> futures = new ArrayList<Future>();
		ExecutorService executor = Executors.newFixedThreadPool(numThreads);

		int start = 0;
		for (int t = 0 ; t < numThreads; t++)
		{
			final int i = t;
			Runnable runnableTask = () -> {
				erfs[i] = calc.getERF("Mean UCERF3 FM3.1", false);
				erfs[i].setParameter("Fault Grid Spacing", 10.0);
				erfs[i].updateForecast();
				ProbEqkSource rupSource = erfs[i].getSource(0);		
				currentRupture[i] = rupSource.getRupture(0);
			};

			futures.add(executor.submit(runnableTask));
		}
		
		for (Future f : futures) {
			try 
			{
				f.get();			
			}
			catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
		}
	}
	
	public void initGMM()
	{
		attenRels = new NGAWestAverage[numThreads];
		for (int i = 0 ; i < numThreads; i++)
		{
		    attenRels[i] = new NGAWestAverage(calc);
		    attenRels[i].setParamDefaults();
		    attenRels[i].setIntensityMeasure("PGA");
		}
	}
	
	public void setIM(String IM)
	{
		for (int i = 0 ; i < numThreads; i++)
		    attenRels[i].setIntensityMeasure(IM);
	}
	
	public void setCurrentRupture(int source, int rupture)
	{
		if (currentRupture == null)
			currentRupture = new ProbEqkRupture[numThreads];
		
		for (int i = 0 ; i < numThreads; i++)
		{
			ProbEqkSource rupSource = erfs[i].getSource(source);		
			currentRupture[i] = rupSource.getRupture(rupture);
		}
		return;
	}
	
	public void setLocations(Location[] locations)
	{
		this.locations = locations;
	}
	
	public void obtainVs30()
	{		
		System.out.print("Obtaining Site Data...");
		long sdStartTime = System.currentTimeMillis();
		//process site data
		Vs30 = new double[locations.length];		

		List<Future> futures = new ArrayList<Future>();
		ExecutorService executor = Executors.newFixedThreadPool(numThreads);

		int start = 0;
		for (int t = 0 ; t < numThreads; t++)
		{
			int end = (t+1) * (int)Math.ceil(locations.length/(double)numThreads);

			if (end > locations.length)
				end = locations.length;
			
			final int startt = start;
			final int endt = end;
			final int currentThread = t;

			Runnable runnableTask = () -> {
				LocationList locationsList = new LocationList();	
				
				for(int i=startt; i < endt; i++)
				{
					locationsList.add(locations[i]);
				}
				ArrayList<SiteDataValueList<?>> availableSiteData;
				OrderedSiteDataProviderList siteDataProviders = OrderedSiteDataProviderList.createSiteDataProviderDefaults();
				try {
					availableSiteData = siteDataProviders.getAllAvailableData(locationsList);
				} catch (IOException e) {
					e.printStackTrace();
					return;
				}
				
				for(SiteDataValueList<?> providerData:availableSiteData)
					if (providerData.getType() == SiteData.TYPE_VS30)
					{
						ArrayList values = providerData.getValues();
						for (int j=0; j < values.size(); j++)
							Vs30[startt+j] = (double)values.get(j);
					}
			};

			futures.add(executor.submit(runnableTask));
			start = end;
		}
		
		for (Future f : futures) {
			try 
			{
				f.get();			
			}
			catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
		}
		
		long sdStopTime = System.currentTimeMillis();
	    long sdElapsedTime = sdStopTime - sdStartTime;
	    System.out.println("[Time Elapsed: " + sdElapsedTime/1000.0 + " Sec.]");
	}
	
	public double[] getVs30()
	{
		return Vs30;
	}
	
	public void setVs30(double[] Vs30)
	{
		this.Vs30 = Vs30;
	}
	
	public void calculateDistances()
	{
		if (lastDistanceSurface == currentRupture[0].getRuptureSurface())
			return;
		
		lastDistanceSurface = currentRupture[0].getRuptureSurface();
		
		distanceToRup = new double[locations.length];
		distanceJB = new double[locations.length];		
		
		List<Future> futures = new ArrayList<Future>();
		ExecutorService executor = Executors.newFixedThreadPool(numThreads);

		int start = 0;
		for (int t = 0 ; t < numThreads; t++)
		{
			RuptureSurface surface = currentRupture[t].getRuptureSurface();

			int end = (t+1) * (int)Math.ceil(locations.length/(double)numThreads);

			if (end > locations.length)
				end = locations.length;
			
			final int startt = start;
			final int endt = end;
			Runnable runnableTask = () -> {
	    		for (int i=startt; i < endt; i++)
				{
					distanceToRup[i] = surface.getDistanceRup(locations[i]);
					distanceJB[i] = surface.getDistanceJB(locations[i]);
				}
			};

			futures.add(executor.submit(runnableTask));
			start = end;
		}
		
		for (Future f : futures) {
			try 
			{
				f.get();			
			}
			catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
		}	

	}
	
	public ProbEqkRupture getRupture()
	{
		return currentRupture[0];
	}
	
	public void calculateIMs()
	{
		if (means == null) 
			means = new double[locations.length];
		
		if (intraEventStdDevs == null)
			intraEventStdDevs = new double[locations.length];
		
		if (interEventStdDevs == null)
			interEventStdDevs = new double[locations.length];
		
		List<Future> futures = new ArrayList<Future>();
		ExecutorService executor = Executors.newFixedThreadPool(numThreads);

		int start = 0;
		for (int t = 0 ; t < numThreads; t++)
		{
			RuptureSurface surface = currentRupture[t].getRuptureSurface();

			int end = (t+1) * (int)Math.ceil(locations.length/(double)numThreads);

			if (end > locations.length)
				end = locations.length;
			
			final int startt = start;
			final int endt = end;
			final int currentThread = t;
			Runnable runnableTask = () -> {
	    		for (int i=startt; i < endt; i++)
    			{
    				double distanceToRupi = distanceToRup[i];
    				double distanceJBi = distanceJB[i];
    				if (distanceToRupi > maxDistance || distanceJBi > maxDistance)
    				{
    					means[i] = -10.0;
    					interEventStdDevs[i] = 0.0;
    					intraEventStdDevs[i] = 0.0;		
    					continue;		
    				}
    				
					
    				attenRels[currentThread].setParameter(DistanceRupParameter.NAME, distanceToRupi);
//    				Parameter distanceToRupParam = attenRels[currentThread].getParameter(DistanceRupParameter.NAME);
//    				distanceToRupParam.setValue(distanceToRupi);
    				
    				attenRels[currentThread].setParameter(DistanceJBParameter.NAME, distanceJBi);
//    				Parameter distanceJBParam = attenRels[currentThread].getParameter(DistanceJBParameter.NAME);
//    				distanceJBParam.setValue(distanceJBi);
    				
    				attenRels[currentThread].setParameter(Vs30_Param.NAME, Vs30[i]);
//    				Parameter vs30Param = attenRels[currentThread].getParameter(Vs30_Param.NAME);
//    				vs30Param.setValue(Vs30[i]);

    				attenRels[currentThread].setParameter(MagParam.NAME, currentRupture[0].getMag());
//    				Parameter magParam = attenRels[currentThread].getParameter(MagParam.NAME);
//    				magParam.setValue(currentRupture[0].getMag());

    				attenRels[currentThread].setParameter(DipParam.NAME, currentRupture[0].getRuptureSurface().getAveDip());
//    				Parameter dipParam = attenRels[currentThread].getParameter(DipParam.NAME);
//    				dipParam.setValue(currentRupture[0].getRuptureSurface().getAveDip());

    				attenRels[currentThread].setParameter(RakeParam.NAME, currentRupture[0].getAveRake());
//    				Parameter rakeParam = attenRels[currentThread].getParameter(RakeParam.NAME);
//    				rakeParam.setValue(currentRupture[0].getAveRake());
   				
    				attenRels[currentThread].setParameter(RupTopDepthParam.NAME, currentRupture[0].getRuptureSurface().getAveRupTopDepth());
//    				Parameter topDepthParam = attenRels[currentThread].getParameter(RupTopDepthParam.NAME);
//    				topDepthParam.setValue(currentRupture[0].getRuptureSurface().getAveRupTopDepth());
    				
    				attenRels[currentThread].setParameter(RupWidthParam.NAME, currentRupture[0].getRuptureSurface().getAveWidth());
//    				Parameter widthParam = attenRels[currentThread].getParameter(RupWidthParam.NAME);
//    				widthParam.setValue(currentRupture[0].getRuptureSurface().getAveWidth());

    				means[i] = attenRels[currentThread].getMean();
    				
    				attenRels[currentThread].setParameter(StdDevTypeParam.NAME, StdDevTypeParam.STD_DEV_TYPE_INTRA);    				
    				intraEventStdDevs[i] = attenRels[currentThread].getStdDev();
    				
    				attenRels[currentThread].setParameter(StdDevTypeParam.NAME, StdDevTypeParam.STD_DEV_TYPE_INTER);    				
    				interEventStdDevs[i] = attenRels[currentThread].getStdDev();
    			}
			};

			futures.add(executor.submit(runnableTask));
			start = end;
		}
		
		for (Future f : futures) {
			try 
			{
				f.get();			
			}
			catch(Exception e)
			{
				System.out.println(e.getMessage());
			}
		}	

		return;
	}
	
	public void setMaxDistance(double maxDistance)
	{		
		for (int t = 0 ; t < numThreads; t++)	
			attenRels[t].setUserMaxDistance(maxDistance);
		this.maxDistance = maxDistance;
	}
	
	public double[] getDistanceToRup()
	{
		return distanceToRup;
	}
	
	public double[] getDistanceJB()
	{
		return distanceJB;
	}
	
	
	public double[] getMeans()
	{
		return means;
	}
	
	public double[] getInterEvStdDevs()
	{
		return interEventStdDevs;
	}
	
	public double[] getIntraEvStdDevs()
	{
		return intraEventStdDevs;
	}
	
	public int getNumSources()
	{
		return erfs[0].getNumSources();
	}
	
	public int getNumRuptures(int source)
	{
		return erfs[0].getNumRuptures(source);
	}
	
	public static void main(String[] args) throws Exception {
		

		
	}
	
	@Override
	public void parameterChangeWarning(ParameterChangeWarningEvent arg0) {
		// TODO Auto-generated method stub
		
	}

}

