package org.designsafe.ci.simcenter;

import com.google.gson.annotations.SerializedName;

//This only supports points
public class GeoJsonGeometry
{
	@SerializedName("Type")
	private final String m_Type = "Point";
	
	@SerializedName("Coordinates")
	private double[] m_Coordinates = new double[2];
	
	GeoJsonGeometry(double latitude, double longitude)
	{
		m_Coordinates[0] = longitude;
		m_Coordinates[1] = latitude;
	}
}