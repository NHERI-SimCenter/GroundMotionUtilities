package org.designsafe.ci.simcenter;

public enum ErrorCode {
	
	INPUT_FILE_MISSING(-1, "Input file does not exist!"),
	INVALID_IMR(-2, "Invalid Intensity Measure Relationship!"),
	INVALID_ERF(-3, "Invalid Earthquake Rupture Forecast"),
	INVALID_INPUT(-4, "Invalid input specified!"),
	UNKNOWN_ERROR(-999, "Unknown error or unhandled exception occured!");

	private final int code;
	private final String description;

	private ErrorCode(int code, String description) 
	{
		this.code = code;
		this.description = description;
	}

	public String getDescription() 
	{
		return description;
	}

	public int getCode() 
	{
		return code;
	}

	@Override
	public String toString()
	{
		return code + ": " + description;
	}
}
