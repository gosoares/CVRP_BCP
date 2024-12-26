#ifndef INSTANCE_H
#define INSTANCE_H

#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Instance {
   public:
    Instance(const std::string& filename);

    int64_t getV() const { return this->V; }
    int64_t getVehicleCapacity() const { return this->vehicleCapacity; }
    int64_t getDistance(int64_t i, int64_t j) const { return this->distances[i][j]; }
    int64_t getDemand(int64_t i) const { return this->demands[i]; }

   private:
    int64_t V;  // Number of vertices (depot + V-1 customers)
    int64_t vehicleCapacity;
    std::vector<std::vector<int64_t> > distances;
    std::vector<int64_t> demands;
    int64_t demandsGcd;  // Greatest common divisor of demands and vehicle capacity

    void readCoordinatesListInstance(std::ifstream& file);
    void readDistanceMatrixInstance(std::ifstream& file);
};

#endif
