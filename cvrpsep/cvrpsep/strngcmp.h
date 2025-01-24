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

#ifndef _H_STRNGCMP
#define _H_STRNGCMP

#include "basegrph.h"

void ComputeStrongComponents(ReachPtr RPtr, ReachPtr ResultPtr,
                             long long *NoOfComponents, long long n,
                             char *CVWrk1,
                             long long *IVWrk1, long long *IVWrk2, long long *IVWrk3,
                             long long *IVWrk4);

#endif
