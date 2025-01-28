#ifndef BRANCHANDPRICE_H
#define BRANCHANDPRICE_H

#pragma once

#include <chrono>

#include "CutsSeparator.h"
#include "Instance.h"
#include "MasterModel.h"
#include "Node.h"
#include "Pricing.h"

class BranchAndPrice {
   public:
    BranchAndPrice(const Instance& instance);
    ~BranchAndPrice();

    void solve();
    const std::vector<double>& solveNode(Node& node);
    void solveColumnGeneration(Node& node);

   private:
    const Instance& instance;
    MasterModel masterModel;
    Pricing pricing;
    CutsSeparator cutsSeparator;

    double upperBound;
    std::vector<double> bestSolution;

    // statistics
    int64_t nSolvedNodes;
    int64_t nextNodeId;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    bool isIntegral(const std::vector<double>& x) const;

    void printLogLine(const Node& node, int64_t nOpenNodes, bool boundPruned) const;
};

#endif
