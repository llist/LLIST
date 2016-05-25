/*
** modul         : llist.h
** type          : 
** version       : 1.91
** author        : patrice
** last update   : 15/07/03      14:07:44
** current date  : 16/05/25      15:42:49
** rel/lev/br/sq : 1    91    0    0 
**
** source file   : /MSP/D/sources/logis/sccs/incl/s.llist.h
** SCCS_File     : s.llist.h 
**
*/

/*------------------------------------------------------------------------*/
/*  <LLIST.h>                                                             */
/*                                                                        */
/*  Copyright SCHOUMACHER Patrice 95-96-97-98-99-Y2K+Y2K1+Y2K2+Y2K3       */
/*                                                                        */
/*  V3.40                                         All Rights Reserved !   */
/*                                                                        */
/* www.llist.fr.st  ou  http://perso.libertysurf.fr/llist/index.htm       */
/* ou http://llist.chez.tiscali.fr/index.htm                              */
/*------------------------------------------------------------------------*/

/**--- #define _PC_H_   ---**/

/*-------------------------------------*/
/* Pour Pc  et Borland Builder C++ V6  */
/*-------------------------------------*/
#ifdef _PC_H_

#define _MEM_PC_

#include<alloc.h>
#include<string.h>
#include<dos.h>
#include<stdio.h>
#include<conio.h>
#include<process.h>
#include<stdarg.h>
#include<stdlib.h>
#include<fcntl.h>
#include<io.h>
#include<sys\stat.h>

#else
/*------------*/
/* Pour UNIX  */
/*------------*/

#ident   "@(#)llist.h   1.91   15/07/03   14:07:44"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#endif


#ifdef MSP_LINUX
  #define _MEM_PC_ 
#endif


#ifndef __LIST_T__
#define __LIST_T__

/*
-----------------------------------------------------------------------------
 Taille des types
-----------------------------------------------------------------------------
*/

#define   LCHAR    sizeof(char)
#define   LSHORT   sizeof(short)
#define   LINT     sizeof(int)
#define   LDOUBLE  sizeof(double)
#define   LFLOAT   sizeof(float)
#define   LLONG    sizeof(long)
#define   LPTR     sizeof(void *)



/*
-----------------------------------------------------------------------------
Codes retours des fonctions sur les llist
-----------------------------------------------------------------------------
*/

#define LOK               0        /* Operation OK            */
#define LNOK             -1        /*                  Not OK */
#define NPC              -2        /* Pas de pointeur courant */
#define NLD              -3        /* Pas de liste definie    */
#define LSTOP            -4        /* Arret du parcours       */
#define L_VIDE           -5        /* Pointeur liste vide     */
#define L_EXIST          -6        /* Element existe          */
#define L_NOFCT          -7        /* Pas de pointeur fct     */
#define L_TYPE           -8        /* Type inconnu            */
#define L_KEY            -9        /* Erreur clef complexe    */
#define L_IKN            -10       /* Erreur sur info interne */
#define D_NULL           -11       /* Element Null            */
#define L_IDT            -12       /* Les 2 listes sont ident.*/
#define L_PDN            -15       /* Pointeur data NULL=GRAVE*/
#define LDEL             -30       /* code retour ds fct walk_*/
				   /* supp element courant    */
#define L_GRAVE          -55       /* Erreur TRES TRES GRAVE  */

#define NIL            NULL

#define WFL               0
#define WLF		  1
#define VIDE              1


/*
-----------------------------------------------------------------------------
Structure d'une LLIST :
-----------------------------------------------------------------------------
*/
typedef struct taille_list
{
  void   *data;
  struct taille_list *suiv;
  struct taille_list *precd;
} LLIST;

extern  LLIST      *_PiPo_;
#define CLOSE_ALL  &_PiPo_



/*
-----------------------------------------------------------------------------
 PRIMITIVES  PRIMITIVES  PRIMITIVES  PRIMITIVES  PRIMITIVES  PRIMITIVES
-----------------------------------------------------------------------------
*/

