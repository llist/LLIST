/*
** modul         : llist.c
** type          : 
** version       : 1.59
** author        : Patrice
** last update   : 15/04/09      17:41:17
** current date  : 16/05/25      15:42:53
** rel/lev/br/sq : 1    59    0    0 
**
** source file   : /MSP/D/sources/logis/sccs/Alib/s.llist.c
** SCCS_File     : s.llist.c 
**
*/

#ident  "@(#)llist.c   1.59   15/04/09   17:41:17"



/****---- #define _PC_ ----****/


#ifndef _PC_
#ident  "@(#)llist.c   1.59   15/04/09   17:41:17"
#endif

/***********************************************
!!COMPILE
llist.c
-c
!!END
***********************************************/

/**
NOMLIB: libw_lib.a
**/

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  Cpwght :  SCHOUMACHER Patrice 95-96-97-98-99-2000-2001-2002-2003-2011 */
/*                                                                        */
/*                                                  All Rights Reserved ! */
/*                                                                        */
/*  www.llist.fr                                                          */
/*  http://perso.libertysurf.fr/llist/index.htm                           */
/*                                                                        */
/*------------------------------------------------------------------------*/
/* V3.5   fin du PCmemcmp => mise en place comparaison typee              */
/*        ajout du signe +/- dans lkey permet d'avoir des cles avec tri   */
/*        different ex: 1er cle croissant 2eme decroissant                */
/*									  */
/*									  */
/* V3.41  walk_mem + kill_all + freewalk pour tjrs avoir la taille de     */
/*        l'element pour le free + suivi du compteur free                 */
/*									  */
/*									  */
/* V3.40 Modif delete_list pour derniere iteration close_all              */
/*       Modif free_mem  Ordre de liberation et du memeset !!             */
/*       Creation fct PCmemcmp pour Version sur PC BC++ V6                */
/*       Ajout fct    PCmemcmp pour datacompare avec flag _MEM_PC_        */
/*       Ptit optimisation fct lad_round pour Mimi                        */
/*                                                                        */
/* V3.35 Repositionnement du curseur pour lsuppw lsupp corps corpi        */
/* V3.34 Modif lkey %d => int et plus un double le double = %g            */
/*       rajout du %f float !!                                            */
/* V3.33 Correction Bug Cdel ( Merci Thierry D.)                          */
/* V3.32 Ajout Fct walk_sameD                                             */
/* V3.31 Ajout fct PCfirst PClast                                         */
/*                                                                        */
/* V3.3.1Ajout Fct walk_same et remplacement dans Rnext Rrand Rprev       */
/*       de Crand par Lrand(p,elmt,LOK)                                   */
/*									  */
/* V3.3  Ajout Fct se sauvegarde/relecture d'une liste dans un fichier    */
/* V3.2.1 Correction du UnSetBkey ( test si une Bkey a deja ete declare   */
/*									  */
/* V3.2  Ajout fct walk_FLDL walk_LFDL                                    */
/* V3.1  Ajout fct walk_FLD  walk_LFD                                     */
/* V3.0  Ajout du test LEQUAL dans la fct lajoute                         */
/* V2.9  modif sur fct ensemble dans le cas des fct sans le where         */
/*       remplace l'appel fct lrcpy par un walk_list pour pouvoir         */
/*       faire le where de la recopie + dans fct ensemble la fct          */
/*       verifsame en premier + chgment du code de retour du test         */
/*       si une des listes est vide.                                      */
/* V2.8  ajout fct TestListelmt sur plusieurs fct ajout fct Lverifsame    */
/*       test si les 2 liste identiques .                                 */
/* V2.7  Ajout a foundL du choix clef complexe. supp de la fct dataexist  */
/*       modif fct lcmp , ajout fct lcmpK modif fct afficheK ajout +      */
/*       ajout fct Prand() + supp et remplacement de Glast Gfist par tete */
/*       et queue. Modif de Bprev,Bnext,Rnextw et Rprevw sur le test de   */
/*       l == NIL || l->suiv == NIL => 2 if else.                         */
/*       Ajout fct SetCurK plus rapide dans certains cas que SetCurr      */
/* V2.6  Ajout LNOK a Crand si LVIDE.   			          */
/*       modif chargement tclkey[] +                                      */
/*       modif de lsupp -> ajout du corps en 1er Cas.                     */
/* V2.5  Ajout fct de fin.(My_exit).                                      */
/* V2.4  Mise a jour des recherches clef interne / compilo linux          */
/*       memset de (l) dans free_mem. modif Getlist.. SetBKey..           */
/* V2.3  Teste + modif fct de delete clef complexe + suppression veriflkey*/
/* V2.2  Modif pour avoir + clefs complexe em meme temps.                 */
/* V2.1  Mise a jour fct ensembles avec test si lgrl = 0                  */
/* V2.0  Fonction de tri automatique suivant clef.                        */
/* V1.9  Ajout fonction Rnextw + Rprevw.                                  */
/* V1.8  Maj fct datacompare.                                             */
/* V1.7  Fonctions sur les ensembles avec clef complexe.                  */
/* V1.6  Version avec gestion clefs complexe.                             */
/* V1.5  Version avec fct sur les ensembles + var de comptage memoire.    */
/* V1.3-4Version optimisation de + fct et des codes retour.               */
/* V1.2  Version avec fct debugs.                                         */
/* V1.1  Version avec fct curseur + fct pour gestion ROLL.                */
/* V1.0	 Version CNAM IUT STRASBOURG !!                                   */
/*									  */
/*									  */
/* -AMEL- : Nom pour la liste ( => pour le debug )                        */
/*          Mettre un code de blocage de fct pour certaines actions :     */
/*          ex: Cdel dans le walk.                                        */
/*          Mettre un type pour le initlist et le initpile => close_all.  */
/*          Optimisation en interne sur le controle de la liste.          */
/*          Declaration d'une liste avec un nom.                          */
/*          Mettre un header sur la liste en plus du kernel.              */
/*									  */
/*          Kernel       +-+   +-+                                        */
/*             |         +-+   +-+                                        */
/*        L1-> +---+->###   ###    etc                                    */
/*             |   |                                                      */
/*             +---+                                                      */
/*             |                                                          */
/*        L2-> +---+                                                      */
/*             |   |                                                      */
/*             +---+                                                      */
/*             |                                                          */
/*        Ln-> +---+                                                      */
/*             |   |                                                      */
/*             +---+                                                      */
/*                                                                        */
/*									  */
/*									  */
/* -------------------                                                    */
/* - Version : 3.40 ] -                                          WbyWM.   */
/*------------------------------------------------------------------------*/


#ifdef _PC_
#include "llist.h"
#define SORTIE    stdout
#else
#include <llist.h>
#define SORTIE    stderr
#endif


/*
-------------------------------------------------------------------------
-------------------------------------------------------------------------
-		 STRUCTURE INTERNE D'UNE CLEF COMPLEXE                  -
-------------------------------------------------------------------------
-------------------------------------------------------------------------
*/

#define _CHAR   'c'
#define _SHORT  'h'
#define _INT    'i'
#define _INTD   'd'
#define _LONG   'l'
#define _DOUBLE 'g'
#define _FLOAT  'f'
#define _STRING 's'


#define MAXNAME 12

#define   _WLK_   ((ret == LOK) || (ret == LDEL))
#define   _SK_    if( (ret == LDEL) && (save == sk) ) sk = (LLIST *)NULL
#define   _LDEL_  if( ret == LDEL) ret = Cdel(p)

/*
-------------------------------------------------------------------------
- Info sur les types de la clef.
-------------------------------------------------------------------------
*/
typedef struct
{
  char  typ;   /* clef de recherche */
  char  name[MAXNAME];
  int   off;
  int   len;
  int   kSense;
  
} CLKEY;      /* complexe list key */

#define LCLKEY  sizeof(CLKEY)

/*
-------------------------------------------------------------------------
- Les types minimums de clef.
-------------------------------------------------------------------------
*/
CLKEY tclkey[]={
		{ _CHAR   , "-  CHAR  -"  , 0 , sizeof(char)  ,0  },
		{ _SHORT  , "- SHORT  -"  , 0 , sizeof(short) ,0  },
		{ _INT    , "-   INT  -"  , 0 , sizeof(int)   ,0  },
		{ _INTD   , "-   INT  -"  , 0 , sizeof(int)   ,0  },
		{ _LONG   , "-  LONG  -"  , 0 , sizeof(long)  ,0  },
		{ _DOUBLE , "- DOUBLE -"  , 0 , sizeof(double),0  },
		{ _FLOAT  , "- FLOAT  -"  , 0 , sizeof(float) ,0  },
		{ _STRING , "- STRING -"  , 0 , sizeof(char)  ,0  },
		{  '\0'   , "\0"          , 0 , 0             ,0  }
	       };

/*
-------------------------------------------------------------------------
- Pour le liste de liste de clef.
-------------------------------------------------------------------------
*/
typedef struct
{
  int    num;
  LLIST *clef;

} IKL;        /* INTERN KEY LIST */


#define LIKL      sizeof(IKL)
#define LIKLCLEF  sizeof(int)

/*
--------------------------------------------------------------------------
		  STRUCTURE INTERNE DE LA LISTE
--------------------------------------------------------------------------
*/

typedef struct
{
    void *adr;      /* adresse du pointeur (LIST *).  */
    void *first;    /* adresse du 1er NODE.           */
    void *last;     /* adresse du dernier NODE.       */
    void *current;  /* adresse du NODE courant.       */

    int  lgr_data;  /* Taille en octet de l'element.  */
    int  lgr_key;   /* Longueur pour la recherche.    */
    int  nbre_elmt; /* Nombre d'element de la liste.  */
    int  bkey;      /* Booleen sur utilisation lkey.  */
    int  kv;        /* numero clef valide.            */
    int  sense;     /* liste croissante/decroissante. */

    LLIST *lkey;    /* liste de liste de clef.        */

} LKERNEL;

static int  __flgefirst__=LNOK;    /* Flag de passage dans une fonction. */
static int  _cptl_;                /* Compteur interne.                  */
static long _nb_calloc_;           /* nombre d'octet  alloue             */
static long _nbfree_;              /* nombre d'octet  libere             */
static int  _Ltrace_Mem_=LNOK;     /* flag comptage octet /allou/desal   */


static LLIST *_KerneL=NULL;        /* Liste interne.                     */
static LLIST *_LtypE_=NULL;        /* Liste de type de variable.         */


#define lgr_LK   sizeof(LKERNEL)   /* taille de la structure KERNEL      */
#define lgr_L    sizeof(LLIST)     /* Taille de la stucture LLIST        */

#define LSMALL  -100               /* Pour le trie                       */
#define LBIG    +100               /*                                    */
#define LEQUAL     0               /*                                    */

LLIST    *_PiPo_;                   /* Liste pipo pour la suppression de */
				    /* toutes les listes declarees !!!!  */

static  LKERNEL *LLIST_exist(LLIST **);


static  void setK(LKERNEL *K,LLIST **,LLIST *,LLIST *,LLIST *,int,int,int);
static  void listsort(LLIST * ,LLIST * ,int (*fct)(void *,void *));
static	void listsortk(LLIST **,LKERNEL *,LLIST *,LLIST *);
static  void exchange_data(LLIST *,LLIST *);
static  void SetCurK(LKERNEL *,LLIST *);
static  void afficheK(LKERNEL *,const char *);
static  void *allocation(int );
static  void first_call(void );
static  void initK(LKERNEL *);
static  void exchg(LLIST *);
static  void My_exit(void);

static  int lrecopy(LLIST **,LLIST **,int,int (*fct)(LLIST **,LLIST **,void *));
static  int walk_list(LLIST **,LLIST **,LLIST **,int ,int ,int,int,int (*)(void *));
static  int datacompare(LLIST **,LKERNEL *,void *,void *);

static  int LunsetCursor(LLIST **);
//#ifdef _MEM_PC_
//static  int PCmemcmp(const void *,const void *,size_t );
//#endif
static  int TestListelmt(LKERNEL **,LLIST **,void *);
static  int walk_mem(LLIST **,int (*)(LLIST **,LKERNEL *));
static  int calcul_offset(void *,void *,int);
static  int lcompare(LLIST **,LLIST **,int);
static  int TestList(LKERNEL **,LLIST **);
static  int Lverifsame(LLIST **,LLIST **);
static  int cherche_type(CLKEY *,char *);
static  int lajoute(LLIST **,void *,int,int);
static  int Lrand(LLIST **,void *,int);
static  int pinf(LLIST *,LLIST *);
static  int free_mem(LLIST *,int);
static  int kill_allkey(LLIST **);
static  int afficheKEY(LLIST **);
static  int kill_all(LLIST **,LKERNEL *);
static  int freewalk(LLIST **,LKERNEL *);
static  int afficheL(LLIST **);
static  int Get_Length_Sense(CLKEY *,char **);
static  int affK(LLIST **);
static  int loadkey(void);

static LLIST **GetlistIKL(LKERNEL *);

static IKL *GetadrIKL(LLIST **,int );

/* Fontions pouvant etre declarees en extern pour l'utilisateur.  */
/* Pour developper des fonctions sur les listes MAIS SEULEMENT    */
/* en ayant une connaissance parfaite de la gestion de celle-ci !!*/

static int   lfoundpos(LLIST **,LKERNEL *,void *,int );

/***************************************************************/
/** Nouvelles fct de comparaison typée                         */


static int memcmp_by_typ(CLKEY *,const void *, const void *);

static int memSHORT(CLKEY *,const void *, const void *);
static int memINT(CLKEY *,const void *, const void *);

/********************************************/
/*  Existe pour un utilisation Futur        */
/*                                          */
/*                                          */
/*  static int   SetCurr(LLIST **,LLIST *); */
/*  static LLIST *Getfirst(LLIST **);       */
/*  static LLIST *Getlast(LLIST **);        */
/*                                          */
/********************************************/

static LLIST *foundL(LLIST **,void *,int);
static LLIST *GetCurr(LLIST **);
static LLIST *GetCurK(LKERNEL *);
static LLIST *Nnext(LLIST *);
static LLIST *Nprev(LLIST *);

/*-------------------------------------------------------------------------*/
/*------[ FONCTIONS SUR LES LISTES ]---------------------------------------*/
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

/*
-----------------------------------------------------------------------------
- LVIDE: fonction booleenne qui teste si une liste est vide (1) ou non (0)  -
-----------------------------------------------------------------------------
*/

int lvide(LLIST **p)
{  /* 1=vide  0=non vide */
    return( *p==NULL );
}

/*
-----------------------------------------------------------------------------
- LGRL: fonction qui renvoie le nombre d element de la liste                -
-----------------------------------------------------------------------------
*/
int lgrl(LLIST **p)
{

  LKERNEL *K;

  K = LLIST_exist(p);

  if(K != NULL)
    return((int)(K->nbre_elmt));

  return(NLD);
}


/*
-----------------------------------------------------------------------------
- QUEUE: fonction qui renvoie la valeur du dernier element de la liste      -
-----------------------------------------------------------------------------
*/
int  queue(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL *K;                        /* info sur la liste */

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
    if(K->last)
    {
      memcpy(elmt,((LLIST *)(K->last))->data,(size_t)K->lgr_data);
      return(LOK);
    }
   return(L_GRAVE);
  }
 return(ret);
}

/*
-----------------------------------------------------------------------------
- TETE: fonction qui renvoie la valeur du premier element de la liste       -
-----------------------------------------------------------------------------
*/
int tete(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL *K;                        /* info sur la liste */

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
    if( K->first )
    {
      memcpy(elmt,((LLIST *)(K->first))->data,(size_t)K->lgr_data);
      return(LOK);
    }
   return(L_GRAVE);
  }
 return(ret);
}


