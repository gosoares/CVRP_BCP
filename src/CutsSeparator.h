#ifndef CAPACITYCUTS_H
#define CAPACITYCUTS_H

#pragma once

#include <cvrpsep/cnstrmgr.h>
#include <ilcplex/ilocplex.h>

#include "Instance.h"
#include "MasterModel.h"

class CutsSeparator {
   public:
    CutsSeparator(const Instance& instance);
    ~CutsSeparator();

    // return true if new cuts are found
    bool capacityCuts(const std::vector<double>& x);

    void getBranchingSet(const std::vector<double>& x, std::vector<int64_t>& branchingSet);

    void applyNewCutsTo(MasterModel& masterModel);

   private:
    const Instance& instance;

    long long maxNbCuts = 100;
    double tolerance = 1e-10;

    long long* demands;

    int64_t nEdges;  // number of edges with non-zero reduced cost
    long long* edgeTail;
    long long* edgeHead;
    double* edgeX;

    CnstrMgrPointer existingCuts;
    CnstrMgrPointer newCuts;

    void loadEdges(const std::vector<double>& x);
    void clearNewCuts();
};

#endif
