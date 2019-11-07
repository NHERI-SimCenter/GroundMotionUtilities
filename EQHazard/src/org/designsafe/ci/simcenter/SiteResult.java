package org.designsafe.ci.simcenter;

import java.util.ArrayList;

import com.google.gson.annotations.SerializedName;

public class SiteResult
{
	@SerializedName("Location")
	SiteLocation m_Location;
	
	@SerializedName("SiteData")
	ArrayList<SiteDataResult> m_SiteData;
	
	@SerializedName("SA")
	SAResult	m_SA = null;
	
	@SerializedName("PGA")
	PGAResult	m_PGA = null;
	
	public SiteResult(SiteLocation location, ArrayList<SiteDataResult> siteData, SAResult result)
	{
		this.m_Location = location;
		this.m_SiteData = siteData;
		this.m_SA = result;
	}
	
	public SiteResult(SiteLocation location, ArrayList<SiteDataResult> siteData, PGAResult result)
	{
		this.m_Location = location;
		this.m_SiteData = siteData;
		this.m_PGA = result;
	}
	
	public SiteResult(SiteLocation location, ArrayList<SiteDataResult> siteData, PGAResult pgaResult, SAResult saResult)
	{
		this.m_Location = location;
		this.m_SiteData = siteData;
		this.m_PGA = pgaResult;
		this.m_SA = saResult;
	}
	
	public SiteLocation Location()
	{
		return this.m_Location;
	}
	
	public SAResult SA()
	{
		return this.m_SA;
	}
	
	public PGAResult PGA()
	{
		return this.m_PGA;
	}
	
	public ArrayList<SiteDataResult> SiteData()
	{
		return this.m_SiteData;
	}
}