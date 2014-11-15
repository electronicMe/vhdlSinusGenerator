vhdlSinusGenerator
==================

VHDL Sinus Generator is a program invented for __TheMedusaProject__.

The software generates a VHDL array which contains the binary data of a sinus.


Usage
-----

Compile the program with the commands:

    cmake CMakeLists.txt
    make


Then run the program like this:

    ./SinusGenerator -n 1 -p 100 -r 10 -v


### Options ###

* `-h`, `--help` Display the help text
* `-n`, `--numberOfSines` The number of sines to produce
* `-p`, `--pointsPerSine` The number of points per sine to generate
* `-r`, `--resolution` The resolution in bit of the output