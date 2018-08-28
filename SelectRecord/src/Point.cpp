#include "Point.h"

Point::Point(){
    this->Set(0.0, 0.0);
}

Point::Point(double x, double y){
    this->Set(x, y);
}

void Point::Set(double x, double y){
    this->m_X = x;
    this->m_Y = y;
}

double  Point::X(){
    return this->m_X;
}

double  Point::Y(){
    return this->m_Y;
}