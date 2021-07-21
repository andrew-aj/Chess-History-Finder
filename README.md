To build this project using CMake the easiest is by using environment variables.
Create a new system variable called CMAKE_PREFIX_PATH and add the directory which 
SFML or any library used is installed onto. For example, I have SFML installed to
C:\CPP_Libs\SFML-2.5.1 with that folder containing bin, include, lib, etc. Once you
do this, restart CLion and there should be no trouble with CMake finding the library
directory.