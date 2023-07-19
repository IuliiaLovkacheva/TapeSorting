#ifndef COUNTS_COLLECTION_H
#define COUNTS_COLLECTION_H
#include <cstdint>

class CountsCollection {
public:
    virtual ~CountsCollection() = default;
    static const int OFFSET = 9;
    virtual void increment(const int& value) = 0;
    virtual size_t get(const int& value) const = 0;
    virtual void reset() = 0;
};

#endif
