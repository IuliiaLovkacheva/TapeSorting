#include "tape-counts-collection.h"

void TapeCountsCollection::increment(const int& value) {
    moveTo(value + OFFSET);
    int oldCount = counts_->read();
    counts_->write(oldCount + 1);
}

size_t TapeCountsCollection::get(const int& value) const {
    moveTo(value + OFFSET);
    return counts_->read();
}

TapeCountsCollection::TapeCountsCollection(TapeCountsCollection::tape_ptr tape) :
        counts_(std::move(tape)) {}

void TapeCountsCollection::moveTo(int position) const {
    if (std::abs(static_cast<int>(position - counts_->getIndex())) <= position) {
        if (counts_->getIndex() > position) {
            while (counts_->getIndex() != position) {
                counts_->back();
            }
            return;
        }
        moveForwardToPosition(position);
        return;
    }
    counts_->rewind();
    moveForwardToPosition(position);
}

void TapeCountsCollection::moveForwardToPosition(int position) const {
    while (counts_->getIndex() != position) {
        counts_->forward();
    }
}

void TapeCountsCollection::reset() {
    size_t initialIndex = counts_->getIndex();
    while (counts_->getIndex() < counts_->getLength()) {
        counts_->write(0);
        counts_->forward();
    }
    if (initialIndex > 0) {
        counts_->rewind();
        while (counts_->getIndex() < initialIndex) {
            counts_->write(0);
            counts_->forward();
        }
    }
}
