#include <iostream>
#include <filesystem>
#include "sort.h"
#include "tape.h"
#include "file-tape.h"
#include "tape-counts-collection.h"
#include "vector-counts-collection.h"


const std::string Sort::TEMP_DIRECTORY_NAME = "tmp";
const std::string Sort::COUNTS_TAPE_FILENAME = TEMP_DIRECTORY_NAME + "/counts.txt";
const std::string Sort::TEMP_TAPE_FILENAME = TEMP_DIRECTORY_NAME + "/temp.txt";

void Sort::operator()(Tape &inputTape, Tape &outputTape, const size_t &memoryLimit, const Delays &delays) {
    if (inputTape.getLength() != outputTape.getLength()) {
        throw std::invalid_argument("The input and output tape lengths don't match");
    }

    createTempIfMissing();

    std::unique_ptr<CountsCollection> counts;
    if (memoryLimit < COUNTS_LENGTH) {
        counts = std::make_unique<TapeCountsCollection>(
                std::make_unique<FileTape>(COUNTS_TAPE_FILENAME, COUNTS_LENGTH, delays, FileTape::Mode::CREATE_OR_OVERWRITE));
    } else {
        counts = std::make_unique<VectorCountsCollection>(COUNTS_LENGTH);
    }


    // TODO place a part in the RAM (if we have some)
    FileTape tempTape(TEMP_TAPE_FILENAME, inputTape.getLength(), delays, FileTape::Mode::CREATE_OR_OVERWRITE);
    // TODO all the values take up RAM => should provide an option of storing them on tapes
    bool writeToTemp = true;
    size_t MAX_DIGIT = 10;
    Tape* readingTape = &inputTape;
    Tape* writingTape = &tempTape;
    for (size_t digitNumber = 0; digitNumber < MAX_DIGIT; ++digitNumber) {
        // subroutine(wt, rt)
        // if wtt
        //      s(temp, out)
        // else
        //      s(out, temp)
        // //^this doesn't account for the input tape
        // adding the digit counting would require some restructuring
        for (size_t i = 0; i < readingTape->getLength(); ++i) {
            int32_t value = readingTape->read();
            int digit = nthDigit(value, digitNumber);
            counts->increment(digit);
            readingTape->forward();
        }
        readingTape->rewind();
        for (int i = 0; i < COUNTS_LENGTH; ++i) {
            size_t count = counts->get(i - CountsCollection::OFFSET);
            int occurrenceCounter = 0;
            while (occurrenceCounter < count) {
                int32_t value = readingTape->read();
                if (nthDigit(value, digitNumber) == (i - CountsCollection::OFFSET)) {
                    writingTape->write(value);
                    writingTape->forward();
                    ++occurrenceCounter;
                }
                readingTape->forward();
            }
            if (readingTape->getIndex() > 0) {
                readingTape->rewind();
            }
        }
        readingTape->rewind();
        writingTape->rewind();
        if (writeToTemp) {
            writeToTemp = false;
            readingTape = &tempTape;
            writingTape = &outputTape;
        } else {
            writeToTemp = true;
            readingTape = &outputTape;
            writingTape = &tempTape;
        }
        counts->reset();
    }

}

inline void Sort::createTempIfMissing() {
    if (!std::filesystem::is_directory(TEMP_DIRECTORY_NAME) || !std::filesystem::exists(TEMP_DIRECTORY_NAME)) { // Check if src folder exists
        std::filesystem::create_directory(TEMP_DIRECTORY_NAME);
    }
}

inline int Sort::nthDigit(std::int32_t value, size_t n) {
    while (n > 0) {
        value /= 10;
        --n;
    }
    int digit = value % 10;
    return digit;
}
