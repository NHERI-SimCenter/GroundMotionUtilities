#ifndef SelectionResult_H
#define SelectionResult_H
#include "GMRecord.h"

class SelectionResult
{
    public:
        SelectionResult(GMRecord record, double scaleFactor);
        GMRecord Record();
        double ScaleFactor();

    private:
        SelectionResult();
        GMRecord m_Record;
        double  m_ScaleFactor;
};

#endif