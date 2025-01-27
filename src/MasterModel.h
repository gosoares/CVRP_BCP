#ifndef MASTERMODEL_H
#define MASTERMODEL_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <set>

#include "Constraints.h"
#include "Instance.h"
#include "Node.h"

class MasterModel {
   private:
    const Instance& instance;
    IloEnv env;
    IloModel model;
    IloCplex cplex;
    IloObjective objective;

    Constraints constraints;
    Constraints branchConstraints;

    IloNumVarArray lambdas;
    IloExprArray edgeLambdas;
    // edgeLambdas[e] = x[e] in terms of lambdas i.e. the linear expression that represents x[e]

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
    void applyBranchConstraint(const BranchConstraint& constraint);
    void applyBranchConstraints(const std::vector<BranchConstraint>& constraints);
    void clearBranchConstraints();

    const std::vector<double>& getSolution();
    const std::vector<double>& getOriginalSolution();

    const IloNumArray& getPrices();
    double getObjectiveValue() const;

    bool isInfeasible() const;
};

#endif
