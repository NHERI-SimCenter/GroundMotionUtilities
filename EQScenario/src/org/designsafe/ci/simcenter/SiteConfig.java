/**
 * 
 */
package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class SiteConfig 
{
	@SerializedName("Type")
	private String m_Type;
	
	@SerializedName("Grid")
	private GridSite m_Grid;
	
	@SerializedName("Location")
	private SiteLocation m_Location;
	
	public SiteConfig(SiteLocation location)
	{
		this.m_Type = "Grid";
		this.m_Location = location;
	}
	
	public SiteConfig(GridSite grid)
	{
		this.m_Type = "SingleLocation";
		this.m_Grid = grid;
	}
	
	public String Type()
	{
		return this.m_Type;
	}
	
	public SiteLocation Location()
	{
		return this.m_Location;
	}
	
	public GridSite Grid()
	{
		return this.m_Grid;
	}
}
