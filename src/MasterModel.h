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
    std::vector<std::vector<int64_t> > edgeLambdas;  // edgeLambdas[e] = lambdas associated with qPaths that has edge e

    IloNumArray prices;
    IloNumVarArray artificials;
    IloNumVar& newArtificial();

    std::vector<double> solution;

   public:
    MasterModel(const Instance& instance);
    ~MasterModel();

    void solve();
    void addColumn(const std::vector<int64_t>& qRouteEdges);

    const std::vector<double>& getSolution();
    const IloNumArray& getPrices();
    double getObjectiveValue() const;

    bool isInfeasible() const;
};

#endif
