/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                 настройки, размеры, шрифты и т.п.
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <ctype.h>
# include <string.h>
# include "mylib.h"
# include "option.h"
# include "video.h"
# include "windows.h"

t_HotKey2 KeyKontext[]= /*OK*/
{
   { 0,0,0, F_HELP,                      "F1"     ,""               ,"Подсказка"   , "Подсказка" },
   { 0,0,0, F_DICT_USE,                  "F2"     ,"Использовать"   ,"Использовать", "УстановитьСброситьФлагИспользовать" },
   { 0,0,0, F_DICT_FILTER,               "F3"     ,"Фильтры"        ,"Фильтры"     , "ЗадатьФильтры" },
   { 0,0,0, F_DICT_MAKE_DICT,            "F4"     ,"Построить"      ,"Построить"   , "СделатьСловарь" },
   { 0,0,0, F_DICT_FIND,                 "F7"     ,"Поиск"          ,"Поиск"       , "СловарьПоиск" },
   { 0,0,0, F_MENU1,                     "F9"     ,""               ,"Меню"        , "Меню" },
   { 0,0,0, F_OKEY,                      "F10"    ,""               ,"ВыходОк"     , "ВыходОк" },
   { 0,0,0, F_DICT_ENTER,                "Enter"  ,"Задать перевод" ,"Слово"       , "ЗадатьПереводСлову" },
   { 0,0,0, F_DICT_DEL,                  "Del"    ,"Удалить"        ,""            , "УдалитьСлово" },
   { 0,0,0, F_DICT_INS,                  "Ins"    ,"Вставить"       ,""            , "ДублироватьСлово" },
   { 0,0,0, F_END_OF_LIST,               ""       ,"" , "" }
};

t_HotKey2 KeyDictMgr1[]=
{
   { 0,0,0, F_DMGR1_MARK_UNKNOWN_WORD,   "="      ,"Выделить известные слова" ,"" ,"СписокКонтекстаВыделитьИзвестныеСлова" },
   { 0,0,0, F_DMGR1_DEL_MARKED_WORD,     "F8"     ,"Удалить выделенные слова" ,"" ,"СписокКонтекстаУдалитьВыделенныеСлова" },
   { 0,0,0, F_DMGR1_UNMARK_MARKED_WORD,  "-"      ,"Погасить выделение      " ,"" ,"СписокКонтекстаПогаситьВыделение" },
   { 0,0,0, F_DMGR1_INVERT_MARKED_WORD,  "Shift+*","Инвертировать выделение " ,"" ,"СписокКонтекстаИнвертироватьВыделение" },
   { 0,0,0, F_DMGR1_WRITE,               ""       ,"Записать словарь в файл " ,"" ,"СписокКонтекстаЗаписатьВФайл" },
   { 0,0,0, F_MENU1,                     "F9"     ,""                   ,"Меню"   ,"Меню" },
   { 0,0,0, F_OKEY,                      "F10"    ,""                ,"ВыходОк"   ,"ВыходОк" },
   { 0,0,0, F_END_OF_LIST,               ""       ,"" , "" }
};

t_HotKey2 KeyDictMgr2[]=
{
   { 0,0,0, F_DMGR2_READ,                ""       ,"Прочитать из файла     " ,"" ,"СписокВнешнийПрочитатьИзФайла" },
   { 0,0,0, F_DMGR2_CALC_PEREVOD,        ""       ,"Угадать перевод        " ,"" ,"СписокВнешнийУгадатьПеревод" },
   { 0,0,0, F_DMGR2_MARK_NEW_WORD,       "="      ,"Новые слова            " ,"" ,"СписокВнешнийНовыеСлова" },
   { 0,0,0, F_DMGR2_MARK_NEW_PEREVOD,    "Shift+?","Новые варианты перевода" ,"" ,"СписокВнешнийНовыеВариантыПеревода" },
   { 0,0,0, F_DMGR2_UNMARK_MARKED_WORD,  "-"      ,"Погасить выделение     " ,"" ,"СписокВнешнийПогаситьВыделение" },
   { 0,0,0, F_DMGR2_INVERT_MARKED_WORD,  "Shift+*","Инвертировать выделение" ,"" ,"СписокВнешнийИнвертироватьВыделение" },
   { 0,0,0, F_DMGR2_COPY_WORD,           "F5"     ,"Вставить в контекст    " ,"" ,"СписокВнешнийВставитьВКонтекст" },
   { 0,0,0, F_DMGR2_FIND,                "F7"     ,"Поиск                  " ,"" ,"СписокВнешнийПоиск" },
   { 0,0,0, F_MENU1,                     "F9"     ,""                   ,"Меню"  ,"Меню" },
   { 0,0,0, F_OKEY,                      "F10"    ,""                ,"ВыходОк"  ,"ВыходОк" },
   { 0,0,0, F_END_OF_LIST,               ""       ,"" , "" }
};