/*
****-----------------------------------------------------------------------------
- exchg: echange l'adresse du suivant avec l adresse du precedent.          -
****-----------------------------------------------------------------------------
*/
static void exchg(LLIST *l)
{
  LLIST *tmp;

       tmp = l->suiv;
  l->suiv  = l->precd;
  l->precd = tmp;

}

/*
-----------------------------------------------------------------------------
- INVL: fonction qui permet d'inverser une liste                            -
-----------------------------------------------------------------------------
*/
int invl(LLIST **p)
{
   LKERNEL *K;
   LLIST *save=*p;
   LLIST *l;

   int endl;

      K = LLIST_exist(p);

      if( K != NULL )
      {
	   if( K->nbre_elmt > 1 )
	   {
	       endl = lgrl(p);
	       while(endl--)
	       {
		     l = save;
		  save = save->suiv;
		  exchg(l);
	       }

		  save = (LLIST *)(K->first);     /* mise a jour du noyau */
	      K->first = K->last;
	      K->last  = (void *)save;

	      *p = ((LLIST*)(K->first));

	    return(LOK);
	  }
	return(LNOK);
      }
  return(NLD);
}

/*
-----------------------------------------------------------------------------
- ADJT: fonction qui permet d'ajouter un element en debut de liste          -
-----------------------------------------------------------------------------
*/
int  adjt(LLIST **p,void * elmt)
{
   LKERNEL  *K;  /* info sur la liste */
   LLIST  *news; /* new  node         */

  if( elmt == (void *)NULL )
     return(D_NULL);

   K = LLIST_exist(p);

   if(K != NULL)
   {
	 news = (LLIST *)allocation(lgr_L);     /* allocation de la LLISTE */
	 news->suiv = NULL;                    /* pas de suivant         */
	 news->precd = NULL;                   /* pas de precedant       */

	 news->data = (void *)allocation(K->lgr_data); /* allocation des data    */
	 memcpy(news->data,elmt,(size_t)K->lgr_data);          /* recopie des data       */

	if (*p == NULL)  /* Cas du 1er element dans la liste */
	{                /* && K->nbre_elmt = 0 */
		K->first     = news;
		K->last      = news;
		K->current   = NIL;
		K->nbre_elmt = 1;
	}
	else  /* rajout de l'element en 1ER */
	{
		news->suiv =  (LLIST *)(K->first);
		((LLIST *)(K->first))->precd=news;

		K->first   = news;
		K->nbre_elmt++;
	}

      *p = news;

      return(LOK);
   }
 return(NLD);
}


/*
-----------------------------------------------------------------------------
- ADJQ: fonction qui permet d'ajouter un element en fin de liste            -
-----------------------------------------------------------------------------
*/
int adjq(LLIST **p,void *elmt)
{
   LKERNEL  *K;  /* info sur la liste */
   LLIST  *news; /* new  node         */

  if( elmt == (void *)NULL )
     return(D_NULL);

   K = LLIST_exist(p);

   if(K != NULL)
   {
	 news = (LLIST *)allocation(lgr_L);    /* allocation de la LLISTE */
	 news->suiv = NULL;                    /* pas de suivant         */
	 news->precd = NULL;                   /* pas de precedant       */

	 news->data = (void *)allocation(K->lgr_data); /* allocation des data    */
	 memcpy(news->data,elmt,(size_t)K->lgr_data);  /* recopie des data       */

	if (*p == NULL)  /* Cas du 1er element dans la liste */
	{                /* && K->nbre_elmt = 0 */
		K->first     = news;
		K->last      = news;
		K->current   = NIL;
		K->nbre_elmt = 1;
		*p = news;
	}
	else  /* rajout sur le dernier */
	{
		((LLIST *)(K->last))->suiv=news;   /* ajout du suivant     */
		news->precd = (LLIST *)(K->last);  /* adresse du precedant */

		K->last      = news;
		K->nbre_elmt++;
	}
      return(LOK);
   }
  return(NLD);
}


/*
-----------------------------------------------------------------------------
- CORPI: fonction renvoyant la ss-liste composee des n-1 premier elements   -
-----------------------------------------------------------------------------
*/
int corpi(LLIST **p)
{
   LKERNEL    *K;   /* info sur la liste */
   LLIST  *precd;
   LLIST   *last;
   int ret;

     ret = TestList(&K,p);

     if(ret == LOK)
     {

	   if(K->nbre_elmt == 1)
	   {
	     last = (LLIST*)(K->first);             /* adresse du dernier = premier */
	     initK(K);
	     *p= NIL;
	   }
	   else
	   {
	     last    = (LLIST*)(K->last);           /* adresse du dernier                      */
	     precd   = ((LLIST*)(K->last))->precd;  /* adresse du precedant                    */

	     if( K->last == K->current )            /* si le curseur = dernier element         */
		 K->current = precd;                /* alors = precedent  qui est le courant   */


	     precd->suiv = NULL;                    /* le suivant du precedant = RIEN          */

	     K->last = precd;                       /* noyau a jour                            */
	     K->nbre_elmt--;
	   }
       free_mem(last,K->lgr_data);
       return(LOK);
     }
 return(ret);
}


/*
-----------------------------------------------------------------------------
- CORPS: fonction renvoyant la ss-liste composee des n-1 derniers elements  -
-----------------------------------------------------------------------------
*/
int corps(LLIST **p)
{
   LKERNEL  *K;                        /* info sur la liste */
   LLIST  *suiv;
   LLIST  *first;
   int ret;

     ret = TestList(&K,p);

     if( ret == LOK )
     {

	   if(K->nbre_elmt == 1)
	   {
	     first = (LLIST*)(K->first); /* adresse du dernier = premier */
	     initK(K);
	     *p = NIL;
	   }
	   else
	   {
	     first  = (LLIST*)(K->first);            /* adresse du premier                         */
	     suiv   = ((LLIST*)(K->first))->suiv;    /* adresse du suivant                         */
	     if( K->first == K->current  )           /* si le curseur = 1 element                  */
		K->current = suiv;                   /* alors = suivant  qui est le 1er maintenant */

	     suiv->precd = NULL;                     /* le precedant du suivant = RIEN */

	     K->first = suiv;                        /* noyau a jour */
	     K->nbre_elmt--;
	     *p = suiv;                              /* debut liste   */
	   }
	 free_mem(first,K->lgr_data);

	 return(LOK);
     }

  return(ret);
}


/*
****-----------------------------------------------------------------------------
- LLIST_exist: fonction renvoyant les infos sur la liste si celle-ci a ete   -
-             initialise par INITLLISTE                                      -
****-----------------------------------------------------------------------------
*/
static LKERNEL *LLIST_exist(LLIST **p)
{
  LLIST *save = _KerneL;
  int   last = LNOK;


  if(!lvide(&_KerneL))   /* si la liste interne n'est vide alors */
  {
     do {

	  if( ((void *)((LKERNEL *)(save->data))->adr) == (void *)(p) )
	    return((LKERNEL*)save->data);

	       if(save->suiv == NULL) /* permet de faire la derniere iteration */
		 last = LOK;
	       else
		 save = save->suiv;

	} while(last == LNOK);
  }

 return(NULL);
}


/*
****-----------------------------------------------------------------------------
- first_call: ne sert que pour l'initialisation de la liste interne a elle  -
-             meme !!!                      				    -
****-----------------------------------------------------------------------------
*/
static void first_call()     /* rajout manuel dans _KerneL de lui meme */
{
 LKERNEL *K;

  _cptl_     = 0;
  _nb_calloc_= 0;
  _nbfree_   = 0;

    _KerneL = (LLIST *)allocation(lgr_L);
	 K = (LKERNEL *)allocation(lgr_LK);

  setK(K,&_KerneL,_KerneL,_KerneL,NIL,lgr_LK,lgr_LK,1);

  _KerneL->data  = K;
  _KerneL->suiv  = NULL;
  _KerneL->precd = NULL;

  __flgefirst__   = LOK;

    atexit(My_exit);   /* fct appelee par exit() */

   if( loadkey() != LOK )
   {
     fprintf(SORTIE,"\n ***[ ERREUR CREATION LISTE DE TYPE INTERNE !]***");
     exit(0);
   }

}

/*
-----------------------------------------------------------------------------
- listborn: Permet de savoir si la liste a deja ete declare par initlist    -
-----------------------------------------------------------------------------
*/
int listborn(LLIST **p)
{
   LKERNEL *K;

   K = LLIST_exist(p);

   if( K == NULL)
     return(LNOK);

return(LOK);
}

/*
-----------------------------------------------------------------------------
- initlist: declaration de la liste => INDISPENSABLE . Taille = taille en   -
-           octet de l'element a gere.					    -
-----------------------------------------------------------------------------
*/
int initlist(LLIST **p,int taille,int key)
{

   LKERNEL _LK;
   LKERNEL *K;

  if(__flgefirst__ == LNOK)
       first_call();

  if(taille > 0 && key <= taille)
  {
    K = LLIST_exist(p);

    if( K == NULL)  /* rajout de la nouvelle liste */
    {
       setK(&_LK ,p,NIL,NIL,NIL,taille,key,0);

       adjq(&_KerneL,&_LK);
       *p = NIL;

     return(LOK);
    }

   return(L_EXIST);
  }

return(LNOK);
}

/*
****-----------------------------------------------------------------------------
- free_mem: liberation de la memoire.                                       -
****-----------------------------------------------------------------------------
*/
static int free_mem(LLIST *l,int eff)
{

  _cptl_++;
  
  if( (_Ltrace_Mem_ == LOK) && l && l->data ) /* efface le bloc avant liberation.  */
      memset((void *)l->data,0,(size_t)eff);

     _nbfree_ += (long)eff;


  free(l->data);  /* d'abord les donnees */

  if( (_Ltrace_Mem_ == LOK) && l )  /* efface le noeud avant liberation. */
      memset((void *)l,0,(size_t)lgr_L);

       _nbfree_ += (long)lgr_L;
  
  free(l);

return(LOK);
}



/*
****-----------------------------------------------------------------------------
- freewalk: liberation de la memoire.                                       -
****-----------------------------------------------------------------------------
*/
static int freewalk(LLIST **l,LKERNEL *K)
{
    if(K)
      return(free_mem((LLIST*)K->current,K->lgr_data));

 fprintf(stderr,"\n freewalk K=NULL");
 return(free_mem(GetCurr(l),0));
}

/*
****-------------------------------------------------------------------------
- walk_mem :parcours de la liste du dernier au 1er avec appel d'une fct.     -
****-------------------------------------------------------------------------
*/
static int walk_mem(LLIST **p,int (*fct1)(LLIST **,LKERNEL *K))
{
 LKERNEL *K;
 LLIST *save;
 int last = LNOK;
 int Mcpt=0;
 int ret;
 int Mlg=0;


    ret = TestList(&K,p);

    if(ret == LOK)
    {
	   Mlg = (int)(K->nbre_elmt);
	   save = (LLIST *)K->last;
	   SetCurK(K,save);
	  do {
		 if(save->precd == NULL) /* permet de faire la derniere iteration */
		 {
		   last = LOK;
		   ret = fct1(p,K);
		   Mcpt++;
		 }
		 else
		  {
		    save = save->precd;
		    ret = fct1(p,K);
		    Mcpt++;
		    SetCurK(K,save);
		  }
		if( Mcpt > Mlg)
		 {
		   fprintf(SORTIE,"\n *******Erreur Grave liste corrompu *******");
		   fprintf(SORTIE,"\n ******* Err LGR L ***fct=%d **Mlg=%d**",Mcpt,Mlg);
		   fflush(SORTIE);
		   exit(0);
		 }

	     } while(last == LNOK && ret == LOK);
	   return(Mcpt);
    }
   return(ret);
}



/*
-----------------------------------------------------------------------------
- delete_list : vide la liste.                                              -
-----------------------------------------------------------------------------
*/
int delete_list(LLIST **p)
{
  int ret;
  LKERNEL *K;
  int cpt=0;

  ret =  TestList(&K,p);

  if( ret == LOK )
  {
       cpt = walk_mem(p,freewalk);
       if(p != &_KerneL)  /* si kernel : bloc deja libere  */
	  initK(K);

       *p = NIL;  /* liste vide */

      return(cpt);
  }

 return(ret);
}

/*
-----------------------------------------------------------------------------
- allocation: Allocation de blocs memoire.
------------------------------------------------------------------------------
*/
static void *allocation(int taille)
{
  void  *pl=NULL;

  pl = (void *)calloc((size_t)1,(size_t)taille);
   
  if(!pl)
  {
    fprintf(SORTIE,"\n ***[ ERREUR ALLOCATION [%ld Octets] a [%d] demande. ]***",_nb_calloc_,_cptl_);
    fprintf(SORTIE,"\n ***         LIBERATION [%ld Octets]                    ",_nbfree_);

    if( _Ltrace_Mem_ == LOK )
         lkernel_debug(LOK);

    exit(0);
  }
  
 _nb_calloc_+=  taille;

return(pl);
}

/*
-----------------------------------------------------------------------------
- lkernel_debug: affiche les differents noeuds du kernel. 0=croissant 1=dec. -
-----------------------------------------------------------------------------
*/
int lkernel_debug(int sens)
{
 return(list_dbg(&_KerneL,sens));
}

/*
-----------------------------------------------------------------------------
- lkey_debug: affiche les differents type de cles.                          -
-----------------------------------------------------------------------------
*/
int lkey_debug(LLIST **p)
{
  LKERNEL *K;

  K = LLIST_exist(p);
  if(K)
   {
    afficheK(K,"** Debug Clef Complexe **");
    if( listborn(&K->lkey) == LOK )
	return(walk_FL(&K->lkey,affK));
    else
      fprintf(SORTIE,"\n ++++++++++++[ PAS DE LISTE INTERNE  !! ]++++++++++");
   }
  else
   {
    fprintf(SORTIE,"\n ++++++++++++[  LISTE INTERNE  !! ]++++++++++");
    fprintf(SORTIE,"\n +++++[ Info sur les types existants !!]+++++");
   }
   return(walk_FL(&_LtypE_,afficheKEY));
}


