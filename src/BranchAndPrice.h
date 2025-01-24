#ifndef BRANCHANDPRICE_H
#define BRANCHANDPRICE_H

#pragma once

#include <chrono>

#include "CapacityCuts.h"
#include "Instance.h"
#include "MasterModel.h"
#include "Pricing.h"

class BranchAndPrice {
   public:
    BranchAndPrice(const Instance& instance);
    ~BranchAndPrice();

    void solve();
    void solveColumnGeneration();

   private:
    const Instance& instance;
    MasterModel masterModel;
    Pricing pricing;
    CapacityCuts capacityCuts;

    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
};

#endif
