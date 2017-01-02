/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//              Структуры для построения контекстного словаря
//     Анисимов Д.В.                                         сегодня
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

# include "core.h"       // это только из t_longList
# include "serial.h"

// ----------------- Упорядоченные слова --------------------------------
struct t_wWord
{  char *str ;
   long  i_slowo ;     // ссылка в Perevod (если есть)
   long  i_word ;      // ссылка Word1<->Word0
   long  num ;         // число таких словоформ
   char  f_perevod ;   // есть ли перевод
};
DECLARE_LIST_TYPE( t_wWord,t_wWordList );

// ----------------- Экземпляры выражений -------------------------------
struct t_wExemplar
{  long  i_struct ;    // было short
   long  index ;       // индекс в массиве t_wExemplarList
   long  i_word,       // положение в тексте Word0
         i_last_word ; // положение в тексте Word0
   long  i_word1 ;     // положение во фразе
   long  i_absatz,i_frasa ; // положение в гипертексте
   long  i_slowo ;
   long  i_type ;      // индекс типа
   char  Str[100] ;    // где хранится выражение
};
DECLARE_LIST_TYPE( t_wExemplar, t_wExemplarList );

// ----------------- Типы выражений -------------------------------------
struct t_wType
{  short used ;        // 0 - используется
                       // 1 - не используется
   short i_struct ;
   long  i_slowo ;     // для WORD TSTRUCT1 (type=0) - ссылка в Perevod 
   long  i_word1 ;     // ссылка в Word1 (только что ввел)
   long  i_exemplar,   // ссылки на экземпляры         t_Kontext->i_Exemplar
         i_last_exemplar ;
   long  num ;         // было short число таких выражений
   long  n_hole ;      // было short число непереведенных экземпляров таких выражений
   char  select ;      // признак того, что слово выбрано в режиме Менеджер Словаря
   char  Str[100] ;    // где хранится выражение и перевод
};
DECLARE_LIST_TYPE( t_wType, t_wTypeList );

// ----------------- Все данные о контексте -----------------------------
struct t_Kontext : public t_Serial
{
   time_t          mtime ;    // последнее время модификации словарей
                              // Исходного текста нет - источником является hypertext
   char           *Word00 ;   // Массив, куда ссылаются t_wWord->str
   t_wWordList     Word0 ;    // Массив слов текста
   t_wWordList     Word1 ;    // Массив слов текста упорядоченный с исключением лишних
   t_wExemplarList Exemplar ; // Экземпляры выражений
   t_wTypeList     Type ;     // Типы выражений
   t_longList      i_Exemplar;// ссылка в массив Exemplar
   t_longList      Interes ;  // "интересные" структуры 0-значит интересные
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
