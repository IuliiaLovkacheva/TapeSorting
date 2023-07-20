#ifndef VECTOR_COUNTS_COLLECTION_H
#define VECTOR_COUNTS_COLLECTION_H

#include <vector>
#include "counts-collection.h"

class VectorCountsCollection : public CountsCollection {
public:
    explicit VectorCountsCollection(const size_t& size);
    void increment(const int& value) override;
    size_t get(const int& value) const override;
    void reset() override;
private:
    std::vector<size_t> counts_;
};

#endif
