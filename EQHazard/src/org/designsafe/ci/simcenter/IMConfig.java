/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author el7ad
 *
 */
public class IMConfig
{
	@SerializedName("Type")
	private String m_Type;
	
	@SerializedName("Periods")
	private double[] m_Periods;
	
	@SerializedName("EnableJsonOutput")
	private boolean	m_hasJsonOutput = true;
	
	@SerializedName("EnableCsvOutput")
	private boolean m_hasCsvOutput = false;
	
	@SerializedName("EnableGeoJsonOutput")
	private boolean m_hasGeoJsonOutput = true;
	
	@SerializedName("ExceedenceProbability")
	private Double m_exceedanceProb = null;
	
	public IMConfig(String type, double[] periods)
	{
		this.m_Periods = periods;
	}
	
	public String Type()
	{
		return this.m_Type;
	}
	
	public boolean hasJsonOutput()
	{
		return this.m_hasJsonOutput;
	}
	
	public boolean hasGeoJsonOutput()
	{
		return this.m_hasGeoJsonOutput;
	}
	
	public boolean hasCsvOutput()
	{
		return this.m_hasCsvOutput;
	}
	
	public double[] Periods()
	{
		return this.m_Periods;
	}
	
	public Boolean hasPeriods()
	{
		return m_Periods != null && m_Periods.length > 0;
	}
	
	public void setPeriods(double[] periods)
	{
		m_Periods = periods;
	}
	
	public Double getExceedanceProb()
	{
		return m_exceedanceProb;
	}
}
