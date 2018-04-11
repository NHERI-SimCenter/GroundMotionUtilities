#ifndef SelectionTarget_H
#define SelectionTarget_H

#include <vector>
#include "shacommon.h"
#include "SelectionConfig.h"
#include "DiscretizedFunction.h"

class SelectionTarget
{
    public:
        SelectionTarget(SelectionConfig config);
        DiscretizedFunction GetSpectrum(int i);
        int NumTargets();
        std::vector<Point> GetLocations();

    private:
        SelectionTarget();
        void GetTargetSpectrums(std::string filename);

        TargetType m_Type;
        std::vector<DiscretizedFunction> m_Targets;
        std::vector<Point> m_Locations;
};

#endif