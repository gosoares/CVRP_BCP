/****************************************************************************/
/*                                                                          */
/*  This file is distributed as part of the BCP_VRP package                 */
/*                                                                          */
/*  It was obtained as part of the CVRPSEP package developed by Jens        */
/*  Lysgaard. Original header follows.                                      */
/*                                                                          */
/****************************************************************************/

/* (C) Copyright 2003 Jens Lysgaard. All rights reserved. */
/* OSI Certified Open Source Software */
/* This software is licensed under the Common Public License Version 1.0 */

#ifndef _H_GRSEARCH
#define _H_GRSEARCH

#include "basegrph.h"
#include "cnstrmgr.h"

void GRSEARCH_SetUnitDemand(char UnitDemand);

void GRSEARCH_SwapNodesInPos(long long* Node, long long* Pos, long long s, long long t);

void GRSEARCH_GetInfeasExt(long long* Pos, long long MinCandidateIdx, long long MaxCandidateIdx,
                           long long NoOfCustomers, long long NodeSum, /* Sum of node numbers. */
                           ReachPtr RPtr, long long RPtrSize, long long* NodeLabel, long long Label, char* CallBack);

void GRSEARCH_AddSet(ReachPtr RPtr, long long Index, long long SetSize, long long* List, char AddFullSumList);

void GRSEARCH_CapCuts(ReachPtr SupportPtr, long long NoOfCustomers, long long* Demand, long long CAP,
                      long long* SuperNodeSize, double* XInSuperNode, double** XMatrix, long long* GeneratedSets,
                      long long* GeneratedAntiSets, ReachPtr SetsRPtr, /* Identified cuts. */
                      ReachPtr AntiSetsRPtr, long long MaxTotalGeneratedSets);

void GRSEARCH_CheckForExistingSet(ReachPtr RPtr, long long RPtrSize, long long* NodeLabel, long long Label,
                                  long long NodeSum, /* Sum of node numbers. */
                                  long long NodeSetSize, char* ListFound);

void GRSEARCH_AddDropCapsOnGS(ReachPtr SupportPtr, /* On GS */
                              long long NoOfCustomers, long long ShrunkGraphCustNodes, long long* SuperDemand,
                              long long CAP, long long* SuperNodeSize, double* XInSuperNode, ReachPtr SuperNodesRPtr,
                              double** SMatrix, /* Shrunk graph */
                              double Eps, CnstrMgrPointer CMPSourceCutList, long long* NoOfGeneratedSets,
                              long long MaxTotalGeneratedSets, ReachPtr SetsRPtr); /* Identified cuts. */

#endif
