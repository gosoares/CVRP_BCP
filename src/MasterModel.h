#ifndef MASTERMODEL_H
#define MASTERMODEL_H

#pragma once

#include <ilcplex/ilocplex.h>

#include <set>

#include "Instance.h"

class EdgeConstraints {
   private:
    const IloCplex& cplex;
    IloRangeArray constraints;
    IloNumArray duals;

   public:
    EdgeConstraints(const IloCplex& cplex);
    void add(const IloRange& constraint);
    const IloNumArray& getDuals() const;
};

class ConstraintSet {
   private:
    const IloCplex& cplex;
    IloRangeArray constraints;
    IloNumArray duals;
    std::vector<std::vector<int64_t> > constraintEdges;  // constraintEdges[i] = edges present in constraint i

   public:
    ConstraintSet(IloEnv& env, IloModel& model);
    void add(const IloRange& constraint);
    void add(const IloRangeArray& constraints);
    void end();
}

class MasterModel {
   private:
    const Instance& instance;
    IloNumArray edgesLength;
    IloEnv& env;
    IloModel model;
    IloCplex cplex;
    IloObjective objective;

    // each client is visited exactly once by some vehicle - constraint i is associated with client i + 1
    IloRangeArray visitedOnceConstraints;

    // K vehicles must leave and enter the depot - this constraint is associated with the depot
    IloRange nbVehiclesConstraint;

    // each subset is served by enough vehicles - each constraint is associated with a subset S in N
    IloRangeArray capacityConstraints;

    // each edge is traversed at most once - each constraint is associated with edge e in E\{0}
    IloRangeArray maxTraversalConstraints;  // each edge is traversed at most once

    IloNumVarArray lambdas;
    IloNumArray prices;
    IloNumVarArray artificials;
    std::vector<double> solution;

    std::vector<EdgeConstraints> edgeConstraints;

   public:
    MasterModel(IloEnv& env, const Instance& instance);
    ~MasterModel();

    void solve();
    void addColumn(const IloNumArray& column);

    const std::vector<double>& getSolution();
    const IloNumArray& getPrices();
    double getObjectiveValue() const;

    bool isInfeasible() const;
};

#endif
