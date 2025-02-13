#ifndef LABELS_H
#define LABELS_H

#pragma once

#include <boost/dynamic_bitset.hpp>
#include <cstdint>
#include <list>  // Changed from vector

struct Label {
    const int64_t vertex;
    const double cost;
    const Label* previous;
    boost::dynamic_bitset<> forbidden;

    bool forbiddenDominates(const Label* other) const {
        return (other->forbidden & this->forbidden) == this->forbidden;
    }
};

class LabelBucket {
   public:
    LabelBucket(int64_t vertex);
    ~LabelBucket();

    // add the corresponding label if it has space or if it is better than any other label
    void add(double cost, boost::dynamic_bitset<> forbidden, const Label* previous);

    const std::list<Label*>& getLabels() const { return this->labels; }

    const Label* getBetter() const { return this->labels.front(); }

    int64_t size() const { return this->labels.size(); }
    void clear();
    bool empty() const { return this->labels.empty(); }

   private:
    const int64_t vertex;

    std::list<Label*> labels;  // Changed from vector
};

#endif
