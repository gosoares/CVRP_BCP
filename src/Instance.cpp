#include "Instance.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

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

    this->demands.resize(this->V);
    this->distances.resize(this->V);
    for (int64_t i = 0; i < this->V; i++) {
        this->distances[i].resize(this->V, 0);
    }

    while (file.get() != ':');
    file >> data;  // EDGE_WEIGHT_TYPE

    if (data == "EUC_2D") {
        readCoordinatesListInstance(file);
    } else if (data == "EXPLICIT") {
        readDistanceMatrixInstance(file);
    } else {
        throw std::runtime_error("Unknown edge weight type: " + data);
    }

    file >> data;
    assert(data == "DEPOT_SECTION");
    file >> data;
    assert(data == "1");
    file >> data;
    assert(data == "-1");
    file >> data;
    assert(data == "EOF");
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
    for (int64_t i = 0; i < this->V; i++) {
        for (int64_t j = i + 1; j < this->V; j++) {
            this->distances[i][j] = std::llround(std::hypot(X[i] - X[j], Y[i] - Y[j]));
            this->distances[j][i] = this->distances[i][j];
        }
    }

    // Read demands
    file >> data;  // DEMAND_SECTION
    assert(data == "DEMAND_SECTION");

    for (int64_t i = 0; i < this->V; i++) {
        file >> id;
        file >> this->demands[id - 1];
    }
}

void Instance::readDistanceMatrixInstance(std::ifstream& file) {
    std::string data;
    int64_t id;

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

    for (int64_t i = 0; i < this->V; i++) {
        for (int64_t j = 0; j < i; j++) {
            file >> this->distances[i][j];
            this->distances[j][i] = this->distances[i][j];
        }
    }

    // Read demands
    file >> data;  // DEMAND_SECTION
    assert(data == "DEMAND_SECTION");

    for (int64_t i = 0; i < this->V; i++) {
        file >> id;
        file >> this->demands[id - 1];
    }
}
