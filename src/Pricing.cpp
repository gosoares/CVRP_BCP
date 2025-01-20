#include "Pricing.h"

#include <limits>

Labels::Labels(int64_t vertex, int64_t maxLabels)
    : vertex(vertex)
    , maxLabels(maxLabels) {};

Labels::~Labels() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}

void Labels::add(double cost, const Label* previous) {
    size_t insertIndex = this->labels.size();
    while (insertIndex > 0 && this->labels[insertIndex - 1]->cost > cost) {
        insertIndex--;
    }

    if (this->labels.size() < this->maxLabels) {  // has space to insert without removing another
        this->labels.insert(this->labels.begin() + insertIndex, new Label{this->vertex, cost, previous});
        return;
    }
    if (insertIndex == this->labels.size()) return;  // its not better than any label

    // shift labels to the right
    delete this->labels.back();
    for (size_t i = this->labels.size() - 2; i >= insertIndex; i--) {
        this->labels[i + 1] = this->labels[i];
    }

    this->labels[insertIndex] = new Label{this->vertex, cost, previous};
}

const Label* Labels::getBestLabelToExtendTo(int64_t vertex) const {
    int64_t index = 0;
    while (this->labels[index]->previous->vertex == vertex) {
        index++;
    }
    return this->labels[index];
}

void Labels::clear() {
    for (Label* label : this->labels) {
        delete label;
    }
    this->labels.clear();
}

Pricing::Pricing(const Instance& instance)
    : instance(instance)
    , V(instance.getV())
    , C(instance.getVehicleCapacity())
    , M(C + 1, std::vector<Labels>()) {
    for (int64_t d = 0; d <= this->C; d++) {
        this->M[d].reserve(this->V);
        for (int64_t v = 0; v < this->V; v++) {
            this->M[d].emplace_back(0, 2);
        }
    }
}

std::vector<std::vector<int64_t> > Pricing::solve(const std::vector<double>& prices) {
    this->initialize();
    int64_t totalDemand;
    double totalCost;

    // calculate the labels
    for (int64_t d = 0; d < this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            for (int64_t w = 0; w < this->V; w++) {
                if (v == w) continue;

                totalDemand = d + this->instance.getDemand(w);
                if (totalDemand > this->C) continue;

                const Label* label = this->M[d][v].getBestLabelToExtendTo(w);
                totalCost = label->cost + prices[instance.getEdgeId(v, w)];

                this->M[totalDemand][w].add(totalCost, label);
            }
        }
    }

    std::vector<std::vector<int64_t> > negativeQPaths;  // qpaths given by the edges in it

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
        if (best->cost > -1e-6) return;

        negativeQPaths.emplace_back();
        auto& qPathEdges = negativeQPaths.back();
        qPathEdges.push_back(instance.getEdgeId(best->vertex, 0));

        for (const Label* current = best; current = current->previous; current->previous != nullptr) {
            qPathEdges.push_back(instance.getEdgeId(current->previous->vertex, current->vertex));
        }
    }

    return negativeQPaths;
}

void Pricing::initialize() {
    for (int64_t d = 0; d <= this->C; d++) {
        for (int64_t v = 0; v < this->V; v++) {
            this->M[d][v].clear();
        }
    }
    this->M[0][0].add(0, new Label({0, 0, nullptr}));
}
