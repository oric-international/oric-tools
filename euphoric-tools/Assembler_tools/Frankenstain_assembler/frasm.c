
/*  A Bison parser, made from frasm.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ACCUM	258
#define	INDEX	259
#define	KOC_BDEF	260
#define	KOC_ELSE	261
#define	KOC_END	262
#define	KOC_ENDI	263
#define	KOC_EQU	264
#define	KOC_IF	265
#define	KOC_INCLUDE	266
#define	KOC_ORG	267
#define	KOC_RESM	268
#define	KOC_SDEF	269
#define	KOC_SET	270
#define	KOC_WDEF	271
#define	KOC_CHSET	272
#define	KOC_CHDEF	273
#define	KOC_CHUSE	274
#define	KOC_CPU	275
#define	KOC_opcode	276
#define	CONSTANT	277
#define	EOL	278
#define	KEOP_AND	279
#define	KEOP_DEFINED	280
#define	KEOP_EQ	281
#define	KEOP_GE	282
#define	KEOP_GT	283
#define	KEOP_HIGH	284
#define	KEOP_LE	285
#define	KEOP_LOW	286
#define	KEOP_LT	287
#define	KEOP_MOD	288
#define	KEOP_MUN	289
#define	KEOP_NE	290
#define	KEOP_NOT	291
#define	KEOP_OR	292
#define	KEOP_SHL	293
#define	KEOP_SHR	294
#define	KEOP_XOR	295
#define	KEOP_locctr	296
#define	LABEL	297
#define	STRING	298
#define	SYMBOL	299
#define	KTK_invalid	300

#line 1 "frasm.y"


/*
HEADER: 	;
TITLE: 		Frankenstein Cross Assemblers;
VERSION: 	2.0;
DESCRIPTION: "	Reconfigurable Cross-assembler producing Intel (TM)
		Hex format object records.  ";
KEYWORDS: 	cross-assemblers, 1805, 2650, 6301, 6502, 6805, 6809, 
		6811, tms7000, 8048, 8051, 8096, z8, z80;
SYSTEM: 	UNIX, MS-Dos ;
FILENAME: 	as6502.y;
WARNINGS: 	"This software is in the public domain.  
		Any prior copyright claims are relinquished.  

		This software is distributed with no warranty whatever.  
		The author takes no responsibility for the consequences 
		of its use.

		Yacc (or Bison) required to compile."  ;
SEE-ALSO: 	as6502.doc,frasmain.c;	
AUTHORS: 	Mark Zenier;
COMPILERS: 	Microport Sys V/AT, ATT Yacc, Turbo C V1.5, Bison (CUG disk 285)
		(previous versions Xenix, Unisoft 68000 Version 7, Sun 3);
*/
/* 65xx instruction generation file */
/* November 17, 1990 */

/*
	description	frame work parser description for framework cross
			assemblers
	history		February 2, 1988
			September 11, 1990 - merge table definition
			September 12, 1990 - short file names
			September 14, 1990 - short variable names
			September 17, 1990 - use yylex as external
*/
#include <stdio.h>
#include "frasmdat.h"
#include "fragcon.h"

#define yylex lexintercept

/*	0000.0000.0000.00xx		address mode selection bits */
#define	ADDRESS		0x3
#define	DIRECT		0x1
#define	EXTENDED	0x2
/*	0000.0000.0000.xx00		index selector bits */
#define	INDEXMASK	0xc
#define	INDEXX		0x4
#define	INDEXY		0x8
/*	0000.0xxx.0000.0000		instruction set enable bits */
#define	INSTCMOS	0x100
#define	INSTROCKWELL	0x200
#define	INST21		0x400
/*					instruction sets */
#define	CPU65		0
#define	CPUR65		INSTROCKWELL
#define	CPU65C		INSTCMOS
#define	CPUR65C		(INSTROCKWELL|INSTCMOS)
#define	CPUR21		(INSTROCKWELL|INST21)
#define ST_INH 0x1
#define ST_ACCUM 0x2
#define ST_EXPR 0x4
#define ST_INDEX 0x8
#define ST_PREINDEX 0x10
#define ST_INDIR 0x20
#define ST_POSTINDEX 0x40
#define ST_IMMED 0x80
#define ST_DIRREL 0x100
	
	int	cpuselect = CPUR65C;
	static char	genbdef[] = "[1=];";
	static char	genwdef[] = "[1=]y"; /* x for normal, y for byte rev */
	char ignosyn[] = "[Xinvalid syntax for instruction";
	char ignosel[] = "[Xinvalid operands/illegal instruction for cpu";

	long	labelloc;
	static int satsub;
	int	ifstkpt = 0;
	int	fraifskip = FALSE;

	struct symel * endsymbol = SYMNULL;


#line 86 "frasm.y"
typedef union {
	int	intv;
	long 	longv;
	char	*strng;
	struct symel *symb;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		122
#define	YYFLAG		-32768
#define	YYNTBASE	54

#define YYTRANSLATE(x) ((unsigned)(x) <= 300 ? yytranslate[x] : 63)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,    53,     2,     2,     2,     2,    51,
    52,    48,    46,    50,    47,     2,    49,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     5,     8,    10,    13,    16,    18,    21,    25,
    29,    32,    34,    36,    38,    42,    45,    48,    50,    53,
    58,    60,    62,    65,    67,    70,    73,    76,    79,    83,
    85,    89,    91,    94,    96,    99,   102,   107,   114,   119,
   126,   130,   135,   138,   141,   144,   147,   150,   154,   158,
   162,   166,   170,   174,   178,   182,   186,   190,   194,   198,
   202,   206,   210,   214,   217,   219,   221,   223,   225,   229
};

static const short yyrhs[] = {    54,
    55,     0,    55,     0,    56,    23,     0,    23,     0,     1,
    23,     0,    42,     7,     0,     7,     0,    11,    43,     0,
    42,     9,    62,     0,    42,    15,    62,     0,    10,    62,
     0,    10,     0,     6,     0,     8,     0,    42,    12,    62,
     0,    12,    62,     0,    42,    17,     0,    19,     0,    19,
    62,     0,    18,    43,    50,    59,     0,    42,     0,    57,
     0,    42,    58,     0,    58,     0,     5,    59,     0,    14,
    60,     0,    16,    59,     0,    13,    62,     0,    59,    50,
    62,     0,    62,     0,    60,    50,    43,     0,    43,     0,
    20,    43,     0,    21,     0,    21,     3,     0,    21,    61,
     0,    21,    61,    50,     4,     0,    21,    51,    61,    50,
     4,    52,     0,    21,    51,    61,    52,     0,    21,    51,
    61,    52,    50,     4,     0,    21,    53,    61,     0,    21,
    61,    50,    61,     0,    46,    62,     0,    47,    62,     0,
    36,    62,     0,    29,    62,     0,    31,    62,     0,    62,
    48,    62,     0,    62,    49,    62,     0,    62,    46,    62,
     0,    62,    47,    62,     0,    62,    33,    62,     0,    62,
    38,    62,     0,    62,    39,    62,     0,    62,    28,    62,
     0,    62,    27,    62,     0,    62,    32,    62,     0,    62,
    30,    62,     0,    62,    35,    62,     0,    62,    26,    62,
     0,    62,    24,    62,     0,    62,    37,    62,     0,    62,
    40,    62,     0,    25,    44,     0,    44,     0,    48,     0,
    22,     0,    43,     0,    51,    61,    52,     0,    61,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   156,   157,   160,   164,   165,   172,   177,   181,   201,   225,
   250,   282,   303,   321,   340,   363,   378,   394,   399,   424,
   489,   502,   505,   518,   524,   533,   541,   550,   566,   571,
   579,   584,   593,   602,   608,   614,   626,   638,   650,   662,
   674,   681,   689,   693,   698,   703,   708,   713,   718,   723,
   728,   733,   738,   743,   748,   753,   758,   763,   768,   773,
   778,   783,   788,   793,   797,   801,   806,   811,   835,   839
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ACCUM",
"INDEX","KOC_BDEF","KOC_ELSE","KOC_END","KOC_ENDI","KOC_EQU","KOC_IF","KOC_INCLUDE",
"KOC_ORG","KOC_RESM","KOC_SDEF","KOC_SET","KOC_WDEF","KOC_CHSET","KOC_CHDEF",
"KOC_CHUSE","KOC_CPU","KOC_opcode","CONSTANT","EOL","KEOP_AND","KEOP_DEFINED",
"KEOP_EQ","KEOP_GE","KEOP_GT","KEOP_HIGH","KEOP_LE","KEOP_LOW","KEOP_LT","KEOP_MOD",
"KEOP_MUN","KEOP_NE","KEOP_NOT","KEOP_OR","KEOP_SHL","KEOP_SHR","KEOP_XOR","KEOP_locctr",
"LABEL","STRING","SYMBOL","KTK_invalid","'+'","'-'","'*'","'/'","','","'('",
"')'","'#'","file","allline","line","labeledline","genline","exprlist","stringlist",
"topexpr","expr", NULL
};
#endif

static const short yyr1[] = {     0,
    54,    54,    55,    55,    55,    56,    56,    56,    56,    56,
    56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
    56,    56,    57,    57,    58,    58,    58,    58,    59,    59,
    60,    60,    56,    58,    58,    58,    58,    58,    58,    58,
    58,    58,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    62,    62
};

