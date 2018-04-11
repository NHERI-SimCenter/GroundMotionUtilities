#ifndef SelectionCriteria_H
#define SelectionCriteria_H

#include "Range.h"

class SelectionCriteria
{
    public:
        SelectionCriteria();
        void SetMagnitudeRange(double min, double max);
        void SetDistanceRange(double min, double max);
        void SetVs30Range(double min, double max);
        Range Magnitude();
        Range Distance();
        Range Vs30();

    private:
        Range m_Magnitude;
        Range m_Distance;
        Range m_Vs30;
};

#endif