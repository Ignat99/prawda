/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                 настройки, размеры, шрифты и т.п.
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# ifndef RESOURCE1_H
# define RESOURCE1_H

# include "mylib.h"
# include "video.h"
# include "list.h"
# include "lexer.h"
# include "window.h"
# include "menu.h"

# define       fon_Color  Option.Color[0]   // цвет фона (в режиме besser)
# define      text_Color  Option.Color[1]   // основной текст
# define       buf_Color  Option.Color[2]   // текст буфера (при выделении)
# define  red_rame_Color  Option.Color[3]   // рамки
# define  red_Rame_Color  Option.Color[4]   // рамки текущего окна
# define     regim_Color  Option.Color[5]   // заголовки режимов
# define      titl_Color  Option.Color[6]   // заголовки окон
# define  titlchen_Color  Option.Color[7]   // заголовки подокон
# define    status_Color  Option.Color[8]   // (статус-лайн) вставка-замена и строка-столбец
# define   hotkey1_Color  Option.Color[9]   // горячие клавиши внизу экрана (клавиши)
# define   hotkey2_Color  Option.Color[10]  // горячие клавиши внизу экрана (функции)
# define   hotkey3_Color  Option.Color[18]  // (нету) горячие клавиши на окнах

# define      menu_Color  Option.Color[11]  // меню
# define curs_menu_Color  Option.Color[12]  // меню текущее
                                // ---- режим подсказка --------------
# define      help_Color  Option.Color[13]  // подсказка
# define bold_help_Color  Option.Color[14]  // выделенный шрифт (в подсказке)
# define link_help_Color  Option.Color[15]  // переходы (в подсказке)
                                // ------- прочая фигня --------------
# define     error_Color  Option.Color[16]  // сообщение об ошибке
# define  cur_item_Color  Option.Color[17]  // текущий элемент какого-нибудь списка
# define  sel_item_Color  Option.Color[19]  // выделенный элемент какого-нибудь списка

// ---------- различные функции интерфейса (один массив) ----------------
enum e_Function 
{
   // ------------- общие действия --------------------------
   F_NOP,                 // нет операции
   F_END_OF_LIST,         // конец списка
   F_HELP,                // подсказка
   F_MENU1,               // меню
   F_EXIT,                // выход без сохранения результата
   F_OKEY,                // выход с сохранением результата
   
   // ============= форматирование ==========================
   F_FORMAT_SET_FRASA,    // установить границу фразы
   F_FORMAT_SET_ABSAZ,    // установить границу абзаца
   F_FORMAT_CLR,          // сбросить границу фразы
   F_FORMAT_CLR_ALL,      // убрать все границы
   F_FORMAT_MAKE_ALL,     // построить все границы автоматически
   
   // ============= контекстный словарь (весь список) =======
   F_DICT_USE,            // установить/сбросить флаг "использовать"
   F_DICT_FORMAT,         // форматировать текст
   F_DICT_MAKE_DICT,      // сделать словарь
   F_DICT_FIND,           // поиск в списке выражений
   F_DICT_ENTER,          // перейти в режим задания перевода слову
   F_DICT_FILTER,         // установить фильтры для списка слов
   F_DICT_DEL,            // удалить словарную статью
   F_DICT_INS,            // дублировать словарную статью
   F_DICT_HOLE,           // пересчитать число непереведенных слов
   
   // ============= контекстный словарь (одно слово) ========
   F_WORD_EXAMPLE_NEXT,   // следующее  использование этого выражения
   F_WORD_EXAMPLE_PREV,   // предыдущее использование этого выражения

   F_WORD_WORD_NEXT,      // следующее  слово подстрочника
   F_WORD_WORD_PREV,      // предыдущее слово подстрочника

   F_WORD_KONTEXT_UP,     // хождение по словарным статьям контекстного словаря 
   F_WORD_KONTEXT_DOWN,   //
   F_WORD_KONTEXT_DELETE, //
   
   F_WORD_MAIN_UP,        // хождение по словарным статьям главного словаря 
   F_WORD_MAIN_DOWN,      //
   F_WORD_MAIN_INSERT,    //
   F_WORD_MAIN_INSERT1,   //
   
   F_WORD_HUMAN_UP,       // вверх списка слов человеческого словаря
   F_WORD_HUMAN_DOWN,     //  вниз списка слов человеческого словаря
   
   F_WORD_WIZARD,         // создать словарную статью визардом переводов
   F_WORD_GET,            // взять кусок текста из человеческого словаря
   F_WORD_SELECT_DICT,    // переключить словарную базу
   
   // ============= менеджер словаря ========================
   F_DMGR1_MARK_UNKNOWN_WORD,  // Выделить известные слова
   F_DMGR1_DEL_MARKED_WORD,    // Удалить выделенные слова
   F_DMGR1_UNMARK_MARKED_WORD, // Погасить выделение
   F_DMGR1_INVERT_MARKED_WORD, // Инвертировать выделение
   F_DMGR1_WRITE,              // Записать словарь в файл
   
