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
#include "memmod.h"
#include "basegrph.h"
#include "sort.h"
#include "cnstrmgr.h"
#include "grsearch.h"

long long *HPMSTAR_MinVVector = NULL;

void HPMSTAR_CreateMinVVector(long long DemandSum, long long CAP)
{
  long long i,MinV;

  if (HPMSTAR_MinVVector != NULL) return;

  HPMSTAR_MinVVector = MemGetIV(DemandSum+1);

  HPMSTAR_MinVVector[0] = 1;
  MinV = 1;

  for (i=1; i<=DemandSum; i++)
  {
    if (((i % CAP) == 1) && (i >= 2)) MinV++;
    HPMSTAR_MinVVector[i] = MinV;
  }
}

long long HPM_CalcMinV(long long DemandSum, long long CAP)
{
  long long MinV,CAPSum;

  if (HPMSTAR_MinVVector != NULL)
  {
    return HPMSTAR_MinVVector[DemandSum];
  }

  MinV = 1;
  CAPSum = CAP;

  while (CAPSum < DemandSum)
  {
    MinV++;
    CAPSum += CAP;
  }

  return MinV;
}

void HPMSTAR_ReduceFrac(long long *A, long long *B)
{
  long long Divisor,MaxDivisor;

  MaxDivisor = *A;
  if ((*B) < (*A)) MaxDivisor = *B;
  if (MaxDivisor == 1) return;

  for (Divisor=MaxDivisor; Divisor>=2; Divisor--)
  {
    if ((((*A) % Divisor) == 0) && (((*B) % Divisor) == 0))
    {
      *A /= Divisor;
      *B /= Divisor;
      return;
    }
  }
}

void HPMSTAR_DeriveCutsFromPolygon(long long MaxAlpha,
                                   long long *LB,
                                   long long *NoOfCuts,  /* Return */
                                   long long *A,         /* - */
                                   long long *B,         /* - */
                                   long long *L,         /* - */
                                   long long *AlphaAtLB) /* - */

{
  long long i;
  long long MinLB,MaxLB;
  long long OriginLB,OriginAlpha,ThisLB,ThisAlpha,JumpLB;
  long long DeltaLB,BestDeltaLB,DeltaAlpha,BestDeltaAlpha;

  *NoOfCuts = 0;

  MinLB = LB[0];
  MaxLB = LB[MaxAlpha];

  if (MaxLB <= MinLB) return;

  for (i=MinLB; i<=MaxLB; i += 2) AlphaAtLB[i] = 0;
  for (i=0; i<=MaxAlpha; i++) AlphaAtLB[LB[i]] = i;

  OriginLB = MinLB;

  while (OriginLB < MaxLB)
  {
    OriginAlpha = AlphaAtLB[OriginLB];

    BestDeltaLB = MaxLB - OriginLB;
    BestDeltaAlpha = AlphaAtLB[MaxLB] - OriginAlpha;

    for (ThisLB=MaxLB-2; ThisLB>OriginLB; ThisLB -= 2)
    {
      ThisAlpha = AlphaAtLB[ThisLB];

      DeltaLB = ThisLB - OriginLB;
      DeltaAlpha = ThisAlpha - OriginAlpha;

      if ((DeltaLB * BestDeltaAlpha) < (DeltaAlpha * BestDeltaLB))
      {
        BestDeltaLB = DeltaLB;
        BestDeltaAlpha = DeltaAlpha;
      }
    }

    (*NoOfCuts)++;

    JumpLB = BestDeltaLB;
    HPMSTAR_ReduceFrac(&BestDeltaLB,&BestDeltaAlpha);

    /* Sigma = A/B, Lambda = L/B. */
    A[*NoOfCuts] = BestDeltaLB;
    B[*NoOfCuts] = BestDeltaAlpha;
    L[*NoOfCuts] = (B[*NoOfCuts] * OriginLB) - (A[*NoOfCuts] * OriginAlpha);

    if ((A[*NoOfCuts] <= B[*NoOfCuts]) && (L[*NoOfCuts] <= 0))
    { /* Dominated */
      (*NoOfCuts)--;
    }

    OriginLB += JumpLB;
  }
}

