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
    int64_t getN() const { return this->N; }

    int64_t getNbVehicles() const { return this->nbVehicles; }
    int64_t getVehicleCapacity() const { return this->vehicleCapacity; }

    int64_t getDemand(int64_t i) const { return this->demands[i]; }
    int64_t getDemandsGcd() const { return this->demandsGcd; }

    int64_t getDistance(int64_t e) const { return this->distances[e]; }
    bool hasDepot(int64_t e) const { return this->edges[e].second == 0; }

    int64_t getNbEdges() const { return this->edges.size(); }
    std::pair<int64_t, int64_t> getEdge(int64_t id) const { return this->edges[id]; }
    const std::vector<std::pair<int64_t, int64_t> >& getEdges() const { return this->edges; }
    int64_t getEdgeId(int64_t i, int64_t j) const { return this->edgeIds[i][j]; };

    const std::vector<int64_t> getCutSet(const std::vector<int64_t>& S) const;

    // for iteration purposes
    const std::vector<int64_t>& getClientIdxs() const { return this->clientIdxs; }
    const std::vector<int64_t>& getVertexIdxs() const { return this->vertexIdxs; }
    const std::vector<int64_t>& getEdgeIdxs() const { return this->edgeIdxs; }

   private:
    int64_t V;           // Number of vertices (depot + V-1 customers)
    int64_t N;           // Number of customers = V-1
    int64_t nbVehicles;  // Number of vehicles (K)
    int64_t vehicleCapacity;

    std::vector<int64_t> distances;                   // distance[e] = distance of edge e
    std::vector<std::pair<int64_t, int64_t> > edges;  // edges[e]: {i, j in V | i > j}
    std::vector<std::vector<int64_t> > edgeIds;

    std::vector<int64_t> demands;
    int64_t demandsGcd;  // Greatest common divisor of demands and vehicle capacity

    // for iteration purposes
    std::vector<int64_t> clientIdxs;
    std::vector<int64_t> vertexIdxs;
    std::vector<int64_t> edgeIdxs;

    void readCoordinatesListInstance(std::ifstream& file);
    void readDistanceMatrixInstance(std::ifstream& file);
};

#endif
