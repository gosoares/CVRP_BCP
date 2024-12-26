#include "Pricing.h"

#include <limits>

Pricing::Pricing(const Instance& instance)
    : instance(instance)
    , V(instance.getV())
    , C(instance.getVehicleCapacity())
    , minCost(C + 1, std::vector<double>(V))
    , before(C + 1, std::vector<int64_t>(V)) {}

void Pricing::solve(const std::vector<double>& prices) {
    this->initialize();
    int64_t totalDemand;
    double totalCost;

    for (int64_t d = 0; d < this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            if (this->minCost[d][v] == std::numeric_limits<double>::infinity()) continue;
            for (int64_t w = 0; w < this->V; w++) {
                if (v == w || w == this->before[d][v]) continue;  // eliminate 2-cycle q-routes
                totalDemand = d + this->instance.getDemand(w);
                totalCost = this->minCost[d][v] + prices[instance.getEdgeId(v, w)];

                if (totalDemand <= this->C && totalCost < this->minCost[totalDemand][w]) {
                    this->minCost[totalDemand][w] = totalCost;
                    this->before[totalDemand][w] = v;
                }
            }
        }
    }
    this->minCost[0][0] = 0;
}

void Pricing::initialize() {
    // Initialize minCost and before
    for (int64_t d = 0; d <= this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            this->minCost[d][v] = std::numeric_limits<double>::infinity();
            this->before[d][v] = -1;
        }
    }
    this->minCost[0][0] = 0;
}
