# Simcenter EQ Ground Motion Tools
This repository contains a suite of tools that are developed by the SimCenter to facilitate the use of seismic hazard analysis(SHA) for different research applications.

The tools in this repository are organized in following folders:
1. [EQScenario](EQScenario/Readme.md)
2. [SimulateGM](SimulateGM/Readme.md)
3. [SelectGM](SelectGM/Readme.md)

## User's Guide
The tools provided in this repository are console applications. Each tool requires an input json file and outputs one or more json files. Sample usage of each tool is printed on the screen when the applications are executed without arguments. In addition, documentation of the input and output is provided in each tool's folder.

## Developer's Guide
Tools in this repositoy use different build systems. EQScenario is a java application and is built using Apache Ant. Eclipse is the prefered IDE to build EQScenario.

SimulateGM and SelectGM are C++ applications that can be built using cmake. SimulateGM uses random number generation feature introduced in the C++11 standard. So, a C++11 compliant compiler is needed to build SimulateGM.

### Build SimulateGM and SelectGM
It is preferred to use an out-of-source build, to keep the source code separated from the binary files. To achive that, this guide assumes a folder called build exists in the parent folder and is used to build the executable files from the source.

#### Unix-based operating system
Building on *nix was tested using both the GCC and intel compilers. The following command can be used to build the SimulateGM and SelectGM for debugging:
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



