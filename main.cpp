//
//    main.cpp
//
//    Author: Sebastian Mach
//    Created on Nov. 14 2014
//

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <bitset>


#define PI2 6.28318530717958647692
#define MAXBITS 64



static void printUsage(char *programPath);
static int  generateSine(int numberOfSines, int pointsPerSine, int resolution, bool verbose, std::string outputPath);



int main(int argc, char ** argv)
{
    std::cout << "VHDL SinusGenerator by Sebastian Mach" << std::endl << std::endl;



    /*================================================================================================================*/
    /* PARAMETERS                                                                                                     */
    /*================================================================================================================*/

    unsigned int numberOfSines = 0;             // -n
    unsigned int pointsPerSine = 0;             // -p
    unsigned int resolution    = 0;             // -r
    bool         verbose       = false;         // -v
    std::string  outputPath    = "/dev/null";   // -o






    /*================================================================================================================*/
    /* PARSE PARAMETERS                                                                                               */
    /*================================================================================================================*/

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numberOfSines") == 0) {
            if (i + 1 < argc) {
                numberOfSines = atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pointsPerSine") == 0) {
            if (i + 1 < argc){
                pointsPerSine = atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--resolution") == 0) {
            if (i + 1 < argc){
                resolution = atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc){
                outputPath = std::string(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
            if (i + 1 < argc) {
                printUsage(argv[0]);
                return EXIT_SUCCESS;
            }
        }
    }






    /*================================================================================================================*/
    /* CHECK PARAMETERS TO BE VALID                                                                                   */
    /*================================================================================================================*/

    if (numberOfSines < 1)
    {
        std::cerr << "Invalid Value for numberOfSines" << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    else if (pointsPerSine < 1)
    {
        std::cerr << "Invalid Value for pointsPerSine" << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    else if (resolution < 1 || resolution > MAXBITS)
    {
        std::cerr << "Invalid Value for resolution (valid values are 1 to " << MAXBITS << ")" << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }






    /*================================================================================================================*/
    /* GENERATE SINE                                                                                                  */
    /*================================================================================================================*/

    return generateSine(numberOfSines, pointsPerSine, resolution, verbose, outputPath);
}



static void printUsage(char *programPath) {
    std::cout << "Usage: " << programPath << " <options>" << std::endl
              << "Options:" << std::endl
              << "\t-h,--help\t\t\t Show this message" << std::endl
              << "\t-n,--numberOfSines\t The number of sines to produce" << std::endl
              << "\t-p,--pointsPerSine\t The number of points per sine to generate" << std::endl
              << "\t-r,--resolution\t\t The resolution in bit of the output" << std::endl
              << std::endl;
}



static int generateSine(int numberOfSines, int pointsPerSine, int resolution, bool verbose, std::string outputPath)
{
    /*================================================================================================================*/
    /* OPEN OUTPUT FILE                                                                                               */
    /*================================================================================================================*/

    std::ofstream outputFile;

    outputFile.open(outputPath, std::ios::out);

    if (!outputFile.is_open())
    {
        std::cerr << "Error opening file at path \"" << outputPath << "\": " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }






    /*================================================================================================================*/
    /* GENERATE SINE VALUES                                                                                           */
    /*================================================================================================================*/


    std::stringstream startText;
    startText << "    type array_type is array (natural range <>) of std_logic_vector(" << (resolution - 1) << " downto 0);" << std::endl;
    startText << "    constant array_const : array_type := (" << std::endl;

    if (verbose) {
        std::cout << startText.str();
    }

    outputFile << startText.str();



    for (int i = 0; i < numberOfSines; i++)
    {
        for (double sinArg = 0; sinArg < PI2; sinArg += (PI2 / pointsPerSine))
        {
            double sineVal        = sin(sinArg);
            double matchedSineVal = sineVal;

            // CALCULATE MATCHED SINE VALUE
            matchedSineVal += 1;
            matchedSineVal /= 2;
            matchedSineVal *= ((2 << (resolution - 1)) - 1);

            std::bitset<MAXBITS> bits(matchedSineVal);
            std::string bitsString = bits.to_string<char,std::string::traits_type,std::string::allocator_type>();
            bitsString = bitsString.substr(MAXBITS - resolution, resolution);

            std::stringstream lineText;
            lineText << "        \"" << bitsString << "\"";


            if (sinArg + (PI2 / pointsPerSine) >= PI2)
                lineText << std::endl;
            else
                lineText << "," << std::endl;


            if (verbose) {
                std::cout << lineText.str();
            }

            outputFile << lineText.str();
        }
    }



    std::string endText = "    );";

    if (verbose) {
        std::cout << endText << std::endl;
    }

    outputFile << endText << std::endl;



    return EXIT_SUCCESS;
}