package org.designsafe.ci.simcenter;

import java.util.ArrayList;

import com.google.gson.annotations.SerializedName;

public class HazardCurvesResult 
{
	
	@SerializedName("Probability")
	private Double[] m_probability;
	
	@SerializedName("IML")
	private Double[] m_IML;
	
	@SerializedName("IM")
	private String m_IM;
	
	@SerializedName("period")
	private Double m_period = null;
	
	void setIM(String IM)
	{
		m_IM = IM;
	}
	
	void setPeriod(double period)
	{
		m_period = period;
		
	}
	
	void setCurve(Double[] imls, Double[] probabilities)
	{
		m_IML = imls;
		m_probability = probabilities;
	}
}