static const short yyr2[] = {     0,
     2,     1,     2,     1,     2,     2,     1,     2,     3,     3,
     2,     1,     1,     1,     3,     2,     2,     1,     2,     4,
     1,     1,     2,     1,     2,     2,     2,     2,     3,     1,
     3,     1,     2,     1,     2,     2,     4,     6,     4,     6,
     3,     4,     2,     2,     2,     2,     2,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     2,     1,     1,     1,     1,     3,     1
};

static const short yydefact[] = {     0,
     0,     0,    13,     7,    14,    12,     0,     0,     0,     0,
     0,     0,    18,     0,    34,     4,    21,     0,     2,     0,
    22,    24,     5,    67,     0,     0,     0,     0,    68,    65,
     0,     0,    66,     0,    25,    70,    30,    11,     8,    16,
    28,    32,    26,    27,     0,    19,    33,    35,     0,     0,
    70,     0,     6,     0,     0,     0,    17,    23,     1,     3,
    64,    46,    47,    45,    43,    44,    70,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    70,    70,     0,     9,
    15,    10,    69,    29,    61,    60,    56,    55,    58,    57,
    52,    59,    62,    53,    54,    63,    50,    51,    48,    49,
    31,    20,     0,    69,    37,    70,     0,     0,    38,    40,
     0,     0
};

static const short yydefgoto[] = {    18,
    19,    20,    21,    22,    35,    43,    36,    52
};

static const short yypact[] = {   156,
   -21,   158,-32768,-32768,-32768,   158,   -30,   158,   158,   -28,
   158,   -27,   158,   -26,    82,-32768,    84,   131,-32768,   -20,
-32768,-32768,-32768,-32768,   -25,   158,   158,   158,-32768,-32768,
   158,   158,-32768,   158,   -29,-32768,   184,   184,-32768,   184,
   184,-32768,   -23,   -29,   -19,   184,-32768,-32768,   158,   158,
   -22,   184,-32768,   158,   158,   158,-32768,-32768,-32768,-32768,
-32768,   184,   184,   234,-32768,-32768,   -32,   158,   158,   158,
   158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
   158,   158,   158,   158,    -9,   158,   -40,     0,    14,   184,
   184,   184,-32768,   184,   234,   258,   258,   258,   258,   258,
-32768,   258,   210,-32768,-32768,   210,   261,   261,-32768,-32768,
-32768,   -29,    18,   -18,-32768,    12,   -15,    34,-32768,-32768,
    40,-32768
};

static const short yypgoto[] = {-32768,
    23,-32768,-32768,    25,    -3,-32768,    -1,    -2
};


#define	YYLAST		310


static const short yytable[] = {    37,
   -36,    23,    60,    38,   -39,    40,    41,    44,    37,   113,
    46,   114,    39,    51,    42,    45,    47,   115,    61,    93,
    68,   117,   -41,    62,    63,    64,    85,    89,    65,    66,
    86,   118,    67,   111,   -42,    24,   119,   120,    25,   122,
    59,    58,    26,     0,    27,     0,     0,    87,    88,    28,
     0,    90,    91,    92,     0,     0,    29,    30,     0,    31,
    32,    33,     0,     0,    34,    94,    95,    96,    97,    98,
    99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
   109,   110,   112,    37,    48,     0,     0,   116,     2,     0,
    53,     0,    54,     0,     0,    55,     9,    10,    56,    11,
    57,     0,     0,    24,    15,     0,    25,     0,     0,     0,
    26,     0,    27,     0,     0,     0,     0,    28,     0,     0,
     0,     0,     0,     0,    29,    30,     0,    31,    32,    33,
   121,     1,    49,     0,    50,     2,     3,     4,     5,     0,
     6,     7,     8,     9,    10,     0,    11,     0,    12,    13,
    14,    15,     0,    16,     0,     0,     1,     0,     0,     0,
     2,     3,     4,     5,     0,     6,     7,     8,     9,    10,
     0,    11,    17,    12,    13,    14,    15,     0,    16,    24,
     0,     0,    25,     0,     0,     0,    26,     0,    27,     0,
     0,     0,     0,    28,     0,     0,     0,    17,     0,     0,
    29,    30,     0,    31,    32,    33,     0,    69,    34,    70,
    71,    72,     0,    73,     0,    74,    75,     0,    76,     0,
    77,    78,    79,    80,     0,     0,     0,     0,     0,    81,
    82,    83,    84,    69,     0,    70,    71,    72,     0,    73,
     0,    74,    75,     0,    76,     0,     0,    78,    79,     0,
     0,     0,     0,     0,     0,    81,    82,    83,    84,    70,
    71,    72,     0,    73,     0,    74,    75,     0,    76,     0,
     0,    78,    79,     0,     0,     0,     0,     0,     0,    81,
    82,    83,    84,-32768,-32768,-32768,     0,-32768,     0,-32768,
    75,     0,-32768,    75,     0,    78,    79,     0,    78,    79,
     0,     0,     0,    81,    82,    83,    84,     0,    83,    84
};

