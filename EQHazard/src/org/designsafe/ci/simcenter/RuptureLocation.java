/**
 * 
 */
package org.designsafe.ci.simcenter;

import org.opensha.commons.geo.Location;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class RuptureLocation 
{
	@SerializedName("Latitude")
	double	m_Latitude;
	
	@SerializedName("Longitude")
	double 	m_Longitude;
	
	@SerializedName("Depth")
	double 	m_Depth = 0.0;
	
	RuptureLocation(Location location)
	{
		this.m_Latitude = location.getLatitude();
		this.m_Longitude = location.getLongitude();
		this.m_Depth = location.getDepth();			
	}
	
	RuptureLocation(double latitude, double longitude)
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
	
	public double Depth()
	{
		return this.m_Depth;
	}
}
