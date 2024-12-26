#ifndef PRICING_H
#define PRICING_H

#pragma once

#include <vector>

#include "Instance.h"

class Pricing {
   public:
    Pricing(const Instance& instance);

    void solve(const std::vector<double>& prices);

    double getMinCost(int64_t d, int64_t v) const { return this->minCost[d][v]; }

    int64_t getBefore(int64_t d, int64_t v) const { return this->before[d][v]; }

   private:
    const Instance& instance;
    const int64_t V;  // Number of vertices (depot + V-1 customers)
    const int64_t C;  // Vehicle capacity

    // minCost[d][v] = least costly walk that reaches vertex v using total demand exactly d
    std::vector<std::vector<double> > minCost;

    // before[d][v] = vertex visited before v in least costly walk
    std::vector<std::vector<int64_t> > before;

    void initialize();
};

#endif