/*
****-------------------------------------------------------------------------
- affK: affiche les infos de la liste des clefs complexes.                  -
****-------------------------------------------------------------------------
*/
static int affK(LLIST **l)
{
 IKL *ikl=((IKL *)Pelmt(l));
 fprintf(SORTIE,"\n ++++[  CLEF N=%d  !! ]++++++++++++++++++++++++++++++++++",ikl->num);
 walk_FL(&ikl->clef,afficheKEY);
 return(LOK);
}
/*
-----------------------------------------------------------------------------
- list_dbg: affiche les differents noeuds de la liste. 0=croissant 1=dec. -
-----------------------------------------------------------------------------
*/
int list_dbg(LLIST **p,int sens)
{
  LKERNEL *K;

  K = LLIST_exist(p);

 if(K)
 {
       afficheK(K," ** Debug  LISTE  ** ");

     _cptl_ = 0;

     fprintf(SORTIE,"\n\n- LIST_DBG() ---------------------------");

     if(sens == WFL)
	return(walk_FL(p,afficheL));
     else
      {
	_cptl_ = 0 - lgrl(p);
	return(walk_LF(p,afficheL));
      }
  }

 fprintf(SORTIE,"\n\n- La Liste n'est pas Declaree !! --------------------");
 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- ldata_debugD: affiche les differents noeuds de la liste. 0=croissant 1=dec. -
-----------------------------------------------------------------------------
*/
int ldata_debugD(LLIST **p,int sens,int (*fct1)(void *))
{
 if(fct1)
 {
  _cptl_ = 0;

  if(sens == WFL)
     return(walk_FLD(p,fct1));
  else
     return(walk_LFD(p,fct1));
 }
return(L_NOFCT);
}

/*
-----------------------------------------------------------------------------
- ldata_debug: affiche les differents noeuds de la liste. 0=croissant 1=dec. -
-----------------------------------------------------------------------------
*/
int ldata_debug(LLIST **p,int sens,int (*fct1)(LLIST **))
{
 if(fct1)
 {
  _cptl_ = 0;

  if(sens == WFL)
     return(walk_FL(p,fct1));
  else
     return(walk_LF(p,fct1));
 }
return(L_NOFCT);
}


/*
****-----------------------------------------------------------------------------
- afficheL: affiche les noeuds de la liste. 				    -
****-----------------------------------------------------------------------------
*/
static int  afficheL(LLIST **l)     /* NODE */
{
  LLIST *save = GetCurr(l);

     fprintf(SORTIE,"\n----------------------------------------");
     fprintf(SORTIE,"\n *%6d*[%p]*** Data =[%p]  ****",abs(++_cptl_),save,save->data);
     fprintf(SORTIE,"\n ******************* Suiv =[%p]  ****",save->suiv);
     fprintf(SORTIE,"\n ******************* Precd=[%p]  ****",save->precd);

  if(l == &_KerneL)
  {   afficheK((LKERNEL *)save->data," ** Debug LKERNEL ** ");  }

return(LOK);
}

/*
****-----------------------------------------------------------------------------
- afficheK: affiche les infos du _KerneL.                                    -
****-----------------------------------------------------------------------------
*/
static void afficheK(LKERNEL *K,const char *s)       /* NODE */
{
 double Ko;
 fprintf(SORTIE,"\n ++[ INFO INTERNE ]+++++++++++[ %s ]+++++++++++++++++++++++++++++++++",s);
 fprintf(SORTIE,"\n + Adr Liste.......= %p   ",K->adr);
 fprintf(SORTIE,"\n +     Adr first...= [%p] ",K->first);
 fprintf(SORTIE,"\n +     Adr last....= [%p] ",K->last);
 fprintf(SORTIE,"\n +     Adr current.= [%p] ",K->current);
 fprintf(SORTIE,"\n +        lgr data = %d   ",K->lgr_data);
 fprintf(SORTIE,"\n +        lgr key  = %d   ",K->lgr_key);
 fprintf(SORTIE,"\n +       nbre_elmt = %d   ",K->nbre_elmt);
 Ko = (K->nbre_elmt*K->lgr_data);
 Ko /= 1024;
 fprintf(SORTIE,"\n +       Memory    = %.2lf Ko",Ko);

 fprintf(SORTIE,"\n + Adr LKEY .......= [%p] ",K->lkey);

 if(K->bkey == LOK )
  fprintf(SORTIE,"\n @ Clef cpx active = %d   ",K->bkey);
 else
  fprintf(SORTIE,"\n @ Clef cpx inacti.= %d   ",K->bkey);
 fprintf(SORTIE,"\n + num clef active = %d   ",K->kv);
 if(K->sense == LSMALL )
 fprintf(SORTIE,"\n @liste Croissante.= [%d] ",K->sense);
 else
 fprintf(SORTIE,"\n @liste Decroissant= [%d] ",K->sense);
 fprintf(SORTIE,"\n +++++++++++++++++++++++++++++++++++++++++++++++++++++");
}

/*
****-----------------------------------------------------------------------------
- afficheKEY: affiche les infos de la clef.                                    -
****-----------------------------------------------------------------------------
*/
static int afficheKEY(LLIST **l)
{
 CLKEY *k;

 k =(CLKEY *)Pelmt(l);
 if(k)
 {
  fprintf(SORTIE,"\n +++++++ CLEF COMPLEXE ++++++++++++");
  fprintf(SORTIE,"\n +      nom  = [%10.10s]",k->name);
  fprintf(SORTIE,"\n +      Type = [ %c ]   ",k->typ);
  fprintf(SORTIE,"\n +      len  = [ %3.3d ]",k->len);
  fprintf(SORTIE,"\n +      off  = [ %3.3d ]",k->off);
  if(k->kSense == LSMALL )
  fprintf(SORTIE,"\n +    Croiss = [%d]     ",k->kSense);
  else
  fprintf(SORTIE,"\n +    Decroi = [%d]     ",k->kSense);
 
  fprintf(SORTIE,"\n ++++++++++++++++++++++++++++++++++");
 return(LOK);
 }

 fprintf(SORTIE,"\n +++ Erreur Aff CLEF => Err Sys GRAVE !!+++");
 return(LNOK);

}


/*
-----------------------------------------------------------------------------
- walk_FLDL: parcours de la liste du 1er au dernier avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_FLDL(LLIST **p,int (*fct1)(LLIST **,void *))
{
 LKERNEL     *K;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt = 0;
 int        ret;

    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if ( ret == LOK)
    {
	  sk = GetCurK(K);                       /* sauve le curseur courant */
	  SetCurK(K,save);

	  do {
		 if(save && save->data)
		   ret = fct1(p,save->data);          /* ou (*fct1)() */
		 else
		   ret = L_PDN;

		 if( _WLK_ )
		 {
		   cpt++;

		   if(save->suiv == NULL)           /* permet de faire la derniere iteration */
		     last = LOK;
		   else
		    {
		     _SK_;
		     save = save->suiv;
		     _LDEL_;
		     SetCurK(K,save);
		    }
		 }

	     }while(last == LNOK && ret == LOK);

	 SetCurK(K,sk);                  /* restore le curseur */
	 return(cpt);                    /* retourne le nombre iteration effectuee */
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_FLD: parcours de la liste du 1er au dernier avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_FLD(LLIST **p,int (*fct1)(void *))
{
 LKERNEL     *K;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt = 0;
 int        ret;

    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if ( ret == LOK)
    {
	  sk = GetCurK(K);                       /* sauve le curseur courant */
	  SetCurK(K,save);

	  do {
		if(save && save->data)
		   ret = fct1(save->data);          /* ou (*fct1)() */
		else
		   ret = L_PDN;

	      if( _WLK_ )
	      {
		cpt++;

		if(save->suiv == NULL)           /* permet de faire la derniere iteration */
		  last = LOK;
		else
		 {
		  _SK_;
		  save = save->suiv;
		  _LDEL_;
		  SetCurK(K,save);
		 }
	      }
	     } while(last == LNOK && ret == LOK);

	 SetCurK(K,sk);                  /* restore le curseur */
	 return(cpt);                    /* retourne le nombre iteration effectuee */
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_FL: parcours de la liste du 1er au dernier avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_FL(LLIST **p,int (*fct1)(LLIST **))
{
 LKERNEL     *K;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt = 0;
 int        ret;

    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if ( ret == LOK)
    {
	  sk = GetCurK(K);               /* sauve le curseur courant */
	  SetCurK(K,save);

	  do {
		ret = fct1(p);           /* ou (*fct1)() */

	      if( _WLK_ )
	      {
		cpt++;

		if(save->suiv == NULL)   /* permet de faire la derniere iteration */
		  last = LOK;
		else
		 {
		  _SK_;
		  save = save->suiv;
		  _LDEL_;
		  SetCurK(K,save);
		 }
	      }
	     } while(last == LNOK && ret == LOK);

	 SetCurK(K,sk);                  /* restore le curseur */
	 return(cpt);                    /* retourne le nombre iteration effectuee */
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_LFDL: parcours de la liste du dernier au 1er avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_LFDL(LLIST **p,int (*fct1)(LLIST **,void *))
{
 LKERNEL *K;
 LLIST *save;
 LLIST *sk;
 int last = LNOK;
 int cpt=0;
 int ret;


    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if(ret == LOK)
    {
	   sk = GetCurK(K);  /* sauve le curseur courant */
	   save = (LLIST *)K->last;
	   SetCurK(K,save);
	  do {
		 if(save && save->data)
		    ret = fct1(p,save->data);  /* ou (*fct1)() */
		 else
		    ret = L_PDN;

		if( _WLK_ )
		{
		 cpt++;
		 if(save->precd == NULL) /* permet de faire la derniere iteration */
		   last = LOK;
		 else
		  {
		    _SK_;
		    save = save->precd;
		    _LDEL_;
		    SetCurK(K,save);
		  }
		}

	     } while(last == LNOK && ret == LOK);
	   SetCurK(K,sk);  /* restore le curseur */
	   return(cpt);
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_LFD: parcours de la liste du dernier au 1er avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_LFD(LLIST **p,int (*fct1)(void *))
{
 LKERNEL *K;
 LLIST *save;
 LLIST *sk;
 int last = LNOK;
 int cpt=0;
 int ret;


    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if(ret == LOK)
    {
	   sk = GetCurK(K);  /* sauve le curseur courant */
	   save = (LLIST *)K->last;
	   SetCurK(K,save);
	  do {
		 if(save && save->data)
		    ret = fct1(save->data);  /* ou (*fct1)() */
		 else
		    ret = L_PDN;

		if( _WLK_ )
		{
		 cpt++;
		 if(save->precd == NULL) /* permet de faire la derniere iteration */
		   last = LOK;
		 else
		  {
		    _SK_;
		    save = save->precd;
		    _LDEL_;
		    SetCurK(K,save);
		  }
		}

	     } while(last == LNOK && ret == LOK);
	   SetCurK(K,sk);  /* restore le curseur */
	   return(cpt);
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_LF: parcours de la liste du dernier au 1er avec appel d'une fct.     -
-----------------------------------------------------------------------------
*/
int walk_LF(LLIST **p,int (*fct1)(LLIST **))
{
 LKERNEL *K;
 LLIST *save;
 LLIST *sk;
 int last = LNOK;
 int cpt=0;
 int ret;


    if(!fct1)
      return(L_NOFCT);

    ret = TestList(&K,p);

    if(ret == LOK)
    {
	   sk = GetCurK(K);  /* sauve le curseur courant */
	   save = (LLIST *)K->last;
	   SetCurK(K,save);
	  do {
		 ret = fct1(p);  /* ou (*fct1)() */

		if( _WLK_ )
		{
		 cpt++;
		 if(save->precd == NULL) /* permet de faire la derniere iteration */
		   last = LOK;
		 else
		  {
		    _SK_;
		    save = save->precd;
		    _LDEL_;
		    SetCurK(K,save);
		  }
		}

	     } while(last == LNOK && ret == LOK);
	   SetCurK(K,sk);  /* restore le curseur */
	   return(cpt);
    }
   return(ret);
}

/*
****-----------------------------------------------------------------------------
-  datacompare: Comparaison de 2 data suivant le type de clef.	    -
****-----------------------------------------------------------------------------
*/
static int datacompare(LLIST **l,LKERNEL *K,void *s,void *d)
{
  CLKEY     clkey;
  LLIST     *save;
  int ret  = LNOK;
  int last = LNOK;
  char *s1;
  char *s2;

  if(!s || !d)
     return(LNOK);

   if( l == (LLIST **)NIL )
   {
//#ifdef _MEM_PC_
//      ret = PCmemcmp((const void *)s,(const void *)d,(size_t)K->lgr_key);
//#else
      ret = memcmp((const void *)s,(const void *)d,(size_t)K->lgr_key);
//#endif
   }
   else
   {
       save = *l;
       do{
	      memcpy(&clkey,save->data,LCLKEY);

	      s1 = (char *)s + clkey.off;
	      s2 = (char *)d + clkey.off;
/*
#ifdef _MEM_PC_
	      ret = PCmemcmp(s1,s2,(size_t)clkey.len);
#else
              ret = memcmp(s1,s2,(size_t)clkey.len);
#endif
*/   
             ret = memcmp_by_typ(&clkey,s1,s2);


	      if(save->suiv == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
	      else
		if( ret == LEQUAL )
		    save = save->suiv;
		else
		    last = LOK;

	 } while(last == LNOK);
   }

  if(  ret < 0 )
       return(LSMALL);
  if(  ret > 0 )
       return(LBIG);

return(LEQUAL);
}


/*
****-----------------------------------------------------------------------------
-  foundL: Recherche un element dans la liste.				    -
	   chx = choix pour la clef complexe LNOK = desactive
					     LOK  = active si existe
****-----------------------------------------------------------------------------
*/
static LLIST *foundL(LLIST **p,void *d,int chx)
{
 LLIST **lclef;
 LKERNEL *K;
 LLIST *save=*p;
 int last = LNOK;

    if( TestList(&K,p) == LOK )
    {
	  if( chx == LOK )           /* inhibe la clef complexe */
	      lclef = GetlistIKL(K);
	  else
	      lclef = ( LLIST **) NIL;

	  do {
		  if( datacompare(lclef,K,save->data,d) ==  LEQUAL)
		  {
		    SetCurK(K,save);
		    return(save);
		  }

		 if(save->suiv == NULL)
		   last = LOK;
		 else
		  save = save->suiv;

	     } while(last == LNOK);

	SetCurK(K,NIL);
    }
   return(NIL);
}


/*
-----------------------------------------------------------------------------
- Lfound: Indique si l'enregistrement existe dans la liste .                -
-----------------------------------------------------------------------------
*/
int Lfound(LLIST **l,void *elmt)
{
  int ret;
  LLIST *s;
  LLIST *p;
  LKERNEL *K;

  ret = TestListelmt(&K,l,elmt);

  if( ret == L_VIDE )
    return( LNOK );

  if( ret == LOK)
  {
    s = GetCurK(K);  /* sauve le curseur   */
    p = foundL(l,elmt,LOK);
    SetCurK(K,s);

    if(p)
      return(LOK);
    else
     return(LNOK);
  }

 return(ret);
}

/*
-----------------------------------------------------------------------------
- Prand: retourne le pointeur sur le data  si celui-ci existe dans la liste .                -
- Pdata_exist : idem.
-----------------------------------------------------------------------------
*/
void *Pdata_exist(LLIST **l,void *elmt)
{
  LLIST *s;
  LLIST *p;
  LKERNEL *K;

  if(TestListelmt(&K,l,elmt) == LOK)
  {
    s = GetCurK(K);  /* sauve le curseur   */
    p = foundL(l,elmt,LOK);
    SetCurK(K,s);

    if(p)
      return((void *)p->data);
  }

 return(NULL);
}

/*
****--------------------------------------------------------------------------
void *PClast(LLIST **l) :
****--------------------------------------------------------------------------
*/
void *PClast(LLIST **p)
{
 LKERNEL *K;
 LLIST *cu;

  if(TestList(&K,p) == LOK)
  {
	 cu = (LLIST *)K->last;

	 if(cu)
	 {
	    SetCurK(K,cu);
	    cu = GetCurK(K);
	    return((void *)cu->data);
	 }
  }

return(NULL);
}



/*
-------------------------------------------------------------------------------
Elle revoie le 1er elemnt et positionne le curseur sur le 1er
-------------------------------------------------------------------------------
*/
int GCfirst(LLIST **p,void *elmt)
{
 int ret=LNOK;
 LKERNEL *K;
 LLIST *cu;

  ret = TestListelmt(&K,p,elmt);

  if( ret ==  LOK)
  {
    cu = (LLIST *)K->first;
    if( cu ) 
     {
        SetCurK(K,cu);
        memcpy(elmt,((LLIST *)(K->first))->data,(size_t)K->lgr_data);
	return(LOK);
     }
     return(L_GRAVE);
  }

return(ret);
}

/*
-------------------------------------------------------------------------------
Positionne le curseur sur le dernier et renvoie l'element
-------------------------------------------------------------------------------
*/
int GClast(LLIST **p,void *elmt)
{
 int ret=LNOK;
 LKERNEL *K;
 LLIST *cu;

  ret = TestListelmt(&K,p,elmt);

  if( ret ==  LOK)
  {
    cu = (LLIST *)K->last;
    if( cu ) 
    {
        SetCurK(K,cu);
        memcpy(elmt,((LLIST *)(K->last))->data,(size_t)K->lgr_data);
      return(LOK);
    }

    return(L_GRAVE);
  }

return(ret);
}


/*
-------------------------------------------------------------------------------
Ne pas utiliser en tant que tel 
-------------------------------------------------------------------------------
*/
int ForeachListTest(LLIST **p,void *d,int sens)
{
    LKERNEL *K;
    int ret;

    ret = TestList(&K,p);

    if( ret == LOK ) 
    {
	switch( sens )
	{
	    case WFL:
		if( Pelmt(p) == Plast(p) )  // si je suis sur le dernier element
		    return(Glast(p,d));     // je retourne le dernier element
		break;
	    case WLF:
		if( Pelmt(p) == Pfirst(p) )  // si je suis sur le 1er element
		    return(Gfirst(p,d));     // je retourne le 1er element
		break;
	    default:
		   return(LNOK);
		break;   

	}

	return(Celmt(p,d));            // sinon elmt courant du curseur
    }

    return(ret); 
}  

/*
-------------------------------------------------------------------------------
Ne pas utiliser en tant que tel 
-------------------------------------------------------------------------------
*/
void *ForeachListTestp(LLIST **p,int sens)
{
  LKERNEL *K;

    if( TestList(&K,p) == LOK ) 
    {
       switch( sens )
       {
         case WFL:
	            if( Pelmt(p) == Plast(p) ) // si je suis sur le dernier element
	                return(Plast(p));      // je retourne le dernier element
	          break;		
	 case WLF:
	            if( Pelmt(p) == Pfirst(p) ) // si je suis sur le dernier element
	                return(Pfirst(p));      // je retourne le dernier element
	          break;		
	 default:
	          return(NULL);
		  break;
        }

      return(Pelmt(p));        // sinon element courant du curseur
    } 

return(NULL);
}  




/*
****--------------------------------------------------------------------------
void *PCfirst(LLIST **l) : positionne le curseur sur le 1er et le retourne
****--------------------------------------------------------------------------
*/
void *PCfirst(LLIST **p)
{
 LKERNEL *K;
 LLIST *cu;

  if(TestList(&K,p) == LOK)
  {
	 cu = (LLIST *)K->first;

	 if(cu)
	 {
	   SetCurK(K,cu);
	   cu = GetCurK(K);
	   return((void *)cu->data);
	 }
  }

return(NULL);
}

/*
-----------------------------------------------------------------------------
- PCrand: retourne le pointeur sur le data  si celui-ci existe dans la liste .                -
	  Positionne le curseur.
-----------------------------------------------------------------------------
*/
void *PCrand(LLIST **l,void *elmt)
{
  LLIST   *p;
  LKERNEL *K;

  if(TestListelmt(&K,l,elmt) == LOK)
  {
    p = foundL(l,elmt,LOK);

    if(p)
      return((void *)p->data);
  }

 return((void *)NULL);
}

/*
-----------------------------------------------------------------------------
- lsuppw: Suppression de tous les memes element dans la liste.                            -
-----------------------------------------------------------------------------
*/
int lsuppw(LLIST **p,void *d)
{
 LKERNEL *K;
 LLIST  *rl;
 LLIST  *pr;
 LLIST  *sv;
 LLIST  *cu;
 int    ret;
 int  cpt=0;

    ret = TestListelmt(&K,p,d);

    if( ret == LOK )
    {
      do{
	    rl = foundL(p,d,LOK);
	    if(rl)
	    {
		    pr = rl->precd;
		    sv = rl->suiv;
		    cpt++;

		    if((pr == sv) || (pr == NIL))               /* cas du 1er      */
		       corps(p);
		    else
		    {
			  if(sv == NIL)                         /* cas du dernier  */
			     corpi(p);
			  else
			  {                                     /* en plein milieu */
			    cu = (LLIST *)(K->current);

			    if( rl == cu )                      /* si on supp l'element pointe par le curseur  */
				K->current = pr;                /* on repositionne le curseur sur le precedent */

			    pr->suiv  = sv;
			    sv->precd = pr;
			    K->nbre_elmt--;
			    free_mem(rl,K->lgr_data);
			  }
		    }
	   }
	    else
		ret = LNOK;

	}while(ret == LOK);

       return(cpt);
    }
  return(ret);
}

/*
-----------------------------------------------------------------------------
- lsupp: Suppression d'un element dans la liste.			    -
-----------------------------------------------------------------------------
*/
int lsupp(LLIST **p,void *d)
{
 LKERNEL *K;
 LLIST  *rl;
 LLIST  *pr;
 LLIST  *sv;
 LLIST  *cu;
 int ret;

    ret = TestListelmt(&K,p,d);

    if( ret == LOK )
    {
       rl = foundL(p,d,LOK);
       if(rl)
       {
	       pr = rl->precd;
	       sv = rl->suiv;

	       if((pr == sv) || (pr == NIL))       /* il y a qu'un elmt 0 == 0      */
		  return(corps(p));                /* il s agit du 1er              */

	      if(sv == NIL)                        /* il s agit du dernier          */
		 return(corpi(p));
	      else                                 /* sinon elmt en milieu de liste */
	      {
	       cu = (LLIST *)(K->current);
	       if( rl == cu )                      /* si on supp l'element pointe par  */
		   K->current = pr;                /* le curseur on positionne le curseur sur le precedent */

		 pr->suiv  = sv;
		 sv->precd = pr;
	      }

	      K->nbre_elmt--;
	      free_mem(rl,K->lgr_data);

	      return(LOK);
       }
       return(LNOK);
    }
  return(ret);
}


/*
****-----------------------------------------------------------------------------
- initK: Initialise une partie du _KerneL.				    -
****-----------------------------------------------------------------------------
*/
static void initK(LKERNEL *K)   /* NODE */
{

 K->nbre_elmt = 0;
 K->first     = NULL;
 K->last      = NULL;
 K->current   = NULL;

}
/*
****-----------------------------------------------------------------------------
- SetCurK: Positionne le curseur.                   		             -
****-----------------------------------------------------------------------------
*/
static void SetCurK(LKERNEL *K,LLIST *l)
{
 if(K)
   K->current = (void *)l;
}

/*
****-----------------------------------------------------------------------------
- SetK: Positionne les infos sur la liste a initialisation. 	             -
****-----------------------------------------------------------------------------
*/
static void setK(LKERNEL *K,LLIST **a,LLIST *f,LLIST *l,LLIST *c,int d,int k,int e)
{
    K->adr       = a;           /* NODE */
    K->first     = (void *)f;
    K->last      = (void *)l;
    K->current   = (void *)c;
    K->lgr_data  = d;
    K->lgr_key   = k;
    K->nbre_elmt = e;
    K->bkey      = LNOK;
    K->kv        = LNOK;
    K->lkey      = NIL;
    K->sense     = LSMALL;
}
/*
****-----------------------------------------------------------------------------
- Kill_allkey: fonction special pour la destruction de toutes les clef interne .-
****-----------------------------------------------------------------------------
*/
static int kill_allkey(LLIST  **kl)
{
   LLIST *l = GetCurr(kl);
   LLIST **p;

   p = (LLIST **)(&((IKL*)(l->data))->clef);

   close_list(p);

 return(LOK);
}


/*
****-----------------------------------------------------------------------------
- Kill_all: fonction special pour la destruction du _KerneL.  		    -
****-----------------------------------------------------------------------------
*/
static int kill_all(LLIST  **ker, LKERNEL *K)
{
   LLIST *l = GetCurr(ker);
   LLIST **p;

   p   = (LLIST **)(((LKERNEL*)(l->data))->adr);

   close_list(p);

 return(LOK);
}

/*
-----------------------------------------------------------------------------
- close_list: Ferme une liste avec suppression de celle-ci dans _KerneL.     -
-----------------------------------------------------------------------------
*/
int close_list(LLIST **p)
{
  LKERNEL *K;
  int rep;

  if( p == CLOSE_ALL)
   {
       _cptl_= 0;
       rep   = walk_mem(&_KerneL,kill_all);
       __flgefirst__=LNOK;
       return(_cptl_);
   }

   rep = delete_list(p);

 if( p != &_KerneL )
   if( rep != NLD )
   {
     K = LLIST_exist(p);

     if(K)
     {
	 if(listborn(&K->lkey) == LOK)
	  {
	     walk_LF(&K->lkey,kill_allkey);
	     close_list(&K->lkey);
	  }

	 return( lsupp(&_KerneL,K) );
     }
   }
 return(rep);
}


/*
****-----------------------------------------------------------------------------
- GetCurr: retourne le curseur.                                             -
****-----------------------------------------------------------------------------
*/
static LLIST *GetCurr(LLIST **l)
{
  LKERNEL  *K;                        /* info sur la liste */

  if(TestList(&K,l) == LOK)
     return((LLIST*)K->current);

 return(NIL);
}

/*
****-----------------------------------------------------------------------------
- GetCurK: retourne le curseur.                                             -
****-----------------------------------------------------------------------------
*/
static LLIST *GetCurK(LKERNEL *K)
{
  if(K)
   return((LLIST*)K->current);
  else
   return(NIL);
}

/*
****-----------------------------------------------------------------------------
- SetCurr: Positionne le curseur.                                             -
  Pas utilise pour l'instant
****-----------------------------------------------------------------------------

int SetCurr(LLIST **l,LLIST *p)
{
  int ret;
  LKERNEL  *K;

  ret = TestList(&K,l);
  if( ret == LOK)
   K->current = p;

 return(ret);
}
*/

/*
-----------------------------------------------------------------------------
- Cfirst: Deplace au debut de la liste le curseur .                         -
-----------------------------------------------------------------------------
*/
int Cfirst(LLIST **l)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */

  ret = TestList(&K,l);

  if( ret == LOK)
    SetCurK(K,(LLIST *)K->first);

 return(ret);
}


/*
-----------------------------------------------------------------------------
- Clast: Deplace a la fin de la liste le curseur .                          -
-----------------------------------------------------------------------------
*/
int Clast(LLIST **l)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */

  ret = TestList(&K,l);

  if( ret == LOK )
      SetCurK(K,(LLIST *)K->last);

 return(ret);
}


/*
-------------------------------------------------------------------------------
Pointeur sur l'element suivant
-------------------------------------------------------------------------------
*/
void *PCNext(LLIST **p)
{
  if( Cnext(p) == LOK ) 
    return( Pelmt(p) );

 return(NULL);
}

/*
-------------------------------------------------------------------------------
Pointeur sur l'element courant
-------------------------------------------------------------------------------
*/
void *PCPrev(LLIST **p)
{
  if( Cnext(p) == LOK ) 
    return( Pelmt(p) );

 return(NULL);
}



/*
-----------------------------------------------------------------------------
- Cnext: Deplace vers la droite curseur .                                   -
-----------------------------------------------------------------------------
*/
int Cnext(LLIST **p)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestList(&K,p);

  if( ret == LOK)
  {
    l =(LLIST *)K->current;

    if(l)
    {
	 if(l->suiv)
	 {
	  SetCurK(K,l->suiv);
	  return(LOK);
	 }
      return(LNOK);
    }
   return(NPC);
  }
 return(ret);
}

/*
-----------------------------------------------------------------------------
- Bnext: Previens s il y a un suivant.                                    -
-----------------------------------------------------------------------------
*/
int Bnext(LLIST **p)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestList(&K,p);

  if( ret == LOK )
  {
    l =(LLIST *)K->current;
    if( l == NIL )
      return(LNOK);
     else
      if( l->suiv == NIL )
	return(LNOK);
      else
	return(LOK);
  }
 return(ret);
}

/*
-----------------------------------------------------------------------------
- Cprev: deplace vers la gauche le curseur .                                -
-----------------------------------------------------------------------------
*/
int Cprev(LLIST **p)
{
  int ret;
  LKERNEL *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestList(&K,p);

  if( ret == LOK )
  {
    l =(LLIST *)K->current;
    if(l)
    {
	if( l->precd)
	{
	 SetCurK(K,l->precd);
	 return(LOK);
	}
	return(LNOK);
    }
   return(NPC);
  }
 return(ret);
}


/*
-----------------------------------------------------------------------------
- Bprev: previens qu il n y a pas de precedent.                             -
-----------------------------------------------------------------------------
*/
int Bprev(LLIST **p)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestList(&K,p);

  if( ret == LOK )
  {
    l =(LLIST *)K->current;
    if( l == NIL )
      return(LNOK);
    else
      if( l->precd == NIL)
	return(LNOK);
      else
	return(LOK);
  }
 return(ret);
}

/*
---------------------------------------------------------------------------
 Celmt : Recopy l'element courant (du curseur) dans elmt.
---------------------------------------------------------------------------
*/
int Celmt(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
    l =(LLIST *)K->current;
    if(l)
    {
      memcpy(elmt,l->data,(size_t)K->lgr_data);
      return(LOK);
    }
   return(NPC);
  }
 return(ret);
}

/*
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
*/
static int LunsetCursor(LLIST **p)
{
 LKERNEL *K;

  K = LLIST_exist(p);

  if(K)
  {
    K->current = (LLIST *)NULL;
    return(LOK);
  }

return(NLD);
}


/*
---------------------------------------------------------------------------
 Pelmt : Retourne le pointeur  l'element courant.
---------------------------------------------------------------------------
*/
void *Pelmt(LLIST **p)
{
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;

  if(TestList(&K,p) == LOK)
  {
    l =(LLIST *)K->current;
    if(l)
      return((void *)l->data);

   return(NULL);
  }
 return(NULL);
}

/*
-----------------------------------------------------------------------------
- Cdel: Supprime element pointer par le curseur .                           -
-----------------------------------------------------------------------------
*/
int Cdel(LLIST **p)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;
  LLIST *pr,*sv;

  ret = TestList(&K,p);

  if( ret == LOK)
  {
     l =(LLIST *)K->current;
     if(l)
     {
	pr = l->precd;
	sv = l->suiv;

	if(K->current == K->first )
	   ret  = corps(p);
	else
	{
	   if(K->current == K->last )
	      ret = corpi(p);
	   else
	   {
	     if(pr)
	       pr->suiv  = sv;

	     if(sv)
		sv->precd = pr;

	     K->nbre_elmt--;
	     free_mem(l,K->lgr_data);

	     ret = LOK;
	   }
	 }

	if(*p && (ret == LOK ))           /* si la liste n'est pas vide alors le courant = precedent */
	 {
	    if(pr)
		K->current = pr;
	    else
	      if(sv)
		 K->current = sv;
	      else
		 K->current = NIL;

	 }

       return(ret);
      }
   return(NPC);
  }

return(ret);

}

/*
-----------------------------------------------------------------------------
- Cchange: Change les datas du curseur .                                    -
-----------------------------------------------------------------------------
*/
int Cchange(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *l;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK )
  {
    l =(LLIST *)K->current;
    if(l)
    {
      memcpy(l->data,elmt,(size_t)K->lgr_data);
      return(LOK);
    }
   return(NPC);
  }
 return(ret);
}

/*
-----------------------------------------------------------------------------
- Cinsr: Insere un element a la droite du curseur .                         -
-----------------------------------------------------------------------------
*/
int Cinsr(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *news;
  LLIST *cu,*sv;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
     if(K->current)
      {
	     if( K->current == K->last || K->first == K->last) /* ajout a la fin */
		adjq(p,elmt);
	      else
	      {
		 cu = (LLIST *)K->current;
		 sv = cu->suiv;

		 news = (LLIST *)allocation(lgr_L);     /* allocation de la LLISTE */
		 news->suiv = cu->suiv;                /* suivant du courant     */
		 news->precd = cu;                     /* le courant             */

		 news->data = (void *)allocation(K->lgr_data); /* allocation des data    */
		 memcpy(news->data,elmt,(size_t)K->lgr_data);          /* recopie des data       */

		 K->nbre_elmt++;
		 cu->suiv = news;
		 sv->precd = news;
	      }
	   return(LOK);
      }
    return(NPC);
  }
 return(ret);
}

/*
-----------------------------------------------------------------------------
- Cinsl: Insere un element a la gauche du curseur .                         -
-----------------------------------------------------------------------------
*/
int Cinsl(LLIST **p,void *elmt)
{
  int ret;
  LKERNEL  *K;                        /* info sur la liste */
  LLIST *news;
  LLIST *cu,*pr;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
	if(K->current)
	{
	    if( K->current == K->first  ||  K->first == K->last)     /* ajout sur la tete */
	      adjt(p,elmt);
	    else
	      {
		 cu = (LLIST *)K->current;
		 pr = cu->precd;

		 news = (LLIST *)allocation(lgr_L);     /* allocation de la LLISTE */
		 news->suiv = cu;                      /* suivant du courant     */
		 news->precd = pr;                     /* le courant             */

		 news->data = (void *)allocation(K->lgr_data); /* allocation des data    */
		 memcpy(news->data,elmt,(size_t)K->lgr_data);          /* recopie des data       */

		 K->nbre_elmt++;
		 cu->precd = news;
		 pr->suiv  = news;
	      }
	   return(LOK);

       }
    return(NPC);
  }

 return(ret);
}

