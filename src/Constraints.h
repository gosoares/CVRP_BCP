#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <vector>

class Constraints {
   private:
    IloRangeArray constraints;
    std::vector<std::vector<int64_t> > edgeConstraints;  // edgeConstraints[e] = constraints that has edge e

   public:
    Constraints(IloEnv& env, int64_t nEdges);
    ~Constraints();

    void add(const IloRange& constraint, const std::vector<int64_t>& edges);

    IloRange& operator[](int64_t i);
    IloRange operator[](int64_t i) const;

    const std::vector<int64_t>& forEdge(int64_t e) const;
};

#endif
