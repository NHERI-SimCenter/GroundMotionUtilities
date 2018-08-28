/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class GridSite 
{
	@SerializedName("Latitude")
	private Division m_Latitude;
	
	@SerializedName("Longitude")
	private Division m_Longitude;
	
	public GridSite(Division latitude, Division longitude)
	{
		this.m_Latitude = latitude;
		this.m_Longitude = longitude;
	}
	
	public Division Latitude()
	{
		return this.m_Latitude;
	}
	
	public Division Longitude()
	{
		return this.m_Longitude;
	}
}
