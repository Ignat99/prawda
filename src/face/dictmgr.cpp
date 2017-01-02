/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                           менеджер словаря
//     Анисимов Д.В.                               сегодня
/*******************************************************************/
/*
    Дальнейшее движение (близко к идеалу, если бы оно еще и работало)
    1) Горячие клавиши
    2) Мыша
*/
# include "string.h"
# include "list.h"
# include "mylib.h"
# include "video.h"
# include "common.h"
# include "menu.h"
# include "dir1.h"
# include "window.h"
# include "slowo3.h"
# include "kontext.h"
# include "option.h"
# include "macrostruct.h"
# include "dictmgr.h"
# include "windows.h"

void  manager_loop( void *V );
void  write_context_slowar( void *Void );
void  inverse_selection1( void *Void );
void  inverse_selection2( void *Void );

// ----------- меню для режима  ------------------------------------
t_Menu Manager_menu1[] =
{
   { F_FUNK  ,0,manager_loop ,(void *)"3", "Выделить известные слова    "},
   { F_FUNK  ,0,manager_loop ,(void *)"1", "Удалить выделенные слова    "},
   { F_FUNK  ,0,manager_loop ,(void *)"2", "Погасить выделение          "},
   { F_FUNK  ,0,inverse_selection1 ,NULL , "Инвертировать выделение     "},
   { F_FUNK  ,0,manager_loop ,(void *)"4", "Записать словарь в файл     "},
   { F_NUMER ,0,NULL         ,(void *)"x", "Выход" },
   { F_ENDE  ,0,NULL         ,NULL,"" }
};
t_Menu Manager_menu2[] =
{
   { F_FUNK  ,0,manager_loop ,(void *)"b", "Прочитать из файла         "},
   { F_FUNK  ,0,manager_loop ,(void *)"7", "Угадать перевод            "},
   { F_FUNK  ,0,manager_loop ,(void *)"a", "Новые слова                "},
   { F_FUNK  ,0,manager_loop ,(void *)"9", "Новые варианты перевода    "},
   { F_FUNK  ,0,manager_loop ,(void *)"8", "Погасить выделение         "},
   { F_FUNK  ,0,inverse_selection2 ,NULL , "Инвертировать выделение    "},
   { F_FUNK  ,0,manager_loop ,(void *)"d", "Вставить в контекст        "},
   { F_ENDE  ,0,NULL         ,NULL,"" }
};
t_Menu Manager_help[] =
{  { F_FUNK  ,0,Window_help ,(void *)"index.html"    ,"Содержание    " },
   { F_FUNK  ,0,Window_help ,(void *)"k_dictmgr.html","Об этом режиме"},
   { F_ENDE  ,0,NULL        ,NULL,"" }
};
t_Menu Manager_menu[] =
{  { F_MENU  ,0,NULL,Manager_menu1,"Контекстный_Словарь "},
   { F_MENU  ,0,NULL,Manager_menu2,"Внешний_Словарь "},
   { F_MENU  ,0,NULL,Manager_help ,"Подсказка "},
   { F_ENDE  ,0,NULL,NULL         ,"" }
};

extern t_Slowo3      Perevod1 ;
extern t_Slowo3      Perevod2 ;
       t_Slowo3      Perevod3(0) ; // внешний словарь
extern t_Kontext     Kontext ;
extern t_Kontext2    Kontext2 ;
extern t_Macrostruct Macrostruct ;
extern t_Dir         Dir ;
extern t_HotKey2     KeyKontext[] ;
extern t_HotKey2     KeyDictMgr1[] ;
extern t_HotKey2     KeyDictMgr2[] ;

t_DictVisard  DictVisard ;

void read_dict( );

