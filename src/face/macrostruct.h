/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//     ������ � "������� ����������" ������
//     �������� �.�.                                         �������
/***************************************************************************/
# ifndef HIPERTEXT_H
# define HIPERTEXT_H

# include "list.h"
# include "edit.h"
# include "serial.h"

# define char16  unsigned short // ���������� ��������� ��������

// ------------ �����, ����� ������� ����� -------------------
struct t_String32
{
   t_myChar *Str ;
   short     Length ;
   
   t_String32();
   ~t_String32();
   void init();
   void del();
   void operator = ( t_String32 &S );
   void set( t_myChar *Str, long L=-1 );
   long strlen();
};
// ------------ ������� ����, ��� ����� ������������ ---------
enum e_Perevod
{
    NOT =0, // �� ������������
    AUTO=1, // �������������
    HAND=2  // ������
};
// ------------ ���� ��������� ���������� --------------------
struct t_TypeAbsatz
{
   // ������ ��������, ���������� � ������
   char       Color ;     // ���� � ��� ����� ���� �����
   char       not_trans ; // �������, ��� �� �������������
   char       Name[40] ;  // ��������
   char      *begin,*end ; // ��� ������ � ������ � ����� ������ (��������� malloc-��)
};
DECLARE_LIST_TYPE( t_TypeAbsatz,t_TypeAbsatzList );
// ------------ ����� ----------------------------------------
struct t_Frasa
{  long       begin ,end  ; // ����� ������, ���������� ������ (��������)
   t_String32 Str ;         // ����� ������, ���������� ������ (��������)
   e_Perevod  perevod     ; // ������� ����, ��� ��� ����� ������������ (���.���.������)
   char       flag1       ; // ����� ���������� �� ��������
   
   t_Frasa();
   ~t_Frasa();
   void init();
   void del();
   void operator = ( t_Frasa &F );
};
DECLARE_LIST_TYPE( t_Frasa,t_FrasaList );
// ------------ �������� -------------------------------------
struct t_Absatz
{
   char        type ;    // ��� ���������
   t_FrasaList Frasa ;   // ������ ���� ����� ���������
public:
   // format(???);       // �������������� � ������������ � �����
   t_Absatz();
   void init();
   void del();
   void operator = ( t_Absatz &A );
};
DECLARE_LIST_TYPE( t_Absatz,t_AbsatzList );
// ------------ ������������� ��������� ----------------------
struct t_HT_Sort
{  long i_absatz ;
   long i_frasa ;
   long begin1 ;
};
DECLARE_LIST_TYPE( t_HT_Sort,t_HT_SortList );

// ------------ ����������������� ����� ----------------------
       
struct t_Macrostruct : public t_Serial
{
   t_Text       *Text ;   // ����� (��������� �����������)
   t_AbsatzList  Absatz ; // ������ ������� (�������� ���� ������� ������)
   t_TypeAbsatzList TypeAbsatz ; // ���� �������
   char16       *begin,*end ; // ��� ������ � ������ � ����� ������
   long          j_Text ;
   t_HT_SortList Sort ;   // ������������� ������
   char          exist ;

   long  L_old ;
   
public:
         t_Macrostruct();
   void  print( char *File );
   void  print( FILE *fw );
   void  print_frasa( t_Frasa &Frasa );
   long  scanf( char *File );
   long  scanf_absatz( long z );
   long  scanf_frasa( t_Absatz &Absatz1, long z );
   long  scanf_absatz_flag( t_Absatz &Absatz1, long z );
   long  scanf_frasa_flag( t_Frasa &Frasa, long z );
   void  settext( char *Text );
   void  make( );
   char  was_maked( void );
   char  format_is_correct( void );
   void  del();
   void  clear();                   // ������ ��������� �� ��������� � �����.
   void  automake1();               // ������� �������������� ��������� �� ��������� � �����.
   void  automake2();               // ������� �������������� ��������� �� ��������� � �����.
   void  set_frasa_point( long i ); // ���������� ������� ����� �������.
   void  res_frasa_point( long i ); // ����� ������� ����� �������.
   void  set_absatz_point( long i ); // ���������� ������� ����� �����������
   void  res_absatz_point( long i ); // ����� ������� ����� �����������
   void  set_type_part( short Type, long Pos ); // ��������� ��� ���������
   long  get_frasa ( long i_Absatz, long i_Frasa, char *Str ); // ���� �����
   long  get_frasa2( long i_Absatz, long i_Frasa, char *Str ); // ���� ����� ��������
   void  put_frasa ( long i_Absatz, long i_Frasa, char *Str ); // �������� ������� �����
   void  make_text1();               // �������������� ������ ��������

   void  make_ht_sort( void );
   void  merge_absatz ( long i_absatz );
   void  merge_frasa  ( long i_absatz, long i_frasa );
   void  split_absatz ( long i_absatz, long i_frasa );
   void  split_frasa  ( long i_absatz, long i_frasa, long px );
   void  insert_absatz( long i_absatz );
   void  delete_absatz( long i_absatz );
   void  delete_frasa ( long i_absatz, long i_frasa );
   void  delete_sort  ( long i_sort );

   void  print_type_absatz( char *File );
   void  scanf_type_absatz( char *File );
   void  print_result( char *File );
   void  print_debug_str32( );

};

# endif