void HPMSTAR_ComputeMaxAlpha(long long CListSize,
                             long long TListSize,
                             long long CTDemandSum,
                             long long CAP,
                             long long *MaxAlpha)
{
  long long MinV;

  MinV = HPM_CalcMinV(CTDemandSum,CAP);

  *MaxAlpha = (CListSize + TListSize - MinV);
  if (*MaxAlpha > (CListSize * 2)) *MaxAlpha = CListSize * 2;
  if (*MaxAlpha > (TListSize * 2)) *MaxAlpha = TListSize * 2;
}

void HPMSTAR_ComputeLBValues(long long MaxAlpha,
                             long long CAP,
                             long long SDemandSum,
                             long long TSize,
                             long long *SortedTDemand,  /* increasing */
                             long long TDemandSum,
                             long long CSize,
                             long long *SortedCDemand,  /* decreasing */
                             long long *LB)
{
  long long i,r,Alpha;
  long long BNDa, BNDb, BNDc, BNDExpand, BNDNew;
  long long DemandSumExpand, MinVExpand, CAPSumExpand, AccNewDemand;
  long long MinVNew, CAPSumNew;
  long long STMinV, SMinV;
  long long STDemandSum;

  STDemandSum = SDemandSum + TDemandSum;

  SMinV = HPM_CalcMinV(SDemandSum,CAP);
  STMinV = HPM_CalcMinV(STDemandSum,CAP);

  BNDb = SMinV * 2;

  DemandSumExpand = SDemandSum;
  MinVExpand = SMinV;
  CAPSumExpand = SMinV * CAP;

  for (Alpha=0; Alpha<=MaxAlpha; Alpha++)
  {
    BNDa = Alpha / 2;
    if ((Alpha % 2) == 1) BNDa++;
    BNDa *= 2;

    BNDc = (STMinV + Alpha - TSize) * 2;

    LB[Alpha] = BNDa;
    if (LB[Alpha] < BNDb) LB[Alpha] = BNDb;
    if (LB[Alpha] < BNDc) LB[Alpha] = BNDc;

    if ((Alpha >= 1) && (Alpha <= TSize))
    {
      DemandSumExpand += SortedTDemand[Alpha];

      while (CAPSumExpand < DemandSumExpand)
      {
        MinVExpand++;
        CAPSumExpand += CAP;
      }

      BNDExpand = MinVExpand * 2;
      if (LB[Alpha] < BNDExpand) LB[Alpha] = BNDExpand;
    }

    if (CSize < TSize)
    if ((Alpha > CSize) && (Alpha <= (CSize*2)))
    {
      r = Alpha - CSize;

      if (r == 1)
      {
        AccNewDemand = SDemandSum - SortedCDemand[1];
        for (i=1; i<=(CSize-1); i++)
        AccNewDemand += SortedTDemand[i];

        MinVNew = HPM_CalcMinV(AccNewDemand,CAP);
        CAPSumNew = MinVNew * CAP;
      }
      else
      {
        AccNewDemand -= SortedCDemand[r];
        AccNewDemand -= SortedTDemand[CSize+1-r];

        while ((CAPSumNew - CAP) >= AccNewDemand)
        {
          MinVNew--;
          CAPSumNew -= CAP;
        }
      }

      BNDNew = (MinVNew + r) * 2;

      if (BNDNew > LB[Alpha])
      {
        LB[Alpha] = BNDNew;
      }
    }
  }
}