static const short yycheck[] = {     2,
    23,    23,    23,     6,    23,     8,     9,    11,    11,    50,
    13,    52,    43,    15,    43,    43,    43,     4,    44,    52,
    50,     4,    23,    26,    27,    28,    50,    50,    31,    32,
    50,    50,    34,    43,    23,    22,    52,     4,    25,     0,
    18,    17,    29,    -1,    31,    -1,    -1,    49,    50,    36,
    -1,    54,    55,    56,    -1,    -1,    43,    44,    -1,    46,
    47,    48,    -1,    -1,    51,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    86,    86,     3,    -1,    -1,    89,     5,    -1,
     7,    -1,     9,    -1,    -1,    12,    13,    14,    15,    16,
    17,    -1,    -1,    22,    21,    -1,    25,    -1,    -1,    -1,
    29,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,
    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    47,    48,
     0,     1,    51,    -1,    53,     5,     6,     7,     8,    -1,
    10,    11,    12,    13,    14,    -1,    16,    -1,    18,    19,
    20,    21,    -1,    23,    -1,    -1,     1,    -1,    -1,    -1,
     5,     6,     7,     8,    -1,    10,    11,    12,    13,    14,
    -1,    16,    42,    18,    19,    20,    21,    -1,    23,    22,
    -1,    -1,    25,    -1,    -1,    -1,    29,    -1,    31,    -1,
    -1,    -1,    -1,    36,    -1,    -1,    -1,    42,    -1,    -1,
    43,    44,    -1,    46,    47,    48,    -1,    24,    51,    26,
    27,    28,    -1,    30,    -1,    32,    33,    -1,    35,    -1,
    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,
    47,    48,    49,    24,    -1,    26,    27,    28,    -1,    30,
    -1,    32,    33,    -1,    35,    -1,    -1,    38,    39,    -1,
    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    26,
    27,    28,    -1,    30,    -1,    32,    33,    -1,    35,    -1,
    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
    47,    48,    49,    26,    27,    28,    -1,    30,    -1,    32,
    33,    -1,    35,    33,    -1,    38,    39,    -1,    38,    39,
    -1,    -1,    -1,    46,    47,    48,    49,    -1,    48,    49
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 161 "frasm.y"
{
				clrexpr();
			;
    break;}
case 5:
#line 166 "frasm.y"
{
				clrexpr();
				yyerrok;
			;
    break;}
case 6:
#line 173 "frasm.y"
{
				endsymbol = yyvsp[-1].symb;
				nextreadact = Nra_end;
			;
    break;}
case 7:
#line 178 "frasm.y"
{
				nextreadact = Nra_end;
			;
    break;}
case 8:
#line 182 "frasm.y"
{
		if(nextfstk >= FILESTKDPTH)
		{
			fraerror("include file nesting limit exceeded");
		}
		else
		{
			infilestk[nextfstk].fnm = savestring(yyvsp[0].strng,strlen(yyvsp[0].strng));
			if( (infilestk[nextfstk].fpt = fopen(yyvsp[0].strng,"r"))
				==(FILE *)NULL )
			{
				fraerror("cannot open include file");
			}
			else
			{
				nextreadact = Nra_new;
			}
		}
			;
    break;}
case 9:
#line 202 "frasm.y"
{
				if(yyvsp[-2].symb -> seg == SSG_UNDEF)
				{
					pevalexpr(0, yyvsp[0].intv);
					if(evalr[0].seg == SSG_ABS)
					{
						yyvsp[-2].symb -> seg = SSG_EQU;
						yyvsp[-2].symb -> value = evalr[0].value;
						prtequvalue("C: 0x%lx\n",
							evalr[0].value);
					}
					else
					{
						fraerror(
					"noncomputable expression for EQU");
					}
				}
				else
				{
					fraerror(
				"cannot change symbol value with EQU");
				}
			;
    break;}
case 10:
#line 226 "frasm.y"
{
				if(yyvsp[-2].symb -> seg == SSG_UNDEF
				   || yyvsp[-2].symb -> seg == SSG_SET)
				{
					pevalexpr(0, yyvsp[0].intv);
					if(evalr[0].seg == SSG_ABS)
					{
						yyvsp[-2].symb -> seg = SSG_SET;
						yyvsp[-2].symb -> value = evalr[0].value;
						prtequvalue("C: 0x%lx\n",
							evalr[0].value);
					}
					else
					{
						fraerror(
					"noncomputable expression for SET");
					}
				}
				else
				{
					fraerror(
				"cannot change symbol value with SET");
				}
			;
    break;}
case 11:
#line 251 "frasm.y"
{
		if((++ifstkpt) < IFSTKDEPTH)
		{
			pevalexpr(0, yyvsp[0].intv);
			if(evalr[0].seg == SSG_ABS)
			{
				if(evalr[0].value != 0)
				{
					elseifstk[ifstkpt] = If_Skip;
					endifstk[ifstkpt] = If_Active;
				}
				else
				{
					fraifskip = TRUE;
					elseifstk[ifstkpt] = If_Active;
					endifstk[ifstkpt] = If_Active;
				}
			}
			else
			{
				fraifskip = TRUE;
				elseifstk[ifstkpt] = If_Active;
				endifstk[ifstkpt] = If_Active;
			}
		}
		else
		{
			fraerror("IF stack overflow");
		}
			;
    break;}
case 12:
#line 283 "frasm.y"
{
		if(fraifskip) 
		{
			if((++ifstkpt) < IFSTKDEPTH)
			{
					elseifstk[ifstkpt] = If_Skip;
					endifstk[ifstkpt] = If_Skip;
			}
			else
			{
				fraerror("IF stack overflow");
			}
		}
		else
		{
			yyerror("syntax error");
			YYERROR;
		}
				;
    break;}
case 13:
#line 304 "frasm.y"
{
				switch(elseifstk[ifstkpt])
				{
				case If_Active:
					fraifskip = FALSE;
					break;
				
				case If_Skip:
					fraifskip = TRUE;
					break;
				
				case If_Err:
					fraerror("ELSE with no matching if");
					break;
				}
			;
    break;}
case 14:
#line 322 "frasm.y"
{
				switch(endifstk[ifstkpt])
				{
				case If_Active:
					fraifskip = FALSE;
					ifstkpt--;
					break;
				
				case If_Skip:
					fraifskip = TRUE;
					ifstkpt--;
					break;
				
				case If_Err:
					fraerror("ENDI with no matching if");
					break;
				}
			;
    break;}
case 15:
#line 341 "frasm.y"
{
				pevalexpr(0, yyvsp[0].intv);
				if(evalr[0].seg == SSG_ABS)
				{
					locctr = labelloc = evalr[0].value;
					if(yyvsp[-2].symb -> seg == SSG_UNDEF)
					{
						yyvsp[-2].symb -> seg = SSG_ABS;
						yyvsp[-2].symb -> value = labelloc;
					}
					else
						fraerror(
						"multiple definition of label");
					prtequvalue("C: 0x%lx\n",
						evalr[0].value);
				}
				else
				{
					fraerror(
					 "noncomputable expression for ORG");
				}
			;
    break;}
case 16:
#line 364 "frasm.y"
{
				pevalexpr(0, yyvsp[0].intv);
				if(evalr[0].seg == SSG_ABS)
				{
					locctr = labelloc = evalr[0].value;
					prtequvalue("C: 0x%lx\n",
						evalr[0].value);
				}
				else
				{
					fraerror(
					 "noncomputable expression for ORG");
				}
			;
    break;}
case 17:
#line 379 "frasm.y"
{
				if(yyvsp[-1].symb -> seg == SSG_UNDEF)
				{
					yyvsp[-1].symb -> seg = SSG_EQU;
					if( (yyvsp[-1].symb->value = chtcreate()) <= 0)
					{
		fraerror( "cannot create character translation table");
					}
					prtequvalue("C: 0x%lx\n", yyvsp[-1].symb -> value);
				}
				else
				{
			fraerror( "multiple definition of label");
				}
			;
    break;}
case 18:
#line 395 "frasm.y"
{
				chtcpoint = (int *) NULL;
				prtequvalue("C: 0x%lx\n", 0L);
			;
    break;}
case 19:
#line 400 "frasm.y"
{
				pevalexpr(0, yyvsp[0].intv);
				if( evalr[0].seg == SSG_ABS)
				{
					if( evalr[0].value == 0)
					{
						chtcpoint = (int *)NULL;
						prtequvalue("C: 0x%lx\n", 0L);
					}
					else if(evalr[0].value < chtnxalph)
					{
				chtcpoint = chtatab[evalr[0].value];
				prtequvalue("C: 0x%lx\n", evalr[0].value);
					}
					else
					{
			fraerror("nonexistent character translation table");
					}
				}
				else
				{
					fraerror("noncomputable expression");
				}
			;
    break;}
case 20:
#line 425 "frasm.y"
{
		int findrv, numret, *charaddr;
		char *sourcestr = yyvsp[-2].strng, *before;

		if(chtnpoint != (int *)NULL)
		{
			for(satsub = 0; satsub < yyvsp[0].intv; satsub++)
			{
				before = sourcestr;

				pevalexpr(0, exprlist[satsub]);
				findrv = chtcfind(chtnpoint, &sourcestr,
						&charaddr, &numret);
				if(findrv == CF_END)
				{
			fraerror("more expressions than characters");
					break;
				}

				if(evalr[0].seg == SSG_ABS)
				{
					switch(findrv)
					{
					case CF_UNDEF:
						{
				if(evalr[0].value < 0 ||
					evalr[0].value > 255)
				{
			frawarn("character translation value truncated");
				}
				*charaddr = evalr[0].value & 0xff;
				prtequvalue("C: 0x%lx\n", evalr[0].value);
						}
						break;

					case CF_INVALID:
					case CF_NUMBER:
				fracherror("invalid character to define", 
					before, sourcestr);
						break;

					case CF_CHAR:
				fracherror("character already defined", 
					before, sourcestr);
						break;
					}
				}
				else
				{
					fraerror("noncomputable expression");
				}
			}

			if( *sourcestr != '\0')
			{
				fraerror("more characters than expressions");
			}
		}
		else
		{
			fraerror("no CHARSET statement active");
		}
			
			;
    break;}
case 21:
#line 490 "frasm.y"
{
			if(yyvsp[0].symb -> seg == SSG_UNDEF)
			{
				yyvsp[0].symb -> seg = SSG_ABS;
				yyvsp[0].symb -> value = labelloc;
				prtequvalue("C: 0x%lx\n", labelloc);

			}
			else
				fraerror(
				"multiple definition of label");
			;
    break;}
case 23:
#line 506 "frasm.y"
{
			if(yyvsp[-1].symb -> seg == SSG_UNDEF)
			{
				yyvsp[-1].symb -> seg = SSG_ABS;
				yyvsp[-1].symb -> value = labelloc;
			}
			else
				fraerror(
				"multiple definition of label");
			labelloc = locctr;
			;
    break;}
case 24:
#line 519 "frasm.y"
{
				labelloc = locctr;
			;
    break;}
case 25:
#line 525 "frasm.y"
{
				genlocrec(currseg, labelloc);
				for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
				{
					pevalexpr(1, exprlist[satsub]);
					locctr += geninstr(genbdef);
				}
			;
    break;}
case 26:
#line 534 "frasm.y"
{
				genlocrec(currseg, labelloc);
				for(satsub = 0; satsub < yyvsp[0].intv; satsub++)
				{
					locctr += genstring(stringlist[satsub]);
				}
			;
    break;}
case 27:
#line 542 "frasm.y"
{
				genlocrec(currseg, labelloc);
				for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
				{
					pevalexpr(1, exprlist[satsub]);
					locctr += geninstr(genwdef);
				}
			;
    break;}
case 28:
#line 551 "frasm.y"
{
				pevalexpr(0, yyvsp[0].intv);
				if(evalr[0].seg == SSG_ABS)
				{
					locctr = labelloc + evalr[0].value;
					prtequvalue("C: 0x%lx\n", labelloc);
				}
				else
				{
					fraerror(
				 "noncomputable result for RMB expression");
				}
			;
    break;}
case 29:
#line 567 "frasm.y"
{
				exprlist[nextexprs ++ ] = yyvsp[0].intv;
				yyval.intv = nextexprs;
			;
    break;}
case 30:
#line 572 "frasm.y"
{
				nextexprs = 0;
				exprlist[nextexprs ++ ] = yyvsp[0].intv;
				yyval.intv = nextexprs;
			;
    break;}
case 31:
#line 580 "frasm.y"
{
				stringlist[nextstrs ++ ] = yyvsp[0].strng;
				yyval.intv = nextstrs;
			;
    break;}
case 32:
#line 585 "frasm.y"
{
				nextstrs = 0;
				stringlist[nextstrs ++ ] = yyvsp[0].strng;
				yyval.intv = nextstrs;
			;
    break;}
case 33:
#line 594 "frasm.y"
{
		if( ! cpumatch(yyvsp[0].strng))
		{
			fraerror("unknown cpu type, R65C02 assumed");
			cpuselect = CPUR65C;
		}
			;
    break;}
case 34:
#line 603 "frasm.y"
{
		genlocrec(currseg, labelloc);
		locctr += geninstr(findgen(yyvsp[0].intv, ST_INH, cpuselect));
			;
    break;}
case 35:
#line 609 "frasm.y"
{
		genlocrec(currseg, labelloc);
		locctr += geninstr(findgen(yyvsp[-1].intv, ST_ACCUM, cpuselect));
			;
    break;}
case 36:
#line 615 "frasm.y"
{
		pevalexpr(1, yyvsp[0].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen( yyvsp[-1].intv, ST_EXPR, 
				  ( (evalr[1].seg == SSG_ABS 
				&& evalr[1].value >= 0
				&& evalr[1].value <= 255 )
				? DIRECT : EXTENDED ) | cpuselect )
				);
			;
    break;}
case 37:
#line 627 "frasm.y"
{
		pevalexpr(1, yyvsp[-2].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen( yyvsp[-3].intv, ST_INDEX, 
				  ( (evalr[1].seg == SSG_ABS 
				&& evalr[1].value >= 0
				&& evalr[1].value <= 255 )
				? DIRECT : EXTENDED ) | cpuselect | yyvsp[0].intv )
				);
			;
    break;}
case 38:
#line 639 "frasm.y"
{
		pevalexpr(1, yyvsp[-3].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen( yyvsp[-5].intv, ST_PREINDEX, 
				  ( (evalr[1].seg == SSG_ABS 
				&& evalr[1].value >= 0
				&& evalr[1].value <= 255 )
				? DIRECT : EXTENDED ) | cpuselect | yyvsp[-1].intv )
				);
			;
    break;}
case 39:
#line 651 "frasm.y"
{
		pevalexpr(1, yyvsp[-1].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen( yyvsp[-3].intv, ST_INDIR, 
				  ( (evalr[1].seg == SSG_ABS 
				&& evalr[1].value >= 0
				&& evalr[1].value <= 255 )
				? DIRECT : EXTENDED ) | cpuselect )
				);
			;
    break;}
case 40:
#line 663 "frasm.y"
{
		pevalexpr(1, yyvsp[-3].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen( yyvsp[-5].intv, ST_POSTINDEX, 
				  ( (evalr[1].seg == SSG_ABS 
				&& evalr[1].value >= 0
				&& evalr[1].value <= 255 )
				? DIRECT : EXTENDED ) | cpuselect | yyvsp[0].intv )
				);
			;
    break;}
case 41:
#line 675 "frasm.y"
{
		pevalexpr(1, yyvsp[0].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen(yyvsp[-2].intv, ST_IMMED, cpuselect));
			;
    break;}
case 42:
#line 682 "frasm.y"
{
		pevalexpr(1, yyvsp[-2].intv);
		pevalexpr(2, yyvsp[0].intv);
		genlocrec(currseg, labelloc);
		locctr += geninstr( findgen(yyvsp[-3].intv, ST_DIRREL, cpuselect));
			;
    break;}
case 43:
#line 690 "frasm.y"
{
				yyval.intv = yyvsp[0].intv;
			;
    break;}
case 44:
#line 694 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_NEG,0,0L,
					SYMNULL);
			;
    break;}
