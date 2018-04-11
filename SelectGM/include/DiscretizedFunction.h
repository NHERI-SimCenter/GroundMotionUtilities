#ifndef DiscretizedFunction_H
#define DiscretizedFunction_H

#include <vector>
#include "Point.h"

//This class encapsulates a function that has x and y values(e.g. a response spectrum)
class DiscretizedFunction
{
    public:
        DiscretizedFunction(int size);
        DiscretizedFunction(std::vector<double> x, std::vector<double> y);

        int Size();
        void AddPoint(double x, double y);
        double InterpolateY(double x);
        Point GetPoint(int i);
        void Reserve(int size);

    private:
        DiscretizedFunction();

        std::vector<Point> m_Points; //this vector is used to hold the 
};


#endif