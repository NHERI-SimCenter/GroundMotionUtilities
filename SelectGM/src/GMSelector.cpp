#include <iostream>
#include <stdio.h>
#include <fstream>
#include <float.h>
#include <math.h>
#include <algorithm>
#include "GMSelector.h"
#include "csvparser.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

GMSelector::GMSelector()
{
    this->m_ErrorMetric = AbsSum;
}

void GMSelector::AddRecord(GMRecord Record)
{
    this->m_Records.push_back(Record);
}

SelectionResult GMSelector::SelectSingleRecord(DiscretizedFunction& target, SelectionCriteria& criteria)
{
    double selectedScaleFactor = 0.0;
    int bestRecIndex = -1;
    double scaleFactor = 0.0;

    double lowestCost = DBL_MAX;

    for (int i = 0; i < m_Records.size(); i++)
    {
        if(this->m_Records[i].CheckCriteria(criteria))
        {
            double cost = this->GetError(target, i, scaleFactor);
            if(cost < lowestCost)
            {
                lowestCost = cost;
                bestRecIndex = i;
                selectedScaleFactor = scaleFactor;
            }
        }
    }
    return SelectionResult(this->m_Records[bestRecIndex], selectedScaleFactor);
}

double GMSelector::GetError(DiscretizedFunction& target, int recordIndex, double& scaleFactor)
{
    double error = 0.0;
    int size = target.Size();
    
    GMRecord& record = this->m_Records[recordIndex];
    std::vector<double> spectralAccels = record.GetSpectrum();
    DiscretizedFunction recordSpectrum(m_Periods, spectralAccels);

    double* interpRecSpectrum = new double[size];
    
    //first let's interpolate the record spectrum
    for (int i = 0; i < size; i++)
    {
        Point p = target.GetPoint(i);
        interpRecSpectrum[i] = recordSpectrum.InterpolateY(p.X());
    }

    //Now let's evaluate the scaling factor
    double dotProduct = 0.0;
    double recSpecNorm = 0.0;
    for (int i = 0; i < size; i++)
    {
        dotProduct += target.GetPoint(i).Y() * interpRecSpectrum[i];
        recSpecNorm += pow(interpRecSpectrum[i], 2);
    }

    scaleFactor = dotProduct/recSpecNorm;

    //calculate the error with the scaling factor
    for (int i = 0; i < size; i++)
    {
        Point p = target.GetPoint(i);
        error += fabs(scaleFactor * interpRecSpectrum[i] - p.Y());
    }

    delete[] interpRecSpectrum;
    return error;
}

