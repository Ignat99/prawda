/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//     Работа с "крупной структурой" текста
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# ifndef HIPERTEXT_H
# define HIPERTEXT_H

# include "list.h"
# include "edit.h"
# include "serial.h"

# define char16  unsigned short // пределение юникодных символов

// ------------ место, чтобы хранить фразу -------------------
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
// ------------ признак того, как фраза переводилась ---------
enum e_Perevod
{
    NOT =0, // не переводилась
    AUTO=1, // автоматически
    HAND=2  // руками
};
// ------------ типы текстовых параграфов --------------------
struct t_TypeAbsatz
{
   // всякие признаки, отнсящиеся к абзацу
   char       Color ;     // цвет и фон этого типа куска
   char       not_trans ; // признак, что не транслировать
   char       Name[40] ;  // название
   char      *begin,*end ; // что писать в начале и конце абзаца (захвачено malloc-ом)
};
DECLARE_LIST_TYPE( t_TypeAbsatz,t_TypeAbsatzList );
// ------------ Фраза ----------------------------------------
struct t_Frasa
{  long       begin ,end  ; // кусок текста, являющийся фразой (источник)
   t_String32 Str ;         // кусок текста, являющийся фразой (приемник)
   e_Perevod  perevod     ; // признак того, что эта фраза переводилась (нет.авт.руками)
   char       flag1       ; // можно переводить на автомате
   
   t_Frasa();
   ~t_Frasa();
   void init();
   void del();
   void operator = ( t_Frasa &F );
};
DECLARE_LIST_TYPE( t_Frasa,t_FrasaList );
// ------------ Параграф -------------------------------------
struct t_Absatz
{
   char        type ;    // тип параграфа
   t_FrasaList Frasa ;   // массив фраз этого параграфа
public:
   // format(???);       // форматирование в соответствии с типом
   t_Absatz();
   void init();
   void del();
   void operator = ( t_Absatz &A );
};
DECLARE_LIST_TYPE( t_Absatz,t_AbsatzList );
// ------------ Сортировочная структура ----------------------
struct t_HT_Sort
{  long i_absatz ;
   long i_frasa ;
   long begin1 ;
};
DECLARE_LIST_TYPE( t_HT_Sort,t_HT_SortList );

// ------------ Структурированный текст ----------------------
       
struct t_Macrostruct : public t_Serial
{
   t_Text       *Text ;   // текст (абсолютно константный)
   t_AbsatzList  Absatz ; // список абзацев (возможно надо вынести отсюда)
   t_TypeAbsatzList TypeAbsatz ; // типы абзацев
   char16       *begin,*end ; // что писать в начале и конце текста
   long          j_Text ;
   t_HT_SortList Sort ;   // Сортировочный список
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
   void  clear();                   // убрать разбиение на параграфы и фразы.
   void  automake1();               // сделать автоматическое разбиение на параграфы и фразы.
   void  automake2();               // сделать автоматическое разбиение на параграфы и фразы.
   void  set_frasa_point( long i ); // установить границу между фразами.
   void  res_frasa_point( long i ); // снять границу между фразами.
   void  set_absatz_point( long i ); // установить границу между параграфами
   void  res_absatz_point( long i ); // снять границу между параграфами
   void  set_type_part( short Type, long Pos ); // устанвить тип параграфа
   long  get_frasa ( long i_Absatz, long i_Frasa, char *Str ); // дать фразу
   long  get_frasa2( long i_Absatz, long i_Frasa, char *Str ); // дать фразу перевода
   void  put_frasa ( long i_Absatz, long i_Frasa, char *Str ); // записать перевод фразы
   void  make_text1();               // форматирование текста перевода

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
