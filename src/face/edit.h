/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                  ����������� ����� ��������� ��������
//     �������� �.�.                                         �������
/***************************************************************************/

# ifndef EDIT_H
# define EDIT_H

# include <stdio.h>
# include <stdlib.h>
# include "window.h"
# include "koi9.h"

# define T_FRASAEND  1
# define T_ABSATZEND 2
# define T_THISFRASA 3

enum E_Function {
   F_Nop,
   F_Exit,
   F_Find,
   F_Replace3,   
   F_GoToLine,
   F_Up,
   F_Down,
   F_Left,
   F_Right,
   F_Home,
   F_End,
   F_PageUp,
   F_PageDown,
   F_BeginText,
   F_EndText,
   F_MUp,
   F_MDown,
   F_MLeft,
   F_MRight,
   F_MHome,
   F_MEnd,
   F_MPageUp,
   F_MPageDown,
   F_MBeginText,
   F_MEndText,
   F_Cat,
   F_Copy,
   F_IncCopy,
   F_Paste,
   F_Insert,
   F_BackSpase,
   F_Delete,
   F_DelStr,
   F_ShiftRight,
   F_ShiftLeft,
   F_PrevFrasa,
   F_NextFrasa,
   F_Mark,
   F_Unmark,
   F_ScrollUp,
   F_ScrollDown,
   F_Menu
};

/* --------------------------------- ����� ������ � ������ --------------- */
class t_Text
{
public: // �� ������ ������ ��� ����������
   t_myChar *Text ;         // ������� ������ ���������� � ������
   long      Length ;       // ������ ������, ����������� � ������
private:
   long      i_str,i_pos ;  // ������� ������ � �� �������
   long      n_String ;      // ����� ����� � ������� Text
public:
	  t_Text( void );
	 ~t_Text( void );
   void       init  ( char *Text );         // ������������ ����� �������
   void       init16( ushort *Text );
   void       init32( t_myChar *Text );
   void       clear( void );
                                   // � e_Latin ���� �������� �������� ��� ������ 
   void       write_file( char *name, e_Latin CodePage=KOI8 ); // �������� ����� � ����
   char       read_file(  char *name, e_Latin CodePage=KOI8 ); // ��������� ���� �� �����
   t_myChar & text( long i );
   t_myChar * text_all( );
   long       length( );
   long       n_string( );
   void       set_reper( long i_Str );
   void       reset_reper( );
   long       pos_str( long i_Str );              // ������ ������ i_Str
   t_myChar  *get_str( long i_Str );                 // �������� �� ������ ������ ����� i_Str
   t_myChar  *get_str( long i_Str, t_myChar *Buf );      // �������� ������ ����� i_Str � �����
   void       put_str_ipos( long i_pos, t_myChar *str ); // �������� � ����� ������ str
   void       put_str_istr( long i_str, t_myChar *str ); // �������� � ����� ������ str
   long       get_text1( long Str1, long Str2, t_myChar *List ); // �������� �� ������ �����
   long       get_text2( long Str1, long Str2, t_myChar *List ); // �������� �� ������ ����� ��� ��������
   void       put_text1( long Str1, t_myChar *List, long l_List );
   void       put_text1p( long Pos1, long Pos2, t_myChar *List ); // �������� ����� ������
   void       insert_str( long PY );      // �������� ������ ������
   void       delete_str( long PY );      // ������� ������ PY
   short      max_length_str( void );
   t_myChar*  get_frasa32( long pos );
   void       put_frasa32( long pos, t_myChar *Frasa );
   char*      get_frasa  ( long pos );
   void       put_frasa  ( long pos, char *Frasa );
   long       next_frasa( long pos );
   long       prev_frasa( long pos );
   long       begin_frasa( long pos );
   long       end_frasa( long pos );
   t_myChar*  get_word_punkt32( long pos );
   char*      get_word_punkt  ( long pos );
   long       prev_word( long z );
   long       next_word( long z );
   
   char      (*is_const)( t_myChar *Str ); // ������� ����������� "����� � ������"
   long      get_pos_xy( long py, long px );
   void      get_xy_pos( long pos1, long *_py, long *_px );
} ;

