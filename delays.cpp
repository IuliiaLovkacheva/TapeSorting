#include "delays.h"
#include <iostream>

std::istream& operator>>(std::istream& is, Delays& obj) {
    // read obj from stream
    is >> obj.read >> obj.write >> obj.move >> obj.rewind;
    if (obj.read < 0 || obj.write < 0 || obj.move < 0 || obj.rewind < 0) {
        is.setstate(std::ios::failbit);
    }
    return is;
}