t_HotKey2 KeyPerevod[]= /*OK*/
{
   { 0,0,0, F_HELP,                      "F1"     ,""                ,"Подсказка","Подсказка" },
   { 0,0,0, F_MENU1,                     "F9"     ,""                ,"Меню"    , "Меню" },
   { 0,0,0, F_OKEY,                      "F10"    ,""                ,"ВыходОк" , "ВыходОк" },
   { 0,0,0, F_EXIT,                      "Esc"    ,""                ,"ВыходEsc", "ВыходEsc" },
   { 0,0,0, F_WORD_EXAMPLE_NEXT,         "Ctrl+PgDn"  ,"   Шаг вверх       1" ,""        , "СледующееИспользованиеВыражения" },
   { 0,0,0, F_WORD_EXAMPLE_PREV,         "Ctrl+PgUp"  ,"   Шаг вниз        1" ,""        , "ПредыдущееИспользованиеВыражения" },

   { 0,0,0, F_WORD_WORD_NEXT,            "Shift+Down" ,"   Шаг вверх       2" ,""        , "СловоПодстрочникаСледующее" },
   { 0,0,0, F_WORD_WORD_PREV,            "Shift+Up"   ,"   Шаг вниз        2" ,""        , "СловоПодстрочникаПредыдущее" },

   { 0,0,0, F_WORD_KONTEXT_UP,           "Ctrl+Up"    ,"   Шаг вверх       3" ,""        , "КонтекстныйСловарьВверх" },
   { 0,0,0, F_WORD_KONTEXT_DOWN,         "Ctrl+Down"  ,"   Шаг вниз        3" ,""        , "КонтекстныйСловарьВниз" },
   { 0,0,0, F_WORD_KONTEXT_DELETE,       "Ctrl+Del"   ,"Удалить"              ,"Удалить" , "КонтекстныйСловарьУдалитьВариант" },
   
   { 0,0,0, F_WORD_MAIN_UP,              "Alt+Up"     ,"   Шаг вверх       4" ,""        , "ГлавныйСловарьВверх" },
   { 0,0,0, F_WORD_MAIN_DOWN,            "Alt+Down"   ,"   Шаг вниз        4" ,""        , "ГлавныйСловарьВниз" },
   { 0,0,0, F_WORD_MAIN_INSERT,          "Alt+Ins"    ,"Добавить"             ,"Добавить", "ГлавныйСловарьВставитьВариант" },
   { 0,0,0, F_WORD_MAIN_INSERT1,         "Alt+="      ,"Выбрать"              ,"Выбрать" , "ГлавныйСловарьВыбратьВариант" },

   { 0,0,0, F_WORD_HUMAN_UP,             "Up"         ,""                     ,""        , "ЧеловеческийСловарьВверх" },
   { 0,0,0, F_WORD_HUMAN_DOWN,           "Down"       ,""                     ,""        , "ЧеловеческийСловарьВниз" },

// { 0,0,0, F_WORD_WIZARD,               "F6"         ,"Создать"              ,"Создать" , "СоздатьСловарнуюСтатьюКонструкторомПереводов" },
   { 0,0,0, F_WORD_GET,                  "Enter"      ,"Взять из словаря"     ,"Взять"   , "ВзятьИзЧеловеческогоСловаря" },
   { 0,0,0, F_WORD_SELECT_DICT,          "Shift+Tab"  ,"Переключить словарь"  ,"Словарь" , "ПереключитьСловарь" },
   { 0,0,0, F_END_OF_LIST,               ""       ,"" , "" }
};

