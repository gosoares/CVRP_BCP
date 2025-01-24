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

#ifndef _H_NEWHTOUR
#define _H_NEWHTOUR

#include "basegrph.h"
#include "cnstrmgr.h"

void NEWHTOUR_ComputeHTours(ReachPtr SupportPtr, long long NoOfCustomers, long long* Demand, long long CAP,
                            double** XMatrix, double** SMatrix, ReachPtr SuperNodesRPtr, ReachPtr SAdjRPtr,
                            long long* SuperDemand, long long ShrunkGraphCustNodes, CnstrMgrPointer CutsCMP,
                            long long MaxHTourCuts, double* MaxHTourViolation);

#endif