   F_DMGR2_READ,               // Прочитать из файла
   F_DMGR2_CALC_PEREVOD,       // Угадать перевод неизвестных слов
   F_DMGR2_MARK_NEW_WORD,      // Выделить новые слова
   F_DMGR2_MARK_NEW_PEREVOD,   // Выделить новые варианты перевода
   F_DMGR2_UNMARK_MARKED_WORD, // Погасить выделение
   F_DMGR2_INVERT_MARKED_WORD, // Инвертировать выделение
   F_DMGR2_COPY_WORD,          // Вставить в контекст
   F_DMGR2_FIND,               // Поиск во внешнем словаре
   
   // ============= перевод фразы (весь список) =============
   F_MACRO_UP,           // шаг по списку фраз вверх
   F_MACRO_DOWN,         // шаг по списку вниз
   F_MACRO_PG_UP,        // шаг по списку на страницу вверх
   F_MACRO_PG_DN,        // шаг по списку на страницу вниз
   F_MACRO_HOME,         // в начало списка
   F_MACRO_END,          // в конец списка
   F_MACRO_DEL,          // удалить  фразу или абзац
   F_MACRO_INS,          // вставить фразу или абзац

   F_MACRO_LANG,     // переключение "оригинал" - "перевод"
   F_MACRO_ENTER,    // переход в окно "перевести фразу"
   F_MACRO_LEFT,     // сдвиг фразы влево
   F_MACRO_RIGHT,    // сдвиг фразы вправо

   F_MACRO_FLAG,     // поставить флаги фразе
   F_MACRO_MAKE,     // построить гипертекст
   F_MACRO_TRANS,    // перевод всего гипертекста
   F_MACRO_TRANS1,   // перевод одной фразы

   // ============= перевод фразы (одна фраза) ==============
   F_FRASA_SRC_UP,        // хождение по исходному тексту
   F_FRASA_SRC_DOWN,

   F_FRASA_WORD_NEXT,     // следующее  слово подстрочника
   F_FRASA_WORD_PREV,     // предыдущее слово подстрочника
   F_FRASA_WORD_VARIANT,  // переход в окно "варианты выбранного слова"
   F_FRASA_WORD_VARIANT1, // вставить первый вариант в текст "как есть"
   F_FRASA_WORD_VARIANT2, // вставить первый вариант в текст оригинал

   //------------ хождение по человеческому словарю ---------------
   F_FRASA_HUMAN_UP,      // вверх списка слов человеческого словаря
   F_FRASA_HUMAN_DOWN,    // вниз  списка слов человеческого словаря

   F_FRASA_FLAGS,
   F_FRASA_UNDO,
   F_FRASA_DEBUG,
   F_FRASA_NEXT,
   F_FRASA_PREV,
   F_FRASA_TRANS,
   F_FRASA_WINDOW_SWITCH,
   F_FRASA_GET,
   F_FRASA_TODICT,       // перейти в режим задания перевода этому слову
   F_FRASA_SELECT_DICT   // переключить словарную базу
};

struct t_OptionParam
{  char Tag[50] ;
   char Value[200] ;
};
DECLARE_LIST_TYPE( t_OptionParam,t_OptionParamList );

// ---------- горячая клавиша -----------------------------------------
struct t_HotKey
{
   short SStatus ;        // шифтстатус, который при этом должен быть
   short key1, key2 ;     // клавиши, которые при этом надо нажимать (простая и служебная)
};

// ---------- действия различных режимов (много массивов) -------------
struct t_HotKey2
{
   t_HotKey    Key  ;
   e_Function  Func ;
   char        DefValue[20]; // значение по умолчанию (а потом - что получилось)
   char        MenuFind[30]; // какую строку найти в меню
   char        HelpFind[20]; // какую строку найти в файле подсказки
   char        Name[50] ;    // имя параметра в файле ресурсов
};

struct t_Option
{
   t_OptionParamList  Param ;
   char               Color[20] ;
   t_Rect             Rect [10] ;

   int        read_all( char *File );
   int        read( char *File );
   char      *get( char *Tag );
   e_Function get_func( t_HotKey2 *HotKey, 
                        short Shift, short key1, short key2 );
   void       get_key( t_HotKey &Key, char *Str, short x );
   void       get_key( ushort *ZZ, ushort *key1, ushort *key2, char *Str, short x );
private:
   void   init_colors( );
   void   set_hotkeys( t_HotKey2 *HotKey, t_Menu *Menu );
   void   set_hotkeys( t_HotKey2 *HotKey, t_Menu *Menu, t_String &Help );
   void   set_color1( char &Color, char Default, char *Name );
   void   set_rect1( t_Rect &Rect, char *Name );
   short  tcolor( char *str );
   char   trect( t_Rect &Rect, char *Str );
   char   set_help_hotkey( t_String &Str, char *Funk, char *Key );
   char   set_menu_hotkey1( char *Menu, char *Key );
   char   set_menu_hotkey( t_Menu *Menu, char *sFind, char *HotKey );
   char   read_hotkey( t_HotKey2 &HotKey, char *Str );
   char   read_hotkey( t_HotKey &Hotkey, char *Str );
   void   set_hotkey0( char *Str );
   void   set_hotkey1( t_HotKey &Key, char *Str1 );
};

extern t_Option Option ;

# endif
