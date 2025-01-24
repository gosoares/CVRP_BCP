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

#ifndef _H_INTAP
#define _H_INTAP

typedef struct
{
  long long **c;       /* Cost matrix */
  long long *u;        /* Dual variables for rows */
  long long *v;        /* Dual variables for columns */
  long long *rall;     /* Row allocations */
  long long *call;     /* Column allocations */
  char *LR;      /* Labeled rows */
  char *UC;      /* Unlabeled columns */
  long long *pi;       /* Minimum reduced costs in unlabeled columns */
  long long *cj;       /* pi[i] is in row cj[i] */
  long long n;         /* The actual problem size  (1,...,n) */
  long long Dim;       /* Allocated dimension (memory is reserved up to n=Dim) */
} INTAPRec;

typedef INTAPRec *INTAPPtr;

void INTAPInitMem(INTAPPtr *P, long long n, long long Dim);
void INTAPFreeMem(INTAPPtr *P);
void INTAPHungarian(INTAPPtr P);
void INTAPIterate(INTAPPtr P, long long Row);
long long INTAPObjValue(INTAPPtr P);

#endif
