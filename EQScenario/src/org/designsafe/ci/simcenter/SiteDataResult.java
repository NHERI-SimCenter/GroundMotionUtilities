package org.designsafe.ci.simcenter;

import org.opensha.commons.data.siteData.SiteDataValue;

import com.google.gson.annotations.SerializedName;

public class SiteDataResult
{
	@SerializedName("Type")
	String m_Type;
	
	@SerializedName("Value")
	Object m_Value;
	
	@SerializedName("Source")
	String m_Source;
	
	SiteDataResult(SiteDataValue value)
	{
		this.m_Type = value.getDataType();
		this.m_Value = value.getValue();
		this.m_Source = value.getSourceName();
	}
	
	SiteDataResult(String type, Object value, String source)
	{
		this.m_Type = type;
		this.m_Value = value;
		this.m_Source = source;
	}
	
	public String Type()
	{
		return this.m_Type;		
	}
	
	public Object Value()
	{
		return this.m_Value;		
	}
	
	public String Source()
	{
		return this.m_Source;		
	}
}