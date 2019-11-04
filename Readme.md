**Builds Status**

| **Windows** | **Mac** | **Linux**|
|---|---|---|
[![Build Status](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_apis/build/status/el7addad.GroundMotionUtilities?branchName=master&jobName=Job&configuration=Job%20windows)](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_build/latest?definitionId=1&branchName=master)|[![Build Status](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_apis/build/status/el7addad.GroundMotionUtilities?branchName=master&jobName=Job&configuration=Job%20mac)](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_build/latest?definitionId=1&branchName=master)|[![Build Status](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_apis/build/status/el7addad.GroundMotionUtilities?branchName=master&jobName=Job&configuration=Job%20linux)](https://dev.azure.com/el7addad/Ground%20Motion%20Utilities/_build/latest?definitionId=1&branchName=master)|

# Simcenter Ground Motion Utilities
This repository contains a suite of tools that are developed by the SimCenter to facilitate the use of seismic hazard analysis(SHA) for different research applications.

The tools in this repository are organized in following folders:
1. [EQHazard](EQHazard/Readme.md)
2. [SimulateIM](SimulateIM/Readme.md)
3. [SelectRecord](SelectRecord/Readme.md)

## User's Guide
The tools provided in this repository are console applications. Each tool requires an input json file and outputs one or more json files. Sample usage of each tool is printed on the screen when the applications are executed without arguments. In addition, documentation of the input and output is provided in each tool's folder.

## Developer's Guide
Tools in this repositoy use different build systems. EQHazard is a java application and is built using Apache Ant. Eclipse is the prefered IDE to build EQHazard. However, the app can also be built using the shell command (path of the build file is relative to EQHazard folder):
```shell
ant -buildfile  ./ant/EQHazardBuild.xml
```

SimulateIM and SelectRecord are C++ applications that can be built using cmake. SimulateIM uses random number generation feature introduced in the C++11 standard. So, a C++11 compliant compiler is needed to build SimulateIM.

### Build SimulateIM and SelectRecord
It is preferred to use an out-of-source build, to keep the source code separated from the binary files. To achive that, this guide assumes a folder called build exists in the parent folder and is used to build the executable files from the source.

#### Unix-like operating system
Building on *nix was tested using both the GCC and intel compilers. The following command can be used to build the SimulateIM and SelectRecord for debugging:
```
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```
For release build:
```
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
```

#### Windows
Building on windows was tested using Microsoft Visual Studio. Cmake is used to generate visual studio projects as follows:
```
cmake .. -G "Visual Studio 15 2017 Win64"
```

The generated visual sudio project can then be used to build, run and debug the tools. CMake generators for other versions of Visual studio can be used as well.

#### Stampede2 HPC
The following command can be used to build the tools on TACC Stampede2 HPC using GCC:
```
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ..
make
```

To build using intel compiler:
```
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=icc -D CMAKE_CXX_COMPILER=icc ..
make
```
## Acknowledgments
The SimCenter would like to acknowledge Prof. Jack Baker for his discussions regarding existing hazard tools, ground motion simulation, selection and scaling.

## License

Please check the readme and license files, and third party license files for each tool and in the root folder.



