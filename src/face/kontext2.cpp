/*****************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//              Структуры для построения контекстного словаря (интерфейс)
//     Анисимов Д.В.                                         сегодня
/*****************************************************************************/

# include "option.h"
# include "video.h"
# include "common.h"
# include "menu.h"
# include "word.h"
# include "windows.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

void  kontext_loop( void *V );
void  perevod_loop( void *V );
void  word_loop( void *V );
void  resize_vindow( void *V );
void  dict_mgr( void *V );

// ----------- меню для режима Kontext ------------------------------------
t_Menu Kontext_menu1[] =
{  { F_FUNK  ,0,kontext_loop ,(void *)"3", "Построить          "},
   { F_FUNK  ,0,kontext_loop ,(void *)"4", "Поиск              "},
   { F_FUNK  ,0,kontext_loop ,(void *)"6", "Фильтры            "},
   { F_FUNK  ,0,kontext_loop ,(void *)"7", "Посчитать дырки    "},
   { F_FUNK  ,0,dict_mgr     ,NULL       , "Конструктор словаря"},
   { F_NUMER ,0,NULL         ,(void *)"x", "Выход" },
   { F_ENDE  ,0,NULL         ,NULL,"" }
};
t_Menu Kontext_menu2[] =
{  { F_FUNK  ,0,kontext_loop ,(void *)"2", "Использовать           "},
   { F_FUNK  ,0,kontext_loop ,(void *)"5", "Задать перевод         "},
   { F_ENDE  ,0,NULL         ,NULL,"" }
};
t_Menu Kontext_help[] =
{  { F_FUNK  ,0,Window_help ,(void *)"index.html"    ,"Содержание    " },
   { F_FUNK  ,0,Window_help ,(void *)"k_kontext.html","Об этом режиме"},
   { F_ENDE  ,0,NULL        ,NULL,"" }
};
t_Menu Kontext_menu[] =
{  { F_MENU  ,0,NULL,Kontext_menu1,"Словарь "},
   { F_MENU  ,0,NULL,Kontext_menu2,"Текущее_слово"},
   { F_MENU  ,0,NULL,Kontext_help ,"Подсказка "},
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
// ----------- меню для режима Set_Perevod --------------------------------
t_Menu Perevod_menu1[]= // Вариант_перевода
{
   { F_FUNK  ,0,perevod_loop   ,(void *)"6"   ,"Удалить                 " },
   { F_FUNK  ,0,perevod_loop   ,(void *)"9"   ,"Добавить                " },
   { F_FUNK  ,0,perevod_loop   ,(void *)"m"   ,"Создать                 " },
   { F_FUNK  ,0,perevod_loop   ,(void *)"n"   ,"Взять из словаря        " },
   { F_FUNK  ,0,perevod_loop   ,(void *)"s"   ,"Переключить словарь     " },
   { F_SCHREI,0,NULL           ,NULL          ,"------------------------" },
   { F_NUMER ,0,NULL           ,(void *)"x"   ,"Выход" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Perevod_menu2[]= // Хождение_по_словарям
{
   { F_SCHREI,0,NULL           ,NULL          ,"Исходный текст   " },
   { F_FUNK  ,1,perevod_loop   ,(void *)"0"   ,"   Шаг вверх       1" },
   { F_FUNK  ,1,perevod_loop   ,(void *)"1"   ,"   Шаг вниз        1" },

   { F_SCHREI,0,NULL           ,NULL          ,"Подстрочник      " },
   { F_FUNK  ,1,perevod_loop   ,(void *)"2"   ,"   Шаг вверх       2" },
   { F_FUNK  ,1,perevod_loop   ,(void *)"3"   ,"   Шаг вниз        2" },

   { F_SCHREI,0,NULL           ,NULL          ,"Контекстный словарь" },
   { F_FUNK  ,1,perevod_loop   ,(void *)"4"   ,"   Шаг вверх       3" },
   { F_FUNK  ,1,perevod_loop   ,(void *)"5"   ,"   Шаг вниз        3" },

   { F_SCHREI,0,NULL           ,NULL          ,"Главный словарь  " },
   { F_FUNK  ,1,perevod_loop   ,(void *)"7"   ,"   Шаг вверх       4" },
   { F_FUNK  ,1,perevod_loop   ,(void *)"8"   ,"   Шаг вниз        4" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_menu2[] = // выражение
{
   { F_FUNK  ,0,word_loop, (void *)"2"  ,"Обструктурить   F2" },
   { F_FUNK  ,0,word_loop, (void *)"3"  ,"Расструктурить  F3" },
   { F_FUNK  ,0,word_loop, (void *)"6"  ,"Cтруктуры1      F6" },
   { F_FUNK  ,0,word_loop, (void *)"7"  ,"Cтруктуры2      F7" },
   { F_FUNK  ,0,word_loop, (void *)"9"  ,"Дублировать ^S Ins" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_menu3[] = // слово
{
   { F_FUNK  ,0,word_loop, (void *)"4"  ,"Части речи      F4" },
   { F_FUNK  ,0,word_loop, (void *)"5"  ,"Части речи все  F5" },
   { F_FUNK  ,0,word_loop, (void *)"8"  ,"Параметры       F8" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Perevod_menu3[]= // Размер окошек
{
   { F_FUNK  ,1,resize_vindow  ,(void *)"1+"   ,"+ Исходный текст " },
   { F_FUNK  ,1,resize_vindow  ,(void *)"1-"   ,"- Исходный текст " },
   { F_FUNK  ,1,resize_vindow  ,(void *)"2+"   ,"+ Подстрочник    " },
   { F_FUNK  ,1,resize_vindow  ,(void *)"2-"   ,"- Подстрочник    " },
   { F_FUNK  ,1,resize_vindow  ,(void *)"3+"   ,"+ Текущий словарь" },
   { F_FUNK  ,1,resize_vindow  ,(void *)"3-"   ,"- Текущий словарь" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Perevod_help[] =
{  { F_FUNK  ,0,Window_help ,(void *)"index.html "       ,"Содержание    " },
   { F_FUNK  ,0,Window_help ,(void *)"k_kontext1.html"   ,"Об этом режиме" },
   { F_FUNK  ,0,Window_help ,(void *)"k_konstruktor.html","Словарная запись"},
   { F_ENDE  ,0,NULL        ,NULL,"" }
};

t_Menu SetPerevod_menu[] =
{
   { F_MENU  ,0,NULL ,Perevod_menu1, "Вариант_перевода"},
   { F_MENU  ,0,NULL ,Perevod_menu2, "Хождение_по_словарям"},
   { F_MENU  ,0,NULL ,Wizard_menu2 , "Выражение "},
   { F_MENU  ,0,NULL ,Wizard_menu3 , "Слово" },
   { F_MENU  ,0,NULL ,Perevod_menu3, "Размер"},
   { F_MENU  ,0,NULL ,Perevod_help , "Подсказка"},
   { F_ENDE  ,0,NULL,NULL          , "" }
};

extern t_Slowo3      Perevod1 ;
extern t_Kontext     Kontext ;
extern t_HotKey2     KeyKontext[] ;
extern t_HotKey2     KeyPerevod[] ;
       t_SetPerevod  SetPerevod ;   // что за безобразие? немедленно в windows.cpp !!!
       char         **AlphaString ; // массив нужный для сортировки типов по алфавиту
/*****************************************************************************/
t_Kontext2 :: t_Kontext2( )
{
   Filter.exist   =ALL ;
   Filter.perevod =ALL ;
   Filter.struct1 =ALL ;
   Sort  =FREQ ;
}
/*****************************************************************************/
//                    нарисовать список выражений
/*****************************************************************************/
void t_Kontext2 :: paint( long Begin, long PY )
{  short i ;
   char  Str[20] ;

   t_Win::paint();
   if( f_2Rame ) s_rame2_F( y1+1, x1, y2, x2, 0x0f );
       else      s_rame1_F( y1+1, x1, y2, x2, 0x0f );
   s_text_yxf( y1,x1+3 ,titl_Color, Name );

   s_rame1_f( y1+2, x1+10, y1+6, x1+20, 0x0f );
   s_rame1_f( y1+3, x1+14, y1+6, x1+20, 0x0f );
   s_rame1_f( y1+4, x1+16, y1+6, x1+20, 0x0f );
   s_rame1_f( y1+5, x1+18, y1+6, x1+20, 0x0f );

   s_text_yxf( y1+2,x1+20 ,0x0f, " Число вхождений" );
   s_text_yxf( y1+3,x1+20 ,0x0f, " Число дырок" );
   s_text_yxf( y1+4,x1+20 ,0x0f, " Есть ли в словаре" );
   s_text_yxf( y1+5,x1+20 ,0x0f, " Использовать" );
   
   s_text_yxf( y1+2,x1+1  ,0x0f, "Debug" );
   s_text_yxf( y1+3,x1+1  ,0x0f, "Index" );
   s_text_yxf( y1+4,x1+1  ,0x0f, "  |  " );
   s_text_yxf( y1+5,x1+1  ,0x0f, "  |  " );

   for( i=0 ; i<=size_y-7 ; i++ )
      paint1( Begin+i, 6+y1+i, Begin+i==PY );

   sprintf( Str," %3d/%3d ",PY,View.j );
   s_text_yxf( y2,x2-10,0x0f, Str );
   paint_hotkey( n_Line-1, 0, hk_Kontext.c_str() );
   paint_header( "Контекстный словарь" );
   null_menu( Kontext_menu,y1 );
}
/*****************************************************************************/
void t_Kontext2 :: paint_small( long Begin, long PY )
{
   s_rame1_F( y1, x1, y2, x2, 0x0f );
   s_text_yxf( y1,x1+3 ,titl_Color, "Контекстный словарь" );
   for( long i=0 ; i<y2-y1-1 ; i++ )
      paint1( Begin+i, y1+i+1, Begin+i==PY );
}
/*****************************************************************************/
//                     нарисовать строку (выражение)
//      i_list - индекс в списке Kontext.Type[View[]]
//      y      - координата где рисовать
//      f      - признак текущего
/*****************************************************************************/
void t_Kontext2 :: paint1( long i_list, long y, char f_current )
{  char f1 ;
   char *s,*s1,*s2,*s3,*s4 ;
   char Str[1000] ;

   if( f_current==0 ) f1=text_Color ; else f1=cur_item_Color ;
   s=emptystr( x2-x1-1 );
   s_text_yxf( y,x1+1 ,f1, s );
   
   if( View.j<=i_list ) return ;

   t_wType &T=Kontext.Type[View[i_list]] ;
   if( T.select     ) f1=(f1 & 0xf0) | (0x0f & sel_item_Color) ;
   if( T.i_slowo<0  ) s3="." ; else s3="1" ;
   if( T.used   ==0 ) s4="u" ; else s4="." ;
   
   long n_exemplar=T.i_last_exemplar-T.i_exemplar ;
   char *S="" ;
   if( BeginX<strlen(T.Str) )
       S=T.Str+BeginX ;
   sprintf( Str,"%4d | %3d %3d %s %s %s", View[i_list], n_exemplar, T.n_hole, s3, s4, S );
   s_text_yxfl( y, x1+1 ,f1, x2-x1-1, Str );
}
/*****************************************************************************/
//                   хождение по списку выражений
/*****************************************************************************/
e_WinMsg t_Kontext2 :: main_loop( void )
{
   short        button,mouse_x,mouse_y,r1 ;
   ushort       ZZ,key1,key2,k ;
   static short sx1,sy1 ;
   e_Function   Func ;
   e_sEvent     Event ;
   e_WinMsg     Ret ;

   Begin=0 ; BeginX=0 ; PY=0 ;
   make_filtred_list( );
   s_get_size( sx1,sy1 );
   set_pos( 0, 1, sx1-1, sy1-2 );
   SetPerevod.set_pos( 0, 1, sx1-1, sy1-2 );

   while( 1 )
   {
      paint( Begin, PY );

      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
          if( mouse_y==1 )
	  {   r1=menu_mouse( Kontext_menu, y1-1,mouse_x ) ;
	      if( 'x'==r1 ) return WM_ESC ;
	      continue ;
	  }
	  if( mouse_y==n_Line-1 )
	      Option.get_key( &ZZ, &key1, &key2, hk_Kontext.c_str(), mouse_x );
          if( button==1 )
	  {   if( y1+4<mouse_y && mouse_y<y2 )
	          PY=min(Begin+mouse_y-(y1+6),View.j-1) ;
	  }
          if( button==3 ){  ZZ=0, key1='\r' ; key2=0  ;  }
          if( button==4 ){  ZZ=0, key1=0    ; key2=S_key_Up  ;  }
          if( button==5 ){  ZZ=0, key1=0    ; key2=S_key_Down;  }
      }
      if( 30<key1 ) k=key1>>8 ;
          else      k=key1 ;

      Func=Option.get_func( KeyKontext, ZZ,k,key2 );
      Ret=loop( Func, ZZ, key1, key2 );
      if( WM_ESC==Ret ) return WM_ESC ;
   }
   return WM_OK ;
}
/*****************************************************************************/
void  kontext_loop( void *V )
{  char *Func=(char *)V ;

   switch( Func[0] )
   {
      case '0' : Kontext2.loop( F_HELP           ,0,0,0 ); break ;
      case '1' : Kontext2.loop( F_OKEY           ,0,0,0 ); break ;
      case '2' : Kontext2.loop( F_DICT_USE       ,0,0,0 ); break ;
      case '3' : Kontext2.loop( F_DICT_MAKE_DICT ,0,0,0 ); break ;
      case '4' : Kontext2.loop( F_DICT_FIND      ,0,0,0 ); break ;
      case '5' : Kontext2.loop( F_DICT_ENTER     ,0,0,0 ); break ;
      case '6' : Kontext2.loop( F_DICT_FILTER    ,0,0,0 ); break ;
      case '7' : Kontext2.loop( F_DICT_HOLE      ,0,0,0 ); break ;
   }   
}
/*************************************************************************/
e_WinMsg t_Kontext2 :: loop( e_Function Func, short ZZ, ushort key1, ushort key2 )
{
   short  sx,sy,r1 ;

try
{  switch( Func )
   {
       case F_HELP: Window_help( (void *)"k_kontext.html"); break ;
       case F_OKEY:
	    s_get_size( sx,sy );
	    Edit1.set_pos( 0, 1, sx-1, sy-2 );
            return WM_ESC ;
       case F_DICT_USE:// ---- Использовать ----------------
	    Kontext.Type[View[PY]].used=1-Kontext.Type[View[PY]].used ;
	    break ;
       case F_DICT_MAKE_DICT: // ---- сделать разбиение текста ---------------
            message("Идет построение контекстного словаря\nЖдите...");
	    if( 0==Macrostruct.exist || 0>Macrostruct.format_is_correct() )
		Macrostruct.make();
	    Kontext.make();
	    Kontext.make_default_perevod( ); // вставил, чтобы делать словарь одной кнопкой
	    make_sort();
	    make_filtred_list();
	    Begin=0 ; PY=0 ;
	    break ;
       case F_DICT_FIND : find(); break ;
       case F_DICT_ENTER:
            Kontext.make_current_perevod(); // не слишком ли шикарно на каждый чих?
            set_main_dict( 1 );
	    SetPerevod.init( View[PY] );
            SetPerevod.main_loop();
            set_main_dict( 0 );
	    break ;	     
       case F_DICT_FILTER: // ----- установить флаги для списка ----------
	    loop_flags( );
	    make_filtred_list( );
	    break ;
       case F_DICT_DEL:  delete_type( ); break ;
       case F_DICT_INS:  double_type( ) ; break ;
       case F_DICT_HOLE: Kontext.calc_n_hole( ); break ;
       case F_MENU1:
	    r1=menu( Kontext_menu, y1 ) ;
	    if( 'x'==r1 ) return WM_ESC ;
	    break ;
       case F_END_OF_LIST:	        
            switch( key1 )
	    {
	        case 0:
	        switch( key2 )
	        {
		    case S_key_Up   : PY-- ; break ;
		    case S_key_Down : PY++ ; break ;
		    case S_key_Left  : if( 0<BeginX ) BeginX-- ; break ;
		    case S_key_Right : BeginX++ ; break ;
		    case S_key_Home : PY=0 ;        break ;
		    case S_key_End  : PY=View.j-1 ; break ;
		    case S_key_PgUp : PY-=(size_y-7); break ;
		    case S_key_PgDn : PY+=(size_y-7); break ;
		}
		if( PY<0 ) PY=0 ;
		if( View.j<=PY ) PY=View.j-1 ;
		if( PY<Begin ) Begin=PY ;
		if( (size_y-7)<PY-Begin ) Begin=PY-(size_y-7) ;
		break ;
	    }
   }
}
catch( ... )
{
    figna( error_get() );
}
return WM_OK ;
}
/*************************************************************************/
//      реакции на кнопки в подокне из окна "конструктор выражения"
/*************************************************************************/
e_WinMsg t_Kontext2 :: loop_small( e_Function Func, short ZZ, ushort key1, ushort key2 )
{   long size1_y=y2-y1-1 ;

    switch( key1 )
    {
	case 0:
	switch( key2 )
	{
	    case S_key_Up :   PY-- ; break ;
	    case S_key_Down : PY++ ; break ;
	    case S_key_Home : PY=0 ; break ;
	    case S_key_End  : PY=View.j-1 ; break ;
	    case S_key_PgUp : PY-=size1_y ; break ;
	    case S_key_PgDn : PY+=size1_y ; break ;
	}
	break ;
    }
    if( PY<0 )       PY=0 ;
    if( View.j<=PY ) PY=View.j-1 ;
    if( PY<Begin )          Begin=PY ;
    if( size1_y<=PY-Begin ) Begin=PY-(size1_y-1) ;
    return WM_OK ;
}
/*************************************************************************/
//                     поиск выражения в списке
/*************************************************************************/
void t_Kontext2 :: find()
{  short  DialogPos ;
   long   size1_y=y2-y1-1 ;
   t_Find Find ;
   char   direct=0 ;
   ushort key1,key2 ;
   e_WinMsg r ;

M_Begin:
   short size_dialog=7 ;
   if( (y2-y1)/2<PY-Begin )
             DialogPos = y1+  (y2-y1-size_dialog)/4 ;
	else DialogPos = y2-6-(y2-y1-size_dialog)/4 ;
 
   r=Find.loop( DialogPos );
   if( WM_ESC==r ) return ;
   if( Find.sfind[0]==0 ) return ;

   while( 1 )
   {  char sFind[256] ;
      myChar_to_koi9( Find.sfind, (uchar *)sFind, 256, 256 );

      if( 0==direct )
      {   for( long i=PY+1 ; i<View.j ; i++ )
	  {  if( NULL!=strstr( Kontext.Type[View[i]].Str,sFind ) )
	     {   PY=i ; break ;  }
	  }
      }
      else
      {   for( long i=PY-1 ; 0<=i ; i-- )
	  {  if( NULL!=strstr( Kontext.Type[View[i]].Str,sFind ) )
	     {   PY=i ; break ;  }
	  }
      }   
      if( PY<Begin ) Begin=PY ;
      if( (size1_y-7)<=PY-Begin ) Begin=PY-(size1_y-7) ;

       paint( Begin,PY );
       if( (y2-y1)/2<PY-Begin )
        	 DialogPos = y1+  (y2-y1-size_dialog)/4 ;
	    else DialogPos = y2-6-(y2-y1-size_dialog)/4 ;
       Find.paint( DialogPos,-5 );
       s_getch( &key1,&key2 );
       switch( key1 )
       {   case 0 : 
	          switch( key2 )
	          {  case S_key_F3  : direct=0 ; break ;
     		     case S_key_F4  : direct=1 ; break ;
		     case S_key_F10 : return ;
		  }
		  break ;
           case S_key_Esc: goto M_Begin ;
	   case '1' : break ;
	   case '2' : break ;
	   case '0' : break ;
       }
   }
}
/*************************************************************************/
//         нарисовать настройки для рисования списка выражений
/*************************************************************************/
void t_Kontext2 :: paint_flags( short py )
{
   short sx,sy ;
   short x1,y1,x2,y2 ;
   char  f ;
   char *StrFilter[]={ "Да ","Нет","Все" };
   char *StrFreq  []={ "частоте   ","алфавиту  ","части речи" };
   char  Buf[100] ;

   s_get_size( sx,sy );
   x1=sx/2-20 ; y1=sy/2-3  ;
   x2=sx/2+20 ; y2=sy/2+3  ;
   s_rame2_F( y1, x1, y2, x2, 0x0f );
   s_text_yxf( y1, x1+3, 0x0b, "Настройки списка" );

   if( py==0 ) f=0xf0 ; else f=0x0f ;
   sprintf( Buf,"[%3s] есть ли в словаре        ",StrFilter[Filter.exist] );
   s_text_yxf( y1+1,x1+1,f,Buf );

   if( py==1 ) f=0xf0 ; else f=0x0f ;
   sprintf( Buf,"[%3s] задан ли перевод         ",StrFilter[Filter.perevod] );
   s_text_yxf( y1+2,x1+1,f,Buf );

   if( py==2 ) f=0xf0 ; else f=0x0f ;
   sprintf( Buf,"[%3s] укладывается ли в STRUCT1",StrFilter[Filter.struct1] );
   s_text_yxf( y1+3,x1+1,f,Buf );
   
   if( py==3 ) f=0xf0 ; else f=0x0f ;
   sprintf( Buf,"сортировать по [%3s]    ",StrFreq[Sort] );
   s_text_yxf( y1+4,x1+1,f,Buf );
}
/*****************************************************************************/
//               установить фильтры и флаги списку выражений
/*****************************************************************************/
e_WinMsg t_Kontext2 :: loop_flags( void )
{
   short  sx,sy ;
   short  x1,y1,x2,y2 ; // вообще-то так писать не надо
   ushort ZZ,key1,key2 ;
   short  button,mouse_x,mouse_y ;
   short  Begin=0,PY=0 ;
   long   MaxY=Freq.j ;
   e_sEvent     Event ;

   s_get_size( sx,sy );
   x1=sx/2-20 ; y1=sy/2-3  ;
   x2=sx/2+20 ; y2=sy/2+3  ;

   while( 1 )
   {
      paint_flags( PY );

      Event=s_getch( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
	  if( mouse_x<x1 || x2<mouse_x || mouse_y<y1 || y2<mouse_y )
	      return WM_ESC ;
          if( 3==button )
	  {   if( y1<mouse_y && mouse_y<y1+5 )
	      {   PY=mouse_y-y1-1 ; key2=S_key_Right ;  }	  
	  }
      }
      
      switch( key1 )
      {
         case S_key_Esc : return WM_ESC ;
	 case '\r'      : return WM_OK ;
      }
      switch( key2 )
      {
	 case S_key_F1 : Window_help( (void *)"k_flags.html") ; continue ;
	 case S_key_Up   : if( 0<PY ) PY-- ; break ;
	 case S_key_Down : if( PY<3 ) PY++ ; break ;
	 case S_key_Left :
	      switch( PY )
	      {   case 0 : Filter.exist  =(e_Filter)((Filter.exist  +2)%3) ; break ;
	          case 1 : Filter.perevod=(e_Filter)((Filter.perevod+2)%3) ; break ;
		  case 2 : Filter.struct1=(e_Filter)((Filter.struct1+2)%3) ; break ;
		  case 3 : Sort         = (e_Sort  )((Sort+2)%3)           ; break ;
	      }
	      break ;
	 case S_key_Right:
	      switch( PY )
	      {   case 0 : Filter.exist  =(e_Filter)((Filter.exist  +1)%3) ; break ;
	          case 1 : Filter.perevod=(e_Filter)((Filter.perevod+1)%3) ; break ;
		  case 2 : Filter.struct1=(e_Filter)((Filter.struct1+1)%3) ; break ;
		  case 3 : Sort         = (e_Sort  )((Sort+1)%3)           ; break ;
	      }
	      break ;

	 case S_key_F10 : return WM_ESC ;
      }
   }
}
/*************************************************************************/
void t_Kontext2 :: double_type( )
{
    Kontext.double_type( View[PY] );
    make_sort( );
    make_filtred_list( );
}
/*************************************************************************/
void t_Kontext2 :: delete_type( )
{
    Kontext.delete_type( View[PY] );
    make_sort( );
    make_filtred_list( );
}
/*************************************************************************/
//       функции сравнения для сортировки списка выражений
/*************************************************************************/
int freq_cmp( const void *a, const void *b ) 
{
   return Kontext.Type[*((long *)b)].num - Kontext.Type[*((long *)a)].num ;
}
/*************************************************************************/
int alpha_cmp( const void *a, const void *b )
{
   return 
//   strcmp( Kontext.Type[*((long *)a)].Str, Kontext.Type[*((long *)b)].Str );
   Strcmp( AlphaString[*((long *)a)], AlphaString[*((long *)b)] );
}
/*************************************************************************/
int struct_cmp( const void *a, const void *b )
{
   t_wType *T1=&Kontext.Type[*((long *)a)] ;
   t_wType *T2=&Kontext.Type[*((long *)b)] ;
   
   return T1->i_slowo-T2->i_slowo ;
}
/*************************************************************************/
//   создать массивы где выражения отсортированы по частоте, алфавиту и пр.
/*************************************************************************/
void t_Kontext2 :: make_sort( )
{  long t,i,j_Mass ;
   t_String Str1 ;

   Freq.Calloc( Kontext.Type.j );
   Alpha.Calloc( Kontext.Type.j );
   Struct.Calloc( Kontext.Type.j );
   for( t=0 ; t<Kontext.Type.j ; t++ )
      Freq[t]=Alpha[t]=Struct[t]=t ;

   char *Mass =(char * )Calloc( Kontext.Type.j*100,sizeof(char) );
   AlphaString=(char **)Calloc( Kontext.Type.j    ,sizeof(char*) );

   for( t=j_Mass=0 ; t<Kontext.Type.j ; t++ )
   {  char *Str0=Kontext.Type[t].Str ;   
      if( NULL==strstr( Str0,"=" ) )
      {   AlphaString[t]=Str0 ;
      }
      else
      {   char f=0 ;
          Str1="" ;
          for( i=0 ; Str0[i]!=0 && Str0[i]!='=' ; i++ )
          {  if( ']'==Str0[i] ) { f=0 ; Str1+=" " ; }
	     if( 1==f          ) Str1+=Str0[i] ;
	     if( '['==Str0[i] ) f=1 ;
	  }
	  AlphaString[t]=Mass+j_Mass ;
	  strcpy( Mass+j_Mass, Str1.c_str() );
	  j_Mass+=Str1.length();
      }
   }

   qsort( Freq.list, Freq.j, sizeof(long), freq_cmp );
   qsort( Alpha.list, Alpha.j, sizeof(long), alpha_cmp );
   qsort( Struct.list, Struct.j, sizeof(long), struct_cmp );
   Free(Mass);
}
/*************************************************************************/
//   Соответствует ли выражение заданным фильтрам
//   Return 0 - соответствует 1 - не соответствует
/*************************************************************************/
char t_Kontext2 :: filter( long i_Type )
{  t_wType *T=&Kontext.Type[i_Type] ;

   if( Filter.exist  ==YES && 0> T->i_slowo ) return 1 ;
   if( Filter.exist  ==NO  && 0<=T->i_slowo ) return 1 ;
   if( Filter.perevod==YES && NULL==strstr( T->Str,"=") ) return 1 ;
   if( Filter.perevod==NO  && NULL!=strstr( T->Str,"=") ) return 1 ;
   return 0 ;
}
/*************************************************************************/
//     создать список выражений для отображения по заданным критериям
/*************************************************************************/
void t_Kontext2 :: make_filtred_list( )
{  long i ;

   View.j=0 ;
   if( Sort==FREQ ) // ----- с сортировкой по частоте --------
   {  for( i=0 ; i<Freq.j ; i++ )
         if( 0==filter(Freq[i]) )
             View.add( Freq[i] );
   }
   if( Sort==ALPHA ) // ----- с сортировкой по алфавиту --------
   {  for( i=0 ; i<Alpha.j ; i++ )
         if( 0==filter(Alpha[i]) )
             View.add( Alpha[i] );
   }
   if( Sort==STRUCT ) // ----- с сортировкой по части речи -----
   {  for( i=0 ; i<Struct.j ; i++ )
         if( 0==filter(Struct[i]) )
             View.add( Struct[i] );
   }
}
/*****************************************************************************/
t_wStructList :: t_wStructList( ) // конструкторы исходных типов
{
   Begin_Y=PY=0 ;
}
/*****************************************************************************/
//         сделать из словарной статьи массив вариантов переводов
/*****************************************************************************/
void t_wStructList :: init( char *Str )
{  long i,z ;
   t_wStr Str1 ;

   Str1.Str[0]=0 ;
   PY=j=0 ;
   for( i=0 ; Str[i]!=0 ; i++ )
      if( Str[i]=='=' )
	  break ;
   if( Str[i]==0 || Str[i+1]==0 ) return ;

   for( z=i ;  ; i++ )
   {  if( Str[i]==';' || Str[i]==0 )
      {   strncpy( Str1.Str, Str+z+1, min(100,i-z) );
          Str1.Str[max(0,min(100,i-z-1))]=0 ;
	  add( Str1 );
	  z=i ;
      }   
      if( Str[i]==0 ) break ;
   }
}
/*********************************************************************************/
void t_wStructList :: paint()
{  char f ;

   t_Win::paint();
   for( long i=0 ; i<size_y-1 ; i++ )
   {  if( Begin_Y+i==PY ) f=cur_item_Color ; else f=0x0f ;
      s_text_yxf ( y1+i+1,x1+1,f,emptystr(size_x-1) );
      if( Begin_Y+i<j )
          s_text_yxfl( y1+i+1,x1+1,f, size_x, list[i].Str );
   }
}
/*********************************************************************************/
//         хождение по списку вариантов перевода
/*********************************************************************************/
e_WinMsg t_wStructList :: main_loop( ushort key1, ushort key2 )
{
   switch( key2 )
   {
      case S_key_Del :
           if( j<=0 ) break ; // или исходный словарь
	   j-- ;
           for( long i=PY ; i<j ; i++ )
	      list[i]=list[i+1] ;           
	   if( j<=PY ) PY=j-1 ;
	   break ;
      case S_key_Up   : if( PY>0 ) PY-- ; break ;
      case S_key_Down : if( PY<j-1 ) PY++ ; break ;
      case S_key_Home : PY=0 ; break ;
      case S_key_End  : PY=j-1 ; break ;
      case S_key_F10  : return WM_ESC ;
   }	 
   if( PY<Begin_Y ) Begin_Y=PY ;
   if( (size_y-2)<PY-Begin_Y ) Begin_Y=PY-(size_y-2) ;
   return WM_NULL1 ;
}
/***************************************************************************/
void t_wStructList :: step_up()
{
   if( PY>0 ) PY-- ;
   if( PY<Begin_Y ) Begin_Y=PY ;
}
/***************************************************************************/
void t_wStructList :: step_dn()
{
   if( PY<j-1 ) PY++ ;
   if( size_y-2<PY-Begin_Y ) Begin_Y=PY-(size_y-2) ;
}
/***************************************************************************/
void t_wStructList :: current_del()
{
   if( j<=0 ) return ;
   j-- ;
   for( long i=PY ; i<j ; i++ )
      list[i]=list[i+1] ;           
   if( j<=PY ) PY=j-1 ;
}
/***************************************************************************/
void t_wStructList :: current_ins( t_wStr &Str )
{
   add( Str );
}
/*****************************************************************************/
//                            редактор строки
/*****************************************************************************/
t_EditLine :: t_EditLine( )
{   Str[0]=0 ;
}
/*****************************************************************************/
e_WinMsg t_EditLine :: main_loop( )
{  return kommand_yxf( y1+1, x1+1, 0x0f, Str, 100, 0 );
}
/*****************************************************************************/
void t_EditLine :: paint( )
{  t_Win::paint();
   s_text_yxf ( y1+1, x1+1, 0x07, emptystr(x2-x1-1) ) ;
   s_text_yxf1( y1+1, x1+1, 0x07, Str ) ;
}
/*****************************************************************************/
//                        задать перевод выражению
/*****************************************************************************/
t_SetPerevod :: t_SetPerevod()
{
   EditSlowo.set_text( &T );
   EditSlowo.set_name("Взять слово");
   CurrentFrame=0 ;
}
/*****************************************************************************/
void t_SetPerevod :: set_pos( short _x1, short _y1, short _x2, short _y2 )
{
   calc_pos( _x1, _y1, _x2, _y2 );
   set_pos( );
}
/******************************************************************************/
void t_SetPerevod :: calc_pos( short _x1, short _y1, short _x2, short _y2 )
{  short xx,sy,yy2,yy4,yy5 ;

   x1 = _x1 ; y1 = _y1 ; x2 = _x2 ; y2 = _y2 ;
   sy=y2-y1 ;
   yy2=y1+sy*3/12 ;
   yy4=y1+sy*6/12 ;
   yy5=y1+sy*8/12 ;
   xx=(x1+x2)/2 ;
   
   RWordWizard.x1=x1 ; RWordWizard.y1= y1+1 ; RWordWizard.x2=x2 ; RWordWizard.y2= y1+5 ;
   REdit1     .x1=x1 ; REdit1     .y1= y1+5 ; REdit1     .x2=x2 ; REdit1     .y2=yy2 ;
   RFirst     .x1=x1 ; RFirst     .y1=yy2   ; RFirst     .x2=x2 ; RFirst     .y2=yy4 ;
   RCurrentL  .x1=x1 ; RCurrentL  .y1=yy4   ; RCurrentL  .x2=xx ; RCurrentL  .y2=yy5 ;
   RMainL     .x1=xx ; RMainL     .y1=yy4   ; RMainL     .x2=x2 ; RMainL     .y2=yy5 ;
   RSlowo     .x1=x1 ; RSlowo     .y1=yy5   ; RSlowo     .x2=x2 ; RSlowo     .y2=y2 ;

//   RFrom    .x1=x1 ; RFrom    .y1=yy4-4 ; RFrom    .x2=x2 ; RFrom    .y2=yy4-2 ;
//   RTo      .x1=x1 ; RTo      .y1=yy4-2 ; RTo      .x2=x2 ; RTo      .y2=yy4 ;

   REditSlowo.x1=Slowo.rect2.x1 ; REditSlowo.y1=Slowo.rect2.y1 ;
   REditSlowo.x2=RSlowo.x2      ; REditSlowo.y2=RSlowo.y2 ;
}
/******************************************************************************/
void t_SetPerevod :: set_pos( )
{
   Edit1      .set_pos( REdit1 );
   First      .set_pos( RFirst );
   WordWizard .set_pos( RWordWizard );
// CurrentList.set_pos( RCurrentL );
   MainList   .set_pos( RMainL );
   Slowo      .set_pos( RSlowo.x1, RSlowo.y1, RSlowo.x2, RSlowo.y2, 1 );

   EditSlowo  .set_pos( Slowo.rect2 );

   First      .set_name( "Подстрочник" );
   WordWizard .set_name( "Словарная запись" );
// CurrentList.set_name( "Текущий словарь" );
   MainList   .set_name( "Главный словарь" );
   Slowo      .set_name( "Человеческий словарь" );

   Edit1      .f_2Rame=0 ;
   First      .f_2Rame=0 ;
   WordWizard .f_2Rame=0 ;
// CurrentList.f_2Rame=0 ;
   MainList   .f_2Rame=0 ;
   Slowo      .f_2Rame=0 ;
}
/*****************************************************************************/
//    _i_type - индекс в массиве  Kontext->Type[]
/*****************************************************************************/
void t_SetPerevod :: init( long _i_type )
{  short i ;
   long  i1 ;
   t_String STmp ;

   i_type =_i_type ;
   F_current=NULL ;
   
   t_wType &T=Kontext.Type[i_type] ;
   WordWizard.EditWord.set_text( T.Str );
   if( 0<=T.i_slowo )
   {  // ------ если в главном словаре есть ---------------------------
      Perevod1.print_record( STmp, T.i_slowo );
      // -------- инициировать Core и список FirstLook ----------------
      ii_exemplar=T.i_exemplar ;
      set_i_exemplar( Kontext.i_Exemplar[T.i_exemplar] );

      // -------- инициировать список главного машинного словаря ------
      MainList.init( STmp.c_str() );

      // ---------- главный словарь ------------------------------------
      Slowo.set_str( First.get_from() );
      Slowo.main_loop1_1( );
   }
   else
   {  // ------ если в главном словаре нет -----------------------------
      ii_exemplar=T.i_exemplar ;
      set_i_exemplar( Kontext.i_Exemplar[T.i_exemplar] );
      MainList.j=0 ;
      Slowo.set_str( T.Str );
      Slowo.main_loop1_1( );
   }
   MainList.PY=0 ;
}
/*****************************************************************************/
//     реакция интерфейса на смену экземпляра выражения 
//     (посмотреть на текст в месте, где оно встречается)
//     i_exemplar - индекс экземпляра
/*****************************************************************************/
void t_SetPerevod :: set_i_exemplar( long i_exemplar )
{  long   PX,PY ;
   char   Str1[1000],Str2[1000] ;

   if( F_current!=NULL )
       Edit1.clr_thisfrasa( F_current->begin, F_current->end );

   t_wExemplar &E=Kontext.Exemplar[i_exemplar] ;
   F_current =&Macrostruct.Absatz[E.i_absatz].Frasa[E.i_frasa] ;

   Edit1.set_thisfrasa( F_current->begin, F_current->end );
   Edit1.Text->get_xy_pos( F_current->begin, &PY, &PX );
   //Edit1.Begin_Y=max(0,PY-2);
   Edit1.screen_set_y( PY-2 );
   // ----- подстрочник не установлен -----------
   Macrostruct.get_frasa( E.i_absatz, E.i_frasa, Str1 );
   Core.translate( Str1, Str2 );
   
   First.set_py( E.i_word1 );
}
/*****************************************************************************/
void t_SetPerevod :: paint( )
{
   WordWizard .f_2Rame=0==CurrentFrame ;
   Slowo      .f_2Rame=1==CurrentFrame ;
   WordWizard .paint();
   Edit1      .paint();
   First      .paint();
   Kontext2   .paint_small( Kontext2.Begin, Kontext2.PY );

/* Лозунг дня:
   "Пролетарии всех стран - соединяйтесь!
   Слава советскому народу - строителю коммунизма!
   Выше знамя советского спорта!
   Комсомолец - на самолет!
   Вся власть советам!
   Работай так, чтобы товарищ Сталин был доволен!
   Коммунизм - будущее всего человечества!
   Сильнее раздувай пожар мировой революции!
   Мы на горе всем буржуям мировой пожар раздуем!
*/

   MainList   .paint();
   Slowo      .paint( Slowo.i_word, Slowo.first_word );
   EditSlowo  .paint();
   s_text_yxf( Slowo.rect2.y2, Slowo.rect2.x1+1,0x07,Slowo.Buchs[Slowo.i_Buch].Name.c_str() );

   t_wType    &T=Kontext.Type[i_type] ;
   char       Str[20] ;
   sprintf( Str,"(%3d/%3d)", ii_exemplar-T.i_exemplar, T.i_last_exemplar-T.i_exemplar );
   s_text_yxf( Edit1.y1, Edit1.x2-12,0x07,Str );
   s_text_yxf( Slowo.rect2.y1, Slowo.rect2.x1+3,0x0b,"Человеческий перевод" );
   if( 0==CurrentFrame )
            paint_hotkey( n_Line-1, 0, "[F1]Подсказка[F2]Обструктурить[F3]Расструктурить[F4]Слово[F5]Слово все[F6]Struct1[F7]Struct2[F8]Param[F9]Меню" );
       else paint_hotkey( n_Line-1, 0, hk_SetPerev.c_str() );
   paint_header( "Перевод одного выражения" );
   null_menu( SetPerevod_menu, y1 );
}
/*****************************************************************************/
e_WinMsg t_SetPerevod :: main_loop()
{
   ushort      ZZ,key1,key2,k ;
   e_Function  Func ;
   e_WinMsg    Ret ;
   e_sEvent    Event ;

   set_pos( );

   Slowo.set_str( First.get_from() );
   Slowo.main_loop1_1( );
   EditSlowo.init( Slowo.answer() );
   
   t_Rect RCurrentLSave=Kontext2.get_pos( );
   Kontext2.set_pos( RCurrentL );
   Kontext2.loop_small( F_NOP, 0, 0, 0 );

   while( 1 )
   {
      paint();
      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short button,x,y ;

          button=s_get_mouse( &x, &y );
	  if( y==REdit1.y1 || y==RFirst.y1 || y==RFrom.y1 || y==RTo.y1 ||
              y==RCurrentL.y1 || y==RMainL.y1 || y==RSlowo.y1 )
          {   loop_rect( y );
	      set_pos( );
          }
	  else if( y==y1 )
	  {   short r1=menu_mouse( SetPerevod_menu, y1, x ); 
	      if( 'x'==r1 ) return WM_ESC ;
	      continue ;
	  }
	  if( button==1 )
	  {
	      if( y==n_Line-1 )
	      {	  Option.get_key( &ZZ, &key1, &key2, hk_SetPerev.c_str(), x );
	      }
	      else if( Slowo.rect2.inside( x,y ) )
	      {   Func=F_WORD_GET ; goto M_loop ;  }
	      else 
	      {	  if( WordWizard.y1<=y && y<=WordWizard.y2 )
	        	     CurrentFrame=0 ;
		      else   CurrentFrame=1 ;
                  continue ;
	      }
	  }
	  if( button==3 )
	  {   if( RCurrentL.inside( x,y ) ){  Func=F_WORD_KONTEXT_DELETE; goto M_loop ;  }
              if( RMainL   .inside( x,y ) ){  Func=F_WORD_MAIN_INSERT   ; goto M_loop ;  }	  
	  }	  
	  if( button==4 )
	  {   if( REdit1   .inside( x,y ) ){  Func=F_WORD_EXAMPLE_PREV ; goto M_loop ;  }
	      if( RFirst   .inside( x,y ) ){  Func=F_WORD_WORD_PREV    ; goto M_loop ;  }
	      if( RCurrentL.inside( x,y ) ){  Func=F_WORD_KONTEXT_UP   ; goto M_loop ;  }
              if( RMainL   .inside( x,y ) ){  Func=F_WORD_MAIN_UP      ; goto M_loop ;  }	      
	  }
	  if( button==5 )
	  {   if( REdit1   .inside( x,y ) ){  Func=F_WORD_EXAMPLE_NEXT ; goto M_loop ;  }
	      if( RFirst   .inside( x,y ) ){  Func=F_WORD_WORD_NEXT    ; goto M_loop ;  }
	      if( RCurrentL.inside( x,y ) ){  Func=F_WORD_KONTEXT_DOWN ; goto M_loop ;  }
              if( RMainL   .inside( x,y ) ){  Func=F_WORD_MAIN_DOWN    ; goto M_loop ;  }
	  }
      }
      if( 30<key1 ) k=key1>>8 ;
          else      k=key1 ;

      Func=Option.get_func( KeyPerevod, ZZ,k,key2 );
M_loop :
      Ret=loop( Func, ZZ, key1, key2 );
      if( WM_ESC==Ret ) goto M_Esc;
   }
M_Esc:
   short sx,sy ;
   s_get_size( sx,sy );
   Edit1.set_pos( 0, 1, sx-1, sy-2 );
   if( F_current!=NULL )
       Edit1.clr_thisfrasa( F_current->begin, F_current->end );
   Kontext2.set_pos( RCurrentLSave );
   return WM_ESC ;
}
/****************************************************************************/
//              переразместить окошки с помощью мышки
/****************************************************************************/
void t_SetPerevod :: loop_rect( short yy )
{  short    x,y ;
   t_Rect  *R1=NULL,
           *R2=NULL ;
   ushort   ZZ,key1,key2 ; // нажатая клавиша
   e_sEvent Event ;

   if( yy==RFirst.y1 )    R1=&RFirst ;
   if( yy==RFrom.y1 )     R1=&RFrom ;
   if( yy==RTo.y1 )       R1=&RTo ;
   if( yy==RMainL.y1 )    R1=&RMainL ;
   if( yy==RSlowo.y1 )    R1=&RSlowo ;

   if( yy==REdit1.y2 )    R2=&REdit1 ;
   if( yy==RFirst.y2 )    R2=&RFirst ;
   if( yy==RFrom.y2 )     R2=&RFrom ;
   if( yy==RTo.y2 )       R2=&RTo ;
   if( yy==RMainL.y2 )    R2=&RMainL ;

   if( R1!=NULL ) s_save_rame( R1->y1,R1->x1,R1->y2,R1->x2 );
   if( R2!=NULL ) s_save_rame( R2->y1,R2->x1,R2->y2,R2->x2 );

   s_set_mouse_move( 1 );
   while( 1 )
   {
      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_MOVE==Event )
      {   s_get_mouse( &x, &y );

          if( R2!=NULL ) s_restore( );
          if( R1!=NULL ) s_restore( );

          if( R1!=NULL )
          {   R1->y1=y ;	      
	      s_save_rame( R1->y1,R1->x1,R1->y2,R1->x2 );
	      s_rame1_f( R1->y1,R1->x1,R1->y2,R1->x2,0x0b );
	  }
          if( R2!=NULL )
          {   R2->y2=y ;
	      s_save_rame( R2->y1,R2->x1,R2->y2,R2->x2 );
	      s_rame1_f( R2->y1,R2->x1,R2->y2,R2->x2,0x0b );
	  }
      }
      if( MOUSE_RELEASE==Event )
      {   break ;
      }
   }
   s_set_mouse_move( 0 );

   RCurrentL.y1=RMainL.y1 ;
   RCurrentL.y2=RMainL.y2 ;
   RFrom.y2    =RFrom.y1+2 ;
   RTo.y1      =RFrom.y2 ;
   RTo.y2      =RTo.y1+2 ;
}
/****************************************************************************/
void  perevod_loop( void *V )
{  char *Func=(char *)V ;

   t_SetPerevod *SP=&SetPerevod ;

   switch( Func[0] )
   {
      case '0' : SP->loop( F_WORD_EXAMPLE_PREV,0,0,0 ); SP->paint();break ;
      case '1' : SP->loop( F_WORD_EXAMPLE_NEXT,0,0,0 ); SP->paint();break ;
      case '2' : SP->loop( F_WORD_WORD_PREV   ,0,0,0 ); SP->paint();break ;
      case '3' : SP->loop( F_WORD_WORD_NEXT   ,0,0,0 ); SP->paint();break ;
      case '4' : SP->loop( F_WORD_KONTEXT_UP  ,0,0,0 ); SP->paint();break ;
      case '5' : SP->loop( F_WORD_KONTEXT_DOWN,0,0,0 );             break ;
      case '6' : SP->loop( F_WORD_KONTEXT_DELETE ,0,0,0 );          break ;
      case '7' : SP->loop( F_WORD_MAIN_UP     ,0,0,0 ); SP->paint();break ;
      case '8' : SP->loop( F_WORD_MAIN_DOWN   ,0,0,0 ); SP->paint();break ;
      case '9' : SP->loop( F_WORD_MAIN_INSERT ,0,0,0 );             break ;
      case 'I' : SP->loop( F_WORD_MAIN_INSERT1,0,0,0 );             break ;
      case 'j' : SP->loop( F_WORD_HUMAN_UP   ,0,0,0 ); break ;
      case 'k' : SP->loop( F_WORD_HUMAN_DOWN ,0,0,0 ); break ;

      case 'm' : SP->loop( F_WORD_WIZARD     ,0,0,0 ); break ;
      case 'n' : SP->loop( F_WORD_GET        ,0,0,0 ); break ;
      case 's' : SP->loop( F_WORD_SELECT_DICT,0,0,0 ); break ;
   }
}
/*****************************************************************************/
e_WinMsg t_SetPerevod :: loop( e_Function Func, short ZZ, ushort key1, ushort key2 )
{
   short      r1 ;
   e_WinMsg   r ;
   t_wType    &T=Kontext.Type[i_type] ;

      switch( Func )
      {
          case F_MENU1: 
	       r1=menu( SetPerevod_menu, y1 ); 
	       if( 'x'==r1 ) return WM_ESC ;
	       break ;
          // ----------- хождение по исходному тексту (по экземплярам) --------
          case F_WORD_EXAMPLE_PREV:
               if( T.i_exemplar<ii_exemplar )
	       {   ii_exemplar-- ;
		   set_i_exemplar( Kontext.i_Exemplar[ii_exemplar] );
	       }
	       break ;
	  case F_WORD_EXAMPLE_NEXT:
               if( ii_exemplar<T.i_last_exemplar-1 )
	       {   ii_exemplar++ ;
		   set_i_exemplar( Kontext.i_Exemplar[ii_exemplar] );
	       }
	       break ;
          // ----------- хождение по подстрочнику ------------------------
	  case F_WORD_WORD_PREV:
	       First.step_up(); 
	       Slowo.set_str( First.get_from() );
	       Slowo.main_loop1_1( );
	       EditSlowo.init( Slowo.answer() );
	       break ;
	  case F_WORD_WORD_NEXT:
	       First.step_dn(); 
	       Slowo.set_str( First.get_from() );
	       Slowo.main_loop1_1( );
	       EditSlowo.init( Slowo.answer() );
	       break ;
          // ------------ хождение в текущем словаре ---------------------
	  case F_WORD_KONTEXT_UP    : 
	       r=loop( F_OKEY, 0, 0, 0 );
	       if( r==WM_ERROR ) break ;
               if( F_current!=NULL )
                   Edit1.clr_thisfrasa( F_current->begin, F_current->end );
	       Kontext2.loop_small( F_END_OF_LIST,0,0,S_key_Up );
	       init( Kontext2.View[Kontext2.PY] );
	       break ;
	  case F_WORD_KONTEXT_DOWN  : 
	       r=loop( F_OKEY, 0, 0, 0 );
	       if( r==WM_ERROR ) break ;
               if( F_current!=NULL )
                   Edit1.clr_thisfrasa( F_current->begin, F_current->end );
	       Kontext2.loop_small( F_END_OF_LIST,0,0,S_key_Down );
	       init( Kontext2.View[Kontext2.PY] );
	       break ;
//	  case F_WORD_KONTEXT_DELETE: CurrentList.current_del(); break ;
	  // ----------- хождение в главном словаре ----------------------
	  case F_WORD_MAIN_UP    : MainList.step_up(); break ;
	  case F_WORD_MAIN_DOWN  : MainList.step_dn(); break ;
	  case F_WORD_MAIN_INSERT:
	       if( MainList.PY<MainList.j )
 	           WordWizard.EditWord.insert_perevod( MainList[MainList.PY].Str ); 
	       break ;
	  case F_WORD_MAIN_INSERT1:
	       if( MainList.PY<MainList.j )
	           WordWizard.EditWord.set_perevod   ( MainList[MainList.PY].Str ); 
	       break ;
	  case F_WORD_HUMAN_UP:
	       if( 0<Slowo.i_word )
	       {   Slowo.set_i_word( Slowo.i_word-1 );
		   EditSlowo.init( Slowo.answer() );
	       }break ;
	  case F_WORD_HUMAN_DOWN:
	       if( Slowo.i_word<Slowo.n_word() )
	       {   Slowo.set_i_word( Slowo.i_word+1 );
		   EditSlowo.init( Slowo.answer() );
	       }break ;
          case F_WORD_GET:
	       answer_loop( );
	       break ;
          case F_WORD_SELECT_DICT:
	       Slowo.select_buch();
	       Slowo.main_loop1_1( );
	       EditSlowo.init( Slowo.answer() );
	       break ;
	  case F_END_OF_LIST:
	       switch( key1 )
	       {  // почти то же самое, как в гипертексте 
        	  case 0 :
		       if( 0==CurrentFrame )
                           WordWizard.loop( ZZ,key1,key2 );
		       break ;
        	  case S_key_Esc: return WM_ESC ;
		  case S_key_Enter:
		       //answer_loop( );
		       break ;
		  case S_key_TabR:
		       CurrentFrame=(CurrentFrame+1)%3 ;
		       if( 2==CurrentFrame )
		       {   Slowo.f_2Rame=0 ;
		           Slowo.paint( Slowo.i_word, Slowo.first_word );
		           answer_loop( );
		           CurrentFrame=0 ;
		       }	   
		       break ;
		  case S_key_Back:
		       if( 0==CurrentFrame )
                       {   WordWizard.loop( ZZ,key1,0 );
		       }
		       if( 1==CurrentFrame )
                       {   Slowo.key_back(); 
		           Slowo.main_loop1_1( );
		           EditSlowo.init( Slowo.answer() );
		       }
		       break ;
		  default :
		       if( 0==CurrentFrame )
                       {   WordWizard.loop( ZZ,key1,0 );
		       }
		       if( 1==CurrentFrame )
		       {   Slowo.key_add( Unicod_to_koi8(key1) );
			   Slowo.main_loop1_1( );
			   EditSlowo.init( Slowo.answer() );
		       }
		       break ;
	       }
	       break ;
	  case F_HELP: Window_help( (void *)"k_kontext1.html"); break ;
	  case F_OKEY:
	       {  char   Str2[100] ;
	          t_sRecord1 Rec ;

		  myChar_to_koi9( WordWizard.EditWord.Str,(uchar *)Str2,100,100 );
		  WordWizard.ErrStr=Rec.parse2( Str2,&Grammar.format1() );
		  if( WordWizard.ErrStr[0]!=0 ) return WM_ERROR ;
		  Strcpy( T.Str, Str2, 100 );
	       }
	       return  WM_ESC ;
	  case F_EXIT:
	       return  WM_ESC ;
      }
      return WM_NULL1 ;
}
/*****************************************************************************/
e_WinMsg t_SetPerevod :: answer_loop( )
{
   ushort ZZ,key1, key2 ;
   e_sEvent    Event ;

   CurrentFrame=2 ;
   paint( );
   EditSlowo.f_2Rame=1 ;
   while( 1 )
   {
      EditSlowo.paint();
      paint_hotkey( n_Line-1, 0, "[F1]Подсказка[Tab]В словарную запись[^S+Insert]Вставить" );
      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short x,y ;
          s_get_mouse( &x, &y );
	  if( WordWizard.y1<=y && y<=WordWizard.y2 )
	  {   CurrentFrame=0 ; goto M_Esc;  }
	  if( !Slowo.rect2.inside( x,y ) )
	  {   CurrentFrame=1 ; goto M_Esc;  }
      }

      if( (ZZ&(S_Shift_L|S_Shift_R))!=0 )
      {
          switch( key2 )
	  {
	  case S_key_Up:    EditSlowo.loop( S_Shift_L,0,S_key_Up   ); break ;
	  case S_key_Down:  EditSlowo.loop( S_Shift_L,0,S_key_Down ); break ;
	  case S_key_Left:  EditSlowo.loop( S_Shift_L,0,S_key_Left ); break ;
	  case S_key_Right: EditSlowo.loop( S_Shift_L,0,S_key_Right); break ;
	  case S_key_Ins:   EditSlowo.loop( S_Ctrl_L, 0, S_key_Ins );
			    WordWizard.EditWord.insert_text( EditSlowo.get_Buf() );
			    CurrentFrame=0 ; 
			    goto M_Esc;
	  }
      }
      else
      {
          switch( key2 )
	  {
	  case S_key_Up:    EditSlowo.loop( 0,0,S_key_Up   ); break ;
	  case S_key_Down:  EditSlowo.loop( 0,0,S_key_Down ); break ;
	  case S_key_Left:  EditSlowo.loop( 0,0,S_key_Left ); break ;
	  case S_key_Right: EditSlowo.loop( 0,0,S_key_Right); break ;      
	  case S_key_F10:   CurrentFrame=0 ; goto M_Esc;
          }
      }
      if( S_key_Esc==key1 || S_key_TabR==key1 )
M_Esc:{   EditSlowo.f_2Rame=0 ;
          paint( );
          return WM_ESC ;
      }
   }
}
/*****************************************************************************/
void resize_vindow( void *Void )
{   SetPerevod.resize_vindow( Void );
}
/*****************************************************************************/
void t_SetPerevod :: resize_vindow( void *Void )
{  char   *Com=(char *)Void ;
   t_Rect *R ;

   if( Com[0]=='1' ) R=&REdit1 ;
   if( Com[0]=='2' ) R=&RFirst ;
   if( Com[0]=='3' ) R=&RMainL ;
   if( Com[1]=='+' ) R->y2++ ;
   if( Com[1]=='-' ) R->y2-- ;
   
   RFirst.y1 =REdit1.y2 ;
   RMainL.y1 =RFirst.y2 ;
   RSlowo.y1 =RMainL.y2 ;

   RFrom.y1 =RMainL.y2 ;
   RFrom.y2 =RMainL.y2+2 ;
   RTo.y1   =RMainL.y2+2 ;
   RTo.y2   =RMainL.y2+4 ;
   RSlowo.y1=RMainL.y2+4 ;

   RCurrentL.y1=RMainL.y1 ;
   RCurrentL.y2=RMainL.y2 ;
   set_pos( );
   paint();
}
/*****************************************************************************/
DEFINE_LIST_BTYPE( t_wStr ,t_wStrList )
