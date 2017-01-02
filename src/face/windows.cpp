/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//     t_Windows - главный класс интерфейса (ничего общего с Микрософтом)
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <string.h>
# include <stdlib.h>
# include <unistd.h>

# include "mylib.h"
# include "core.h"
# include "video.h"
# include "common.h"
# include "option.h"
# include "menu.h"
# include "edit.h"
# include "edit1.h"
# include "edit2.h"
# include "debug.h"
# include "word.h"
# include "slowo1.h"
# include "macrostruct.h"
# include "kontext2.h"
# include "windows.h"

void  seans_read( void *Void );
void  seans_write( void *Void );
void  hiper_read( void *Void );
void  hiper_write( void *Void );
void  kontext_read( void *Void );
void  kontext_write( void *Void );
void  dict_mgr( void *Void );
void  WW_write_perevod( void *Void );

extern t_Menu Main_menu[] ;

// ----------- меню для режима besser --------------------------------------
t_Menu file_menu1[] =
{
   { F_FUNK  ,0,WW_create    ,(void *)"k" ,"Открыть    koi8-r" },
   { F_FUNK  ,0,WW_create    ,(void *)"w" ,"Открыть    cp1251" },
   { F_FUNK  ,0,WW_create    ,(void *)"1" ,"Открыть   Latin_1" },
   { F_FUNK  ,0,WW_create    ,(void *)"2" ,"Открыть   Latin_2" },
   { F_FUNK  ,0,WW_create    ,(void *)"3" ,"Открыть   Latin_3" },
   { F_FUNK  ,0,WW_create    ,(void *)"4" ,"Открыть   Latin_4" },
   { F_FUNK  ,0,WW_write_perevod,NULL     ,"Записать   koi8-r" },
   { F_SCHREI,0,NULL         ,NULL        ,"-----------------" },
   { F_FUNK  ,0,WW_read_langtxt ,NULL     ,"прочесть lang.txt" },
   { F_FUNK  ,0,WW_read_alldata ,NULL     ,"прочесть все даные" },
   { F_FUNK  ,0,WW_exit      ,NULL        ,"Выход            " },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};
t_Menu image_menu[] =
{
   { F_FUNK  ,0,seans_read   ,NULL   ,"Прочитать образ сеанса" },
   { F_FUNK  ,0,seans_write  ,NULL   ,"Записать  образ сеанса" },
   { F_SCHREI,0,NULL         ,NULL   ,"-----------------------" },
   { F_FUNK  ,0,Sorry        ,NULL   ,"Записать  словарь" },
   { F_FUNK  ,0,dict_mgr     ,NULL   ,"Конструктор словаря" },
   { F_ENDE  ,0,NULL,NULL        ,"" }
};

t_Menu help_besser[] =
{  { F_FUNK  ,0,Window_help  ,(void *)"help.html"       ,"О Программе   " },
   { F_FUNK  ,0,Window_help  ,(void *)"index.html"      ,"Содержание    " },
   { F_FUNK  ,0,Window_help  ,(void *)"besser.html"     ,"Об этом режиме" },
   { F_FUNK  ,0,Window_help  ,(void *)"menu_besser.html","Меню          " },
   { F_FUNK  ,0,Window_help  ,(void *)"tip0.html"       ,"Полезные советы" },
   { F_FUNK  ,0,Sorry        ,(void *)"Настройка"       ,"Настройка" },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};

t_Menu Besser_menu[] =
{  { F_MENU  ,0,NULL,file_menu1  ,"Файлы"    },
   { F_MENU  ,0,NULL,image_menu  ,"Образы"   },
   { F_SCHREI,0,NULL,NULL        ,"Направление" },
   { F_SCHREI,0,NULL,NULL        ,"Вых. формат" },
   { F_MENU  ,0,NULL,help_besser ,"Подсказка"},
   { F_ENDE  ,0,NULL,NULL        ,"" }
};

char hk_Edit[]  ="[F1]Подсказ[F2]Словарь[^C ->]Фраза+1[^C <-]Фраза-1[F7]Пер.текста[F8]Пер.фразы[F9]Варианты[F10]Меню" ;
char hk_Help[]  ="[Enter] Переход по ссылке [F2] Содержание [F3] Взад [F10] Возврат в основной режим" ;
char hk_Path[]  ="[F1] Подсказка [Tab] переход между окнами [Стрелки] хождение по дереву вариантов [F10] Выход" ;
char hk_Slowo[] ="[F1] Подсказка [Стрелки] выбрать [Shift+PgDn/PgUp] След/пред слово [Esc] стереть слово [F10] Выход" ;
char hk_Word[]  ="[F1] Подсказка [^C <-] След режим [^C ->] пред режим [->] Выбор структуры [F10] Выход" ;
char hk_Word1[] ="[F1] Подсказка [<- ->] След/пред фраза [Enter] Задать перевод [F10] Выход" ;

