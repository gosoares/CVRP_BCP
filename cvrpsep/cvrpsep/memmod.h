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

#ifndef _H_MEMMOD
#define _H_MEMMOD

#include <malloc.h>

void* MemGet(long long NoOfBytes);
void* MemReGet(void *p, long long NewNoOfBytes);
void MemFree(void *p);
char* MemGetCV(long long n);
char** MemGetCM(long long Rows, long long Cols);
void MemFreeCM(char **p, long long Rows);
long long* MemGetIV(long long n);
long long** MemGetIM(long long Rows, long long Cols);
void MemFreeIM(long long **p, long long Rows);
double* MemGetDV(long long n);
double** MemGetDM(long long Rows, long long Cols);
void MemFreeDM(double **p, long long Rows);

#endif
