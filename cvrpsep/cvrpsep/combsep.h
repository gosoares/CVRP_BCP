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

#ifndef _H_COMBSEP
#define _H_COMBSEP

#include "cnstrmgr.h"

void COMBSEP_SeparateCombs(long long NoOfCustomers, long long* Demand, long long CAP, long long QMin,
                           long long NoOfEdges, long long* EdgeTail, long long* EdgeHead, double* EdgeX,
                           long long MaxNoOfCuts, double* MaxViolation, CnstrMgrPointer CutsCMP);

#endif
