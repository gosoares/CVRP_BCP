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

#ifndef _H_CNSTRMGR
#define _H_CNSTRMGR

/* Definition of constants for identification of constraint types. */

#define CMGR_CT_MIN_ROUTES 101
#define CMGR_CT_NODE_DEGREE 102 /* Degree = 2 for each customer. */
#define CMGR_CT_CAP 103         /* Capacity constraint. */
#define CMGR_CT_GENCAP 104      /* Generalized capacity constraint. */
#define CMGR_CT_FCI 104         /* For public version. */
#define CMGR_CT_TWOMATCHING 105
#define CMGR_CT_COMB 106
#define CMGR_CT_STR_COMB 107 /* Strengthened comb. */
#define CMGR_CT_HYPOTOUR 108
#define CMGR_CT_EXT_HYPOTOUR 109
#define CMGR_CT_MSTAR 110             /* Homogeneous multistar. */
#define CMGR_CT_WMSTAR 111            /* Weak multistar. */
#define CMGR_CT_DJCUT 112             /* Disjunctive cut. */
#define CMGR_CT_GOMORY 113            /* By variable numbers */
#define CMGR_CT_TWOEDGES_HYPOTOUR 114 /* 2EH inequality */
#define CMGR_BT_CLIQUE_DOWN 201       /* x(S:S) <= RHS */
#define CMGR_BT_CLIQUE_UP 202         /* x(S:S) >= RHS */
#define CMGR_BT_STAR_DOWN 301         /* x(i:F) <=RHS */
#define CMGR_BT_STAR_UP 302           /* x(i:F) >=RHS */

#define CMGR_CT_SLB 401 /* x(F) >= RHS. Simple lower bound */

#include <cstdio>

typedef struct {
    long long CType; /* Constraint Type. */
    long long Key;
    long long IntListSize;
    long long* IntList;
    long long ExtListSize;
    long long* ExtList;
    long long CListSize;
    long long* CList;
    double* CoeffList;
    long long A, B, L; /* For MSTARs: Lambda=L/B, Sigma=A/B. */
    double RHS;
    long long BranchLevel;
    long long GlobalNr;
} CnstrRecord;
typedef CnstrRecord* CnstrPointer;

typedef CnstrPointer* CnstrPointerList;

typedef struct {
    CnstrPointerList CPL;
    long long Dim; /* Vector is zero-based (0..Dim-1). */
    long long Size;
} CnstrMgrRecord;
typedef CnstrMgrRecord* CnstrMgrPointer;

void CMGR_CreateCMgr(CnstrMgrPointer* CMP, long long Dim);
void CMGR_FreeMemCMgr(CnstrMgrPointer* CMP);
void CMGR_AddCnstr(CnstrMgrPointer CMP, long long CType, long long Key, long long IntListSize, long long* IntList,
                   double RHS);
void CMGR_AddBranchCnstr(CnstrMgrPointer CMP, long long CType, long long Key, long long IntListSize, long long* IntList,
                         double RHS, long long BranchLevel);
void CMGR_AddExtCnstr(CnstrMgrPointer CMP, long long CType, long long Key, long long IntListSize, long long* IntList,
                      long long ExtListSize, long long* ExtList, double RHS);
void CMGR_AddExplicitCnstr(CnstrMgrPointer CMP, long long CType, long long Key, long long ListSize, long long* IntList,
                           long long* ExtList, double* CoeffList, double RHS);
void CMGR_AddGomoryCnstr(CnstrMgrPointer CMP, long long CType, long long Key, long long ListSize,
                         long long* IntList,             /* Variable numbers */
                         double* CoeffList, double RHS); /* >= RHS */
void CMGR_AppendCMP(CnstrMgrPointer Sink, CnstrMgrPointer Source);
void CMGR_SearchCap(CnstrMgrPointer CMP, long long IntListSize, long long* IntList, char* CapExists);
void CMGR_AddMStar(CnstrMgrPointer CMP, long long CType, long long Key, long long IntListSize, long long* IntList,
                   long long ExtListSize, long long* ExtList, long long A, long long B, long long L);
void CMGR_AddPartialMStar(CnstrMgrPointer CMP, long long CType, long long Key, long long IntListSize,
                          long long* IntList, long long ExtListSize, long long* ExtList, long long CListSize,
                          long long* CList, long long A, long long B, long long L);
void CMGR_MoveCnstr(CnstrMgrPointer SourcePtr, CnstrMgrPointer SinkPtr, long long SourceIndex, long long SinkIndex);
void CMGR_ClearCnstr(CnstrMgrPointer SourcePtr, long long Index);
void CMGR_CompressCMP(CnstrMgrPointer CMP);
void CMGR_ChgRHS(CnstrMgrPointer CMP, long long CnstrNr, double NewRHS);
void CMGR_SaveCMP(FILE* f, CnstrMgrPointer CMP, char MatchType, long long CnstrType, char WriteLabel, long long Label);
void CMGR_WriteCMP(CnstrMgrPointer CMP, long long MinCNr);

#endif
