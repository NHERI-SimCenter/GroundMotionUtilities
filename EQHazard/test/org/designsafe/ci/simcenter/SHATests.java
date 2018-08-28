/**
 * 
 */
package org.designsafe.ci.simcenter;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

/**
 * @author el7ad
 *
 */
class SHATests {

	private static double dTol = 1e-3;

	@Test
	void test() {
		EQHazardCalc eqCalc = new EQHazardCalc();
		
		//Define the rupture
		EqRuptureConfig eqRupCfg = new EqRuptureConfig("USGS/CGS 2002 Adj. Cal. ERF", 0, 0);
		
		//Define the GMPE
		GMPEConfig gmpeCfg = new GMPEConfig("Abrahamson, Silva & Kamai (2014)");
		
		//Define IM
		IMConfig imCfg = new IMConfig("SA", new double[]{0.2, 1.0});
		
		//Define the Site
		SiteConfig siteCfg = new SiteConfig(new SiteLocation(34.0543, -118.0821));

		EQHazardConfig scenarioConfig = new EQHazardConfig(siteCfg, eqRupCfg, gmpeCfg, imCfg);

		eqCalc.PerformSHA(scenarioConfig);
		SHAOutput output = eqCalc.GetOutput();
		assertEquals(2, output.Periods().length);
		
		assertEquals(-5.6709, output.Result(0).SA().Mean(0), dTol);
		assertEquals(-5.4719, output.Result(0).SA().Mean(1), dTol);		
	}

}