/*
-----------------------------------------------------------------------------
- Crand: Se positionne a l'enregistrement souhaite.                         -
-----------------------------------------------------------------------------
*/
int Crand(LLIST **p,void *elmt)
{
 return(Lrand(p,elmt,LOK));
}

/*
****-------------------------------------------------------------------------
- Lrand: Se positionne a l'enregistrement souhaite.                         -
****-------------------------------------------------------------------------
*/
static int Lrand(LLIST **p,void *elmt,int chx)
{
  int ret;
  LLIST *l;
  LKERNEL *K;

  ret = TestListelmt(&K,p,elmt);

  if( ret == L_VIDE )
     return( LNOK );

  if( ret == LOK )
  {
    l = foundL(p,elmt,chx);
    if(l)
      return(LOK);
    else
     return(LNOK);
  }
 return(ret);
}

/*
****-------------------------------------------------------------------------
- TestList: Test si la liste est declare et si elle a 1 ou plusieurs elmt . -
****-------------------------------------------------------------------------
*/
static int TestList(LKERNEL **K,LLIST **p)
{
  if( p && !lvide(p)) // test du p important ..attention au flatulence 
  {
    *K = LLIST_exist(p);

    if((*K) != NULL)
    {
      if( (*K)->nbre_elmt > 0 )
	return(LOK);
      else
	return(LNOK);
    }
    else
     return(NLD);
 }
 return(L_VIDE);
}

