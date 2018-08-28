#ifndef SelectionConfig_H
#define SelectionConfig_H
#include <string>
#include "shacommon.h"
#include "SelectionCriteria.h"

class SelectionConfig
{
    public:
        SelectionConfig();
        SelectionConfig(const SelectionConfig& config);
        SelectionConfig(SelectionCriteria criteria, std::string databaseFile, std::string targetFile);

        void ReadFromFile(const char* configFile);
        SelectionCriteria   Criteria();
        std::string         DatabaseFile();
        std::string         TargetFile();
        TargetType          GetTargetType();

    private:
        DbType ParseDbType(const char* dbType);
        TargetType ParseTargetType(const char* targetType);

        SelectionCriteria   m_Criteria;
        std::string         m_DatabaseFile;
        std::string         m_TargetFile;
        DbType              m_DatabaseType;
        TargetType          m_TargetType;
};

#endif