STATUS GMSelector::PopulateDatabase(std::string databaseFile)
{
    this->m_Periods.clear();
    this->m_Records.clear();

    //TODO: we need to reserve Records based on count

    CsvParser* pCsvParser = CsvParser_new(databaseFile.c_str(), ",", 1);
    const CsvRow* pHeader = CsvParser_getHeader(pCsvParser);

    if (!pHeader) {
        printf("%s\n", CsvParser_getErrorMessage(pCsvParser));
        return FAIL;
    }

    //we need to get the periods of the spectrum
    const char** ppColumnNames = CsvParser_getFields(pHeader);

    int numFields = CsvParser_getNumFields(pHeader);
    std::map<std::string, int> fieldMap;
    std::map<double, int> spectrumMap;

    for(int i = 0; i < numFields; i++)
    {
        std::string columnName = std::string(ppColumnNames[i]);
        if(0 == columnName.compare("RecId"))
        {
            fieldMap.insert(std::pair<std::string, int>("RecId", i));
        }
        else if(0 == columnName.compare("Magnitude"))
        {
            fieldMap.insert(std::pair<std::string, int>("Magnitude", i));
        }
        else if(0 == columnName.compare("Vs30"))
        {
            fieldMap.insert(std::pair<std::string, int>("Vs30", i));
        }
        else if(0 == columnName.compare("ClstD"))
        {
            fieldMap.insert(std::pair<std::string, int>("ClstD", i));
        }
        else if(0 == columnName.compare("FileNameHorizontal1"))
        {
            fieldMap.insert(std::pair<std::string, int>("FileNameHorizontal1", i));
        }
        else if(0 == columnName.compare("FileNameHorizontal2"))
        {
            fieldMap.insert(std::pair<std::string, int>("FileNameHorizontal2", i));
        }
        else if(0 == columnName.compare("FileNameVertical"))
        {
            fieldMap.insert(std::pair<std::string, int>("FileNameVertical", i));
        }
        else if(columnName[0] == 'T' && columnName[columnName.size()-1] == 'S')
        {
            //We found an entry in the spectrum
            std::string strPeriod = columnName.substr(1, columnName.size()-2);
            double period = strtod (strPeriod.c_str(), NULL);

            spectrumMap.insert(std::pair<double, int>(period, i));
        }
    }

    std::map<double, int>::iterator iterSA;
    for (iterSA = spectrumMap.begin(); iterSA != spectrumMap.end(); iterSA++)
    {
            m_Periods.push_back(iterSA->first);
    }

    CsvRow* pRow = NULL;
    int count = 0;
    while((pRow = CsvParser_getRow(pCsvParser)))
    {
        std::cout << "\rReading record "<< ++count;
        
        const char** ppRowVals = CsvParser_getFields(pRow);

        int id = atoi (ppRowVals[fieldMap["RecId"]]);
        double magnitude = strtod (ppRowVals[fieldMap["Magnitude"]], NULL);
        double distance = strtod (ppRowVals[fieldMap["ClstD"]], NULL);
        double vs30 = strtod (ppRowVals[fieldMap["Vs30"]], NULL);
        std::string hzFile1 = ppRowVals[fieldMap["FileNameHorizontal1"]];
        std::string hzFile2 = ppRowVals[fieldMap["FileNameHorizontal2"]];
        std::string vlFile = ppRowVals[fieldMap["FileNameVertical"]];

        std::vector<double> spectrum;

        for(iterSA = spectrumMap.begin(); iterSA != spectrumMap.end(); iterSA++)
        {    
            //We found an entry in the spectrum
            double sa = strtod (ppRowVals[iterSA->second], NULL);
            spectrum.push_back(sa);
        }

        GMRecord newRecord(id, magnitude, distance, vs30, hzFile2, hzFile2, vlFile, spectrum);
        this->AddRecord(newRecord);
        CsvParser_destroy_row(pRow);
    }
    std::cout << std::endl;
    CsvParser_destroy(pCsvParser);
    
    return SUCCESS;
}

STATUS GMSelector::WriteRecords(std::vector<int> recIds)
{
    for (int i = 0; i < recIds.size(); i++)
    {
        Document recordJson;
        recordJson.SetObject();
        Document::AllocatorType& allocator = recordJson.GetAllocator();

        Value Acceleration(kObjectType);
        Value Horizontal1(kArrayType);
        Value Horizontal2(kArrayType);
        Value Vertical(kArrayType);

        //We need to read the acceleration time series
        //TODO: We need to figure out how to retrieve the files 
        //TODO: Temporarily file names are hardcoded, returning a single record
        std::vector<double> hz1Accel;
        double dT;
        ReadRecordTimeSeries("RSN1_HELENA.A_A-HMC180.AT2", hz1Accel, dT);
        for(int j = 0; j < hz1Accel.size(); j++)
        {
            Horizontal1.PushBack(hz1Accel[j], allocator);
        }

        std::vector<double> hz2Accel;
        ReadRecordTimeSeries("RSN1_HELENA.A_A-HMC270.AT2", hz2Accel, dT);
        for(int j = 0; j < hz2Accel.size(); j++)
        {
            Horizontal2.PushBack(hz2Accel[j], allocator);
        }

        std::vector<double> vlAccel;
        ReadRecordTimeSeries("RSN1_HELENA.A_A-HMCDWN.AT2", vlAccel, dT);
        for(int j = 0; j < vlAccel.size(); j++)
        {
            Vertical.PushBack(vlAccel[j], allocator);
        }

        Acceleration.AddMember("Horizontal1", Horizontal1, allocator);
        Acceleration.AddMember("Horizontal2", Horizontal2, allocator);
        Acceleration.AddMember("Vertical", Vertical, allocator);
        Acceleration.AddMember("dT", 0.01, allocator);

        recordJson.AddMember("Acceleration", Acceleration, allocator);
        
        char idBuffer[10];
        snprintf(idBuffer, 10, "%d", recIds[i]);
        std::string recordFileName("./Records/Record");
        recordFileName.append(idBuffer);
        recordFileName.append(".json");

        std::ofstream ofs(recordFileName.c_str());
        OStreamWrapper osw(ofs);
        PrettyWriter<OStreamWrapper> writer(osw);
        recordJson.Accept(writer);
    }
    return SUCCESS;
}

