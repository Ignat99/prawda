/*****************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//              Структуры для построения контекстного словаря (интерфейс)
//     Анисимов Д.В.                                         сегодня
/*****************************************************************************/
# ifndef KONTEXT2_H
# define KONTEXT2_H

# include "core.h"       // это только из t_longList
# include "window.h"
# include "kontext.h"
# include "macrostruct.h"
# include "word.h"      // Xa-xa-xa!!!

// --------------- каким образом фильтровать признак --------------
enum e_Filter
{  YES =0,
   NO  =1,
   ALL =2
};
// --------------- Как сортировать структуры -------------------
enum e_Sort
{  FREQ  =0,              // по частоте
   ALPHA =1,              // по алфавиту
   STRUCT=2               // части речи
};
// --------------- фильтр интересующих структур -------------------
struct t_Filter
{
   e_Filter exist   ;  // есть ли в словаре
   e_Filter perevod ;  // задан ли перевод
   e_Filter struct1 ;  // укладывается ли в TSTRUCT1

   t_Filter(){ exist=perevod=struct1=ALL ; };
};

class t_Kontext2 : public t_Win
{
public:
   t_Filter   Filter ;
   e_Sort     Sort ;
   t_longList Freq ;  // список, отсортированных по частоте
   t_longList Alpha ; // список, отсортированных по алфавиту
   t_longList Struct ;// список, отсортированных по части речи
   t_longList View ;  // видимый список
   short      Begin,BeginX,PY ;
public:
            t_Kontext2( );
   void     paint( long Begin, long PY );
   void     paint_small( long Begin, long PY );
   void     paint1( long i_list, long y, char f );
   e_WinMsg main_loop( void );
   e_WinMsg loop( e_Function Func, short ZZ, ushort key1, ushort key2 );
   e_WinMsg loop_small( e_Function Func, short ZZ, ushort key1, ushort key2 );  
   void     double_type( );
   void     delete_type( );
   void     paint_flags( short py );
   e_WinMsg loop_flags( void );
   void     make_sort( );
   void     make_filtred_list( );
   char     filter( long i_Type );
   void     find();
};
/**************************************************************/

// ----------- редактор строки (перетащить этот класс в common.cpp) 
struct t_EditLine : public t_Win
{   char Str[100] ;
    
             t_EditLine();
    e_WinMsg main_loop();
    void     paint();
};
// ------------------------------------------------------
struct t_wStr
{  char Str[100] ; // заменить на юникодную или оставить в koi9?
};
DECLARE_LIST_TYPE( t_wStr ,t_wStrList );

// -------- редактор вариантов переводов ----------------
struct t_wStructList : public t_wStrList, public t_Win
{  
   short Begin_Y,PY ;
   
            t_wStructList();
   void     paint( );
   e_WinMsg main_loop( ushort key1, ushort key2 );
   void     init( char *Str );
   void     step_up();
   void     step_dn();
   void     current_del();
   void     current_ins( t_wStr &Str );
};
// ------------- Задать перевод одному выражению ---------------
struct t_SetPerevod : public t_Win
{
   t_Rect        REdit1,  // геометрия подокон
	         RFirst,
		 RWordWizard,
	         RFrom,
	         RTo,
	         RCurrentL,
	         RMainL,
	         RSlowo,
	         REditSlowo,
	         RAddWord ;

   long          i_type ;      // Индекс типа выражения
   long          ii_exemplar ; // Индекс экземпляр выражения
   t_Frasa      *F_current ;   // Текущая фраза
   t_FirstLook   First ;       // Переведенный текст
   t_WordWizard  WordWizard ;  // Конструктор словарной статьи
// t_wStructList CurrentList ; // Текущий словарь
   t_wStructList MainList ;    // Главный словарь
   t_EditTrans   EditSlowo ;   // перевод человеческого словаря
                               // Клипборд (пока проигнорирован)
   t_Text        T ;           // Текст для EditSlowo
   char          CurrentFrame ;// 0-словарная запись 1-словарь 2-ответ словаря

             t_SetPerevod( );
   void      calc_pos( short x1, short y1, short x2, short y2 );
   void      set_pos ( short x1, short y1, short x2, short y2 );
   void      set_pos( );
   void      init( long _i_type );
   void      paint( );
   e_WinMsg  main_loop( );
   e_WinMsg  loop( e_Function Func, short ZZ, ushort key1, ushort key2 );
   e_WinMsg  answer_loop( );
   void      loop_rect( short yy );
   void      resize_vindow( void *Void );
   void      set_i_exemplar( long i_exemplar );
   void      flat_perevod( char *_Str );
};

# endif
