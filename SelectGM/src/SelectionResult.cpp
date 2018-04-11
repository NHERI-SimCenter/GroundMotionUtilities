#include "SelectionResult.h"


SelectionResult::SelectionResult(GMRecord record, double scaleFactor):m_Record(record)
{
    this->m_ScaleFactor = scaleFactor;
}

GMRecord SelectionResult::Record()
{
    return m_Record;
}

double SelectionResult::ScaleFactor()
{
    return m_ScaleFactor;
}