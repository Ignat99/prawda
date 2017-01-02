/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                    "������������" ������� (�����)
//     �������� �.�.                                         �������
/***************************************************************************/
# ifndef SLOWO1_H
# define SLOWO1_H

# include <stdio.h>
# include <malloc.h>
# include <string.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "mylib.h"
# include "core.h"
# include "window.h"
# include "slowo.h"

/****************************************************************************/
class t_Slowo1 : public t_Win
{
   public:
   char     fView ;         // �������, ��� ������� �������� ����������� �����
   char     fDirect ;       // ������� ��������������� ������������ ����
   char     Str[40] ;       // ������, � ������� ���������� �����
   short    px ;            // ������� ������� � ������
   long     first_word ;    // � ������ ����� ���������� ����� ������
   long     i_word ;        // ������ �������� �����

   t_Rect   rect1 ;         // ���� ������ ����
   t_Rect   rect2 ;         // ���� ��������� ������
   short    x, y ;          // ��������� ������ � ���� ��������
   t_Slowo *Buchs ;         // ��������� ����
   short    i_Buch,
            n_Buch ;

   public:
            t_Slowo1( void );
   void     set_pos( short x1, short y1, short x2, short y2, char fDirect );
   e_WinMsg main_loop ( void );
   void     paint( long i_wort, long first_word );
   void     paint2( char *Ans, t_Rect rect, short x, short y );
   void     paint_d( long i_word1, long first_word1 );
   void     antwort( long i_wort );
// char     read_buch( char *Str );
   void     read_buchs( );
   void     select_buch( );
   void     set_str ( char *_Str );
   void     set_i_word( long _i_word );
   long     get_i_word();
   long     get_first_word();
   void     main_loop1_1( void );
   void     key_back( void );
   void     key_add( char key );
   char    *answer( );
   long     n_word( );
};

# endif
