/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                  Простенький такой текстовый редактор
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# ifndef EDIT_H
# define EDIT_H

# include <stdio.h>
# include <stdlib.h>
# include "window.h"
# include "koi9.h"

# define T_FRASAEND  1
# define T_ABSATZEND 2
# define T_THISFRASA 3

enum E_Function {
   F_Nop,
   F_Exit,
   F_Find,
   F_Replace3,   
   F_GoToLine,
   F_Up,
   F_Down,
   F_Left,
   F_Right,
   F_Home,
   F_End,
   F_PageUp,
   F_PageDown,
   F_BeginText,
   F_EndText,
   F_MUp,
   F_MDown,
   F_MLeft,
   F_MRight,
   F_MHome,
   F_MEnd,
   F_MPageUp,
   F_MPageDown,
   F_MBeginText,
   F_MEndText,
   F_Cat,
   F_Copy,
   F_IncCopy,
   F_Paste,
   F_Insert,
   F_BackSpase,
   F_Delete,
   F_DelStr,
   F_ShiftRight,
   F_ShiftLeft,
   F_PrevFrasa,
   F_NextFrasa,
   F_Mark,
   F_Unmark,
   F_ScrollUp,
   F_ScrollDown,
   F_Menu
};

/* --------------------------------- образ текста в памяти --------------- */
class t_Text
{
public: // не забыть убрать это безобразие
   t_myChar *Text ;         // участок текста хранящийся в памяти
   long      Length ;       // длинна текста, хранящегося в памяти
private:
   long      i_str,i_pos ;  // текущая строка и ее позиция
   long      n_String ;      // число строк в массиве Text
public:
	  t_Text( void );
	 ~t_Text( void );
   void       init  ( char *Text );         // инициировать текст строкой
   void       init16( ushort *Text );
   void       init32( t_myChar *Text );
   void       clear( void );
                                   // в e_Latin надо добавить значения для Юникда 
   void       write_file( char *name, e_Latin CodePage=KOI8 ); // записать текст в файл
   char       read_file(  char *name, e_Latin CodePage=KOI8 ); // прочитать тект из файла
   t_myChar & text( long i );
   t_myChar * text_all( );
   long       length( );
   long       n_string( );
   void       set_reper( long i_Str );
   void       reset_reper( );
   long       pos_str( long i_Str );              // начало строки i_Str
   t_myChar  *get_str( long i_Str );                 // прочесть из текста строку номер i_Str
   t_myChar  *get_str( long i_Str, t_myChar *Buf );      // прочесть строку номер i_Str в буфер
   void       put_str_ipos( long i_pos, t_myChar *str ); // вставить в текст строку str
   void       put_str_istr( long i_str, t_myChar *str ); // вставить в текст строку str
   long       get_text1( long Str1, long Str2, t_myChar *List ); // вытащить из текста кусок
   long       get_text2( long Str1, long Str2, t_myChar *List ); // вытащить из текста кусок без стирания
   void       put_text1( long Str1, t_myChar *List, long l_List );
   void       put_text1p( long Pos1, long Pos2, t_myChar *List ); // вставить фразу взамен
   void       insert_str( long PY );      // вставить пустую строку
   void       delete_str( long PY );      // удалить строку PY
   short      max_length_str( void );
   t_myChar*  get_frasa32( long pos );
   void       put_frasa32( long pos, t_myChar *Frasa );
   char*      get_frasa  ( long pos );
   void       put_frasa  ( long pos, char *Frasa );
   long       next_frasa( long pos );
   long       prev_frasa( long pos );
   long       begin_frasa( long pos );
   long       end_frasa( long pos );
   t_myChar*  get_word_punkt32( long pos );
   char*      get_word_punkt  ( long pos );
   long       prev_word( long z );
   long       next_word( long z );
   
   char      (*is_const)( t_myChar *Str ); // функция определения "слова с точкой"
   long      get_pos_xy( long py, long px );
   void      get_xy_pos( long pos1, long *_py, long *_px );
} ;

