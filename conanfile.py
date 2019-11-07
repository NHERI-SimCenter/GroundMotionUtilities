from conans import ConanFile, CMake, tools
import os


class GMUConan(ConanFile):
    name = "GMU"
    description = "Ground Motion Utilities"
    topics = ("Earthquake", "Ground Motion", "Hazards")
    url = "https://github.com/NHERI-SimCenter/GroundMotionUtilities"
    homepage = "https://github.com/NHERI-SimCenter/GroundMotionUtilities/blob/master/Readme.md"
    license = "BSD"
    version = "0.2.0"

    # Options may need to change depending on the packaged library
    settings = "os", "arch", "compiler", "build_type"

    _build_subfolder = "build"
    _source_subfolder = "."
    exports_sources = "*"

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(build_folder=self._build_subfolder)
        return cmake

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()
        cmake.install()
        
        self.run("mvn -f EQHazard/pom.xml package")

    def package(self):
        self.copy(pattern="EQHazard/target/EQHazard.jar", dst="bin", keep_path=False)
        self.copy(pattern="build/bin/*", dst="bin", keep_path=False)
        self.copy(pattern="SelectRecord/examples/NGAWest2.csv", dst="data", keep_path=False)

