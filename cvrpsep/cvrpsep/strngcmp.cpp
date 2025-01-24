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

#include <stdio.h>
#include <stdlib.h>
#include "basegrph.h"
#include "strngcmp.h"

long long ConnectCallNr;
long long StackSize, ComponentSize;
long long ComponentNr;
char *OnStack;
long long *NUMBER, *LOWLINK, *Stack, *Component;
ReachPtr AdjPtr, ResCompPtr;

void StrongConnect(long long v)
{
  long long a,w;

  ConnectCallNr++;

  LOWLINK[v]=ConnectCallNr;
  NUMBER[v]=ConnectCallNr;

  Stack[++StackSize]=v;
  OnStack[v]=1;

  for (a=1; a<=AdjPtr->LP[v].CFN; a++)
  {
    w=AdjPtr->LP[v].FAL[a];
    if (NUMBER[w]==0)
    {
      StrongConnect(w);
      if (LOWLINK[w] < LOWLINK[v]) LOWLINK[v] = LOWLINK[w];
    }
    else
    if (NUMBER[w] < NUMBER[v])
    {
      if (OnStack[w])
      {
        if (NUMBER[w] < LOWLINK[v]) LOWLINK[v] = NUMBER[w];
      }
    }
  }


  if (LOWLINK[v]==NUMBER[v])
  {
    ComponentNr++;
    ComponentSize=0;

    while ((StackSize > 0) && (NUMBER[Stack[StackSize]] >= NUMBER[v]))
    {
      OnStack[Stack[StackSize]]=0;
      Component[++ComponentSize]=Stack[StackSize--];
    }

    ReachSetForwList(ResCompPtr,Component,ComponentNr,ComponentSize);
  }

}


void ComputeStrongComponents(ReachPtr RPtr, ReachPtr ResultPtr,
                             long long *NoOfComponents, long long n,
                             char *CVWrk1,
                             long long *IVWrk1, long long *IVWrk2, long long *IVWrk3,
                             long long *IVWrk4)
{
  long long i;

  OnStack = CVWrk1;

  NUMBER  = IVWrk1;
  LOWLINK = IVWrk2;
  Stack   = IVWrk3;
  Component = IVWrk4;

  for (i=1; i<=n; i++)
  {
    NUMBER[i]=0;
    OnStack[i]=0;
  }

  ConnectCallNr = 0;
  StackSize = 0;
  ComponentNr = 0;

  AdjPtr = RPtr;
  ResCompPtr = ResultPtr;

  for (i=1; i<=n; i++)
  {
    if (NUMBER[i]==0)
    StrongConnect(i);
  }

  *NoOfComponents = ComponentNr;
}
