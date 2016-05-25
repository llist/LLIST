/*
** modul         : pile.h
** type          : 
** version       : 1.13
** author        : patrice
** last update   : 04/11/26      15:31:54
** current date  : 16/05/25      15:43:13
** rel/lev/br/sq : 1    13    0    0 
**
** source file   : /MSP/D/sources/logis/sccs/incl/s.pile.h
** SCCS_File     : s.pile.h 
**
*/
#ident   "@(#)pile.h   1.13   04/11/26   15:31:54"

/*------------------------------------------------------------------------*/
/*                                                                        */
/* PILE.H                                                                 */
/*                                                                        */
/*  Copyright SCHOUMACHER Patrice 1995                                    */
/*  All Rights Reserved                                                   */
/*                                                          V1.1          */
/*------------------------------------------------------------------------*/

#include <llist.h>


#ifndef _PILE_
#define _PILE_




#define PILE  LLIST
#define POK   LOK
#define PNOK  LNOK

#define  initpile(p,a)    initlist(p,a,a)
#define  close_pile(p)    close_list(p)
#define  pileborn(p)      listborn(p)
#define  pilevide(p)      lvide(p)
#define  pile_exist(p,e)  Pdata_exist(p,e)

#define  empiler(p,e)     adjq(p,e)
#define  depiler(p)       corpi(p)
#define  sommet(p,e)      queue(p,e)
#define  Psommet(p)       Plast(p)
#define  hauteur(p)       lgrl(p)
#define  fifo_sommet(p,e) tete(p,e)
#define  fifo_depiler(p)  corps(p)

#define  SetKey(p,a)      SetlenKey(p,a)
#define  GetKey(p,a)      GetlenKey(p,a)

#endif
