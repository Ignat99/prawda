/*******************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                   ���� ����� ���� � ���������
//      �������� �.�.                               �������
/*******************************************************************/
# ifndef WORD_H
# define WORD_H

# include "mylib.h"
# include "grammar.h"
# include "common.h"
# include "window.h"

// --------- �������� ������, ���������� �� ��������� ������ ----
class t_EditLine2 : public t_EditLine1
{
public:
   void   insert_struct( char TO, char *Str );
   void   insert_part( char *Str );
   void   insert_all( char fTo, char *Str1 );
   void   insert_param( char *Str );
   char   get_tag( t_String &Tag );
   void   to_plain( );
   void   double_str( );
   void   set_perevod( char *Str1 );
   void   insert_perevod( char *Str1 );
};

// ---------- ������� ������ ��������� ������� ��������� -------
struct t_WordTree 
{  long  up ;
   short index ;
   short i_word,i_last_word ;
   short i_struct ;
   char  end ;    // �������������� ����� ������ �������
};
DECLARE_LIST_TYPE( t_WordTree,t_WordTreeList );

// ---------- ������ ��������� ������� ��������� ---------------
struct t_Wizard1
{
   char      *Variants ;
   short      n_struct ;     // ����� �������� n_struct=Grammar.Trans.j
   short      n_word ;       // ����� ���� � �������������� n_word=From.j
   t_strList  From ;         // ������ ���� ��������������
   t_WordTreeList WordTree ; // ������ ������� ���������

   void     make_from( char *Str );
   void     make_main_table( char fTo );
   char     tst_struct( short i_struct, short i_word );   
   void     string_of_variant( char fTo, short i_struct, long i_tree, t_String &Str );
   void     make_tree_main( t_Struct *S, short i_struct );
   void     make_tree( t_Struct *S, long up, short i_word, short index );
};

// --------- ����������� ��������� --------------------------------
class t_WordWizard : public t_Win, public t_Wizard1
{
public:   
   t_EditLine2 EditWord ;    // �������� ��������� ������
   char       *ErrStr ;

   t_ViewListSelect Select ;

public:
            t_WordWizard();
   void     set_pos( short _x1, short _y1, short _x2, short _y2 );
   void     set_pos( t_Rect &R );
   e_WinMsg main_loop(  );
   e_WinMsg loop( short ZZ, ushort key1, ushort key2 );

   char     in_dst( );
   void     paint( void );
   void     word_of_struct( char fTo, short i_struct, t_String &Ant );
   // ------- ������������� ������� ---------------
   void     make_struct();
   void     struct1( );
   void     struct2( );
   void     part( );
   void     part_all( );
   void     param( );
};

# endif

