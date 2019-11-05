#include "GMSimulator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <random>
#include <ctime>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;
int GMSimulator::s_seed = 1;


STATUS GMSimulator::SimulateGM(std::string gmFile, std::string OutputFile, bool isCorrelated, int numSimulations)
{
    Document doc;
    std::ifstream ifs(gmFile.c_str());
    IStreamWrapper isw(ifs);
    doc.ParseStream(isw);

    rapidjson::Value::Array periods = doc["Periods"].GetArray();

    Value::Array GMs = doc["GroundMotions"].GetArray();
    std::vector<double> SA;

    int spectrumSize = periods.Size();
    int numSites = GMs.Size();
    
    MatrixXd simGM(spectrumSize, numSites);

    std::vector<Location> siteLocations(numSites);

    //We need to read all sites locations to be able to compute covariance matrix
    for (int i = 0; i < GMs.Size(); i++)
    {
        Value::Object Location = GMs[i]["Location"].GetObject();
            
        siteLocations[i].Longitude = Location["Longitude"].GetDouble();
        siteLocations[i].Latitude = Location["Latitude"].GetDouble();
    }

    Document::AllocatorType& allocator = doc.GetAllocator();

    //Simulating ground motions
    for (int k = 0; k < numSimulations; k++)
    {
        std::cout << "Processing simulation " << k+1 << ":" << std::endl;

        VectorXd eta = GetRandnVector(periods.Size());
        for (int i = 0; i < periods.Size(); i++)
        {
            std::cout << "\t Processing period: " << periods[i].GetDouble();
            clock_t begin_time = clock();            
            
            if(isCorrelated)
            {
                MatrixXd epsilons = GetEpsilon(siteLocations, periods[i].GetDouble());

                for (int j = 0; j < GMs.Size(); j++)
                {

                    Value::Array mu = GMs[j]["SA"]["Mean"].GetArray();
                    Value::Array tau = GMs[j]["SA"]["InterEvStdDev"].GetArray();
                    Value::Array phi = GMs[j]["SA"]["IntraEvStdDev"].GetArray();

                    if(mu.Size() != periods.Size())
                        return STATUS::FAIL;         
                    if(tau.Size() != periods.Size())
                        return STATUS::FAIL;         
                    if(phi.Size() != periods.Size())
                        return STATUS::FAIL;
                    
                    simGM(i,j) = mu[i].GetDouble() + tau[i].GetDouble() * epsilons(0,j) + phi[i].GetDouble() * eta(i);          
                }
            }
            else
            {
                for (int j = 0; j < GMs.Size(); j++)
                {
                    Value::Array mu = GMs[j]["SA"]["Mean"].GetArray();
                    Value::Array sigma = GMs[j]["SA"]["TotalStdDev"].GetArray();

                    if(mu.Size() != periods.Size())
                        return STATUS::FAIL;         
                    if(sigma.Size() != periods.Size())
                        return STATUS::FAIL;

                    simGM(i,j) = mu[i].GetDouble() + sigma[i].GetDouble() * eta(i);
                }
            }

            float elapsedTime = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
            std::cout << " [Elapsed time: " << elapsedTime << " sec.]" << std::endl;
        }

        for (int i = 0; i < numSites; i++)
        {
            Value simulation(kArrayType);
            for (int j = 0; j < spectrumSize; j++)
            {
                simulation.PushBack(simGM(j, i), allocator);
            }

            //Add a new simulations array or add simulation to an existing array
            if(GMs[i]["SA"].HasMember("Simulations"))
            {
                GMs[i]["SA"]["Simulations"].GetArray().PushBack(simulation, allocator);
            }
            else
            {
                Value simulations(kArrayType);
                simulations.PushBack(simulation, allocator);
                GMs[i]["SA"].AddMember("Simulations", simulations, allocator);
            }
        }
    }

    std::ofstream ofs(OutputFile.c_str());
    OStreamWrapper osw(ofs);
    PrettyWriter<OStreamWrapper> writer(osw);
    doc.Accept(writer);
    return STATUS::SUCCESS;
}

MatrixXd GMSimulator::GetEpsilon(std::vector<Location> locations, double period)
{
    int numLocations = locations.size();
    MatrixXd covariance(numLocations, numLocations);

    double b = 0.0;
    if(period < 1)
        b = 40.7 - 15.0 * period;
    else
        b = 22.0 + 3.70 * period;

    for (int i = 0; i < numLocations; i++)
    {
        Location loci = locations[i];

        for (int j = i; j < numLocations; j++)
        {
            Location locj = locations[j];
            double d = GetDistance(loci, locj);
            covariance(i, j) = covariance(j, i) = exp(-3.0 * d / b);
        }
    }

    VectorXd rndVec = GetRandnVector(numLocations);
    MatrixXd epsilons(1, numLocations);
    epsilons = rndVec.transpose() * covariance.llt().matrixU();

    return epsilons;
}

//Using Haversine formula to get distance in kilometers
double GMSimulator::GetDistance(Location location1, Location location2)
{
    double raduis = 6371; // in km

    double long1 = location1.Longitude * M_PI/ 180;
    double lat1 = location1.Latitude * M_PI/ 180;
    double long2 = location2.Longitude * M_PI/ 180;
    double lat2 = location2.Latitude * M_PI/ 180;

    double dlat = lat2 - lat1;
    double dlong = long2 - long1;

    double a = pow(sin(dlat / 2.0), 2.0) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2.0), 2);
    double c = 2.0 * asin(sqrt(a));

    return raduis * c;
}

VectorXd GMSimulator::GetRandnVector(int nRows)
{
    std::default_random_engine rndEngine;
    rndEngine.seed(s_seed++);

    std::normal_distribution<> normDist{0, 1};

    VectorXd randnVec(nRows);
    for (int i = 0; i < nRows; i++)
    {
        randnVec(i) = normDist(rndEngine);
    }
    
    return randnVec;
}

void GMSimulator::SetSeed(int seed)
{
    s_seed = seed;
}
