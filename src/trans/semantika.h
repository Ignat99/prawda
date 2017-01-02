/*************************************************************************/
//                 ������ ��� ������� ���������
//
/*************************************************************************/
# ifndef SEMANTIKA_H
# define SEMANTIKA_H

# include "list.h"
# include "grammar.h"
# include "slowo2.h"

DECLARE_LIST_TYPE( t_Shabl, t_ShablList );

// ------------- ������� -------------------------------------
struct t_Affix
{
   char  *Name ;     // ��� ������� 
   short  i_link ;   // ��� �����
   short  i_struct ; // ����� ���� (-1 - �����)
};
DECLARE_LIST_TYPE( t_Affix, t_AffixList );

// ------------- ����� ---------------------------------------
struct t_Root
{
   char  *Str ;      // ������ ������
   char  *Str2 ;     // ������ ������
};
DECLARE_LIST_TYPE( t_Root, t_RootList );

// ------------- ������ --------------------------------------
struct t_mLink
{
   short type ;      // ������ �� ������ ����� ������ t_Semantika->TypeLink
   long  i_word ;    // c ����� ������ �������
   long  i0_word ;   // ����� ����� ������� (������ ���� ������)
   char  *str1 ;     // ������� ������
                     // ������� �������� ������
   // ----------------- ����� ��� ������ -----------
   long  i_struct ;   // ����� ���� �����
//   char  *Name ;     // ��� ���� ������
   char  *str ;      // �����
   char  *remark ;   // ���������� �����
};
DECLARE_LIST_TYPE( t_mLink, t_mLinkList );

// ------------- ����� ---------------------------------------
struct t_mWord
{
   short  i_struct ;  // ������ ��������� �    t_Grammar->Trans
   char  *str ;       // ������ �� �����       t_Semantika->Mass,Mass1
   char  *remark ;    // ����������� (��� ���������� ��������)
   t_Form Param ;     // ����������� ���������
   long   i_link,     // ������ �� ������ ������ t_Semantika->Link
          n_link ;    // ����� ������
   long   i_shablon ; // ������ �������������
   char   type ;      // ��� - ����� �/��� ������
};
DECLARE_LIST_TYPE( t_mWord, t_mWordList );

/***************************************************************************/
//                   ���� ������������� ������
/***************************************************************************/
class t_Semantika
{
   char        FileName[40] ;
   t_Format4  *Format ;    // ������ �������

   char       *Mass ;      // ������, ����� ������� ��� ��������
   char       *Mass1 ;     // ������ ����� ������ (�� �����������������)
   long        n_Mass,n_Mass1,j_Mass1 ;
   char        EmptyStr[1] ;

public:
   t_ShablList    *Shabl ;     // ������� ������������� (����� �� slowo2.h)
                               // ������ ������� ����� ����� ������ ����
// t_TypeLinkList  TypeLink ;  // ���� ������
   t_AffixList     Prefix,
                   Suffix ;    // ��������� � ��������
   t_RootList      Root ;      // �����
   t_mLinkList     Link ;      // ������
   t_mWordList     Word ;      // �����
   t_Lexer         LShablon,   // �������
                   LTypeLink,
		   LPrefix,
		   LPostfix,
		   LRoot ;

   public:
         t_Semantika();
        ~t_Semantika();
   void  read( char *File, t_Format4 *Format );
   void  read_shablon ( char *Mass, long L );
   void  read_shablon1( char *Mass, long L );
   void  read_typelink( char *M, long L );
   void  read_affix ( char *Mass, long L, t_AffixList &Affix, char fpostfix );
   void  read_root   ( char *Mass, long L );
   void  read_word   ( char *Mass, long L );
   void  read_word1  ( t_mWord &Word, long i_word, char *Text, long &j_Link );
   void  reverse( void );

   void  write( char *File );
   void  print_param( FILE *fw,  t_mWord *Word );
   void  print_param( char *Str, t_mWord *Word );
   void  print_param( char *Str, t_mWord *Word, char empty_par=0 );
   void  print_link( FILE *fw, long i_word, t_mLink *L );
   
   long  find_TypeLink( char *Name );
   short find_value1( short i_struct, short i_param, char fdst, char *Str1 );
   // ------ �������� ���-������ ---------------
   void  add_word( t_mWord &Word );
   void  set_remark( long i_Word, char *Remark );
   void  add_typelink( t_TypeLink &TypeLink );
   void  add_link( t_mLink &Link );
   void  add_links( t_mLink *Link, long n_Link );
   void  add_root( t_Root &Root );
   void  add_prefix( t_Affix &Affix );
   void  add_suffix( t_Affix &Affix );

   long  find_word( short i_struct , char *Str );
   void  link_word( long i_word, long i_typeLink, t_longList &List );
};

# endif