STATUS GMSelector::ReadRecordTimeSeries(const char* recordFileName, std::vector<double>& timeSeries, double& dT)
{
    std::ifstream recordStream(recordFileName);
    char lineBuffer[256];

    int totalCount = 0;
    int count = 0;
    
    while(!recordStream.eof())
    {
        recordStream.getline(lineBuffer, 255);

        if(0 == totalCount)
        {
            std::string line (lineBuffer);

            if(0 == line.compare(0, 5, "NPTS="))
            {
                std::string nToken = line.substr(0, line.find(','));
                std::string dtToken = line.substr(line.find(','),  std::string::npos);

                totalCount = atoi(nToken.substr(5,  std::string::npos).c_str());
                timeSeries.reserve(totalCount);

                dT = atof(nToken.substr(3,  std::string::npos).c_str());
            }
        }
        else
        {
            char* token;
            token = strtok(lineBuffer ,"\\t");
            while(token)
            {
                double accel = atof(token);
                timeSeries.push_back(accel);
                token = strtok(NULL ,"\\t");
            }
        }
    }
    recordStream.close();
    return SUCCESS;
}

STATUS GMSelector::WriteSelectionResults(const char* outputFile, std::vector<SelectionResult>& selectionResults, std::vector<Point>& locations)
{
    Document output;
    output.SetObject();
    Document::AllocatorType& allocator = output.GetAllocator();
    Value GroundMotions(kArrayType);

    for (int i = 0; i < selectionResults.size(); i++)
    {
        Value result(kObjectType);
        Value selectedRecord(kObjectType);

        Value Id(selectionResults[i].Record().GetId());
        selectedRecord.AddMember("Id", Id, allocator);

        Value source;
        std::string sourceName = selectionResults[i].Record().GetSource();
        source.SetString(sourceName.c_str(), sourceName.length(), allocator);
        selectedRecord.AddMember("Source", source, allocator);

        result.AddMember("Record", selectedRecord, allocator);
        
        result.AddMember("ScaleFactor", selectionResults[i].ScaleFactor(), allocator);
        
        GroundMotions.PushBack(result, allocator);
    }
    output.AddMember("GroundMotions", GroundMotions, allocator);
    std::ofstream ofs(outputFile);
    OStreamWrapper osw(ofs);
    PrettyWriter<OStreamWrapper> writer(osw);
    output.Accept(writer);

    // //Now we can also retrieve the records
    // std::vector<int> recIds;
    // recIds.reserve(selectionResults.size());

    // for(int i = 0 ; i < selectionResults.size(); i++)
    // {
    //     recIds.push_back(selectionResults[i].Record().GetId());
    // }
    // std::sort(recIds.begin(), recIds.end());
    // std::vector<int>::iterator last = std::unique(recIds.begin(), recIds.end());
    // recIds.erase(last, recIds.end());
    // this->WriteRecords(recIds);

    //Creating GeoJsonOutput
    Document geoJson;
    geoJson.SetObject();
    Document::AllocatorType& geoJsonAllocator = geoJson.GetAllocator();
    geoJson.AddMember("Type", "FeatureCollection", geoJsonAllocator);

    Value features(kArrayType);
    for(int i = 0; i < locations.size(); i++)
    {
        Value feature(kObjectType);
        feature.AddMember("Type", "Feature", geoJsonAllocator);
        
        Value geometry(kObjectType);
        geometry.AddMember("Type", "Point", geoJsonAllocator);

        //add the coordinates
        Value coordinates(kArrayType);
        coordinates.PushBack(locations[i].X(), geoJsonAllocator);
        coordinates.PushBack(locations[i].Y(), geoJsonAllocator);
        geometry.AddMember("Coordinates", coordinates, geoJsonAllocator);
        feature.AddMember("Geometry", geometry, geoJsonAllocator);

        //add properties
        Value properties(kObjectType);
        properties.AddMember("RecordId", selectionResults[i].Record().GetId(), geoJsonAllocator);
        feature.AddMember("Properties", properties, geoJsonAllocator);

        features.PushBack(feature, geoJsonAllocator);
    }
    geoJson.AddMember("Features", features, allocator);
    std::ofstream geoJsonOfs("SelectionOutput_Geo.json");
    OStreamWrapper geoJsonOsw(geoJsonOfs);
    PrettyWriter<OStreamWrapper> geoJsonWriter(geoJsonOsw);
    geoJson.Accept(geoJsonWriter);

	return STATUS::SUCCESS;
}