#ifndef Range_H
#define Range_H

class Range
{
    public:
        Range();
        Range(double min, double max);
        void Set(double min, double max);
        double Min();
        double Max();
    
    private:
        double m_Min;
        double m_Max;
};

#endif