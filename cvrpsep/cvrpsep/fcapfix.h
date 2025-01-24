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

#ifndef _H_FCAPFIX
#define _H_FCAPFIX

#include "basegrph.h"

void FCAPFIX_ComputeCuts(ReachPtr SupportPtr, long long NoOfCustomers, long long* Demand, long long CAP,
                         long long* SuperNodeSize, double** XMatrix, long long MaxCuts, long long MaxRounds,
                         long long* NoOfGeneratedCuts, ReachPtr ResultRPtr);

#endif
