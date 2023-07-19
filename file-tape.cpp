#include "file-tape.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <string>
#include <vector>

FileTape::FileTape(std::string filename, size_t length, const Delays& delays, Mode mode) :
        length_(length),
        index_(0),
        delays_(delays),
        filename_(std::move(filename)),
        nonIntegerValueError_("The \"" + filename + "\n file contains a non-integer value"),
        notEnoughElementsError_("\"" + filename_ + "\" doesn't contain enough elements"),
        tooManyElementsError_("\"" + filename_ + "\" contains too many elements") {
    switch (mode) {
        case Mode::OPEN_EXISTING:
            openExisting();
            break;
        case Mode::CREATE_OR_OVERWRITE:
            createOrOverwrite();
            break;
        case Mode::CREATE_IF_MISSING:
            createIfMissing();
            break;
    }
}


void FileTape::forward() {
    if (index_ == length_) {
        throw std::out_of_range("Can't move to a position after the end of the tape");
    }
    sleep(delays_.move);
    ++index_;
}

void FileTape::back() {
    if (index_ == 0) {
        throw std::out_of_range("Can't move to a position before the start of the tape");
    }
    sleep(delays_.move);
    --index_;
}

void FileTape::rewind() {
    sleep(delays_.rewind);
    index_ = 0;

}

std::int32_t FileTape::read() {
    sleep(delays_.read);
    std::fstream file(filename_);
    if (!file) {
        file.close();
        throw std::runtime_error("\"" + filename_ + "\" could not be opened for reading");
    }
    int elementCounter = 0;
    int value = 0;
    while (elementCounter <= index_ && elementCounter < length_) {
        if (file.eof()) {
            file.close();
            throw std::runtime_error(notEnoughElementsError_);
        }
        file >> value >> std::ws;
        if (file.fail()) {
            file.close();
            throw std::runtime_error(nonIntegerValueError_);
        }
        ++elementCounter;
    }
    file >> std::ws;
    if (elementCounter == length_ && !file.eof()) {
        file.close();
        throw std::runtime_error(tooManyElementsError_);
    }
    file.close();
    return value;
}

void FileTape::write(std::int32_t value) {
    sleep(delays_.write);
    std::vector<int> values(length_);
    std::ifstream inFile(filename_);
    const std::string notOpenedMessage = "\"" + filename_ + "\" could not be opened for writing";
    if (!inFile) {
        inFile.close();
        throw std::runtime_error(notOpenedMessage);
    }
    int temp;
    int tempIndex = 0;
    while (!inFile.eof()) {
        inFile >> temp;
        if (inFile.fail() && !inFile.eof()) {
            inFile.close();
            throw std::runtime_error(nonIntegerValueError_);
        }
        if (!inFile.eof()) {
            if (tempIndex >= length_) {
                inFile.close();
                throw std::runtime_error(tooManyElementsError_);
            }
            values[tempIndex] = temp;
            ++tempIndex;
        }
    }
    if (tempIndex < length_) {
        inFile.close();
        throw std::runtime_error(notEnoughElementsError_);
    }
    inFile.close();

    values[index_] = value;

    std::ofstream outFile(filename_, std::ios::trunc);
    if (!outFile) {
        inFile.close();
        throw std::runtime_error(notOpenedMessage);
    }
    std::ostream_iterator<int> outIterator(outFile, "\n");
    std::copy(values.begin(), values.end(), outIterator);
    outFile.close();
}

void FileTape::sleep(int time) {
    std::this_thread::sleep_for(std::chrono::microseconds(time));
}

size_t FileTape::getLength() {
    return length_;
}

size_t FileTape::getIndex() {
    return index_;
}

void FileTape::createIfMissing() {
    std::ifstream file(filename_);
    if (file) {
        throwIfEmpty(file);
        file.close();
        return;
    }
    createFile();
}

void FileTape::openExisting() {
    std::ifstream file(filename_);
    if (file) {
        throwIfEmpty(file);
    } else {
        file.close();
        throw std::runtime_error("The file \"" + filename_ + "\" could not be opened");
    }
    file.close();
}

void FileTape::throwIfEmpty(std::ifstream& file) {
    file.seekg(0, std::ios::end);
    if (file.tellg() == 0) {
        file.close();
        throw std::runtime_error("The file \"" + filename_ + "\" is empty\n");
    }
}

void FileTape::createOrOverwrite() {
    std::ofstream file(filename_, std::ios::trunc);
    if (!file) {
        file.close();
        throw std::runtime_error("Could not create or open the file \"" + filename_ + "\"");
    }
    fillWithZeros(file);
    file.close();
}

void FileTape::createFile() {
    std::ofstream newFile(filename_);
    if (newFile) {
        fillWithZeros(newFile);
    } else {
        newFile.close();
        throw std::runtime_error("Could not create or open the file \"" + filename_ + "\"");
    }
    newFile.close();
}

void FileTape::fillWithZeros(std::ofstream& file) {
    for (size_t i = 0; i < length_; ++i) {
        file << 0 << '\n';
    }
}