/*
****-------------------------------------------------------------------------
- TestListelmt : Test si la liste est declare et si elle a 1 ou plusieurs elmt . -
****-------------------------------------------------------------------------
*/
static int TestListelmt(LKERNEL **K,LLIST **p,void *elmt)
{
   if( elmt == (void *)NULL )
    return(D_NULL);

 return( TestList(K,p) );
}


/*************************************************************************/
/** INFO DIRECTE DE LA LLISTE                                            **/
/*************************************************************************/
/*
-----------------------------------------------------------------------------
- lgetsense : Retourne le type de tri de la liste.
-----------------------------------------------------------------------------
*/
int lgetsense(LLIST **l)
{
 LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
    return( K->sense );

 return(NLD);
}

/*
-----------------------------------------------------------------------------
- lsetincrease : Positionne la liste en sense croissant.
-----------------------------------------------------------------------------
*/
int lsetincrease(LLIST **l)
{
 LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
  {
    K->sense = LSMALL;
    return(LOK);
  }
 return(NLD);
}


/*
-----------------------------------------------------------------------------
- lsetdecrease : Positionne la liste en sense decroissant.
-----------------------------------------------------------------------------
*/
int lsetdecrease(LLIST **l)
{
 LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
  {
    K->sense = LBIG;
    return(LOK);
  }
 return(NLD);
}


/*
-----------------------------------------------------------------------------
- Getlendata : retourne la longueur en octets de element de la liste courante
-----------------------------------------------------------------------------
*/
int Getlendata(LLIST **l)
{
  LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
    return(K->lgr_data);

 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- Getlenkey : retourne la longueur de la clef pour la recherche.
-----------------------------------------------------------------------------
*/
int GetlenKey(LLIST **l)
{
  LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
    return(K->lgr_key);

 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- lget_mem_alloc : retourne le d'octet alloue au moment T.
-----------------------------------------------------------------------------
*/
long lget_mem_alloc()
{
  return(_nb_calloc_);
}

/*
-----------------------------------------------------------------------------
- lget_mem_free : retourne le d'octet dessalloue au moment T.
-----------------------------------------------------------------------------
*/
long lget_mem_free()
{
   return(_nbfree_);
}

/*
-----------------------------------------------------------------------------
- lget_set_trace_mem : si == LOK -> comptage octets alloue / desallou
-----------------------------------------------------------------------------
*/
void lset_trace_mem(int lchx)
{
 _Ltrace_Mem_ = lchx;
}

/*
-----------------------------------------------------------------------------
- lget_trace : retourne la valeur du flag de comptage memoire
-----------------------------------------------------------------------------
*/
int lget_trace_mem()
{
 return(_Ltrace_Mem_);
}

/*
****--------------------------------------------------------------------------
- Getfirst : retourne le noeud du 1er element de la liste !!
****--------------------------------------------------------------------------

LLIST *Getfirst(LLIST **l)
{
  LKERNEL *K;

  if(TestList(&K,l) == LOK)
    return((LLIST *)K->first);

 return(NIL);
}
*/

/*
****-----------------------------------------------------------------------------
- Getlast : retourne le noeud du dernier element de la liste !!
****-----------------------------------------------------------------------------

LLIST *Getlast(LLIST **l)
{
  LKERNEL *K;

  if(TestList(&K,l) == LOK)
    return((LLIST *)K->last);

 return(NIL);
}
*/

/*
-----------------------------------------------------------------------------
- Pfirst : retourne l'adresse des data du 1er element de la liste !!
-----------------------------------------------------------------------------
*/
void *Pfirst(LLIST **l)
{
  LKERNEL *K;

  if(TestList(&K,l) == LOK)
    return( ((LLIST *)(K->first))->data  );

 return(NULL);
}


/*
-----------------------------------------------------------------------------
- Plast : retourne l'adresse des data du dernier element de la liste !!
-----------------------------------------------------------------------------
*/
void *Plast(LLIST **l)
{
  LKERNEL *K;

  if(TestList(&K,l) == LOK)
    return(((LLIST *)(K->last))->data);

 return(NULL);
}

/*
-----------------------------------------------------------------------------
- SetBKey : Autorise ou non la clef complexe num.                          -
-----------------------------------------------------------------------------
*/
int SetBKey(LLIST **l,int num)
{
 IKL  *ikl;
 LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
  {
       if( listborn(&K->lkey) == LOK )
       {
	   ikl = GetadrIKL(l,num);

	  if(ikl)
	   {
	      K->kv   = num;
	      K->bkey = LOK;
	      return( LOK );
	    }

	 return( L_IKN );
      }
      else
      {
	K->bkey = LNOK;
	K->kv   = LNOK;
	return( L_KEY );
      }
 }

  return(NLD);
}
/*
-----------------------------------------------------------------------------
- GetBKey : Permet de savoir si la clef complexe est active                 -
-----------------------------------------------------------------------------
*/
int GetBKey(LLIST **l)
{
 LKERNEL *K;

  K = LLIST_exist(l);
  if(K)
  {
     if( listborn(&K->lkey) == LOK ) /* que s'il y a une clef */
     {
	if( K->bkey == LOK )
	   return( K->kv );

       return(LNOK);
     }
     return( L_KEY );
  }

  return(NLD);
}
/*
-----------------------------------------------------------------------------
- UnSetBKey : Bloque la clef complexe en interne.
-----------------------------------------------------------------------------
*/
int UnSetBKey(LLIST **l)
{
  LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
   {
     if( listborn(&K->lkey) == LOK )
      {
	K->bkey = LNOK;
	return(LOK);
      }
      else
      {
	 K->bkey = LNOK;
	 K->kv   = LNOK;
	 return( L_KEY );
      }
   }
 return(NLD);
}


/*
-----------------------------------------------------------------------------
- SetlenKey : modifie la longueur de la clef !!
-----------------------------------------------------------------------------
*/
int SetlenKey(LLIST **l,int newKey)
{
  LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
   {
    if(newKey > 0 && newKey <= K->lgr_data)
      {
	K->lgr_key = newKey;
	return(LOK);
      }
     return(LNOK);
   }
 return(NLD);
}


/*
****--------------------------------------------------------------------------
- Nnext : retourne le pointeur sur le suivant !!
****--------------------------------------------------------------------------
*/
static LLIST *Nnext(LLIST *m)
{
  if(m && m->suiv)
   return(m->suiv);

return(NIL);
}

/*
****-------------------------------------------------------------------------
- Nprev : retourne le pointeur sur le precedent !!
****-------------------------------------------------------------------------
*/
static LLIST *Nprev(LLIST *m)
{
  if(m && m->precd)
   return(m->precd);

return(NIL);
}


/*
****-------------------------------------------------------------------------
- pinf : recherche si le noeud (d) se trouve avant le noeud (p)
****-------------------------------------------------------------------------
*/
static int pinf(LLIST *p,LLIST *d)
{
 int cpt=0;
 LLIST *pr=p;

 if( !p || !d || p == d)
   return(LNOK);
 else
 {
  do{
       if(Nprev(pr))
       { cpt++;
	 pr = Nprev(pr);
       }

      if(pr == d )
	 return(LNOK);

     if(cpt > 4)   /* vrai pour le sort */
		   /* la recherche en arriere n'exede pas 2 */
	return(LOK);

    }while(pr->precd);
 }
return(LOK);
}


/*
****-------------------------------------------------------------------------
- exchange_data : Echange le pointeur data .
****-------------------------------------------------------------------------
*/
static void exchange_data(LLIST *s,LLIST *d)
{
 void *data;

 if(s && d)
 {
     data    = s->data;
     s->data = d->data;
     d->data = data;
 }
}


/*
****-------------------------------------------------------------------------
- listsort : fait le tri de la liste : algo du tri sort !!
****-------------------------------------------------------------------------
*/
static void listsort(LLIST *p,LLIST *d,int (*fct)(void *a,void *b))
{
  void *cle;
  LLIST *m,*c;

  if(pinf(p,d) == LOK)
  {
      _cptl_++;

      c   = p;
      cle = p->data;
      m   = p;

      do{
	    if(Nnext(c))
	     {
		c = Nnext(c);

		if(fct(c->data,cle) == LOK)
		{
		    if(Nnext(m))
		    {
		      m = Nnext(m);
		      exchange_data(c,m);
		    }
		}
	      }
	      else
		break; /* jamais utilise !! */

      }while(c != d);

     exchange_data(p,m);

    if(Nprev(m))
     listsort(p,Nprev(m),fct);

    if(Nnext(m))
     listsort(Nnext(m),d,fct);
  }
}


/*
-----------------------------------------------------------------------------
- ltri : fait le tri de la liste en fonction du code retour de fct .!!
-----------------------------------------------------------------------------
*/
int ltri(LLIST **l,int (*fct)(void *a,void *b))
{
  int ret;
  int lgr;
  LKERNEL *K;

 if(fct)
 {
    ret = TestList(&K,l);

    if( ret == LOK )
    {
      _cptl_=0;

      lgr=lgrl(l);

      if(lgr > 1)
      {
       listsort((LLIST *)K->first,(LLIST *)K->last,fct);
       return(_cptl_);
      }
      return(LOK);
    }
    return(ret);
 }
return(LNOK);
}

/*
****-------------------------------------------------------------------------
- listsortk : fait le tri de la liste : algo du tri sort !!
****-------------------------------------------------------------------------
*/
static void listsortk(LLIST **lk,LKERNEL *K,LLIST *p,LLIST *d)
{
  void *cle;
  LLIST *m,*c;

  if(pinf(p,d) == LOK)
  {
      _cptl_++;

      c   = p;
      cle = p->data;
      m   = p;

      do{
	    if(Nnext(c))
	     {
		c = Nnext(c);

		 if ( datacompare(lk,K,c->data,cle) == K->sense )
		  {
		      if(Nnext(m))
		      {
			m = Nnext(m);
			exchange_data(c,m);
		      }
		  }
	      }
	      else
		break; /* jamais utilise !! */

      }while(c != d);

     exchange_data(p,m);

    if(Nprev(m))
     listsortk(lk,K,p,Nprev(m));

    if(Nnext(m))
       listsortk(lk,K,Nnext(m),d);

  }
}


/*
-----------------------------------------------------------------------------
- lsort : fait le tri de la liste en fonction de la clef.!!
-----------------------------------------------------------------------------
*/
int lsort(LLIST **l)
{
  int ret;
  LLIST **lclef;
  LKERNEL *K;

    ret = TestList(&K,l);

    if( ret == LOK )
    {
      _cptl_ = 0;

      lclef = GetlistIKL(K);

/*    if( lclef == (LLIST **)NIL )
	return( L_KEY ); */

	listsortk(lclef,K,(LLIST *)K->first,(LLIST *)K->last);

      return(_cptl_);
    }
 return(ret);
}



/*
****-------------------------------------------------------------------------
- lrecopy : fait la copie de la liste (d) dans la liste (s) si chx=OK alors
	    recopie sinon cat
****------------------------------------------------------------------------
*/
static int lrecopy(LLIST **s,LLIST **d,int chx,int (*fct)(LLIST **,LLIST **,void *))
{
  int ret=LOK;
  int last=LNOK;
  LLIST *save=*s;
  LLIST *sks,*skd;
  LKERNEL *Ks,*Kd;

 if( Lverifsame(s,d) != LOK )
    return( L_IDT );

  Kd = LLIST_exist(d);

 if(TestList(&Ks,s) == LOK && Kd)
 {
       if(Getlendata(s) != Getlendata(d))
	  return(LNOK);
       else
       {
	  _cptl_ = 0;
	  sks = GetCurK(Ks); /* sauve les curseurs */
	  skd = GetCurK(Kd);

	  SetCurK(Ks,save);  /* positionne sur le 1er curseur */

	  if(chx == LOK)
	    delete_list(d);

	  do {
		_cptl_++;

		 if(fct)
		   ret = fct(s,d,save->data);

		 if(ret == LOK)
		    if( Kd->bkey == LOK )
		       lajout(d,save->data);
		    else
		       adjq(d,save->data);

		 if(save->suiv == NIL) /* permet de faire la derniere iteration */
		   last = LOK;
		 else
		 {
		    save = save->suiv;
		    SetCurK(Ks,save);
		 }

	     }while( last == LNOK && ret != LSTOP );

	 SetCurK(Ks,sks);
	 SetCurK(Kd,skd);

	 return(_cptl_);
       }
 }

 return(NLD);
}


/*
-----------------------------------------------------------------------------
- lrcpy : fait la copie de la liste (d) dans la liste (s).
-----------------------------------------------------------------------------
*/
int lrcpy(LLIST **s,LLIST **d)
{
  return(lrecopy(s,d,LOK,NULL));
}

/*
-----------------------------------------------------------------------------
- lrcat : concatenation de la liste (d) dans la liste (s).
-----------------------------------------------------------------------------
*/
int lrcat(LLIST **s,LLIST **d)
{
  return(lrecopy(s,d,LNOK,NULL));
}


/*
-----------------------------------------------------------------------------
- lwrcpy : recopie la liste (d) dans la liste (s) suivant code retour
	   de fct.
-----------------------------------------------------------------------------
*/
int lwrcpy(LLIST **s,LLIST **d,int (*fct)(LLIST **,LLIST **,void *))
{
  if(fct)
    return(lrecopy(s,d,LOK,fct));

 return(LNOK);
}


/*
-----------------------------------------------------------------------------
- lwrcat : concatenation de la liste (d) dans la liste (s) suivant code retour
	  de fct.
-----------------------------------------------------------------------------
*/
int lwrcat(LLIST **s,LLIST **d,int (*fct)(LLIST **,LLIST **,void *))
{
  if(fct)
     return(lrecopy(s,d,LNOK,fct));

 return(LNOK);
}


/*
-----------------------------------------------------------------------------
- lmemcmp : comparaison de la liste (d) dans la liste (s). membre a membre
-----------------------------------------------------------------------------
*/
int lmemcmp(LLIST **s,LLIST **d)
{
  int stop=LOK;
  LKERNEL *Ks;
  LKERNEL *Kd;

  if( Lverifsame(s,d) != LOK )
      return( L_IDT );

    if( (TestList(&Ks,s) == LOK) && (TestList(&Kd,d) == LOK) )
    {
      if((Ks->lgr_data == Kd->lgr_data) && (Ks->nbre_elmt == Kd->nbre_elmt))
       {

	 Cfirst(s);
	 Cfirst(d);

	 do{
	     stop = Bnext(s);

	     if(memcmp(Pelmt(s),Pelmt(d),(size_t)Ks->lgr_data) ==  LEQUAL )
	     {
	       Cnext(s);
	       Cnext(d);
	     }
	     else
	      return(LNOK);

	   }while(stop == LOK);

	 return(LOK);
       }
     return(LNOK);
    }

 return(NLD);
}

/*
-----------------------------------------------------------------------------
- lcmpD : comparaison de la liste (d) dans la liste (s) sur la taille complete
	  des datas
-----------------------------------------------------------------------------
*/
int lcmpD(LLIST **s,LLIST **d)
{
  int lens,lend,ret;

 if( (listborn(s) == LOK ) &&  (listborn(d) == LOK ) )
  {
      lens = GetlenKey(s);         /* sauve la taille de la clef */
      lend = GetlenKey(d);

      SetlenKey(s,Getlendata(s));  /* la clef = taille des datas */
      SetlenKey(d,Getlendata(d));

      ret = lcompare(s,d,LNOK);

      SetlenKey(s,lens);
      SetlenKey(d,lend);

      return(ret);
  }

 return(NLD);
}

/*
-----------------------------------------------------------------------------
- lcmp : comparaison de la liste (d) dans la liste (s).
-----------------------------------------------------------------------------
*/
int lcmp(LLIST **s,LLIST **d)
{
 return(lcompare(s,d,LNOK));
}


/*
-----------------------------------------------------------------------------
- lcmpK : comparaison de la liste (d) dans la liste (s) avec clef complexe.
-----------------------------------------------------------------------------
*/
int lcmpK(LLIST **s,LLIST **d)
{
 return(lcompare(s,d,LOK));
}

/*
****-------------------------------------------------------------------------
- lcompare : comparaison de la liste (d) dans la liste (s).
****-------------------------------------------------------------------------
*/
static int lcompare(LLIST **s,LLIST **d,int chx)
{
  int stop=LOK;
  LKERNEL *Ks;
  LKERNEL *Kd;

  if( Lverifsame(s,d) != LOK )
      return( L_IDT );

    if( (TestList(&Ks,s) == LOK) && (TestList(&Kd,d) == LOK) )
    {
      if((Ks->lgr_data == Kd->lgr_data) && (Ks->nbre_elmt == Kd->nbre_elmt))
       {
	   Cfirst(s);

	     do{
		 stop = Bnext(s);

		 if( Lrand(d,Pelmt(s),chx) == LOK )
		     Cnext(s);
		  else
		    return(LNOK);

	      }while(stop == LOK);

	   return(LOK);
       }
      return(LNOK);
    }

 return(NLD);
}


/*
-----------------------------------------------------------------------------
- Rlast Positionne le curseur sur le dernier elmt et recopie                   -
-----------------------------------------------------------------------------
*/
int Rlast(LLIST **p,void *elmt)
{
  LKERNEL  *K;                        /* info sur la liste */
  int ret;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
      SetCurK(K,(LLIST *)K->last);
      memcpy(elmt,((LLIST *)(K->last))->data,(size_t)K->lgr_data);
  }
  return(ret);
}

/*
-----------------------------------------------------------------------------
- Rfirst Positionne le curseur sur le 1er elmt et recopie                   -
-----------------------------------------------------------------------------
*/
int Rfirst(LLIST **p,void *elmt)
{
  LKERNEL  *K;                        /* info sur la liste */
  int ret;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK)
  {
      SetCurK(K,(LLIST *)K->first);
      memcpy(elmt,((LLIST *)(K->first))->data,(size_t)K->lgr_data);
  }
  return(ret);
}

