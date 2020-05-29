/**
 * 
 */
package org.designsafe.ci.simcenter;

import java.util.ArrayList;
import java.util.Map;

import com.google.gson.annotations.SerializedName;


/**
 * @author Wael Elhaddad
 *
 */
public class EqRuptureConfig 
{
	@SerializedName("Type")
	private String	m_Type;
	
	@SerializedName("Magnitude")
	private double	m_Magnitude;

	@SerializedName("Probability")
	private Double	m_Probability = null;
	
	@SerializedName("MeanAnnualRate")
	private Double	m_MeanAnnualRate = null;
	
	@SerializedName("Location")
	private RuptureLocation	m_Location;
	
	@SerializedName("AverageRake")
	private double	m_AverageRake;
	
	@SerializedName("AverageDip")
	private double	m_AverageDip;
	
	@SerializedName("RuptureForecast")
	private String 	m_RuptureForecast;
	
	@SerializedName("SourceIndex")
	private Integer	m_SourceIndex = null;
	
	@SerializedName("RuptureIndex")
	private Integer	m_RuptureIndex = null;
	
	@SerializedName("Surface")
	private ArrayList<RuptureLocation> m_Surface;
	
	@SerializedName("ExportGeoJson")
	private Boolean m_exportGeoJson = null;
	
	@SerializedName("MaxDistance")
	private Double m_maxDistance = null;
	
	@SerializedName("MaxSources")
	private Integer m_maxSources = null;
	
	@SerializedName("Parameters")
	private Map<String, Object> m_parameters = null;
	
	
	public EqRuptureConfig(RuptureLocation location, double magnitude, double averageDip, double averageRake)
	{
		this.m_Type = "PointSource";
		this.m_Magnitude = magnitude;
		this.m_Location = location;
		this.m_AverageDip = averageDip;
		this.m_AverageRake = averageRake;
	}
	
	public EqRuptureConfig(String ruptureForecast, int sourceIndex, int ruptureIndex)
	{
		this(ruptureForecast, sourceIndex, ruptureIndex, null);
	}

	public EqRuptureConfig(String ruptureForecast, int sourceIndex, int ruptureIndex, ArrayList<RuptureLocation> surface)
	{
		this.m_Type = "ERF";
		this.m_RuptureForecast = ruptureForecast;
		this.m_SourceIndex = sourceIndex;
		this.m_RuptureIndex = ruptureIndex;
		this.m_Surface = surface;
	}
	
	public double Magnitude()
	{
		return m_Magnitude;
	}
	
	public double AverageRake()
	{
		return this.m_AverageRake;
	}
	
	public double AverageDip()
	{
		return this.m_AverageDip;
	}
	
	public RuptureLocation Location()
	{
		return this.m_Location;
	}
	
	public String RuptureForecast()
	{
		return this.m_RuptureForecast;
	}
	
	public String Type()
	{
		return this.m_Type;
	}
	
	public int SourceIndex()
	{
		return this.m_SourceIndex;
	}
	
	public int RuptureIndex()
	{
		return this.m_RuptureIndex;
	}
	
	public double MaxDistance()
	{
		return this.m_maxDistance;
	}
	
	public int MaxSources()
	{
		return this.m_maxSources;
	}
	
	public Map<String, Object> Parameters()
	{
		return this.m_parameters;
	}
	
	public void SetRupture(double magnitude,  double averageDip, double averageRake, double probability, double meanAnnualRate,  ArrayList<RuptureLocation> surface)
	{
		this.m_Magnitude = magnitude;
		this.m_AverageDip = averageDip;
		this.m_AverageRake = averageRake;
		this.m_Surface = surface;
		this.m_Probability = probability;
		this.m_MeanAnnualRate = meanAnnualRate;
	}
}
