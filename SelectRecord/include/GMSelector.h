#ifndef GMSelector_H
#define GMSelector_H

#include <vector>
#include <map>
#include "shacommon.h"
#include "GMRecord.h"
#include "SelectionCriteria.h"
#include "SelectionResult.h"
#include <list>

class GMSelector
{
    public:
        GMSelector();

        SelectionResult SelectSingleRecord(DiscretizedFunction& target, SelectionCriteria& criteria);
		std::vector<SelectionResult> SelectMultipleRecords(DiscretizedFunction& target, SelectionCriteria& criteria, int n = 1);
        STATUS PopulateDatabase(std::string databaseFile);
        STATUS WriteSelectionResults(const char* outputFile, std::vector<std::vector<SelectionResult>>& selectedRecords, std::vector<Point>& locations);
        
    private:
        void AddRecord(GMRecord record);
        double GetError(DiscretizedFunction& target, int recordIndex, double& scaleFactor);
        STATUS ReadRecordTimeSeries(const char* recordFileName, std::vector<double>& timeSeries, double& dT);
        STATUS WriteRecords(std::vector<int> recIds);

        std::vector<GMRecord> m_Records;
        std::vector<double> m_Periods;
        ErrorMetric m_ErrorMetric;
};


#endif