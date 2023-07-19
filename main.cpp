#include <iostream>
#include <string>
#include <vector>
#include "file-tape.h"
#include "sort.h"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::invalid_argument("The input and output file names are not provided");
        }
        if (argc < 3) {
            throw std::invalid_argument("The output file name is not provided");
        }

        std::string inFileName = argv[1];
        std::string outFileName = argv[2];

        int tapeLength = 0;
        int memoryLimit = 0;

        std::cout << "Specify the length of the input and output tapes (N) => ";
        std::cin >> tapeLength;
        if (std::cin.fail()) {
            throw std::invalid_argument("The tape length (N) must be an integer");
        }
        if (tapeLength <= 0) {
            throw std::invalid_argument("The tape length (N) must be positive");
        }

        std::cout << "Specify the memory limit in bytes (M) => ";
        std::cin >> memoryLimit;

        if (std::cin.fail()) {
            throw std::invalid_argument("The memory limit (M) must be an integer");
        }
        if (memoryLimit <= 0) {
            // TODO **must** it be positive???
            throw std::invalid_argument("The memory limit (M) must be positive");
        }

        const std::string DELAYS_FILENAME = "config";
        Delays delays = {0};
        std::ifstream delaysFile(DELAYS_FILENAME);
        if (!delaysFile) {
            delaysFile.close();
            throw std::runtime_error("The delay configuration file " + DELAYS_FILENAME + "could not be opened");
        }
        delaysFile >> delays;
        if (delaysFile.fail()) {
            throw std::invalid_argument("Error while reading delays from the configuration file");
        }
        delaysFile >> std::ws;
        if (!delaysFile.eof()) {
            throw std::invalid_argument("The delay configuration file should only contain the delays");
        }
        delaysFile.close();

        std::cout << delays.read << ' ' << delays.write << ' ' << delays.move << ' ' << delays.rewind << '\n';
        FileTape inFileTape(inFileName, tapeLength, delays);
        FileTape outFileTape(outFileName, tapeLength, delays);
        Sort sort;
        sort(inFileTape, outFileTape, memoryLimit, delays);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
