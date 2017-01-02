/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//     Работа с "крупной структурой" текста. Интерфейс.
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# ifndef HIPERTEXT2_H
# define HIPERTEXT2_H

# include "common.h"
# include "window.h"
# include "macrostruct.h"
# include "koi9.h"
# include "edit.h"
# include "edit1.h"
# include "edit2.h"
# include "slowo1.h"

struct t_Cursor1
{  long begin ;
   long current ;
   t_Cursor1(){  begin=current=0 ;  };
   void init(){  begin=current=0 ;  };
};
// ----------------------- вариант перевода слова -------------------
struct t_WordVariant
{
   e_Type  type ;
   short   i_word,      // номер слова, с которого начинается
           i_last_word,
	   i_struct;    // номер структуры в t_Grammar->Trans[]
   long	   i_variant;   // номер варианта перевода 
   char    i_dic ;      // 0-словарь перевода, 1-словарь догадок (Переводик)
   long    i_slowo ;    // индекс в словаре переводов
   short   i_slowo1 ;   // способ перевода
   char    Antwort[80]; // как оно переводится (по-русски)
};
DECLARE_LIST_TYPE( t_WordVariant, t_WordVariantList );

// ------------ Структурированный текст (интерфейс) ------------------
class t_Macrostruct2 : public t_Win
{
public :
   short  Begin,PY ;      // положение курсора
   short  Begin_X ;
   char   f_Dst ;         // отображать источник или приемник
   
public :
            t_Macrostruct2( );
   void     paint( long Begin, long PY );
   void     paint1( long i_list, long y, char f );
   e_WinMsg main_loop ( void );
   e_WinMsg loop( e_Function Func, short ZZ, ushort key1, ushort key2 );
   e_WinMsg flag_frasa( t_Frasa &F );
   e_WinMsg flag_absatz( t_Absatz &A );
   e_WinMsg frasa_work( long i_absatz, long i_frasa );
   void     translate_all( );
   void     translate_frasa( long i_absatz, long i_frasa );
};

// ----------- выбор варианта перевода слова --------------------------
class t_VariantView : public t_Win
{
   public:
   long   begin,
          current ;
   t_WordVariantList  WordVariant ;

   t_VariantView();
  ~t_VariantView();
   void      paint();
   e_WinMsg  loop();
// void      loop_rect( short yy );
   void      make_variant_list( short i_word );
};

// --------- выбор формы выражения -----------------------------------
class t_FormWisard : public t_Win
{
   public:
   short  i_struct,i_word,i_variant ;
   private:
   short  i_param,i_value ;
   t_Form Form ;
   char   SResult[1000]; // результирующая фраза

   public:
   t_FormWisard();
  ~t_FormWisard();
   void      paint();
   e_WinMsg  main_loop();
   void      func_ok();
};

// ------------- класс для ручного перевода фразы ---------------------
class t_FrasaFace : public t_Win
{
public:

   t_Rect       RFrom,   // геометрия подокон
                RFirst,
	        RSlowo,
	        RTo,
	        REditSlowo,
		RVariantView,
	        RFormWisard ;

   long         i_Absatz,i_Frasa ;
   t_FirstLook  First ; // первый взгляд на фразу
   t_EditTrans *From ;  // исходный текст (с исходной фразой) 
                        // (переименовать в Edit1)
   t_EditTrans  To ;    // переведенная фраза
   t_EditTrans  EditSlowo ; // перевод человеческого словаря
   char         fReg ;  // 0-верхнее окно 1-нижнее окно
   short        yyy3 ;  // граница между верхним м нижним окном (принадлежит фразе)
   t_String32   Undo ;  // строка, где храниться первоначальное состояние

   t_ViewListSelect ViewSelect ; // окошко чтобы что-нибудь выбирать

public:
                t_FrasaFace();
   void         calc_pos( short x1, short y1, short x2, short y2 );
   void         set_pos ( short x1, short y1, short x2, short y2 );
   void         set_pos( );
   void         init( long i_Absatz, long i_Frasa, t_myChar *Str, long L );
   void         paint();
   e_WinMsg     main_loop();
   e_WinMsg     loop( e_Function Func, short ZZ, ushort key1, ushort key2 );
   void         loop_debug();
   e_WinMsg     answer_loop();
   void         resize_vindow( void *Void );
   void         loop_rect( short yy );
   void         translate();
   void         variant( short i_word );
   void         variant1( short i_word );
   void         variant2( short i_word );
// void         make_variant_list( short i_word );
   void         to_dict();
   char        *str_fine( char *Str );
};

# endif
