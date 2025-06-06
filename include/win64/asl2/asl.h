﻿/*******************************************************************
Copyright (C) 2017, 2018, 2020 AMPL Optimization, Inc.; written by David M. Gay.

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that the copyright notice and this permission notice and warranty
disclaimer appear in supporting documentation.

The author and AMPL Optimization, Inc. disclaim all warranties with
regard to this software, including all implied warranties of
merchantability and fitness.  In no event shall the author be liable
for any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether in an
action of contract, negligence or other tortious action, arising out
of or in connection with the use or performance of this software.
*******************************************************************/

#ifndef ASL_included
#define ASL_included

#include "arith.h"	/* for Long and Double_Align */
#include "math.h"
#include "stdio1.h"
#include "string.h"

#include "stdlib.h"
#include "errno.h"

#define _ASL_EW_

#define Const const	/* for remnants of KR_headers */
#define VOID void	/* ditto */

#ifdef __cplusplus
#define Cextern extern "C"
extern "C" {
#else
#define Cextern extern
#endif

#ifndef Stderr
extern FILE *Stderr;
#endif

#ifndef Long
#define Long long
#endif
#ifndef F2C_INCLUDE
typedef Long fint;
typedef Long ftnlen;
#endif
#ifndef Intcast
#define Intcast (size_t)
#endif

#ifndef Not_Used
#define Not_Used(x) x = x	/* silence non-use warnings */
#endif

#define Malloc(x) mymalloc((size_t)(x))
#define Realloc(x,y) myralloc(x,(size_t)(y))
#define Sizeof(x) sizeof(x)
/* prior to 20110912, Sizeof(x) was (fint)sizeof(x) */

 /* Place qsortv declaration ahead of funcadd.h in case Permit_AE_redefs is #defined. */
 extern void qsortv(void*, size_t, size_t, int(*)(const void*,const void*,void*), void*);

#ifndef FUNCADD_H_INCLUDED
#ifndef Permit_AE_redefs
#define No_AE_redefs
#endif
#include "funcadd.h"
#endif
#include "setjmp.h"

 typedef size_t (*Fwrite)(const void*, size_t, size_t, FILE*);
 typedef int (*Add_Indicator)(void*, int, int, int, int, int*, real*, real);
 typedef struct {jmp_buf jb;} Jmp_buf;
 typedef struct ASL ASL;
 typedef struct EvalWorkspace EvalWorkspace;
 typedef struct MPEC_Adjust MPEC_Adjust;
 typedef struct Objrep Objrep;
 typedef struct Option_Info Option_Info;
 typedef struct cgrad cgrad;
 typedef struct cplist cplist;
 typedef struct derp derp;
 typedef struct func_info func_info;
 typedef struct lincoef lincoef;
 typedef struct ograd ograd;
 typedef struct plterm plterm;
 typedef unsigned int uint;


 typedef struct
SputInfo {
	fint *hcolstarts;
	fint *hrownos;
	size_t *hcs[2];
	fint *hrn[2];
	ssize_t *ulinc;
	ssize_t *ulcopy;
	ssize_t *ulinc0;
	ssize_t *ulcopy0;
	ssize_t *ulcend;
	ssize_t nod;
	int nobj;	/* from sphsetup() */
	int ow;		/* from sphsetup() */
	int y;		/* from sphsetup() */
	int khinfo;	/* mblk size in sphes_setup */
	int khinfob;
	int uptri;	/* from sphsetup() */
	int *uptolow;
	size_t *hcolstartsZ;
	} SputInfo;

 typedef union
uirp {
	int i;
	real *rp;
	void *vp;
	} uirp;

#ifdef ASL_big_goff /*{{ for jacval() with nzc >= 2^31 */
 struct
cgrad {
	real coef;
	cgrad *next;
	ssize_t varno;
	size_t goff;
	};

 struct
ograd {
	real coef;
	ograd *next;
	ssize_t varno;
	};
typedef ssize_t varno_t;
#else	/*}{ less memory */
 struct
cgrad {
	real coef;
	cgrad *next;
	int varno;
	int goff;
	};

 struct
ograd {
	real coef;
	ograd *next;
	int varno;
	};
typedef int varno_t;
#endif /*}}*/

 struct
cplist {
	cplist	*next;
	uirp	ca;
	real	*cfa;
	};

 struct
derp { uint a, b; int c; };

 typedef struct
derpblock {
	derp *d0, *de;
	struct derpblock *next;
	size_t nxt;
	} derpblock;

 struct
func_info {
	func_info	*next, *fnext;
	const char	*name;
	ufunc		*funcp;
	int		ftype;
	int		nargs;
	void		*funcinfo;
	};

 struct
lincoef {
	size_t	varno;
	real	coef;
	};

 typedef struct
linpart {
	uint n;	/* number of coefficients */
	int a;
	lincoef lc[1];	/* really lc[n] */
	} linpart;

 struct
plterm {
	int	n;	/* number of slopes */
	int	z;	/* bs[z] == slope at 0 */
	real	bs[1];	/* slope 1, bkpt 1, slope 2, bkpt 2, ..., slope n */
	};

 typedef struct
EdRead_ASL {
	ASL *asl;
	FILE *nl;
	void *S;	/* Static */
	Long Line;
	int lineinc;
	int can_end;
	char rl_buf[80];
	void (*iadjfcn)(void*, size_t);
	void (*dadjfcn)(void*, size_t);
	} EdRead_ASL;
#define EdRead EdRead_ASL

 typedef struct ASLhead ASLhead; struct
ASLhead { ASLhead *next, *prev; };

#define NFHASH 23

 typedef struct
Edagpars {
	ASLhead h;
	/* stuff initialized to nonzero values */
	real hffactor;
	int FUNNEL_MIN_;
	int maxfwd_;
	int need_funcadd_;
	int vrefGulp_;
	int want_derivs_;
	int ihd_limit_;
	int solve_code_;
	real (*Objval)		(EvalWorkspace*,int nobj, real *X, fint *nerror);
	real (*Objval_nomap)	(EvalWorkspace*,int nobj, real *X, fint *nerror);
	void (*Objgrd)		(EvalWorkspace*,int nobj, real *X, real *G, fint *nerror);
	void (*Objgrd_nomap)	(EvalWorkspace*,int nobj, real *X, real *G, fint *nerror);
	void (*Conval)		(EvalWorkspace*,real *X, real *R, fint *nerror);
	void (*Jacval)		(EvalWorkspace*,real *X, real *J, fint *nerror);
	real (*Conival)		(EvalWorkspace*,int ncon, real *X, fint *nerror);
	real (*Conival_nomap)	(EvalWorkspace*,int ncon, real *X, fint *nerror);
	void (*Congrd)		(EvalWorkspace*,int nc, real *X, real *G, fint *nerror);
	void (*Congrd_nomap)	(EvalWorkspace*,int nc, real *X, real *G, fint *nerror);
	void (*Hvcomp)		(EvalWorkspace*,real *hv, real *p, int no, real *ow, real *y);
	void (*Hvcomp_nomap)	(EvalWorkspace*,real *hv, real *p, int no, real *ow, real *y);
	void (*Hvcompe)		(EvalWorkspace*,real *hv, real *p, int no, real *ow, real *y, fint*);
	void (*Hvcompe_nomap)	(EvalWorkspace*,real *hv, real *p, int no, real *ow, real *y, fint*);
	void (*Hvcompd)		(EvalWorkspace*,real *hv, real *p, int co);
	void (*Hvcompde)	(EvalWorkspace*,real *hv, real *p, int co, fint*);
	varno_t (*Hvcomps)	(EvalWorkspace*,real *hv, real *p, int co, varno_t nz, varno_t *z);
	varno_t (*Hvcompse)	(EvalWorkspace*,real *hv, real *p, int co, varno_t nz, varno_t *z, fint*);
	void (*Hvinit)		(EvalWorkspace*,int hid_limit, int nobj, real *ow, real *y);
	void (*Hvinit_nomap)	(EvalWorkspace*,int hid_limit, int nobj, real *ow, real *y);
	void (*Hvinite)		(EvalWorkspace*,int hid_limit, int nobj, real *ow, real *y, fint*);
	void (*Hvinite_nomap)	(EvalWorkspace*,int hid_limit, int nobj, real *ow, real *y, fint*);
/*	void (*Hesset)		(EvalWorkspace*,int flags, int no, int nno, int nc, int nnc);*/
	int  (*Lconval)		(EvalWorkspace*,int ncon, real *X, fint *nerror);
	int  (*Xknown)		(EvalWorkspace*,real*, fint*);
	void (*Duthes)		(EvalWorkspace*,real *H, int nobj, real *ow, real *y);
	void (*Duthes_nomap)	(EvalWorkspace*,real *H, int nobj, real *ow, real *y);
	void (*Duthese)		(EvalWorkspace*,real *H, int nobj, real *ow, real *y, fint*);
	void (*Duthese_nomap)	(EvalWorkspace*,real *H, int nobj, real *ow, real *y, fint*);
	void (*Fulhes)		(EvalWorkspace*,real *H, fint LH, int no, real *ow, real *y);
	void (*Fulhes_nomap)	(EvalWorkspace*,real *H, fint LH, int no, real *ow, real *y);
	void (*Fulhese)		(EvalWorkspace*,real *H, fint LH, int no, real *ow, real *y, fint*);
	void (*Fulhese_nomap)	(EvalWorkspace*,real *H, fint LH, int no, real *ow, real *y, fint*);
	void (*Sphes)		(EvalWorkspace*,SputInfo**, real *H, int nobj, real *ow, real *y);
	void (*Sphes_nomap)	(EvalWorkspace*,SputInfo**, real *H, int nobj, real *ow, real *y);
	void (*Sphese)		(EvalWorkspace*,SputInfo**, real *H, int nobj, real *ow, real *y, fint*);
	void (*Sphese_nomap)	(EvalWorkspace*,SputInfo**, real *H, int nobj, real *ow, real *y, fint*);
	fint (*Sphset)		(EvalWorkspace*,SputInfo**, int nobj, int ow, int y, int uptri);
	fint (*Sphset_nomap)	(EvalWorkspace*,SputInfo**, int nobj, int ow, int y, int uptri);
	EvalWorkspace* (*EWalloc)(ASL*);
	} Edagpars;

 extern Edagpars edagpars_ASL;

#define objval(np,x,ne)		asl->p.Objval(asl->i.Ew0,np,x,ne)
#define objgrd(np,x,g,ne)	asl->p.Objgrd(asl->i.Ew0,np,x,g,ne)
#define conval(x,r,ne)		asl->p.Conval(asl->i.Ew0,x,r,ne)
#define jacval(x,j,ne)		asl->p.Jacval(asl->i.Ew0,x,j,ne)
#define conival(I,x,ne)		asl->p.Conival(asl->i.Ew0,I,x,ne)
#define congrd(I,x,g,ne)	asl->p.Congrd(asl->i.Ew0,I,x,g,ne)
#define hvcomp(hv,P,no,ow,y)	asl->p.Hvcomp(asl->i.Ew0,hv,P,no,ow,y)
#define hvcompd(hv,P,co)	asl->p.Hvcompd(asl->i.Ew0,hv,P,co)
#define hvcomps(hv,P,co,nz,z)	asl->p.Hvcomps(asl->i.Ew0,hv,P,co,nz,z)
#define hvinit(no,ow,y)		asl->p.Hvinit(asl->i.Ew0,ihd_limit,no,ow,y)
/*#define hesset(f,o,n,c,nc)	asl->p.Hesset(asl->i.Ew0,f,o,n,c,nc)*/
#define duthes(h,n,ow,y)	asl->p.Duthes(asl->i.Ew0,h,n,ow,y)
#define duthese(h,n,ow,y,ne)	asl->p.Duthese(asl->i.Ew0,h,n,ow,y,ne)
#define fullhes(h,lh,n,ow,y)	asl->p.Fulhes(asl->i.Ew0,h,lh,n,ow,y)
#define fullhese(h,lh,n,ow,y,e)	asl->p.Fulhese(asl->i.Ew0,h,lh,n,ow,y,e)
#define lconval(I,x,ne)		asl->p.Lconval(asl->i.Ew0,I,x,ne)
#define sphes(h,no,ow,y)	asl->p.Sphes(asl->i.Ew0,0,h,no,ow,y)
#define sphese(h,no,ow,y,ne)	asl->p.Sphese(asl->i.Ew0,0,h,no,ow,y,ne)
#define sphsetup(no,ow,y,b)	asl->p.Sphset(asl->i.Ew0,0,no,ow,y,b)
#define xknown(x)		asl->p.Xknown(asl->i.Ew0,x,0)
#define xknown_ew(ew,x)		asl->p.Xknown(ew,x,0)
#define xknowne(x,ne)		asl->p.Xknown(asl->i.Ew0,x,ne)
#define xknowne_ew(ew,x,ne)	asl->p.Xknown(ew,x,ne)
#define objval_ew(ew,np,x,ne)	asl->p.Objval(ew,np,x,ne)
#define objgrd_ew(ew,np,x,g,ne)	asl->p.Objgrd(ew,np,x,g,ne)
#define conval_ew(ew,x,r,ne)	asl->p.Conval(ew,x,r,ne)
#define jacval_ew(ew,x,j,ne)	asl->p.Jacval(ew,x,j,ne)
#define conival_ew(ew,i,x,ne)	asl->p.Conival(ew,i,x,ne)
#define congrd_ew(ew,i,x,g,ne)	asl->p.Congrd(ew,i,x,g,ne)
#define hvcomp_ew(ew,hv,P,no,ow,y)	asl->p.Hvcomp(ew,hv,P,no,ow,y)
#define hvcompd_ew(ew,hv,P,co)		asl->p.Hvcompd(ew,hv,P,co)
#define hvcomps_ew(ew,hv,P,co,nz,z)	asl->p.Hvcomps(ew,hv,P,co,nz,z)
#define hvinit_ew(ew,no,ow,y)		asl->p.Hvinit(ew,ihd_limit,no,ow,y)
#define duthes_ew(ew,h,n,ow,y)		asl->p.Duthes(ew,h,n,ow,y)
#define duthese_ew(ew,h,n,ow,y,ne)	asl->p.Duthese(ew,h,n,ow,y,ne)
#define fullhes_ew(ew,h,lh,n,ow,y)	asl->p.Fulhes(ew,h,lh,n,ow,y)
#define fullhese_ew(ew,h,lh,n,ow,y,ne)	asl->p.Fulhese(ew,h,lh,n,ow,y,ne)
#define lconval_ew(ew,i,x,ne)		asl->p.Lconval(ew,i,x,ne)
#define sphes_ew(ew,h,no,ow,y)		asl->p.Sphes(ew,0,h,no,ow,y)
#define sphese_ew(ew,h,no,ow,y,ne)	asl->p.Sphese(ew,0,h,no,ow,y,ne)
#define sphsetup_ew(ew,no,ow,y,b)	asl->p.Sphset(ew,0,no,ow,y,b)
#define xunknown()			xunknown_ASL(asl->i.Ew0)
#define xunknown_ew(ew)			xunknown_ASL(ew)
#define ewalloc()			asl->p.EWalloc(asl)

#define FUNNEL_MIN	asl->p.FUNNEL_MIN_
#define hesset(f,o,n,c,nc)	/* nothing */
#define ihd_limit	asl->p.ihd_limit_
#define maxfwd		asl->p.maxfwd_
#define need_funcadd	asl->p.need_funcadd_
#define solve_code	asl->p.solve_code_
#define solve_result_num asl->p.solve_code_
#define sputinfo	asl->i.Ew0->Sputinfo
#define vrefGulp	asl->p.vrefGulp_
#define want_deriv	asl->i.Ew0->wantderiv
#define want_derivs	asl->p.want_derivs_

 typedef struct
SufDesc {	/* suffix description */
	char *sufname;	/* suffix name */
	char *table;	/* for return to AMPL, suffix_table value */
	int kind;
	int nextra;	/* extra entries to allocate in u.i or u.r */
	struct {	/* was union, but this made debugging harder */
		int *i;
		real *r;
		} u;
	struct SufDesc *next;
	/* The number of entries in u.i or u.r is */
	/* (&asl->i.n_var_)[kind & ASL_Sufkind_mask] . */
	} SufDesc;

 enum { /* kind bits for SufDesc */
	ASL_Sufkind_var  = 0,
	ASL_Sufkind_con  = 1,
	ASL_Sufkind_obj  = 2,
	ASL_Sufkind_prob = 3,
	ASL_Sufkind_mask = 3,	/* mask for getting one of the above */
				/* values from SufDesc.kind */
	ASL_Sufkind_real = 4,	/* use SufDesc.u.r rather than .i */
	ASL_Sufkind_iodcl = 8,	/* tell AMPL to make this an INOUT suffix */
	ASL_Sufkind_output = 16,/* return this suffix to AMPL */
	ASL_Sufkind_input = 32,	/* input values were received from AMPL */
	ASL_Sufkind_outonly = 64/* reject as an input value */
	};

 typedef struct
SufDecl {	/* Pass array of SufDecl's to suf_declare(). */
		char *name;
		char *table;
		int kind;
		int nextra;
		} SufDecl;

 typedef struct Exitcall Exitcall; struct
Exitcall {
		Exitcall *prev;
		Exitfunc *ef;
		void *v;
		};

#define NO_PTHREADS
#ifdef ALLOW_OPENMP
#undef MULTIPLE_THREADS
#define MULTIPLE_THREADS
#include "omp.h"
#undef MBLK_LOCK
#define MBLK_LOCK omp_lock_t
#define ACQUIRE_MBLK_LOCK(I,L) if ((I)->rd_M1z_bytes) omp_set_lock(&((I)->L))
#define FREE_MBLK_LOCK(I,L) if ((I)->rd_M1z_bytes) omp_unset_lock(&((I)->L))
#else
#if !defined(MULTIPLE_THREADS) || defined(NO_MBLK_LOCK)
#undef MBLK_LOCK
#undef MULTIPLE_THREADS
#define ACQUIRE_MBLK_LOCK(I,L) /*nothing*/
#define FREE_MBLK_LOCK(I,L) /*nothing*/
#else
#undef NO_PTHREADS
#include <pthread.h>
#define MBLK_LOCK pthread_mutex_t
#ifndef MULTIPLE_THREADS
#define MULTIPLE_THREADS
#endif
#define ACQUIRE_MBLK_LOCK(I,L) pthread_mutex_lock(&((I)->L))
#define FREE_MBLK_LOCK(I,L) pthread_mutex_unlock(&((I)->L))
#endif
#endif

 typedef struct
EvalStats {
	/* numbers of evaluations */
	int objval;
	int objgrd;
	int conval;
	int jacval;
	int conival;
	int conigrd;
	int hesmat;
	int hesvec;
	int lconval;
	/* numbers of x values */
	int newx;	/* distinct x values */
	int oldx;	/* checks that could have been avoided with xknown() */
	} EvalStats;

 typedef struct DerivErrInfo DerivErrInfo;
 typedef struct linarg linarg;
 typedef struct ParHvInfo ParHvInfo;
 typedef struct ParHvInit ParHvInit;

 struct
EvalWorkspace {
	ASL *asl;
	real *w;	/* current X == first n_var entries in w == old Lastx */
			/* negative w subscripts are for constants  */
	void *dv;	/* start of shared defined variables */
	void *dv1;	/* start of defined variables used in only one entity */
	int	wantderiv;
	int	x0kind;
	int	co_index;	/* set this to (constraint number - 1) or */
				/* -(objective number) to identify the */
				/* constraint or objective being evaluated */
				/* (used in report_where()) */
	int	cv_index;	/* used internally */
	size_t	nxval;		/* used by conival and objval */
	size_t	onxval;		/* for xpsg_check */
	Jmp_buf	*err_jmpw;	/* If nonzero when an error is detected, */
				/* longjmp here (without printing an error */
				/* message). */
	Jmp_buf	*err_jmpw1;	/* If nonzero when an error is detected */
				/* (and err_jmp_ == 0), longjmp here after */
				/* printing an error message. */
	size_t *ncxval;		/* for conival */
	size_t *ncxgval;	/* for conigrd */
	size_t *nlxval;		/* for lconval */
	size_t *noxval;		/* for objval */
	size_t *noxgval;	/* for objgrd */
	real *Lastx;

	real *derps;
	real *unopscr;		/* scratch for unary-op chains */

	/* for derivative errors */
	DerivErrInfo *Derrs, *Derrs0;

	/* for Hessians */

	SputInfo *Sputinfo;	/* used by sputhes() */
	/* for simplifying complementarities */
	real *orscratch;	/* scratch (if needed) */
	int	ihdcur;		/* Current max internal Hessian dimension, */
				/* set by hvpinit. */
	int	ndhmax;		/* set by hvpinit. */
	int	nhvprod;	/* # of Hessian-vector products at this Hessian */
	int	npsgcomp;	/* Has psgcomp been called?  For sphes_setup. */
	real	*oyow;		/* for xpsg_check */
	real	*oyow0;		/* for xpsg_check */
	real	*H0;		/* for sphes */
	real	*hvscratch;	/* for parallel Hessian-vector products */
	struct Hesoprod *hop_free, *hop_free0, *hop_free_end;
	struct uHeswork *uhw_free, *uhw_free0, *uhw_free_end;
	arglist	*al;
	int	onobj;		/* for xpsg_check */
	int	nynz;		/* for xpsg_check */
	int	hes_setup_called;
	int	nlv;		/* max(nlvc, nlvo) */
	void	*dbuginfo;
	EvalWorkspace *ewthread;
	ParHvInfo *parhvinfo;
	ParHvInit *parhvinit;
	EvalStats stats;
	};

 enum {MBLK_KMAX_ASL = 30};

 typedef struct
MBhead {
	uint klass;
	uint nalloc;
	} MBhead;

 typedef struct
MBavail {
	MBhead h;
	struct MBavail *mbnext;
	} MBavail;

 typedef struct
MBFree {
	uint nalloc, nfree;
	MBavail *a;
	} MBFree;

 typedef struct
Edaginfo {
	int ASLtype;
	/* stuff initialized to zero values */
	int amplflag_;
	int need_nl_;
	int nlmode;
	func_info **funcs_, *funcsfirst_, *funcslast_;
	int (*xscanf_)(EdRead*, const char*, ...);
	func_info *fhash_[NFHASH];

	real	*LUrhs_,	/* constraint lower (and, if Urhsx == 0, */
				/* upper) bounds */
		*Urhsx_,	/* constraint upper bounds (if nonzero) */
		*X0_,		/* initial guess (if nonzero) */
		*LUv_,		/* variable lower (and, if Uvx == 0, upper) */
				/* bounds */
		*Uvx_,		/* variable upper bounds (if nonzero) */
		*pi0_,		/* dual initial guess */
		*objconst;	/* objective constant terms */

	char	*objtype_;	/* object type array: 0 == min, 1 == max */
	char	*havex0_;	/* if nonzero, havex0_[i] != 0 ==> */
				/* X0_[i] was specified: this lets you */
				/* tell explicit 0's from default 0's */
	char	*havepi0_;	/* analogous to havex0_, but for dual values */
	real	*A_vals_;	/* If nonzero, store constant Jacobian values */
				/* (possibly 0 when nonlinearities are involved) */
				/* in A_vals, A_rownos, and A_colstarts, */
				/* rather than in Cgrad_. */
	int	*A_rownos_,	/* row numbers corresponding to A_vals_ */
		*A_colstarts_;	/* offsets of columns in A_vals_ */
	size_t	*A_colstartsZ_;	/* for huge problems -- 2^31 or more nonzeros */

	cgrad	**Cgrad_;	/* constraint gradient info. (linear part) */
	ograd	**Ograd_;	/* objective  gradient info. (linear part) */
	cgrad	**Cgrad0;	/* unmapped */

	int	Fortran_;	/* adjustment to A_rownos, A_colstarts, */
				/* ccind1, and ccind2 */

			/* stuff for common expressions (from "defined" vars) */
	int	c_vars_;
	int	comb_;
	int	combc_;
	int	comc1_;
	int	comc_;
	int	como1_;
	int	como_;
	int	defvar0;	/* offset of first defined var */
	int	maxvar;		/* offset of first added defined var */

	int	lnc_;		/* no. of linear network constraints */
	int	nbv_;		/* no. of linear binary variables */
	int	niv_;		/* no. of linear non-binary integer variables */
	int	nlc_;		/* total no. of nonlinear constraints */
	int	n_eqn_;		/* number of equality constraints or -1 */
				/* if unknown (ampl prior to 19970627) */
	int	n_cc_;		/* total complementarity conditions */
	int	nlcc_;		/* nonlinear complementarity conditions */
	int	ndcc_;		/* number of complementarities involving */
				/* double inequalities (for ASL_cc_simplify) */
	int	nzlb_;		/* number of complemented variables with a */
				/* nonzero lower bound (for ASL_cc_simplify) */
	int	nlnc_;		/* no. of nonlinear network constraints */
	int	nlo_;		/* no. of nonlinear objectives */
	int	nlvb_;		/* no. of nonlinear variables in both */
				/* constraints and objectives */
	int	nlvc_;		/* no. of nonlinear variables in constraints */
	int	nlvo_;		/* no. of nonlinear variables in objectives */
				/* nlvc_ and nlvo_ include nlvb_ */
	int	nlvbi_;		/* integer nonlinear variables in both */
				/* constraints and objectives */
	int	nlvci_;		/* integer nonlinear vars just in constraints */
	int	nlvoi_;		/* integer nonlinear vars just in objectives */
	int	nwv_;		/* no. of (linear) network variables (arcs) */
	int	nzc_;		/* no. of nonzeros in constraints' Jacobian */
	int	nzo_;		/* no. of nonzeros in all objective gradients */
	int	n_var_;		/* total no. of variables */
	int	n_con_;		/* total no. of constraints */
	int	n_obj_;		/* total no. of objectives */
	int	n_prob;		/* 1 (for use with SufDesc): SufDesc.u has */
	/* (&asl->i.n_var_)[SufDesc.kind & ASL_Sufkind_mask] entries */
	int	n_lcon_;	/* no. of logical constraints */
	int	flags;		/* 1 = want output suffixes */
	int	n_conjac_[2];	/* Conval and Jacval operate on constraint i */
				/* for n_conjac_[0] <= i < n_conjac_[1]. */
				/* Initialized by jac0dim to 0 and n_con. */
				/* The .nl reader may adjust these values when */
				/* processing ASL_cc_simplify or ASL_obj_replace. */


			/* internal stuff */

	int	nclcon_;	/* n_con + n_lcon */
	int	ncom0_;		/* comb + comc + como */
	int	ncom1_;		/* comc1 + como1 */
	int	nfunc_;
	int	o_vars_;
	int	x0kindinit;	/* for EvalWorkspace.x0kind */
	int	rflags;		/* flags given to the .nl reader */
	size_t	nzjac_;
	size_t	x0len_;
	size_t	nZc_;		/* no. of nonzeros in constraints' Jacobian */
	size_t	nZo_;		/* no. of nonzeros in all objective gradients */

	char	*filename_;	/* stub + current extension */
	char	*stub_end_;	/* copy new extension (starting with ".") */
				/* here to adjust filename */
	void	*archan_;	/* channel for reading from AMPL */
	void	*awchan_;	/* channel from writing to AMPL */
	Jmp_buf	*err_jmp_;	/* If nonzero when an error is detected when */
				/* reading the .nl file, longjmp here */
				/* (without printing an error message). */
	Jmp_buf	*err_jmp1_;	/* If nonzero when an error is detected when */
				/* reading the .nl file, longjmp here */
				/* after printing an error message. */
	int	binary_nl_;	/* 0 = ASCII format, 1 = binary */
	int	return_nofile_;	/* 0 ==> jacdim0 should exit if stub.nl */
				/* does not exist; 1 ==> return 0 */
	int	plterms_;	/* no. of piecewise-linear terms */
	int	maxrownamelen_;	/* length of longest constraint name */
				/* (if stub.row exists) */
	int	maxcolnamelen_;	/* length of longest constraint name */
				/* (if stub.col exists) */
	fint	ampl_options_[10];
	fint	obj_no_;	/* objective number (for write_sol and */
				/* read_sol) */
	int	nranges_;	/* no. of ranges (constraints with */
				/* negInfinity < lhs < rhs < Infinity) */
	int	want_xpi0_;	/* & 1 ==> allocate X0_ if an */
				/* initial guess is available */
				/* & 2 ==> allocate pi0_ if a dual */
				/* initial guess is available */

		/* starting subscripts for cexp1's */

	int	*c_cexp1st_;	/* cexp1 starts for constraints */
	int	*l_cexp1st_;	/* cexp1 starts for logical constraints */
	int	*o_cexp1st_;	/* cexp1 starts for objectives */

		/* for complementarity constraints */

	int	*cvar_;	/* cvar[i] > 0 means constraint i complements */
			/* variable cvar[i] - 1 */

	int	*ccind1, *ccind2; /* populated when ASL_cc_simplify is */
			/* "on" in the flags argument to the .nl reader */

	/* extra info for write_sol */
	real ampl_vbtol_;

	/* for nlc */

	int	skip_int_derivs_;

	/* for suffixes */

	int	nsuffixes;
	int	nsufext[4];
	int	nsuff[4];
	SufDesc *suffixes[4];

	/* for sparse gradients */

	int	**zerograds_;
	int	congrd_mode;	/* How to treat arg G to Congrd: */
				/* 0 == expand to n_var vector (default) */
				/* 1 == compact storage in G */
				/* 2 == use goff fields of struct cgrad */
	int nlvog;		/* objgrd(np,x,g,ne) sets g[i] = 0 */
				/* if the objective does not depend on x[i] */
				/* and i < nlvog (or i < max(c_vars, o_vars) */
				/* if nlvog is left at 0); nlvog must be set */
				/* before the .nl reader is called. */
	size_t zap_J;		/* used by dense_j */
	size_t derplen;		/* length of scratch array for derprop */
	size_t wlen;		/* length of array EvalWorkspace.w */
	size_t numlen;		/* length of numvals */
	size_t opunused;	/* length of unused portion of last op block */
	size_t derpunused;	/* length of unused porttion of last derpblock */
	real *numvals;

	/* for ASL_free */
	void **Mbnext, **Mblast, *Mb;
	/* for mem */
	char *memNext, *memLast;

	/* for user-defined functions */
	AmplExports *ae;

	/* for con_name(), obj_name(), var_name() */

	char **connames;
	char **lconnames;
	char **objnames;
	char **varnames;
	int vcochecked;

	/* for imported functions*/
	int ra_max;	/* max number of real args */
	int sa_max;	/* max number of symbolic args */
	int nfinv;	/* number of imported function invocations */
	void **invd;	/* invocation details; length nfinv; details vary */

	/* for possible user-specific use */
	void *uinfo;

	/* for reading alternate binary formats */
	void (*iadjfcn)(void*, size_t);
	void (*dadjfcn)(void*, size_t);
	const char *opfmt;	/* format of opcodes */

	/* for scaling */
	real *cscale;	/* constraints */
	real *vscale;	/* variables */
	real *lscale;	/* Lagrange-multiplier scale */

	/* for at_reset() */

	Exitcall *arlast;
	Exitcall *arnext;
	Exitcall *arprev;

	/* for suf_sos() */

	SufDesc	*csd;
	SufDesc *rsd;
	int	n_var0;	/* number of variables before suf_sos() */
	int	n_con0;	/* number of constraints before suf_sos() */
	int	n_var1; /* index of next variable to be added */
	int	n_con1; /* index of next constraint to be added */
	int	*vmap;	/* for mapping variables, length n_var */
	int	*cmap;	/* for mapping constraints, length n_con */
	int	*vzap;	/* for zeroing primal variables in eliminated suf_sos */
			/* constraints: vzmap[i] for 1 <= i <= vzmap[0] */
	int	*czap;	/* for zeroing dual variables of eliminated suf_sos */
			/* constraints: czap[i] for 1 <= i <= czmap[0] */
	int	*vminv;	/* inverse of vmap; allocated on demand */

	/* for modifying objectives */
	Objrep	**Or;
	void (*opify)(ASL*);
	int nlc0, nlo0;	/* values of nlc_ and nlo_ before obj_adj() */

	/* for simplifying complementarities */
	MPEC_Adjust *mpa;

	/* bounds and solution filenames */
	char *boundsfile;
	char *solfile;

	/* for new_mblk and del_mblk */
	MBFree mblk_free[MBLK_KMAX_ASL];

	/* for single-threaded evaluations */
	EvalWorkspace *Ew0;

	/* for debugging */
	EvalWorkspace *ewthread, **pewthread;

	/* memory use statistics */
	size_t temp_rd_bytes;	/* bytes temporarily allocated during .nl read */
	size_t tot_M1z_bytes;	/* total allocated by M1alloc and M1zapalloc */
	size_t rd_M1z_bytes;	/* tot_M1z_bytes after reading the .nl file */
	size_t ew_bytes;	/* bytes allocated for one EvalWorkspace */
	size_t n_ew0;		/* number of EvalWorkspace structures allocated */

#ifdef MBLK_LOCK
	MBLK_LOCK MemLock, Mem1Lock;
#endif
	} Edaginfo;

 struct
ASL {
	Edagpars p;
	Edaginfo i;
	};

 typedef struct
NewVCO {
	int	nnv;	/* number of new variables */
	int	nnc;	/* number of new constraints */
	int	nno;	/* number of new objectives */
	ograd	**newc;	/* new constraint nonzeros */
	ograd	**newo;	/* new objective nonzeros */
	real	*LUnv;	/* nnv variable lower bounds or, if Unv is null, */
			/* nnv (lower,upper) bound pairs */
	real	*Unv;	/* null or nnv variable upper bounds */
	real	*LUnc;	/* nnc constraint lower bounds or, if Unc is null, */
			/* nnc (lower,upper) bound pairs */
	real	*Unc;	/* null or nnc constraint upper bounds */
	real	*oc;	/* null or nno objective constants */
	char	*ot;	/* objective types (1 = max, 0 = min) */
			/* taken to be all zeros (min) if null */
	real	*x0;	/* null or nnv primal initial guesses */
	real	*d0;	/* null or nnc dual initial guesses */
	} NewVCO;

 struct
TMInfo {
	union {
		TMInfo *prev;
		double align;
		} u;
	};

#define A_colstarts	asl->i.A_colstarts_
#define A_colstartsZ	asl->i.A_colstartsZ_
#define A_rownos	asl->i.A_rownos_
#define A_vals		asl->i.A_vals_
#define Cgrad		asl->i.Cgrad_
#define Fortran		asl->i.Fortran_
#define LUrhs		asl->i.LUrhs_
#define LUv		asl->i.LUv_
#define Ograd		asl->i.Ograd_
#define Urhsx		asl->i.Urhsx_
#define Uvx		asl->i.Uvx_
#define X0		asl->i.X0_
#define ampl_options	asl->i.ampl_options_
#define ampl_vbtol	asl->i.ampl_vbtol_
#define amplflag	asl->i.amplflag_
#define archan		asl->i.archan_
#define awchan		asl->i.awchan_
#define binary_nl	asl->i.binary_nl_
#define c_cexp1st	asl->i.c_cexp1st_
#define c_vars		asl->i.c_vars_
#define comb		asl->i.comb_
#define combc		asl->i.combc_
#define comc		asl->i.comc_
#define comc1		asl->i.comc1_
#define como		asl->i.como_
#define como1		asl->i.como1_
#define cvar		asl->i.cvar_
#define err_jmp		asl->i.err_jmp_
#define err_jmp1	asl->i.err_jmp1_
#define fhash		asl->i.fhash_
#define filename	asl->i.filename_
#define funcs		asl->i.funcs_
#define funcsfirst	asl->i.funcsfirst_
#define funcslast	asl->i.funcslast_
#define havepi0		asl->i.havepi0_
#define havex0		asl->i.havex0_
#define l_cexp1st	asl->i.l_cexp1st_
#define lnc		asl->i.lnc_
#define maxcolnamelen	asl->i.maxcolnamelen_
#define maxrownamelen	asl->i.maxrownamelen_
#define nZc		asl->i.nZc_
#define nZo		asl->i.nZo_
#define n_cc		asl->i.n_cc_
#define n_con		asl->i.n_con_
#define n_conjac	asl->i.n_conjac_
#define n_eqn		asl->i.n_eqn_
#define n_lcon		asl->i.n_lcon_
#define n_obj		asl->i.n_obj_
#define n_var		asl->i.n_var_
#define nbv		asl->i.nbv_
#define nclcon		asl->i.nclcon_
#define ncom0		asl->i.ncom0_
#define ncom1		asl->i.ncom1_
#define need_nl		asl->i.need_nl_
#define nfunc		asl->i.nfunc_
#define niv		asl->i.niv_
#define nlc		asl->i.nlc_
#define nlcc		asl->i.nlcc_
#define nlnc		asl->i.nlnc_
#define nlo		asl->i.nlo_
#define nlogv		asl->i.nbv_	/* nbv used to be called nlogv */
#define nlvb		asl->i.nlvb_
#define nlvbi		asl->i.nlvbi_
#define nlvc		asl->i.nlvc_
#define nlvci		asl->i.nlvci_
#define nlvo		asl->i.nlvo_
#define nlvoi		asl->i.nlvoi_
#define nranges		asl->i.nranges_
#define nwv		asl->i.nwv_
#define nzc		asl->i.nzc_
#define nzjac		asl->i.nzjac_
#define nzo		asl->i.nzo_
#define o_cexp1st	asl->i.o_cexp1st_
#define o_vars		asl->i.o_vars_
#define obj_no		asl->i.obj_no_
#define objtype		asl->i.objtype_
#define pi0		asl->i.pi0_
#define plterms		asl->i.plterms_
#define return_nofile	asl->i.return_nofile_
#define skip_int_derivs	asl->i.skip_int_derivs_
#define stub_end	asl->i.stub_end_
#define want_xpi0	asl->i.want_xpi0_
#define x0len		asl->i.x0len_
#define xscanf		asl->i.xscanf_
#define zaC		asl->i.zaC_
#define zac		asl->i.zac_
#define zao		asl->i.zao_
#define zerograds	asl->i.zerograds_

#define M1alloc(n)	M1alloc_ASL(&asl->i,n)
#define M1free		M1free_ASL
#define M1record(n)	M1record_ASL(&asl->i,n)
#define M1zapalloc(n)	M1zapalloc_ASL(&asl->i,n)
#define Mach		Mach_ASL
#define ascanf		ascanf_ASL
#define badline		badline_ASL
#define badread		badread_ASL
#define bscanf		bscanf_ASL
#define derprop		derprop_ASL
#define dynlink		dynlink_ASL
#define edag_peek	edag_peek_ASL
#define edagread_one	edag_one_ASL
#define fpecatch	fpecatch_ASL
#define fpe_jmpbuf	fpe_jmpbuf_ASL
#define func_add	func_add_ASL
#define func_lookup	func_lookup_ASL
#define g_fmt		g_fmt_ASL
#define g_fmtp		g_fmtp_ASL
#define g_fmtop		g_fmtop_ASL
#define g_fmt_E		gfmt_E_ASL
#define g_fmt_decpt	gfmt_decpt_ASL
#define hscanf		hscanf_ASL
#define htcl		htcl_ASL
#define mem(n)		mem_ASL((ASL*)asl,n)
#define mymalloc	mymalloc_ASL
#define mypow		mypow_ASL
#define myralloc	myralloc_ASL
#define obj_prec	obj_prec_ASL
#define optype		op_type_ASL
#define pr_unknown	pr_unknown_ASL
#define read_line	read_line_ASL
#define report_where	report_where_ASL
#define scream		scream_ASL
#define what_prog	what_prog_ASL

 extern real Infinity, edagread_one, negInfinity;
 extern char g_fmt_E, *progname;
 extern int g_fmt_decpt;
 extern ASL *cur_ASL;

enum { /* mode bits for ASLtype */
	ASL_read_f	= 1,
	ASL_read_fg	= 2,
	ASL_read_fgh	= 3,
	ASL_read_pfg	= 4,
	ASL_read_pfgh	= 5
	};

enum { /* bits for x0kind */
	ASL_have_conval	= 1,
	ASL_need_objcom	= 2,
	ASL_first_x	= 4,
	ASL_have_funnel = 8,	/* in con[12]ival */
	ASL_need_funnel	= 16,	/* in pshv_prod */
	ASL_need_concom = 32,
	ASL_need_comba  = 64,	/* need to compute comb adjoints */
	ASL_need_comca  = 128,	/* need to compute comc adjoints */
	ASL_need_comoa  = 256,	/* need to compute como adjoints */
	ASL_x_known	= 512,
	ASL_xknown_ignore = 1024 /* for debugging: ignore xknown() */
	};

enum ASL_reader_flag_bits {	/* bits in flags arg */
	/* values just relevant to pfg_read and pfgh_read: */
				/* If you do not need the sparsity pattern,  */
				/* you can omit linear coefficients of zero. */
	ASL_J_zerodrop  =  1,	/* Omit 0 linear terms in constraint derivs. */
	ASL_G_zerodrop	=  2,	/* Omit 0 linear terms in objective  derivs. */
	ASL_GJ_zerodrop =  3,	/* Omit both kinds of zero linear terms. */
	ASL_findOgroups =  4,	/* Find objective  group structure. */
	ASL_findCgroups =  8,	/* Find constraint group structure. */
	ASL_findgroups	= 12,	/* Find both group structures; you want this */
				/* unless you're a solver like LANCELOT that */
				/* deals explicitly with group structure. */
	ASL_find_c_class = 32,	/* Find c_class and c_class_max: see nlp.h */
	ASL_find_o_class = 64,	/* Find o_class and o_class_max: or nlp2.h */
	ASL_find_co_class = 96,	/* Find all four */
	ASL_no_ew0_alloc = 0x1000000,	/* Do not allocate asl->i.Ew0; for */
					/* use when using multiple threads. */

	/* applicable to all .nl file readers: */
	ASL_return_read_err = 16,
	ASL_keep_all_suffixes = 0x80,
	ASL_allow_CLP = 0x1000,	/* permit CLP extensions */
	ASL_find_default_no_groups = 0x2000,	/* Assume ASL_findgroups */
						/* when this bit is off. */
	/* When ASL_find_default_no_groups is on, pfg_read and pfgh_read */
	/* only honor explicit specification of the ASL_findgroups bits. */

	/* Stuff for fg_wread: */

	ASL_omit_all_suffixes = 0x100,
	ASL_keep_derivs = 0x200,
	ASL_allow_missing_funcs = 0x400,
	ASL_forbid_missing_funcs = 0x800,

	/* problem adjustments */

	ASL_no_linear_cc_rhs_adjust = 0x4000,	/* Omit constant term adjustment */
						/* of linear complementarities. */

	ASL_cc_simplify = 0x8000,	/* Provide arrays ccind1 and ccind2 and */
					/* modify complementarities (possibly */
					/* increasing n_cc) so the complementarity */
					/* constraints have the form */
					/* _svar[i] >= 0 complements _svar[j] >= 0 */
					/* for i = ccind1[k] - Fortran */
					/* and j = ccind2[k] - Fortran, */
					/* 0 <= k < n_cc. */

	ASL_obj_replace_ineq = 0x10000,	/* Whether to replace an objective of the */
					/* form minimize or maximize const*var and */
					/* a corresponding constraint of the form */
					/* var relop expr with const*expr, where */
					/* relop is either >= or <=, depending */
					/* on the sense of optimization, and var */
					/* does not appear elsewhere. */

	ASL_obj_replace_eq = 0x20000,	/* Similar to ASL_obj_replace, but where */
					/* relop is == . */

	ASL_rowwise_jac = 0x40000,	/* Whether Jacobian nonzeros should be stored */
					/* row-wise rather than column-wise, in which */
					/* case the A_colstarts array provides offsets */
					/* of rows rather than columns. */

	ASL_want_A_vals = 0x80000,	/* Allocate and use A_vals (if NULL), allowing */
					/* space needed for ASL_cc_simplify. */

	ASL_sep_U_arrays = 0x100000,	/* Allocate and use Uvx and Urhsx */

	ASL_allow_Z	= 0x200000,	/* Accept problems with nZc >= 2^31, populating */
					/* A_colstarsZ rather than  A_colstarts. */
	ASL_use_Z	= 0x400000,	/* Use A_colstartsZ rather than A_colstarts, */
					/* regardless of problem size. */
	ASL_opified	= 0x800000,	/* internal use: qp_opify called */

	/* for fx_read() */

	ASL_defer_nonlin = 0x2000000	/* Assume nonlinear constraints will be added by */
					/* calls on solver-specific interface routines. */
	};

enum ASL_reader_error_codes {
	/* return codes from .nl readers with flag ASL_ret_read_err */
	ASL_readerr_none   = 0, /* all went well */
	ASL_readerr_nofile = 1, /* cannot open .nl file */
	ASL_readerr_nonlin = 2,	/* model involves nonlinearities (ed0read) */
	ASL_readerr_argerr = 3, /* user-defined function with bad args */
	ASL_readerr_unavail= 4, /* user-defined function not available */
	ASL_readerr_corrupt= 5, /* corrupt .nl file */
	ASL_readerr_bug	   = 6,	/* bug in .nl reader */
	ASL_readerr_CLP    = 7  /* solver cannot handle CLP extensions */
	};

enum ASL_suf_sos_flags { /* bits in flags parameter of suf_sos() */
	ASL_suf_sos_explict_free = 1,	/* caller will explicitly free */
					/* returned arrays */
	ASL_suf_sos_ignore_sosno = 2,	/* ignore .sosno */
	ASL_suf_sos_ignore_amplsos = 4	/* ignore SOS information from */
					/* transformations of piecewise- */
					/* linear terms (etc.) by AMPL */
	};

enum ASL_write_flags {
	ASL_write_ASCII = 1,
	ASL_write_CR = 2,
	ASL_write_binary = 4,
	ASL_write_no_X0 = 8,
	ASL_write_no_pi0 = 16
	};

enum ASL_writer_error_codes {
	ASL_writeerr_openfail = 1,
	ASL_writeerr_badrops = 2,
	ASL_writeerr_badcexp1st = 3,
	ASL_writeerr_badNewVCO = 4
	};

#define f_OPNUM f_OPNUM_ASL
#undef basename
#define basename basename_ASL
#ifndef Sig_ret_type
#define Sig_ret_type void
#endif

 typedef struct
QPinfo {
	int nc;	/* number of nonempty columns */
	int nz;	/* number of nonzeros */
	int *colno;	/* column numbers of nonempty columns */
	size_t *colbeg;	/* nonzeros for column colno[i]: (rowno[j], delsq[j]) */
	int *rowno;	/* for colbeg[i] <= j < colbeg[i+1], except that values */
	real *delsq;	/* in colno, colbeg, and rowno are incremented by Fortran */
	} QPinfo;

 extern ASL *ASL_alloc(int);
 extern void ASL_free(ASL**);
 extern long ASLdate_ASL;
 extern void Del_mblk_ASL(ASL*, void*);
 extern EdRead *EdReadInit_ASL(EdRead*, ASL*, FILE*, void*);
 extern void LUcopy_ASL(int, real*, real*, real*);
 extern void *M1alloc_ASL(Edaginfo*, size_t);
 extern void M1free(Edaginfo*, void**, void**);
 extern void **M1record_ASL(Edaginfo*, void*);
 extern void *M1zapalloc_ASL(Edaginfo*, size_t);
 extern void MAIN__(void);
 extern void Mach_ASL(void);
 extern void No_derivs_ASL(const char*);
 extern int Sprintf(char*, const char*, ...);
 extern void Stderr_init_ASL(void);
 extern void Suf_read_ASL(EdRead*, int);
 extern void adjust_zerograds_ASL(ASL*, int);
 extern int already_ASL(const char*);
 extern int ascanf(EdRead*, const char*, ...);
 extern void auxinfo_ASL(AmplExports*);
 extern void *b_search_ASL(void *ow, int owsize, int n, char **sp, char **peq);
 extern void badasl_ASL(ASL*,int,const char*);
 extern void badline(EdRead*);
 extern void badread(EdRead*);
 extern char *basename(const char*);
 extern int bscanf(EdRead*, const char*, ...);
 extern char *con_name_ASL(ASL*,int);
 extern char *con_name_nomap_ASL(ASL*,int,int*);
 extern int conadj_ASL(ASL*,int*,int);
 extern void congrd_(fint *N, fint *I, real *X, real *G, fint *nerror);
 extern real cnival_(fint *N, fint *I, real *X, fint *nerror);
 extern void colstart_inc_ASL(ASL*);
 extern void conscale_ASL(ASL*, int, real, fint*);
 extern void conval_(fint *M, fint *N, real *X, real *F, fint *nerror);
 extern int degree_ASL(ASL*, int, void**);
 extern void delprb_(void);
 extern void dense_j_ASL(ASL*);
 extern void densej_(void);
 extern void deriv2_errchk_ASL(EvalWorkspace *ew, int jv);
 extern void deriv_errchk_ASL(EvalWorkspace *ew, int coi, int n, int jv);
 extern void deriv_errclear_ASL(EvalWorkspace *);
 extern void derprop(derpblock*, real*, real*, real);
 extern char *dtoa_r(double, int, int, int*, int*, char**, char*, size_t);
 extern void duthes_ew_ASL(EvalWorkspace*, real *H, int nobj, real *ow, real *y);
 extern ufunc *dynlink_ASL(const char*);
 extern int edag_peek(EdRead*);
 extern void equ_adjust_ASL(ASL*, int*, int*);
 extern EvalWorkspace* ewalloc1_ASL(ASL *asl);
 extern EvalWorkspace* ewalloc2_ASL(ASL *asl);
 extern void exit_ASL(EdRead*,int);
 extern int f_read_ASL(ASL*, FILE*, int);
 extern int fg_read_ASL(ASL*, FILE*, int);
 extern int fg_wread_ASL(ASL*, FILE*, int);
 extern int fgh_read_ASL(ASL*, FILE*, int);
 extern int fg_write_ASL(ASL*, const char*, NewVCO*, int);
 extern int fx_read_ASL(ASL*, FILE*, int);
 extern void fintrouble_ASL(EvalWorkspace*, func_info*, const char*, TMInfo*);
 extern void flagsave_ASL(ASL*, int);
 extern char *fread_sol_ASL(ASL*, const char *fname, real**xp, real **yp);
 extern void fullhes_ew_ASL(EvalWorkspace*, real *H, fint LH, int no, real *ow, real *y);
 extern Sig_ret_type fpecatch(int);
 extern jmp_buf fpe_jmpbuf_ASL;
 extern func_info *func_lookup(ASL*, const char*, int add);
 extern void func_add(ASL*);
 extern int g_fmt(char*, double);
 extern int g_fmtop(char*, double);
 extern int g_fmtp(char*, double, int);
 extern void gen_rownos_ASL(ASL*);
 extern ASL *get_cur_ASL(void);
 extern int *get_vcmap_ASL(ASL*, int);
 extern int *get_vminv_ASL(ASL*);
 extern char *getenv_ASL(const char*);
 extern void goff_comp_ASL(ASL*);
 extern int hscanf(EdRead*, const char*, ...);
 extern int htcl_ASL(size_t);
 extern void hvcomp_(real *hv, real *p, fint *nobj, real *ow, real *y);
 extern void hvcomp_ASL(ASL*, real *hv, real *p, int no, real *ow, real *y);
 extern void hvcompd_ASL(ASL*, real *hv, real *p, int co);
 extern varno_t hvcomps_ASL(ASL*, real *hv, real *p, int co, varno_t nz, varno_t *z);
 extern void hvinit_(fint *nobj, real *ow, real *y);
 extern void hvinit_ASL(ASL*, int hid_limit, int nobj, real *ow, real *y);
 extern int indicator_constrs_ASL(ASL*, void*, Add_Indicator, int errinfo[2]);
 extern void intcatch_ASL(ASL*, void (*)(int,void*), void*);
 extern void introuble_ASL(EvalWorkspace*, const char *who, real a, int jv);
 extern void introuble2_ASL(EvalWorkspace*, const char *who, real a, real b, int jv);
 extern FILE *jac0dim_ASL(ASL*, const char *stub, ftnlen stub_len);
 extern int  jac1dim_ASL(ASL*, const char *stub, fint *M, fint *N, fint *NO,
			fint *NZ, fint *MXROW, fint *MXCOL, ftnlen stub_len);
 extern FILE *jac_dim_ASL(ASL*, const char *stub, fint *M, fint *N, fint *NO,
			fint *NZ, fint *MXROW, fint *MXCOL, ftnlen stub_len);
 extern int  jacdim_(const char *stub, fint *M, fint *N, fint *NO, fint *NZ,
			fint *MXROW, fint *MXCOL, ftnlen stub_len);
 extern void jacinc_(fint *M, fint *N, fint *NZ,
			fint *JP, short *JI, real *X, real *L, real *U,
			real *Lrhs, real *Urhs, real *Inf);
 extern int  jacpdim_ASL(ASL*, const char *stub, fint *M, fint *N, fint *NO,
		fint *NZ, fint *MXROW, fint *MXCOL, ftnlen stub_len);
 extern void jacval_(fint *M, fint *N, fint *NZ, real *X,
			real *JAC, fint *nerror);
 extern int ka_read_ASL(ASL *, EdRead *, int, int**, size_t**);
 extern void lagscale_ASL(ASL*, real, fint*);
 extern char *lcon_name_ASL(ASL*,int);
 extern int lconval_ASL(EvalWorkspace*, int ncon, real *X, fint *nerror);
 extern void mainexit_ASL(int);
 extern void *mem_ASL(ASL*, unsigned int);
 extern void memfailure_ASL(const char *who, const char *what, size_t len);
 extern int mip_pri_ASL(ASL*,int**startp,int**nump,int**prip,fint pmax);
 extern void mnnzchk_ASL(ASL*asl,fint*M,fint*N,size_t NZ,const char*who);
 extern void mpec_adjust_ASL(ASL*);
 extern void mpec_auxvars_ASL(ASL*, real *c, real *x);
 extern fint mqpcheck_ASL(ASL*, int co, fint **rowqp, fint **colqp, real **delsqp);
 extern ssize_t mqpcheckZ_ASL(ASL*, int co, fint **rowqp, size_t **colqp, real **delsqp);
 extern ssize_t mqpcheckv_ASL(ASL*, int co, QPinfo **QPIp, void **vp);
 extern void mqpcheckv_free_ASL(ASL*, void **vp);
 extern void *mymalloc(size_t);
 extern real mypow(real,real);
 extern void *myralloc(void *, size_t);
 extern void *new_mblk_ASL(ASL*, uint k);
 extern int nl_obj_ASL(ASL*,int);
 extern fint nqpcheck_ASL(ASL*, int co, fint **rowqp, fint **colqp, real **delsqp);
 extern ssize_t nqpcheckZ_ASL(ASL*, int co, fint **rowqp, size_t **colqp, real **delsqp);
 extern char *obj_name_ASL(ASL*,int);
 extern int obj_prec(void);
 extern void obj_adj_ASL(ASL*);
 extern void obj_adj_xy_ASL(ASL *asl, real *x, real *x0, real *y);
 extern real objconst_ASL(ASL*,int);
 extern void objgrd_(fint *N, real *X, fint *NOBJ, real *G, fint *nerror);
 extern real objval_(fint *N, real *X, fint *NOBJ, fint *nerror);
 extern char optype[];
 extern int pfg_read_ASL(ASL*, FILE*, int flags);
 extern int pfgh_read_ASL(ASL*, FILE*, int flags);
 extern char *pr_unknown(FILE*, char*);
 extern int prob_adj_ASL(ASL*);
 extern fint qpcheck_ASL(ASL*, fint **rowqp, fint **colqp, real **delsqp);
 extern ssize_t qpcheckZ_ASL(ASL*, fint **rowqp, size_t **colqp, real **delsqp);
 extern char *read_line(EdRead*);
 extern void report_where_ASL(ASL*);
 extern char *read_sol_ASL(ASL*, real**xp, real **yp);
 extern void repwhere_ASL(EvalWorkspace*, int jv); /* jv = 1 == function */
							/* 2 == gradient */
							/* 3 == Hessian */
 extern void scream(EdRead*, int rc, const char *fmt, ...);
 extern ASL *set_cur_ASL(ASL*);	/* returns previous value */
 extern real set_randseed_ASL(real nseed);	/* returns new seed, usually nseed, but */
						/* automaticaally chosen if nseed == 0. */
 extern void show_funcs_ASL(ASL*);
 extern void sigcatch_ASL(void);
 extern void* sos_add_ASL(ASL*, FILE*, int);
 extern int sos_finish_ASL(ASL*,void**,int,int*,int**,int*,int**,int**,real**);
 extern void sphes_ew_ASL(EvalWorkspace*, SputInfo**, real *H, int nobj, real *ow, real *y);
 extern fint sphsetup_ew_ASL(EvalWorkspace*, SputInfo**, int nobj, int ow, int y, int uptri);
 extern void student_check_ASL(ASL*);
 extern void suf_declare_ASL(ASL*, SufDecl*, int);
 extern SufDesc *suf_get_ASL(ASL*, const char*, int);
 extern SufDesc *suf_iput_ASL(ASL*, const char*, int, int*);
 extern SufDesc *suf_rput_ASL(ASL*, const char*, int, real*);
 extern int suf_sos_ASL(ASL*,int,int*,char**,int**,int*,int**,int**,real**);
 extern char *var_name_ASL(ASL*,int);
 extern char *var_name_nomap_ASL(ASL*,int,int*);
 extern void varscale_ASL(ASL*, int, real, fint*);
 extern void what_prog(void);
 extern void write_sol_ASL(ASL*, const char *msg, double *x, double *y, Option_Info*);
 extern int  write_solf_ASL(ASL*, const char *msg, double *x, double *y, Option_Info *,
			const char*);
 extern int  write_solfx_ASL(ASL*, const char *msg, double *x, double *y, Option_Info *,
			Fwrite, Fwrite, Fwrite, const char*);
 extern void wrsolw_(char *msg, fint *nmsg, real *x, real *y, fint *wantsol,
			ftnlen msg_len);
 extern void wrtsol_(char *msg, fint *nmsg, real *x, real *y,
			ftnlen msg_len);
 extern real xectim_(void);
 extern void xknown_ASL(EvalWorkspace*, real*, fint*);
 extern void xknowe_(real *x, fint *nerror);
 extern void xknown_(real *x);
 extern void xunkno_(void);
 extern void xunknown_ASL(EvalWorkspace*);
 extern void zero_div_ASL(EvalWorkspace*, real, const char*);

#ifndef No_dtoa
 extern double strtod_ASL(const char*, char**);
#ifndef strtod	/* if not set by previous funcadd.h */
#define strtod strtod_ASL
#endif
extern void ACQUIRE_DTOA_LOCK(unsigned int);
extern void FREE_DTOA_LOCK(unsigned int);
extern int dtoa_get_threadno(void);
extern void init_dtoa_locks(void);
extern void set_max_dtoa_threads(unsigned int);
#endif

#ifdef __cplusplus
	}
