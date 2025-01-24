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

#ifndef _H_CUTBASE
#define _H_CUTBASE

#include "basegrph.h"

void CUTBASE_CompXSumInSet(ReachPtr SupportPtr, long long NoOfCustomers, char* InNodeSet, long long* NodeList,
                           long long NodeListSize, double** XMatrix, double* XSumInSet);

void CUTBASE_CompVehiclesForSet(long long NoOfCustomers, char* NodeInSet, long long* NodeList, long long NodeListSize,
                                long long* Demand, long long CAP, long long* MinV);

void CUTBASE_CompCapViolation(ReachPtr SupportPtr, long long NoOfCustomers, char* NodeInSet, long long* NodeList,
                              long long NodeListSize, long long* Demand, long long CAP, double** XMatrix,
                              double* Violation);

#endif
