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

#ifndef _H_BASEGRPH
#define _H_BASEGRPH

typedef struct
{
  long long CFN; /* Cardinality (number) of Forward Nodes */
  long long CBN; /* Cardinality (number) of Backward Nodes */
  long long FLD; /* Forward List Dimension (memory is allocated for <FLD> records */
  long long BLD; /* Backward List Dimension (memory is allocated for <BLD> records */
  long long *FAL; /* Forward Arc List */
  long long *BAL; /* Backward Arc List */
} ReachNodeRec;

typedef struct
{
  long long n;
  ReachNodeRec *LP; /* List Pointer */
} ReachTopRec;

typedef ReachTopRec *ReachPtr;

typedef struct
{
  long long Row;
  long long CFN;
  long long *FAL;
} CompFReachNodeRec;

typedef struct
{
  long long NoOfRows;
  CompFReachNodeRec *FLP;
} CompFReachRec;

typedef CompFReachRec *CompFReachPtr;

void WriteReachPtr(ReachPtr P);
void ReachInitMem(ReachPtr *P, long long n);
void ReachPtrExpandDim(ReachPtr P, long long NewN);
void ReachClearLists(ReachPtr P);
void ReachClearForwLists(ReachPtr P);
void ReachSetForwList(ReachPtr P, long long *ArcList, long long Row, long long Arcs);
void ReachSetBackwList(ReachPtr P, long long *ArcList, long long Col, long long Arcs);
void ReachCreateInLists(ReachPtr P);
void ReachAddArc(ReachPtr P, long long Row, long long Col);
void ReachAddForwArc(ReachPtr P, long long Row, long long Col);
void CopyReachPtr(ReachPtr SourcePtr, ReachPtr *SinkPtr);
void ReachFreeMem(ReachPtr *P);
void WriteCompPtr(CompFReachPtr P);
void CopyReachPtrToCompPtr(ReachPtr SourcePtr, CompFReachPtr *SinkPtr);
void CompFPtrFreeMem(CompFReachPtr *P);

#endif

