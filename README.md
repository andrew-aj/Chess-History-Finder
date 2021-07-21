To build this project using CMake the easiest is by using environment variables.
Create a new system variable called CMAKE_PREFIX_PATH and add the directory which 
SFML or any library used is installed onto. For example, I have SFML installed to
C:\CPP_Libs\SFML-2.5.1 with that folder containing bin, include, lib, etc. Once you
do this, restart CLion and there should be no trouble with CMake finding the library
directory.

In this there are certain standards we should keep to ensure readability and to
decrease possible confusion. The defines file will contain shorted names for data
types like 64 bit unsigned integers and others as we see fit.

The project will use namespace Chess for all of created classes and data types 
to separate them from the standard library.

Use camel case for naming variables. Use pascal case for naming classes.