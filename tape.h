#ifndef TAPE_H
#define TAPE_H


class Tape {
public:
    virtual ~Tape() = default;
    virtual void forward() = 0;
    virtual void back() = 0;
    virtual void rewind() = 0;
    virtual int32_t read() = 0;
    virtual void write(int32_t value) = 0;
    virtual size_t getLength() = 0;
    virtual size_t getIndex() = 0;
};


#endif
