/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                      абстрактный класс окна
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# ifndef WINDOW_H
# define WINDOW_H

enum e_WinMsg
{  WM_NULL1,
   // -------- сообщения общего вида ------
   WM_NEXT,
   WM_PREV,
   WM_ESC,
   WM_OK,
   WM_HELP,
   // -------- переход в конкретные окна --
   WM_EDIT1,
   WM_EDIT2,
   WM_FIRST,
   WM_SRC,
   WM_DST,
   WM_DEBUG,
   WM_DEBUG1,
   WM_GRAMMAR,
   WM_SLOWO,
   WM_BASE,
   WM_BASE1,
   WM_SLOWO1,
   WM_ERROR,
   // -------- конкретная операция --------
   WM_RESIZE,
   WM_N_FRASA,
   WM_P_FRASA,
   WM_N_WORD,
   WM_P_WORD,
   WM_SELECT2
};
// ------- прямоугольник (для координат окон) ---------
struct t_Rect
{
   short x1,y1,x2,y2 ;

   char  inside( short x, short y );
};

class t_Win  // ----- абстрактный класс окна ----------
{
public:
   short x1,y1,x2,y2 ;
protected:
   short size_x,size_y ;
public:
   char  f_2Rame ;
   char  Name[100] ;
	  t_Win( void );
virtual	 ~t_Win( void );
virtual   void   set_pos( short x1, short y1, short x2, short y2 );
virtual   void   set_pos( t_Rect &Rect );
          t_Rect get_pos( );
          void   loop_rect( short yy );
virtual   void   set_name( char *Name );
virtual   void   paint( char Select=0 );
};

# endif
