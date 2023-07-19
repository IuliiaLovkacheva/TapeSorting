#include "vector-counts-collection.h"

void VectorCountsCollection::increment(const int& value) {
    ++counts_[value + OFFSET];
}

size_t VectorCountsCollection::get(const int& value) const {
    return counts_[value + OFFSET];
}

VectorCountsCollection::VectorCountsCollection(const size_t& size):
    counts_(std::vector<size_t>(size))
{}

void VectorCountsCollection::reset() {
    std::fill(counts_.begin(), counts_.end(), 0);
}
