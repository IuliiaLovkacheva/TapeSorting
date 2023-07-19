#ifndef TAPE_COUNTS_COLLECTION_H
#define TAPE_COUNTS_COLLECTION_H

#include <string>
#include <memory>
#include "counts-collection.h"
#include "tape.h"

class TapeCountsCollection : public CountsCollection {
public:
    using tape_ptr = std::unique_ptr<Tape>;
    explicit TapeCountsCollection(tape_ptr tape);
    void increment(const int& value) override;
    size_t get(const int& value) const override;

    void reset() override;

private:
    std::unique_ptr<Tape> counts_;
    void moveTo(int position) const;
};

#endif
