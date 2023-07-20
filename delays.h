#ifndef DELAYS_H
#define DELAYS_H
#include <iosfwd>

struct Delays {
    int read;
    int write;
    int rewind;
    int move;
    friend std::istream& operator>>(std::istream& is, Delays& obj);
};

#endif
