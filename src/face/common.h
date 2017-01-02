# ifndef _COMMON_H_
# define _COMMON_H_

# include "list.h"
# include "koi9.h"
# include "window.h"

// ---------- редактор строки ---------------------------------
struct t_EditLine1
{
   short     x1,y1,x2 ; // координаты строки на экране
   short     BeginX,Cursor,Length,MaxLength ;
   char      Color ;    // цвет строки
   char      REG ;      //вставка-замена
   char      fMouseExit ;// выходить ли по мышке
   t_myChar  Str[10000],alt_Str[10000] ;

             t_EditLine1();   
   void      set_pos( short x1, short y, short x2 );
   void      paint();
   e_WinMsg  loop( short ZZ, ushort key1, ushort key2 );
   e_WinMsg  main_loop( );
   void      set_text( t_myChar *str );
   void      set_text( char *str );
   t_myChar *get_text(  );
   char     *get_ctext( );
   void      insert_text( char *Str1 );
   void      insert_text( t_myChar *Str1 );
   void      insert_text( short z, t_myChar *Str1 );
   void      insert_text( short z, char *Str1 );
   void      fdelete( short z, short z1 );

   // ----- дополнительные функции -----
   void  word_left(  );
   void  word_right(  );
   void  to_plain(  );
};

// --------- список выбираемых элементов ----------------------
//           (структур, частей речи, параметров и тп) 
struct t_View
{  short index ;
   char  Str[100] ;
};
DECLARE_LIST_TYPE( t_View,t_ViewList );

// ------ списочек, из которого можно выбрать строчку ---------
class t_ViewListSelect : public t_Win
{
public:   
   short      PY,Begin,Begin_X,size_y ;
   t_ViewList ViewList ;

          t_ViewListSelect();
	 ~t_ViewListSelect();
   void   calc_pos();
   void   paint( void );
   long   loop( short _PY=0 );
   void   clr( );
   void   add( t_View &V );
};


void  paint_cursor( short y, short x );
void  message( char *str );
void  figna( char *str );
short fragen( char *str );
void  paint_hotkey( short y, short x, const char *Str, short x2=-1 );
void  paint_header( char *Str );


short kommand_yxf1( short y, short x, char **str, short n_str );
e_WinMsg kommand_yxf( short y, short x, char f, char *str, short lenght, char EIN );
e_WinMsg kommand32_yxf( short y, short x, char f, t_myChar *str, short lenght, char EIN,
		   t_myChar **str1=NULL, short n_str1=0 );


# endif
