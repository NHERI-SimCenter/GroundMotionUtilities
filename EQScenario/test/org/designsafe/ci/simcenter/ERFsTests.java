/**
 * 
 */
package org.designsafe.ci.simcenter;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;
import org.opensha.sha.earthquake.EqkRupture;

/**
 * @author Wael Elhaddad
 *
 */
class ERFsTests {

	private static double dTol = 1e-3;
	
	@Test
	void test() {
		//Create an instance of EQScenario Calculator
		EQScenarioCalc eqCalc = new EQScenarioCalc();
		
		EqRuptureConfig eqRupCfg1 = new EqRuptureConfig("WGCEP (2007) UCERF2 - Single Branch", 0 , 0);
		EqkRupture eqRup1 = eqCalc.getEqRuptureFromERF(eqRupCfg1);
		assertNotNull(eqRup1, "Failed to get rupture from WGCEP (2007) UCERF2 - Single Branch!");
		assertEquals(5.95, eqRup1.getMag(), dTol);
		assertEquals(180.0, eqRup1.getAveRake(), dTol);
		assertEquals(90.0, eqRup1.getRuptureSurface().getAveDip(), dTol);

		EqRuptureConfig eqRupCfg2 = new EqRuptureConfig("GEM1 CEUS ERF", 0 , 0);
		EqkRupture eqRup2 = eqCalc.getEqRuptureFromERF(eqRupCfg2);
		assertNotNull(eqRup2, "Failed to get rupture from GEM ERF!");
//		assertEquals(5.95, eqRup2.getMag(), dTol);
//		assertEquals(180.0, eqRup2.getAveRake(), dTol);
//		assertEquals(90.0, eqRup2.getRuptureSurface().getAveDip(), dTol); 
		
		EqRuptureConfig eqRupCfg3 = new EqRuptureConfig("Mean UCERF3", 0 , 0);
		EqkRupture eqRup3 = eqCalc.getEqRuptureFromERF(eqRupCfg3);
		assertNotNull(eqRup3, "Failed to get rupture from Mean UCERF3!");
		assertEquals(7.950895, eqRup3.getMag(), dTol);
		assertEquals(178.83948, eqRup3.getAveRake(), dTol);
		assertEquals(89.065636, eqRup3.getRuptureSurface().getAveDip(), dTol);
		
		EqRuptureConfig eqRupCfg4 = new EqRuptureConfig("Mean UCERF3 FM3.1", 0 , 0);
		EqkRupture eqRup4 = eqCalc.getEqRuptureFromERF(eqRupCfg4);
		assertNotNull(eqRup4, "Failed to get rupture from Mean UCERF3 FM3.1!");
		assertEquals(8.021972, eqRup4.getMag(), dTol);
		assertEquals(177.82668, eqRup4.getAveRake(), dTol);
		assertEquals(86.93273, eqRup4.getRuptureSurface().getAveDip(), dTol);
		
		EqRuptureConfig eqRupCfg5 = new EqRuptureConfig("Mean UCERF3 FM3.2", 0 , 0);
		EqkRupture eqRup5 = eqCalc.getEqRuptureFromERF(eqRupCfg5);
		assertNotNull(eqRup5, "Failed to get rupture from Mean UCERF3 FM3.2!");
		assertEquals(7.8046412, eqRup5.getMag(), dTol);
		assertEquals(173.21, eqRup5.getAveRake(), dTol);
		assertEquals(88.155594, eqRup5.getRuptureSurface().getAveDip(), dTol);
		
		EqRuptureConfig eqRupCfg6 = new EqRuptureConfig("USGS/CGS 2002 Adj. Cal. ERF", 0 , 0);
		EqkRupture eqRup6 = eqCalc.getEqRuptureFromERF(eqRupCfg6);
		assertNotNull(eqRup6, "Failed to get rupture from USGS/CGS 2002 Adj. Cal. ERF!");
		assertEquals(6.15, eqRup6.getMag(), dTol);
		assertEquals(0.0, eqRup6.getAveRake(), dTol);
		assertEquals(90.0, eqRup6.getRuptureSurface().getAveDip(), dTol);
	}

}