case 45:
#line 699 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_NOT,0,0L,
					SYMNULL);
			;
    break;}
case 46:
#line 704 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_HIGH,0,0L,
					SYMNULL);
			;
    break;}
case 47:
#line 709 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_LOW,0,0L,
					SYMNULL);
			;
    break;}
case 48:
#line 714 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_MUL,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 49:
#line 719 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_DIV,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 50:
#line 724 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_ADD,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 51:
#line 729 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SUB,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 52:
#line 734 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_MOD,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 53:
#line 739 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SHL,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 54:
#line 744 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SHR,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 55:
#line 749 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_GT,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 56:
#line 754 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_GE,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 57:
#line 759 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_LT,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 58:
#line 764 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_LE,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 59:
#line 769 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_NE,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 60:
#line 774 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_EQ,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 61:
#line 779 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_AND,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 62:
#line 784 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_OR,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 63:
#line 789 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_XOR,yyvsp[0].intv,0L,
					SYMNULL);
			;
    break;}
case 64:
#line 794 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_DEF,0,IGP_DEFINED,0,0L,yyvsp[0].symb);
			;
    break;}
case 65:
#line 798 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_SYMB,0,IFC_SYMB,0,0L,yyvsp[0].symb);
			;
    break;}
case 66:
#line 802 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_PROGC,0,IFC_PROGCTR,0,
					labelloc, SYMNULL);
			;
    break;}
case 67:
#line 807 "frasm.y"
{
				yyval.intv = exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,yyvsp[0].longv,
					SYMNULL);
			;
    break;}
case 68:
#line 812 "frasm.y"
{
				char *sourcestr = yyvsp[0].strng;
				long accval = 0;

				if(strlen(yyvsp[0].strng) > 0)
				{
					accval = chtran(&sourcestr);
					if(*sourcestr != '\0')
					{
						accval = (accval << 8) +
							chtran(&sourcestr);
					}

					if( *sourcestr != '\0')
					{
	frawarn("string constant in expression more than 2 characters long");
					}
				}
				yyval.intv = exprnode(PCCASE_CONS, 0, IGP_CONSTANT, 0,
					accval, SYMNULL);
			;
    break;}
