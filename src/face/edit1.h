/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                      Редактор, заточенный под перевод 
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# ifndef EDIT1_H
# define EDIT1_H

# include "edit.h"

class t_EditTrans : public t_Edit
{
public :
   t_Text Text1 ;

	 t_EditTrans( void );
        ~t_EditTrans( void );
   void  set_name( char *Name );
   char* get_frasa( long i );                    // !!! функции должны делать get_str()
   void  put_frasa  ( long i, char *Frasa );
   void  put_frasa32( long i, t_myChar *Frasa );
   void  step( short f );
   void  next_frasa( void );
   void  prev_frasa( void );
   char* get_word( long i );
   void  next_word( void );
   void  prev_word( void );   
   void  set_funk_const( char (*is_const)( char *Str ) );

   long  current_pos( void );
   e_WinMsg main_loop( void );
   e_WinMsg loop_format( void );
   e_WinMsg loop_frasa( void );
   e_WinMsg loop_help( char *File );

   void format_make();
   void format_clr();
   void format_frasa();
   void format_absatz();
   
   void  set_thisfrasa( long begin, long end );
   void  clr_thisfrasa( long begin, long end );   
};

# endif
