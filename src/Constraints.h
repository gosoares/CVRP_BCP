#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <map>
#include <vector>

class Constraints {
   private:
    std::vector<IloRange> constraints;
    std::vector<std::vector<int64_t> > edgeConstraints;  // edgeConstraints[e] = constraints that has edge e

   public:
    Constraints(int64_t nEdges);
    ~Constraints();

    void add(const IloRange& constraint, const std::vector<int64_t>& edges);
    void addColumn(const IloNumVar& lambda, const std::map<int64_t, int64_t>& q);
    void clearFrom(IloModel& model);

    IloRange& operator[](int64_t i);
    const IloRange operator[](int64_t i) const;

    struct EdgeConstraintsIterator {
        EdgeConstraintsIterator(Constraints& constraints, int64_t edge, int64_t i = 0);

        int64_t id() const;
        const IloRange& constraint() const;

        const EdgeConstraintsIterator& operator*() const;
        const EdgeConstraintsIterator& operator++();
        bool operator!=(const EdgeConstraintsIterator& other);

        EdgeConstraintsIterator begin();
        EdgeConstraintsIterator end();

       private:
        Constraints& constraints;
        int64_t i;
        int64_t edge;
    };

    EdgeConstraintsIterator forEdge(int64_t e);
};

#endif