void HPMSTAR_CalcNextCSize(long long TSize,
                           long long TDemandSum,
                           long long CAP,
                           long long *CDemand, /* decreasing */
                           long long *CSize)
{
  long long i;
  long long InitMaxAlpha,NewMaxAlpha;
  long long CTDemandSum;

  if (*CSize >= TSize)
  {
    CTDemandSum = TDemandSum;
    for (i=1; i<=(*CSize); i++) CTDemandSum += CDemand[i];

    HPMSTAR_ComputeMaxAlpha(*CSize,TSize,CTDemandSum,CAP,&InitMaxAlpha);

    NewMaxAlpha = InitMaxAlpha;
    while ((*CSize >= TSize) && (NewMaxAlpha >= InitMaxAlpha))
    {
      CTDemandSum -= CDemand[*CSize];
      (*CSize)--;
      HPMSTAR_ComputeMaxAlpha(*CSize,TSize,CTDemandSum,CAP,&NewMaxAlpha);
    }
  }
  else
  {
    (*CSize)--;
  }
}

void HPMSTAR_GetCutsForSTC(long long SDemandSum,
                           long long TCard,
                           long long *SortedTDemand,  /* increasing */
                           long long TDemandSum,
                           long long CCard,
                           long long *SortedCDemand,  /* decreasing */
                           long long CDemandSum,
                           long long CAP,
                           long long *NoOfCuts,  /* Return */
                           long long *A,         /* - */
                           long long *B,         /* - */
                           long long *L,         /* - */
                           long long *MinLB,
                           long long *MaxLB,
                           long long *AlphaAtLB) /* - */
{
  long long MaxAlpha,CTDemandSum;
  long long *LB;

  CTDemandSum = CDemandSum + TDemandSum;
  HPMSTAR_ComputeMaxAlpha(CCard,TCard,CTDemandSum,CAP,&MaxAlpha);

  LB = MemGetIV(MaxAlpha+1);
  HPMSTAR_ComputeLBValues(MaxAlpha,CAP,
                          SDemandSum,
                          TCard,SortedTDemand,TDemandSum,
                          CCard,SortedCDemand,
                          LB);

  HPMSTAR_DeriveCutsFromPolygon(MaxAlpha,LB,
                                NoOfCuts,
                                A,B,L,
                                AlphaAtLB);

  *MinLB = LB[0];
  *MaxLB = LB[MaxAlpha];

  MemFree(LB);
}

