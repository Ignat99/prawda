/*******************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                 ���������, �������, ������ � �.�.
//     �������� �.�.                               �������
/*******************************************************************/

# ifndef RESOURCE1_H
# define RESOURCE1_H

# include "mylib.h"
# include "video.h"
# include "list.h"
# include "lexer.h"
# include "window.h"
# include "menu.h"

# define       fon_Color  Option.Color[0]   // ���� ���� (� ������ besser)
# define      text_Color  Option.Color[1]   // �������� �����
# define       buf_Color  Option.Color[2]   // ����� ������ (��� ���������)
# define  red_rame_Color  Option.Color[3]   // �����
# define  red_Rame_Color  Option.Color[4]   // ����� �������� ����
# define     regim_Color  Option.Color[5]   // ��������� �������
# define      titl_Color  Option.Color[6]   // ��������� ����
# define  titlchen_Color  Option.Color[7]   // ��������� �������
# define    status_Color  Option.Color[8]   // (������-����) �������-������ � ������-�������
# define   hotkey1_Color  Option.Color[9]   // ������� ������� ����� ������ (�������)
# define   hotkey2_Color  Option.Color[10]  // ������� ������� ����� ������ (�������)
# define   hotkey3_Color  Option.Color[18]  // (����) ������� ������� �� �����

# define      menu_Color  Option.Color[11]  // ����
# define curs_menu_Color  Option.Color[12]  // ���� �������
                                // ---- ����� ��������� --------------
# define      help_Color  Option.Color[13]  // ���������
# define bold_help_Color  Option.Color[14]  // ���������� ����� (� ���������)
# define link_help_Color  Option.Color[15]  // �������� (� ���������)
                                // ------- ������ ����� --------------
# define     error_Color  Option.Color[16]  // ��������� �� ������
# define  cur_item_Color  Option.Color[17]  // ������� ������� ������-������ ������
# define  sel_item_Color  Option.Color[19]  // ���������� ������� ������-������ ������

// ---------- ��������� ������� ���������� (���� ������) ----------------
enum e_Function 
{
   // ------------- ����� �������� --------------------------
   F_NOP,                 // ��� ��������
   F_END_OF_LIST,         // ����� ������
   F_HELP,                // ���������
   F_MENU1,               // ����
   F_EXIT,                // ����� ��� ���������� ����������
   F_OKEY,                // ����� � ����������� ����������
   
   // ============= �������������� ==========================
   F_FORMAT_SET_FRASA,    // ���������� ������� �����
   F_FORMAT_SET_ABSAZ,    // ���������� ������� ������
   F_FORMAT_CLR,          // �������� ������� �����
   F_FORMAT_CLR_ALL,      // ������ ��� �������
   F_FORMAT_MAKE_ALL,     // ��������� ��� ������� �������������
   
   // ============= ����������� ������� (���� ������) =======
   F_DICT_USE,            // ����������/�������� ���� "������������"
   F_DICT_FORMAT,         // ������������� �����
   F_DICT_MAKE_DICT,      // ������� �������
   F_DICT_FIND,           // ����� � ������ ���������
   F_DICT_ENTER,          // ������� � ����� ������� �������� �����
   F_DICT_FILTER,         // ���������� ������� ��� ������ ����
   F_DICT_DEL,            // ������� ��������� ������
   F_DICT_INS,            // ����������� ��������� ������
   F_DICT_HOLE,           // ����������� ����� �������������� ����
   
   // ============= ����������� ������� (���� �����) ========
   F_WORD_EXAMPLE_NEXT,   // ���������  ������������� ����� ���������
   F_WORD_EXAMPLE_PREV,   // ���������� ������������� ����� ���������

   F_WORD_WORD_NEXT,      // ���������  ����� ������������
   F_WORD_WORD_PREV,      // ���������� ����� ������������

   F_WORD_KONTEXT_UP,     // �������� �� ��������� ������� ������������ ������� 
   F_WORD_KONTEXT_DOWN,   //
   F_WORD_KONTEXT_DELETE, //
   
   F_WORD_MAIN_UP,        // �������� �� ��������� ������� �������� ������� 
   F_WORD_MAIN_DOWN,      //
   F_WORD_MAIN_INSERT,    //
   F_WORD_MAIN_INSERT1,   //
   
   F_WORD_HUMAN_UP,       // ����� ������ ���� ������������� �������
   F_WORD_HUMAN_DOWN,     //  ���� ������ ���� ������������� �������
   
   F_WORD_WIZARD,         // ������� ��������� ������ �������� ���������
   F_WORD_GET,            // ����� ����� ������ �� ������������� �������
   F_WORD_SELECT_DICT,    // ����������� ��������� ����
   
   // ============= �������� ������� ========================
   F_DMGR1_MARK_UNKNOWN_WORD,  // �������� ��������� �����
   F_DMGR1_DEL_MARKED_WORD,    // ������� ���������� �����
   F_DMGR1_UNMARK_MARKED_WORD, // �������� ���������
   F_DMGR1_INVERT_MARKED_WORD, // ������������� ���������
   F_DMGR1_WRITE,              // �������� ������� � ����
   
