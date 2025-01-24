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

#ifndef _H_SORT
#define _H_SORT

void SortIVInc(long long *Value, long long n);
void SortIVDec(long long *Value, long long n);
void SortDVInc(double *Value, long long n);
void SortDVDec(double *Value, long long n);
void SortIndexIVInc(long long *Index, long long *Value, long long n);
void SortIndexIVDec(long long *Index, long long *Value, long long n);
void SortIndexDVDec(long long *Index, double *Value, long long n);
void SortIndexDVInc(long long *Index, double *Value, long long n);

#endif


