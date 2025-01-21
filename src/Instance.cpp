#include "Instance.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>

int64_t gcd(int64_t a, int64_t b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

Instance::Instance(const std::string& filename) {
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    std::string data;

    std::getline(file, data);  // NAME
    std::getline(file, data);  // COMMENT
    std::getline(file, data);  // TYPE

    while (file.get() != ':');
    file >> this->V;  // DIMENSION;
    this->N = this->V - 1;

    this->demands.resize(this->V);

    while (file.get() != ':');
    file >> data;  // EDGE_WEIGHT_TYPE

    if (data == "EUC_2D") {
        readCoordinatesListInstance(file);
    } else if (data == "EXPLICIT") {
        readDistanceMatrixInstance(file);
    } else {
        throw std::runtime_error("Unknown edge weight type: " + data);
    }

    // populate edgeIds
    this->edgeIds.resize(this->V, std::vector<int64_t>(this->V, -1));
    for (int64_t e = 0; e < this->edges.size(); e++) {
        this->edgeIds[this->edges[e].first][this->edges[e].second] = e;
        this->edgeIds[this->edges[e].second][this->edges[e].first] = e;
    }

    // Read demands
    file >> data;  // DEMAND_SECTION
    assert(data == "DEMAND_SECTION");

    for (int64_t i = 0, id; i < this->V; i++) {
        file >> id;
        file >> this->demands[id - 1];
    }

    file >> data;
    assert(data == "DEPOT_SECTION");
    file >> data;
    assert(data == "1");
    file >> data;
    assert(data == "-1");
    file >> data;
    assert(data == "EOF");

    // Populate iterators
    this->clientIdxs.resize(this->N);
    std::iota(this->clientIdxs.begin(), this->clientIdxs.end(), 1);
    this->vertexIdxs.resize(this->V);
    std::iota(this->vertexIdxs.begin(), this->vertexIdxs.end(), 0);
    this->edgeIdxs.resize(this->edges.size());
    std::iota(this->edgeIdxs.begin(), this->edgeIdxs.end(), 0);

    // Calculate number of vehicles as 2*minVehicles where minVehicles = ceil(sum(demands)/vehicleCapacity)
    int64_t sumDemands = 0;
    for (int64_t v : this->getVertexIdxs()) {
        sumDemands += this->demands[v];
    }
    this->nbVehicles = 2 * ceil(sumDemands / this->vehicleCapacity);

    // normalize demands by the greatest common divisor
    this->demandsGcd = this->vehicleCapacity;
    for (int64_t d : this->demands) {
        this->demandsGcd = gcd(this->demandsGcd, d);
    }
    this->vehicleCapacity /= this->demandsGcd;
    for (int64_t v : this->getVertexIdxs()) {
        this->demands[v] /= this->demandsGcd;
    }
}

void Instance::readCoordinatesListInstance(std::ifstream& file) {
    std::string data;

    while (file.get() != ':');
    file >> this->vehicleCapacity;  // CAPACITY

    // Read coordinates
    file >> data;  // NODE_COORD_SECTION
    assert(data == "NODE_COORD_SECTION");

    int64_t id;
    std::vector<int64_t> X(this->V);
    std::vector<int64_t> Y(this->V);
    for (int64_t i = 0; i < this->V; i++) {
        file >> id;
        file >> X[id - 1];
        file >> Y[id - 1];
    }

    // Calculate euclidean distances
    for (int64_t i = 1; i < this->V; i++) {
        for (int64_t j = 0; j < i; j++) {
            this->edges.emplace_back(i, j);
            this->distances.push_back(std::llround(std::hypot(X[i] - X[j], Y[i] - Y[j])));
        }
    }
}

void Instance::readDistanceMatrixInstance(std::ifstream& file) {
    std::string data;

    while (file.get() != ':');
    file >> data;  // EDGE_WEIGHT_FORMAT
    assert(data == "LOWER_ROW");

    while (file.get() != ':');
    file >> data;  // DISPLAY_DATA_TYPE
    assert(data == "NO_DISPLAY");

    while (file.get() != ':');
    file >> this->vehicleCapacity;  // CAPACITY

    // Read distance matrix
    file >> data;  // EDGE_WEIGHT_SECTION
    assert(data == "EDGE_WEIGHT_SECTION");

    for (int64_t i = 1, d; i < this->V; i++) {
        for (int64_t j = 0; j < i; j++) {
            file >> d;
            this->edges.emplace_back(i, j);
            this->distances.push_back(d);
        }
    }
}
