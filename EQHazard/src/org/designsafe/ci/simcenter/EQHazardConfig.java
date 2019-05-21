/**
 * 
 */
package org.designsafe.ci.simcenter;


import com.google.gson.annotations.SerializedName;

enum ConfigType 
{ 
    Scenario, UHS, ERFEXPORTER
} 

/**
 * @author Wael Elhaddad
 *
 */
public class EQHazardConfig {
	
	@SerializedName("Site")
	private SiteConfig m_Site;
	
	@SerializedName("EqRupture")
	private EqRuptureConfig m_EqRupture;
	
	@SerializedName("GMPE")
	private GMPEConfig m_GMPE;
	
	@SerializedName("IntensityMeasure")
	private IMConfig m_IntensityMeasure;
	
	public EQHazardConfig(SiteConfig site, EqRuptureConfig rupture, GMPEConfig gmpe, IMConfig im)
	{
		this.m_Site = site;
		this.m_EqRupture = rupture;
		this.m_GMPE = gmpe;
		this.m_IntensityMeasure = im;
	}
	
	public SiteConfig GetSiteConfig()
	{
		return this.m_Site;
	}
	
	public EqRuptureConfig GetRuptureConfig()
	{
		return this.m_EqRupture;
	}
	
	public GMPEConfig GetGMPEConfig()
	{
		return this.m_GMPE;
	}
	
	public IMConfig GetIMConfig()
	{
		return this.m_IntensityMeasure;
	}
	
	public ConfigType getType()
	{
		if(this.m_IntensityMeasure == null && this.m_EqRupture.Type().equalsIgnoreCase("ERF"))
			return ConfigType.ERFEXPORTER;
			
		if(this.m_IntensityMeasure.Type().equalsIgnoreCase("UHS"))
			return ConfigType.UHS;
				
		return ConfigType.Scenario;
		
	}
}
