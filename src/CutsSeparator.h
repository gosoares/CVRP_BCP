#ifndef CAPACITYCUTS_H
#define CAPACITYCUTS_H

#pragma once

#include <cvrpsep/capsep.h>
#include <cvrpsep/cnstrmgr.h>
#include <ilcplex/ilocplex.h>

#include "Instance.h"
#include "MasterModel.h"

class CutsSeparator {
   public:
    CutsSeparator(const Instance& instance);
    ~CutsSeparator();

    // return true if new cuts are added
    bool capacityCuts(const std::vector<double>& x);
    void applyNewCutsTo(MasterModel& masterModel);

   private:
    const Instance& instance;

    long long maxNbCuts = 999999;
    double tolerance = 1e-6;

    long long* demands;
    long long* edgeTail;
    long long* edgeHead;
    double* edgeX;

    CnstrMgrPointer existingCuts;
    CnstrMgrPointer newCuts;

    void clearNewCuts();
};

#endif
