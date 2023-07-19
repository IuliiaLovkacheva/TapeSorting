#ifndef SORT_H
#define SORT_H

#include <cstddef>
#include "tape.h"
#include "delays.h"

class Sort {
public:
    void operator()(Tape& inputTape, Tape& outputTape, const size_t& memoryLimit, const Delays& delays);
private:
    static const size_t COUNTS_LENGTH = 19;
    static const size_t BYTES_IN_INT32 = 4;
    static const size_t MAX_DIGIT = 10;
    static const std::string TEMP_DIRECTORY_NAME;
    static const std::string COUNTS_TAPE_FILENAME;
    static const std::string TEMP_TAPE_FILENAME;
    static int nthDigit(int32_t value, size_t n);
};

#endif
