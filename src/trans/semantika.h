/*************************************************************************/
//                 Данные для анализа семантики
//
/*************************************************************************/
# ifndef SEMANTIKA_H
# define SEMANTIKA_H

# include "list.h"
# include "grammar.h"
# include "slowo2.h"

DECLARE_LIST_TYPE( t_Shabl, t_ShablList );

// ------------- аффиксы -------------------------------------
struct t_Affix
{
   char  *Name ;     // сам суффикс 
   short  i_link ;   // тип связи
   short  i_struct ; // часть речи (-1 - любая)
};
DECLARE_LIST_TYPE( t_Affix, t_AffixList );

// ------------- корни ---------------------------------------
struct t_Root
{
   char  *Str ;      // первый корень
   char  *Str2 ;     // второй корень
};
DECLARE_LIST_TYPE( t_Root, t_RootList );

// ------------- связки --------------------------------------
struct t_mLink
{
   short type ;      // ссылка на массив типов связок t_Semantika->TypeLink
   long  i_word ;    // c каким словом связано
   long  i0_word ;   // какое слово связано (откуда идет ссылка)
   char  *str1 ;     // литерал ссылки
                     // признак обратной ссылки
   // ----------------- нужно при чтении -----------
   long  i_struct ;   // часть речи слова
//   char  *Name ;     // имя типа связки
   char  *str ;      // слово
   char  *remark ;   // коментарий слова
};
DECLARE_LIST_TYPE( t_mLink, t_mLinkList );

// ------------- слова ---------------------------------------
struct t_mWord
{
   short  i_struct ;  // индекс структуры в    t_Grammar->Trans
   char  *str ;       // ссылка на слово       t_Semantika->Mass,Mass1
   char  *remark ;    // комментарий (для различения омонимов)
   t_Form Param ;     // константные параметры
   long   i_link,     // ссылка на массив связок t_Semantika->Link
          n_link ;    // число связок
   long   i_shablon ; // способ словоизменеия
   char   type ;      // тип - слово и/или группа
};
DECLARE_LIST_TYPE( t_mWord, t_mWordList );

/***************************************************************************/
//                   граф семантических связей
/***************************************************************************/
class t_Semantika
{
   char        FileName[40] ;
   t_Format4  *Format ;    // формат словаря

   char       *Mass ;      // массив, через который это читается
   char       *Mass1 ;     // массив новых данных (не перезахватывается)
   long        n_Mass,n_Mass1,j_Mass1 ;
   char        EmptyStr[1] ;

public:
   t_ShablList    *Shabl ;     // шаблоны словоизменеия (взяты из slowo2.h)
                               // размер массива равен числу частей речи
// t_TypeLinkList  TypeLink ;  // типы связок
   t_AffixList     Prefix,
                   Suffix ;    // приставки и суффиксы
   t_RootList      Root ;      // корни
   t_mLinkList     Link ;      // связки
   t_mWordList     Word ;      // слова
   t_Lexer         LShablon,   // лексеры
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
   // ------ добавить что-нибудь ---------------
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