char hk_First[] ="[F1] Подсказка [F3] Источник [F4] Приемник [F5] Отладчик [Enter] Подробнее [F10]Выход" ;
char hk_Sou[]   ="[F1] Подсказка [F2] Первое чтение [F4] Приемник [F5] Отладчик [F10] Выход" ;
char hk_Dst[]   ="[F1] Подсказка [F2] Первое чтение [F3] Источник [F5] Отладчик [F10] Выход" ;
char hk_Debug1[]="[F1] Подсказка [<- ->] След/пред слово [Пробел] полый/сокр список [Enter] Список вариантов [F10] Выход";
char hk_Debug2[]="[F1] Подсказка [Enter] Структура варианта [<-|] возврат в главную таблицу [F10] Выход" ;
char hk_Debug3[]="[F1] Подсказка [Enter] шаг вниз [<-|] шаг вверх по дереву конструкции [F10] Выход" ;
char hk_Gramm[] ="[F1]Подсказка[Enter]вниз[<-|]вверх по дереву грамматики[Tab]Источник/приемник[F8]Параметры[F10]Выход" ;
char hk_Perev[] ="[F1]Подсказка [Enter]шаг вниз [<-|]шаг вверх по дереву словаря [F10]Выход" ;
char hk_Dicts[] ="[F1]Подсказка [Enter] просмотреть список форм словаря [F10]Выход" ;
char hk_Format[]="[F1]Подсказка [F4] Деформатир.[F5] Форматировать [F7] Фраза [F8] Абзац [F9] Меню [F10] Выход" ;
char hk_FrasaEdit[]="[F1] Подсказка [Tab] Переключение [F10] Выход" ;
char hk_DictMgr[]="[F1] Подсказка [F5] Копия [F9] Меню [F10] Выход" ;

t_String  hk_Kontext    ="[F1]Подсказка[F2]Использовать[F3]Фильтры[F4]Построить[F7]Поиск[Enter]Слово[F9]Меню[F10]Выход" ;
t_String  hk_SetPerev   ="[F1]Подсказка[^C+Del]Удалить[^A+Ins]Добавить[F6]Создать[Enter]Взять[F9]Меню[F10]Выход" ;
t_String  hk_Macrostruct="[F1]Подсказка[F3]Флаги[F4]Построить[F5]Перевести[F6]ПерАвт[Enter]ПерРуч[F9]Меню[F10]Выход" ;
t_String  hk_Frasa      ="[F1]Подсказка[F3]Флаги[F4]Отмена[F5]Отладчик[F8]Перевод[PgUp]Пред[PgDn]След фраза[F9]Меню[F10]Выход" ;
t_String  hk_Frasa1     ="[F1]Подсказка[Enter]Выбрать[^S+Enter]Формы[F10]Выход" ;
t_String  hk_Frasa2     ="[F1]Подсказка[<- ->]След/пред параметр[AV]значение параметра[Enter]Выбрать[F10]Выход" ;
char      hk_Besser[]   ="[F1]Подсказка [F9] Меню" ;

t_Dir          Dir ;
t_HelpData     HelpData ;
t_Text         HelpText,ErrorText ;
t_EditTrans    HelpView,ErrorView ;
t_EditTrans    Edit1,Edit2 ;
t_FirstLook    EditFrom ;
t_Edit3        EditTree1(0),EditTree2(1) ;
t_Debug        Debug ;
t_Core1View    Core1View ;
t_GrammarView  GrammarView ;
t_SlowoView    SlowoView ;
t_Slowo3View   Slowo3View ;
t_Slowo3View1  Slowo3View1 ;
t_Macrostruct  Macrostruct ;
t_Macrostruct2 Macrostruct2 ;
t_Kontext      Kontext ;
t_Kontext2     Kontext2 ;
t_Slowo1       Slowo ;
t_Option       Option ;
t_DirTrans    *DirFormat ;
short          n_DirFormat ;

char   Reg  ;   // текущее окно
char   Reg1 ;   // предыдущее окно
t_String       MercuryDir="../" ;

// ----------- статусы --------------------------------
t_String       Direct   ; // направление перевода
t_String       FileName ; // имя прочитанного файла
t_String       DicName  ; // имя прочитанного контекстного словаря
t_String       FormatName; // имя выходного формата

void select_lang( void *S );
void select_format( void *S );

