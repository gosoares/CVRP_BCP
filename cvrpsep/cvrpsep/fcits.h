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

#ifndef _H_FCITS
#define _H_FCITS

#include "basegrph.h"
#include "cnstrmgr.h"

void FCITS_MainCutGen(ReachPtr SupportPtr, long long NoOfCustomers, long long* Demand, long long CAP, double** XMatrix,
                      ReachPtr InitSuperNodesRPtr, ReachPtr InitSAdjRPtr, long long* InitSuperDemand,
                      long long InitShrunkGraphCustNodes, long long MaxFCITSLoops, long long MaxNoOfCuts,
                      double* MaxViolation, long long* NoOfGeneratedCuts, CnstrMgrPointer CutsCMP);

#endif
