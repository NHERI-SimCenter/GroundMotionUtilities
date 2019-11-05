#ifndef SHACommon_H
#define SHACommon_H

#include <exception>
#include <string>

enum class STATUS {SUCCESS = 0, FAIL = -1};

enum class ErrorMetric {AbsSum = 0, RMS = 1, MAPE = 2, AbsMax = 3};
enum class DbType {CSV = 0, SQLite = 1};
enum class TargetType {Mean = 0, Simulated = 1};

class SHAException
{
    public:
        SHAException() throw();
        SHAException(std::string message){this->m_Message = message;}
        const char* what(){return m_Message.c_str();}

    private:
        std::string m_Message;
};

struct Location
{
    double Longitude;
    double Latitude;
};

#endif