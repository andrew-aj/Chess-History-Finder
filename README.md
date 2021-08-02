To build this project using CMake, the easiest way is by using environment variables.
Create a new system/user variable called CMAKE_PREFIX_PATH and add the directory which 
SFML or any library used is installed onto. For example, I have SFML installed to
C:\CPP_Libs\SFML-2.5.1 with that folder containing bin, include, lib, etc. Once you
do this, restart CLion and there should be no trouble with CMake finding the library
directory.

In this program there are certain standards that we should keep to ensure readability and to
decrease possible confusion. The defines file will contain shorted names for data
types like 64 bit unsigned integers and others as we see fit.

The project will use namespace Chess for all of created classes and data types 
to separate them from the standard library.

Use camel case for naming variables. Use pascal case for naming classes.

For formatting the pgn files into a usable format, we use pgn-extract which is licensed
under the GNU General Public License https://www.cs.kent.ac.uk/people/staff/djb/pgn-extract/.

For using pgn-extract use the following command:
```
pgn-extract -oout.pgn -Whalg -w99999 --nochecks --nocomments --nonags --novars --quiet test.pgn
```

For chess piece assets, we modified the pieces from this open source chess asset pack:
https://opengameart.org/content/chess-pieces-and-board-squares

The font used for outputting moves in the gui is Times Newer Roman, and open source font:
https://timesnewerroman.com/

This free to use flag icon was used for the castling rights buttons:
https://pixabay.com/images/id-1636453/