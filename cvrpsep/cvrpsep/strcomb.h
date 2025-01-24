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

#ifndef _H_STRCOMB
#define _H_STRCOMB

#include "basegrph.h"
#include "cnstrmgr.h"

void STRCOMB_MainStrengthenedCombs(ReachPtr SupportPtr,                                       /* Original graph */
                                   long long NoOfCustomers, long long CAP, long long* Demand, /* Original demand */
                                   long long QMin, double** XMatrix, long long MaxNoOfCuts, double* MaxViolation,
                                   CnstrMgrPointer CutsCMP);

#endif
