/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                             менюшка
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# ifndef MENU_H
# define MENU_H

enum e_Menu
{  F_ENDE  =0 ,  // признак конца массива меню
   F_SCHREI=1 ,  // надпись,
   F_MENU  =2 ,  // подменю
   F_FUNK  =3 ,  // функция
   F_NUMER =4    // возвр. значение
};

struct t_Menu
{  e_Menu flag ;
   char   ret ;             /* признак возврата 0-в голову, 1 - в пред. меню */
   void   (*funk)(void *) ; /* вызываемая функция */
   void   *data ;           /* параметры для функции или массив подменюшек */
   char name[30] ;          /* название меню */
} ;

void  Sorry( void *V );
void  Window_help( void *Void );

short   menu( t_Menu *Menu, short y=0 ) ;
short   menu_mouse( t_Menu *Menu, short y, short x );
void    null_menu( t_Menu *Menu, short y=0 );
//void    menu_set( char *sFind, char *sReplace );
char    menu_set( t_Menu *Menu, char *sFind, char *sReplace );
t_Menu* menu_find( t_Menu *Menu, char *sFind );
# endif



