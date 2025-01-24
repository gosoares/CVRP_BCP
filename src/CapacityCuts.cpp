#include "CapacityCuts.h"

CapacityCuts::CapacityCuts(const Instance& instance, MasterModel& masterModel)
    : instance(instance)
    , masterModel(masterModel)
    , demands(new long long[instance.getV()])
    , edgeTail(new long long[instance.getNbEdges()])
    , edgeHead(new long long[instance.getNbEdges()])
    , edgeX(new double[instance.getNbEdges()]) {
    CMGR_CreateCMgr(&(this->existingCuts), 100);
    CMGR_CreateCMgr(&(this->newCuts), 100);

    for (int64_t i = 0; i < instance.getV(); i++) {
        this->demands[i] = instance.getDemand(i);
    }
}

CapacityCuts::~CapacityCuts() {
    delete[] demands;
    delete[] edgeTail;
    delete[] edgeHead;
    delete[] edgeX;

    // todo delete cuts
}

bool CapacityCuts::separate(const std::vector<double>& x) {
    char integerAndFeasible;
    double maxViolation;
    long long nEdges = 0;  // number of edges with non-zero reduced cost

    for (int64_t e : this->instance.getEdgeIdxs()) {
        if (x[e] < this->tolerance) continue;

        const auto& edge = this->instance.getEdge(e);
        this->edgeTail[nEdges] = edge.first;
        this->edgeHead[nEdges] = edge.second;
        this->edgeX[nEdges] = x[e];
        nEdges++;
    }

    CAPSEP_SeparateCapCuts(
        this->instance.getN(),
        this->demands,
        this->instance.getVehicleCapacity(),
        nEdges,
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

    if (integerAndFeasible) return false;
    if (this->newCuts->Size == 0) return false;

    std::cout << "Capacity Cuts Found: " << this->newCuts->Size << std::endl;
    std::cout << "Existing Capacity Cuts: " << this->existingCuts->Size << std::endl;
    this->addCuts();

    // Move new cuts to existing cuts
    for (int64_t i = 0; i < this->newCuts->Size; i++) {
        CMGR_MoveCnstr(this->newCuts, this->existingCuts, i, 0);
    }
    this->newCuts->Size = 0;

    return true;
}

void CapacityCuts::addCuts() {
    std::vector<int64_t> edges;

    for (int64_t c = 0; c < this->newCuts->Size; c++) {
        edges.clear();
        auto cut = this->newCuts->CPL[c];

        for (int64_t i = 1; i < cut->IntListSize; i++) {
            int64_t tail = cut->IntList[i];
            for (int64_t j = i + 1; j < cut->IntListSize; j++) {
                edges.push_back(this->instance.getEdgeId(tail, cut->IntList[j]));
            }
        }

        this->masterModel.addCut(edges, cut->RHS);
    }
}