/* --------- собственно редактор ---------------------------------------- */
class t_Edit : public t_Win
{
//protected:
public:
   t_Text *Text ;
protected:                   // !!! сделать private
   long      Begin_Y ;       /* первая строка, видимая в окне */
   short     Begin_X ;       /* первая позиция, видимая в окне */
   short     PX ;            /* положение курсора */
   long      PY ;            /* номер редактируемой строки */
   long      old_PY ;
   short     otm1,otm2,      /* номера выделеных символов */
             Otm1,Otm2 ;     /* номера выделеных символов (упорядоченные) */
   long      str1,str2,      /* номера выделеных строк */
             Str1,Str2 ;     /* номера выделеных строк (упорядоченные) */
   t_myChar  Str[256],       /* текущая строка */
             Buf[256] ;
   char      fview ;         // если fview==1 - то запрет редактирования
   char      fOvertype ;     /* вставка-замена */
   char      Flag_b ;        /* флаг начала работы с буфером */   
public :
            t_Edit( );
 E_Function translate( short shift, ushort key1, ushort key2 );
   e_WinMsg main( void );
   e_WinMsg loop( short ZZ, ushort key1, ushort key2 );
   void     paint( void );                          // отобразить текст
   e_WinMsg int_to_msg( short i );
   void     raden_y3( );
   short    raden_Y( long PY );                   // действия связанные со сменой текущей строки
   short    raden_X( short px );                  // действия связанные со сменой текущей колонки
   void     delet_str1( short px );               // удалить из текущей строки символ
   void     insert_str1( short px, ushort key );  // добавить в текущую строку символ
   short    beg_str( void );                      // вычислить начало строки
   short    end_str( void );                      // вычислить конец строки
   short    char_exist( short px );               // есть ли в строке символы
   short    key_WK( void );                       // реакция на клавишу Enter
   void     key_Delet( void );                    // реакция на клавишу Delete
   void     delet_str( short Otm1,short Otm2,char f=0 );   // удалить текст в буфер
   void     insert_str( short px );               // вставть буфер в текущую строку (сделать приватной)
   // ----- новые функции -------------
   void     insert_str( t_myChar *Str );         // вставить нечто в текущую позицию
   void     cursor_right( );                     // подвинуть курсор вправо
   void     screen_up();                         // подвинуть экран вверх
   void     screen_down();                       // подвинуть экран вниз
   void     screen_set_y( long Begin_Y );        // установить первую строку экрана
//   void     set_py( long  _PY );
//   void     set_px( short _PX );

   void     find_forward( void );
   void     find_backward( void );
   void     goto_str( void );                     // пойти в заданную строку
   void     str_shift( char R );                  // сдвиг нескольких строчек
   short    dialog_position( short size_y );      // вычислить позицию диалога 
   char     find1( void );
   char     find( char Richt, char     *sFind );
   char     find( char Richt, t_myChar *sFind );
   void     replace( void );                      // ПРОКОМЕНТИРОВАТЬ ЭТИ ФУНКЦИИ
   void     replace1( t_myChar *sFind, t_myChar *sReplace );
   void     replace1( char *sFind, char *sReplace );
   void     replace3( void );
   void     set_text( t_Text *Text );
   void     init  ( char     *_Text );
   void     init32( t_myChar *_Text );
   void     set_view( char f );                   // установить флаг "только чтение"
   t_Text  *text();                               // !!! эту функцию закрыть, вместо нее сделать get_all_text
   t_myChar *text_all( );
   t_myChar *get_Buf();
   void     set_Buf( t_myChar *Str );
   void     write( char *Name, e_Latin CodePage=KOI8 );
   void     read(  char *Name, e_Latin CodePage=KOI8 );
//protected:
   void     paint_body( );                           // отобразить текст
   void     get_str( void );                         // взять    Str из Текста
   void     put_str( void );                         // положить Str в Текст
} ;

/* ------------------------ все, что относится к режиму поиска и замены -- */
class t_Find
{  public :
   t_myChar  sfind[256] ;   // строка, которую надо найти
   t_myChar  sreplace[256] ;// строка, на которую надо заменить
   t_myChar *xlat ;         // соответствие больших и маленьких букв (НАДО ДЕЛАТЬ ЧЕРЕЗ КОИ9)
   t_myChar  Mass[2560] ;   // предыдущие поиски
   t_myChar *prev_find[10] ;// предыдущие поиски
   char  fxlat,         // 0-различать 1-не различать большие и маленькие буквы
	 fcontinue,     // 1-продолжать поиск сначала (или с конца) файла
	 fdirect,       // направление поиска 0-вперед 1-назад
	 falles ;       // 1-заменить все выражения не задавая вопросов

	     t_Find( void );
   e_WinMsg  loop( short y0 );
   void      paint( short y0, short punkt );
} ;

# endif