/* --------- ���������� �������� ---------------------------------------- */
class t_Edit : public t_Win
{
//protected:
public:
   t_Text *Text ;
protected:                   // !!! ������� private
   long      Begin_Y ;       /* ������ ������, ������� � ���� */
   short     Begin_X ;       /* ������ �������, ������� � ���� */
   short     PX ;            /* ��������� ������� */
   long      PY ;            /* ����� ������������� ������ */
   long      old_PY ;
   short     otm1,otm2,      /* ������ ��������� �������� */
             Otm1,Otm2 ;     /* ������ ��������� �������� (�������������) */
   long      str1,str2,      /* ������ ��������� ����� */
             Str1,Str2 ;     /* ������ ��������� ����� (�������������) */
   t_myChar  Str[256],       /* ������� ������ */
             Buf[256] ;
   char      fview ;         // ���� fview==1 - �� ������ ��������������
   char      fOvertype ;     /* �������-������ */
   char      Flag_b ;        /* ���� ������ ������ � ������� */   
public :
            t_Edit( );
 E_Function translate( short shift, ushort key1, ushort key2 );
   e_WinMsg main( void );
   e_WinMsg loop( short ZZ, ushort key1, ushort key2 );
   void     paint( void );                          // ���������� �����
   e_WinMsg int_to_msg( short i );
   void     raden_y3( );
   short    raden_Y( long PY );                   // �������� ��������� �� ������ ������� ������
   short    raden_X( short px );                  // �������� ��������� �� ������ ������� �������
   void     delet_str1( short px );               // ������� �� ������� ������ ������
   void     insert_str1( short px, ushort key );  // �������� � ������� ������ ������
   short    beg_str( void );                      // ��������� ������ ������
   short    end_str( void );                      // ��������� ����� ������
   short    char_exist( short px );               // ���� �� � ������ �������
   short    key_WK( void );                       // ������� �� ������� Enter
   void     key_Delet( void );                    // ������� �� ������� Delete
   void     delet_str( short Otm1,short Otm2,char f=0 );   // ������� ����� � �����
   void     insert_str( short px );               // ������� ����� � ������� ������ (������� ���������)
   // ----- ����� ������� -------------
   void     insert_str( t_myChar *Str );         // �������� ����� � ������� �������
   void     cursor_right( );                     // ��������� ������ ������
   void     screen_up();                         // ��������� ����� �����
   void     screen_down();                       // ��������� ����� ����
   void     screen_set_y( long Begin_Y );        // ���������� ������ ������ ������
//   void     set_py( long  _PY );
//   void     set_px( short _PX );

   void     find_forward( void );
   void     find_backward( void );
   void     goto_str( void );                     // ����� � �������� ������
   void     str_shift( char R );                  // ����� ���������� �������
   short    dialog_position( short size_y );      // ��������� ������� ������� 
   char     find1( void );
   char     find( char Richt, char     *sFind );
   char     find( char Richt, t_myChar *sFind );
   void     replace( void );                      // ���������������� ��� �������
   void     replace1( t_myChar *sFind, t_myChar *sReplace );
   void     replace1( char *sFind, char *sReplace );
   void     replace3( void );
   void     set_text( t_Text *Text );
   void     init  ( char     *_Text );
   void     init32( t_myChar *_Text );
   void     set_view( char f );                   // ���������� ���� "������ ������"
   t_Text  *text();                               // !!! ��� ������� �������, ������ ��� ������� get_all_text
   t_myChar *text_all( );
   t_myChar *get_Buf();
   void     set_Buf( t_myChar *Str );
   void     write( char *Name, e_Latin CodePage=KOI8 );
   void     read(  char *Name, e_Latin CodePage=KOI8 );
//protected:
   void     paint_body( );                           // ���������� �����
   void     get_str( void );                         // �����    Str �� ������
   void     put_str( void );                         // �������� Str � �����
} ;

/* ------------------------ ���, ��� ��������� � ������ ������ � ������ -- */
class t_Find
{  public :
   t_myChar  sfind[256] ;   // ������, ������� ���� �����
   t_myChar  sreplace[256] ;// ������, �� ������� ���� ��������
   t_myChar *xlat ;         // ������������ ������� � ��������� ���� (���� ������ ����� ���9)
   t_myChar  Mass[2560] ;   // ���������� ������
   t_myChar *prev_find[10] ;// ���������� ������
   char  fxlat,         // 0-��������� 1-�� ��������� ������� � ��������� �����
	 fcontinue,     // 1-���������� ����� ������� (��� � �����) �����
	 fdirect,       // ����������� ������ 0-������ 1-�����
	 falles ;       // 1-�������� ��� ��������� �� ������� ��������

	     t_Find( void );
   e_WinMsg  loop( short y0 );
   void      paint( short y0, short punkt );
} ;

# endif
