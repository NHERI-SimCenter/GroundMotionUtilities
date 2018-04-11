/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class Division 
{
	@SerializedName("Min")
	private double 	m_Min;
	
	@SerializedName("Max")
	private double	m_Max;
	
	@SerializedName("Divisions")
	private int		m_Divisions;
	
	public Division(double min, double max, int divisions)
	{
		this.m_Min = min;
		this.m_Max = max;
		this.m_Divisions = divisions;

		EnsureMinMax();
	}
	
	private void EnsureMinMax()
	{
		if(m_Max < m_Min)
		{
			double swap = m_Max;
			m_Max = m_Min;
			m_Min = swap;
		}
	}
	
	public double GetStep()
	{
		return (m_Max - m_Min)/m_Divisions;
	}
	
	public double Min()
	{
		return this.m_Min;
	}
	
	public double Max()
	{
		return this.m_Max;
	}
}