t_HotKey2 KeyMacro[]= /*OK*/
{
   { 0,0,0, F_HELP,                 "F1"     ,""                 ,"Подсказка","Подсказка" },
   { 0,0,0, F_MENU1,                "F9"     ,""                 ,"Меню"     , "Меню" },
   { 0,0,0, F_OKEY,                 "F10"    ,""                 ,"ВыходОк"  , "ВыходОк" },

   { 0,0,0, F_MACRO_TRANS1,         "F6"     ,"Перевести Авт."   ,"ПерАвт"   , "ПереводФразы" },
   { 0,0,0, F_MACRO_ENTER,          "Enter"  ,"Перевести Руч."   ,"ПерРуч"   , "ПереходВОкноПеревестиФразу" },
   { 0,0,0, F_MACRO_FLAG,           "F3"     ,"Флаги"            ,"Флаги"    , "ПоставитьФлагиФразе" },
   { 0,0,0, F_MACRO_INS,            "Ins"    ,"Вставить"         ,""         , "ВставитьФразуИлиАбзац" },
   { 0,0,0, F_MACRO_DEL,            "Del"    ,"Удалить"          ,""         , "УдалитьФразуИлиАбзац" },

   { 0,0,0, F_MACRO_LANG,           "Tab"    ,"Переключить язык" ,""         , "ПереключениеОригиналПеревод" },
   { 0,0,0, F_MACRO_LEFT,           "Left"   ,"Сдвиг влево"      ,""         , "СдвигФразыВлево" },
   { 0,0,0, F_MACRO_RIGHT,          "Right"  ,"Сдвиг вправо"     ,""         , "СдвигФразыВправо" },

   { 0,0,0, F_MACRO_MAKE,           "F4"     ,""                 ,"Построить", "ПостроитьГипертекст" },
   { 0,0,0, F_MACRO_TRANS,          "F5"     ,""                 ,"Перевести", "ПереводВсегоГипертекста" },
   { 0,0,0, F_END_OF_LIST,          ""       ,"" , "" }
};

t_HotKey2 KeyFrasa[]= /*OK*/
{
   { 0,0,0, F_HELP,                      "F1"     ,""                         ,"Подсказка","Подсказка" },
   { 0,0,0, F_MENU1,                     "F9"     ,""                         ,"Меню"     , "Меню" },
   { 0,0,0, F_OKEY,                      "F10"    ,""                         ,"ВыходОк"  , "ВыходОк" },
   { 0,0,0, F_EXIT,                      "Esc"    ,""                         ,"ВыходEsc" , "ВыходEsc" },

   { 0,0,0, F_FRASA_SRC_UP,          "Ctrl+PgUp"  ,"   Шаг вверх             2" ,""       , "ИсходныйТекстВверх" },
   { 0,0,0, F_FRASA_SRC_DOWN,        "Ctrl+PgDn"  ,"   Шаг вниз              2" ,""       , "ИсходныйТекстВниз" },

   { 0,0,0, F_FRASA_WORD_NEXT,       "Shift+Down" ,"   Шаг вверх             3" ,""       , "СловоПодстрочникаСледующее" },
   { 0,0,0, F_FRASA_WORD_PREV,       "Shift+Up"   ,"   Шаг вниз              3" ,""       , "СловоПодстрочникаПредыдущее" },
   { 0,0,0, F_FRASA_WORD_VARIANT,    "Shift+Enter","Все варианты              " ,""       , "СловоПодстрочникаВарианты" },
   { 0,0,0, F_FRASA_WORD_VARIANT1,   "Alt+Enter"  ,"Вставить перевод          " ,""       , "СловоПодстрочникаВставить1" },
   { 0,0,0, F_FRASA_WORD_VARIANT2,   "Ctrl+Enter" ,"Вставить оригинал         " ,""       , "СловоПодстрочникаВставить2" },

   { 0,0,0, F_FRASA_HUMAN_UP,            "Up"     ,"Шаг вверх        ?" ,""         , "ЧеловеческийСловарьВверх" },
   { 0,0,0, F_FRASA_HUMAN_DOWN,          "Down"   ,"Шаг вниз         ?" ,""         , "ЧеловеческийСловарьВниз" },

   { 0,0,0, F_FRASA_FLAGS,               "F3"     ,"Флаги"                    ,"Флаги"    , "ПоставитьФлагиФразе" },
   { 0,0,0, F_FRASA_UNDO,                "F4"     ,"Отмена"                   ,"Undo"     , "ОтменаРедактирования" },
   { 0,0,0, F_FRASA_DEBUG,               "F5"     ,"Отладчик"                 ,"Отладчик" , "ОкноОтладчик" },
   { 0,0,0, F_FRASA_TRANS,               "F8"     ,"Перевод"                  ,"Перевод"  , "ПереводФразы" },
   { 0,0,0, F_FRASA_NEXT,                "PgDn"   ,"СледФраза"                ,"СледФраза", "СледующаяФраза" },
   { 0,0,0, F_FRASA_PREV,                "PgUp"   ,"ПредФраза"                ,"ПредФраза", "ПредыдущаяФраза" },
   { 0,0,0, F_FRASA_WINDOW_SWITCH,       "Tab"    ,"Переход фокуса"           ,""         , "ПереключитьОкна" },
   { 0,0,0, F_FRASA_GET,                 "Enter"  ,"Взять из словаря"         ,""         , "ВзятьИзЧеловеческогоСловаря2" },
   { 0,0,0, F_FRASA_TODICT,              "Shift+Ins","Перейти в словарь"      ,""         , "ПерейтиВСловарь" },
   { 0,0,0, F_FRASA_SELECT_DICT,         "Shift+Tab","Переключить словарь"    ,"Словарь"  , "ПереключитьСловарь2" },
   { 0,0,0, F_END_OF_LIST,               ""       ,"" , "" }
};