/*
-----------------------------------------------------------------------------
- Rrand: Se positionne a l'enregistrement souhaite.                         -
-----------------------------------------------------------------------------
*/
int Rrand(LLIST **p,void *elmt)
{
  if(Lrand(p,elmt,LOK) == LOK)
    return(Celmt(p,elmt));

 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- Rprev: deplace vers la gauche le curseur .                                -
-----------------------------------------------------------------------------
*/
int Rprev(LLIST **p,void *elmt)
{
  if(Lrand(p,elmt,LOK) == LOK)
      if(Cprev(p) == LOK)
	return(Celmt(p,elmt));

 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- Rnext: Deplace vers la droite curseur .                                   -
-----------------------------------------------------------------------------
*/
int Rnext(LLIST **p,void *elmt)
{

  if(Lrand(p,elmt,LOK) == LOK)
    if(Cnext(p) == LOK)
       return(Celmt(p,elmt));

return(LNOK);
}

/*
-----------------------------------------------------------------------------
- Rnextw: Deplace vers la droite curseur d'apres la clef.                   -
-----------------------------------------------------------------------------
*/
int Rnextw(LLIST **p,void *elmt)
{
  LLIST **lclef;
  LLIST *l;
  LKERNEL  *K;                        /* info sur la liste */
  int ret;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK )
  {
    l = (LLIST *)K->current;

    if( l == NIL )
      return(LNOK);
    else
      if( l->suiv == NIL )
	return(LNOK);

    l = l->suiv; /* on pointe sur le suivant */

    lclef = GetlistIKL(K);

    if( datacompare(lclef,K,l->data,elmt) ==  LEQUAL )
    {
       SetCurK(K,l); /* courant = suivant */
       return(Celmt(p,elmt));
    }
    return(LNOK);
  }
return(ret);
}

/*
-----------------------------------------------------------------------------
- Rprevw: Deplace vers la gauche curseur d'apres la clef.                   -
-----------------------------------------------------------------------------
*/
int Rprevw(LLIST **p,void *elmt)
{
  LLIST **lclef;
  LLIST *l;
  LKERNEL  *K;                        /* info sur la liste */
  int ret;

  ret = TestListelmt(&K,p,elmt);

  if( ret == LOK )
  {
    l = (LLIST *)K->current;
    if( l == NIL )
      return(LNOK);
    else
      if( l->precd == NIL )
	return(LNOK);


    l = l->precd; /* on pointe sur le precedent */

    lclef = GetlistIKL(K);

    if( datacompare(lclef,K,l->data,elmt) ==  LEQUAL )
    {
       SetCurK(K,l); /* courant = precedent */
       return(Celmt(p,elmt));
    }
   return(LNOK);
  }
return(ret);
}



/***************************************************************************/
/***************************************************************************/

/*
****-------------------------------------------------------------------------
- walk_list: Parcours la liste S					    -
****-------------------------------------------------------------------------
*/
static int walk_list(LLIST **s,LLIST **d,LLIST **r,int del,int chx,int choix,int fnd,int (*fct)(void *))
{
  int cpt = 0;
  int ret = LOK;
  LLIST *save=*s;
  LLIST *ss;
  int last = LNOK;
  int retd = LNOK;
  LKERNEL *Ks,*Kr,*Kd=(LKERNEL *)NULL;
  
 /* suivant les cas on prend on compte le fait d'etre vide pour la liste(d) */

 if( fnd == LNOK)
    retd = TestList(&Kd,d);
  else      /* on laisse faire la fct mem si la liste (d) est vide */
   {
     Kd = LLIST_exist(d);
     if(Kd)
       retd = LOK;
     else
       retd = LNOK;
   }


  if( TestList(&Ks,s) == LOK )
      if( retd  == LOK )  /*  if( TestList(&Kd,d) == LOK )*/
      {

	Kr = LLIST_exist(r);
	if(Kr)
	{
	    if( Kr->lgr_data != Ks->lgr_data || Ks->lgr_data != Kd->lgr_data )
	       return(LNOK);

	    if( del == LOK )
		 delete_list(r);

	    ss = GetCurK(Ks);

	    do {
		 if(fct)
		   ret = fct(save->data);

		 if(ret == LOK )
		   if( Crand(d,save->data) == chx )
		      if( Crand(r,save->data) == choix )
			      if( Ks->bkey == LOK )
			      {
			       lajout(r,save->data);
			       cpt++;
			      }
			      else
			      {
				adjq(r,save->data);
				cpt++;
			      }

		    if(save->suiv == NULL || ret == LSTOP ) /* permet de faire la derniere iteration */
		       last = LOK;
		    else
		      save = save->suiv;

		} while(last == LNOK);

	    SetCurK(Ks,ss);
	    return(cpt);
	}
     }
 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- linterlw: intersection entre la liste s et d r = s n d                    -
-----------------------------------------------------------------------------
*/
int linterlw(LLIST **s,LLIST **d,LLIST **r,int (*fct)(void *))
{
  delete_list(r);

 if( Lverifsame(s,d) != LOK )
       return( L_IDT );

  if( lgrl(s) == 0 || lgrl(d) == 0 )
       return( lgrl(r) );   /* ou 0 dans ce cas */


  return( walk_list(s,d,r,LOK,LOK,LNOK,LNOK,fct) );
}

/*
-----------------------------------------------------------------------------
- lunionlw: r = union entre la liste s et d r = s u d                        -
-----------------------------------------------------------------------------
*/
int lunionlw(LLIST **s,LLIST **d,LLIST **r,int (*fct)(void *))
{
  delete_list( r );

   if( Lverifsame(s,d) != LOK )
       return( L_IDT );

   if( lgrl(s) == 0 && lgrl(d) == 0 )
       return( lgrl(r) );

   if( !fct && lgrl(s) == 0 )
      return( lrcpy(d,r) );

   if( fct && lgrl(s) == 0 )
      return( walk_list(d,s,r,LNOK,LNOK,LNOK,LOK,fct) );

   if( !fct && lgrl(d) == 0 )
       return( lrcpy(s,r) );

   if( fct && lgrl(d) == 0 )
       return( walk_list(s,d,r,LNOK,LNOK,LNOK,LOK,fct) );

  if( lrcpy(s,r) > 0 )
     return( walk_list(d,s,r,LNOK,LNOK,LNOK,LNOK,fct) );

 return(LNOK);
}

/*
-----------------------------------------------------------------------------
- lxorlw: = tous les elements qui existe chez (s,d) et pas chez (d,s)      -
-----------------------------------------------------------------------------
*/
int lxorlw(LLIST **s,LLIST **d,LLIST **r,int (*fct)(void *))
{
    delete_list( r );

   if( Lverifsame(s,d) != LOK )
       return( L_IDT );

   if( lgrl(s) == 0 && lgrl(d) == 0 )
       return( lgrl(r) );

   if( !fct && lgrl(s) == 0 )
	return( lrcpy(d,r) );

   if( fct && lgrl(s) == 0 )
       return( walk_list(d,s,r,LNOK,LNOK,LNOK,LOK,fct) );

   if( !fct && lgrl(d) == 0 )
      return( lrcpy(s,r) );

   if( fct && lgrl(d) == 0 )
       return( walk_list(s,d,r,LNOK,LNOK,LNOK,LOK,fct) );

   if( walk_list(s,d,r,LOK,LNOK,LNOK,LNOK,fct) >=  0  )
	return( walk_list(d,s,r,LNOK,LNOK,LNOK,LNOK,fct) );

return(LNOK);
}


/*
-----------------------------------------------------------------------------
- ldifflw: (r) = tous les elements qui existe chez (s) et pas chez (d) sur flt         -
-----------------------------------------------------------------------------
*/
int ldifflw(LLIST **s,LLIST **d,LLIST **r,int (*fct)(void *))
{
    delete_list(r);

   if( Lverifsame(s,d) != LOK )
      return( L_IDT );

   if( lgrl(s) == 0 )
       return( lgrl(r) );

   if( !fct && lgrl(d) == 0 )
      return( lrcpy(s,r) );

  return( walk_list(s,d,r,LOK,LNOK,LNOK,LOK,fct) );
}


/*
-----------------------------------------------------------------------------
- lcount: Compte le nombre de data identique.                               -
-----------------------------------------------------------------------------
*/
int lcount(LLIST **p,void *d)
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST  *save=*p;
 int last = LNOK;
 int    cpt;
 int    ret;

   if( lgrl(p) == 0 ) /* si la liste est vide */
    return( 0 );

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK)
    {
	  cpt = 0;

	  lclef = GetlistIKL(K);

	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		    cpt++;

		if(save->suiv == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
		else
		  save = save->suiv;

	     }while(last == LNOK);

	 return(cpt);    /* retourne le nombre ide meme data  */
    }
   return(ret);
}



/***************************************************************************/
/**************************** CLEF COMPLEXE ********************************/
/***************************************************************************/

/*
****-------------------------------------------------------------------------
- loadkey: Charge en liste les types de clef les plus courants.             -
****-------------------------------------------------------------------------
*/
static int loadkey()
{
  CLKEY clkey;
  int i=0;

  if(initlist(&_LtypE_,LCLKEY,sizeof(char)) == LOK)
  {
     while(tclkey[i].typ)
     {
	clkey = tclkey[i];
	if( tclkey[i].typ )
	  adjq(&_LtypE_,&clkey);

      i++;
    }

    return(LOK);
  }
return(LNOK);
}


/*
****-------------------------------------------------------------------------
- calcul_offset: Calcul un offset entre 2 adresses.                         -
****-------------------------------------------------------------------------
*/
static int calcul_offset(void *elmt,void *pos,int max)
{
 char *deb;
 int off;

 deb = (char *)elmt;

 for(off=0;off< max;off++)
  if( (char *)deb++ == (char *)pos)
    return(off);

 return(LNOK);
}


