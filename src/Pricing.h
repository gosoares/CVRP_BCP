#ifndef PRICING_H
#define PRICING_H

#pragma once

#include <vector>

#include "Instance.h"

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

    const Label* operator[](size_t index) const { return this->labels[index]; }

    const Label* getBetter() const { return this->labels.front(); }

    const Label* getBestLabelToExtendTo(int64_t vertex) const;

    size_t size() const { return this->labels.size(); }
    void clear();
    bool empty() const { return this->labels.empty(); }

   private:
    const int64_t vertex;
    const int64_t maxLabels;

    std::vector<Label*> labels;
};

class Pricing {
   public:
    Pricing(const Instance& instance);

    std::vector<std::vector<int64_t> > solve(const std::vector<double>& prices);

   private:
    const Instance& instance;
    const int64_t V;  // Number of vertices (depot + V-1 customers)
    const int64_t C;  // Vehicle capacity

    // M[d][v] = least costly walks that reaches vertex v using total demand exactly d
    std::vector<std::vector<Labels> > M;

    void initialize();
};

#endif
