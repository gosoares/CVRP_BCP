#ifndef NODE_H
#define NODE_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <cstdint>
#include <vector>

enum BranchDirection {
    BD_LEFT,  // x(cutSet(S)) == 2
    BD_RIGHT  // x(cutSet(S)) >= 4
};

struct BranchConstraint {
    const std::vector<int64_t> edges;  // edges in cutset(S)
    const BranchDirection direction;

    bool isLeft() const { return direction == BD_LEFT; }
    bool isRight() const { return direction == BD_RIGHT; }
};

enum NodeStatus { UNPROCESSED, BRANCHED, INTEGRAL, INFEASIBLE };

class Node {
   public:
    Node();
    Node(int64_t id, const Node& parent, const BranchConstraint& constraint);
    ~Node();

    int64_t getId() const;
    int64_t getDepth() const;
    const std::vector<BranchConstraint>& getConstraints() const;

    double getLowerBound() const;
    double getObjectiveValue() const;
    void setLowerBound(double lowerBound);
    void setObjectiveValue(double objectiveValue);

    bool isRoot() const;
    bool isSolved() const;
    bool isIntegral() const;
    bool isInfeasible() const;

    void setBranched();
    void setIntegral();
    void setInfeasible();

   private:
    const int64_t id;
    const int64_t depth;
    std::vector<BranchConstraint> constraints;

    NodeStatus status;
    double lowerBound;
    double objectiveValue;
};

#endif
