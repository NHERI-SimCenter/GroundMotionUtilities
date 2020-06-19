package org.designsafe.ci.simcenter;

import java.lang.reflect.Field;
import java.util.List;

import org.opensha.commons.param.Parameter;
import org.opensha.commons.param.event.ParameterChangeWarningListener;
import org.opensha.sha.imr.ScalarIMR;
import org.opensha.sha.imr.attenRelImpl.MultiIMR_Averaged_AttenRel;
import org.opensha.sha.imr.attenRelImpl.NGAWest_2014_Averaged_AttenRel;
import org.opensha.sha.imr.attenRelImpl.NGAWest_2014_Averaged_AttenRel.NGAWest_2014_Averaged_AttenRel_NoIdriss;

public class NGAWestAverage extends NGAWest_2014_Averaged_AttenRel_NoIdriss {
	List<? extends ScalarIMR> imrs;
	public NGAWestAverage(ParameterChangeWarningListener listener) {
		super(listener);
		Field imrField;
		try {
			imrField = MultiIMR_Averaged_AttenRel.class.getDeclaredField("imrs");
			imrField.setAccessible(true);
			imrs = (List<? extends ScalarIMR>)imrField.get(this);
			
		} catch (NoSuchFieldException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SecurityException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	
	public void setParameter(String name, Object value)
	{
		for (ScalarIMR imr : imrs)
		{
			Parameter param = imr.getParameter(name);
			param.setValue(value);
		}
	}
}