   F_DMGR2_READ,               // ��������� �� �����
   F_DMGR2_CALC_PEREVOD,       // ������� ������� ����������� ����
   F_DMGR2_MARK_NEW_WORD,      // �������� ����� �����
   F_DMGR2_MARK_NEW_PEREVOD,   // �������� ����� �������� ��������
   F_DMGR2_UNMARK_MARKED_WORD, // �������� ���������
   F_DMGR2_INVERT_MARKED_WORD, // ������������� ���������
   F_DMGR2_COPY_WORD,          // �������� � ��������
   F_DMGR2_FIND,               // ����� �� ������� �������
   
   // ============= ������� ����� (���� ������) =============
   F_MACRO_UP,           // ��� �� ������ ���� �����
   F_MACRO_DOWN,         // ��� �� ������ ����
   F_MACRO_PG_UP,        // ��� �� ������ �� �������� �����
   F_MACRO_PG_DN,        // ��� �� ������ �� �������� ����
   F_MACRO_HOME,         // � ������ ������
   F_MACRO_END,          // � ����� ������
   F_MACRO_DEL,          // �������  ����� ��� �����
   F_MACRO_INS,          // �������� ����� ��� �����

   F_MACRO_LANG,     // ������������ "��������" - "�������"
   F_MACRO_ENTER,    // ������� � ���� "��������� �����"
   F_MACRO_LEFT,     // ����� ����� �����
   F_MACRO_RIGHT,    // ����� ����� ������

   F_MACRO_FLAG,     // ��������� ����� �����
   F_MACRO_MAKE,     // ��������� ����������
   F_MACRO_TRANS,    // ������� ����� �����������
   F_MACRO_TRANS1,   // ������� ����� �����

   // ============= ������� ����� (���� �����) ==============
   F_FRASA_SRC_UP,        // �������� �� ��������� ������
   F_FRASA_SRC_DOWN,

   F_FRASA_WORD_NEXT,     // ���������  ����� ������������
   F_FRASA_WORD_PREV,     // ���������� ����� ������������
   F_FRASA_WORD_VARIANT,  // ������� � ���� "�������� ���������� �����"
   F_FRASA_WORD_VARIANT1, // �������� ������ ������� � ����� "��� ����"
   F_FRASA_WORD_VARIANT2, // �������� ������ ������� � ����� ��������

   //------------ �������� �� ������������� ������� ---------------
   F_FRASA_HUMAN_UP,      // ����� ������ ���� ������������� �������
   F_FRASA_HUMAN_DOWN,    // ����  ������ ���� ������������� �������

   F_FRASA_FLAGS,
   F_FRASA_UNDO,
   F_FRASA_DEBUG,
   F_FRASA_NEXT,
   F_FRASA_PREV,
   F_FRASA_TRANS,
   F_FRASA_WINDOW_SWITCH,
   F_FRASA_GET,
   F_FRASA_TODICT,       // ������� � ����� ������� �������� ����� �����
   F_FRASA_SELECT_DICT   // ����������� ��������� ����
};

struct t_OptionParam
{  char Tag[50] ;
   char Value[200] ;
};
DECLARE_LIST_TYPE( t_OptionParam,t_OptionParamList );

// ---------- ������� ������� -----------------------------------------
struct t_HotKey
{
   short SStatus ;        // ����������, ������� ��� ���� ������ ����
   short key1, key2 ;     // �������, ������� ��� ���� ���� �������� (������� � ���������)
};

// ---------- �������� ��������� ������� (����� ��������) -------------
struct t_HotKey2
{
   t_HotKey    Key  ;
   e_Function  Func ;
   char        DefValue[20]; // �������� �� ��������� (� ����� - ��� ����������)
   char        MenuFind[30]; // ����� ������ ����� � ����
   char        HelpFind[20]; // ����� ������ ����� � ����� ���������
   char        Name[50] ;    // ��� ��������� � ����� ��������
};

struct t_Option
{
   t_OptionParamList  Param ;
   char               Color[20] ;
   t_Rect             Rect [10] ;

   int        read_all( char *File );
   int        read( char *File );
   char      *get( char *Tag );
   e_Function get_func( t_HotKey2 *HotKey, 
                        short Shift, short key1, short key2 );
   void       get_key( t_HotKey &Key, char *Str, short x );
   void       get_key( ushort *ZZ, ushort *key1, ushort *key2, char *Str, short x );
private:
   void   init_colors( );
   void   set_hotkeys( t_HotKey2 *HotKey, t_Menu *Menu );
   void   set_hotkeys( t_HotKey2 *HotKey, t_Menu *Menu, t_String &Help );
   void   set_color1( char &Color, char Default, char *Name );
   void   set_rect1( t_Rect &Rect, char *Name );
   short  tcolor( char *str );
   char   trect( t_Rect &Rect, char *Str );
   char   set_help_hotkey( t_String &Str, char *Funk, char *Key );
   char   set_menu_hotkey1( char *Menu, char *Key );
   char   set_menu_hotkey( t_Menu *Menu, char *sFind, char *HotKey );
   char   read_hotkey( t_HotKey2 &HotKey, char *Str );
   char   read_hotkey( t_HotKey &Hotkey, char *Str );
   void   set_hotkey0( char *Str );
   void   set_hotkey1( t_HotKey &Key, char *Str1 );
};

extern t_Option Option ;

# endif