#endif

#define qp_read_ASL fg_read_ASL
#define con_name(n) con_name_ASL((ASL*)asl,n)
#define conadj(cv) conadj_ASL((ASL*)asl,cv,1)
#define conscale(i,s,ie) conscale_ASL((ASL*)asl,i,s,ie)
#define dense_j() dense_j_ASL((ASL*)asl)
#define edagread(f) fg_read_ASL((ASL*)asl,f,0)	/* dreg */
#define equ_adjust(x,y) equ_adjust_ASL((ASL*)asl,x,y)
#define f_read(a,b) f_read_ASL((ASL*)asl,a,b)
#define fg_read(a,b) fg_read_ASL((ASL*)asl,a,b)
#define fg_wread(a,b) fg_wread_ASL((ASL*)asl,a,b)
#define fg_write(a,b,c) fg_write_ASL((ASL*)asl,a,b,c)
#define fgh_read(a,b) fgh_read_ASL((ASL*)asl,a,b)
#define fread_soln(f,x,y) fread_sol_ASL((ASL*)asl,f,x,y)
#define gen_rownos() gen_rownos_ASL((ASL*)asl)
#undef getenv
#define getenv getenv_ASL
#define int_catch(f,v) intcatch_ASL((ASL*)asl,f,v)
#define jac0dim(stub,len) jac0dim_ASL((ASL*)asl,stub,len)
#define jac1dim(s,m,n,no,nz,mxr,mxc,L) jac1dim_ASL((ASL*)asl,s,m,n,no,nz,mxr,mxc,L)
#define jacdim(stub,M,N,NO,NZ,MXR,MXC,len) jac_dim_ASL((ASL*)asl,stub,M,N,NO,NZ,MXR,MXC,len)
#define jacdim0(stub,len) jac0dim_ASL((ASL*)asl,stub,len)
#define jacpdim(s,m,n,no,nz,mxr,mxc,L) jacpdim_ASL((ASL*)asl,s,m,n,no,nz,mxr,mxc,L)
#define lagscale(s,ie) lagscale_ASL((ASL*)asl,s,ie)
#define lcon_name(n) lcon_name_ASL((ASL*)asl,n)
#define mip_pri(a,b,c,d) mip_pri_ASL((ASL*)asl,a,b,c,d)
#define mqpcheck(a,b,c,d) mqpcheck_ASL((ASL*)asl,a,b,c,d)
#define mqpcheckv(a,b,c) mqpcheckv_ASL((ASL*)asl,a,b,c)
#define mqpcheckv_free(a) mqpcheckv_free_ASL((ASL*)asl,a)
#define nl_obj(n) nl_obj_ASL((ASL*)asl,n)
#define nqpcheck(a,b,c,d) nqpcheck_ASL((ASL*)asl,a,b,c,d)
#define obj_name(n) obj_name_ASL((ASL*)asl,n)
#define objconst(n) objconst_ASL((ASL*)asl,n)
#define pfg_read(a,b) pfg_read_ASL((ASL*)asl,a,b)
#define pfgh_read(a,b) pfgh_read_ASL((ASL*)asl,a,b)
#define qp_opify() /* nothing */
#define qp_opify_ASL() /* nothing */
#define qp_read(a,b) qp_read_ASL((ASL*)asl,a,b)
#define qpcheck(a,b,c) qpcheck_ASL((ASL*)asl,a,b,c)
#define read_soln(x,y) read_sol_ASL((ASL*)asl,x,y)
#define show_funcs() show_funcs_ASL((ASL*)asl)
#define sos_add(a,b) sos_add_ASL((ASL*)asl,a,b)
#define sos_finish(a,b,c,d,e,f,g,h) sos_finish_ASL((ASL*)asl,a,b,c,d,e,f,g,h)
#define suf_declare(x,n) suf_declare_ASL((ASL*)asl,x,(int)(n))
#define suf_get(s,i) suf_get_ASL((ASL*)asl,s,i)
#define suf_iput(n,i,x) suf_iput_ASL((ASL*)asl,n,i,x)
#define suf_rput(n,i,x) suf_rput_ASL((ASL*)asl,n,i,x)
#define suf_sos(a,b,c,d,e,f,g,h) suf_sos_ASL((ASL*)asl,a,b,c,d,e,f,g,h)
#define var_name(n) var_name_ASL((ASL*)asl,n)
#define varscale(i,s,ie) varscale_ASL((ASL*)asl,i,s,ie)
#define write_sol(m,x,y,oi) write_sol_ASL((ASL*)asl,m,x,y,oi)
#define write_soln(m,x,y) write_sol_ASL((ASL*)asl,m,x,y,0)
#define del_mblk(a) Del_mblk_ASL((ASL*)asl,a)
#define new_mblk(n) new_mblk_ASL((ASL*)asl,n)

#define exit mainexit_ASL

#ifdef MULTIPLE_THREADS /*{{*/
#ifndef MEM_LOCK
#define MEM_LOCK 2
#endif
#ifndef MEM1_LOCK
#define MEM1_LOCK 3
#endif
#else	/*}{ MULTIPLE_THREADS */
#define ACQUIRE_DTOA_LOCK(n)	/*nothing*/
#define FREE_DTOA_LOCK(n)	/*nothing*/
#endif	/*}} MULTIPLE_THREADS */

#ifdef X64_bit_pointers
#define Opalign(x) case x: ++o;
#define alignarg(x) x
#else
#define Opalign(x) /*nothing*/
#define alignarg(x) /*nothing*/
#endif

#define ASL_CHECK(a,n,w) if(!(a)||(a)->i.ASLtype!=n)badasl_ASL(a,n,w);cur_ASL=a;
#endif /* ASL_included */
