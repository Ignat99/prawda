// ****************************************************************************
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//             Работа с "крупной структурой" текста (интерфейс)
//     Анисимов Д.В.                               сегодня
// ****************************************************************************
# include "mylib.h"
# include "option.h"
# include "video.h"
# include "common.h"
# include "debug.h"
# include "windows.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

void  macro_loop( void *V );
void  frasa_loop( void *V );
void  resize_vindow1( void *V );

// -------------------------------------------------------------------------
t_Menu Macro_menu1[] =
{
   { F_FUNK  ,0,macro_loop, (void *)"7"  ,"(Пере)Построить" },
   { F_FUNK  ,0,macro_loop, (void *)"8"  ,"Перевести все" },
   { F_NUMER ,0,NULL      , (void *)"x"  ,"Выход" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Macro_menu2[] =
{
   { F_FUNK  ,0,macro_loop, (void *)"9"  ,"Перевести Авт.      " },
   { F_FUNK  ,0,macro_loop, (void *)"2"  ,"Перевести Руч.      " },
   { F_FUNK  ,0,macro_loop, (void *)"6"  ,"Флаги               " },
   { F_FUNK  ,0,Sorry     , NULL         ,"Вставить            " },
   { F_FUNK  ,0,macro_loop, (void *)"5"  ,"Удалить             " },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Macro_menu3[] =
{
   { F_FUNK  ,0,macro_loop, (void *)"1"  ,"Переключить язык     " },
   { F_FUNK  ,1,macro_loop, (void *)"3"  ,"Сдвиг вправо         " },
   { F_FUNK  ,1,macro_loop, (void *)"4"  ,"Сдвиг влево          " },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Macro_help[] =
{  { F_FUNK  ,0,Window_help ,(void *)"index.html " ,"Содержание    "},
   { F_FUNK  ,0,Window_help ,(void *)"m_macro.html","Об этом режиме"},
   { F_ENDE  ,0,NULL        ,NULL,"" }
};
t_Menu Macro_menu[] =
{
   { F_MENU  ,0,NULL       ,Macro_menu1, "Гипертекст"},
   { F_MENU  ,0,NULL       ,Macro_menu2, "Фразу"    },
   { F_MENU  ,0,NULL       ,Macro_menu3, "Просмотр"},
   { F_MENU  ,0,NULL       ,Macro_help , "Подсказка"},
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
// ----------- меню для режима Фраза ---------------------------
t_Menu Frasa_menu1[] = // действие
{
   { F_FUNK  ,0,frasa_loop   ,(void *)"6"   ,"Перевод            " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"7"   ,"Отмена             " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"a"   ,"СледФраза          " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"9"   ,"ПредФраза          " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"5"   ,"Флаги              " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"8"   ,"Отладчик           " },
   { F_NUMER ,0,NULL         ,(void *)"x"   ,"Выход" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Frasa_menu2[] = // окна
{
   { F_FUNK  ,0,frasa_loop   ,(void *)"m"   ,"Переход фокуса         " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"n"   ,"Взять из словаря       " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"k"   ,"Перейти в словарь      " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"s"   ,"Переключить словарь    " },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Frasa_menu3[] = // шаги
{
   { F_SCHREI,0,NULL         ,NULL          ,"Исходный текст            " },
   { F_FUNK  ,1,frasa_loop   ,(void *)"0"   ,"   Шаг вверх             2" },
   { F_FUNK  ,1,frasa_loop   ,(void *)"1"   ,"   Шаг вниз              2" },
   { F_SCHREI,0,NULL         ,NULL          ,"Подстрочник               " },
   { F_FUNK  ,1,frasa_loop   ,(void *)"2"   ,"   Шаг вверх             3" },
   { F_FUNK  ,1,frasa_loop   ,(void *)"3"   ,"   Шаг вниз              3" },
   { F_FUNK  ,0,frasa_loop   ,(void *)"4"   ,"Все варианты              " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"d"   ,"Вставить перевод          " },
   { F_FUNK  ,0,frasa_loop   ,(void *)"e"   ,"Вставить оригинал         " },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Frasa_menu4[]= // Размер окошек
{
   { F_FUNK  ,1,resize_vindow1 ,(void *)"1+"   ,"+ Исходный текст      " },
   { F_FUNK  ,1,resize_vindow1 ,(void *)"1-"   ,"- Исходный текст      " },
   { F_FUNK  ,1,resize_vindow1 ,(void *)"2+"   ,"+ Подстрочник         " },
   { F_FUNK  ,1,resize_vindow1 ,(void *)"2-"   ,"- Подстрочник         " },
   { F_FUNK  ,1,resize_vindow1 ,(void *)"3+"   ,"+ Человеческий словарь" },
   { F_FUNK  ,1,resize_vindow1 ,(void *)"3-"   ,"- Человеческий словарь" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Frasa_help[] =
{  { F_FUNK  ,0,Window_help ,(void *)"index.html " ,"Содержание    " },
   { F_FUNK  ,0,Window_help ,(void *)"m_frasa.html","Об этом режиме"},
   { F_ENDE  ,0,NULL        ,NULL,"" }
};
t_Menu Frasa_menu[] =
{
   { F_MENU  ,0,NULL       ,Frasa_menu1, "Действие"},
   { F_MENU  ,0,NULL       ,Frasa_menu2, "Окна" },
   { F_MENU  ,0,NULL       ,Frasa_menu3, "Хождение"},
   { F_MENU  ,0,NULL       ,Frasa_menu4, "Размер"},
   { F_MENU  ,0,NULL       ,Frasa_help , "Подсказка"},
   { F_ENDE  ,0,NULL,NULL         ,"" }
};


       t_Text       T2 ; // текст для окна фразы
       t_Text       T3 ; // текст для окна словаря
extern t_Kontext    Kontext ;
extern t_FirstLook  EditFrom ;
extern t_Edit3      EditTree1,EditTree2 ;
extern t_Debug      Debug ;
extern t_HotKey2    KeyMacro[] ;
extern t_HotKey2    KeyFrasa[] ;

t_FrasaFace       Frasa ;
t_VariantView VariantView ;
t_FormWisard  FormWisard ;

void canon_string( t_String32 &Std, t_String32 &Str );
void format( t_String &In, t_String &Out, short First, short Left, short Right );

/*****************************************************************************/
//
/*****************************************************************************/
t_Macrostruct2 :: t_Macrostruct2( )
{
    Begin=0 ;
    PY   =0 ;
}
/*****************************************************************************/
//                   нарисовать список фраз и абзацев
/*****************************************************************************/
void t_Macrostruct2 :: paint( long Begin, long PY )
{  short i ;

   t_Win::paint();
   s_rame2_F( y1+1, x1, y2, x2, 0x0f );
   s_text_yxf( y1+1,x1+3 ,titl_Color, Name );

   s_rame1_f( y1+2, x1+ 5, y1+4, x1+17, 0x0f );
   s_rame1_f( y1+3, x1+10, y1+4, x1+17, 0x0f );

   s_text_yxf( y1+2,x1+17 ,0x0f, " Как сделан перевод" );
   s_text_yxf( y1+3,x1+17 ,0x0f, " Подлежит ли переводу" );

   for( i=0 ; i<=size_y-5 ; i++ )
      paint1( Begin+i, 4+y1+i, Begin+i==PY );
   paint_hotkey( n_Line-1, 0, hk_Macrostruct.c_str() );
   paint_header( "Перевод фраза за фразой" );
   null_menu( Macro_menu,y1 );
}
/*****************************************************************************/
//                   нарисовать строку (абзац или фразу)
//      i_list - индекс в списке
//      y      - координата где рисовать
//      f      - признак текущего
/*****************************************************************************/
void t_Macrostruct2 :: paint1( long i_list, long y, char f )
{  short i,j ;
   char  f1 ;
   char *s,*s1,*s2 ;
   char  Str[100] ;
   static t_myChar *S2, Str2[200] ;

   if( f==0 ) f1=0x07 ; else f1=0x70 ;
   s=emptystr( x2-x1-1 );
   s_text_yxf( y,x1+1 ,f1, s );

   if( Macrostruct.Sort.j<=i_list ) return ;
   t_HT_Sort &S=Macrostruct.Sort[i_list] ;

   if( S.i_frasa<0 )
   {   // ------- нарисовать абзац --------------------
       t_Absatz     *A =&Macrostruct.Absatz[S.i_absatz] ;
       if( 0<=A->type && A->type<Macrostruct.TypeAbsatz.j )
       {   t_TypeAbsatz *TA=&Macrostruct.TypeAbsatz[A->type] ;
	   sprintf( Str,"Абзац %2d %s",A->type, TA->Name );
	   if( 0==f ) f1=TA->Color ;
               else   f1=((TA->Color&0xf0)>>4) | ((TA->Color&0x0f)<<4) ;
       }
       else
       {   sprintf( Str,"Абзац %2d неизвестный",A->type );
       }
       s_text_yxf( y, x1+1 ,f1, s );
       s_text_yxf( y, x1+1, f1, Str );
   }
   else
   {   // ------- нарисовать фразу --------------------
       t_Frasa   &F=Macrostruct.Absatz[S.i_absatz].Frasa[S.i_frasa] ;
       switch( F.perevod )
       {  case NOT  : s1="Нет " ; break ;
          case AUTO : s1="Авто" ; break ;
          case HAND : s1="Чел." ; break ;
       }
       if( F.flag1==0 ) s2="Нет" ;
           else         s2="Да " ;

       long L=min( x2-x1-19,99 );
       L=min( L,F.end-F.begin+1 );
       sprintf( Str,"   %s %s",s1,s2 );
       s_text_yxf( y,x1+1 ,f1, Str );
       if( 0==f_Dst )
       {   // ------- отобразить оригинал -----------------
           // ------- исключить из фразы "мусорные" символы
	   S2=Macrostruct.Text->text_all()+F.begin ;
	   L=F.end-F.begin ;
	   for( j=0,i=0 ; i<L && j<198 ; i++ )
              if( 0x0d00 < S2[i].Char )
		  Str2[j++]=S2[i] ;
	   Str2[j]=0 ;
	   if( Begin_X<j )
	   {   Str2[min(199,Begin_X+x2-x1-20)]=0 ;
	       s_text32_yxf( y,x1+20 ,f1, Str2+Begin_X );
           }
       }
       else
       {   // ------- отобразить перевод ------------------
           L=0 ;
	   if( F.Str.Str!=NULL )
               L=strlen32(F.Str.Str);
	   if( 0<F.Str.Length )
	   {   if( Begin_X<L )
	       {   strncpy32( Str2,F.Str.Str+Begin_X,199 );
		   Str2[min(199,L-Begin_X)]=0 ;
		   Str2[min(199,x2-x1-20)]=0 ;
	       }
	       else Str2[0]=0 ;
	   }
	   else Str2[0]=0 ;
	   if( Begin_X<L )
	       s_text32_yxf( y,x1+20 ,f1, Str2 );
       }
   }
}
/*****************************************************************************/
//
/*****************************************************************************/
e_WinMsg t_Macrostruct2 :: main_loop ( void )
{
   short       sx1,sy1,button,mouse_x,mouse_y,r1 ;
   ushort      ZZ,key1,key2,k ;
   e_Function  Func ;
   e_sEvent    Event ;
   e_WinMsg    Ret ;

   s_get_size( sx1,sy1 );
   set_pos( 0, 1, sx1-1, sy1-2 );

   if( 0!=Macrostruct.was_maked() )
       if( 0>Macrostruct.format_is_correct() )
       {   Macrostruct.automake2();
           Macrostruct.make_ht_sort();
       }

   Begin_X=0 ;
   f_Dst=0 ;
   while( 1 )
   {
      paint( Begin, PY );

      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
          if( mouse_y==1 )
	  {   r1=menu_mouse( Macro_menu, y1-1,mouse_x ) ;
	      if( 'x'==r1 ) return WM_ESC ;
	      continue ;
	  }
	  if( mouse_y==n_Line-1 )
	      Option.get_key( &ZZ, &key1, &key2, hk_Macrostruct.c_str(), mouse_x );
          if( button==1 )
	  {   if( y1+3<mouse_y && mouse_y<y2 )
	          PY=min(Begin+mouse_y-(y1+4),Macrostruct.Sort.j-1) ;
	  }
          if( button==3 ){  ZZ=0, key1='\r' ; key2=0  ;  }
          if( button==4 ){  ZZ=0, key1=0    ; key2=S_key_Up  ;  }
          if( button==5 ){  ZZ=0, key1=0    ; key2=S_key_Down;  }
      }
      if( 30<key1 ) k=key1>>8 ;
          else      k=key1 ;

      Func=Option.get_func( KeyMacro, ZZ,k,key2 );

      Ret=loop( Func, ZZ, key1, key2 );
      if( WM_ESC==Ret ) return WM_ESC ;
   }
   return WM_OK ;
}
/*****************************************************************************/
void  macro_loop( void *V )
{  char *Func=(char *)V ;

   switch( Func[0] )
   {
      case '0' : Macrostruct2.loop( F_HELP       ,0,0,0 ); break ;
      case '1' : Macrostruct2.loop( F_MACRO_LANG ,0,0,0 ); break ;
      case '2' : Macrostruct2.loop( F_MACRO_ENTER,0,0,0 ); break ;
      case '3' : Macrostruct2.loop( F_MACRO_LEFT ,0,0,0 );
                 Macrostruct2.paint( Macrostruct2.Begin, Macrostruct2.PY );
                 break ;
      case '4' : Macrostruct2.loop( F_MACRO_RIGHT,0,0,0 ); 
                 Macrostruct2.paint( Macrostruct2.Begin, Macrostruct2.PY );
                 break ;
      case '5' : Macrostruct2.loop( F_MACRO_DEL   ,0,0,0 ); break ;
      case '6' : Macrostruct2.loop( F_MACRO_FLAG  ,0,0,0 ); break ;
      case '7' : Macrostruct2.loop( F_MACRO_MAKE  ,0,0,0 ); break ;
      case '8' : Macrostruct2.loop( F_MACRO_TRANS ,0,0,0 ); break ;
      case '9' : Macrostruct2.loop( F_MACRO_TRANS1,0,0,0 ); break ;
   }   
}
/*****************************************************************************/
e_WinMsg t_Macrostruct2 :: loop( e_Function Func, short ZZ, ushort key1, ushort key2 )
{
   short     r1 ;
   t_HT_Sort *S=&Macrostruct.Sort[PY] ;
      
try
{
   switch( Func )
   {
      case F_HELP: Window_help( (char *)"m_macro.html" ); break ;
      case F_MACRO_LANG:// ------ переключение "оригинал" - "перевод"
	   if( 0==f_Dst ) f_Dst=1 ; else f_Dst=0 ; break ;
      case F_MACRO_ENTER: // ------ переход в окно "перевести фразу" 
M_Frasa :    S=&Macrostruct.Sort[PY] ;
           if( 0<=S->i_frasa )
	   {	  e_WinMsg Ret ;
	       Ret=frasa_work( S->i_absatz, S->i_frasa );
	       if( WM_P_FRASA==Ret )
	       {   for( short i=PY-1 ; 0<=i ; i-- )
		      if( 0<=Macrostruct.Sort[i].i_frasa ) 
		      {   PY=i ; goto M_Frasa ;  }
	       }
	       if( WM_N_FRASA==Ret )
	       {   for( short i=PY+1 ; i<Macrostruct.Sort.j ; i++ )
		      if( 0<=Macrostruct.Sort[i].i_frasa ) 
		      {   PY=i ; goto M_Frasa ;  }
	       }
	   }
	   break ;
      case F_MACRO_LEFT : if( Begin_X>0 ) Begin_X-- ; break ;
      case F_MACRO_RIGHT : Begin_X++ ; break ;
      case F_MACRO_DEL :// ----- удалить фразу или абзац ------
	   Macrostruct.delete_sort( PY ); 
	   break ;
      case F_MACRO_INS :
	   if( S->i_frasa<0 )
	           Macrostruct.insert_absatz( S->i_absatz );
	      else Macrostruct.split_absatz( S->i_absatz, S->i_frasa );
	   Macrostruct.make_ht_sort( );
	   break ;
      case F_MACRO_FLAG: // ----- поставить флаги фразе --------
	   if( S->i_frasa<0 ) flag_absatz( Macrostruct.Absatz[S->i_absatz] );
	       else           flag_frasa( Macrostruct.Absatz[S->i_absatz].Frasa[S->i_frasa] );
	   break ;
      case F_MACRO_MAKE:// ----- построить гипертекст ---------
 	   Macrostruct.make();
	   break ;
      case F_MACRO_TRANS1: // ----- перевод одной фразы ---------
           if( 0<=S->i_frasa )
               translate_frasa( S->i_absatz, S->i_frasa );
	   break ;
      case F_MACRO_TRANS: // ----- перевод всего гипертекста ----
	   translate_all();
	   f_Dst=1 ;
	   break;
      case F_MENU1:
	   r1=menu( Macro_menu, y1 ) ;
	   if( 'x'==r1 ) return WM_ESC ;	      
	   break;
      case F_OKEY: return WM_ESC ;
      case F_END_OF_LIST:
	   switch( key1 )
	   {
	      case 0:
	      {   switch( key2 )
		  {  // ------------ хождение вверх-вниз по списку фраз ----
		     case S_key_Up :
			  if( PY>0 ) PY-- ;
			  if( PY<Begin ) Begin=PY ;
			  break ;
		     case S_key_Down :
			  if( PY<Macrostruct.Sort.j-1 ) PY++ ;
			  if( (size_y-5)<PY-Begin ) Begin=PY-(size_y-5) ;
			  break ;
		     case S_key_Home : PY=0 ; break ;
		     case S_key_End  : PY=Macrostruct.Sort.j-1 ; break ;
		     case S_key_PgUp :
			  PY=max(0,PY-(size_y-5) );
			  if( PY<Begin ) Begin=PY ;
			  break ;
		     case S_key_PgDn :
			  PY=min( Macrostruct.Sort.j-1,PY+(size_y-5) );
			  if( (size_y-5)<PY-Begin ) Begin=PY-(size_y-5) ;
			  break ;
		  }
	      }
	   }
   }
}
catch( ... )
{
    figna( error_get() );
}
   return WM_OK ;
}
/*****************************************************************************/
//                     перевести все фразы гипертекста
/*****************************************************************************/
void t_Macrostruct2 :: translate_all()
{
   char  Str2[100] ;

   Kontext.make_current_perevod();
   set_main_dict( 1 );
   for( long i=0 ; i<Macrostruct.Sort.j ; i++ )
   {
      sprintf( Str2,"Переведено %d/%d",i,Macrostruct.Sort.j );
      message( Str2 );

      t_HT_Sort *S=&Macrostruct.Sort[i] ;
      if( S->i_frasa<0 ) continue ;
      t_Frasa   *F=&Macrostruct.Absatz[S->i_absatz].Frasa[S->i_frasa] ;
      if( F->flag1==0 || F->perevod==HAND  ) continue ;
      translate_frasa( S->i_absatz, S->i_frasa );
   }
   set_main_dict( 0 );
}
/*****************************************************************************/
void t_Macrostruct2 :: translate_frasa( long i_absatz, long i_frasa )
{
   static char       Str1[1000],Str2[1000] ;
   static t_myChar   Str3[1000] ;
   static t_String   StrFine ;

   t_Frasa  *F  =&Macrostruct.Absatz[i_absatz].Frasa[i_frasa] ;
   t_myChar *Str=Macrostruct.Text->text_all()+F->begin ;
   long      L  =F->end-F->begin ;

   myChar_to_koi9( Str, (uchar *)Str1, L, 1000 );
   Core.translate( Str1,Str2 );
   StrFine=Frasa.str_fine( Str2 );
   koi9_to_myChar( (char *)StrFine.c_str(), Str3, 1000, 1000 );
   F->Str.set( Str3 ) ;
   F->perevod=AUTO ;
}
/*****************************************************************************/
//                        установить флаги фразе
/*****************************************************************************/
e_WinMsg t_Macrostruct2 :: flag_frasa( t_Frasa &F )
{
   short  xx,yy,i_Flag=0,f ;
   ushort ZZ,key1,key2 ;
   char  *s1,*s2 ;
   short  button,mouse_x,mouse_y ;
   e_sEvent  Event ;
   char  *StrPerevod[]={ "Нет ","Авто","Чел." };
   char  *StrFlag   []={ "Нет" ,"Да " };

   xx=(x1+x2)/2 ;
   yy=(y1+y2)/2 ;
   
   s_rame2_F( yy-3, xx-30, yy+3, xx+30, 0x0f );
   s_text_yxf( yy-3,xx-27 , 0x0b, "Флаги" );

   while( 1 )   
   {  s1=StrPerevod[F.perevod] ;
      s2=StrFlag[F.flag1] ;

      if( i_Flag==0 ) f=cur_item_Color ; else f=0x07 ;
      s_text_yxf( yy-2,xx-29 , f, "Перевод: " );
      s_text_yxf( yy-2,xx-20 , f, s1 );

      if( i_Flag==1 ) f=cur_item_Color ; else f=0x07 ;
      s_text_yxf( yy-1,xx-29 , f, "Автомат: " );
      s_text_yxf( yy-1,xx-20 , f, s2 );

      Event=s_getch( &key1,&key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
	  if( mouse_x<xx-30 || xx+30<mouse_x || mouse_y<yy-3 || yy+3<mouse_y )
	      return WM_ESC ;
          if( 3==button )
	  {   if( yy-3<mouse_y && mouse_y<yy+3 )
	      {   i_Flag=mouse_y-yy+2 ; key2=S_key_Right ;  }	  
	  }
      }

      switch( key1 )
      {
         case ' ' : break ;
         case S_key_Esc : case '\r' :
	      return WM_ESC ;
	 case 0:
	 {   switch( key2 )
	     {
		case S_key_F1   : Window_help( (void *)"m_flags2.html") ; break ;
		case S_key_Up   : i_Flag-- ; break ;
		case S_key_Down : i_Flag++ ; break ;
		case S_key_Left :
                     if( i_Flag==0 ) F.perevod=(e_Perevod)((F.perevod+2)%3) ;
		     if( i_Flag==1 ) F.flag1=1-F.flag1 ;		     
		     break ;
		case S_key_Right :
                     if( i_Flag==0 ) F.perevod=(e_Perevod)((F.perevod+1)%3) ;
		     if( i_Flag==1 ) F.flag1=1-F.flag1 ;		     
		     break ;
		case S_key_F10 : return WM_ESC ;
	     }
	 }
      }
      if( i_Flag<0  ) i_Flag=1 ;
      if( 2<=i_Flag ) i_Flag=0 ;
   }
}
/*****************************************************************************/
//                       установить флаги абзацу
/*****************************************************************************/
e_WinMsg t_Macrostruct2 :: flag_absatz( t_Absatz &A )
{
   short  xx,yy,i_Flag=0,f ;
   ushort ZZ,key1,key2 ;
   char  *s1,*s2 ;
   short  button,mouse_x,mouse_y ;
   e_sEvent  Event ;
   t_String Buf ;

   xx=(x1+x2)/2 ;
   yy=(y1+y2)/2 ;
   
   s_rame2_F( yy-3, xx-30, yy+3, xx+30, 0x0f );
   s_text_yxf( yy-3,xx-27 , 0x0b, "Флаги параграфа" );

   while( 1 )
   {
      Buf="Тип: " ;
      if( 0<=A.type && A.type<Macrostruct.TypeAbsatz.j )
               Buf+=Macrostruct.TypeAbsatz[A.type].Name ;
	  else Buf+="Неизвестный" ;
      s_text_yxf( yy-2,xx-29 , 0x70,  emptystr(58) );
      s_text_yxf( yy-2,xx-29 , 0x70, (char *)Buf.c_str() );

      Event=s_getch( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
	  if( mouse_x<xx-30 || xx+30<mouse_x || mouse_y<yy-3 || yy+3<mouse_y )
	      return WM_ESC ;
          if( 3==button )
	  {   if( yy-2==mouse_y )
	      {   key2=S_key_Right ;  }	  
	  }
      }

      switch( key1 )
      {
         case '\r'     : return WM_OK ;
         case S_key_Esc: return WM_ESC ;
      }
      switch( key2 )
      {
 	 case S_key_F1   : Window_help( (void *)"m_flags1.html") ; break ;
         case S_key_Left : if( 0<A.type )          A.type-- ; break ;
	 case S_key_Right: if( A.type<Macrostruct.TypeAbsatz.j-1 ) A.type++ ; break ;
	 case S_key_F10:   return WM_ESC ;
      }   
   }
}
/*****************************************************************************/
//                      перевести фразу вручную
/*****************************************************************************/
e_WinMsg t_Macrostruct2 :: frasa_work( long i_absatz, long i_frasa )
{  short    sx1,sy1 ;
   e_WinMsg Ret ;

try
{
   Kontext.make_current_perevod( );
   set_main_dict( 1 );
   t_Frasa &F=Macrostruct.Absatz[i_absatz].Frasa[i_frasa] ;

   s_get_size( sx1,sy1 );
   Frasa.set_pos( 0,1,sx1-1,sy1-1 );
   Frasa.init( i_absatz, i_frasa, Macrostruct.Text->text_all()+F.begin, F.end-F.begin );
   Ret=Frasa.main_loop();

   set_main_dict( 0 );
   return Ret ;
}
catch( int E )
{  figna( error_get() );
   return WM_ESC ;
}
}

/****************************************************************************/
t_FrasaFace :: t_FrasaFace()
{
   fReg =0 ;
   From  =&  Edit1 ;
   To  .set_text( &T2 );

   Slowo.set_name( "Словарь" );
   First.set_name( "Подстрочник" );
   To   .set_name( "Переведенная фраза" );
   EditSlowo.set_name("Взять слово");

   First.f_2Rame=0 ;
   Slowo.f_2Rame=0 ;
   From->f_2Rame=0 ;
}
/****************************************************************************/
void t_FrasaFace :: set_pos( short _x1, short _y1, short _x2, short _y2 )
{
   calc_pos(  _x1, _y1, _x2, _y2 );
   set_pos( );
}
/******************************************************************************/
void t_FrasaFace :: calc_pos( short _x1, short _y1, short _x2, short _y2 )
{  short yy1,yy2,yy3 ;

   x1 = _x1 ; y1 = _y1 ; x2 = _x2 ; y2 = _y2 ;

   yy2=(y1 + y2)/2 ;
   yy1=(y1 +yy2)/2 ;
   yy3=(yy2+ y2)/2 ;
   yyy3 = yy3 ;

   RFrom .x1=x1 ; RFrom .y1=y1+2  ; RFrom .x2=x2 ; RFrom .y2=yy1 ;
   RFirst.x1=x1 ; RFirst.y1=yy1   ; RFirst.x2=x2 ; RFirst.y2=yy2 ;
   RSlowo.x1=x1 ; RSlowo.y1=yy2   ; RSlowo.x2=x2 ; RSlowo.y2=yy3 ;
   RTo   .x1=x1 ; RTo   .y1=yy3+1 ; RTo   .x2=x2 ; RTo   .y2=y2-1 ;
   
   RFormWisard.x1=x1+2             ; RFormWisard.y1=RFrom.y2+2 ; 
   RFormWisard.x2=x2-2             ; RFormWisard.y2=RTo.y1-2 ;
   RVariantView=RFormWisard ;
}
/******************************************************************************/
void t_FrasaFace :: set_pos( )
{
   From->set_pos( RFrom );
   First.set_pos( RFirst );
   Slowo.set_pos( RSlowo.x1,  RSlowo.y1, RSlowo.x2,  RSlowo.y2, 1 );
   To   .set_pos( RTo );

   EditSlowo.set_pos( Slowo.rect2 );
   VariantView.set_pos( RVariantView );
   FormWisard.set_pos( RFormWisard );
}
/****************************************************************************/
//          задать переводимую фразу
/****************************************************************************/
void t_FrasaFace :: init( long _i_Absatz, long _i_Frasa, t_myChar *Str, long L )
{
   static char Str1[1000],Str2[1000] ;

   i_Absatz=_i_Absatz ;
   i_Frasa =_i_Frasa ;
   Undo    = Macrostruct.Absatz[i_Absatz].Frasa[i_Frasa].Str ;
   L=myChar_to_koi9( Str, (uchar *)Str1, L, 1000 );
   Core.translate( Str1,Str2 );
   First.set_py( 0 );
}
/****************************************************************************/
void t_FrasaFace :: paint()
{  short yy ;

   From->paint();
   First.paint();
   Slowo.paint( Slowo.i_word, Slowo.first_word );
   EditSlowo.paint();
   yy = max( RFrom.y2, RFirst.y2 );
   yy = max( yy, RSlowo.y2  );
   if( fReg==0 ) s_rame2_f( y1+1,x1,yy,x2,0x0f );
       else      s_rame1_f( y1+1,x1,yy,x2,0x0f );
   To.paint();

   paint_hotkey( n_Line-1, 0, hk_Frasa.c_str() );
   paint_header( "Перевод фразы" );
   null_menu( Frasa_menu, y1 );
}
/****************************************************************************/
e_WinMsg t_FrasaFace :: main_loop()
{  long   PY, PX ;
   ushort ZZ,key1,key2,k ;
   e_Function  Func ;
   e_WinMsg    Ret ;
   e_sEvent    Event ;
   static char Str[1000] ; // безобразие

   t_Frasa *F=&Macrostruct.Absatz[i_Absatz].Frasa[i_Frasa] ;

   fReg=0 ; To.f_2Rame=0 ;
   Slowo.set_name( "Человеческий словарь" );
   Slowo.f_2Rame=0 ;
   From->f_2Rame=0 ;
   From->set_thisfrasa( F->begin, F->end );
   From->Text->get_xy_pos( F->begin, &PY, &PX );
   From->screen_set_y(PY-2);

   t_String In,Out ;
   Macrostruct.get_frasa2( i_Absatz, i_Frasa, Str );
   In=Str ;  
   format( In, Out, 0, 0, 70 );
   To.init( Out.c_str() );

   Undo=F->Str ;
   Slowo.set_str( First.get_from() );
   Slowo.main_loop1_1( );
   EditSlowo.init( Slowo.answer() );

   set_pos( );

   while( 1 )
   {  Func=F_NOP ;
      paint();
      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short button,x,y ;
          button=s_get_mouse( &x, &y );
          if( 1==button )
	  {   if( y==RFirst.y1 || y==RTo.y1 || y==RSlowo.y1 || y==RSlowo.y2 )
              {   loop_rect( y );
		  set_pos( );
		  continue ;
              }
	      if( RTo.inside( x,y ) )
	      {   Func=F_FRASA_WINDOW_SWITCH ; goto M_loop ;  }
	      if( y==y1 )
	      {   short r1=menu_mouse( Frasa_menu, y1, x ); 
		  if( 'x'==r1 ) return WM_ESC ;
		  continue ;
	      }
	      if( y==n_Line-1 )
	      {   Option.get_key( &ZZ, &key1, &key2, hk_Frasa.c_str(), x );
		  goto M_KeyPress ;
	      }
	      if( Slowo.rect2.inside( x,y ) )
	      {   Func=F_FRASA_GET ; goto M_loop ;  }
	      if( RFirst.inside( x,y ) )
	      {   First.PY=First.Begin+y-First.y1-1 ;
	          if( First.MaxY<=First.PY ) First.PY=First.MaxY-1 ;
	      }
          }
	  if( button==3 )
	  {   if( RFirst.inside( x,y ) ){  Func=F_FRASA_WORD_VARIANT ; goto M_loop ;  }
	  }
	  if( button==4 )
	  {   if( RFrom .inside( x,y ) ){  Func=F_FRASA_SRC_UP    ; goto M_loop ;  }
	      if( RFirst.inside( x,y ) ){  Func=F_FRASA_WORD_PREV ; goto M_loop ;  }
	  }
	  if( button==5 )
	  {   if( RFrom .inside( x,y ) ){  Func=F_FRASA_SRC_DOWN  ; goto M_loop ;  }
	      if( RFirst.inside( x,y ) ){  Func=F_FRASA_WORD_NEXT ; goto M_loop ;  }
	  }
      }
      if( KEY_PRESS==Event )
      {  M_KeyPress :
         if( 30<key1 ) k=key1>>8 ;
             else      k=key1 ;
         Func=Option.get_func( KeyFrasa, ZZ,k,key2 );
      }

      if( Func==F_FRASA_PREV ){  From->clr_thisfrasa( F->begin, F->end ); return WM_P_FRASA ; }
      if( Func==F_FRASA_NEXT ){  From->clr_thisfrasa( F->begin, F->end ); return WM_N_FRASA ; }
      
M_loop:
      Ret=loop( Func, ZZ, key1, key2 );
      if( WM_ESC==Ret ) goto M_Exit ;
   }
M_Exit:
   From->clr_thisfrasa( F->begin, F->end );
   From->f_2Rame=1 ;
   return WM_ESC ;
}
/****************************************************************************/
void t_FrasaFace :: loop_rect( short yy )
{  short    x,y ;
   t_Rect  *R1=NULL,
           *R2=NULL ;
   ushort   ZZ,key1,key2 ; // нажатая клавиша
   e_sEvent Event ;

//   if( yy==RFrom.y1 )     R1=&RFrom ;
   if( yy==RFirst.y1 )    R1=&RFirst ;
   if( yy==RSlowo.y1 )    R1=&RSlowo ;
   if( yy==RTo.y1 )       R1=&RTo ;

   if( yy==RFrom.y2 )     R2=&RFrom ;
   if( yy==RFirst.y2 )    R2=&RFirst ;
   if( yy==RSlowo.y2 )    R2=&RSlowo ;
//   if( yy==RTo.y2 )       R2=&RTo ;

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
}
/****************************************************************************/
void  frasa_loop( void *V )
{  char *Func=(char *)V ;

   switch( Func[0] )
   {
      case '0' : Frasa.loop( F_FRASA_SRC_UP       ,0,0,0 ); Frasa.paint(); break ;
      case '1' : Frasa.loop( F_FRASA_SRC_DOWN     ,0,0,0 ); Frasa.paint(); break ;
      case '2' : Frasa.loop( F_FRASA_WORD_PREV    ,0,0,0 ); Frasa.paint(); break ;
      case '3' : Frasa.loop( F_FRASA_WORD_NEXT    ,0,0,0 ); Frasa.paint(); break ;
      case '4' : Frasa.loop( F_FRASA_WORD_VARIANT ,0,0,0 ); break ;
      case 'd' : Frasa.loop( F_FRASA_WORD_VARIANT1,0,0,0 ); break ;
      case 'e' : Frasa.loop( F_FRASA_WORD_VARIANT2,0,0,0 ); break ;
      case '5' : Frasa.loop( F_FRASA_FLAGS        ,0,0,0 ); break ;
      case '6' : Frasa.loop( F_FRASA_TRANS        ,0,0,0 ); break ;
      case '7' : Frasa.loop( F_FRASA_UNDO         ,0,0,0 ); break ;
      case '8' : Frasa.loop( F_FRASA_DEBUG        ,0,0,0 ); break ;
      case '9' : Frasa.loop( F_FRASA_PREV         ,0,0,0 ); break ;
      case 'a' : Frasa.loop( F_FRASA_NEXT         ,0,0,0 ); break ;
      case 'b' : Frasa.loop( F_FRASA_HUMAN_UP     ,0,0,0 ); break ;
      case 'c' : Frasa.loop( F_FRASA_HUMAN_DOWN   ,0,0,0 ); break ;
      case 'm' : Frasa.loop( F_FRASA_WINDOW_SWITCH,0,0,0 ); break ;
      case 'n' : Frasa.loop( F_FRASA_GET          ,0,0,0 ); break ;
      case 'k' : Frasa.loop( F_FRASA_TODICT       ,0,0,0 ); break ;
      case 's' : Frasa.loop( F_FRASA_SELECT_DICT  ,0,0,0 ); break ;
   }
}
/****************************************************************************/
e_WinMsg t_FrasaFace :: loop( e_Function Func, short ZZ, ushort key1, ushort key2 )
{
   e_WinMsg  r ;
   short     r1 ;
   t_Frasa  *F=&Macrostruct.Absatz[i_Absatz].Frasa[i_Frasa] ;

   switch( Func )
   {
       case F_MENU1: 
            r1=menu( Frasa_menu, y1 );
	    if( 'x'==r1 ) return WM_ESC ;
            break ;
       // ----------- хождение по исходному тексту ---------------------
       case F_FRASA_SRC_UP   : 
	    From->screen_up();
	    break ;
       case F_FRASA_SRC_DOWN : 
	    From->screen_down();
	    break ;
       // ----------- хождение по подстрочнику ------------------------
       case F_FRASA_WORD_PREV:
	    First.step_up(); 
	    Slowo.set_str( First.get_from() );
	    Slowo.main_loop1_1( );
	    EditSlowo.init( Slowo.answer() );
	    break ;
       case F_FRASA_WORD_NEXT:
	    First.step_dn(); 
	    Slowo.set_str( First.get_from() );
	    Slowo.main_loop1_1( );
	    EditSlowo.init( Slowo.answer() );
	    break ;
       case F_FRASA_WORD_VARIANT : variant( First.get_py() ); break ;
       case F_FRASA_WORD_VARIANT1: variant1( First.get_py() ); break ;
       case F_FRASA_WORD_VARIANT2: variant2( First.get_py() ); break ;
       case F_FRASA_FLAGS:        Macrostruct2.flag_frasa( *F ); break ;
       case F_FRASA_TRANS:        translate(); break ;
       case F_FRASA_UNDO:         To.init32( Undo.Str ); break ;
       case F_FRASA_DEBUG: // -------- переход в окно "отладчик" -------
	    loop_debug();
	    break ;
       case F_FRASA_PREV: // -------- переход к предыдущей фразе -----------------
	    F->Str.set( To.text_all() );
	    From->clr_thisfrasa( F->begin, F->end );
	    return WM_P_FRASA ;
       case F_FRASA_NEXT: // -------- переход к следующей фразе -----------------
	    F->Str.set( To.text_all() );
	    From->clr_thisfrasa( F->begin, F->end );
	    return WM_N_FRASA ;
       case F_FRASA_HUMAN_UP:
	    if( 0<Slowo.i_word )
	    {   Slowo.set_i_word( Slowo.i_word-1 );
		EditSlowo.init( Slowo.answer() );
	    }break ;
       case F_FRASA_HUMAN_DOWN:
	    if( Slowo.i_word<Slowo.n_word() )
	    {   Slowo.set_i_word( Slowo.i_word+1 );
		EditSlowo.init( Slowo.answer() );
	    }break ;
       case F_FRASA_WINDOW_SWITCH: // ------- перейти в окно "переведенная фраза" -----
	    fReg=1 ; To.f_2Rame=1 ;
	    paint();
	    r=To.loop_frasa();
	    if( WM_ESC==r ) // надо с F10 унифицировать
	    {	  F->Str.set( To.text_all() );
	        From->f_2Rame=1 ;
		if( Undo.Str!=NULL )
        	{   t_String32 Str1,Str2 ;
		    canon_string( Str1, F->Str );
		    canon_string( Str2, Undo );
		    if( 0!=strncmp32( Str1.Str, Str2.Str, 1000 ) )
		        F->perevod=HAND ;
                }
	        return WM_ESC ;
	    }
	    fReg=0 ; To.f_2Rame=0 ;
	    break ;
       case F_EXIT:
	    From->clr_thisfrasa( F->begin, F->end );
	    return WM_ESC ;
       case F_HELP: Window_help( (void *)"m_frasa.html"); break ;
       case F_OKEY:
            To.put_str( );
	    F->Str.set( To.text_all() );
	    From->f_2Rame=1 ;
	    if( Undo.Str!=NULL ) // ---- здесь происходит ошибка из за незначимых символов
            {   t_String32 Str1,Str2 ;
		canon_string( Str1, F->Str );
		canon_string( Str2, Undo );
		if( 0!=strncmp32( Str1.Str, Str2.Str, 1000 ) )
		    F->perevod=HAND ;
	    }
	    return WM_ESC ;
       case F_FRASA_SELECT_DICT:
	    Slowo.select_buch();
	    Slowo.main_loop1_1( );
	    EditSlowo.init( Slowo.answer() );
	    break ;
       case F_FRASA_GET:
	    answer_loop( );
	    break ;
       case F_FRASA_TODICT:
            to_dict( );
            break ;
       case F_END_OF_LIST:
	    switch( key1 )
	    {
               case ' ' : break ; // в словаре пробелы только мешаются
	       case S_key_Back:    // ----- "забой" в человеческом словаре
		    Slowo.key_back();
		    Slowo.main_loop1_1( );
		    EditSlowo.init( Slowo.answer() );
		    break ;
	       case S_key_Enter:
		    //answer_loop( );
		    break ;
	       default :          // ------ добавить букву в человеческом словаре
		    Slowo.key_add( Unicod_to_koi8(key1) );
		    Slowo.main_loop1_1( );
		    EditSlowo.init( Slowo.answer() );
		    break ;
	       case 0:
	       {   switch( key2 )
		   {
                      case S_key_Del: Slowo.key_back(); break ;
      //		case S_key_F7 : FrasaWisard.main_loop() ; break ;
		      case S_key_F9 : // сам забыл, что делает эта кнопка
			   F->Str.set( To.text_all() );
			   break ;
		   }
	       }
	    }
	    break ;
   }
   return WM_OK ;
}
/*****************************************************************************/
e_WinMsg t_FrasaFace :: answer_loop( )
{
   ushort ZZ,key1, key2 ;
   e_sEvent    Event ;

   EditSlowo.f_2Rame=1 ;
   while( 1 )
   {
      EditSlowo.paint();
      paint_hotkey( n_Line-1, 0, "[F1]Подсказка[^S+Insert]Вставить" );

      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short x,y ;
          s_get_mouse( &x, &y );
	  if( !Slowo.rect2.inside( x,y ) )
	      goto M_Esc;
      }

      if( (ZZ&(S_Shift_L|S_Shift_R))!=0 )
      {
          switch( key2 )
	  {
	  case S_key_Up:    EditSlowo.loop( S_Shift_L,0,S_key_Up   ); break ;
	  case S_key_Down:  EditSlowo.loop( S_Shift_L,0,S_key_Down ); break ;
	  case S_key_Left:  EditSlowo.loop( S_Shift_L,0,S_key_Left ); break ;
	  case S_key_Right: EditSlowo.loop( S_Shift_L,0,S_key_Right); break ;
	  case S_key_Ins:
	       {  uchar s1[100] ;
	          EditSlowo.loop( S_Ctrl_L, 0, S_key_Ins );
		  t_myChar *s=EditSlowo.get_Buf();
		  To.set_Buf( s );
		  To.loop( S_Shift_L,0,S_key_Ins );
	       }
	       EditSlowo.f_2Rame=0 ;
	       return WM_OK ;
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
	  case S_key_F10:   EditSlowo.f_2Rame=0 ; return WM_ESC ;	          
          }
      }
      if( S_key_Esc==key1 )
M_Esc:{   EditSlowo.f_2Rame=0 ;
          return WM_ESC ;
      }
   }
}
/****************************************************************************/
void t_FrasaFace :: loop_debug()
{  e_WinMsg r=WM_DEBUG ; // (надо бы оформить в виде функции t_Windows)
   while( 1 )
   {   switch( r )
       {  case WM_FIRST: r=EditFrom.loop( );   break ;
	  case WM_SRC  : r=EditTree1.loop( );  break ;
	  case WM_DST  : r=EditTree2.loop( );  break ;
	  case WM_DEBUG: r=Debug.loop( );      break ;
	  default: return ;
       }
   }
}
/****************************************************************************/
//                     перевести фразу автоматически
/****************************************************************************/
void t_FrasaFace :: translate()
{
   char Str1[1000],Str2[1000] ;
   t_String Str3 ;

   Macrostruct.get_frasa( i_Absatz, i_Frasa, Str1 );
   Core.translate( Str1,Str2 );
   Str3=str_fine( Str2 );
   Macrostruct.put_frasa( i_Absatz, i_Frasa, (char *)Str3.c_str() );
   To.init( (char *)Str3.c_str() );
}
/*************************************************************************/
//     сделать фразу "покрасивее" убрать лишние пробелы и подчеркивания
/*************************************************************************/
char * t_FrasaFace :: str_fine( char *Str )
{
   static t_String Str1 ;
   char c1 ;

   Str1="" ;
   for( short i=0 ; Str[i]!=0 ; i++ )
   {  c1=Str[i+1] ;
      if( '_'==Str[i] )
      {   if( ' '==c1 || '_'==c1 ) continue ;
  	      else Str1+=' ' ; continue ;
      }
      if( ' '==Str[i] && (' '==c1 || '_'==c1) ) continue ;
      if( ' '==Str[i] && (','==c1 || ';'==c1) ) continue ;
      Str1+=Str[i] ;
   }
   return Str1.c_str() ;
}
/************************************************************************/
//    убрать из строки все незначимые для человека символы
/************************************************************************/
void canon_string( t_String32 &Std, t_String32 &Str )
{  long i,j ;
   char f ;
   Std=Str ;

   for( i=0 ; i<Std.Length && Std.Str[i]!=0 ; i++ )
   {  if( Std.Str[i]=='\n' ) Std.Str[i]=' ' ;
      if( Std.Str[i]=='\r' ) Std.Str[i]=' ' ;
   }
   f=1 ;
   for( i=j=0 ; i<Std.Length && Std.Str[i]!=0 ; i++ )
   {  if( Str.Str[i]==' ' && f ) continue ;
      Str.Str[j++]=Str.Str[i] ;
      f=Str.Str[i]==' ' ;
   }
   Std.Str[j]=0 ;
   if( Std.Str[j-1]==' ' )
       Std.Str[j-1]=0 ;
}
/*************************************************************************/
//                       взять вариант перевода
/*************************************************************************/
void t_FrasaFace :: variant( short i_word )
{
// VariantView.set_pos( x1+1, y1+1, x2-1, y2-1 );
   VariantView.make_variant_list( i_word );
   VariantView.loop();
}
/**************************************************************************/
//                вставить (первый) перевод слова
/**************************************************************************/
void t_FrasaFace :: variant1( short i_word )
{
   // ----- взято из t_FirstLook :: paint1 ------------
   char        *Str1 ;
   t_ItemList  &From =Core.from( );

   if( From[i_word].exist==0 ) return ;

   short       i_struct=Core.struct_of_word( i_word );
   t_Variants  *VV     =Core.variants( i_word, i_struct );
   t_rStruct   *VV1    =&VV->Variant[0] ;
   char        *Str    =From[i_word].Str ;

   if( 0==VV1->i_dic )
            Str1=Perevod->translate_word_i( Str, VV1->i_slowo, VV1->i_slowo1 );
       else Str1=Dogadka.translate_word_i ( Str, VV1->i_slowo, VV1->i_slowo1 );

   // ----- унифицировать с t_FormWisard :: func_ok() ----------
   t_myChar   myChar[256] ;

   koi9_to_myChar( Str1, myChar, 255, 255 );
   Frasa.To.set_Buf( myChar );	      
   //Frasa.To.insert_str( Frasa.To.PX );
   Frasa.To.insert_str( myChar );
   short px1=Frasa.To.end_str();
   if( 70<px1 )
   {   //for( short i=70 ; 0<i ; i-- )
       //  if( Frasa.To.Str[i]==' ' )
       //  {  Frasa.To.key_WK(); break ; }
       Frasa.To.key_WK();
   }
   Frasa.To.loop( 0, 0, S_key_End );
   Frasa.To.cursor_right( );
}
/**************************************************************************/
//                вставить слово без перевода
/**************************************************************************/
void t_FrasaFace :: variant2( short i_word )
{
   t_ItemList  &From =Core.from( );
   char        *Str  =From[i_word].Str ;
   t_myChar     myChar[256] ;

   koi9_to_myChar( Str, myChar, 255, 255 );
   Frasa.To.set_Buf( myChar );	      
   //Frasa.To.insert_str( Frasa.To.PX );	      
   Frasa.To.insert_str( myChar );
   short px1=Frasa.To.end_str();
   if( 70<px1 )
   {   //for( short i=70 ; 0<i ; i-- )
       //  if( Frasa.To.Str[i]==' ' )
       //  {  Frasa.To.key_WK(); break ; }
       Frasa.To.key_WK();
   }
   Frasa.To.loop( 0, 0, S_key_End );
   Frasa.To.cursor_right( );
}
/*****************************************************************************/
void resize_vindow1( void *Void )
{   Frasa.resize_vindow( Void );
}
/*****************************************************************************/
void t_FrasaFace :: resize_vindow( void *Void )
{  char   *Com=(char *)Void ;
   t_Rect *R ;

   if( Com[0]=='1' ) R=&RFrom ;
   if( Com[0]=='2' ) R=&RFirst ;
   if( Com[0]=='3' ) R=&RSlowo ;
   if( Com[1]=='+' ) R->y2++ ;
   if( Com[1]=='-' ) R->y2-- ;
   
   RFirst.y1=RFrom.y2 ;
   RSlowo.y1=RFirst.y2 ;
   RTo.y1   =RSlowo.y2+1 ;

   set_pos( );
   paint();
}
/**************************************************************************/
void t_FrasaFace :: to_dict()
{
   long   i,i1 ;
   t_View V ;
   
   ViewSelect.clr();
   for( i=0 ; i<Kontext.Type.j ; i++ )
   {  t_wType *T=&Kontext.Type[i] ;
      for( i1=T->i_exemplar ; i1<T->i_last_exemplar ; i1++ )
      {  t_wExemplar *E=&Kontext.Exemplar[Kontext.i_Exemplar[i1]] ;
	 if( E->i_absatz==i_Absatz && E->i_frasa==i_Frasa && E->i_word1==First.get_py() )
         {   V.index=i ;
	     strcpy( V.Str,T->Str );
	     ViewSelect.add( V ) ;
	 }
      }
   }
   ViewSelect.calc_pos();
   i=ViewSelect.loop();
   if( 0<=i )
   {   SetPerevod.init( ViewSelect.ViewList[i].index );
       SetPerevod.main_loop();
   }
   set_pos( );
}
/**************************************************************************/
static int   funk0( const void *a, const void *b )
{  t_WordVariant *A=(t_WordVariant *) a ;
   t_WordVariant *B=(t_WordVariant *) b ;

   return A->i_last_word - B->i_last_word ;
}
/************************************************************************/
//
/************************************************************************/
t_VariantView :: t_VariantView()
{
    begin=current=0 ;
    WordVariant.init();
}
/************************************************************************/
t_VariantView :: ~t_VariantView()
{
   WordVariant.del();
}
/**************************************************************************/
//
/**************************************************************************/
void t_VariantView :: paint()
{  short  i ;
   char   f ;

   s_rame2_F( y1-1,x1-1,y2+1,x2+1, 0x0f );
   s_text_yxf( y1-1, x1+3, 0x0b, "Варианты перевода слова" );
   for( i=0 ; i<y2-y1 ; i++ )
   {
      if( begin+i==current ) f=cur_item_Color ; else f=0x07 ;
      s_text_yxf( y1+i, x1, f, emptystr(x2-x1+1) );
      if( WordVariant.j<=begin+i ) continue ;
      s_text_yxf( y1+i, x1, f, WordVariant[i].Antwort );
   }
   paint_hotkey( n_Line-1, 0, hk_Frasa1.c_str() );
}
/**************************************************************************/
//
/**************************************************************************/
e_WinMsg t_VariantView :: loop()
{
   ushort    ZZ,key1,key2 ; // нажатая клавиша
   short     L ;
   e_WinMsg  r ;
   e_sEvent  Event ;
   t_WordVariant *V ;

   while( 1 )
   {
      paint( );
      Event=s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();
      
      if( MOUSE_PRESS==Event )
      {   short button,x,y ;
          button=s_get_mouse( &x, &y );
	  if( 1==button )
	  {   if( y==y1-1 || y==y2+1 )
        	  loop_rect( y );
              if( x<x1-1 || x2+1<x || y<y1-1 || y2+1<y )
		  return WM_ESC ;
              if( y1<y && y<y2 )
	      {   current=begin+y-y1 ;
		  if( WordVariant.j<=current )
	              current=WordVariant.j-1 ;
              }
	  }
	  if( 3==button ) key1='\r' ;
          if( 4==button ) key2=S_key_Up ;
          if( 5==button ) key2=S_key_Down ;
      }
      
      switch( key1 )
      {
         case '\r' :
	 {    // ----- переход в визард параметров -----------
	      V=&WordVariant[current] ;
	      FormWisard.i_struct =V->i_struct ;
	      FormWisard.i_word   =V->i_word ;
	      FormWisard.i_variant=V->i_variant ;
	      r=FormWisard.main_loop();
	      return r ;
	 }
      }
      switch( key2 )
      {
	 case S_key_Up  :
	      if( 0<current ) current-- ;
	      if( current<begin ) begin=current ;
	      break ;
         case S_key_Down:
	      L=y2-y1 ;
	      if( current<WordVariant.j-1 ) current++ ;
	      if( begin+L<current )         begin=current-L ;
	      break ;
	 case S_key_Ins : /* вставить слово во фразу */
	      // ----- переход в визард параметров -----------
	      V=&WordVariant[current] ;
	      FormWisard.i_struct =V->i_struct ;
	      FormWisard.i_word   =V->i_word ;
	      FormWisard.i_variant=V->i_variant ;
	      r=FormWisard.main_loop();
	      return r ;
	 case S_key_F1 : Window_help( (void *)"m_frasa1.html") ; continue ;
	 case S_key_F10:
	      return WM_ESC ;
      }
   }
}
/**************************************************************************/
//     сделать массив слов (для одного i_word) для вставки в гнездо
/**************************************************************************/
void t_VariantView :: make_variant_list( short i_word )
{
   long  i_struct,i1 ;
   t_Variants *VV ;
   t_rStruct  *V1 ;
   t_WordVariant R ;
   t_Antwort     Antwort ;

   if( Kontext.Interes.j<=0 )
       Kontext.make_interes_struct();
   // ------- составить список перевдов слова ------------
   WordVariant.j=0 ;
   for( i_struct=0 ; i_struct<Grammar.n_trans() ; i_struct++ )
   {  if( Kontext.Interes[i_struct]!=0 ) continue ;
      VV=Core.variants( i_word,i_struct ) ;
      for( i1=0 ; i1<VV->Variant.j ; i1++ )
      {
         V1=&VV->Variant[i1] ;
	 if( 4<V1->i_last_word-i_word ) continue ;
	 R.type       =VV->Variant[i1].type ;
	 R.i_struct   =i_struct ;
	 R.i_variant  =i1 ;
	 R.i_word     =V1->i_word ;
	 R.i_last_word=V1->i_last_word ;
	 R.i_dic      =V1->i_dic ;
	 R.i_slowo    =V1->i_slowo ;
	 R.i_slowo1   =V1->i_slowo1 ;
	 Antwort.make( V1->i_word, i_struct, i1, VV->Variant[i1].Form );
	 strncpy( R.Antwort, Antwort.str( ), 80 );
	 WordVariant.add(R);
      }
   }
   // ------- отсортировать его --------------------------
   qsort( WordVariant.list, WordVariant.j, sizeof(t_WordVariant), funk0 );
   // ------- а еще его можно поджать :-) ----------------
}
/************************************************************************/
t_FormWisard :: t_FormWisard()
{
}
/************************************************************************/
t_FormWisard :: ~t_FormWisard()
{
}
/************************************************************************/
void t_FormWisard :: paint()
{  short i,z ;
   char  f ;

   s_rame2_F( y1-1,x1-1,y2+1,x2+1, 0x0f );
   s_rame1_F( y1  ,x1-1,y1+2,x2+1, 0x0f );
   s_rame2_f( y1-1,x1-1,y2+1,x2+1, 0x0f );
   s_text_yxf( y1-1, x1+3, 0x0b, "Задать грамматическую форму" );
   s_text_yxf( y1  , x1+3, 0x0a, "Параметры" );
   s_text_yxf( y1+2, x1+3, 0x0a, "Выражение в этой форме" );

   s_text_yxf( y1, x2-30, 0x07, Grammar[i_struct].To.Name );

   // ------- нарисовать параметры -----------------------------
   t_Param1List &Param1 = Grammar[i_struct].To.Param ;
   char s[50] ;
   for( i=z=0 ; i<Param1.j ; i++ )
   {  s[0]=0 ;
      if( Param1[i].Dir==1 ) strcpy( s,"&" );
      strcat( s,Param1[i].Name ) ;
      if( i==i_param ) f=cur_item_Color ; else f=0x07 ;
      s_text_yxf( y1+1, x1+z, f, s );
      z+=strlen( s )+1;
   }
   
   // ------- нарисовать значения и получившееся выражение -----
   t_Param &Param=Grammar.to().Param[ Param1[i_param].param ];
   t_Form  Form1=Form ;
   for( i=0 ; i<Param.Value.j ; i++ )
   {  char       Str[80] ;
      t_Antwort  Antwort ;

      if( i==i_value ) f=0x70 ; else f=0x07 ;
      s_text_yxf( y1+3+i, x1, f, emptystr(x2-x1+1) );
      sprintf( Str, "%2d %-12s ",i,Param.Value[i].Name );
      Str[16]=0 ;
      s_text_yxf( y1+3+i, x1, f, Str );
      Form1.value[i_param]=i ;
      Antwort.make( i_word, i_struct, i_variant, Form1 );
      s_text_yxf( y1+3+i, x1+17, f, Antwort.str() );
   }
   paint_hotkey( n_Line-1, 0, hk_Frasa2.c_str() );
}
/************************************************************************/
e_WinMsg t_FormWisard :: main_loop()
{
   ushort   ZZ,key1,key2 ; // нажатая клавиша
   e_sEvent Event ;
   short i ;

   t_Param1List &Param1 = Grammar[i_struct].To.Param ;
   i_param=-1 ;
   for( i=0 ; i<Param1.j ; i++ )
      if( Param1[i].Dir==0 ) 
      {   i_param=i ; break ;  }
   if( i_param<0 ) {  func_ok( ); return WM_OK ;  }

   i_value=0 ;
   while( 1 )
   {
      t_Param &Param=Grammar.to().Param[ Param1[i_param].param ];

      paint( );
      Event=s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short button,x,y ;

          button=s_get_mouse( &x, &y );
	  if( 1==button )
	  {   if( y==y1-1 || y==y2+1 )
        	  loop_rect( y );
              if( x<x1-1 || x2+1<x || y<y1-1 || y2+1<y )
		  return WM_ESC ;
              if( y1+2<y && y<y2 )
	      {   i_value=y-y1-3 ;
		  if( Param.Value.j<=i_value )
	              i_value=Param.Value.j-1 ;
              }
          }
	  if( 3==button ) key2=S_key_Right ;
          if( 4==button ) key2=S_key_Up ;
          if( 5==button ) key2=S_key_Down ;
      }

      switch( key1 )
      {
         case '\r' : func_ok( ); return WM_OK ;
      }
      
      switch( key2 )
      {
	 case S_key_Up  :
	      if( 0<i_value ) i_value-- ;
	      Form.value[i_param]=i_value ;
	      break ;
         case S_key_Down:
	 {    //t_Param &Param=Grammar.to().Param[ Param1[i_param].param ];
	      if( i_value<Param.Value.j-1 ) i_value++ ;
	      Form.value[i_param]=i_value ;
	 }    break ;
         case S_key_Left: 
	      for( i=i_param-1 ; 0<=i ; i-- )
	         if( Param1[i].Dir==0 )
		 {   i_param=i ; break ;  }
	      i_value=Form.value[i_param] ;
	      break ;
         case S_key_Right:
	      for( i=i_param+1 ; i<Param1.j ; i++ )
	         if( Param1[i].Dir==0 )
		 {   i_param=i ; break ;  }
	      i_value=Form.value[i_param] ;
	      break ;
	 case S_key_F1 : Window_help( (void *)"m_frasa2.html") ; continue ;
         case S_key_F10:
	      return WM_ESC;
      }
   }
}
/**************************************************************************/
void t_FormWisard :: func_ok( )
{
    t_Antwort  Antwort ;
    t_myChar   myChar[256] ;

    Antwort.make( i_word, i_struct, i_variant, Form );
    char *Koi9=Antwort.str();
    koi9_to_myChar( Koi9, myChar, 255, 255 );
    Frasa.To.set_Buf( myChar );	      
    //Frasa.To.insert_str( Frasa.To.PX );	      
    Frasa.To.insert_str( myChar );
    short px1=Frasa.To.end_str();
    if( 70<px1 )
    {   //for( short i=70 ; 0<i ; i-- )
	//   if( Frasa.To.Str[i]==' ' )
	//   {  Frasa.To.key_WK(); break ; }
        Frasa.To.key_WK();
    }
    Frasa.To.loop( 0, 0, S_key_End );
    Frasa.To.cursor_right();
}

/**************************************************************************/
DEFINE_LIST_BTYPE( t_WordVariant, t_WordVariantList )