#ifdef __cplusplus
extern "C" {
#endif

       int initlist(LLIST **,int,int);
       int close_list(LLIST **);
       int listborn(LLIST **);


       int delete_list(LLIST **);
       int lgrl(LLIST **);
       int lvide(LLIST **);
       int tete(LLIST **,void *);
       int queue(LLIST **,void *);
       int Lfound(LLIST **,void *);

       int walk_same(LLIST **,void *,int (*)(LLIST **,void *));
       int walk_sameD(LLIST **,void *,int (*)(void *));

       int walk_FLDL(LLIST **,int (*)(LLIST **,void *));
       int walk_LFDL(LLIST **,int (*)(LLIST **,void *));

       int walk_FLD(LLIST **,int (*)(void *));
       int walk_LFD(LLIST **,int (*)(void *));

       int walk_FL(LLIST **,int (*)(LLIST **));
       int walk_LF(LLIST **,int (*)(LLIST **));

       int lcount(LLIST **,void *);
       int lrcpy(LLIST **,LLIST **);
       int lrcat(LLIST **,LLIST **);
       int lwrcpy(LLIST **,LLIST **,int (*)(LLIST **,LLIST **,void *));
       int lwrcat(LLIST **,LLIST **,int (*)(LLIST **,LLIST **,void *));
       int lcmpD(LLIST **,LLIST **);
       int lcmp(LLIST **,LLIST **);
       int lcmpK(LLIST **,LLIST **);
       int lmemcmp(LLIST **,LLIST **);

       int lunionlw(LLIST **,LLIST **,LLIST **,int (*)(void *));
       int lxorlw(LLIST **,LLIST **,LLIST **,int (*)(void *));
       int linterlw(LLIST **,LLIST **,LLIST **,int (*)(void *));
       int ldifflw(LLIST **,LLIST **,LLIST **,int (*)(void *));

       int SetlenKey(LLIST **,int);
       int GetlenKey(LLIST **);
       int Getlendata(LLIST **);
       int SetBKey(LLIST **,int );
       int GetBKey(LLIST **);
       int UnSetBKey(LLIST **);

       int luadd(LLIST **,void *,int);
       int lajout(LLIST **,void *);
       int luajout(LLIST **,void *);

       int lajoutc(LLIST **,void *);
       int luajoutc(LLIST **,void *);

       int lsuppw(LLIST **,void *);
       int lsupp(LLIST **,void *);
       int adjq(LLIST **,void *);
       int adjt(LLIST **,void *);
       int lad_round(LLIST **,void *);

       int corps(LLIST **);
       int corpi(LLIST **);
       int invl(LLIST **);
       int lsort(LLIST **);

       int ltri(LLIST **,int (*)(void *,void *));
       int lkey(LLIST **,void *,int,const char *,...);
       int ladd_type(char *,char ,int );
       int lsetincrease(LLIST **);
       int lsetdecrease(LLIST **);
       int lgetsense(LLIST **);

       int Cfirst(LLIST **);
       int Clast(LLIST **);
       int Cnext(LLIST **);
       int Cprev(LLIST **);
       int Cdel(LLIST **);
       int Crand(LLIST **,void *);
       int Celmt(LLIST **,void *);
       int Cchange(LLIST **,void *);
       int Cinsl(LLIST **,void *);
       int Cinsr(LLIST **,void *);
       int Cend(LLIST **);

       int GFIRSTWC(LLIST **,void *,int ,int);
       int GLASTWC(LLIST **,void *,int ,int);

       void *PGFIRSTWC(LLIST **,void *,int ,int);
       void *PGLASTWC(LLIST **,void *,int ,int);


       int Rnext(LLIST **,void *);
       int Rprev(LLIST **,void *);
       int Rrand(LLIST **,void *);
       int Rfirst(LLIST **,void *);
       int Rlast(LLIST **,void *);


       int Rnextw(LLIST **,void *);

       int Rprevw(LLIST **,void *);

       int Bnext(LLIST **);
       int Bprev(LLIST **);

       void *Pelmt(LLIST **);
       void *Plast(LLIST **);
       void *Pfirst(LLIST **);

       void *PCrand(LLIST **,void *);
       void *PCfirst(LLIST **);
       void *PClast(LLIST **);
       void *PCNext(LLIST **);
       void *PCPrev(LLIST **);

       void *Pdata_exist(LLIST **,void *);


       int  list_dbg(LLIST **,int);
       int  ldata_debug(LLIST **,int,int (*)(LLIST **));
       int  ldata_debugD(LLIST **,int,int (*)(void *));
       int  lkernel_debug(int);
       int  lkey_debug(LLIST**);
       long lget_mem_alloc(void);
       long lget_mem_free(void);
       void lset_trace_mem(int);
       int  lget_trace_mem(void);

       int  FtoLandLtoF(LLIST **,char *,int ,int );

//       int  LunsetCursor(LLIST **); sauf si necessaire 
       int  GCfirst(LLIST **,void *);
       int  GClast(LLIST **,void *);
      
       int lmvctof(LLIST **); // move current to first
       int lmvctol(LLIST **); //              to last 


       // Fcts dediées a foreach ------------------------------
       int ForeachListTest(LLIST **,void *,int);
       void *ForeachListTestp(LLIST **,int);
       int ForeachListTestw(LLIST **,void *);
       void *ForeachListTestwp(LLIST **,void *);

       int FCprev(LLIST **); 
       int FCnext(LLIST **); 
       int FCnextw(LLIST **,void *);
       int FCprevw(LLIST **,void *);
       //-------------------------------------------------------

#ifdef __cplusplus
}
#endif

