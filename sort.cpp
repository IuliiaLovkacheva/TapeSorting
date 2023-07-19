#include <iostream>
#include <filesystem>
#include "sort.h"
#include "tape.h"
#include "file-tape.h"
#include "utils.h"
#include "tape-counts-collection.h"
#include "vector-counts-collection.h"


const std::string Sort::TEMP_DIRECTORY_NAME = "tmp";
const std::string Sort::COUNTS_TAPE_FILENAME = TEMP_DIRECTORY_NAME + "/counts.txt";
const std::string Sort::TEMP_TAPE_FILENAME = TEMP_DIRECTORY_NAME + "/temp.txt";

void Sort::operator()(Tape& inputTape, Tape& outputTape, const size_t& memoryLimit, const Delays& delays) {
    if (inputTape.getLength() != outputTape.getLength()) {
        throw std::invalid_argument("The input and output tape lengths don't match");
    }

    createDirectoryIfMissing(TEMP_DIRECTORY_NAME);

    std::unique_ptr<CountsCollection> counts;
    if (memoryLimit < COUNTS_LENGTH * BYTES_IN_INT32) {
        counts = std::make_unique<TapeCountsCollection>(
                std::make_unique<FileTape>(COUNTS_TAPE_FILENAME, COUNTS_LENGTH, delays,
                                           FileTape::Mode::CREATE_OR_OVERWRITE));
    } else {
        counts = std::make_unique<VectorCountsCollection>(COUNTS_LENGTH);
    }


    // TODO place a part in the RAM (if we have some)
    FileTape tempTape(TEMP_TAPE_FILENAME, inputTape.getLength(), delays, FileTape::Mode::CREATE_OR_OVERWRITE);
    // TODO all the values take up RAM => should provide an option of storing them on tapes
    bool writeToTemp = true;
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
            counts->increment(nthDigit(value, digitNumber));
            readingTape->forward();
        }
        readingTape->rewind();
        for (int i = 0; i < COUNTS_LENGTH; ++i) {
            int occurrenceCounter = 0;
            while (occurrenceCounter < counts->get(i - CountsCollection::OFFSET)) {
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

int Sort::nthDigit(std::int32_t value, size_t n) {
    while (n > 0) {
        value /= 10;
        --n;
    }
    return value % 10;
}
