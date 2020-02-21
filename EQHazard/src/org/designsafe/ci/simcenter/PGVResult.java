package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

public class PGVResult
{
	@SerializedName("Mean")
	private double m_Mean = 0.0;
	
	@SerializedName("TotalStdDev")
	private double m_TotalStdDev = 0.0;
	
	@SerializedName("InterEvStdDev")
	private Double m_InterEvStdDev = null;
	
	@SerializedName("IntraEvStdDev")
	private Double m_IntraEvStdDev = null;

	PGVResult(double mean, double stdDev)
	{
		this.m_Mean = mean;
		this.m_TotalStdDev = stdDev;
	}
	
	PGVResult(double mean, double stdDev, double interEvStdDev , double intraEvStdDev)
	{
		this(mean, stdDev);
		this.m_InterEvStdDev = interEvStdDev;
		this.m_IntraEvStdDev = intraEvStdDev;
	}
	
	public double Mean()
	{
		return this.m_Mean;
	}
}