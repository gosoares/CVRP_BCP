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

#ifndef _H_MSTARSEP
#define _H_MSTARSEP

#include "cnstrmgr.h"

void MSTARSEP_SeparateMultiStarCuts(long long NoOfCustomers, long long* Demand, long long CAP, long long NoOfEdges,
                                    long long* EdgeTail, long long* EdgeHead, double* EdgeX,
                                    CnstrMgrPointer CMPExistingCuts, long long MaxNoOfCuts, double* MaxViolation,
                                    CnstrMgrPointer CutsCMP);

#endif
