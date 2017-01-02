/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//              ��������� ��� ���������� ������������ �������
//     �������� �.�.                                         �������
/***************************************************************************/
# ifndef KONTEXT_H
# define KONTEXT_H

# ifdef __WIN__
   # include <dir.h>
#else
   # include <unistd.h>
#endif
# include <sys/types.h>
# include <sys/stat.h>

# include "core.h"       // ��� ������ �� t_longList
# include "serial.h"

// ----------------- ������������� ����� --------------------------------
struct t_wWord
{  char *str ;
   long  i_slowo ;     // ������ � Perevod (���� ����)
   long  i_word ;      // ������ Word1<->Word0
   long  num ;         // ����� ����� ���������
   char  f_perevod ;   // ���� �� �������
};
DECLARE_LIST_TYPE( t_wWord,t_wWordList );

// ----------------- ���������� ��������� -------------------------------
struct t_wExemplar
{  long  i_struct ;    // ���� short
   long  index ;       // ������ � ������� t_wExemplarList
   long  i_word,       // ��������� � ������ Word0
         i_last_word ; // ��������� � ������ Word0
   long  i_word1 ;     // ��������� �� �����
   long  i_absatz,i_frasa ; // ��������� � �����������
   long  i_slowo ;
   long  i_type ;      // ������ ����
   char  Str[100] ;    // ��� �������� ���������
};
DECLARE_LIST_TYPE( t_wExemplar, t_wExemplarList );

// ----------------- ���� ��������� -------------------------------------
struct t_wType
{  short used ;        // 0 - ������������
                       // 1 - �� ������������
   short i_struct ;
   long  i_slowo ;     // ��� WORD TSTRUCT1 (type=0) - ������ � Perevod 
   long  i_word1 ;     // ������ � Word1 (������ ��� ����)
   long  i_exemplar,   // ������ �� ����������         t_Kontext->i_Exemplar
         i_last_exemplar ;
   long  num ;         // ���� short ����� ����� ���������
   long  n_hole ;      // ���� short ����� �������������� ����������� ����� ���������
   char  select ;      // ������� ����, ��� ����� ������� � ������ �������� �������
   char  Str[100] ;    // ��� �������� ��������� � �������
};
DECLARE_LIST_TYPE( t_wType, t_wTypeList );

// ----------------- ��� ������ � ��������� -----------------------------
struct t_Kontext : public t_Serial
{
   time_t          mtime ;    // ��������� ����� ����������� ��������
                              // ��������� ������ ��� - ���������� �������� hypertext
   char           *Word00 ;   // ������, ���� ��������� t_wWord->str
   t_wWordList     Word0 ;    // ������ ���� ������
   t_wWordList     Word1 ;    // ������ ���� ������ ������������� � ����������� ������
   t_wExemplarList Exemplar ; // ���������� ���������
   t_wTypeList     Type ;     // ���� ���������
   t_longList      i_Exemplar;// ������ � ������ Exemplar
   t_longList      Interes ;  // "����������" ��������� 0-������ ����������
   char            exist ;

   long  j_Word00 ;

         t_Kontext( );
   void  make( );
   void  make_word1( );
   void  make_exemplar( );
   void  make_type( );
   void  make_overlap( );
   void  make_interes_struct();
   void  make_default_perevod();
   void  make_current_perevod();
   void  double_type( long i_Type );
   void  delete_type( long i_Type );
   void  correct_i_type();
   void  calc_n_hole( );
   void  del( );
   void  test( );

   void  print_word( t_wWord &Word );
   void  print_exemplar( t_wExemplar &E );
   void  print_type( t_wType &T );
   void  print_all( FILE *fw );
   void  print_all( char *File );
   void  scanf_all( FILE *fr );
   void  scanf_all( char *File );
   void  scanf_word( t_wWordList &WL );
   void  scanf_exemplar( );
   void  scanf_type( );
   void  scanf_i_exemplar( );
   void  scanf_i_overlap( );
   void  restore_i_slowo( );
   void  restore_i_slowo1( );
};

# endif
