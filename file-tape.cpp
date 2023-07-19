#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include "file-tape.h"

void print_state (const std::ios& stream) {
    std::cout << " good()=" << stream.good();
    std::cout << " eof()=" << stream.eof();
    std::cout << " fail()=" << stream.fail();
    std::cout << " bad()=" << stream.bad();
}

// TODO getLength & getIndex??
// TODO end? some way to check that this is the last "cell"?
FileTape::FileTape(std::string filename, size_t length, const Delays& delays, Mode mode) :
    length_(length),
    index_(0),
    delays_(delays),
    filename_(std::move(filename))
{
    switch (mode) {
        case Mode::CREATE_IF_MISSING:


    }
    createFileIfMissing();
}


void FileTape::forward() {
    sleep(delays_.move);
    ++index_;
}

void FileTape::back() {
    sleep(delays_.move);
    --index_;
}

void FileTape::rewind() {
    sleep(delays_.rewind);
    index_ = 0;

}

std::int32_t FileTape::read() {
    // TODO if the file doesn't exist, create a zero-filled one
    sleep(delays_.read);
    std::fstream file(filename_);
    int elementCounter = 0;
    int value = 0;
    try {

        while (elementCounter <= index_ && elementCounter < length_) {
            file >> value;
            if (file.fail()) {
                throw std::runtime_error("The file contains a non-integer value");
            }
            ++elementCounter;
        }
        // TODO throw if not enough elements?
//        if (elementCounter < index_) {
//            throw std::runtime_error("The file doesn't ")
//        }
        // TODO should this be moved somewhere?
        file.close();
    // TODO could there be other exceptions?
    } catch (const std::runtime_error& e) {
        file.close();
        throw e;
    }
    return value;
}

void FileTape::write(std::int32_t value) {
    sleep(delays_.write);
    std::vector<int> storage(length_);

    std::ifstream inFile(filename_);
    int temp;
    int tempIndex = 0;
    // TODO check if empty/doesn't match the length, pad w/ zeroes in that case (or throw?)
    while (!inFile.eof()) {
        inFile >> temp;
        if (inFile.fail() && !inFile.eof()) {
            inFile.close();
            throw std::runtime_error("The file contains a non-integer value");
        }
        if (!inFile.eof()) {
            assert(tempIndex < length_);
            storage[tempIndex] = temp;
            ++tempIndex;
        }
    }
//    print_state(inFile);
    inFile.close();

    // TODO could be out of range if the file is invalid
    assert(index_ < length_);
    storage[index_] = value;

    std::ofstream outFile(filename_, std::ios::trunc);
    std::ostream_iterator<int> outIterator(outFile, "\n");
    std::copy(storage.begin(), storage.end(), outIterator);
    // TODO could errors occur during output
    outFile << std::flush;
    outFile.close();
}

void FileTape::sleep(int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

size_t FileTape::getLength() {
    return length_;
}

size_t FileTape::getIndex() {
    return index_;
}

void FileTape::createFileIfMissing() {
    std::ifstream file(filename_);
    // TODO file.good()???
    if (file) {
        file.seekg(0, std::ios::end);
        if (file.tellg() == 0) {
            file.close();
            throw std::runtime_error("The file is empty\n");
        }
    } else {
        // The file doesn't exist or couldn't be opened, so create it and write a zero to it
        std::ofstream newFile(filename_);
        if (newFile) {
            for (size_t i = 0; i < length_; ++i) {
                newFile << 0 << '\n';
            }
        } else {
            file.close();
            newFile.close();
            throw std::runtime_error("Could not create or open the file\n");
        }
        newFile.close();
    }
    file.close();
}

