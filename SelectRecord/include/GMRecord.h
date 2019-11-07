#ifndef GMRecord_H
#define GMRecord_H

#include <string>
#include <vector>
#include "DiscretizedFunction.h"
#include "SelectionCriteria.h"

class GMRecord
{
    public:
        std::string             GetSource();
        int                     GetId();
        double                  GetMagnitude();
        std::vector<double>     GetSpectrum();
		std::string             GetHorizontalFile1();
		std::string             GetHorizontalFile2();
		std::string             GetVerticalFile();

        GMRecord(int id, double magnitude, double distance, double vs30, 
                std::string hzFile1, std::string hzFile2, std::string vlFile, std::vector<double> spectrum);
        bool CheckCriteria(SelectionCriteria& criteria);
        void SetSpectrum(std::vector<double>);

    private:
        int                     m_Id;
        double                  m_Magnitude;
        double                  m_Distance;
        double                  m_Vs30;
        std::vector<double>     m_Spectrum;
        std::string             m_HzFile1;
        std::string             m_HzFile2;
        std::string             m_VlFile;
        std::string             m_Source;
};


#endif