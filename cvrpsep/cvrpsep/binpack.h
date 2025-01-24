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

#ifndef _H_BINPACK
#define _H_BINPACK

void BP_ExactBinPacking(long long CAP,
                        long long *ItemSize,
                        long long n,
                        long long *LB,
                        long long *UB,
                        long long *Bin);

#endif

