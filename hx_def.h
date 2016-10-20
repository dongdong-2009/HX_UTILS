#ifndef __COMPILER_H__
#define __COMPILER_H__

//=====================================================================
#ifndef uint
#define byte	unsigned char
#define uchar	unsigned char
#define ushort	unsigned short
#define uint	unsigned int
#define ulong	unsigned long
#define uint64	unsigned long long
#endif

#ifndef WORD
#define BYTE	unsigned char
#define WORD	unsigned short
#define DWORD	unsigned int
#define QWORD	unsigned long long
#endif

#ifndef BOOL
#define BOOL	unsigned int
#endif

#ifndef NULL
#define NULL    ((void *)0L)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif	

#endif