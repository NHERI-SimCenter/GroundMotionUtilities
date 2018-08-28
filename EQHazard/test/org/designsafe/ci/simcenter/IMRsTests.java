/**
 * 
 */
package org.designsafe.ci.simcenter;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Arrays;
import java.util.List;

import org.junit.jupiter.api.Test;
import org.opensha.sha.imr.AttenuationRelationship;

/**
 * @author Wael Elhaddad
 *
 */
class IMRsTests {

	@Test
	void test() {
		
		//Create an array list of the Attnuatuation relationships that should be supported
		List<String> imrList = Arrays.asList(
				"Abrahamson, Silva & Kamai (2014)",
				"Boore, Stewart, Seyhan & Atkinson (2014)",
				"Campbell & Bozorgnia (2014)",
				"Chiou & Youngs (2014)",
				"Idriss (2014)",
				"Campbell & Bozorgnia (2008)",
				"Boore & Atkinson (2008)",
				"Abrahamson & Silva (2008)",
				"Chiou & Youngs (2008)",
				"Boore & Atkinson (2006)",
				"Chiou & Youngs (2006)",
				"Campbell & Bozorgnia (2006)",
				"Campbell and Bozorgnia (2003)",
				"Field (2000)",
				"Campbell (1997) w/ erratum (2000) changes",
				"Abrahamson & Silva (1997)",
				"Boore, Joyner & Fumal (1997)");
		
		//Create an instance of EQScenario Calculator
		EQHazardCalc eqCalc = new EQHazardCalc();
		
		//Loop through a list of the supported IMRs and check if we can create them
		for(String imrName:imrList)
		{			
			AttenuationRelationship imr = null;
			try
			{
				imr = eqCalc.CreateIMRInstance(imrName);
			}
			catch(Exception e)
			{
				fail(e.getMessage());
			}
			assertNotNull(imr, "Failed to create IMR: " + imrName);
		}
	}
}
