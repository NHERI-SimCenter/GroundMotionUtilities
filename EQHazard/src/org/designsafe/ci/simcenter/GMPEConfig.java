/**
 * 
 */
package org.designsafe.ci.simcenter;

import java.util.HashMap;

import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class GMPEConfig 
{
	@SerializedName("Type")
	private String 	m_Type;
	
	@SerializedName("Parameters")
	private HashMap<String, String>	m_Parameters = null;
	
	public GMPEConfig(String type)
	{
		this.m_Type = type;
	}
	
	public String Type()
	{
		return m_Type;
	}
}
