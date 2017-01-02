/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//            библиотека интерфейса с текстовым терминалом
//     Анисимов Д.В.                               сегодня
/*******************************************************************/
# ifndef SCHONE_H
# define SCHONE_H

# include <sys/types.h>

# include "koi9.h"
/************** Определение клавиш ************************/

/* --------------- Функциональныx ---------------- */
   #define S_key_F1   59
   #define S_key_F2   60
   #define S_key_F3   61
   #define S_key_F4   62
   #define S_key_F5   63
   #define S_key_F6   64
   #define S_key_F7   65
   #define S_key_F8   66
   #define S_key_F9   67
   #define S_key_F10  68
   #define S_key_F11  69
   #define S_key_F12  70

/* ---------- Управляющих ------------------------ */
   #define S_key_Esc    27
   #define S_key_Enter '\r'
   #define S_key_TabR   '\t'
// #define S_key_TabL   5
   #define S_key_Back   8
   #define S_key_Ins    82
   #define S_key_Del    83
   #define S_key_Home   71
   #define S_key_End    79
   #define S_key_PgUp   73
   #define S_key_PgDn   81
   #define S_key_Left   75
   #define S_key_Right  77
   #define S_key_Up     72
   #define S_key_Down   80
   #define S_key_space ' '

/* ------------------ служебных ------------------ */
   #define S_Shift_L  0x20
   #define S_Ctrl_L   0x10
   #define S_Alt_L    0x08
   #define S_Alt_R    0x04
   #define S_Ctrl_R   0x02
   #define S_Shift_R  0x01

   enum e_sEvent
   {  KEY_PRESS,
      KEY_RELEASE,
      MOUSE_PRESS,
      MOUSE_RELEASE,
      MOUSE_MOVE 
   };

   extern short n_Line,n_Col ;

/* ----------------------------------------------------------------------- */
   void   s_begin_schone( void );
   void   s_begin_schone( int argc, char *argv[] );
   void   s_end_schone( void );
   void   s_nacht( void );
   void   s_set_size( short sx, short sy );
   void   s_get_size( short &sx, short &sy );
   short  s_get_sx( void );
   short  s_get_sy( void );

   void   s_text_yxf ( short y, short x, uchar f, char *str );
   void   s_text_yxf1( short y, short x, uchar f, char *str );
   void   s_text_yxfl( short y, short x, uchar f, short L, char *str );
   void   s_text_yx  ( short y, short x, char *str );

   void   s_text32_yxf(  short y, short x, uchar f, t_myChar *str );
   void   s_text32_yxf1( short y, short x, uchar f, t_myChar *str );
   void   s_text32_yxfl( short y, short x, uchar f, short L, t_myChar *str );

   void   s_color_yxt( short y, short x, uchar *t, char *str );
   void   s_foreground_yxt( short y, short x, uchar *t, char *str );

   void   s_rame1_f( short y1, short x1, short y2, short x2, uchar f );
   void   s_rame1_F( short y1, short x1, short y2, short x2, uchar f );
   void   s_rame2_f( short y1, short x1, short y2, short x2, uchar f );
   void   s_rame2_F( short y1, short x1, short y2, short x2, uchar f );

   void   s_save     ( short y1, short x1, short y2, short x2 );
   void   s_save_rame( short y1, short x1, short y2, short x2 );
   void   s_restore( void );

   void   s_redraw( void );
   void   s_redraw_str( short y );

   short  s_clear_cursor( void );
   short  s_set_cursor( short S );
   void   s_goto_xy( short y,short x );
   void   s_set_mouse_move( char f );
   e_sEvent  s_getch  ( unsigned short *, unsigned short *);
   e_sEvent  s_getch16( unsigned short *, unsigned short *);
   short  s_get_mouse( short *x, short *y );
   char   s_break( );

   int    s_shiftstatus( void );
   void   s_set_font( char *Name );
   char  *s_get_font( void );
   void   s_refresh( void );
   void   s_set_ruskomb( short Komb );
   char   s_run();
   char   s_x_version( );

   void   s_debug( );

# endif
