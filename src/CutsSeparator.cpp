#include "CutsSeparator.h"

#include <cvrpsep/brnching.h>
#include <cvrpsep/capsep.h>

#include <format>

CutsSeparator::CutsSeparator(const Instance& instance)
    : instance(instance)
    , demands(new long long[instance.getV()])
    , edgeTail(new long long[instance.getNbEdges() + 1])
    , edgeHead(new long long[instance.getNbEdges() + 1])
    , edgeX(new double[instance.getNbEdges() + 1]) {
    CMGR_CreateCMgr(&(this->existingCuts), 100);
    CMGR_CreateCMgr(&(this->newCuts), 100);

    for (int64_t i = 0; i < instance.getV(); i++) {
        this->demands[i] = instance.getDemand(i);
    }
}

CutsSeparator::~CutsSeparator() {
    delete[] this->demands;
    delete[] this->edgeTail;
    delete[] this->edgeHead;
    delete[] this->edgeX;
    CMGR_FreeMemCMgr(&(this->existingCuts));
    CMGR_FreeMemCMgr(&(this->newCuts));
}

bool CutsSeparator::capacityCuts(const std::vector<double>& x) {
    this->loadEdges(x);

    char integerAndFeasible;
    double maxViolation;
    CAPSEP_SeparateCapCuts(
        this->instance.getN(),
        this->demands,
        this->instance.getVehicleCapacity(),
        this->nEdges,
        this->edgeTail,
        this->edgeHead,
        this->edgeX,
        this->existingCuts,
        this->maxNbCuts,
        this->tolerance,
        &integerAndFeasible,
        &maxViolation,
        this->newCuts
    );

    // std::cout << "Capacity Cuts Found: " << this->newCuts->Size << std::endl;
    // std::cout << "Existing Cuts: " << this->existingCuts->Size << std::endl;
    // std::cout << "Max Violation: " << maxViolation << std::endl;

    // for (int64_t c = 0; c < this->newCuts->Size; c++) {
    //     auto cut = this->newCuts->CPL[c];

    //     double lhs = 0;
    //     for (int64_t i = 1; i <= cut->IntListSize; i++) {
    //         int64_t tail = cut->IntList[i];
    //         for (int64_t j = i + 1; j <= cut->IntListSize; j++) {
    //             lhs += x[this->instance.getEdgeId(tail, cut->IntList[j])];
    //         }
    //     }

    //     std::cout << "Cut " << c << ": " << lhs << " <= " << cut->RHS << std::endl;
    // }

    return !integerAndFeasible && this->newCuts->Size > 0 && maxViolation > 1e-6;
}

void CutsSeparator::getBranchingSet(const std::vector<double>& x, std::vector<int64_t>& branchingSet) {
    this->loadEdges(x);

    const int64_t maxNbSets = 20;

    CnstrMgrPointer sets;
    CMGR_CreateCMgr(&sets, maxNbSets);

    BRNCHING_GetCandidateSets(
        this->instance.getN(),
        this->demands,
        this->instance.getVehicleCapacity(),
        this->nEdges,
        this->edgeTail,
        this->edgeHead,
        this->edgeX,
        this->existingCuts,
        2.7,  // boundary target from fukasawa article
        maxNbSets,
        1,  // divide by demand
        sets
    );

    double bestSetScore = std::numeric_limits<double>::infinity();

    for (int64_t s = 0; s < sets->Size; s++) {
        const auto& set = sets->CPL[s];

        double xSet = 0;
        for (int64_t i = 1; i <= set->IntListSize; i++) {
            for (int64_t j = i + 1; j <= set->IntListSize; j++) {
                xSet += x[this->instance.getEdgeId(set->IntList[i], set->IntList[j])];
            }
        }

        double setScore = abs(xSet - 2.7);

        if (setScore < bestSetScore) {
            bestSetScore = setScore;
            branchingSet.clear();
            for (int64_t i = 1; i <= set->IntListSize; i++) {
                branchingSet.push_back(set->IntList[i]);
            }
        }
    }
    CMGR_FreeMemCMgr(&sets);

    // std::cout << "Branching Set: ";
    // for (int64_t j : branchingSet) {
    //     std::cout << j << " ";
    // }
    // std::cout << "  Score: " << bestSetScore << std::endl;

    // for (int64_t e : this->instance.getEdgeIdxs()) {
    //     const auto& edge = this->instance.getEdge(e);
    //     std::cout << std::format("x[{},{}] = {}", edge.first, edge.second, x[e]) << std::endl;
    // }
    // std::cout << std::endl;

    // std::cout << "Branching Sets: " << std::endl;
    // for (const auto& set : branchingSets) {
    //     for (int64_t j : set) {
    //         std::cout << j << " ";
    //     }
    //     double xSet = 0;
    //     for (int64_t i = 0; i < set.size(); i++) {
    //         for (int64_t j = i + 1; j < set.size(); j++) {
    //             xSet += x[this->instance.getEdgeId(set[i], set[j])];
    //         }
    //     }
    //     std::cout << "xSet: " << xSet << std::endl;
    // }
}

void CutsSeparator::applyNewCutsTo(MasterModel& masterModel) {
    std::vector<int64_t> edges;

    for (int64_t c = 0; c < this->newCuts->Size; c++) {
        edges.clear();
        auto cut = this->newCuts->CPL[c];

        for (int64_t i = 1; i <= cut->IntListSize; i++) {
            int64_t tail = cut->IntList[i];
            for (int64_t j = i + 1; j <= cut->IntListSize; j++) {
                edges.push_back(this->instance.getEdgeId(tail, cut->IntList[j]));
            }
        }

        masterModel.addCut(edges, cut->RHS);
    }

    this->clearNewCuts();
}

void CutsSeparator::loadEdges(const std::vector<double>& x) {
    const int64_t depotId = this->instance.getV();

    this->nEdges = 0;
    for (int64_t e : this->instance.getEdgeIdxs()) {
        if (x[e] < this->tolerance) continue;

        this->nEdges++;
        const auto& edge = this->instance.getEdge(e);
        this->edgeTail[this->nEdges] = edge.second == 0 ? depotId : edge.second;
        this->edgeHead[this->nEdges] = edge.first == 0 ? depotId : edge.first;
        this->edgeX[this->nEdges] = x[e];
    }
}

void CutsSeparator::clearNewCuts() {
    // Move new cuts to existing cuts
    for (int64_t i = 0; i < this->newCuts->Size; i++) {
        CMGR_MoveCnstr(this->newCuts, this->existingCuts, i, 0);
    }
    this->newCuts->Size = 0;
}