/***************************************************************************/
//          функция определения "слова с точкой"
/***************************************************************************/
char   is_const( char *Str )
{
   short       part[20] ;
   if( 0<Perevod->part( Str,part ) ) return 1 ;
   return 0 ;
}
/***************************************************************************/
void WW_init( void )
{
   Reg='1' ;

   HelpView.set_text( &HelpText );
   HelpView.set_view( 1 ) ;
   ErrorView.set_text( &ErrorText );
   ErrorView.set_view( 1 ) ;
   Macrostruct.Text=&Edit1.Text1 ;

   Edit1.set_name("Исходный_текст");
   Edit2.set_name("Переведенный_текст");
   EditFrom.set_name("Первое чтение");
   EditTree1.set_name("Структура источника");
   EditTree2.set_name("Структура приемника");
   HelpView.set_name("Подсказка");
   ErrorView.set_name("Ошибки описания грамматики");
   Slowo.set_name("Словарь");
   Edit1.set_funk_const( &is_const );
}
/***************************************************************************/
void WW_set_pos( void )
{
   short sx,sy,y ;
   s_get_size( sx,sy );
   y=sy/2 ;

   Edit1.set_pos      ( 0, 1  , sx-1, y    );
   Edit2.set_pos      ( 0, y+1, sx-1, sy-2 );
   EditFrom.set_pos   ( 1, 2  , sx-2, sy-2 );
   EditTree1.set_pos  ( 1, 2  , sx-2, sy-2 );
   EditTree2.set_pos  ( 1, 2  , sx-2, sy-2 );
   Debug.set_pos      ( 1, 2  , sx-2, sy-2 );
   Core1View.set_pos  ( 1, 2  , sx-2, sy-2 );
   GrammarView.set_pos( 1, 2  , sx-2, sy-2 );
   SlowoView.set_pos  ( 1, 2  , sx-2, sy-2 );
   Slowo3View.set_pos ( 1, 2  , sx-2, sy-2 );
   Slowo3View1.set_pos( 1, 2  , sx-2, sy-2 );
   HelpView.set_pos   ( 1, 3  , sx-2, sy-3 );
   ErrorView.set_pos  ( 1, 2  , sx-2, sy-2 );
}
/***************************************************************************/
void WW_paint( void )
{  
   short sx,sy,yy ;
   static short sx1=80,sy1=25 ;

   s_get_size( sx,sy );
   yy=sy/2 ;
   s_rame2_f( 1,0,sy-1,sx-1,red_rame_Color );
   if( sx!=sx1 || sy!=sy1 )
   {  // ------------ переразместить все окошки ----------------
      WW_set_pos( );
      null_menu( Main_menu );
   }
   Edit1.f_2Rame=Edit2.f_2Rame=0 ; ;
   if( Reg=='1' ) Edit1.f_2Rame=1 ;
   if( Reg=='2' ) Edit2.f_2Rame=1 ;
   Edit1.paint();
   Edit2.paint();
}
/***************************************************************************/
void WW_main_loop( void ) 
{  e_WinMsg r ;

   WW_init();
   WW_set_pos();
   null_menu( Main_menu );

   while( 1 )
   {
      WW_paint( );
      switch( Reg )
      {  case '1': r=Edit1.main_loop( ); Reg1=Reg ; break ;
	 case '2': r=Edit2.main_loop( ); Reg1=Reg ; break ;
	 case '3': r=EditFrom.loop( );   break ;
	 case '4': r=EditTree1.loop( );  break ;
	 case '5': r=EditTree2.loop( );  break ;
	 case '6': r=Debug.loop( );      break ;
         case 'f': r=Core1View.loop();   break ;
	 case '7': r=GrammarView.loop( );break ;
	 case '8': r=SlowoView.loop( );  break ;
	 case '9': r=Slowo3View.loop( ); break ;
	 case 'a': r=Slowo3View1.loop( );break ;
	 case 's': r=WW_win_slowo( );    break ;
	 case 'e': r=ErrorView.main();   break ;
      }      
      switch( r )
      {
         case WM_NEXT :
	      if( Reg=='1' ) { Reg='2' ; break ; }
	      if( Reg=='2' ) { Reg='1' ; break ; }
         case WM_ESC    :
	      if( Reg=='1' || Reg=='2' ) return ;
	      Reg=Reg1 ; 
	      break ;
	 case WM_EDIT1  : Reg='1' ; break ;
	 case WM_EDIT2  : Reg='2' ; break ;
	 case WM_FIRST  : Reg='3' ; break ;
	 case WM_SRC    : Reg='4' ; break ;
	 case WM_DST    : Reg='5' ; break ;
	 case WM_DEBUG  : Reg='6' ; break ;
	 case WM_DEBUG1 : Reg='f' ; break ;
	 case WM_GRAMMAR: Reg='7' ; break ;
	 case WM_SLOWO  : Reg='8' ; break ;
	 case WM_BASE   : Reg='9' ; break ;
	 case WM_BASE1  : Reg='a' ; break ;
	 case WM_SLOWO1 : Reg='s' ; break ;
	 case WM_ERROR  : Reg='e' ; break ;
      }
   }
}
/***************************************************************************/
//                  сделать или прочитать файл
//      void *Void - кодовая страница
/***************************************************************************/
void WW_create( void *Void )
{  char f ;
   t_String Buf ;
   char    *CodePage=(char *)Void ;
   e_Latin  CodePage1 ;

try
{  
   switch( CodePage[0] )
   {   case 'k' : CodePage1=KOI8   ; break ;
       case 'w' : CodePage1=CP1251 ; break ;
       case '1' : CodePage1=LATIN1 ; break ;
       case '2' : CodePage1=LATIN2 ; break ;
       case '3' : CodePage1=LATIN3 ; break ;
       case '4' : CodePage1=LATIN4 ; break ;
       default  : CodePage1=KOI8   ; break ;
   }
   
   f=Dir.main_loop( Buf,0 );
   if( f==0 ) Edit1.read( Buf.c_str(), CodePage1 );
   if( f>=0 ) {  Edit1.set_name( Buf.c_str() ); FileName=Buf ; }

   Edit1.paint();
   Edit2.paint();
}
catch( ... )
{   WW_paint();
    figna( error_get() );
}
}
/***************************************************************************/
void WW_save_src( void *Void )
{  char Buf[200] ;

   //Edit1.write_file( Edit1.Name ,f_cp1251 );
   Edit1.write( Edit1.Name, KOI8 );
   sprintf( Buf,"Файл \"%s\" сохранен",Edit1.Name );
   figna( Buf );
}
/***************************************************************************/
//                 записать файл перевода
/***************************************************************************/
void WW_save( void *Void )
{
   //Edit2.write_file( "mercury_out.txt",f_cp1251 );
   Edit2.write( Edit1.Name, KOI8 );
   figna( "Файл \"mercury_out.txt\" сохранен" );
}
/***************************************************************************/
//                 записать файл перевода (под именем)
/***************************************************************************/
void WW_save_as( void *Void )
{  char f ;
   t_String Buf ;

   f=Dir.main_loop( Buf,1 );   
   if( f<0 )
       Buf="mercury_out.txt" ;
   Edit2.set_name( Buf.c_str() );
//   Edit2.write_file( Buf, f_cp1251 );
   Edit2.write( Edit1.Name, KOI8 );

   WW_paint( );
}
/***************************************************************************/
//           прочесть файл lang.txt (описание грамматики)
/***************************************************************************/
void WW_read_langtxt( void *Void )
{
    null_menu( Main_menu );
    message("Идет загрузка данных.\n        Ждите...");
    s_refresh();
    if( 0>reread_grammar( MercuryDir.c_str(),z_DirTrans ))
    {    ErrorView.text()->read_file("mercury.err");
	 ErrorView.main( );
    }
    else ErrorView.init("");
    Direct=z_DirTrans->Name ;
    WW_paint();
}
/***************************************************************************/
//           прочесть все данные (грамматику и словари)
/***************************************************************************/
void WW_read_alldata( void *Void )
{
    select_lang( z_DirTrans );
}
/***************************************************************************/
void WW_exit( void *Void )
{
   if( 1==fragen( "Запомнить файл перевода? (1-Да 0-Нет)"  ) ) 
       WW_write_perevod( NULL ) ;

   s_nacht( ) ;
   s_redraw( );
   s_end_schone( );
   ::exit(0);
}
/***************************************************************************/
//                    поиск-замена в тексте
/***************************************************************************/
void WW_replace3( void *Void )
{  
   if( Reg=='1') Edit1.replace3( );
   if( Reg=='2') Edit2.replace3( );
}
/***************************************************************************/
# ifdef FIND1
void t_Windows :: WW_replace2( void *Void )
{  
   if( Reg=='1') Edit1.replace2( );
   if( Reg=='2') Edit2.replace2( );
}
# endif
/***************************************************************************/
//                 перейти в такую-то строку
/***************************************************************************/
void WW_goto_str( void *Void )
{
   if( Reg=='1' ) Edit1.goto_str( );
   if( Reg=='2' ) Edit2.goto_str( );
}
/***************************************************************************/
//                     перевести весь текст
/***************************************************************************/
void WW_trans( void *Void )
{  long  i,L ;
   char *Str,Str1[1000],Str2[50] ;
   char *Text ;
   
   if( Read_Error!=0 )
   {  figna("Грамматика или словари не прочитаны.\n       Перевод невозможен!!!");
      return ;
   }
   Text=(char *)Calloc( 1000000,sizeof(char) );

   L=Edit1.text()->length() ;
   for( i=0 ; i<L ; i=Edit1.text()->next_frasa(i) )
   {  sprintf( Str2,"Переведено %d/%d",i,L );
      message( Str2 );
      Str =Edit1.get_frasa(i);
      Core.translate( Str,Str1 );
      strcat( Text, Str1 );
      strcat( Text, "\n" );
   }
   Edit2.init( Text );
   Free( Text );
   
   Edit1.paint();
   Edit2.get_str();
   Edit2.paint();
}
/***************************************************************************/
//                       перевести одну фразу
/***************************************************************************/
void WW_trans_frasa( void *Void )
{  
   char *Str ;
   long p ;
   static char Str1[1000] ;

   if( Read_Error!=0 )
   {  figna("Грамматика или словари не прочитаны.\n       Перевод невозможен!!!");
      return ;
   }
   p=Edit1.current_pos( ) ;
   Str =Edit1.get_frasa( p );
   Core.translate( Str,Str1 );
   Edit2.put_frasa( Edit2.current_pos( ),Str1 );

   Edit1.paint();
   Edit2.get_str();
   Edit2.paint();
}
/***************************************************************************/
//                   сделать перевод-билингву
/***************************************************************************/
void WW_trans_bilingvo( void *Void )
{  long  i,i1,i2,z,L ;
   char *Str,Str1[1000],Str2[50] ;
   char *Text ;
   
   if( Read_Error!=0 )
   {  figna("Грамматика или словари не прочитаны.\n       Перевод невозможен!!!");
      return ;
   }
   Text=(char *)Calloc( 1000000,sizeof(char) );

   L=Edit1.text()->length() ;
   for( i=0 ; i<L ; )
   {  z=i ;
      // ----- вставить пять фраз на языке оригинала
      strcat( Text, "\n" );
      for( i1=0 ; i1<5 && i<L ; i1++ )
      { 
         Str =Edit1.get_frasa(i);
	 for( i2=0 ; Str[i2]!=0 ; i2++ )
	    if( Str[i2]=='\n' ) Str[i2]=' ' ;
	 // ----- искривление фразы --------
	 long i2,z2 ;
	 char c ;
	 for( i2=z2=0 ; Str[i2]!=0 ; i2++ )
	 {  c=Str[i2] ;
	    if( 80<=(i2-z2) && (c==' ' || c=='\t') )
        	Str[i2]='\n' ;
	    if( Str[i2]=='\n' ) z2=i2 ;
	 }
	 strcat( Text, Str );
	 strcat( Text, "\n" );
         i=Edit1.text()->next_frasa(i);
      }
      // ----- вставить пять фраз на языке перевода
      strcat( Text, "\n" );
      i=z ;
      for( i1=0 ; i1<5 && i<L ; i1++ )
      {  sprintf( Str2,"Переведено %d/%d",i,L );
	 message( Str2 );
	 Str =Edit1.get_frasa(i);
	 Core.translate( Str,Str1 );
	 // ----- искривление фразы --------
	 long i2,z2 ;
	 char c ;
	 for( i2=z2=0 ; Str1[i2]!=0 ; i2++ )
	 {  c=Str1[i2] ;
	    if( 80<=(i2-z2) && (c==' ' || c=='\t') )
        	Str1[i2]='\n' ;
	    if( Str1[i2]=='\n' ) z2=i2 ;
	 }
	 
	 strcat( Text, Str1 );
	 strcat( Text, "\n" );
	 i=Edit1.text()->next_frasa(i);
      }
   }
   Edit2.init( Text );
   Free( Text );
   
   Edit1.paint();
   Edit2.get_str();
   Edit2.paint();
}
/***************************************************************************/
//                    записать перевод гипертекста
/***************************************************************************/
void WW_write_perevod( void *Void )
{  char f,f_CFNeed ;
   t_String Buf ;

   f_CFNeed=Dir.f_CFNeed ;
   Dir.f_CFNeed=1 ;
   f=Dir.main_loop( Buf,1 );
   Dir.f_CFNeed=f_CFNeed ;

   if( f<0 )
      Buf="mercury_out.txt" ;

   select_format( (void *)(&DirFormat[Dir.i_Format]) );
   Macrostruct.print_result( Buf.c_str() );
}
/***************************************************************************/
//               нарисовать меню в режиме "по-нормальному"
/***************************************************************************/
void paint_besser( short x1, short y1, short x2, short y2, short Reg )
{  char  f ;

   s_nacht();
   null_menu( Besser_menu );
   s_rame2_F( y1, x1, y2, x2, 0x70 );
   if( 1==Reg ) f=0x1f ; else f=0x70 ;
   s_text_yxf( y1+1, x1+1, f, "Форматирование текста  " );
   if( 2==Reg ) f=0x1f ; else f=0x70 ;
   s_text_yxf( y1+2, x1+1, f, "Контекстный словарь    " );
   if( 3==Reg ) f=0x1f ; else f=0x70 ;
   s_text_yxf( y1+3, x1+1, f, "Перевод фраза за фразой" );
   if( 4==Reg ) f=0x1f ; else f=0x70 ;
   s_text_yxf( y1+4, x1+1, f, "Перевод как в Стилусе" );

   t_String Str ;
   Str="Направление= " ; Str+=Direct ;     s_text_yxf( n_Line-5, 5, 0x07, Str.c_str() );
   Str="Файл       = " ; Str+=FileName ;   s_text_yxf( n_Line-4, 5, 0x07, Str.c_str() );
   Str="Словарь    = " ; Str+=DicName ;    s_text_yxf( n_Line-3, 5, 0x07, Str.c_str() );
   Str="Формат     = " ; Str+=FormatName ; s_text_yxf( n_Line-2, 5, 0x07, Str.c_str() );
   
   paint_hotkey( n_Line-1, 0, hk_Besser );
}
/***************************************************************************/
//               работа в режиме "по-нормальному"
/***************************************************************************/
void WW_trans_besser( void *Void )
{
   short    r1 ;
   short    Reg=1 ;
   ushort   ZZ,key1,key2 ;
   e_sEvent Event ;
   short    sx,sy,x1,y1,x2,y2,mouse_x,mouse_y ;

   null_menu( Besser_menu );
   
   Direct=DirTrans[0].Name ; // безобразно грубый хак

   while( 1 )
   {
       s_get_size( sx, sy );
       x1=sx/2-12 ; x2=sx/2+12 ;
       y1=sy/2-2  ; y2=sy/2+3 ;
       paint_besser( x1,y1,x2,y2,Reg );

       Event=s_getch( &key1,&key2 );

      if( MOUSE_PRESS==Event )
      {
          s_get_mouse( &mouse_x, &mouse_y );
          if( mouse_y==0 ) goto M_Menu ;
	  if( mouse_y==n_Line-1 )
	      Option.get_key( &ZZ, &key1, &key2, hk_Besser, mouse_x );
          if( x1<mouse_x && mouse_x<x2 && y1<mouse_y && mouse_y<y2 )
	  {   Reg=mouse_y-y1 ;
	      ZZ=0 ; key1='\r' ; key2=0 ;
	  }
      }

       switch( key2 )
       {
           case S_key_Up  : if( 1<Reg ) Reg-- ; break ;
           case S_key_Down: if( Reg<4 ) Reg++ ; break ;
           case S_key_F1  : Window_help( (void *)"besser.html"); break ;
           case S_key_F9  : mouse_x=-1 ;
	        M_Menu    : r1=menu_mouse( Besser_menu,0,mouse_x );
	                    if( 'x'==r1 )
			    {   if( 1==fragen( "Вы действительно хотите выйти из программы?\n 1-Да 0-Нет" ) )
			            return ;
                            }
			    break ;
       }
       switch( key1 )
       {
	   case '\r' :
                switch( Reg )
		{
		    case 1 : 
			  {  t_Rect R=Edit1.get_pos();
			     Edit1.f_2Rame=1 ;
			     Edit1.set_pos( 0, 2, s_get_sx()-1, s_get_sy()-2 );
			     Edit1.loop_format();
			     Edit1.set_pos( R.x1, R.y1, R.x2, R.y2 );
			  }  break ;
		    case 2 : if( 0==Macrostruct.exist )
                                 Macrostruct.make();
		             if( 0==Kontext.exist )
			     {   message("Идет построение словаря.\n        Ждите...");
                                 s_refresh();
			         Kontext.make();
				 Kontext.make_default_perevod( );
			     }
			     Kontext2.make_sort();
			     Kontext2.main_loop();
			     break ;
		    case 3 : if( 0==Macrostruct.exist )
		                 Macrostruct.make();
                             Macrostruct2.main_loop();
		             break ;
                    case 4 : WW_main_loop( );
		             break ;
		}	   
	   break ;
       }       
   }
}
/***************************************************************************/
//               окно "подсказка"
//    Void - строка имя файла подсказки
/***************************************************************************/
void  Window_help( void *Void )
{  char *Str=(char *)Void ;
   short sx,sy ;

   s_get_size( sx,sy );
   s_save( 1,1,sy-1,sx-1 );
   //HelpView.init( HelpData.get_help( Str ) );
   HelpView.loop_help( Str );
   s_restore();
}
/***************************************************************************/
//                   работа со словарем для человека
/***************************************************************************/
e_WinMsg WW_win_slowo( void )
{
   // ------ установление размеров окошек ---------------
   short sx,sy,xx,yy,y1,y2 ;

   if( Read_Error!=0 )
   {  figna("Грамматика или словари не прочитаны.\n       Перевод невозможен!!!");
      return WM_ESC ;
   }
   s_get_size( sx,sy );

   yy=sy/2 ;
   y2=sy/2 ; y1=y2/2 ;

   Edit1.set_pos( 0, 1   , sx-1, y1   );
   Edit2.set_pos( 0, y1+1, sx-1, y2   );
   Slowo.set_pos( 0, y2+1, sx-1, sy-2, 1 );

   char *Str1="" ;
   e_WinMsg r ;

   null_menu( Main_menu );

   Edit1.raden_y3();
   Edit2.raden_y3();
   Edit1.paint();
   Edit2.paint();

   if( 0<Grammar.File2.j )
   {
      Slowo.read_buchs( ); // нехорошо, что все время перечитывается
   }
   else
   {  figna( "А словаря-то нет!" );
      return WM_ESC ;
   }
   while( 1 )
   {  short i,n ;
      char  c,Str[300] ;
   
      if( Reg1=='1' ) Str1=Edit1.get_word( Edit1.current_pos( ) );
          else        Str1=Edit2.get_word( Edit2.current_pos( ) );

      // ------ отрезать знаки препинания от слова -------------
      strcpy( Str,Str1 );
      n=strlen( Str );
      for( i=n-1 ; 0<i ; i-- )
      {  c=Str[i] ;
         if( !(c==',' || c=='.' || c=='!' || c=='?' || c==':' ||
	       c=='\''|| c=='`'|| c=='\"'|| c=='-' )) break ;
         Str[i]=0 ;
      }
            
      Slowo.set_str( Str );
      r=Slowo.main_loop( );
      switch( r )
      {
         case WM_ESC : goto M_End ;
	 case WM_N_WORD:
	      if( Reg1=='1' )
	      {   Edit1.next_word( );
	          Edit1.paint();
	      }
	      else
	      {   Edit2.next_word( );
	          Edit2.paint();
	      }
	      break ;
	 case WM_P_WORD:
	      if( Reg1=='1' )
	      {   Edit1.prev_word( );
	          Edit1.paint();
	      }
	      else
	      {   Edit2.prev_word( );
	          Edit2.paint();
	      }
	      break ;
      }
   }
M_End :
   Edit1.set_pos( 0, 1   , sx-1, yy   );
   Edit2.set_pos( 0, yy+1, sx-1, sy-1 );
   return WM_ESC ;
}
/***************************************************************************/
//             синхронный шаг на одну фразу назад
/***************************************************************************/
void WW_prev_frasa( void *Void )
{
   Edit1.prev_frasa();
   Edit2.prev_frasa();
   Edit1.paint();
   Edit2.paint();
}
/***************************************************************************/
//             синхронный шаг на одну фразу вперед
/***************************************************************************/
void WW_next_frasa( void *Void )
{
   Edit1.next_frasa();
   Edit2.next_frasa();
   Edit1.paint();
   Edit2.paint();
}
/************************************************************************/
//             установить направление перевода
//   t_DirTrans S - структура задающая имя языка и файл описания грамматики
/************************************************************************/
void select_lang( void *S )
{  int  r,r1=0 ;

   null_menu( Main_menu );
   message("Идет загрузка данных.\n        Ждите...");
   s_refresh();

   z_DirTrans=(t_DirTrans *)S ;
   r=read_grammar( MercuryDir.c_str(),z_DirTrans );
   // ---------- человеческий словарь 
   if( 0<Grammar.File2.j )
   {  
      Slowo.read_buchs(  );
   }
   if( r<0 ) 
   {  ErrorView.text()->read_file("mercury.err");
      error_add("Грамматика или словари не прочитаны!!!\n");
      error_add("Корректная работа невозможна!!!");
      figna( error_get() );
      ErrorView.main( );
   }
   else
   {  ErrorView.init("");
      Direct=z_DirTrans->Name ;
   }
   WW_paint();
}
/************************************************************************/
//             установить выходной формат
//   t_DirTrans S - структура задающая имя языка и файла формата
/************************************************************************/
void select_format( void *S )
{  t_String File ;

   File =MercuryDir ;
   File+=((t_DirTrans *)S)->Dir ;
   Macrostruct.scanf_type_absatz( (char *)File.c_str() );
   FormatName=((t_DirTrans *)S)->Name ;
}
/************************************************************************/
//             построить данные для направления перевода
//      че-то мне не нравится, что это здесь
//      надо разделить на две функции - чтение и формирование_меню
/************************************************************************/
void language_rc( char *File )
{  long   i,L,n_Lang ;
   char  *s,*s1,*Mass ;
   FILE  *ff ;

try
{
   // -------- чтение файла --------------------------
   ff=fopen( File,"r" );
   if( ff==NULL ) throw(-1) ;
   fclose(ff);
   L=file_to_mass( File,&Mass );

   t_Lexer Lexer ;

   Lexer.set_breaker((uchar *)":");
   Lexer.init( Mass,L );
   Lexer.remark();
   Lexer.make_words();

   t_Menu *PP_Menu=(t_Menu *)Calloc(1+Lexer.n_word()/2,sizeof(t_Menu) );
   DirTrans=(t_DirTrans *)Calloc( Lexer.n_Word/2 , sizeof(t_DirTrans) );

   // ------- формирование массива DirectTranslation ------------------
   n_DirTrans=n_Lang=Lexer.n_Word/2 ;
   for( i=0 ; i<n_Lang ; i++ )
   {  s=Lexer.Word[2*i] ;
      s1=(char *)Calloc( strlen(s)+1,sizeof(char) );
      strcpy( s1,s );
      DirTrans[i].Dir=s1 ;
      s=Lexer.Word[2*i+1] ;
      s1=(char *)Calloc( strlen(s)+1,sizeof(char) );
      strcpy( s1,s );
      DirTrans[i].Name=s1 ;
   }

   // ------- формирование меню ------------------------
   for( i=0 ; i<n_Lang ; i++ )
   {  PP_Menu[i].flag=F_FUNK ;
      PP_Menu[i].ret =0 ;
      PP_Menu[i].funk=select_lang ;
      PP_Menu[i].data=&DirTrans[i] ;
      Strcpy( PP_Menu[i].name,DirTrans[i].Name,30 );
   }
   PP_Menu[i].flag=F_ENDE ;

   // ------- вставить в главное меню ---------------
   t_Menu *PP1_Menu=menu_find( Main_menu, "Направление" );
   if( PP1_Menu!=NULL )
   {   PP1_Menu->flag=F_MENU ;
       PP1_Menu->data=PP_Menu ;
   }
   PP1_Menu->flag=F_MENU ;

   // ------- вставить в меню "besser"---------------
   PP1_Menu=menu_find( Besser_menu, "Направление" );
   if( PP1_Menu!=NULL )
   {   PP1_Menu->flag=F_MENU ;
       PP1_Menu->data=PP_Menu ;
   }
   PP1_Menu->flag=F_MENU ;
}
catch( int E )
{     
   printf("File \""); printf( File ); printf("\" not read." );
   throw( E );
}
}
/************************************************************************/
//             построить данные для выходных форматов
//             до чего похоже на предыдущий случай!
/************************************************************************/
void out_format_rc( char *File )
{  long   i,L ;
   char  *s,*s1,*Mass ;
   FILE  *ff ;

try
{   // -------- чтение файла --------------------------
   n_DirFormat=0 ;
   ff=fopen( File,"r" );
   if( ff==NULL ) throw(-1) ;
   fclose(ff);
   L=file_to_mass( File,&Mass );

   t_Lexer Lexer ;

   Lexer.set_breaker((uchar *)":");
   Lexer.init( Mass,L );
   Lexer.remark();
   Lexer.make_words();

   t_Menu *PP_Menu=(t_Menu *)Calloc(1+Lexer.n_word()/2,sizeof(t_Menu) );
   DirFormat=(t_DirTrans *)Calloc( Lexer.n_Word/2 , sizeof(t_DirTrans) );

   // ------- формирование массива DirFormat ------------------
   n_DirFormat=Lexer.n_Word/2 ;
   for( i=0 ; i<n_DirFormat ; i++ )
   {  s=Lexer.Word[2*i] ;
      s1=(char *)Calloc( strlen(s)+1,sizeof(char) );
      strcpy( s1,s );
      DirFormat[i].Dir=s1 ;
      s=Lexer.Word[2*i+1] ;
      s1=(char *)Calloc( strlen(s)+1,sizeof(char) );
      strcpy( s1,s );
      DirFormat[i].Name=s1 ;
   }

   // ------- формирование меню ------------------------
   for( i=0 ; i<n_DirFormat ; i++ )
   {  PP_Menu[i].flag=F_FUNK ;
      PP_Menu[i].ret =0 ;
      PP_Menu[i].funk=select_format ;
      PP_Menu[i].data=&DirFormat[i] ;
      Strcpy( PP_Menu[i].name,DirFormat[i].Name,30 );
   }
   PP_Menu[i].flag=F_ENDE ;

   t_Menu *PP1_Menu=menu_find( Besser_menu, "Вых. формат" );
   if( PP1_Menu!=NULL )
   {   PP1_Menu->flag=F_MENU ;
       PP1_Menu->data=PP_Menu ;
   }
   PP1_Menu->flag=F_MENU ;
}
catch( int E )
{     
   printf("File \""); printf( File ); printf("\" not read." );
   throw( E );
}
}
/************************************************************************/
//               чтение и запись гипертекста
/************************************************************************/
void  hiper_write( void *Void )
{  t_String Buf ;

   if( 0>Dir.main_loop( Buf,0 ) ) return ;
   Macrostruct.print( Buf.c_str() );
}
/************************************************************************/
void  hiper_read( void *Void )
{   t_String Buf ;
try
{
   if( 0>Dir.main_loop( Buf,0 ) ) return ;   

   Macrostruct.scanf( Buf.c_str() );
   Macrostruct.make_ht_sort();
   FileName=Buf ;
}
catch( int E )
{  printf("\n File %s not read",Buf.c_str() );
}
}
/************************************************************************/
//                чтение и запись контекстного словаря
/************************************************************************/
void  kontext_write( void *Void )
{  t_String Buf ;

   if( 0>Dir.main_loop( Buf,0 ) ) return ;
   Kontext.print_all( Buf.c_str() );
}
/************************************************************************/
void  kontext_read( void *Void )
{   t_String Buf ;
try
{
   if( 0>Dir.main_loop( Buf,0 ) ) return ;   

   Kontext.scanf_all( Buf.c_str() );
   DicName =Buf ;
}
catch( int E )
{  printf("\n File %s not read",Buf.c_str() );
}
}
/************************************************************************/
//               чтение и запись сеанса
/************************************************************************/
void seans_write( void *Void )
{  t_String Buf ;
   char     StrSave[100];

   Dir.get_schabl( StrSave );
   Dir.set_schabl( "*.seans" );
   short r=Dir.main_loop( Buf,1 );
   Dir.set_schabl( StrSave );
   if( 0>r ) return ;

   FILE *fw=Fopen( Buf.c_str(),"w" );
   Macrostruct.print( fw );
   Kontext.print_all( fw );
   Fclose(fw);
}
/************************************************************************/
void seans_read( void *Void )
{  t_String Buf ;
   char     StrSave[100];
try
{
   Dir.get_schabl( StrSave );
   Dir.set_schabl( "*.seans" );
   short r=Dir.main_loop( Buf,0 );
   Dir.set_schabl( StrSave );
   if( 0>r ) return ;

   Macrostruct.scanf( Buf.c_str() );
   Macrostruct.make_ht_sort();
   Kontext.scanf_all( Buf.c_str() );
   FileName=Buf ;
   DicName =Buf ;
}
catch( int E )
{  printf("\n File %s not read",Buf.c_str() );
}
}
