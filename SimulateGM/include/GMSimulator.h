#ifndef GMSimulator_H
#define GMSimulator_H

#include <vector>
#include "shacommon.h"
#include "Eigen/Dense"

using namespace Eigen;

class GMSimulator
{
    public:
    static STATUS SimulateGM(std::string filename, std::string OutputFile, bool isCorrelated = true, int numSimulations = 1);
    
    private:
    static MatrixXd GetEpsilon(std::vector<Location> locations, double period);
    static double GetDistance(Location location1, Location location2);
    static VectorXd GetRandnVector(int nRows);
};

#endif