/***********************************************************************/
void  dict_mgr( void *Void )
{  short sx1,sy1 ;
   t_Rect R=Kontext2.get_pos();

   s_get_size( sx1,sy1 );
   Kontext.make_current_perevod();
   DictVisard.f_Half=0 ; 
   DictVisard.set_pos( 0, 1, sx1-1, sy1-2 );
   Kontext2.f_2Rame=1 ;
   Kontext2.set_pos( DictVisard.rect_Current );   
   DictVisard.loop();
   Kontext2.set_pos( R );
}
/***********************************************************************/
t_DictVisard :: t_DictVisard()
{
   Extern.init();

   Cursor=0 ; 
   Begin =0 ;
   BeginX=0 ;
   f_Half=0 ;
   DictFile="" ;
}
/***********************************************************************/
void t_DictVisard :: set_pos( short x1, short y1, short x2, short y2 )
{
   t_Win::set_pos( x1, y1, x2, y2 );
   short xx=(x1+x2)/2 ;

   rect_Current.y1=y1 ;
   rect_Extern.y1=y1+1 ;
   rect_Current.y2=rect_Extern.y2=y2 ;
   rect_Current.x1=x1 ;
   rect_Extern .x2=x2 ;
   rect_Current.x2=xx ;
   rect_Extern .x1=xx+1 ;
}
/***********************************************************************/
void t_DictVisard :: paint(  )
{
   t_Rect Rect=rect_Extern ;
   short  sy=Rect.y2-Rect.y1-1 ;
   short  sx=Rect.x2-Rect.x1-1 ;
   t_String Str ;

   Kontext2.f_2Rame=1-f_Half ;
   Kontext2.paint( Kontext2.Begin, Kontext2.PY );

   if( f_Half ) s_rame2_F( Rect.y1, Rect.x1, Rect.y2, Rect.x2, 0x0f );
       else     s_rame1_F( Rect.y1, Rect.x1, Rect.y2, Rect.x2, 0x0f );

   if( DictFile.length()<sx-5 )
   {   Str=DictFile.c_str() ;  }
   else
   {   Str="..." ;
       Str+=DictFile.c_str()+(DictFile.length()-sx+5);
   }   
   s_text_yxf( Rect.y1,Rect.x1+1,0x0b,Str.c_str() );

   char     f ;
   
   for( long i=0 ; i<sy ; i++ )
   {  short y    =Rect.y1+i+1 ;
      long  index=Begin+i ;
      if( index==Cursor ) f=cur_item_Color ; else f=text_Color ;
      s_text_yxf( y,Rect.x1+1,f,emptystr(sx) );
      if( Extern.j<=index ) continue ;
      if( Extern[index].select )
          f=(f & 0xf0) | (0x0f & sel_item_Color) ;
      Perevod3.print_record( Str,index );
      if( BeginX<Str.length() )  
          s_text_yxfl( y,Rect.x1+1,f,sx,Str.c_str()+BeginX );
   }

   if( 0==f_Half ) paint_hotkey( n_Line-1, 0, hk_Kontext.c_str() );
       else        paint_hotkey( n_Line-1, 0, hk_DictMgr );
   paint_header( "Менеджер словаря" );
   null_menu( Manager_menu,y1 );
}
/***********************************************************************/
e_WinMsg t_DictVisard :: loop( )
{
   short page_size=y2-y1-3 ;
   short       button,mouse_x,mouse_y,r1 ;
   e_Function  Func ;
   e_sEvent    Event ;
   e_WinMsg    Ret ;
   
   while( 1 )
   {  ushort key1,key2,ZZ,k ;
   
      paint();
      Event=s_getch16( &key1,&key2 );
      ZZ=s_shiftstatus();

      // ======= мышиные события ======================================
      if( MOUSE_PRESS==Event )
      {
          button=s_get_mouse( &mouse_x, &mouse_y );
          if( mouse_y==1 )
	  {   r1=menu_mouse( Manager_menu, y1,mouse_x ) ;
	      if( 'x'==r1 ) return WM_ESC ;
	      continue ;
	  }
	  if( mouse_y==n_Line-1 )
	  {
	      if( 0==f_Half ) Option.get_key( &ZZ, &key1, &key2, hk_Kontext.c_str(), mouse_x );
	          else        Option.get_key( &ZZ, &key1, &key2, hk_DictMgr        , mouse_x );
          }
	  if( button==1 )
	  {
	      if( rect_Current.inside( mouse_x,mouse_y ) )
	      {   f_Half=0 ;
		  if( y1+4<mouse_y && mouse_y<y2 )
	              Kontext2.PY=min(Kontext2.Begin+mouse_y-(y1+6),Kontext2.View.j-1) ;	      
	      }
	      if( rect_Extern.inside( mouse_x,mouse_y ) )
	      {   f_Half=1 ;
	          Cursor=Begin+mouse_y-rect_Extern.y1-1 ;
	      }
	  }
          if( button==3 ){  ZZ=0, key1='\r' ; key2=0  ;  }
          if( button==4 ){  ZZ=0, key1=0    ; key2=S_key_Up  ;  }
          if( button==5 ){  ZZ=0, key1=0    ; key2=S_key_Down;  }
      }
      // ======= клавиатурные события ==================================
      if( 30<key1 ) k=key1>>8 ;
          else      k=key1 ;

      if( f_Half ) Func=Option.get_func( KeyDictMgr2, ZZ,k,key2 );
	  else     Func=Option.get_func( KeyDictMgr1, ZZ,k,key2 );
	   
      switch( Func )
      {
         case F_HELP : Window_help( (void *)"k_dictmgr.html"); break ;
	 case F_DMGR1_MARK_UNKNOWN_WORD  : select_old_word( ); break ;
	 case F_DMGR2_MARK_NEW_WORD      : select_new_word( ); break ;
	 case F_DMGR1_UNMARK_MARKED_WORD : deselect_current( ); break ;
	 case F_DMGR2_UNMARK_MARKED_WORD : deselect_extern( ); break ;
	 case F_DMGR1_INVERT_MARKED_WORD : inverse_selection1( NULL ); break ;
	 case F_DMGR2_INVERT_MARKED_WORD : inverse_selection2( NULL ); break ;
	 case F_DMGR2_MARK_NEW_PEREVOD   : select_new_perevod( ); break ;
         case F_DMGR2_COPY_WORD          : add_selected_word(); break ;
	 case F_DMGR1_DEL_MARKED_WORD    : delete_selected( ); break ;
	 case F_DMGR2_FIND               : find(); break ;
         case F_MENU1 :
              r1=menu( Manager_menu, y1 ) ;
	      if( 'x'==r1 ) return WM_ESC ;
              break ;
      
         case F_END_OF_LIST:
	      if( key1==S_key_TabR ) {  f_Half=1-f_Half ; break ;  }
	      if( key2==S_key_F10 )  return WM_ESC ;
	      if( f_Half==0 )
	      {
		  if( key2==S_key_Ins )
		  {   long i=Kontext2.View[Kontext2.PY] ;
		      Kontext.Type[i].select=1-Kontext.Type[i].select ;
		      Kontext2.PY++ ;
		      continue ;
		  }
        	  Func=Option.get_func( KeyKontext, ZZ,k,key2 );
        	  Ret =Kontext2.loop( Func, ZZ, key1, key2 );
        	  if( WM_ESC==Ret ) return WM_ESC ;
	      }
	      else
	      {   switch( key2 )
		  {
		     case S_key_Up  : Cursor-- ; break ;
		     case S_key_Down: Cursor++ ; break ;
		     case S_key_PgUp: Cursor-=page_size ; break ;
		     case S_key_PgDn: Cursor+=page_size ; break ;
        	     case S_key_Right: BeginX++ ; break ;
        	     case S_key_Left: if( 0<BeginX ) BeginX-- ; break ;
		     case S_key_Ins :  // Выделить
			  Extern [Cursor].select=1-Extern [Cursor].select ;
			  Cursor++ ;
			  break ;
		  }
		  if( Cursor<0         ) Cursor=0 ;
		  if( Extern.j<=Cursor ) Cursor=max( 0,Extern.j-1 ) ;
		  if( Cursor<Begin     ) Begin =Cursor ;
		  if( Begin+page_size<Cursor ) Begin=Cursor-page_size ;
	      }
              break ;
      }
   }
}
/***********************************************************************/
void  manager_loop( void *V )
{  char *VV=(char *)V ;
   switch( VV[0] )
   {
      case '1' : DictVisard.delete_selected( ); break ;
      case '2' : DictVisard.deselect_current( ); break ;
      case '3' : DictVisard.select_old_word( ); break ;
      case '4' : write_context_slowar( NULL ); break ;
      case '7' : DictVisard.ugoday_perevod( ); break ;
      case '8' : DictVisard.deselect_extern( ); break ;
      case '9' : DictVisard.select_new_perevod( ); break ;
      case 'a' : DictVisard.select_new_word( ); break ;
      case 'b' : DictVisard.read_dict(); break ;
      case 'd' : DictVisard.add_selected_word(); break ;
   }
}
/***********************************************************************/
void t_DictVisard :: read_dict( )
{  char f ;
   long i,N ;
   t_String Buf ;
   char     StrSave[100];

   Dir.get_schabl( StrSave );
   Dir.set_schabl( "*.dic" );
   f=Dir.main_loop( Buf,0 );
   Dir.set_schabl( StrSave );
   if( 0>f ) return ;

   if( f==0 ) 
   {   Perevod3.read( Buf.c_str(), &(Grammar.format1()) );
       N=Perevod3.n_record() ;
       Extern.Calloc( N );
       for( i=0 ; i<N ; i++ )
	  Extern[i].i_kontext  =-1 ;
       DictVisard.DictFile=Buf ;
   }
}
/***********************************************************************/
//     сравнение двух структур
//     return
//     -1 +1 различаются типы структур 
//     -2 +2 различаются части речи, входящие в структуру
//     -3 +3 различаются слова [] входящие в структуру
//     -4 +4 различаются параметры слов
//     -5 +5 различается число слов
//     надо бы еще сравнивать параметры самой структуры
//     функция slowo3.cpp::funk0 делает то же самое
/***********************************************************************/
int  sStruct_cmp( t_sStruct *S1, t_sStruct *S2 )
{
   int    f,i1,is1,is2,iw,nw ;
   t_Form Form1,Form2 ;

   f=S1->i_struct-S2->i_struct ;
   if( f<0 ) return -1 ;
   if( 0<f ) return  1 ;
   
   nw=min( S1->n_Word,S2->n_Word ) ;
   for( iw=0 ; iw<nw ; iw++ )
   {
      is1=S1->Word[iw].i_struct ;
      is2=S2->Word[iw].i_struct ;
      if( is1<is2 ) return -2 ;
      if( is2<is1 ) return  2 ;
      
      f=Strcmp( S1->Word[iw].str,S2->Word[iw].str );
      if( f<0 ) return -3 ;
      if( 0<f ) return  3 ;

      Form1=S1->Word[iw].Param.form() ;
      Form2=S2->Word[iw].Param.form() ;
      for( i1=0 ; i1<10 ; i1++ )
      {  f=Form1.value[i1]-Form2.value[i1] ;
         if( f<0 ) return -4 ;
         if( 0<f ) return  4 ;
      }
      f=S1->n_Word-S2->n_Word ;
      if( f<0 ) return -5 ;
      if( 0<f ) return  5 ;
   }
   return 0 ;   
}
int  sStruct_cmp1( const void *S1, const void *S2 )
{
   return sStruct_cmp( (t_sStruct *)S1, (t_sStruct *)S2 );
}
/***********************************************************************/
//     сравнение двух словарных записей
//     Возвращает тоже что и sStruct_cmp
//     10 - существуют новые варианты перевода
/***********************************************************************/
int  sRecord_cmp( t_sRecord *R1, t_sRecord *R2 )
{  int f,i1,i2 ;

   f=sStruct_cmp( R1->Struct,R2->Struct );
   if( f!=0 ) return f ;
   for( i2=1 ; i2<R2->n_struct ; i2++ )
   {  for( i1=1 ; i1<R1->n_struct ; i1++ )
      {  if( 0==sStruct_cmp( &R1->Struct[i1], &R2->Struct[i2] ) )
	     goto M_Found ;
      }
      return 10 ;
      M_Found:;
   }
   return 0 ;
}
/***********************************************************************/
//     убрать выделение слева (в контекстном словаре)
/***********************************************************************/
void t_DictVisard :: deselect_current( )
{
   for( long i=0 ; i<Kontext.Type.j ; i++ )
      Kontext.Type[i].select=0 ;
}
/***********************************************************************/
//     убрать выделение справа (во внешнем словаре)
/***********************************************************************/
void t_DictVisard :: deselect_extern( )
{
   for( long i=0 ; i<Extern.j ; i++ )
      Extern[i].select=0 ;
}
/***********************************************************************/
//     удалить выделенные слова из контекстного словаря
//     Может перенести в t_Kontext-?
/***********************************************************************/
void t_DictVisard :: delete_selected( )
{  long i,i1,j ;

   for( i=j=0 ; i<Kontext.Type.j ; i++ )
   {  if( Kontext.Type[i].select==0 )
          Kontext.Type[j++]=Kontext.Type[i] ;
   }
   Kontext.Type.j=j ;

   Kontext.correct_i_type();
   Kontext2.make_sort( );
   Kontext2.make_filtred_list( );
}
/***********************************************************************/
//     добавить выделенные слова из внешнего словаря в контекстный
/***********************************************************************/
void t_DictVisard :: add_selected_word( )
{  short       Reg=0,r=0 ;
   t_String    STmp,STmp1 ;
   t_sRecord1  Record1 ;

   for( long i=0 ; i<Extern.j ; i++ )
   {  if( Extern[i].select==0 ) continue ;
      if( 0<=Extern[i].i_kontext )
      {   // ----- если такое слово уже есть в словаре -------------------
          if( 4==Reg ) continue ;
          t_wType   *T=&Kontext.Type[Extern[i].i_kontext] ;
	  Perevod3.print_record( STmp, i );
	  if( 0==Reg )          
              r=dialog_owerwrite( T->Str, STmp.c_str() );
	  if( 1==r ) 
	      continue ;
	  if( 2==r || 5==r ) 
	      strncpy( T->Str, STmp.c_str(), 100 );
	  if( 3==r || 6==r )
	  {   Record1.parse1( T->Str, &Grammar.format1(),1 );
	      print_new_perevod( STmp1, &Record1, i );
	      STmp=T->Str ;
	      STmp+=STmp1 ;
	      strncpy( T->Str, STmp.c_str(), 100 );
	  }	   
	  if( 4==r ) Reg=4 ;
	  if( 5==r ) Reg=5 ;
	  if( 6==r ) Reg=6 ;
      }
      else
      {   // ----- если нет, то просто добавить --------------------------
	  t_wType    T ;
	  t_sStruct *S=Perevod3.get_from(i);
	  Perevod3.print_record( STmp, i );

	  T.used      = 0 ;
	  T.i_struct  = S->i_struct ;
	  T.i_slowo   =-1 ;
	  T.i_exemplar=-1 ;
	  T.n_hole    = 0 ;
	  strncpy( T.Str, STmp.c_str(), 100 );
	  Kontext.Type.add(T) ;
      }
   }
   Kontext2.make_sort( );
   Kontext2.make_filtred_list( );
}
/***********************************************************************/
//     выделить слова, которых нет в контекстном словаре
/***********************************************************************/
void t_DictVisard :: select_new_word( )
{
   make_reflection( );
   for( long i=0 ; i<Extern.j ; i++ )
      if( Extern[i].i_kontext<0 )
          Extern[i].select=1 ;
}
/***********************************************************************/
//     выделить слова, которые имеют новые переводы
/***********************************************************************/
void t_DictVisard :: select_new_perevod( )
{  long        z,z1 ;
   t_sRecord  *Record ;
   t_sRecord1  Record1 ;

   make_reflection( );
   for( long i=0 ; i<Extern.j ; i++ )
   {  if( 0<=Extern[i].i_kontext )
      {   Record1.parse1( Kontext.Type[Extern[i].i_kontext].Str,&Grammar.format1(),1 );
          Record=Perevod3.record( i );
	  if( 0x01 & exist_new_perevod( &Record1, Record ) )
             Extern[i].select=1 ;
      }
      M_Next :;
   }
}
/***********************************************************************/
//    есть ли новые переводы
//    return 0x01 - если есть "лишние" верианты Record
//    return 0x02 - если есть "лишние" верианты Record1
/***********************************************************************/
short t_DictVisard :: exist_new_perevod( t_sRecord1 *Record1, t_sRecord *Record )
{  short f1=0,f2=0 ;
   long  z,z1 ;

   for( z=1 ; z<Record->n_struct ; z++ )
   {  for( z1=1 ; z1<Record1->Struct.j ; z1++ )
	 if( 0==sStruct_cmp( &Record->Struct[z], &Record1->Struct[z1] ) )
	     goto M_Found ;
      f1=1 ;
      goto M_Next ;
      M_Found :;
   }      
   M_Next :
   for( z1=1 ; z1<Record1->Struct.j ; z1++ )
   {  for( z=1 ; z<Record->n_struct ; z++ )  
	 if( 0==sStruct_cmp( &Record->Struct[z], &Record1->Struct[z1] ) )
	     goto M_Found1 ;
      f2=2 ;
      goto M_Next1 ;
      M_Found1 :;
   }      
   M_Next1 :
   return f1|f2 ;
}
/***********************************************************************/
//     сделать строчку, в которой будут варианты, которых еще нет в контекстном словаре
/***********************************************************************/
void t_DictVisard :: print_new_perevod( t_String &Str, t_sRecord1 *Record1, long i_record )
{  short f1=0,f2=0 ;
   long  z,z1 ;
   t_String Str1 ;

   t_sRecord *Record=Perevod3.record( i_record );
   for( z=1 ; z<Record->n_struct ; z++ )
   {  for( z1=1 ; z1<Record1->Struct.j ; z1++ )
	 if( 0==sStruct_cmp( &Record->Struct[z], &Record1->Struct[z1] ) )
	     goto M_Found ;
      Perevod3.print_struct( Str1, i_record, z );
      Str+=";" ;
      Str+=Str1 ;
      M_Found :;
   }
}
/***********************************************************************/
//     Сделать индексы соответствия внешнего и контекстного словаря
/***********************************************************************/
void t_DictVisard :: make_reflection( )
{  long   i,j,f,i2,i3,N2,N3 ;
   t_sStruct *S2,*S3 ;
try
{
   t_sRecord1 *Record=(t_sRecord1 *)Calloc( Kontext.Type.j,sizeof(t_sRecord1) );
   t_sStruct  *Struct=(t_sStruct  *)Calloc( Kontext.Type.j,sizeof(t_sStruct ) );
   
   for( i=j=0 ; i<Kontext.Type.j ; i++ )
   {  t_wType &T=Kontext.Type[i] ;
      if( NULL!=strstr( T.Str,"=") )
      {   Record[j].parse1( T.Str,&Grammar.format1(),1 );
          Struct[j]=Record[j].Struct[0] ;
	  Struct[j].i_relation=i ;
	  j++ ;
      }
   }
   qsort( Struct,j,sizeof(t_sStruct),sStruct_cmp1 );
      
   N2=j ;
   N3=Perevod3.n_record() ;

   for( i3=0 ; i3<N3 ; i3++ )
      Extern[i3].i_kontext=-1 ;

   i2=i3=0 ;
   while( i2<N2 && i3<N3 )
   {
      S2=&Struct[i2] ;
      S3=Perevod3.get_from( i3 );
      f=sStruct_cmp( S2, S3 );

      if( f<0 && i2<N2 ) i2++ ;
      if( 0<f && i3<N3 ) i3++ ;
      if( f==0 )
      {   Extern[i3].i_kontext=Struct[i2].i_relation ;
          if( i2<N2 ) i2++ ;
	  if( i3<N3 ) i3++ ;
      }
   }
   Free( Record );
   Free( Struct );
}
catch( int E )
{ ;
}
}
/***********************************************************************/
//     Отметить слова, которые уже есть в главном словаре
/***********************************************************************/
void t_DictVisard :: select_old_word( )
{  long   i,j,f,i2,i3,N2,N3 ;
   t_sStruct *S2,*S3 ;
try
{
   t_sRecord1 *Record=(t_sRecord1 *)Calloc( Kontext.Type.j,sizeof(t_sRecord1) );
   t_sStruct  *Struct=(t_sStruct  *)Calloc( Kontext.Type.j,sizeof(t_sStruct ) );
   
   for( i=j=0 ; i<Kontext.Type.j ; i++ )
   {  t_wType &T=Kontext.Type[i] ;
      if( NULL!=strstr( T.Str,"=") )
      {   Record[j].parse1( T.Str,&Grammar.format1(),1 );
          Struct[j]=Record[j].Struct[0] ;
	  Struct[j].i_relation=i ;
	  j++ ;
      }
   }
   qsort( Struct,j,sizeof(t_sStruct),sStruct_cmp1 );
      
   N2=j ;
   N3=Perevod1.n_record() ;

   i2=i3=0 ;
   while( i2<N2 && i3<N3 )
   {
      S2=&Struct[i2] ;
      S3=Perevod1.get_from( i3 );
      f=sStruct_cmp( S2, S3 );

      if( f<0 && i2<N2 ) i2++ ;
      if( 0<f && i3<N3 ) i3++ ;
      if( f==0 )
      {   Kontext.Type[Struct[i2].i_relation].select=1 ;
          if( i2<N2 ) i2++ ;
	  if( i3<N3 ) i3++ ;
      }
   }
   Free( Record );
   Free( Struct );
}
catch( int E )
{ ;
}
}
/***********************************************************************/
short t_DictVisard :: dialog_owerwrite( char *Old, char *New )
{  short  L1,L2,sx,size_x ;
   ushort key1,key2 ;
   t_Rect Rect ;

   L1=1+strlen( Old );
   L2=1+strlen( New );
   sx=x2-x1-10 ;
   size_x=min( sx,max(L1,L2) );
   size_x=max( 24,size_x );
   Rect.x1=(x1+x2-size_x)/2 ;
   Rect.x2=Rect.x1+size_x ;
   Rect.y1=(y1+y2-8)/2 ;
   Rect.y2=Rect.y1+8 ;
   
   s_save   ( Rect.y1, Rect.x1, Rect.y2, Rect.x2 );
   s_rame1_F( Rect.y1, Rect.x1, Rect.y2, Rect.x2, 0x70 );
   s_text_yxf( Rect.y1+1,Rect.x1+1,0x70,"Старая запись" );
   s_text_yxf( Rect.y1+2,Rect.x1+1,0x17,emptystr(size_x-1) );
   s_text_yxf( Rect.y1+2,Rect.x1+1,0x17,Old );
   s_text_yxf( Rect.y1+3,Rect.x1+1,0x70,"Новая запись" );
   s_text_yxf( Rect.y1+4,Rect.x1+1,0x17,emptystr(size_x-1) );
   s_text_yxf( Rect.y1+4,Rect.x1+1,0x17,New );

   s_text_yxf( Rect.y1+5,Rect.x1+1,0x70,"1 - пропустить  4 - все" );
   s_text_yxf( Rect.y1+6,Rect.x1+1,0x70,"2 - переписать  5 - все" );
   s_text_yxf( Rect.y1+7,Rect.x1+1,0x70,"3 - добавть     6 - все" );

   while( 1 )
   {   s_getch( &key1, &key2 );
       switch( key1 )
       {   case '1' :      s_restore(); return 1 ;
           case '2' :      s_restore(); return 2 ;
           case '3' :      s_restore(); return 3 ;
           case '4' :      s_restore(); return 4 ;
           case '5' :      s_restore(); return 5 ;
           case '6' :      s_restore(); return 6 ;
	   case S_key_Esc: s_restore(); return 4 ;
       }
   }   
}
/***********************************************************************/
//       угадать переводы с помощью внешнего словаря
//     не нравится, что цикл по всем фразам
//     не нравится, что цикл по всем экземплярам
//     не нравится, накопление j_Word
/***********************************************************************/
void t_DictVisard :: ugoday_perevod( void )
{
   char *Mass,*Mass1,*Mass2,*Mass3 ;
   long  L1,L2,L3 ;
   long  i,j,i4,ie,j_Word ;
   short i_struct,i_word,i_last_word,i_slowo,n_word ;
   char  f_VarRep ;
   char  present[10] ; // есть ли у нас конструкция такй длинны
   char  Str[1000],Str1[1000] ;

   t_HT_Sort *S ;
   t_String   STmp ;
   t_Slowo3   Perevod4(0);
   t_Kontext *K=&Kontext ;

   // ---- создать новый словарь ------
   L1=Perevod1.print_mass( &Mass1 );
   L2=Perevod2.print_mass( &Mass2 );
   L3=Perevod3.print_mass( &Mass3 );
   Mass=(char *)Malloc( L1+L2+L3+2,sizeof(char) );
   for( i=j=0 ; i<L1 ; i++ )
      Mass[j++]=Mass1[i] ;
   for( i=0 ; i<L2 ; i++ )
      Mass[j++]=Mass2[i] ;
   for( i=0 ; i<L3 ; i++ )
      Mass[j++]=Mass3[i] ;
   Mass[j]=0 ;
   Perevod4.make( Mass, &Grammar.format1() );
   
   // ======== сделать новый перевод =================================
   Perevod=&Perevod4 ;
   f_VarRep=Core.f_VarRep ;
   Core.f_VarRep=1 ;
   
   for( long i=j_Word=0 ; i<Macrostruct.Sort.j ; i++ )
   {
      sprintf( Str1,"Построено %d/%d",i,Macrostruct.Sort.j );
      message( Str1 );

      S=&Macrostruct.Sort[i] ;
      if( S->i_frasa<0 ) continue ;
      t_Frasa &Frasa=Macrostruct.Absatz[S->i_absatz].Frasa[S->i_frasa] ;
      Macrostruct.get_frasa ( S->i_absatz, S->i_frasa, Str );

      Core.translate( Str,Str1 );
      t_ItemList &From=Core.from();

      // -------- построение Exemplar -----------------------
      for( i_struct=0 ; i_struct<Core.fn_struct(); i_struct++ )
      {
         if( 1==K->Interes[i_struct] ) continue ;
         for( i_word=0 ; i_word<Core.fn_word(); i_word++ )
         {  t_Variants *VV=Core.variants( i_word, i_struct );
	    for( i4=0 ; i4<10 ; i4++ )
	       present[i4]=0 ;
	    for( i4=0 ; i4<VV->Variant.j ; i4++ )
	    {  t_rStruct *V=&VV->Variant[i4] ;
	       n_word=V->i_last_word-V->i_word ;
	       if( n_word<6 && present[n_word]==0 )
	       {   // Внести очередной вариант
		   i_slowo      =V->i_slowo ;
	           i_word       =j_Word+V->i_word ;
		   i_last_word  =j_Word+V->i_last_word ;
		   for( ie=0 ; ie<K->Exemplar.j ; ie++ ) // полный цикл по вариантам (нехорошо)
		   {  if( K->Exemplar[ie].i_word==i_word && K->Exemplar[ie].i_last_word==i_last_word )
		      {   t_wType *T=&Kontext.Type[K->Exemplar[ie].i_type] ;
		          if( NULL==strstr( T->Str,"=" ) && 0<=V->i_slowo )
		          {   Perevod->print_record( STmp,V->i_slowo );
			      strncpy( T->Str, STmp.c_str(), 100 );
			      T->select=1 ;
		          }
		      }
		   }
	           present[n_word]=1 ;
	       }
	    }
	 }
      }
      j_Word+=From.j ;
   }
   Core.f_VarRep=f_VarRep ;
   Perevod=&Perevod1 ; // надо сделать как было
   
   Free( Mass  );
   Free( Mass1 );
   Free( Mass2 );
   Free( Mass3 );
   Kontext2.make_sort( );
   Kontext2.make_filtred_list( );
}
/***********************************************************************/
void write_context_slowar( void *Void )
{  char     f ;
   t_String Buf ;
   char     StrSave[100];

   Dir.get_schabl( StrSave );
   Dir.set_schabl( "*.dic" );
   f=Dir.main_loop( Buf,1 );
   Dir.set_schabl( StrSave );
   if( 0>f ) return ;

   Kontext.make_current_perevod();
   Perevod2.print( Buf.c_str() );
}
/***********************************************************************/
void inverse_selection1( void *Void )
{
   for( long i=0 ; i<Kontext.Type.j ; i++ )
      Kontext.Type[i].select=!Kontext.Type[i].select ;
}
/***********************************************************************/
void inverse_selection2( void *Void )
{
   for( long i=0 ; i<DictVisard.Extern.j ; i++ )
      DictVisard.Extern[i].select=!DictVisard.Extern[i].select ;
}
/***********************************************************************/
void t_DictVisard :: find()
{  short  DialogPos ;
   long   size1_y=y2-y1-1 ;
   t_Find Find ;
   char   direct=0 ;
   ushort key1,key2 ;
   e_WinMsg r ;
   t_String Str ;

M_Begin:
   short size_dialog=7 ;
   if( (y2-y1)/2<Cursor-Begin )
             DialogPos = y1+  (y2-y1-size_dialog)/4 ;
	else DialogPos = y2-6-(y2-y1-size_dialog)/4 ;
 
   r=Find.loop( DialogPos );
   if( WM_ESC==r ) return ;
   if( Find.sfind[0]==0 ) return ;

   while( 1 )
   {  char sFind[256] ;
      myChar_to_koi9( Find.sfind, (uchar *)sFind, 256, 256 );

      if( 0==direct )
      {   for( long i=Cursor+1 ; i<Perevod3.n_record() ; i++ )
	  {  Perevod3.print_record( Str,i );
	     if( NULL!=strstr( Str.c_str(), sFind ) )
	     {   Cursor=i ; break ;  }
	  }
      }
      else
      {   for( long i=Cursor-1 ; 0<=i ; i-- )
	  {  Perevod3.print_record( Str,i );
	     if( NULL!=strstr( Str.c_str(), sFind ) )
	     {   Cursor=i ; break ;  }
	  }
      }   
      if( Cursor<Begin ) Begin=Cursor ;
      if( (size1_y-7)<=Cursor-Begin ) Begin=Cursor-(size1_y-7) ;

       paint( );
       if( (y2-y1)/2<Cursor-Begin )
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
/***********************************************************************/
DEFINE_LIST_BTYPE( t_dvWord,t_dvWordList )













