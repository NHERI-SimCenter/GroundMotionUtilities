# SimulateGM

This application will simulate ground motion intensity measures(IMs) given the mean and standard deviation. For simulation of geographically distributed systems, spatial correlation can be established among the simulated IMs for different sites across a region. The implementation of ground motion correlation is based on Jayaram and Baker(2009) method [1].

One application of this tool is to obtain simulations of spectral accelerations(SAs) at different sites across a region to use it for ground motion selection and scaling. The inputs for this tool, such as mean and standard deviations of SAs can be obtained using EQScenario tool.

For the regional simulations, the tool can be used to obtain a single simulation of the SAs at each site, to be used to select a single ground motion record.

## Usage
SimulateGM can be executed as follows:

```shell
SimulateGM Config.json Output.json
```

SimulateGM requires an input config file that specifies a ground motions file, the number of simulations to be performed and a flag to determine if spatial correlation will be used. An example input is:

```json
{
    "GroundMotions":{
        "File":"Scenario1_SA.json"
    },
    "NumSimulations": 10,
    "SpatialCorrelation": false
}
```
The ground motions file need to have the mean and standard deviations of the ground motion IM. In addition, the inter-event and intra-event standard deviations are required to simulate spatially-correlated ground motions. If the IM used is spectral acceleration, then an array of periods needs to be specified, as well. A sample ground motion file will be as follows:

```json
{
  "Periods": [0.05, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0],
  "GroundMotions": [
    {
      "Location": {
        "Latitude": 37.0,
        "Longitude": -122.6
      },
      "SA": {
        "Mean": [
          -2.5464460423760964,
          -2.1644966528792438,
          -1.6850676522191543,
          -1.684364386543265,
          -1.9716359492745434,
          -2.4386205322240557,
          -3.045528969081932,
          -4.209327188902733
        ],
        "TotalStdDev": [
          0.658391980510091,
          0.7007160881389554,
          0.5826808732059086,
          0.583692556060123,
          0.6744101126169446,
          0.6930663748877159,
          0.7064764681148269,
          0.6495667787071626
        ],
        "InterEvStdDev": [
          0.426,
          0.458,
          0.309,
          0.224,
          0.298,
          0.329,
          0.335,
          0.239
        ],
        "IntraEvStdDev": [
          0.502,
          0.5303197489974896,
          0.49400000000000005,
          0.5389999999999999,
          0.605,
          0.61,
          0.622,
          0.604
        ]
      }
    },...]
}

```
The output file will include the same ground motions included in the input, in addition to a simulations array, as follows:
```json

{
"Periods": [...],
"GroundMotions": [
    {
        "Location": {...},
        "SiteData": [...],
        "SA": {
            "Mean": [...],
            "TotalStdDev": [...],
            "InterEvStdDev": [...],
            "IntraEvStdDev": [...],
            "Simulations": [
                [
                    -2.234838372135673,
                    -2.154264271124297,
                    -1.631265200432125,
                    -1.0837243004092927,
                    -2.1443199669329489,
                    -1.6185189185624763,
                    -3.524066429144307,
                    -2.8773682552372907
                ]
            ]
        }
    },...]
}
```

## Example 1
### Single simulation of spatially-correlated response spectra for a grid of sites in SF Bay Area
In this example, the ground motion is simulated for a grid of sites in SF Bay Area (using outputs from example 2 in [EQScenario documentation](../EQScenario/Readme.md)). The input file is as follows:

```json
{
    "GroundMotions":{
        "File":"Scenario2_SA.json"
    },
    "NumSimulations": 1,
    "SpatialCorrelation": true
}
```
To run this simulation, in the examples folder execute this command:
```shell
SimulateGM SimGMConfig.json Scenario2_SA_Sim.json
```
The ground motion file output from EQScenario ([Scenario2_SA.json](examples/Scenario2_SA.json)) is specified in the input config to be used for the simulation. The output of the simulation is shown in this file ([Scenario2_SA_Sim.json](examples/Scenario2_SA_Sim.json)) in the examples folder.

## References
[1] Jayaram N. and Baker J.W. (2009), "Correlation model for spatially-distributed ground-motion intensities," Earthquake Engineering and Structural Dynamics, 38(15), 1687-1708.