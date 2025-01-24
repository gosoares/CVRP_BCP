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

#ifndef _H_MXF
#define _H_MXF

typedef void *MaxFlowPtr;

void MXF_WriteArcList(MaxFlowPtr Ptr);
void MXF_WriteNodeList(MaxFlowPtr Ptr);
void MXF_InitMem(MaxFlowPtr *Ptr,
                 long long TotalNodes,
                 long long TotalArcs);
void MXF_FreeMem(MaxFlowPtr Ptr);
void MXF_ClearNodeList(MaxFlowPtr Ptr);
void MXF_ClearArcList(MaxFlowPtr Ptr);
void MXF_SetNodeListSize(MaxFlowPtr Ptr,
                         long long TotalNodes);
void MXF_AddArc(MaxFlowPtr Ptr,
                long long Tail,
                long long Head,
                long long Capacity);
void MXF_ChgArcCap(MaxFlowPtr Ptr,
                   long long Tail,
                   long long Head,
                   long long Capacity);
void MXF_CreateMates(MaxFlowPtr Ptr);

void MXF_CreateArcMap(MaxFlowPtr Ptr);

void MXF_SolveMaxFlow(MaxFlowPtr Ptr,
                      char InitByZeroFlow,
                      long long Source,
                      long long Sink,
                      long long *CutValue,
                      char GetSinkSide,
                      long long *NodeListSize,
                      long long *NodeList);

void MXF_GetNetworkSize(MaxFlowPtr Ptr,
                        long long *Nodes,
                        long long *Arcs);

void MXF_GetCurrentFlow(MaxFlowPtr Ptr,
                        long long *ArcResidualCapacity,
                        long long *NodeExcess);

void MXF_SetFlow(MaxFlowPtr Ptr,
                 long long *ArcResidualCapacity,
                 long long *NodeExcess);

void MXF_ComputeGHCutTree(MaxFlowPtr Ptr,
                          long long CenterNode,
                          long long *CutValue,
                          long long *NextOnPath);

#endif
