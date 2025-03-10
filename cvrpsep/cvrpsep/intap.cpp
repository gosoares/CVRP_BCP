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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "memmod.h"
#include "intap.h"

void INTAPInitMem(INTAPPtr *P, long long n, long long Dim)
{
  (*P) = (INTAPRec *) MemGet(sizeof(INTAPRec));
  (*P)->n   = n;
  (*P)->Dim = Dim;

  (*P)->c = MemGetIM(Dim+1,Dim+1);

  (*P)->u          = MemGetIV(Dim+1);
  (*P)->v          = MemGetIV(Dim+1);
  (*P)->rall       = MemGetIV(Dim+1);
  (*P)->call       = MemGetIV(Dim+1);
  (*P)->LR         = MemGetCV(Dim+1);
  (*P)->UC         = MemGetCV(Dim+1);
  (*P)->pi         = MemGetIV(Dim+1);
  (*P)->cj         = MemGetIV(Dim+1);
}

void INTAPExpandDim(INTAPPtr P)
{
  long long i,OldDim;

  OldDim = P->Dim;
  P->Dim *= 2;

  P->c = (long long **) MemReGet(P->c,sizeof(long long *)*(P->Dim+1));

  for (i=OldDim+1; i<=P->Dim; i++)
  P->c[i]=NULL;

  for (i=0; i<=P->Dim; i++)
  P->c[i] = (long long *) MemReGet(P->c[i],sizeof(long long)*(P->Dim+1));

  P->u    = (long long *) MemReGet(P->u,sizeof(long long)*(P->Dim+1));
  P->v    = (long long *) MemReGet(P->v,sizeof(long long)*(P->Dim+1));

  P->rall = (long long *) MemReGet(P->rall,sizeof(long long)*(P->Dim+1));
  P->call = (long long *) MemReGet(P->call,sizeof(long long)*(P->Dim+1));

  P->LR   = (char *) MemReGet(P->LR,sizeof(char)*(P->Dim+1));
  P->UC   = (char *) MemReGet(P->UC,sizeof(char)*(P->Dim+1));

  P->pi    = (long long *) MemReGet(P->pi,sizeof(long long)*(P->Dim+1));
  P->cj    = (long long *)  MemReGet(P->cj,sizeof(long long)*(P->Dim+1));
}

void INTAPFreeMem(INTAPPtr *P)
{
  MemFree((*P)->u);
  MemFree((*P)->v);
  MemFree((*P)->rall);
  MemFree((*P)->call);
  MemFree((*P)->LR);
  MemFree((*P)->UC);
  MemFree((*P)->pi);
  MemFree((*P)->cj);

  MemFreeIM((*P)->c,(*P)->Dim+1);

  MemFree(*P);
  *P=NULL;
}

void INTAPInit(INTAPPtr P)
{
  char assign;
  long long i,j,k,r,n,x;
  long long *p;

  n = P->n;
  p = MemGetIV(n+1);

  /* Begin Phase 1 */

  for (i=1; i<=n; i++)
  {
    P->rall[i] = 0;
    P->call[i] = 0;
    P->u[i] = 0.0;
    P->v[i] = 0.0;
    p[i] = 0;
  }

  for (j=1; j<=n; j++)
  {
    for (i=2, x=P->c[1][j], r=1; i<=n; i++)
    {
      if (P->c[i][j] < x)
      {
        x = P->c[i][j];
        r = i;
      }
    }
    P->v[j]=x;
    if (P->rall[r]==0)
    {
      P->call[j]=r;
      P->rall[r]=j;
      p[r]=j+1;
    }
  }

  /* End Phase 1 */

  /* Begin Phase 2 */

  for (i=1; i<=n; i++)
  {
    if (P->rall[i]==0)
    {
      for (k=2, x=P->c[i][1]-P->v[1], j=1; k<=n; k++)
      {
        if (P->c[i][k]-P->v[k] < x)
        {
          x = P->c[i][k]-P->v[k];
          j = k;
        }
      }
      P->u[i]=x;
      if (P->call[j]==0) assign=1; else assign=0;

      while ((assign==0) && (j<=n))
      {
        if (P->c[i][j]-P->u[i]-P->v[j] == 0)
        {
          r=P->call[j];
          k=p[r];

          while ((assign==0) && (k<=n))
          {
            if ((P->call[k]==0) && (P->c[r][k]-P->u[r]-P->v[k] == 0))
            {
              assign=1;
              P->rall[r]=k;
              P->call[k]=r;
            }
            else
            k++;
          }
          p[r]=k+1;
        }
        if (assign==0) j++;
      }

      if (assign==1)
      {
        P->rall[i]=j;
        P->call[j]=i;
        p[i]=j+1;
      }
    }
  }

  /* End Phase 2 */

  MemFree(p);
}

void INTAPPath(INTAPPtr P, long long r, long long *Col)
{
  long long i,j,k,n,d,x;
  long long inf = INT_MAX;

  n = P->n;
  for (i=1; i<=n; i++) { P->LR[i]=0; P->UC[i]=1; P->pi[i]=inf; };
  P->LR[r]=1;

  do
  {
    for (j=1; j<=n; j++)
    {
      if (P->UC[j])
      {
        x=P->c[r][j]-P->u[r]-P->v[j];
        if (x < P->pi[j])
        {
          P->pi[j]=x;
          P->cj[j]=r;
        }
      }
    }

    for (j=1; (j<=n) && ((P->pi[j] > 0) || (!P->UC[j])); j++);

    if (j>n)
    {
      for (k=1, d=inf; k<=n; k++)
      {
        if ((P->UC[k]) && (P->pi[k]<d)) { d=P->pi[k]; };
      }

      for (k=1; k<=n; k++)
      {
        if (P->LR[k]) { P->u[k]+=d; };
      }

      for (k=1; k<=n; k++)
      {
        if (P->pi[k] > 0) P->pi[k]-=d; else P->v[k]-=d;
      }

      for (j=1; ((P->pi[j] > 0) || (!P->UC[j])); j++);
    }

    if (P->call[j])
    {
      P->LR[P->call[j]]=1;
      P->UC[j]=0;
      r=P->call[j];
    }
  }
  while (P->call[j]);

  *Col=j;
}

void INTAPIncrease(INTAPPtr P, long long i, long long j)
{
  long long l,k;

  do
  {
    l=P->cj[j];
    P->call[j]=l;
    k=P->rall[l];
    P->rall[l]=j;
    j=k;
  } while (!(l==i));
}

void INTAPHungarian(INTAPPtr P)
{
  long long i,j;

  INTAPInit(P);

  for (i=1; i<=P->n; i++)
  {
    if (P->rall[i]==0)
    {
      INTAPPath(P,i,&j);
      INTAPIncrease(P,i,j);
    }
  }
}

void INTAPIterate(INTAPPtr P, long long Row)
{
  long long j;

  INTAPPath(P,Row,&j);
  INTAPIncrease(P,Row,j);
}

long long INTAPObjValue(INTAPPtr P)
{
  long long i,z;

  for (z=0, i=1; i<=P->n; i++)
  {
    z+=P->u[i];
    z+=P->v[i];
  }

  return z;
}