extern t_FrasaFace   Frasa ;
extern t_SetPerevod  SetPerevod ;
extern t_Menu        Kontext_menu[] ;
extern t_Menu        SetPerevod_menu[] ;
extern t_Menu        Macro_menu[] ;
extern t_Menu        Frasa_menu[] ;
extern t_Menu        Manager_menu1[] ;
extern t_Menu        Manager_menu2[] ;

t_String hk_Empty="" ;
t_String Help="[F1] Help" ;

char is_number( char *Str );

/************************************************************************/
//               прочесть и настроить все параметры
/************************************************************************/
int t_Option :: read_all( char *File )
{
    read( File );
    // ------- установить цвета ----------------
    init_colors();
    
    // -------- установить горячие клавиши --------------
    set_hotkeys( KeyKontext , Kontext_menu   , hk_Kontext );
    set_hotkeys( KeyPerevod , SetPerevod_menu, hk_SetPerev );
    set_hotkeys( KeyMacro   , Macro_menu     , hk_Macrostruct );
    set_hotkeys( KeyFrasa   , Frasa_menu     , hk_Frasa );

    set_hotkeys( KeyDictMgr1, Manager_menu1  , hk_Empty );
    set_hotkeys( KeyDictMgr2, Manager_menu2  , hk_Empty );

    // -------- установить конфигурацию окон ------------
    char  Str[200]="" ;
    short sx=80,sy=50 ;
    Strcpy( Str,Option.get("size_x"),200 );
    if( is_number( Str ) && Str[0]!=0 ) sx=atoi(Str);
    Strcpy( Str,Option.get("size_y"),200 );
    if( is_number( Str ) && Str[0]!=0 ) sy=atoi(Str);
    Frasa     .calc_pos( 0,1,sx-1,sy-1 );
    SetPerevod.calc_pos( 0,1,sx-1,sy-2 );

    set_rect1( Frasa.RFrom         ,"ОкноФразаИсходныйТекст" );
    set_rect1( Frasa.RFirst        ,"ОкноФразаПодстрочник" );
    set_rect1( Frasa.RSlowo        ,"ОкноФразаСловарь" );
    set_rect1( Frasa.RTo           ,"ОкноФразаПереведеннаяФраза" );
    set_rect1( Frasa.RVariantView  ,"ОкноФразаВариантПеревода" );
    set_rect1( Frasa.RFormWisard   ,"ОкноФразаКонструкторФорм" );

    set_rect1( SetPerevod.REdit1   ,"ОкноСловоИсходныйТекст" );
    set_rect1( SetPerevod.RFirst   ,"ОкноСловоПодстрочник" );
    set_rect1( SetPerevod.RFrom    ,"ОкноСловоИсточник" );
    set_rect1( SetPerevod.RTo      ,"ОкноСловоПриемник" );
    set_rect1( SetPerevod.RCurrentL,"ОкноСловоТекущийСловарь" );
    set_rect1( SetPerevod.RMainL   ,"ОкноСловоГлавныйСловарь" );
    set_rect1( SetPerevod.RSlowo   ,"ОкноСловоСловарь" );
    set_rect1( SetPerevod.RAddWord ,"ОкноСловоРучноеЗаданиеПеревода" );
 
    return 0 ;
}
/************************************************************************/
//                    прочесть файл ресурсов
/************************************************************************/
int t_Option :: read( char *File )
{  long   i,L ;
   char  *Mass ;
   FILE  *ff ;
   t_OptionParam PP ;

   Param.init() ;
   ff=fopen( File,"r" );
   if( ff==NULL ) return -1 ;
   fclose(ff);
   L=file_to_mass( File,&Mass );

   t_Lexer Lexer ;

   Lexer.set_breaker((uchar *)":");
   Lexer.init( Mass,L );
   Lexer.remark();
   Lexer.make_words();

   for( i=0 ; i<Lexer.n_word(); i++ )
      if( 0==strcmp(Lexer.word(i),":") )
      {   if( i==0 || i==Lexer.n_word()-1 ) continue ;
          Strcpy( PP.Tag  ,Lexer.word(i-1),50 );
	  Strcpy( PP.Value,Lexer.word(i+1),200 );
	  Param.add( PP );
      }
   return 0 ;
}
/************************************************************************/
void t_Option :: init_colors()
{
    set_color1(       fon_Color ,0x0f ,"ЦветФона" );
    set_color1(      text_Color ,0x07 ,"ЦветТекста" );
    set_color1(       buf_Color ,0x70 ,"ЦветБуфера" );
    set_color1(  red_rame_Color ,0x07 ,"ЦветРамки" );
    set_color1(  red_Rame_Color ,0x0f ,"ЦветТекущейРамки" );
    set_color1(     regim_Color ,0xbf ,"ЦветЗаголовкаРежима" );
    set_color1(      titl_Color ,0x0b ,"ЦветЗаголовкаОкна" );
    set_color1(  titlchen_Color ,0x0a ,"ЦветЗаголовкаПодокна" );
    set_color1(    status_Color ,0x70 ,"ЦветСтрокиСостояния" );
    set_color1(   hotkey1_Color ,0xb0 ,"ЦветГорячихКлавиш" );
    set_color1(   hotkey2_Color ,0x3f ,"ЦветГорячихФункций" );
    set_color1(   hotkey3_Color ,0x09 ,"ЦветГорячихКлавиш1" );

    set_color1(      menu_Color ,0x07 ,"ЦветМеню" );
    set_color1( curs_menu_Color ,0x1f ,"ЦветМенюТекущего" );
    set_color1(      help_Color ,0x0f ,"ЦветПодсазки" );
    set_color1( bold_help_Color ,0x0e ,"ЦветПодсказкиВыделенный" );
    set_color1( link_help_Color ,0x0a ,"ЦветПодсказкиПерехода" );
    set_color1(     error_Color ,0x07 ,"ЦветОшибки" );
    set_color1(  cur_item_Color ,0x70 ,"ЦветТекущегоЭлементаСписка" );
    set_color1(  sel_item_Color ,0x0e ,"ЦветВыделенногоЭлементаСписка" );
}
/************************************************************************/
//   установить значание горячих клавиш для одного режима
//   а также скорректировать меню и подсказку 
/************************************************************************/
void t_Option :: set_hotkeys( t_HotKey2 *HotKey, t_Menu *Menu, t_String &Help )
{   char Buf[40] ;

    for( int i=0 ; HotKey[i].Func!=F_END_OF_LIST ; i++ )
    {  
       char *s=get( HotKey[i].Name );
       if( s[0]!=0 )
        	read_hotkey( HotKey[i], s );
	   else read_hotkey( HotKey[i], HotKey[i].DefValue );
       strcpy( Buf,HotKey[i].DefValue );
       set_hotkey0( Buf );
       if( HotKey[i].MenuFind[0]!=0 )
           set_menu_hotkey( Menu, HotKey[i].MenuFind, Buf );
       if( HotKey[i].HelpFind[0]!=0 )
           set_help_hotkey( Help, HotKey[i].HelpFind, Buf );
    }
}
/************************************************************************/
void t_Option :: set_color1( char &Color, char Default, char *Name )
{
    Color=Default ;    
    char *s=get( Name );
    if( s[0]!=0 ) 
    {   int Color1=tcolor( s );
        if( 0<=Color1 ) Color=Color1 ;
    }
}
/************************************************************************/
//                  сделать из строки "0x0f" цвет 0x0f
/************************************************************************/
short t_Option :: tcolor( char *str )
{  short f1=0,f2=7 ;

   if( str[0]!='0' || str[1]!='x' ) return -1 ;
   if( '0'<=str[2] && str[2]<='9' ) f1=str[2]-'0' ;
   if( 'a'<=str[2] && str[2]<='f' ) f1=str[2]-'a'+10 ;
   if( 'A'<=str[2] && str[2]<='F' ) f1=str[2]-'A'+10 ;
   if( '0'<=str[3] && str[3]<='9' ) f2=str[3]-'0' ;
   if( 'a'<=str[3] && str[3]<='f' ) f2=str[3]-'a'+10 ;
   if( 'A'<=str[3] && str[3]<='F' ) f2=str[3]-'A'+10 ;
   return (f1<<4)+f2 ;
}
/************************************************************************/
void t_Option :: set_rect1( t_Rect &Rect, char *Name )
{
   char    f ;
   char   *s=get( Name );

   if( s[0]!=0 ) 
   {   t_Rect  R ;
       f=trect( R, s );
       if( 0==f ) Rect=R ;
   }
}
/************************************************************************/
//            сделать прямоугольник из строки "0,12,80,24"
/************************************************************************/
char t_Option :: trect( t_Rect &Rect, char *Str )
{  
   short i,j,n ;
   char  c ;
   char  Str1[10] ;

   // -------- проверить на корректность --------------
   for( n=i=0 ; Str[i]!=0 ; i++ )
   {  c=Str[i] ;
      if( c!=',' && 0==isdigit(c) ) return -1 ;
      if( c==',' ) n++ ;
   }
   if( n!=3 ) return -1 ;

   for( i=j=n=0 ; Str[i]!=0 ; i++ )
   {  c=Str[i] ;
      if( c==',' )
      {   Str1[j]=0 ;
          if( n==0 ) Rect.x1=atoi( Str1 );
          if( n==1 ) Rect.y1=atoi( Str1 );
          if( n==2 ) Rect.x2=atoi( Str1 );
	  n++ ; j=0 ;
	  continue ;
      }       
      Str1[j++]=c ;
   }
   Str1[j]=0 ;
   Rect.y2=atoi( Str1 );    
   return 0 ;
}
/************************************************************************/
//                  дать параметр Tag - имя параметра 
/************************************************************************/
char * t_Option :: get( char *Tag )
{  long i ;

   for( i=0 ; i<Param.j ; i++ )
      if( 0==strcmp(Tag,Param[i].Tag) )
          return Param[i].Value ;
    return "" ;
}
/************************************************************************/
e_Function t_Option :: get_func( t_HotKey2 *HotKey1,
                                 short Shift, short key1, short key2 )
{  long i ;

   for( i=0 ; F_END_OF_LIST!=HotKey1[i].Func ; i++ )
      if( HotKey1[i].Key.SStatus==Shift &&
          HotKey1[i].Key.key1   ==key1  &&
	  HotKey1[i].Key.key2   ==key2 ) 
	  return HotKey1[i].Func ;
   return F_END_OF_LIST ;
}
/************************************************************************/
//     сделать клавишу из строки подсказки (для тыканья мышкой)
/************************************************************************/
void t_Option :: get_key( ushort *ZZ, ushort *key1, ushort *key2, char *Str, short x )
{
   t_HotKey Key ;

   get_key( Key, Str, x );
   *ZZ  =Key.SStatus ; 
   *key1=Key.key1 ; 
   *key2=Key.key2 ;
}
/************************************************************************/
//     сделать клавишу из строки подсказки (для тыканья мышкой)
/************************************************************************/
void t_Option :: get_key( t_HotKey &Key, char *Str, short x )
{  short i,j ;
   char  Str1[40] ;

   Key.SStatus=0 ; Key.key1=0 ; Key.key2=0 ;
   // убрать рассогласование за счет скобочек[]
   for( i=0 ; i<x ; i++ )
   {  if( Str[i]==0 ) return ;
      if( Str[i]=='[' || Str[i]==']' ) x++ ;
   }
   for( i=x ; 0<i ; i-- )
      if( Str[i]=='[' ) break ;
   if( Str[i]!='[' ) return ;
   for( j=0,i++ ; Str[i]!=0 ; i++ )
   {  if( Str[i]==']' ) break ;
      Str1[j++]=Str[i] ;
   }
   Str1[j]=0 ;
   read_hotkey( Key, Str1 );
}
/************************************************************************/
//           поменять хоткей в строке подсказки
/************************************************************************/
char t_Option :: set_help_hotkey( t_String &Str, char *Funk, char *Key )
{
   char  cStr[100] ; 
   strcpy( cStr,Str.c_str() );
   int   LStr =strlen( cStr );
   int   LFunk=strlen( Funk );

   for( int i=0 ; i<LStr ; i++ )
   {  if( 0==strncmp(cStr+i,Funk,LFunk) )
      {  for( ; 0<=i ; i-- )
         {  if( cStr[i]=='[' )
	    {
	       t_String Str1="" ;

	       cStr[i+1]=0 ;
	       Str1+=cStr ;
	       Str1+=Key ;
	       for( ; i<LStr ; i++ )
	       {  if( cStr[i]==']' )
		  {   Str1+=cStr+i ;
		      Str=Str1 ;
		      return 0 ;
		  }
	       }
	    }
         }
      }
   }
   return -1 ;
}
/************************************************************************/
//      внести вменю подсказку о горячих клавишах (только внести)
/************************************************************************/
char t_Option :: set_menu_hotkey1( char *Menu, char *Key )
{
   int L =strlen( Menu );
   int L1=strlen( Key );
   if( (L/2)<L1 ) return -1 ;
   strcpy( Menu+L-L1,Key );
   return 0 ;
}
/************************************************************************/
//      внести в меню подсказку о горячих клавишах найти и внести
/************************************************************************/
char t_Option :: set_menu_hotkey( t_Menu *Menu, char *sFind, char *HotKey )
{  short i,L ;

   L=strlen( sFind );
   for( i=0 ; i<100 ; i++ )
   {  if( Menu[i].flag==F_ENDE ) break ;
      if( 0==strncmp(Menu[i].name,sFind,L) )
      {   set_menu_hotkey1( Menu[i].name,HotKey );
          return 1 ;
      }	  
      if( Menu[i].flag==F_MENU )
          if( 1==set_menu_hotkey( (t_Menu *)Menu[i].data,sFind, HotKey ) ) return 1;
   }
   return 0 ;
}
/************************************************************************/
//      сделать из строки вида "Shift+Up" горячую клавишу
/************************************************************************/
char t_Option :: read_hotkey( t_HotKey2 &Hotkey, char *Str )
{
   int  i,j ;
   char Str1[40] ;

   strcpy( Hotkey.DefValue,Str );
   for( i=j=0 ; Str[i]!=0 ; i++ )
   {
      if( Str[i]=='+' )
      {   Str1[j]=0 ; j=0 ;
          set_hotkey1( Hotkey.Key, Str1 );
	  continue ;
      }
      Str1[j++]=Str[i] ;
   }
   Str1[j]=0 ;
   set_hotkey1( Hotkey.Key, Str1 );
   return 0 ;
}
/************************************************************************/
//      сделать из строки вида "Shift+Up" горячую клавишу (облегченная)
/************************************************************************/
char t_Option :: read_hotkey( t_HotKey &Key, char *Str )
{
   int  i,j ;
   char Str1[40] ;

   Key.SStatus=0 ; Key.key1=0 ; Key.key2=0 ;

   for( i=j=0 ; Str[i]!=0 ; i++ )
   {
      if( Str[i]=='+' )
      {   Str1[j]=0 ; j=0 ;
          set_hotkey1( Key, Str1 );
	  continue ;
      }
      Str1[j++]=Str[i] ;
   }
   Str1[j]=0 ;
   set_hotkey1( Key, Str1 );
   return 0 ;
}
/************************************************************************/
//              Укоротить модификаторы "Shift" -> "^S"
/************************************************************************/
void t_Option :: set_hotkey0( char *Str )
{  int i,j ;

   for( i=0 ; Str[i]!=0 ; i++ )
   {  if( 0==strncmp( Str+i,"Shift",5 ) ) strncpy( Str+i,"^S   ",5 );
      if( 0==strncmp( Str+i,"Ctrl" ,4 ) ) strncpy( Str+i,"^C  " ,4 );
      if( 0==strncmp( Str+i,"Alt"  ,3 ) ) strncpy( Str+i,"^A "  ,3 );
   }
   for( i=j=0 ; Str[i]!=0 ; i++ )
      if( Str[i]!=' ' )
          Str[j++]=Str[i] ;
   Str[j]=0 ;
}
/************************************************************************/
//           установить горячкей клавише одну из компонент
/************************************************************************/
void t_Option :: set_hotkey1( t_HotKey &Key, char *Str )
{
   // --------- просто буквы ------------------------------
   if( 0==Str[1] )
   {   Key.key1=Str[0] ; return ; }

   // -------- шифтстатусы -------------------------------
   if( 0==strcmp( Str,"Shift" ) ) {  Key.SStatus=S_Shift_L ; return ; }
   if( 0==strcmp( Str,"Ctrl"  ) ) {  Key.SStatus=S_Ctrl_L  ; return ; }
   if( 0==strcmp( Str,"Alt"   ) ) {  Key.SStatus=S_Alt_L   ; return ; }

   // -------- служебные клавиши -------------------------
   if( 0==strcmp( Str,"Esc"   ) ) {  Key.key1=S_key_Esc ; return ; }
   if( 0==strcmp( Str,"F1"    ) ) {  Key.key2=S_key_F1 ;  return ; }
   if( 0==strcmp( Str,"F2"    ) ) {  Key.key2=S_key_F2 ;  return ; }
   if( 0==strcmp( Str,"F3"    ) ) {  Key.key2=S_key_F3 ;  return ; }
   if( 0==strcmp( Str,"F4"    ) ) {  Key.key2=S_key_F4 ;  return ; }
   if( 0==strcmp( Str,"F5"    ) ) {  Key.key2=S_key_F5 ;  return ; }
   if( 0==strcmp( Str,"F6"    ) ) {  Key.key2=S_key_F6 ;  return ; }
   if( 0==strcmp( Str,"F7"    ) ) {  Key.key2=S_key_F7 ;  return ; }
   if( 0==strcmp( Str,"F8"    ) ) {  Key.key2=S_key_F8 ;  return ; }
   if( 0==strcmp( Str,"F9"    ) ) {  Key.key2=S_key_F9 ;  return ; }
   if( 0==strcmp( Str,"F10"   ) ) {  Key.key2=S_key_F10 ; return ; }
   if( 0==strcmp( Str,"F11"   ) ) {  Key.key2=S_key_F11 ; return ; }
   if( 0==strcmp( Str,"F12"   ) ) {  Key.key2=S_key_F12 ; return ; }

   if( 0==strcmp( Str,"Enter" ) ) {  Key.key1='\r'        ; return ; }
   if( 0==strcmp( Str,"Tab"   ) ) {  Key.key1='\t'        ; return ; }
   if( 0==strcmp( Str,"Back"  ) ) {  Key.key1=S_key_Back  ; return ; }
   if( 0==strcmp( Str,"Ins"   ) ) {  Key.key2=S_key_Ins   ; return ; }
   if( 0==strcmp( Str,"Del"   ) ) {  Key.key2=S_key_Del   ; return ; }
   if( 0==strcmp( Str,"Home"  ) ) {  Key.key2=S_key_Home  ; return ; }
   if( 0==strcmp( Str,"End"   ) ) {  Key.key2=S_key_End   ; return ; }
   if( 0==strcmp( Str,"PgUp"  ) ) {  Key.key2=S_key_PgUp  ; return ; }
   if( 0==strcmp( Str,"PgDn"  ) ) {  Key.key2=S_key_PgDn  ; return ; }
   if( 0==strcmp( Str,"Left"  ) ) {  Key.key2=S_key_Left  ; return ; }
   if( 0==strcmp( Str,"Right" ) ) {  Key.key2=S_key_Right ; return ; }
   if( 0==strcmp( Str,"Up"    ) ) {  Key.key2=S_key_Up    ; return ; }
   if( 0==strcmp( Str,"Down"  ) ) {  Key.key2=S_key_Down  ; return ; }
   if( 0==strcmp( Str,"Space" ) ) {  Key.key2=S_key_space ; return ; }
   printf( "Option:Строчка %s не распозналась",Str );
}

DEFINE_LIST_BTYPE( t_OptionParam,t_OptionParamList )
