/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author el7ad
 *
 */
public class SiteSpec {
	@SerializedName("Location")
	private SiteLocation m_Location = null;
	
	@SerializedName("Vs30")
	private Double m_Vs30 = null;
	
	public SiteSpec(SiteLocation location)
	{
		this.m_Location = new SiteLocation(location.Latitude(), location.Longitude());
	}
	
	public SiteSpec(double latitude, double longitude)
	{
		this.m_Location = new SiteLocation(latitude, longitude);
	}
	
	public Boolean hasVs30()
	{
		if (null == m_Vs30)
			return false;
		return true;
	}
	
	public SiteLocation Location()
	{
		return this.m_Location;
	}
	
	public Double Vs30()
	{
		return this.m_Vs30;
	}

}
