/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class SiteLocation 
{
	@SerializedName("Latitude")
	private double m_Latitude;
	
	@SerializedName("Longitude")
	private double m_Longitude;
	
	SiteLocation(double latitude, double longitude)
	{
		this.m_Latitude = latitude;
		this.m_Longitude = longitude;
	}
	
	public double Latitude()
	{
		return this.m_Latitude;
	}
	
	public double Longitude()
	{
		return this.m_Longitude;
	}
}
