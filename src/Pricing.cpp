#include "Pricing.h"

#include <algorithm>
#include <limits>

const int64_t ngSetsSize = 4;  // Number of nearest neighbors to consider

Pricing::Pricing(const Instance& instance)
    : instance(instance)
    , V(instance.getV())
    , C(instance.getVehicleCapacity())
    , ngSets(instance.getV(), boost::dynamic_bitset<>(instance.getV()))
    , M(C + 1, std::vector<LabelBucket>()) {
    // Calculate the ng-sets for each vertex
    for (int64_t v = 0; v < this->V; ++v) {
        std::vector<std::pair<int64_t, int64_t>> distances;
        for (int64_t w = 0; w < this->V; ++w) {
            if (v != w) {
                distances.emplace_back(this->instance.getDistance(this->instance.getEdgeId(v, w)), w);
            }
        }
        std::sort(distances.begin(), distances.end());
        for (int i = 0; i < std::min(ngSetsSize, static_cast<int64_t>(ngSets[v].size())); ++i) {
            ngSets[v].set(distances[i].second);
        }
    }

    // initialize the label buckets
    for (int64_t d = 0; d <= this->C; d++) {
        this->M[d].reserve(this->V);
        for (int64_t v : this->instance.getVertexIdxs()) {
            this->M[d].emplace_back(v);
        }
    }
}

double Pricing::solve(const IloNumArray& prices) {
    this->initialize();
    int64_t totalDemand;
    double totalCost;

    // calculate the labels
    for (int64_t d = 0; d < this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            if (this->M[d][v].empty()) continue;

            for (int64_t w : this->instance.getVertexIdxs()) {
                if (v == w) continue;

                totalDemand = d + this->instance.getDemand(w);
                if (totalDemand > this->C) continue;

                for (const Label* label : this->M[d][v].getLabels()) {
                    if (label->forbidden.test(w)) continue;

                    totalCost = label->cost + prices[instance.getEdgeId(v, w)];
                    boost::dynamic_bitset<> forbidden = label->forbidden & this->ngSets[w];
                    forbidden.set(w);

                    this->M[totalDemand][w].add(totalCost, forbidden, label);
                }
            }
        }
    }

    double globalBestReducedCost = std::numeric_limits<double>::infinity();

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

        if (bestCost < globalBestReducedCost) {
            globalBestReducedCost = bestCost;
        }

        // if it has a negative reduced cost, add it to the list of negative reduced cost paths
        if (bestCost > -1e-6) continue;

        this->solutionQPaths.emplace_back();
        auto& qPathEdges = this->solutionQPaths.back();
        qPathEdges.push_back(instance.getEdgeId(best->vertex, 0));

        for (const Label* current = best; current->previous != nullptr; current = current->previous) {
            qPathEdges.push_back(instance.getEdgeId(current->previous->vertex, current->vertex));
        }
    }

    return globalBestReducedCost;
}

void Pricing::initialize() {
    for (int64_t d = 0; d <= this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            this->M[d][v].clear();
        }
    }
    this->M[0][0].add(0, boost::dynamic_bitset<>(this->instance.getV()), nullptr);

    this->solutionQPaths.clear();
}
