#ifndef MASTERMODEL_H
#define MASTERMODEL_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <set>

#include "Instance.h"

class MasterModel {
   private:
    const Instance& instance;
    IloEnv env;
    IloModel model;
    IloCplex cplex;
    IloObjective objective;

    IloRangeArray constraints;
    std::vector<std::vector<int64_t> > edgeConstraints;  // edgeConstraints[e] = constraints that has edge e

    IloNumVarArray lambdas;
    std::vector<std::vector<std::pair<int64_t, int64_t> > > edgeLambdas;
    // edgeLambdas[e] = {lamdaId, q_e} associated with qPaths that has edge e

    IloNumArray prices;
    IloNumVarArray artificials;
    IloNumVar& newArtificial();

    std::vector<double> solution;          // lambda values
    std::vector<double> originalSolution;  // x_e values

   public:
    MasterModel(const Instance& instance);
    ~MasterModel();

    void solve();
    void addColumn(const std::vector<int64_t>& qRouteEdges);
    void addCut(const std::vector<int64_t>& edges, double rhs);

    const std::vector<double>& getSolution();
    const std::vector<double>& getOriginalSolution();

    const IloNumArray& getPrices();
    double getObjectiveValue() const;

    bool isInfeasible() const;
};

#endif
