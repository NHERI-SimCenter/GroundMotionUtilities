package org.designsafe.ci.simcenter;

import java.util.HashMap;

import com.google.gson.annotations.SerializedName;

public class GeoJsonFeature
{
	@SerializedName("Type")
	private final String m_Type = "Feature";
	
	@SerializedName("Geometry")
	private GeoJsonGeometry m_Geometry;
	
	@SerializedName("Properties")
	private HashMap<String, Object> m_Properties = new HashMap<String, Object>();
	
	public GeoJsonFeature(GeoJsonGeometry geometry)
	{
		this.m_Geometry = geometry;
	}
	
	public void AddProperty(String key, Object value)
	{
		this.m_Properties.put(key, value);
	}
}