void HPMSTAR_CheckNonUnitHPMsForSV5(ReachPtr SupportPtr, /* Original support graph. */
                                    long long NoOfCustomers,
                                    long long *Demand,
                                    long long CAP,
                                    long long *DemandIndex,
                                    long long *SList,
                                    long long SSize,
                                    double **XMatrix,
                                    char SearchPartialMStars,
                                    long long MaxCSize,
                                    double *MaxViolation,
                                    long long MaxTotalCuts,
                                    CnstrMgrPointer CutsCMP)
{
  /* Including partial HPMs */
  long long Idx,i,j,k,CutNr;
  long long TSize,CSize,OrigTSize;
  long long TIdx,CIdx;
  long long TXBestNode,RemovedNodes;
  long long SDemandSum,TDemandSum,CDemandSum;
  long long NoOfCuts;
  long long MinLB,MaxLB;
  long long MaxAlpha;
  double ActualCTVal,ActualCTValCopy;
  double SBoundary;
  double XVal;
  double Lambda,Sigma,LHS,RHS;
  char *InS, *InT, *InC, *InTCopy;
  long long *TList, *CList;
  long long *SortedTDemand, *SortedCDemand;
  long long *AVector, *BVector, *LVector;
  long long *AlphaAtLB;
  long long *TXIndex, *CXIndex;
  double *TXVal, *CXVal, *CXScore;

  if (SearchPartialMStars)
  {
    if (MaxCSize >= SSize)
    {
      MaxCSize = SSize - 1;

      if (MaxCSize <= 1)
      {
        return;
      }
    }
  }

  InS = MemGetCV(NoOfCustomers+1);
  InT = MemGetCV(NoOfCustomers+1);
  InC = MemGetCV(NoOfCustomers+1);
  InTCopy = MemGetCV(NoOfCustomers+1);
  TList = MemGetIV(NoOfCustomers+1);
  CList = MemGetIV(NoOfCustomers+1);
  TXIndex = MemGetIV(NoOfCustomers+1);
  CXIndex = MemGetIV(NoOfCustomers+1);
  TXVal = MemGetDV(NoOfCustomers+1);
  CXVal = MemGetDV(NoOfCustomers+1);
  CXScore = MemGetDV(NoOfCustomers+1);

  SortedTDemand = MemGetIV(NoOfCustomers+1);
  SortedCDemand = MemGetIV(NoOfCustomers+1);

  AVector = MemGetIV(NoOfCustomers+1);
  BVector = MemGetIV(NoOfCustomers+1);
  LVector = MemGetIV(NoOfCustomers+1);
  AlphaAtLB = MemGetIV((NoOfCustomers*2)+1);

  for (i=1; i<=NoOfCustomers; i++)
  {
    InS[i] = 0;
    InT[i] = 0;
    InC[i] = 0;
    TXVal[i] = 0.0;
    CXVal[i] = 0.0;
  }

  SDemandSum = 0;
  for (i=1; i<=SSize; i++)
  {
    j = SList[i];
    InS[j] = 1;
    SDemandSum += Demand[j];
  }

  SBoundary = 0.0;

  for (Idx=1; Idx<=SSize; Idx++)
  {
    i = SList[Idx];

    for (j=1; j<=SupportPtr->LP[i].CFN; j++)
    {
      k = SupportPtr->LP[i].FAL[j];
      if (k > NoOfCustomers)
      SBoundary += XMatrix[i][k];
      else
      if (InS[k] == 0)
      {
        XVal = XMatrix[i][k];
        SBoundary += XVal;
        CXVal[i] += XVal;
      }
    }
  }

  if (SearchPartialMStars == 0)
  {
    for (i=1; i<=SSize; i++) CXIndex[i] = SList[i];
  }
  else
  {
    for (i=1; i<=NoOfCustomers; i++)
    {
      CXIndex[i] = i;

      if (InS[i])
      CXScore[i] = CXVal[i];
      else
      CXScore[i] = -1.0;
    }

    SortIndexDVDec(CXIndex,CXScore,NoOfCustomers);
  }

  if (SearchPartialMStars)
  CSize = MaxCSize;
  else
  CSize = SSize;

  ActualCTVal = 0;

  for (Idx=1; Idx<=CSize; Idx++)
  {
    i = CXIndex[Idx];
    InC[i] = 1;

    for (j=1; j<=SupportPtr->LP[i].CFN; j++)
    {
      k = SupportPtr->LP[i].FAL[j];
      if (k <= NoOfCustomers)
      if (InS[k] == 0)
      {
        InT[k] = 1;
        TXVal[k] += XMatrix[i][k];
        ActualCTVal += XMatrix[i][k];
      }
    }
  }

  do
  {
    TIdx = 0;
    CIdx = CSize+1;

    TDemandSum = 0;
    CDemandSum = 0;

    for (i=1; i<=NoOfCustomers; i++)
    {
      j = DemandIndex[i];

      if (InT[j])
      {
        TIdx++;
        SortedTDemand[TIdx] = Demand[j];
        TXIndex[TIdx] = j;
        TDemandSum += Demand[j];
      }

      if (InC[j])
      {
        CIdx--;
        SortedCDemand[CIdx] = Demand[j];
        CList[CIdx] = j;
        CDemandSum += Demand[j];
      }
    }

    TSize = TIdx;
    OrigTSize = TSize;

    if (TSize >= 2) SortIndexDVInc(TXIndex,TXVal,TSize);

    RemovedNodes = 0;
    ActualCTValCopy = ActualCTVal;
    for (i=1; i<=NoOfCustomers; i++) InTCopy[i] = InT[i];

    while (TSize >= 1)
    {
      HPMSTAR_GetCutsForSTC(SDemandSum,
                            TSize,SortedTDemand,TDemandSum,
                            CSize,SortedCDemand,CDemandSum,
                            CAP,
                            &NoOfCuts,
                            AVector,BVector,LVector,
                            &MinLB,
                            &MaxLB,
                            AlphaAtLB);

      HPMSTAR_ComputeMaxAlpha(CSize,TSize,TDemandSum+CDemandSum,
                              CAP,&MaxAlpha);

      for (CutNr=1; CutNr<=NoOfCuts; CutNr++)
      {
        Lambda = (1.0 * LVector[CutNr]) / (1.0 * BVector[CutNr]);
        Sigma  = (1.0 * AVector[CutNr]) / (1.0 * BVector[CutNr]);

        RHS = (ActualCTValCopy * Sigma) + Lambda;
        LHS = SBoundary;

        if (LHS < (RHS - 0.01))
        {
          if ((RHS - LHS) > (*MaxViolation))
          {
            *MaxViolation = (RHS - LHS);
          }

          TSize = 0;
          for (i=1; i<=NoOfCustomers; i++)
          if (InTCopy[i]) TList[++TSize] = i;

          CSize = 0;
          for (i=1; i<=NoOfCustomers; i++)
          if (InC[i]) CList[++CSize] = i;

          CMGR_AddPartialMStar(CutsCMP,
                                CMGR_CT_MSTAR,0,
                                SSize,SList,
                                TSize,TList,
                                CSize,CList,
                                AVector[CutNr],
                                BVector[CutNr],
                                LVector[CutNr]);

          if (CutsCMP->Size >= MaxTotalCuts) goto EndOfHPMSV5;
        }
      }

      RemovedNodes++;
      TXBestNode = TXIndex[RemovedNodes];

      InTCopy[TXBestNode] = 0;
      ActualCTValCopy -= TXVal[TXBestNode];

      TIdx = 0;
      TDemandSum = 0;

      for (i=1; i<=NoOfCustomers; i++)
      {
        j = DemandIndex[i];

        if (InTCopy[j])
        {
          TIdx++;
          SortedTDemand[TIdx] = Demand[j];
          TDemandSum += Demand[j];
        }
      }

      TSize = TIdx;
    } /* while TSize >= 1 */

    if (SearchPartialMStars == 0) break;

    if (CSize <= 2) break;

    while (CSize > 2)
    {
      i = CXIndex[CSize];
      InC[i] = 0;
      CSize--;

      for (j=1; j<=SupportPtr->LP[i].CFN; j++)
      {
        k = SupportPtr->LP[i].FAL[j];
        if (k <= NoOfCustomers)
        if (InS[k] == 0)
        {
          TXVal[k] -= XMatrix[i][k];
          ActualCTVal -= XMatrix[i][k];
          if (TXVal[k] < 0.01)
          {
            ActualCTVal -= TXVal[k];
            InT[k] = 0;
          }
        }
      }

      if (CSize <= MaxCSize) break;
    }

  } while (CSize >= 2);

  EndOfHPMSV5:

  MemFree(InS);
  MemFree(InT);
  MemFree(InC);
  MemFree(InTCopy);
  MemFree(TList);
  MemFree(CList);
  MemFree(TXIndex);
  MemFree(CXIndex);
  MemFree(TXVal);
  MemFree(CXVal);
  MemFree(CXScore);

  MemFree(SortedTDemand);
  MemFree(SortedCDemand);

  MemFree(AVector);
  MemFree(BVector);
  MemFree(LVector);
  MemFree(AlphaAtLB);
}

