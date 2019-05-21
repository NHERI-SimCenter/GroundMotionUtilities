package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

public class SAResult
{
	@SerializedName("Mean")
	private double[] m_Mean;
	
	@SerializedName("TotalStdDev")
	private double[] m_TotalStdDev;
	
	@SerializedName("InterEvStdDev")
	private double[] m_InterEvStdDev;
	
	@SerializedName("IntraEvStdDev")
	private double[] m_IntraEvStdDev;

	@SerializedName("UHS")
	private double[] m_UHS;
	
	SAResult()
	{
		
	}
	
	
	SAResult(int n, boolean hasIEStats)
	{
		this.m_Mean = new double[n];
		this.m_TotalStdDev = new double[n];
		if(hasIEStats)
		{
			this.m_InterEvStdDev = new double[n];
			this.m_IntraEvStdDev = new double[n];
		}
	}
	
	public void SetResult(int index, double mean, double stdDev)
	{
		this.m_Mean[index] = mean;
		this.m_TotalStdDev[index] = stdDev;
	}
	
	public void SetResult(int index, double mean, double stdDev, double interEvStdDev, double intraEvStdDev)
	{
		SetResult(index, mean, stdDev);
		this.m_InterEvStdDev[index] = interEvStdDev;
		this.m_IntraEvStdDev[index] = intraEvStdDev;
	}
	
	public void SetUHS(double[] UHS)
	{
		m_UHS = UHS;
	}
	
	public double Size()
	{
		return m_Mean.length;
	}
	
	public double Mean(int index)
	{
		return m_Mean[index];
	}
}