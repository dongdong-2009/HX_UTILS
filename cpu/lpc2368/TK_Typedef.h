/*******************************************************************
 *    TCPS.,Co. (c) Copyright 2008
 *
 *    FILENAME: TK_typedef volatile.h
 *
 *    DESCRIPTION: 程序要用到的定义
 *
 *    AUTHOR: Tiger.gao
 *
 *    HISTORY: 2008-07-18
 *
 *******************************************************************/
#ifndef TK_Typedef_h
#define  TK_Typedef_h
typedef char tk_int8;
typedef unsigned char tk_uint8;
typedef short tk_int16;
typedef unsigned short tk_uint16;
typedef long tk_int32;
typedef unsigned long tk_uint32;
typedef volatile char tk_vint8;
typedef volatile unsigned char tk_vuint8;
typedef volatile short tk_vint16;
typedef volatile unsigned short tk_vuint16;
typedef volatile long tk_vint32;
typedef volatile unsigned long tk_vuint32;
typedef union UI_UC_UION_
{
    unsigned short uiVal;
    unsigned char ucVal[2];
} UI_UC_UNION;
typedef union I_UC_UNION_
{
    tk_int16 iVal;
    tk_uint8 ucVal[2];
} I_UC_UNION;
typedef union I_C_UNION_
{
    tk_int16 iVal;
    tk_int8 cVal[2];
} I_C_UNION;
typedef union UI_C_UNION_
{
    tk_uint16 uiVal;
    tk_int8 cVal[2];
} UI_C_UNION;
typedef union UL_UC_UNION_
{
    tk_uint32 ulVal;
    tk_uint8 ucVal[4];
} UL_UC_UNION;
typedef union UL_C_UNION_
{
    tk_uint32 ulVal;
    tk_int8 ucVal[4];
} UL_C_UNION;
typedef union L_UC_UNION_
{
    tk_int32 lVal;
    tk_uint8 ucVal[4];
} L_UC_UNION;
typedef union L_C_UNION_
{
    tk_int32 lVal;
    tk_int8 cVal[4];
} L_C_UNION;
#endif