/*
****-------------------------------------------------------------------------
- Get_Length_Sense: Permet convertir un CHAR en INT.                                 -
****-------------------------------------------------------------------------
*/
static int Get_Length_Sense(CLKEY *pclkey,char **pf)
{
 char num[20];
 int  cpt=0;

 memset(num,0,sizeof(num));

 num[0] = '1'; /* longeur minimum */

 if( pclkey && pf ) 
 {
     pclkey->kSense = LSMALL;

     if( **pf == '-' || **pf == '+' ) 
     {
       if( **pf == '-' )
         pclkey->kSense = LBIG;

       ++*pf;
     }


     while(**pf >='0' && **pf <='9' && cpt < 16)
     {
        num[cpt++] = **pf;
        ++*pf;
     }
 }

 return( atoi(num) );
}


/*
****-------------------------------------------------------------------------
- cherche_type: Permet de savoir si le type *pf existe dans la liste.       -
****-------------------------------------------------------------------------
*/
static int  cherche_type(CLKEY *clkey,char *pf)
{
  CLKEY clef;

  clef.typ = *pf;

 if( Crand(&_LtypE_,&clef) == LOK)
  {
    if (  Celmt(&_LtypE_,&clef) == LOK)
    {
	clkey->typ  =   clef.typ;
	clkey->len *=   clef.len;
	strcpy(clkey->name,clef.name);

      return(LOK);
    }
  }

   clkey->typ = '\0';
   return(LNOK);
}

/*
-----------------------------------------------------------------------------
- ladd_type: Permet a l'utilisateur de rajouter un nouveau type.            -
-----------------------------------------------------------------------------
*/
int ladd_type(char *name,char c,int len)
{
  CLKEY clef;

  clef.typ = c;

  if( Crand(&_LtypE_,&clef) == LOK)
    return(LNOK);

  clef.typ = c;
  clef.len = len;

    if((int)strlen(name) > MAXNAME)
      strncpy(clef.name,name,MAXNAME);
    else
     strcpy(clef.name,name);

return(  adjq(&_LtypE_,&clef) );

}

/*
-----------------------------------------------------------------------------
- lkey: Permet la declaration d'une clef pour les elements d'une liste.     -
-----------------------------------------------------------------------------
*/
int lkey(LLIST **l,void *elmt,int num,const char *fmt,...)
{
  LKERNEL   *K;
  CLKEY  clkey;
  IKL      ikl;
  IKL      *ik;
  LLIST  *ltmp;
  va_list   pa;
  void    *adr;
  char     *pf;
  char      *fmt2;
  int     ret=LOK;
  int     taille= strlen(fmt)+1;

  K = LLIST_exist(l);

  if(K)
  {
    if(elmt && fmt)
    {
      fmt2 = (char *)allocation( taille );

      strcpy(fmt2,fmt);

      va_start(pa,fmt);

      pf = strtok(fmt2,"%");

      if(!pf)
	 return(LNOK);

      initlist(&ltmp,LCLKEY,LCLKEY);

       do{
	    adr = (void *)va_arg(pa,void *); /* recuperation adresse */

            if(adr)
	      {
		  memset(&clkey,0,LCLKEY);

		  clkey.len = Get_Length_Sense(&clkey,&pf);  /* longueur des data    */

		  if( cherche_type(&clkey,pf) == LOK )
		  {
		      clkey.off = calcul_offset(elmt,adr,K->lgr_data); /* offset des data */
		      if(clkey.off != LNOK)
			  ret = adjq(&ltmp,&clkey);
		      else
		 	  ret = LNOK;
		   }
		  else
		    ret = L_TYPE;
	      }
	      else
		 ret = LNOK;

	      pf=strtok(NULL,"%");

       }while(pf && ret == LOK);

       va_end(pa);
       free(fmt2);
       _nbfree_ +=taille;

       if( ret == LOK )
       {
	   if(listborn(&K->lkey) == LOK)
	   {
	     ik = GetadrIKL(l,num);
	     if(ik)
	       delete_list(&ik->clef);
	     else
	     {
		ikl.num = num;
		adjq(&K->lkey,&ikl);
		ik = GetadrIKL(l,num);
		initlist(&ik->clef,LCLKEY,LCLKEY);
	     }

	   }
	   else
	   {
	     initlist(&K->lkey,LIKL,LIKLCLEF);

	     ikl.num = num;
	     adjq(&K->lkey,&ikl);
	     ik = GetadrIKL(l,num);
	     initlist(&ik->clef,LCLKEY,LCLKEY);
	   }

	   lrcpy(&ltmp,&ik->clef);    /* recopie de la liste des clefs. */
	   SetBKey(l,num);           /* autorise la clef.              */
       }

       close_list(&ltmp);

       return(ret);
    }
   return(LNOK);
  }
 return(NLD);

}


/*
****-------------------------------------------------------------------------
- GetadrIKL : retourne l'adresse de IKL num clef interne
****-------------------------------------------------------------------------
*/
static IKL *GetadrIKL(LLIST **l,int num)
{
   IKL ikl;
   LKERNEL   *K;

   K = LLIST_exist(l);

  if( K && (listborn(&K->lkey) == LOK) )
  {
       ikl.num = num;
       if( Crand(&K->lkey,&ikl) == LOK )
	 return( ((IKL *)Pelmt(&K->lkey)) );
  }

 return((IKL*)NULL);
}

/*
****-------------------------------------------------------------------------
GetlistIKL : retourne l'adresse de la liste des clefs pour une liste.
****-------------------------------------------------------------------------
*/
static LLIST **GetlistIKL(LKERNEL *K)
{
   IKL ikl;
   IKL *likl;

  if( K && K->bkey == LOK )            /* si lkey autorisee */
  {
      if( listborn(&K->lkey) == LOK )
      {
	 ikl.num = K->kv;
	 if( Crand(&K->lkey,&ikl) == LOK )
	 {
	   likl = ((IKL *)Pelmt(&K->lkey));
	   return((LLIST **)(&likl->clef));
	 }
     }
  }
 return((LLIST **)NIL);
}

/*
****-------------------------------------------------------------------------
- Lverifsame: Verifie si les 2 listes sont differents                       -
****-------------------------------------------------------------------------
*/
static int Lverifsame(LLIST **s,LLIST **d)
{
   LKERNEL   *KS,*KD;

   KS = LLIST_exist(s);

   KD = LLIST_exist(d);

   if( KS && KD )
      if(  KD == KS  )
	return( L_IDT );

   return(LOK);
}


/*---------------------- FIN pour CLEF complexe ---------------------------*/

/*
-----------------------------------------------------------------------------
- luajout : ajoute un element dans la liste en fonction de la clef.ajout trie -
	    que si celui-ci n'existe pas dans celle-ci.
------------------------------------------------------------------------------
*/
int luajout(LLIST **p,void *elmt)
{
  return( lajoute(p,elmt,LOK,LOK) );
}

/*
-----------------------------------------------------------------------------
- lajout : ajoute un element dans la liste en fonction de la clef.ajout trie -
---------------------------------------------------------------------------------
*/
int lajout(LLIST **p,void *elmt)
{
  return( lajoute(p,elmt,LNOK,LOK) );
}

/*
-----------------------------------------------------------------------------
- luajoutc : ajoute un element dans la liste en fonction de la clef.ajout trie -
	    que si celui-ci n'existe pas dans celle-ci.
------------------------------------------------------------------------------
*/
int luajoutc(LLIST **p,void *elmt)
{
  return( lajoute(p,elmt,LOK,LNOK) );
}

/*
-----------------------------------------------------------------------------
- lajout : ajoute un element dans la liste en fonction de la clef.ajout trie -
---------------------------------------------------------------------------------
*/
int lajoutc(LLIST **p,void *elmt)
{
  return( lajoute(p,elmt,LNOK,LNOK) );
}

/*
--------------------------------------------------------------------------------
- luadd : ajoute un element unique sans tri sans curseur                    -
--------------------------------------------------------------------------------
*/
int luadd(LLIST **p,void *elmt,int ou)
{
   LKERNEL   *K; /* info sur la liste */

   K = LLIST_exist(p);

   if(K != NULL)
   {
      if(K->nbre_elmt == 0 )
	return( adjq(p,elmt) ) ;

      if( Crand(p,elmt) == LOK )
         return( L_EXIST );

      if( ou )
         return(adjt(p,elmt));

      return( adjq(p,elmt) ) ; 		       
   }
  return(NLD);
}


/*
****----------------------------------------------------------------------------
- lajoute : ajoute un element dans la liste en fonction de la clef.ajout trie -
****----------------------------------------------------------------------------
*/
static int lajoute(LLIST **p,void *elmt,int unique,int cur)
{
   LKERNEL   *K; /* info sur la liste */
   LLIST    *sk;
   int      ret;
   int      chx;

   K = LLIST_exist(p);

   if(K != NULL)
   {
     /*  if( cur == LOK ) on peut tjours le faire au cas ou !!*/
	 sk = GetCurK(K);  /* sauve le curseur courant */

      if(K->nbre_elmt == 0 )
	ret = adjt(p,elmt);
      else
      {
	  if( unique == LOK )
	   if( Crand(p,elmt) == LOK )
	   {
	     if( cur == LOK )
		SetCurK(K,sk);  /* restore le curseur */
	     return( L_EXIST );
	   }

	  chx =  lfoundpos(p,K,elmt,K->sense);

	  if( K->sense == LSMALL )
	  {
	      switch( chx )
	      {
		 case LEQUAL:
		 case LBIG:
			   ret = Cinsl(p,elmt);
			  break;
		 case LSMALL:
		 default:
			  ret = Cinsr(p,elmt);
			  break;

	      }
	  }
	  else
	  {
	      switch( chx )
	      {
		 case LEQUAL:
		 case LSMALL:
			   ret = Cinsl(p,elmt);
			  break;
		 case LBIG:
		 default:
			  ret = Cinsr(p,elmt);
			  break;
	      }

	  }

     }

      if( cur == LOK )
	  SetCurK(K,sk);  /* restore le curseur */
      else
	 ret = Crand(p,elmt);  /* positionne le curseur sur element */

    return( ret );
   }
  return(NLD);
}



/*
****-----------------------------------------------------------------------------
-  lfoundpos: Recherche un element dans la liste.				    -
****-----------------------------------------------------------------------------
*/
static int lfoundpos(LLIST **p,LKERNEL *K,void *d,int chx )
{
 LLIST **lclef;
 LLIST *save=*p;
 int last = LNOK;
 int ret= LNOK;

    lclef = GetlistIKL(K);

  do {
	ret = datacompare(lclef,K,save->data,d);

	SetCurK(K,save);

	if( ret != LEQUAL  && ret != chx )
	  return(ret);

       if(save->suiv == NULL)
	    last = LOK;
       else
	  save = save->suiv;

    }while( last == LNOK );

   return(ret);
}

/*
****---------------------------------------------------------------------------
My_exit : En cas d'erreur fermeture de toutes les listes et exit du pgm.
****---------------------------------------------------------------------------
*/
static void My_exit()
{
  close_list(CLOSE_ALL);
}


/*
------------------------------------------------------------------------------

fct = 0 --> Pushllist sauvegarde la liste
fct = 1 --> Popllist  recharge   la liste

 Si fct = 0
    et chx = 0  => ne fait rien
    et chx = 1  => vide la liste apres sauvegarde

 Si fct = 1
  et chx = 0 => ajout du fichier a la liste ( ne vide pas la liste pas del fichier )
	   1 => supprime le fichier apres ajout
	   2 => vide d'abord la liste puis ajout le fichier
	   3 => option 2 + 1
------------------------------------------------------------------------------
*/
int   FtoLandLtoF(LLIST **p,char *path,int fct,int chx)
{
 LKERNEL   *K;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt = 0; /* nombre d'iteration */
 int        ret=NLD;
 char    *fbuff; /* buffer de lecture */
 int        mfp=-1; /* file pointer      */


    K = LLIST_exist(p);

    if(K)
    {
      if(fct == 0 )
      {
	if( K->nbre_elmt > 0  )   /* Pushllist */
	  mfp=open(path,O_RDWR|O_CREAT,S_IWRITE|S_IREAD);
	else
	  return(-100);
      }

      if( fct == 1 )  /* Popllist */
	 mfp=open(path,O_RDONLY);
    /*     mfp=open(path,O_RDWR);*/

	if( mfp != -1 ) /* fichier OK */
	{

	       if(fct == 0)   /* Pushllist */
	       {
			  sk = GetCurK(K);               /* sauve le curseur courant */
			  SetCurK(K,save);

			  do {
				cpt++;
				if( write(mfp,(const void *)save->data,K->lgr_data) )
				   ret = LOK;

				if(save->suiv == NULL)   /* permet de faire la derniere iteration */
				  last = LOK;
				else
				 {
				  save = save->suiv;
				  SetCurK(K,save);
				 }
			     } while(last == LNOK && ret == LOK);

			 SetCurK(K,sk);                  /* restore le curseur */
			 if(chx == 1)                    /* si decharge alors vide liste */
			    delete_list(p);


	       }
	       else
	       {
		  if(chx == 2 || chx == 3) /* si charge alors vide liste */
		    delete_list(p);

		 fbuff = (char *)allocation( K->lgr_data + 2 );

		 while ( read(mfp,(void *)fbuff,K->lgr_data) )
		     adjq(p,fbuff);

		 free(fbuff);
		  _nbfree_ +=(K->lgr_data + 2 );

		 if( chx == 1 || chx == 3 )
		    remove(path);

		 cpt = K->nbre_elmt;
	       }

	   close(mfp);
	   return(cpt);                    /* retourne le nombre iteration effectuee */

	}
	return(LSTOP);
    }
   return(ret);

}



/*
-----------------------------------------------------------------------------
- walk_sameD : parcours la liste uniquement pour les elmts identiques
-----------------------------------------------------------------------------
*/
int walk_sameD(LLIST **p,void *d,int (*fct1)(void *))
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt;
 int    ret;

   if(!fct1)
      return(L_NOFCT);

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK)
    {
	  cpt = 0;

	  sk = GetCurK(K);       /* sauve le curseur courant */
	  SetCurK(K,save);

	  lclef = GetlistIKL(K);

	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		{   
		    ret = fct1(save->data);
		    if(ret == LOK ) 
			cpt++;
                }
		
		if( ret == LOK   )
		{
		   if(save->suiv == NULL) /* permet de faire la derniere iteration */
		     last = LOK;
		   else
		     save = save->suiv;

		     SetCurK(K,save);
		}

	     }while((last == LNOK) && (ret == LOK));

	 SetCurK(K,sk);                  /* restore le curseur */

	 return(cpt);    /* retourne le nombre ide meme data  */
    }
   return(ret);
}

/*
-----------------------------------------------------------------------------
- walk_same : parcours la liste uniquement pour les elmts identiques
-----------------------------------------------------------------------------
*/
int walk_same(LLIST **p,void *d,int (*fct1)(LLIST **,void *))
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST  *save=*p;
 LLIST  *sk;
 int last = LNOK;
 int    cpt;
 int    ret;


   if(!fct1)
      return(L_NOFCT);

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK)
    {
	  sk = GetCurK(K);                       /* sauve le curseur courant */
	  SetCurK(K,save);

	  cpt = 0;

	  lclef = GetlistIKL(K);

	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		    ret = fct1(p,save->data);


		if( ret == LOK )
		{
		  cpt++;

		  if(save->suiv == NULL) /* permet de faire la derniere iteration */
		    last = LOK;
		  else
		    save = save->suiv;

		 SetCurK(K,save);
		}

	     }while((last == LNOK) && (ret == LOK));

	 SetCurK(K,sk);                  /* restore le curseur */

	 return(cpt);    /* retourne le nombre ide meme data  */
    }
   return(ret);
}





