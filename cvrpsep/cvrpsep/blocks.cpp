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
#include "memmod.h"
#include "basegrph.h"
#include "blocks.h"

long long BLStackSize, BLNodeListSize;
long long BLComponentNr, BLCounter;
long long u1,u2;
char *BLNodeInComponent;
long long *BLNUMBER, *BLLOWPT, *BLStackLeft, *BLStackRight, *BLNodeList;
ReachPtr BLAdjPtr, BLResCompPtr;

void BiConnect(long long v, long long u)
{
  long long a,w;

  BLCounter++;
  BLNUMBER[v]=BLCounter;
  BLLOWPT[v]=BLCounter;

  for (a=1; a<=BLAdjPtr->LP[v].CFN; a++)
  {
    w=BLAdjPtr->LP[v].FAL[a];
    if (BLNUMBER[w]==0)
    {
      BLStackSize++;
      BLStackLeft[BLStackSize]=v;
      BLStackRight[BLStackSize]=w;

      BiConnect(w,v);

      if (BLLOWPT[w] < BLLOWPT[v])
      {
        BLLOWPT[v] = BLLOWPT[w];
      }

      if (BLLOWPT[w] >= BLNUMBER[v])
      {
        BLComponentNr++;
        BLNodeListSize=0;

        while ((BLStackSize > 0) &&
               (BLNUMBER[BLStackLeft[BLStackSize]] >= BLNUMBER[w]))
        {
          u1 = BLStackLeft[BLStackSize];
          u2 = BLStackRight[BLStackSize];
          BLStackSize--;

          if (BLNodeInComponent[u1] == 0)
          {
            BLNodeList[++BLNodeListSize] = u1;
            BLNodeInComponent[u1] = 1;
          }

          if (BLNodeInComponent[u2] == 0)
          {
            BLNodeList[++BLNodeListSize] = u2;
            BLNodeInComponent[u2] = 1;
          }
        }

        BLStackSize--;

        if (BLNodeInComponent[v] == 0)
        {
          BLNodeList[++BLNodeListSize] = v;
          BLNodeInComponent[v] = 1;
        }

        if (BLNodeInComponent[w] == 0)
        {
          BLNodeList[++BLNodeListSize] = w;
          BLNodeInComponent[w] = 1;
        }

        ReachSetForwList(BLResCompPtr,BLNodeList,
                         BLComponentNr,BLNodeListSize);

        for (u1=1; u1<=BLNodeListSize; u1++)
        {
          u2 = BLNodeList[u1];
          BLNodeInComponent[u2] = 0;
        }
      }
    }
    else
    if ((BLNUMBER[w] < BLNUMBER[v]) && (w != u))
    {
      BLStackSize++;
      BLStackLeft[BLStackSize]=v;
      BLStackRight[BLStackSize]=w;

      if (BLNUMBER[w] < BLLOWPT[v])
      {
        BLLOWPT[v] = BLNUMBER[w];
      }
    }
  }
}

void ComputeBlocks(ReachPtr RPtr, ReachPtr ResultPtr,
                   long long n, long long *NoOfBlocks)
{
  long long i,EdgesDim;

  BLNodeInComponent = MemGetCV(n+1);
  BLNUMBER = MemGetIV(n+1);
  BLLOWPT = MemGetIV(n+1);
  BLNodeList = MemGetIV(n+1);

  EdgesDim = 0;
  for (i=1; i<=n; i++)
  EdgesDim += (RPtr->LP[i].CFN);

  BLStackLeft = MemGetIV(EdgesDim+1);
  BLStackRight = MemGetIV(EdgesDim+1);

  BLCounter=0;

  for (i=1; i<=n; i++)
  {
    BLNUMBER[i]=0;
    BLNodeInComponent[i]=0;
  }

  BLStackSize = 0;
  BLComponentNr = 0;

  BLAdjPtr = RPtr;
  BLResCompPtr = ResultPtr;

  for (i=1; i<=n; i++)
  {
    if (BLNUMBER[i]==0)
    BiConnect(i,0);
  }

  *NoOfBlocks = BLComponentNr;

  MemFree(BLNodeInComponent);
  MemFree(BLNUMBER);
  MemFree(BLLOWPT);
  MemFree(BLNodeList);

  MemFree(BLStackLeft);
  MemFree(BLStackRight);
}

