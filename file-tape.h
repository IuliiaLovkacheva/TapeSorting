#ifndef FILETAPE_H
#define FILETAPE_H

#include <fstream>
#include "tape.h"
#include "delays.h"

class FileTape : public Tape {
public:
    enum class Mode
    {
        OPEN_EXISTING,
        CREATE_IF_MISSING,
        CREATE_OR_OVERWRITE
    };
    FileTape(std::string filename, size_t length, const Delays& delays, Mode mode);
    virtual ~FileTape() = default;
    void back() override;
    void rewind() override;
    int32_t read() override;
    void write(int32_t value) override;
    void forward() override;
    size_t getLength() override;
    size_t getIndex() override;
private:
    const std::string filename_;
    const std::string nonIntegerValueError_;
    const std::string notEnoughElementsError_;
    const std::string tooManyElementsError_;
    const size_t length_;
    size_t index_;
    const Delays& delays_;
    static void sleep(int time);
    void createIfMissing();
    void createOrOverwrite();
    void createFile();
    void openExisting();
    void throwIfEmpty(std::ifstream& file);
    void fillWithZeros(std::ofstream& newFile);
};


#endif
