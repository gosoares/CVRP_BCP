#include "Pricing.h"

#include <limits>

Pricing::Pricing(const Instance& instance)
    : instance(instance)
    , V(instance.getV())
    , C(instance.getVehicleCapacity())
    , M(C + 1, std::vector<Labels>()) {
    for (int64_t d = 0; d <= this->C; d++) {
        this->M[d].reserve(this->V);
        for (int64_t v : this->instance.getVertexIdxs()) {
            this->M[d].emplace_back(v, 2);
        }
    }
}

void Pricing::solve(const IloNumArray& prices) {
    this->initialize();
    int64_t totalDemand;
    double totalCost;

    // calculate the labels
    for (int64_t d = 0; d < this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            if (this->M[d][v].empty()) continue;

            for (int64_t w : this->instance.getClientIdxs()) {
                if (v == w) continue;

                totalDemand = d + this->instance.getDemand(w);
                if (totalDemand > this->C) continue;

                const Label* label = this->M[d][v].getBestLabelToExtendTo(w);
                if (label == nullptr) continue;

                totalCost = label->cost + prices[instance.getEdgeId(v, w)];
                this->M[totalDemand][w].add(totalCost, label);
            }
        }
    }

    // for each vertex v get the least costly walk that reaches v using total demand exactly C
    // if it has a negative reduced cost, add it to the list of negative reduced cost paths
    for (int64_t v : this->instance.getClientIdxs()) {
        // get least costly label for vertex v
        const Label* best;
        double bestCost = std::numeric_limits<double>::infinity();

        for (int64_t d = 0; d <= this->C; d++) {
            if (!this->M[d][v].empty()) {
                const Label* label = this->M[d][v].getBetter();
                double cost =
                    label->cost + prices[instance.getEdgeId(label->vertex, 0)];  // adds the cost to return to the depot

                if (cost < bestCost) {
                    bestCost = cost;
                    best = label;
                }
            }
        }

        // if it has a negative reduced cost, add it to the list of negative reduced cost paths
        if (bestCost > -1e-6) return;

        this->solutionQPaths.emplace_back();
        auto& qPathEdges = this->solutionQPaths.back();
        qPathEdges.push_back(instance.getEdgeId(best->vertex, 0));

        for (const Label* current = best; current->previous != nullptr; current = current->previous) {
            qPathEdges.push_back(instance.getEdgeId(current->previous->vertex, current->vertex));
        }
    }
}

void Pricing::initialize() {
    for (int64_t d = 0; d <= this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            this->M[d][v].clear();
        }
    }
    this->M[0][0].add(0, nullptr);

    this->solutionQPaths.clear();
}
