#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <random>
#include <time.h>
#include <sys/stat.h>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "GMSimulator.h"


using namespace rapidjson;


int main(int argc, const char **argv)
{
    if(argc != 3)
    {
        //Input is not proper, so usage will be printed to the screen
        std::cout << "Usage:" << std::endl;
        std::cout << "\tSimulateGM SimulationConfig.json Output.json" << std::endl;

        return -1;
    }
    
    //first we need to open the config file to read the input
    Document doc;
    std::ifstream ifs(argv[1]);
    IStreamWrapper isw(ifs);
    doc.ParseStream(isw);
    std::string gmFileName(doc["GroundMotions"]["File"].GetString());

    int numSimulations = 1;
    if(doc.HasMember("NumSimulations"))
        numSimulations = doc["NumSimulations"].GetInt();

    bool isCorrelated = true;
    if(doc.HasMember("SpatialCorrelation"))
        isCorrelated = doc["SpatialCorrelation"].GetBool();

    int seed =1;
    if(doc.HasMember("Seed"))
        seed = doc["Seed"].GetInt();

    GMSimulator::SetSeed(seed);
    
    std::string outputFileName(argv[2]);

    //Now we can run the simulation
    try{
        if(STATUS::SUCCESS != GMSimulator::SimulateGM(gmFileName, outputFileName, isCorrelated, numSimulations))
        {
            std::cout << "\nFailed to simulate ground motions!\n";
            return -1;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Failed to simulation ground motions!" << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}