/*
------------------------------------------------------------------------------
REDECLARATION de NOMS de FONCTIONS
------------------------------------------------------------------------------
*/
// Pouquoi je n'y ai pas pensé avant ????
// 22/02/2014  Merci jansson 
#define llist_foreach(L,d)  for( GCfirst(L,d)   ; ForeachListTest(L,d,WFL)==LOK     ;FCnext(L))
#define llist_foreachp(L,d) for( (d=PCfirst(L)) ; (d=ForeachListTestp(L,WFL))!=NULL ;FCnext(L))

#define llist_iforeach(L,d)  for( GClast(L,d)   ; ForeachListTest(L,d,WLF)==LOK     ;FCprev(L))
#define llist_iforeachp(L,d) for( (d=PClast(L)) ; (d=ForeachListTestp(L,WLF))!=NULL ;FCprev(L))


#define llist_foreachw(L,d)     for( GFIRSTWC(L,d,LOK,LOK)  ; ForeachListTestw(L,d)==LOK         ;FCnextw(L,d))
#define llist_foreachpw(L,d,p)  for( GFIRSTWC(L,d,LOK,LOK)  ; (p=ForeachListTestwp(L,d)) !=NULL  ;FCnextw(L,d))

#define llist_iforeachw(L,d)     for( GLASTWC(L,d,LOK,LOK)  ; ForeachListTestw(L,d)==LOK         ;FCprevw(L,d))
#define llist_iforeachpw(L,d,p)  for( GLASTWC(L,d,LOK,LOK)  ; (p=ForeachListTestwp(L,d)) !=NULL  ;FCprevw(L,d))



//
// LLIST *Li; // list d'entier
// int i;
//
// llist_foreach(&L,&i)
// {
//   fprintf(stderr,"\n i=%d",i);
// }
//
//int *pi; // avec un pointeur
//llist_foreachp(&L,pi)
//{
//   fprintf(stderr,"\n i=%d",*pi);
//}
//
//


#define Pushllist(l,p,c)      FtoLandLtoF(l,p,0,c)
#define Popllist(l,p,c)       FtoLandLtoF(l,p,1,c)
#define Gfirst(L,e)           tete(L,e)
#define Glast(L,e)            queue(L,e)
#define Prand(L,e)            Pdata_exist(L,e)
#define Ldata_exist(L,e)      Lfound(L,e)
#define ldiffl(l1,l2,l3)      ldifflw(l1,l2,l3,NULL)
#define linterl(l1,l2,l3)     linterlw(l1,l2,l3,NULL)
#define lxorl(l1,l2,l3)       lxorlw(l1,l2,l3,NULL)
#define lunionl(l1,l2,l3)     lunionlw(l1,l2,l3,NULL)
#define lappend(l1,e)         luadd(l1,e,0)
#define lahead(l1,e)          luadd(l1,e,1)

#define Gfirstw(l,e)          GFIRSTWC(l, e ,LNOK,LNOK)
#define Gfirstwc(l,e)         GFIRSTWC(l, e ,LOK,LNOK)
#define Glastw(l,e)           GLASTWC( l, e ,LNOK,LNOK)
#define Glastwc(l,e)          GLASTWC( l, e ,LOK,LNOK)


#define PGfirstw(l,e)         PGFIRSTWC(l, e ,LNOK,LNOK)
#define PGfirstwc(l,e)        PGFIRSTWC(l, e ,LOK,LNOK)
#define PGlastw(l,e)          PGLASTWC( l, e ,LNOK,LNOK)
#define PGlastwc(l,e)         PGLASTWC( l, e ,LOK,LNOK)


#endif