void HPMSTAR_CalcQXMatrix(ReachPtr SupportPtr, /* Original support graph. */
                          long long NoOfCustomers,
                          long long *Demand,         /* Original demand vector. */
                          double **XMatrix,
                          long long NoOfSuperNodes,
                          long long *SuperNodeNr,
                          double **QXMatrix)
{
  long long i,j,k,a,b;
  double XVal;

  for (i=1; i<=NoOfSuperNodes; i++)
  for (j=1; j<=NoOfSuperNodes; j++)
  QXMatrix[i][j] = 0.0;

  for (i=1; i<=NoOfCustomers; i++)
  {
    for (j=1; j<=SupportPtr->LP[i].CFN; j++)
    {
      k = SupportPtr->LP[i].FAL[j];
      if (k > NoOfCustomers) continue;

      XVal = XMatrix[i][k];
      a = SuperNodeNr[i];
      b = SuperNodeNr[k];

      if (a != b)
      {
        QXMatrix[a][b] += (XVal * Demand[k]);
        QXMatrix[b][a] += (XVal * Demand[i]);
      }
    }
  }
}

void HPMSTAR_DirectX(ReachPtr SupportPtr, /* Original support graph. */
                     ReachPtr SAdjRPtr,   /* Shrunk support graph. */
                     long long NoOfCustomers,
                     long long *Demand,         /* Original demand vector. */
                     long long CAP,
                     long long NoOfSuperNodes,
                     double *XInSuperNode,
                     double **XMatrix,
                     double **SMatrix,
                     ReachPtr SuperNodesRPtr,
                     char SelectionRule,
                     long long MaxTotalCuts,
                     char SearchPartialMStars,
                     CnstrMgrPointer CutsCMP,
                     double *MaxViolation)
{
  const double EpsViolation = 0.01;
  char CutAdded;
  char CallBackAntiSets;
  long long i,j,k;
  long long Source,BestNode,NodeSum;
  long long MinCandidateIdx,MaxCandidateIdx;
  long long OrigNodesInSet;
  long long Label;
  long long MaxCSize;
  double XInSet,Score,BestScore,SBoundary,SToDepot;
  double LHS,RHS;
  long long SListSize;
  long long SDemandSum;
  long long *Node, *Pos, *NodeLabel;
  long long *DemandIndex;
  long long *SList;
  long long *SuperNodeNr, *SuperNodeSize, *SuperDemand;
  double *XVal;
  double *QXVal;
  double **QXMatrix;

  long long GeneratedAntiSets;
  ReachPtr AntiSetsRPtr;

  MaxCSize = 4;

  SList = MemGetIV(NoOfCustomers+1);

  Node = MemGetIV(NoOfSuperNodes+1);
  Pos = MemGetIV(NoOfSuperNodes+1);
  NodeLabel = MemGetIV(NoOfSuperNodes+1);

  SuperNodeNr = MemGetIV(NoOfCustomers+1);
  SuperNodeSize = MemGetIV(NoOfSuperNodes+1);
  SuperDemand = MemGetIV(NoOfSuperNodes+1);

  XVal = MemGetDV(NoOfSuperNodes+1);
  QXVal = MemGetDV(NoOfSuperNodes+1);

  QXMatrix = NULL;

  DemandIndex = MemGetIV(NoOfCustomers+1);
  for (i=1; i<=NoOfCustomers; i++) DemandIndex[i] = i;
  SortIndexIVInc(DemandIndex,Demand,NoOfCustomers);

  ReachInitMem(&AntiSetsRPtr,NoOfSuperNodes);
  GeneratedAntiSets = 0;

  for (i=1; i<=NoOfSuperNodes; i++)
  {
    Node[i] = i;
    Pos[i] = i;
    NodeLabel[i] = 0;
  }

  for (i=1; i<=NoOfSuperNodes; i++)
  {
    SuperDemand[i] = 0;
    for (j=1; j<=SuperNodesRPtr->LP[i].CFN; j++)
    {
      k = SuperNodesRPtr->LP[i].FAL[j];
      SuperNodeNr[k] = i;
      SuperDemand[i] += Demand[k];
    }

    SuperNodeSize[i] = SuperNodesRPtr->LP[i].CFN;
  }

  Label = 0;

  if (SelectionRule == 3)
  {
    QXMatrix = MemGetDM(NoOfSuperNodes+1,NoOfSuperNodes+1);
    HPMSTAR_CalcQXMatrix(SupportPtr,
                         NoOfCustomers,
                         Demand,
                         XMatrix,
                         NoOfSuperNodes,
                         SuperNodeNr,
                         QXMatrix);
  }

  for (Source=1; Source<=NoOfSuperNodes; Source++)
  {
    /* Put Source in position 1. */
    GRSEARCH_SwapNodesInPos(Node,Pos,1,Pos[Source]);

    SDemandSum = SuperDemand[Source];
    OrigNodesInSet = SuperNodeSize[Source];
    XInSet = XInSuperNode[Source];
    SToDepot = SMatrix[Source][NoOfSuperNodes+1];
    SBoundary = 2.0 * (OrigNodesInSet - XInSet);

    if (SelectionRule == 3)
    {
      for (i=1; i<=NoOfSuperNodes; i++)
      QXVal[i] = QXMatrix[Source][i];
    }

    MinCandidateIdx = 2;
    MaxCandidateIdx = 1; /* Max < Min <=> no candidates. */

    for (j=1; j<=SAdjRPtr->LP[Source].CFN; j++)
    {
      k = SAdjRPtr->LP[Source].FAL[j];
      if (k <= NoOfSuperNodes)
      {
        MaxCandidateIdx++;
        GRSEARCH_SwapNodesInPos(Node,Pos,MaxCandidateIdx,Pos[k]);
        XVal[k] = SMatrix[Source][k];
      }
    }

    SListSize = 0;
    for (j=1; j<=SuperNodesRPtr->LP[Source].CFN; j++)
    {
      k = SuperNodesRPtr->LP[Source].FAL[j];
      SList[++SListSize] = k;
    }

    if (SListSize >= 2)
    {
      HPMSTAR_CheckNonUnitHPMsForSV5(SupportPtr,
                                      NoOfCustomers,
                                      Demand, /* Original demand */
                                      CAP,
                                      DemandIndex,
                                      SList,
                                      SListSize,
                                      XMatrix,
                                      SearchPartialMStars,
                                      MaxCSize,
                                      MaxViolation,
                                      MaxTotalCuts,
                                      CutsCMP);
    }

    if (CutsCMP->Size >= MaxTotalCuts)
    {
      goto EndOfDirectX;
    }

    NodeSum = Source;

    CallBackAntiSets = 1;

    BestNode = 1;
    CutAdded = 0;

    while ((MinCandidateIdx <= MaxCandidateIdx) &&
           (BestNode > 0) &&
           (CutsCMP->Size < MaxTotalCuts))
    {
      Label++;

      if (Label > 10000000)
      {
        for (i=1; i<=NoOfSuperNodes; i++) NodeLabel[i] = 0;
        Label = 1;
      }

      if (CallBackAntiSets)
      {
        GRSEARCH_GetInfeasExt(Pos,
                              MinCandidateIdx,MaxCandidateIdx,
                              NoOfSuperNodes,
                              NodeSum,
                              AntiSetsRPtr,
                              GeneratedAntiSets,
                              NodeLabel,
                              Label,
                              &CallBackAntiSets);
      }

      BestNode = 0;
      BestScore = 0.0;

      for (i=MinCandidateIdx; i<=MaxCandidateIdx; i++)
      {
        if (NodeLabel[Node[i]] == Label)
        {
          continue;
        }

        /* Calculate score for node Node[i] */
        if (SelectionRule == 1)
        Score = XVal[Node[i]];
        else
        if (SelectionRule == 2)
        Score = XVal[Node[i]] - SMatrix[Node[i]][NoOfSuperNodes+1];
        else
        if (SelectionRule == 3)
        {
          RHS = SDemandSum + SuperDemand[Node[i]];
          RHS -= QXVal[Node[i]]; /* If Node[i] is included in S */
          for (j=1; j<=NoOfSuperNodes; j++)
          if (j != Node[i])
          RHS += QXMatrix[Node[i]][j];

          RHS *= (2.0 / CAP);

          LHS = 2.0 * (OrigNodesInSet + SuperNodeSize[Node[i]] -
                       XInSet - XVal[Node[i]] - XInSuperNode[Node[i]]);

          Score = RHS - LHS; /* = Violation of GLM */
        }

        if ((BestNode == 0) || (Score > BestScore))
        {
          BestNode = Node[i];
          BestScore = Score;
        }
      }

      if (BestNode > 0)
      { /* Include BestNode. */
        GRSEARCH_SwapNodesInPos(Node,Pos,MinCandidateIdx,Pos[BestNode]);
        MinCandidateIdx++;

        NodeSum += BestNode;

        SDemandSum += SuperDemand[BestNode];
        OrigNodesInSet += SuperNodeSize[BestNode];
        XInSet += (XVal[BestNode] + XInSuperNode[BestNode]);
        SToDepot += SMatrix[BestNode][NoOfSuperNodes+1];
        SBoundary = 2.0 * (OrigNodesInSet - XInSet);

        if (SelectionRule == 3)
        {
          for (i=1; i<=NoOfSuperNodes; i++)
          QXVal[i] += QXMatrix[BestNode][i];
        }

        for (j=1; j<=SuperNodesRPtr->LP[BestNode].CFN; j++)
        {
          k = SuperNodesRPtr->LP[BestNode].FAL[j];
          SList[++SListSize] = k;
        }

        HPMSTAR_CheckNonUnitHPMsForSV5(SupportPtr,
                                        NoOfCustomers,
                                        Demand, /* Original demand */
                                        CAP,
                                        DemandIndex,
                                        SList,
                                        SListSize,
                                        XMatrix,
                                        SearchPartialMStars,
                                        MaxCSize,
                                        MaxViolation,
                                        MaxTotalCuts,
                                        CutsCMP);

        if (CutsCMP->Size >= MaxTotalCuts)
        {
          goto EndOfDirectX;
        }

        /* Update X-values and candidate set. */
        for (j=1; j<=SAdjRPtr->LP[BestNode].CFN; j++)
        {
          k = SAdjRPtr->LP[BestNode].FAL[j];
          if (k > NoOfSuperNodes) continue;

          if (Pos[k] > MaxCandidateIdx)
          {
            /* k is a new candidate. */
            XVal[k] = SMatrix[BestNode][k];
            MaxCandidateIdx++;
            GRSEARCH_SwapNodesInPos(Node,Pos,MaxCandidateIdx,Pos[k]);
          }
          else
          if (Pos[k] >= MinCandidateIdx)
          { /* Existing candidate */
            XVal[k] += SMatrix[BestNode][k];
          }
        }
      }
    }

    GeneratedAntiSets++;
    GRSEARCH_AddSet(AntiSetsRPtr,
                    GeneratedAntiSets,
                    MinCandidateIdx-1,
                    Node,1);
  }

  EndOfDirectX:

  MemFree(SList);

  MemFree(Node);
  MemFree(Pos);
  MemFree(NodeLabel);
  MemFree(XVal);
  MemFree(QXVal);

  MemFree(SuperNodeNr);
  MemFree(SuperNodeSize);
  MemFree(SuperDemand);

  MemFree(DemandIndex);

  if (QXMatrix != NULL)
  MemFreeDM(QXMatrix,NoOfSuperNodes+1);

  ReachFreeMem(&AntiSetsRPtr);


}

