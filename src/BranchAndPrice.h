#ifndef BRANCHANDPRICE_H
#define BRANCHANDPRICE_H

#pragma once

#include "Instance.h"
#include "MasterModel.h"
#include "Pricing.h"

class BranchAndPrice {
   public:
    BranchAndPrice(const Instance& instance);
    ~BranchAndPrice();

    void solve();

   private:
    const Instance& instance;
    MasterModel masterModel;
    Pricing pricing;
};

#endif
