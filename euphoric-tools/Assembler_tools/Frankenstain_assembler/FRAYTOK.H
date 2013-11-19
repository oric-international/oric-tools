typedef union {
	int	intv;
	long 	longv;
	char	*strng;
	struct symel *symb;
} YYSTYPE;
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


extern YYSTYPE yylval;
