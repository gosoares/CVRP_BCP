#ifndef PRICING_H
#define PRICING_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <boost/dynamic_bitset.hpp>
#include <cstdint>
#include <vector>

#include "Instance.h"
#include "Labels.h"

class Pricing {
   public:
    Pricing(const Instance& instance);

    double solve(const IloNumArray& prices);

    const std::vector<std::vector<int64_t>>& getSolutionQPaths() const { return this->solutionQPaths; }

   private:
    const Instance& instance;
    const int64_t V;  // Number of vertices (depot + V-1 customers)
    const int64_t C;  // Vehicle capacity
    std::vector<boost::dynamic_bitset<>> ngSets;

    // M[d][v] = least costly walks that reaches vertex v using total demand exactly d
    std::vector<std::vector<LabelBucket>> M;

    std::vector<std::vector<int64_t>> solutionQPaths;  // qpaths given by the edges in it

    void initialize();
};

#endif