case 69:
#line 836 "frasm.y"
{
				yyval.intv = yyvsp[-1].intv;
			;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 844 "frasm.y"


lexintercept()
/*
	description	intercept the call to yylex (the lexical analyzer)
			and filter out all unnecessary tokens when skipping
			the input between a failed IF and its matching ENDI or
			ELSE
	globals 	fraifskip	the enable flag
*/
{
#undef yylex

	int rv;

	if(fraifskip)
	{
		for(;;)
		{

			switch(rv = yylex())

			{
			case 0:
			case KOC_END:
			case KOC_IF:
			case KOC_ELSE:
			case KOC_ENDI:
			case EOL:
				return rv;
			default:
				break;
			}
		}
	}
	else
		return yylex();
#define yylex lexintercept
}



setreserved()
{

	reservedsym("and", KEOP_AND, 0);
	reservedsym("defined", KEOP_DEFINED,0);
	reservedsym("eq", KEOP_EQ, 0);
	reservedsym("ge", KEOP_GE, 0);
	reservedsym("gt", KEOP_GT, 0);
	reservedsym("high", KEOP_HIGH, 0);
	reservedsym("le", KEOP_LE, 0);
	reservedsym("low", KEOP_LOW, 0);
	reservedsym("lt", KEOP_LT, 0);
	reservedsym("mod", KEOP_MOD, 0);
	reservedsym("ne", KEOP_NE, 0);
	reservedsym("not", KEOP_NOT, 0);
	reservedsym("or", KEOP_OR, 0);
	reservedsym("shl", KEOP_SHL, 0);
	reservedsym("shr", KEOP_SHR, 0);
	reservedsym("xor", KEOP_XOR, 0);
	reservedsym("AND", KEOP_AND, 0);
	reservedsym("DEFINED", KEOP_DEFINED,0);
	reservedsym("EQ", KEOP_EQ, 0);
	reservedsym("GE", KEOP_GE, 0);
	reservedsym("GT", KEOP_GT, 0);
	reservedsym("HIGH", KEOP_HIGH, 0);
	reservedsym("LE", KEOP_LE, 0);
	reservedsym("LOW", KEOP_LOW, 0);
	reservedsym("LT", KEOP_LT, 0);
	reservedsym("MOD", KEOP_MOD, 0);
	reservedsym("NE", KEOP_NE, 0);
	reservedsym("NOT", KEOP_NOT, 0);
	reservedsym("OR", KEOP_OR, 0);
	reservedsym("SHL", KEOP_SHL, 0);
	reservedsym("SHR", KEOP_SHR, 0);
	reservedsym("XOR", KEOP_XOR, 0);

	/* machine specific token definitions */
	reservedsym("a", ACCUM, 0);
	reservedsym("x", INDEX, INDEXX);
	reservedsym("y", INDEX, INDEXY);
	reservedsym("A", ACCUM, 0);
	reservedsym("X", INDEX, INDEXX);
	reservedsym("Y", INDEX, INDEXY);

}


cpumatch(str)
	char * str;
{
	int msub;

	static struct
	{
		char * mtch;
		int   cpuv;
	} matchtab[] =
	{
		{"R65C", CPUR65C },
		{"r65c", CPUR65C },
		{"65C", CPU65C },
		{"65c", CPU65C },
		{"R65", CPUR65 },
		{"r65", CPUR65 },
		{"21", CPUR21 },
		{"65", CPU65 },
		{"", 0} 
	};

	for(msub = 0; matchtab[msub].mtch[0] != '\0'; msub++)
	{
		if(strcontains(str, matchtab[msub].mtch))
		{
			cpuselect = matchtab[msub].cpuv;
			return TRUE;
		}
	}

	return FALSE;
}


strcontains(s1, sm)
	char * s1, *sm;
{
	int l1 = strlen(s1), lm = strlen(sm);

	for(; l1 >= lm; l1--, s1++)
	{
		if(strncmp(s1, sm, lm) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*
	description	Opcode and Instruction generation tables
	usage		Unix, framework crossassembler
	history		September 25, 1987
*/

#define NUMOPCODE 124
#define NUMSYNBLK 166
#define NUMDIFFOP 221

int gnumopcode = NUMOPCODE;

int ophashlnk[NUMOPCODE];

struct opsym optab[NUMOPCODE+1]
	= {
	{"invalid", KOC_opcode, 2, 0 },
	{"ADC", KOC_opcode, 6, 2 },
	{"AND", KOC_opcode, 6, 8 },
	{"ASL", KOC_opcode, 3, 14 },
	{"BBR0", KOC_opcode, 1, 17 },
	{"BBR1", KOC_opcode, 1, 18 },
	{"BBR2", KOC_opcode, 1, 19 },
	{"BBR3", KOC_opcode, 1, 20 },
	{"BBR4", KOC_opcode, 1, 21 },
	{"BBR5", KOC_opcode, 1, 22 },
	{"BBR6", KOC_opcode, 1, 23 },
	{"BBR7", KOC_opcode, 1, 24 },
	{"BBS0", KOC_opcode, 1, 25 },
	{"BBS1", KOC_opcode, 1, 26 },
	{"BBS2", KOC_opcode, 1, 27 },
	{"BBS3", KOC_opcode, 1, 28 },
	{"BBS4", KOC_opcode, 1, 29 },
	{"BBS5", KOC_opcode, 1, 30 },
	{"BBS6", KOC_opcode, 1, 31 },
	{"BBS7", KOC_opcode, 1, 32 },
	{"BCC", KOC_opcode, 1, 33 },
	{"BCS", KOC_opcode, 1, 34 },
	{"BEQ", KOC_opcode, 1, 35 },
	{"BGE", KOC_opcode, 1, 36 },
	{"BIT", KOC_opcode, 3, 37 },
	{"BLT", KOC_opcode, 1, 40 },
	{"BMI", KOC_opcode, 1, 41 },
	{"BNE", KOC_opcode, 1, 42 },
	{"BPL", KOC_opcode, 1, 43 },
	{"BRA", KOC_opcode, 1, 44 },
	{"BRK", KOC_opcode, 2, 45 },
	{"BVC", KOC_opcode, 1, 47 },
	{"BVS", KOC_opcode, 1, 48 },
	{"BYTE", KOC_BDEF, 0, 0 },
	{"CHARDEF", KOC_CHDEF, 0, 0 },
	{"CHARSET", KOC_CHSET, 0, 0 },
	{"CHARUSE", KOC_CHUSE, 0, 0 },
	{"CHD", KOC_CHDEF, 0, 0 },
	{"CLC", KOC_opcode, 1, 49 },
	{"CLD", KOC_opcode, 1, 50 },
	{"CLI", KOC_opcode, 1, 51 },
	{"CLV", KOC_opcode, 1, 52 },
	{"CMP", KOC_opcode, 6, 53 },
	{"CPU", KOC_CPU, 0, 0 },
	{"CPX", KOC_opcode, 2, 59 },
	{"CPY", KOC_opcode, 2, 61 },
	{"DB", KOC_BDEF, 0, 0 },
	{"DEC", KOC_opcode, 3, 63 },
	{"DEX", KOC_opcode, 1, 66 },
	{"DEY", KOC_opcode, 1, 67 },
	{"DW", KOC_WDEF, 0, 0 },
	{"ELSE", KOC_ELSE, 0, 0 },
	{"END", KOC_END, 0, 0 },
	{"ENDI", KOC_ENDI, 0, 0 },
	{"EOR", KOC_opcode, 6, 68 },
	{"EQU", KOC_EQU, 0, 0 },
	{"FCB", KOC_BDEF, 0, 0 },
	{"FCC", KOC_SDEF, 0, 0 },
	{"FDB", KOC_WDEF, 0, 0 },
	{"IF", KOC_IF, 0, 0 },
	{"INC", KOC_opcode, 3, 74 },
	{"INCL", KOC_INCLUDE, 0, 0 },
	{"INCLUDE", KOC_INCLUDE, 0, 0 },
	{"INX", KOC_opcode, 1, 77 },
	{"INY", KOC_opcode, 1, 78 },
	{"JMP", KOC_opcode, 3, 79 },
	{"JSR", KOC_opcode, 1, 82 },
	{"LDA", KOC_opcode, 6, 83 },
	{"LDX", KOC_opcode, 3, 89 },
	{"LDY", KOC_opcode, 3, 92 },
	{"LSR", KOC_opcode, 3, 95 },
	{"MUL", KOC_opcode, 1, 98 },
	{"NOP", KOC_opcode, 1, 99 },
	{"ORA", KOC_opcode, 6, 100 },
	{"ORG", KOC_ORG, 0, 0 },
	{"PHA", KOC_opcode, 1, 106 },
	{"PHP", KOC_opcode, 1, 107 },
	{"PHX", KOC_opcode, 1, 108 },
	{"PHY", KOC_opcode, 1, 109 },
	{"PLA", KOC_opcode, 1, 110 },
	{"PLP", KOC_opcode, 1, 111 },
	{"PLX", KOC_opcode, 1, 112 },
	{"PLY", KOC_opcode, 1, 113 },
	{"RESERVE", KOC_RESM, 0, 0 },
	{"RMB0", KOC_opcode, 1, 114 },
	{"RMB1", KOC_opcode, 1, 115 },
	{"RMB2", KOC_opcode, 1, 116 },
	{"RMB3", KOC_opcode, 1, 117 },
	{"RMB4", KOC_opcode, 1, 118 },
	{"RMB5", KOC_opcode, 1, 119 },
	{"RMB6", KOC_opcode, 1, 120 },
	{"RMB7", KOC_opcode, 1, 121 },
	{"RMB", KOC_RESM, 0, 0 },
	{"ROL", KOC_opcode, 3, 122 },
	{"ROR", KOC_opcode, 3, 125 },
	{"RTI", KOC_opcode, 1, 128 },
	{"RTS", KOC_opcode, 1, 129 },
	{"SBC", KOC_opcode, 6, 130 },
	{"SEC", KOC_opcode, 1, 136 },
	{"SED", KOC_opcode, 1, 137 },
	{"SEI", KOC_opcode, 1, 138 },
	{"SET", KOC_SET, 0, 0 },
	{"SMB0", KOC_opcode, 1, 139 },
	{"SMB1", KOC_opcode, 1, 140 },
	{"SMB2", KOC_opcode, 1, 141 },
	{"SMB3", KOC_opcode, 1, 142 },
	{"SMB4", KOC_opcode, 1, 143 },
	{"SMB5", KOC_opcode, 1, 144 },
	{"SMB6", KOC_opcode, 1, 145 },
	{"SMB7", KOC_opcode, 1, 146 },
	{"STA", KOC_opcode, 5, 147 },
	{"STRING", KOC_SDEF, 0, 0 },
	{"STX", KOC_opcode, 2, 152 },
	{"STY", KOC_opcode, 2, 154 },
	{"STZ", KOC_opcode, 2, 156 },
	{"TAX", KOC_opcode, 1, 158 },
	{"TAY", KOC_opcode, 1, 159 },
	{"TRB", KOC_opcode, 1, 160 },
	{"TSB", KOC_opcode, 1, 161 },
	{"TSX", KOC_opcode, 1, 162 },
	{"TXA", KOC_opcode, 1, 163 },
	{"TXS", KOC_opcode, 1, 164 },
	{"TYA", KOC_opcode, 1, 165 },
	{"WORD", KOC_WDEF, 0, 0 },
	{ "", 0, 0, 0 }};

struct opsynt ostab[NUMSYNBLK+1]
	= {
/* invalid 0 */ { 0, 1, 0 },
/* invalid 1 */ { 0xffff, 1, 1 },
/* ADC 2 */ { ST_EXPR, 2, 2 },
/* ADC 3 */ { ST_IMMED, 1, 4 },
/* ADC 4 */ { ST_INDEX, 3, 5 },
/* ADC 5 */ { ST_INDIR, 1, 8 },
/* ADC 6 */ { ST_POSTINDEX, 1, 9 },
/* ADC 7 */ { ST_PREINDEX, 1, 10 },
/* AND 8 */ { ST_EXPR, 2, 11 },
/* AND 9 */ { ST_IMMED, 1, 13 },
/* AND 10 */ { ST_INDEX, 3, 14 },
/* AND 11 */ { ST_INDIR, 1, 17 },
/* AND 12 */ { ST_POSTINDEX, 1, 18 },
/* AND 13 */ { ST_PREINDEX, 1, 19 },
/* ASL 14 */ { ST_ACCUM, 1, 20 },
/* ASL 15 */ { ST_EXPR, 2, 21 },
/* ASL 16 */ { ST_INDEX, 2, 23 },
/* BBR0 17 */ { ST_DIRREL, 1, 25 },
/* BBR1 18 */ { ST_DIRREL, 1, 26 },
/* BBR2 19 */ { ST_DIRREL, 1, 27 },
/* BBR3 20 */ { ST_DIRREL, 1, 28 },
/* BBR4 21 */ { ST_DIRREL, 1, 29 },
/* BBR5 22 */ { ST_DIRREL, 1, 30 },
/* BBR6 23 */ { ST_DIRREL, 1, 31 },
/* BBR7 24 */ { ST_DIRREL, 1, 32 },
/* BBS0 25 */ { ST_DIRREL, 1, 33 },
/* BBS1 26 */ { ST_DIRREL, 1, 34 },
/* BBS2 27 */ { ST_DIRREL, 1, 35 },
/* BBS3 28 */ { ST_DIRREL, 1, 36 },
/* BBS4 29 */ { ST_DIRREL, 1, 37 },
/* BBS5 30 */ { ST_DIRREL, 1, 38 },
/* BBS6 31 */ { ST_DIRREL, 1, 39 },
/* BBS7 32 */ { ST_DIRREL, 1, 40 },
/* BCC 33 */ { ST_EXPR, 1, 41 },
/* BCS 34 */ { ST_EXPR, 1, 42 },
/* BEQ 35 */ { ST_EXPR, 1, 43 },
/* BGE 36 */ { ST_EXPR, 1, 44 },
/* BIT 37 */ { ST_EXPR, 2, 45 },
/* BIT 38 */ { ST_IMMED, 1, 47 },
/* BIT 39 */ { ST_INDEX, 2, 48 },
/* BLT 40 */ { ST_EXPR, 1, 50 },
/* BMI 41 */ { ST_EXPR, 1, 51 },
/* BNE 42 */ { ST_EXPR, 1, 52 },
/* BPL 43 */ { ST_EXPR, 1, 53 },
/* BRA 44 */ { ST_EXPR, 2, 54 },
/* BRK 45 */ { ST_IMMED, 1, 56 },
/* BRK 46 */ { ST_INH, 1, 57 },
/* BVC 47 */ { ST_EXPR, 1, 58 },
/* BVS 48 */ { ST_EXPR, 1, 59 },
/* CLC 49 */ { ST_INH, 1, 60 },
/* CLD 50 */ { ST_INH, 1, 61 },
/* CLI 51 */ { ST_INH, 1, 62 },
/* CLV 52 */ { ST_INH, 1, 63 },
/* CMP 53 */ { ST_EXPR, 2, 64 },
/* CMP 54 */ { ST_IMMED, 1, 66 },
/* CMP 55 */ { ST_INDEX, 3, 67 },
/* CMP 56 */ { ST_INDIR, 1, 70 },
/* CMP 57 */ { ST_POSTINDEX, 1, 71 },
/* CMP 58 */ { ST_PREINDEX, 1, 72 },
/* CPX 59 */ { ST_EXPR, 2, 73 },
/* CPX 60 */ { ST_IMMED, 1, 75 },
/* CPY 61 */ { ST_EXPR, 2, 76 },
/* CPY 62 */ { ST_IMMED, 1, 78 },
/* DEC 63 */ { ST_ACCUM, 1, 79 },
/* DEC 64 */ { ST_EXPR, 2, 80 },
/* DEC 65 */ { ST_INDEX, 2, 82 },
/* DEX 66 */ { ST_INH, 1, 84 },
/* DEY 67 */ { ST_INH, 1, 85 },
/* EOR 68 */ { ST_EXPR, 2, 86 },
/* EOR 69 */ { ST_IMMED, 1, 88 },
/* EOR 70 */ { ST_INDEX, 3, 89 },
/* EOR 71 */ { ST_INDIR, 1, 92 },
/* EOR 72 */ { ST_POSTINDEX, 1, 93 },
/* EOR 73 */ { ST_PREINDEX, 1, 94 },
/* INC 74 */ { ST_ACCUM, 1, 95 },
/* INC 75 */ { ST_EXPR, 2, 96 },
/* INC 76 */ { ST_INDEX, 2, 98 },
/* INX 77 */ { ST_INH, 1, 100 },
/* INY 78 */ { ST_INH, 1, 101 },
/* JMP 79 */ { ST_EXPR, 1, 102 },
/* JMP 80 */ { ST_INDIR, 1, 103 },
/* JMP 81 */ { ST_PREINDEX, 1, 104 },
/* JSR 82 */ { ST_EXPR, 1, 105 },
/* LDA 83 */ { ST_EXPR, 2, 106 },
/* LDA 84 */ { ST_IMMED, 1, 108 },
/* LDA 85 */ { ST_INDEX, 3, 109 },
/* LDA 86 */ { ST_INDIR, 1, 112 },
/* LDA 87 */ { ST_POSTINDEX, 1, 113 },
/* LDA 88 */ { ST_PREINDEX, 1, 114 },
/* LDX 89 */ { ST_EXPR, 2, 115 },
/* LDX 90 */ { ST_IMMED, 1, 117 },
/* LDX 91 */ { ST_INDEX, 2, 118 },
/* LDY 92 */ { ST_EXPR, 2, 120 },
/* LDY 93 */ { ST_IMMED, 1, 122 },
/* LDY 94 */ { ST_INDEX, 2, 123 },
/* LSR 95 */ { ST_ACCUM, 1, 125 },
/* LSR 96 */ { ST_EXPR, 2, 126 },
/* LSR 97 */ { ST_INDEX, 2, 128 },
/* MUL 98 */ { ST_INH, 1, 130 },
/* NOP 99 */ { ST_INH, 1, 131 },
/* ORA 100 */ { ST_EXPR, 2, 132 },
/* ORA 101 */ { ST_IMMED, 1, 134 },
/* ORA 102 */ { ST_INDEX, 3, 135 },
/* ORA 103 */ { ST_INDIR, 1, 138 },
/* ORA 104 */ { ST_POSTINDEX, 1, 139 },
/* ORA 105 */ { ST_PREINDEX, 1, 140 },
/* PHA 106 */ { ST_INH, 1, 141 },
/* PHP 107 */ { ST_INH, 1, 142 },
/* PHX 108 */ { ST_INH, 2, 143 },
/* PHY 109 */ { ST_INH, 2, 145 },
/* PLA 110 */ { ST_INH, 1, 147 },
/* PLP 111 */ { ST_INH, 1, 148 },
/* PLX 112 */ { ST_INH, 2, 149 },
/* PLY 113 */ { ST_INH, 2, 151 },
/* RMB0 114 */ { ST_EXPR, 1, 153 },
/* RMB1 115 */ { ST_EXPR, 1, 154 },
/* RMB2 116 */ { ST_EXPR, 1, 155 },
/* RMB3 117 */ { ST_EXPR, 1, 156 },
/* RMB4 118 */ { ST_EXPR, 1, 157 },
/* RMB5 119 */ { ST_EXPR, 1, 158 },
/* RMB6 120 */ { ST_EXPR, 1, 159 },
/* RMB7 121 */ { ST_EXPR, 1, 160 },
/* ROL 122 */ { ST_ACCUM, 1, 161 },
/* ROL 123 */ { ST_EXPR, 2, 162 },
/* ROL 124 */ { ST_INDEX, 2, 164 },
/* ROR 125 */ { ST_ACCUM, 1, 166 },
/* ROR 126 */ { ST_EXPR, 2, 167 },
/* ROR 127 */ { ST_INDEX, 2, 169 },
/* RTI 128 */ { ST_INH, 1, 171 },
/* RTS 129 */ { ST_INH, 1, 172 },
/* SBC 130 */ { ST_EXPR, 2, 173 },
/* SBC 131 */ { ST_IMMED, 1, 175 },
/* SBC 132 */ { ST_INDEX, 3, 176 },
/* SBC 133 */ { ST_INDIR, 1, 179 },
/* SBC 134 */ { ST_POSTINDEX, 1, 180 },
/* SBC 135 */ { ST_PREINDEX, 1, 181 },
/* SEC 136 */ { ST_INH, 1, 182 },
/* SED 137 */ { ST_INH, 1, 183 },
/* SEI 138 */ { ST_INH, 1, 184 },
/* SMB0 139 */ { ST_EXPR, 1, 185 },
/* SMB1 140 */ { ST_EXPR, 1, 186 },
/* SMB2 141 */ { ST_EXPR, 1, 187 },
/* SMB3 142 */ { ST_EXPR, 1, 188 },
/* SMB4 143 */ { ST_EXPR, 1, 189 },
/* SMB5 144 */ { ST_EXPR, 1, 190 },
/* SMB6 145 */ { ST_EXPR, 1, 191 },
/* SMB7 146 */ { ST_EXPR, 1, 192 },
/* STA 147 */ { ST_EXPR, 2, 193 },
/* STA 148 */ { ST_INDEX, 3, 195 },
/* STA 149 */ { ST_INDIR, 1, 198 },
/* STA 150 */ { ST_POSTINDEX, 1, 199 },
/* STA 151 */ { ST_PREINDEX, 1, 200 },
/* STX 152 */ { ST_EXPR, 2, 201 },
/* STX 153 */ { ST_INDEX, 1, 203 },
/* STY 154 */ { ST_EXPR, 2, 204 },
/* STY 155 */ { ST_INDEX, 1, 206 },
/* STZ 156 */ { ST_EXPR, 2, 207 },
/* STZ 157 */ { ST_INDEX, 2, 209 },
/* TAX 158 */ { ST_INH, 1, 211 },
/* TAY 159 */ { ST_INH, 1, 212 },
/* TRB 160 */ { ST_EXPR, 2, 213 },
/* TSB 161 */ { ST_EXPR, 2, 215 },
/* TSX 162 */ { ST_INH, 1, 217 },
/* TXA 163 */ { ST_INH, 1, 218 },
/* TXS 164 */ { ST_INH, 1, 219 },
/* TYA 165 */ { ST_INH, 1, 220 },
	{ 0, 0, 0 } };

struct igel igtab[NUMDIFFOP+1]
	= {
/* invalid 0 */   { 0 , 0, 
		"[Xnullentry" },
/* invalid 1 */   { 0 , 0, 
		"[Xinvalid opcode" },
/* ADC 2 */   { ADDRESS , DIRECT, 
		"65;[1=];" },
/* ADC 3 */   { ADDRESS , EXTENDED, 
		"6d;[1=]y" },
/* ADC 4 */   { 0 , 0, 
		"69;[1=];" },
/* ADC 5 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"75;[1=];" },
/* ADC 6 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"7d;[1=]y" },
/* ADC 7 */   { INDEXMASK , INDEXY, 
		"79;[1=]y" },
/* ADC 8 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"72;[1=];" },
/* ADC 9 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"71;[1=];" },
/* ADC 10 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"61;[1=];" },
/* AND 11 */   { ADDRESS , DIRECT, 
		"25;[1=];" },
/* AND 12 */   { ADDRESS , EXTENDED, 
		"2d;[1=]y" },
/* AND 13 */   { 0 , 0, 
		"29;[1=];" },
/* AND 14 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"35;[1=];" },
/* AND 15 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"3d;[1=]y" },
/* AND 16 */   { INDEXMASK , INDEXY, 
		"39;[1=]y" },
/* AND 17 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"32;[1=];" },
/* AND 18 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"31;[1=];" },
/* AND 19 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"21;[1=];" },
/* ASL 20 */   { 0 , 0, 
		"0a;" },
/* ASL 21 */   { ADDRESS , DIRECT, 
		"06;[1=];" },
/* ASL 22 */   { ADDRESS , EXTENDED, 
		"0e;[1=]y" },
/* ASL 23 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"16;[1=];" },
/* ASL 24 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"1e;[1=]y" },
/* BBR0 25 */   { INSTROCKWELL , INSTROCKWELL, 
		"0f;[1=];[2=].Q.1+-r" },
/* BBR1 26 */   { INSTROCKWELL , INSTROCKWELL, 
		"1f;[1=];[2=].Q.1+-r" },
/* BBR2 27 */   { INSTROCKWELL , INSTROCKWELL, 
		"2f;[1=];[2=].Q.1+-r" },
/* BBR3 28 */   { INSTROCKWELL , INSTROCKWELL, 
		"3f;[1=];[2=].Q.1+-r" },
/* BBR4 29 */   { INSTROCKWELL , INSTROCKWELL, 
		"4f;[1=];[2=].Q.1+-r" },
/* BBR5 30 */   { INSTROCKWELL , INSTROCKWELL, 
		"5f;[1=];[2=].Q.1+-r" },
/* BBR6 31 */   { INSTROCKWELL , INSTROCKWELL, 
		"6f;[1=];[2=].Q.1+-r" },
/* BBR7 32 */   { INSTROCKWELL , INSTROCKWELL, 
		"7f;[1=];[2=].Q.1+-r" },
/* BBS0 33 */   { INSTROCKWELL , INSTROCKWELL, 
		"8f;[1=];[2=].Q.1+-r" },
/* BBS1 34 */   { INSTROCKWELL , INSTROCKWELL, 
		"9f;[1=];[2=].Q.1+-r" },
/* BBS2 35 */   { INSTROCKWELL , INSTROCKWELL, 
		"af;[1=];[2=].Q.1+-r" },
/* BBS3 36 */   { INSTROCKWELL , INSTROCKWELL, 
		"bf;[1=];[2=].Q.1+-r" },
/* BBS4 37 */   { INSTROCKWELL , INSTROCKWELL, 
		"cf;[1=];[2=].Q.1+-r" },
/* BBS5 38 */   { INSTROCKWELL , INSTROCKWELL, 
		"df;[1=];[2=].Q.1+-r" },
/* BBS6 39 */   { INSTROCKWELL , INSTROCKWELL, 
		"ef;[1=];[2=].Q.1+-r" },
/* BBS7 40 */   { INSTROCKWELL , INSTROCKWELL, 
		"ff;[1=];[2=].Q.1+-r" },
/* BCC 41 */   { 0 , 0, 
		"90;[1=].Q.1+-r" },
/* BCS 42 */   { 0 , 0, 
		"b0;[1=].Q.1+-r" },
/* BEQ 43 */   { 0 , 0, 
		"f0;[1=].Q.1+-r" },
/* BGE 44 */   { 0 , 0, 
		"b0;[1=].Q.1+-r" },
/* BIT 45 */   { ADDRESS , DIRECT, 
		"24;[1=];" },
/* BIT 46 */   { ADDRESS , EXTENDED, 
		"2c;[1=]y" },
/* BIT 47 */   { INSTCMOS , INSTCMOS, 
		"89;[1=];" },
/* BIT 48 */   { ADDRESS|INDEXMASK|INSTCMOS , DIRECT|INDEXX|INSTCMOS, 
		"34;[1=];" },
/* BIT 49 */   { ADDRESS|INDEXMASK|INSTCMOS , EXTENDED|INDEXX|INSTCMOS, 
		"3c;[1=]y" },
/* BLT 50 */   { 0 , 0, 
		"90;[1=].Q.1+-r" },
/* BMI 51 */   { 0 , 0, 
		"30;[1=].Q.1+-r" },
/* BNE 52 */   { 0 , 0, 
		"d0;[1=].Q.1+-r" },
/* BPL 53 */   { 0 , 0, 
		"10;[1=].Q.1+-r" },
/* BRA 54 */   { INST21 , INST21, 
		"80;[1=].Q.1+-r" },
/* BRA 55 */   { INSTCMOS , INSTCMOS, 
		"80;[1=].Q.1+-r" },
/* BRK 56 */   { 0 , 0, 
		"00;[1=];" },
/* BRK 57 */   { 0 , 0, 
		"00;" },
/* BVC 58 */   { 0 , 0, 
		"50;[1=].Q.1+-r" },
/* BVS 59 */   { 0 , 0, 
		"70;[1=].Q.1+-r" },
/* CLC 60 */   { 0 , 0, 
		"18;" },
/* CLD 61 */   { 0 , 0, 
		"d8;" },
/* CLI 62 */   { 0 , 0, 
		"58;" },
/* CLV 63 */   { 0 , 0, 
		"b8;" },
/* CMP 64 */   { ADDRESS , DIRECT, 
		"c5;[1=];" },
/* CMP 65 */   { ADDRESS , EXTENDED, 
		"cd;[1=]y" },
/* CMP 66 */   { 0 , 0, 
		"c9;[1=];" },
/* CMP 67 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"d5;[1=];" },
/* CMP 68 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"dd;[1=]y" },
/* CMP 69 */   { INDEXMASK , INDEXY, 
		"d9;[1=]y" },
/* CMP 70 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"d2;[1=];" },
/* CMP 71 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"d1;[1=];" },
/* CMP 72 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"c1;[1=];" },
/* CPX 73 */   { ADDRESS , DIRECT, 
		"e4;[1=];" },
/* CPX 74 */   { ADDRESS , EXTENDED, 
		"ec;[1=]y" },
/* CPX 75 */   { 0 , 0, 
		"e0;[1=];" },
/* CPY 76 */   { ADDRESS , DIRECT, 
		"c4;[1=];" },
/* CPY 77 */   { ADDRESS , EXTENDED, 
		"cc;[1=]y" },
/* CPY 78 */   { 0 , 0, 
		"c0;[1=];" },
/* DEC 79 */   { INSTCMOS , INSTCMOS, 
		"3a;" },
/* DEC 80 */   { ADDRESS , DIRECT, 
		"c6;[1=];" },
/* DEC 81 */   { ADDRESS , EXTENDED, 
		"ce;[1=]y" },
/* DEC 82 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"d6;[1=];" },
/* DEC 83 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"de;[1=]y" },
/* DEX 84 */   { 0 , 0, 
		"ca;" },
/* DEY 85 */   { 0 , 0, 
		"88;" },
/* EOR 86 */   { ADDRESS , DIRECT, 
		"45;[1=];" },
/* EOR 87 */   { ADDRESS , EXTENDED, 
		"4d;[1=]y" },
/* EOR 88 */   { 0 , 0, 
		"49;[1=];" },
/* EOR 89 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"55;[1=];" },
/* EOR 90 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"5d;[1=]y" },
/* EOR 91 */   { INDEXMASK , INDEXY, 
		"59;[1=]y" },
/* EOR 92 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"52;[1=];" },
/* EOR 93 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"51;[1=];" },
/* EOR 94 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"41;[1=];" },
/* INC 95 */   { INSTCMOS , INSTCMOS, 
		"1a;" },
/* INC 96 */   { ADDRESS , DIRECT, 
		"e6;[1=];" },
/* INC 97 */   { ADDRESS , EXTENDED, 
		"ee;[1=]y" },
/* INC 98 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"f6;[1=];" },
/* INC 99 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"fe;[1=]y" },
/* INX 100 */   { 0 , 0, 
		"e8;" },
/* INY 101 */   { 0 , 0, 
		"c8;" },
/* JMP 102 */   { 0 , 0, 
		"4c;[1=]y" },
/* JMP 103 */   { 0 , 0, 
		"6c;[1=]y" },
/* JMP 104 */   { INSTCMOS|INDEXMASK , INSTCMOS|INDEXX, 
		"7c;[1=]y" },
/* JSR 105 */   { 0 , 0, 
		"20;[1=]y" },
/* LDA 106 */   { ADDRESS , DIRECT, 
		"a5;[1=];" },
/* LDA 107 */   { ADDRESS , EXTENDED, 
		"ad;[1=]y" },
/* LDA 108 */   { 0 , 0, 
		"a9;[1=];" },
/* LDA 109 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"b5;[1=];" },
/* LDA 110 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"bd;[1=]y" },
/* LDA 111 */   { INDEXMASK , INDEXY, 
		"b9;[1=]y" },
/* LDA 112 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"b2;[1=];" },
/* LDA 113 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"b1;[1=];" },
/* LDA 114 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"a1;[1=];" },
/* LDX 115 */   { ADDRESS , DIRECT, 
		"a6;[1=];" },
/* LDX 116 */   { ADDRESS , EXTENDED, 
		"ae;[1=]y" },
/* LDX 117 */   { 0 , 0, 
		"a2;[1=];" },
/* LDX 118 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"b6;[1=];" },
/* LDX 119 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXY, 
		"be;[1=]y" },
/* LDY 120 */   { ADDRESS , DIRECT, 
		"a4;[1=];" },
/* LDY 121 */   { ADDRESS , EXTENDED, 
		"ac;[1=]y" },
/* LDY 122 */   { 0 , 0, 
		"a0;[1=];" },
/* LDY 123 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"b4;[1=];" },
/* LDY 124 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"bc;[1=]y" },
/* LSR 125 */   { 0 , 0, 
		"4a;" },
/* LSR 126 */   { ADDRESS , DIRECT, 
		"46;[1=];" },
/* LSR 127 */   { ADDRESS , EXTENDED, 
		"4e;[1=]y" },
/* LSR 128 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"56;[1=];" },
/* LSR 129 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"5e;[1=]y" },
/* MUL 130 */   { INST21 , INST21, 
		"02;" },
/* NOP 131 */   { 0 , 0, 
		"ea;" },
/* ORA 132 */   { ADDRESS , DIRECT, 
		"05;[1=];" },
/* ORA 133 */   { ADDRESS , EXTENDED, 
		"0d;[1=]y" },
/* ORA 134 */   { 0 , 0, 
		"09;[1=];" },
/* ORA 135 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"15;[1=];" },
/* ORA 136 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"1d;[1=]y" },
/* ORA 137 */   { INDEXMASK , INDEXY, 
		"19;[1=]y" },
/* ORA 138 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"12;[1=];" },
/* ORA 139 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"11;[1=];" },
/* ORA 140 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"01;[1=];" },
/* PHA 141 */   { 0 , 0, 
		"48;" },
/* PHP 142 */   { 0 , 0, 
		"08;" },
/* PHX 143 */   { INSTCMOS , INSTCMOS, 
		"da;" },
/* PHX 144 */   { INST21 , INST21, 
		"da;" },
/* PHY 145 */   { INSTCMOS , INSTCMOS, 
		"5a;" },
/* PHY 146 */   { INST21 , INST21, 
		"5a;" },
/* PLA 147 */   { 0 , 0, 
		"68;" },
/* PLP 148 */   { 0 , 0, 
		"28;" },
/* PLX 149 */   { INSTCMOS , INSTCMOS, 
		"fa;" },
/* PLX 150 */   { INST21 , INST21, 
		"fa;" },
/* PLY 151 */   { INSTCMOS , INSTCMOS, 
		"7a;" },
/* PLY 152 */   { INST21 , INST21, 
		"7a;" },
/* RMB0 153 */   { INSTROCKWELL , INSTROCKWELL, 
		"07;[1=];" },
/* RMB1 154 */   { INSTROCKWELL , INSTROCKWELL, 
		"17;[1=];" },
/* RMB2 155 */   { INSTROCKWELL , INSTROCKWELL, 
		"27;[1=];" },
/* RMB3 156 */   { INSTROCKWELL , INSTROCKWELL, 
		"37;[1=];" },
/* RMB4 157 */   { INSTROCKWELL , INSTROCKWELL, 
		"47;[1=];" },
/* RMB5 158 */   { INSTROCKWELL , INSTROCKWELL, 
		"57;[1=];" },
/* RMB6 159 */   { INSTROCKWELL , INSTROCKWELL, 
		"67;[1=];" },
/* RMB7 160 */   { INSTROCKWELL , INSTROCKWELL, 
		"77;[1=];" },
/* ROL 161 */   { 0 , 0, 
		"2a;" },
/* ROL 162 */   { ADDRESS , DIRECT, 
		"26;[1=];" },
/* ROL 163 */   { ADDRESS , EXTENDED, 
		"2e;[1=]y" },
/* ROL 164 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"36;[1=];" },
/* ROL 165 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"3e;[1=]y" },
/* ROR 166 */   { 0 , 0, 
		"6a;" },
/* ROR 167 */   { ADDRESS , DIRECT, 
		"66;[1=];" },
/* ROR 168 */   { ADDRESS , EXTENDED, 
		"6e;[1=]y" },
/* ROR 169 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"76;[1=];" },
/* ROR 170 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"7e;[1=]y" },
/* RTI 171 */   { 0 , 0, 
		"40;" },
/* RTS 172 */   { 0 , 0, 
		"60;" },
/* SBC 173 */   { ADDRESS , DIRECT, 
		"e5;[1=];" },
/* SBC 174 */   { ADDRESS , EXTENDED, 
		"ed;[1=]y" },
/* SBC 175 */   { 0 , 0, 
		"e9;[1=];" },
/* SBC 176 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"f5;[1=];" },
/* SBC 177 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"fd;[1=]y" },
/* SBC 178 */   { INDEXMASK , INDEXY, 
		"f9;[1=]y" },
/* SBC 179 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"f2;[1=];" },
/* SBC 180 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"f1;[1=];" },
/* SBC 181 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"e1;[1=];" },
/* SEC 182 */   { 0 , 0, 
		"38;" },
/* SED 183 */   { 0 , 0, 
		"f8;" },
/* SEI 184 */   { 0 , 0, 
		"78;" },
/* SMB0 185 */   { INSTROCKWELL , INSTROCKWELL, 
		"87;[1=];" },
/* SMB1 186 */   { INSTROCKWELL , INSTROCKWELL, 
		"97;[1=];" },
/* SMB2 187 */   { INSTROCKWELL , INSTROCKWELL, 
		"a7;[1=];" },
/* SMB3 188 */   { INSTROCKWELL , INSTROCKWELL, 
		"b7;[1=];" },
/* SMB4 189 */   { INSTROCKWELL , INSTROCKWELL, 
		"c7;[1=];" },
/* SMB5 190 */   { INSTROCKWELL , INSTROCKWELL, 
		"d7;[1=];" },
/* SMB6 191 */   { INSTROCKWELL , INSTROCKWELL, 
		"e7;[1=];" },
/* SMB7 192 */   { INSTROCKWELL , INSTROCKWELL, 
		"f7;[1=];" },
/* STA 193 */   { ADDRESS , DIRECT, 
		"85;[1=];" },
/* STA 194 */   { ADDRESS , EXTENDED, 
		"8d;[1=]y" },
/* STA 195 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"95;[1=];" },
/* STA 196 */   { ADDRESS|INDEXMASK , EXTENDED|INDEXX, 
		"9d;[1=]y" },
/* STA 197 */   { INDEXMASK , INDEXY, 
		"99;[1=]y" },
/* STA 198 */   { INSTCMOS|ADDRESS , DIRECT|INSTCMOS, 
		"92;[1=];" },
/* STA 199 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"91;[1=];" },
/* STA 200 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"81;[1=];" },
/* STX 201 */   { ADDRESS , DIRECT, 
		"86;[1=];" },
/* STX 202 */   { ADDRESS , EXTENDED, 
		"8e;[1=]y" },
/* STX 203 */   { ADDRESS|INDEXMASK , DIRECT|INDEXY, 
		"96;[1=];" },
/* STY 204 */   { ADDRESS , DIRECT, 
		"84;[1=];" },
/* STY 205 */   { ADDRESS , EXTENDED, 
		"8c;[1=]y" },
/* STY 206 */   { ADDRESS|INDEXMASK , DIRECT|INDEXX, 
		"94;[1=];" },
/* STZ 207 */   { ADDRESS|INSTCMOS , DIRECT|INSTCMOS, 
		"64;[1=];" },
/* STZ 208 */   { ADDRESS|INSTCMOS , EXTENDED|INSTCMOS, 
		"9c;[1=]y" },
/* STZ 209 */   { ADDRESS|INDEXMASK|INSTCMOS , DIRECT|INDEXX|INSTCMOS, 
		"74;[1=];" },
/* STZ 210 */   { ADDRESS|INDEXMASK|INSTCMOS , EXTENDED|INDEXX|INSTCMOS, 
		"9e;[1=]y" },
/* TAX 211 */   { 0 , 0, 
		"aa;" },
/* TAY 212 */   { 0 , 0, 
		"a8;" },
/* TRB 213 */   { ADDRESS|INSTCMOS , DIRECT|INSTCMOS, 
		"14;[1=];" },
/* TRB 214 */   { ADDRESS|INSTCMOS , EXTENDED|INSTCMOS, 
		"1c;[1=]y" },
/* TSB 215 */   { ADDRESS|INSTCMOS , DIRECT|INSTCMOS, 
		"04;[1=];" },
/* TSB 216 */   { ADDRESS|INSTCMOS , EXTENDED|INSTCMOS, 
		"0c;[1=]y" },
/* TSX 217 */   { 0 , 0, 
		"ba;" },
/* TXA 218 */   { 0 , 0, 
		"8a;" },
/* TXS 219 */   { 0 , 0, 
		"9a;" },
/* TYA 220 */   { 0 , 0, 
		"98;" },
	{ 0,0,""} };
/* end fraptabdef.c */
