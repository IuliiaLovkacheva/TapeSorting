#ifndef DELAY_DATA_H
#define DELAY_DATA_H
#include <iosfwd>

struct Delays {
    int read;
    int write;
    int rewind;
    int move;
    friend std::istream& operator>>(std::istream& is, Delays& obj);
};


#endif
