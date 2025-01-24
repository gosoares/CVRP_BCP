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

#ifndef _H_HPMSTAR
#define _H_HPMSTAR

#include "basegrph.h"
#include "cnstrmgr.h"

void HPMSTAR_CreateMinVVector(long long DemandSum, long long CAP);

void HPMSTAR_DirectX(ReachPtr SupportPtr,                        /* Original support graph. */
                     ReachPtr SAdjRPtr,                          /* Shrunk support graph. */
                     long long NoOfCustomers, long long* Demand, /* Original demand vector. */
                     long long CAP, long long NoOfSuperNodes, double* XInSuperNode, double** XMatrix, double** SMatrix,
                     ReachPtr SuperNodesRPtr, char SelectionRule, long long MaxGeneratedCuts, char SearchPartialMStars,
                     CnstrMgrPointer CutsCMP, double* MaxViolation);

#endif
