#ifndef Point_H
#define Point_H

class Point
{   
    public:    
        Point();
        Point(double x, double y);
        void Set(double x, double y);
        double X();
        double Y();

    private:
        double m_X;
        double m_Y;
};

#endif