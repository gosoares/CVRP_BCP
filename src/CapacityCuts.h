#ifndef CAPACITYCUTS_H
#define CAPACITYCUTS_H

#pragma once

#include <cvrpsep/capsep.h>
#include <cvrpsep/cnstrmgr.h>
#include <ilcplex/ilocplex.h>

#include "Instance.h"
#include "MasterModel.h"

class CapacityCuts {
   public:
    CapacityCuts(const Instance& instance, MasterModel& masterModel);
    ~CapacityCuts();

    // return true if new cuts are added
    bool separate(const std::vector<double>& x);

   private:
    const Instance& instance;
    MasterModel& masterModel;

    long long maxNbCuts = 999999;
    double tolerance = 1e-6;

    long long* demands;
    long long* edgeTail;
    long long* edgeHead;
    double* edgeX;

    CnstrMgrPointer existingCuts;
    CnstrMgrPointer newCuts;

    void addCuts();
};

#endif
