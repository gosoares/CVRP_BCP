#ifndef LABELS_H
#define LABELS_H

#pragma once

#include <cstdint>
#include <vector>

struct Label {
    const int64_t vertex;
    const double cost;
    const Label* previous;
};

class Labels {
   public:
    Labels(int64_t vertex, int64_t maxLabels = 2);
    ~Labels();

    // add the corresponding label if it has space or if it is better than any other label
    void add(double cost, const Label* previous);

    const Label* operator[](int64_t index) const { return this->labels[index]; }

    const Label* getBetter() const { return this->labels.front(); }

    const Label* getBestLabelToExtendTo(int64_t vertex) const;

    int64_t size() const { return this->labels.size(); }
    void clear();
    bool empty() const { return this->labels.empty(); }

   private:
    const int64_t vertex;
    const int64_t maxLabels;

    std::vector<Label*> labels;
};

#endif
