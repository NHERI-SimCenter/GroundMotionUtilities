/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author WaelElhaddad
 *
 */
public class SHAOutput
{
	@SerializedName("EqRupture")
	private EqRuptureConfig m_EqRupture;
	
	@SerializedName("Periods")
	private double[] m_Periods;
	
	@SerializedName("GroundMotions")
	private SiteResult[] m_Results;
	
	public SHAOutput(int size, double[] periods, EqRuptureConfig eqRupture)
	{
		this.m_Results = new SiteResult[size];
		this.m_Periods = periods;
		this.m_EqRupture = eqRupture;
	}
	
	public SiteResult[] Results()
	{
		return m_Results;
	}
	
	public SiteResult Result(int index)
	{
		return m_Results[index];
	}
	
	double[] Periods()
	{
		return this.m_Periods;
	}
	
	public void SetResult(int index, SiteResult result)
	{
		this.m_Results[index] = result;
	}
	
	public SiteResult GetResult(int index)
	{
		return this.m_Results[index];
	}
}