/*
------------------------------------------------------------------------------
int Cend: permet d'annule  le curseur
------------------------------------------------------------------------------
*/
int Cend(LLIST **l)
{
  LKERNEL *K;

  K = LLIST_exist(l);

  if(K)
  {
     SetCurK(K,NIL);
     return(LOK);
  }

 return(NLD);
}




/*
------------------------------------------------------------------------------
int   GFIRSTWC :recherche le 1er element qui correspond a la cles
------------------------------------------------------------------------------
*/
int   GFIRSTWC(LLIST **p,void *d,int Cur,int CurDel)
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST  *save=*p;
 int last = LNOK;
 int    ret;

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK )
    {
          if( CurDel == LOK ) 
	         LunsetCursor(p);

	  lclef = GetlistIKL(K);

	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		{
		  if(Cur == LOK )
			SetCurK(K,save);

		  memcpy(d,save->data,(size_t)K->lgr_data);
		  return(LOK);
		}

		if(save->suiv == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
		else
		  save = save->suiv;

	     }while((last == LNOK) && (ret == LOK));

	 return(LNOK);    /* retourne le nombre ide meme data  */
    }
   return(ret);
}


/*
------------------------------------------------------------------------------
int   GLASTWC(LLIST **p,void *d,int Cur) :
------------------------------------------------------------------------------
*/
int   GLASTWC(LLIST **p,void *d,int Cur,int Curdel)
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST     *save;
 int last = LNOK;
 int    ret;

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK )
    {
	  lclef = GetlistIKL(K);
	  save = (LLIST *)K->last;
	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		{
		  if(Cur == LOK )
			SetCurK(K,save);

		  memcpy(d,save->data,(size_t)K->lgr_data);
		  return(LOK);
		}

		if(save->precd == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
		else
		  save = save->precd;

	     }while((last == LNOK) && (ret == LOK));

	 return(LNOK);    /* retourne le nombre ide meme data  */
    }
   return(ret);
}



/*
------------------------------------------------------------------------------
void *PGlastwc:
------------------------------------------------------------------------------
*/
void *PGLASTWC(LLIST **p,void *d,int Cur,int CurDel)
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST     *save;
 int last = LNOK;
 int    ret;

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK )
    {
          if( CurDel == LOK ) 
	         LunsetCursor(p);

	  lclef = GetlistIKL(K);
	  save = (LLIST *)K->last;
	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		{
		  if(Cur == LOK )
			SetCurK(K,save);

		  memcpy(d,save->data,(size_t)K->lgr_data);
		  return(save->data);
		}

		if(save->precd == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
		else
		  save = save->precd;

	     }while((last == LNOK) && (ret == LOK));
    }

return( NULL );
}


/*
------------------------------------------------------------------------------
void *PGfirstwc:
------------------------------------------------------------------------------
*/
void *PGFIRSTWC(LLIST **p,void *d,int Cur,int CurDel)
{
 LKERNEL      *K;
 LLIST   **lclef;
 LLIST  *save=*p;
 int last = LNOK;
 int    ret;

    ret = TestListelmt(&K,p,d);

    if ( ret == LOK )
    {
         if( CurDel == LOK )
             LunsetCursor(p);

	  lclef = GetlistIKL(K);

	  do {
		if( datacompare(lclef,K,save->data,d) ==  LEQUAL )
		{
		  if(Cur == LOK )
			SetCurK(K,save);

		  memcpy(d,save->data,(size_t)K->lgr_data);
		  return(save->data);
		}

		if(save->suiv == NULL) /* permet de faire la derniere iteration */
		  last = LOK;
		else
		  save = save->suiv;

	     }while((last == LNOK) && (ret == LOK));
    }

return(NULL);
}

/*
------------------------------------------------------------------------------
int ad_round:
------------------------------------------------------------------------------
*/
int lad_round(LLIST **p,void *elmt)
{
    LKERNEL       *K;  /* info sur la liste */

    K = LLIST_exist(p);

    if( K )
    {

	if( K->nbre_elmt == 0 )
	    return( adjq(p,elmt) );

	if( (K->nbre_elmt & 1) == 1 )
	    return( adjq(p,elmt) );
	else
	    return( adjt(p,elmt) );
    }

  return(NLD);
}

/*
#ifdef _MEM_PC_

//
//****--------------------------------------------------------------------------
//int PCmemcmp:  comparaison pour PC car les donnees sont stockees en ordre
//	       inverse en memoire.
//****--------------------------------------------------------------------------
//
static int PCmemcmp(const void *s1, const void *s2, size_t n)
{
  unsigned char *c1,*c2;
  int  taille;
  int  ret;

  if( n == 1 )
      return( memcmp(s1,s2,n) );

  c1 = ( unsigned char *)s1;
  c2 = ( unsigned char *)s2;

  ret = 0;
  taille = n-1;
  for( taille = n-1; ((ret == 0) && (taille >=0)) ;taille--)
    ret = memcmp( &c1[taille] , &c2[taille] , sizeof(char) );
// OU   ret =  c1[taille] - c2[taille];    

  return(ret);
}

#endif

*/


/*
****---------------------------------------------------------------------------
compare des types double
****---------------------------------------------------------------------------
*/
static int memDOUBLE(CLKEY *pclkey,const void *s1, const void *s2)
{
  int ret = LEQUAL;
  int off = 0;
  int len = 0;
  
  double *_Id1=(double *)NULL;
  double *_Id2=(double *)NULL;


   if( pclkey && s1 && s2)
   {
      len = pclkey->len / LDOUBLE;


      for(off=0 ;off < len ; off++)
      {
        _Id1 = ((double *)s1);
	_Id2 = ((double *)s2);


	    switch( pclkey->kSense )
	    {
		    default:
		case LSMALL: // croissant
			       if( _Id1[off] > _Id2[off] ) 
				   return( LBIG );

				if( _Id1[off] < _Id2[off] )
				   return( LSMALL );
			    break;

		case LBIG:   // decroissant		  
			    if( _Id1[off] > _Id2[off] ) 
				return( LSMALL );

			    if( _Id1[off] < _Id2[off] )
				return( LBIG );
			break;		  

	    }

      }

   }

 return(ret);  
}




/*
****---------------------------------------------------------------------------
compare des types FLOAT
****---------------------------------------------------------------------------
*/
static int memFLOAT(CLKEY *pclkey,const void *s1, const void *s2)
{
  int ret = LEQUAL;
  int off = 0;
  int len = 0;
  
  float  *_If1=(float  *)NULL;
  float  *_If2=(float  *)NULL;


   if( pclkey && s1 && s2)
   {
      len = pclkey->len / LFLOAT;


      for(off=0 ;off < len ; off++)
      {
        _If1 = ((float *)s1);
	_If2 = ((float *)s2);


	    switch( pclkey->kSense )
	    {
		    default:
		case LSMALL: // croissant
			       if( _If1[off] > _If2[off] ) 
				   return( LBIG );

				if( _If1[off] < _If2[off] )
				   return( LSMALL );
			    break;

		case LBIG:   // decroissant		  
			    if( _If1[off] > _If2[off] ) 
				return( LSMALL );

			    if( _If1[off] < _If2[off] )
				return( LBIG );
			break;		  

	    }

      }

   }

 return(ret);  
}


/*
****---------------------------------------------------------------------------
compare des types LONG
****---------------------------------------------------------------------------
*/
static int memLONG(CLKEY *pclkey,const void *s1, const void *s2)
{

  int ret = LEQUAL;
  int off=0;
  int len=0;

  long *_Il1=(long  *)NULL;
  long *_Il2=(long  *)NULL;


   if( pclkey && s1 && s2)
   {
      len = pclkey->len / LLONG;


      for(off=0 ;off < len ; off++)
      {
        _Il1 = ((long *)s1);
	_Il2 = ((long *)s2);

        switch( pclkey->kSense )
	{
	       default:
	   case LSMALL: // croissant
	               if( _Il1[off] > _Il2[off] ) 
               		 return( LBIG );

	               if( _Il1[off] < _Il2[off] )
		         return( LSMALL );
                  break;

           case LBIG:   // decroissant		  
	               if( _Il1[off] > _Il2[off] ) 
  		          return( LSMALL );

	               if( _Il1[off] < _Il2[off] )
		          return( LBIG );
	           break;		  

        }

      }

   }

 return(ret);  

}

/*
****---------------------------------------------------------------------------
compare des types SHORT
****---------------------------------------------------------------------------
*/
static int memSHORT(CLKEY *pclkey,const void *s1, const void *s2)
{
  int ret = LEQUAL;
  int off=0;
  int len=0;

  short *_Is1=(short *)NULL;
  short *_Is2=(short *)NULL;


   if( pclkey && s1 && s2)
   {
      len = pclkey->len / LSHORT;


      for(off=0 ;off < len ; off++)
      {
        _Is1 = ((short *)s1);
	_Is2 = ((short *)s2);


        switch( pclkey->kSense )
	{
	       default:
	   case LSMALL: // croissant
	               if( _Is1[off] > _Is2[off] ) 
               		 return( LBIG );

	               if( _Is1[off] < _Is2[off] )
		         return( LSMALL );
                  break;

           case LBIG:   // decroissant		  
	               if( _Is1[off] > _Is2[off] ) 
  		          return( LSMALL );

	               if( _Is1[off] < _Is2[off] )
		          return( LBIG );
	           break;		  

        }


      }

   }

 return(ret);  

}

/*
****---------------------------------------------------------------------------
compare des types INTEGER
****---------------------------------------------------------------------------
*/
static int memINT(CLKEY *pclkey,const void *s1, const void *s2)
{
  int ret = LEQUAL;
  int off=0;
  int len=0;
  int *_Ii1=(int *)NULL;
  int *_Ii2=(int *)NULL;


   if( pclkey && s1 && s2)
   {
      len = pclkey->len / LINT;


      for(off=0 ;off < len ; off++)
      {
        _Ii1 = ((int *)s1);
	_Ii2 = ((int *)s2);


	    switch( pclkey->kSense )
	    {
		    default:
		case LSMALL: // croissant
			       if( _Ii1[off] > _Ii2[off] ) 
				   return( LBIG );

				if( _Ii1[off] < _Ii2[off] )
				   return( LSMALL );
			    break;

		case LBIG:   // decroissant		  
			    if( _Ii1[off] > _Ii2[off] ) 
				return( LSMALL );

			    if( _Ii1[off] < _Ii2[off] )
				return( LBIG );
			break;		  

	    }

      }

   }

 return(ret);  
}





/*
****---------------------------------------------------------------------------
Comparaison typee de donnee
****---------------------------------------------------------------------------
*/
static int memcmp_by_typ(CLKEY *pclkey,const void *s1, const void *s2)
{
  int ret = LEQUAL;

 if( pclkey && s1 && s2)
 {

    switch(pclkey->typ)
    {
       case _SHORT:
                    ret = memSHORT(pclkey,s1,s2);
                  break;
       case _INT:
       case _INTD:
                    ret = memINT(pclkey,s1,s2);
                  break;
       case _LONG:  
                    ret = memLONG(pclkey,s1,s2);
                 break;
       case _DOUBLE:
                    ret = memDOUBLE(pclkey,s1,s2); 
                 break;
       case _FLOAT: 
                    ret = memFLOAT(pclkey,s1,s2);
		 break;   
       default:
       case   _CHAR:
       case _STRING:     
                    ret = memcmp(s1,s2,(size_t)pclkey->len); 
                   break;

    }


 }


 return(ret);
}




/*
-------------------------------------------------------------------------------
Move current to last
-------------------------------------------------------------------------------
*/
int lmvctol(LLIST **p)
{
   LKERNEL *K;
   LLIST *cu;
   int ret;

   ret = TestList(&K,p);

   if(ret == LOK)
   {

     if( K->nbre_elmt == 0 ) 
         return(LNOK);

      cu = (LLIST *)K->current;
      if( cu ) 
      {
           if( K->nbre_elmt == 1 ) 
              return(LNOK);

           if( cu == (LLIST *)K->last)
	      return(LNOK);

            if( adjq(p,cu->data) == LOK ) 
	      return(Cdel(p));
       }
	else
	  return(NPC);
   }

return(ret);
}

/*
-------------------------------------------------------------------------------
Move current to first
-------------------------------------------------------------------------------
*/
int lmvctof(LLIST **p)
{
   LKERNEL *K;
   LLIST *cu;
   int ret;

   ret = TestList(&K,p);

   if(ret == LOK)
   {

     if( K->nbre_elmt == 0 ) 
         return(LNOK);

      cu = (LLIST *)K->current;
      if( cu ) 
      {
           if( K->nbre_elmt == 1 ) 
              return(LNOK);

           if( cu == (LLIST *)K->first)
	      return(LNOK);

            if( adjt(p,cu->data) == LOK ) 
	      return(Cdel(p));
       }
	else
	  return(NPC);
   }

return(ret);
}

 /*
-----------------------------------------------------------------------------
- FCnext: Deplace vers la droite curseur .     -
          supprime le curseur a la fin
-----------------------------------------------------------------------------
*/
int FCnext(LLIST **p)
{
    int ret;
    LKERNEL  *K;                        /* info sur la liste */
    LLIST *l;

    ret = TestList(&K,p);

    if( ret == LOK)
    {
	l =(LLIST *)K->current;

	if(l)
	{
	    if( K->current == K->last ) 
	    {
		LunsetCursor(p);
		return(LNOK);
	    }

	    if(l->suiv)
	    {
		SetCurK(K,l->suiv);
		return(LOK);
	    }

	   return(LNOK);
	}
	return(NPC);
    }
    return(ret);
}

/*
-----------------------------------------------------------------------------
- FCprev: deplace vers la gauche le curseur .                                -
-----------------------------------------------------------------------------
*/
int FCprev(LLIST **p)
{
    int ret;
    LKERNEL *K;                        /* info sur la liste */
    LLIST *l;

    ret = TestList(&K,p);

    if( ret == LOK )
    {
	l =(LLIST *)K->current;
	if(l)
	{
	    if( K->current == K->first ) 
	    {
		LunsetCursor(p);
		return(LNOK);
	    }

	    if( l->precd)
	    {
		SetCurK(K,l->precd);
		return(LOK);
	    }
	    return(LNOK);
	}
	return(NPC);
    }
    return(ret);
}


/*
-------------------------------------------------------------------------------
Deplace vers la gauche le curseur en fct de la clé si on trouve
-------------------------------------------------------------------------------
*/
int FCnextw(LLIST **p,void *d)
{
   if( Rnextw(p,d) == LOK ) 
      return(LOK);

   LunsetCursor(p);

return(LNOK);
}

/*
-------------------------------------------------------------------------------
Deplace vers la droite le curseur en fct de la clé si on trouve
-------------------------------------------------------------------------------
*/
int FCprevw(LLIST **p,void *d)
{
   if( Rprevw(p,d) == LOK ) 
      return(LOK);

   LunsetCursor(p);

return(LNOK);
}


/*
-------------------------------------------------------------------------------
Permet de savoir si le curseur courant est placé
-------------------------------------------------------------------------------
*/
int ForeachListTestw(LLIST **p,void *d)
{
    int ret;
    LKERNEL *K;                        /* info sur la liste */
    ret = TestList(&K,p);

    if( ret == LOK )
    {
       if( K->current )
          return(LOK);

    }

return(LNOK);
}


/*
-------------------------------------------------------------------------------
Permet de savoir si le curseur courant est placé et retourne le pointeur sur data
-------------------------------------------------------------------------------
*/
void *ForeachListTestwp(LLIST **p,void *d)
{
   if( ForeachListTestw(p,d) == LOK ) 
          return(Pelmt(p) );

return( (void *)NULL);
}

