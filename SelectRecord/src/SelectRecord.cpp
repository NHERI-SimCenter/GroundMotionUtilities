#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "shacommon.h"
#include "GMSelector.h"
#include "SelectionConfig.h"
#include "SelectionTarget.h"

int main(int argc, const char **argv)
{
    if(argc < 3)
    {
        std::cout << "Usage: SelectGM ConfigFile OutputFile";
        return -1;
    }

    SelectionConfig config;
    try
    {
        config.ReadFromFile(argv[1]);
    }
    catch(SHAException& e)
    {
        std::cout << "Failed to read selection config" << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }

    GMSelector selector;

    clock_t begin_time = clock();    
    //First, we need to create the database of ground motions
    if(STATUS::SUCCESS != selector.PopulateDatabase(config.DatabaseFile()))
    {
        std::cout << "Failed to read records database file";
        return -1;
    }

    float elapsedTime = float( clock () - begin_time ) /  CLOCKS_PER_SEC;

    std::cout << "Spent "<< elapsedTime << " seconds to read records database" << std::endl;
    
    //Let's read the target spectrums
    begin_time = clock();
    SelectionTarget target(config);
    elapsedTime = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    std::cout << "Spent "<< elapsedTime << " seconds to read target spectra" << std::endl;

    //Now, select records from the database
    begin_time = clock();
    std::vector<std::vector<SelectionResult>> selectedRecords;
    selectedRecords.reserve(target.NumTargets());

    for(int i = 0 ; i < target.NumTargets(); i++)
    {
        std::cout << "\rSelecting record for target "<< i+1;
        SelectionCriteria critera = config.Criteria();
        DiscretizedFunction targetSpectrum = target.GetSpectrum(i);
        auto selectionResult = selector.SelectMultipleRecords(targetSpectrum, critera, 10);

        selectedRecords.push_back(selectionResult);
    }
    std::cout << std::endl;
    elapsedTime = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    std::cout << "Spent "<< elapsedTime << " seconds to select records from database" << std::endl;

    std::vector<Point> locations = target.GetLocations();
    //Write Selection Results
    if(STATUS::SUCCESS != selector.WriteSelectionResults(argv[2], selectedRecords, locations))
    {
        std::cout << "Failed to write results!";
        return -1;
    }

    return 0;
}


