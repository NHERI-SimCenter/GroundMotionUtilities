package org.designsafe.ci.simcenter;

import java.util.ArrayList;

import com.google.gson.annotations.SerializedName;

public class GeoJsonOutput
{
	@SerializedName("Type")
	private final String m_Type = "FeatureCollection";
	
	@SerializedName("Features")
	private ArrayList<GeoJsonFeature> m_Features = new ArrayList<GeoJsonFeature>();
	
	public void AddFeature(GeoJsonFeature feature)
	{
		this.m_Features.add(feature);
	}
}