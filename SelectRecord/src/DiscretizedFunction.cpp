#include "DiscretizedFunction.h"

DiscretizedFunction::DiscretizedFunction(std::vector<double> x, std::vector<double> y)
{
    if(x.size() != y.size())
        throw "Invalid vector size!";

    this->m_Points.reserve(x.size());
    for (int i = 0; i < x.size(); i++)
    {
        this->AddPoint(x[i],y[i]);
    } 
}

DiscretizedFunction::DiscretizedFunction(int size){
    m_Points.reserve(size);    
}

void DiscretizedFunction::AddPoint(double x, double y)
{
    this->m_Points.push_back(Point(x,y));
}

double DiscretizedFunction::InterpolateY(double x)
{
    for (int i = 0; i < m_Points.size()-1 ; i++)
    {
		if (x <= m_Points[0].X())
			return m_Points[0].Y();

		if (x >= m_Points[m_Points.size() - 1].X())
			return m_Points[m_Points.size() - 1].Y();

        if(m_Points[i].X() <= x && m_Points[i+1].X() >= x)
        {
            double d = m_Points[i+1].X() - m_Points[i].X();

            double w1 = (x - m_Points[i].X())/d;
            double w2 = (m_Points[i+1].X() - x)/d;

            return m_Points[i].Y() * w1 + m_Points[i+1].Y() * w2;
        }
    }

    throw "Failed to interpolate, the provided value of x is out of range!";
}

Point DiscretizedFunction::GetPoint(int i)
{
    return m_Points[i];
}

int DiscretizedFunction::Size()
{
    return m_Points.size();
}