/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                    "Человеческий" словарь (морда)
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# include <stdio.h>
# include <malloc.h>
# include <string.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "mylib.h"
# include "option.h"
# include "common.h"
# include "windows.h"
# include "video.h"
# include "slowo1.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

/*
Дальнейшее движение (Unicode)
2) Все внутренние строки - Unicode
3) Интерфейс функций - koi9+Unicode
4) Преобразование koi8->Unicode
5) Вызовы
*/

extern t_String   MercuryDir ;

/***************************************************************************/

/***************************************************************************/
t_Slowo1 :: t_Slowo1( void )
{  short i ;

   fView =0 ;
   Buchs =NULL ;
   i_Buch=0 ;
   n_Buch=0 ;
}
/***************************************************************************/
void t_Slowo1 :: set_pos( short _x1, short _y1, short _x2, short _y2, char flag )
{
   t_Win::set_pos( _x1,_y1,_x2,_y2 );
   fDirect=flag ;
   if( 0==flag )
   {  // -------- вертикальное расположение подокон ------
      rect1.x1=x1 ;
      rect1.x2=x2 ;
      rect1.y1=y1 ;
      rect1.y2=(y1+y2)/2 ;
      rect2.x1=x1 ;
      rect2.x2=x2 ;
      rect2.y1=rect1.y2 ;
      rect2.y2=y2 ;
   }
   else
   {  // -------- горизонтальное расположение подокон ------
      rect1.x1=x1 ;
      rect1.x2=x1+size_x/4 ;
      rect1.y1=y1 ;
      rect1.y2=y2 ;
      rect2.x1=rect1.x2 ;
      rect2.x2=x2 ;
      rect2.y1=y1 ;
      rect2.y2=y2 ;
   }
   short  yy=rect2.y1+(rect2.y2-rect2.y1)/3 ;
}
/***************************************************************************/
//               реакция на кнопки когда выбирается слово
/***************************************************************************/
e_WinMsg t_Slowo1 :: main_loop( void )
{
   long  i ;
   short size1_y ;
   t_Slowo *Buch=&Buchs[i_Buch];

   first_word=i_word ;
   size1_y   =rect1.y2-rect1.y1 ;

   while( 1 )
   {  ushort t1,t2,ZZ ;

      paint(  i_word, first_word );
      paint2( Buch->answer( i_word ), rect2, x, y );
      s_text_yxf( rect2.y2,rect2.x1+1,0x07,Name );
      s_redraw( );
      s_goto_xy( rect1.y2-1,rect1.x1+px );
      s_getch( &t1,&t2 );
      ZZ=s_shiftstatus();

      switch( t1 )
      {
      case S_key_Back: case S_key_Del:
		       if( 0<px ) Str[--px]=0 ; break ;
      case '\r':       continue ;
//      case '\r':       paint( i_word, first_word );
//		       antwort( i_word ); Str[px=0]=0 ; break ;
      case S_key_Esc : Str[px=0]=0 ;
                       break ;
      default:         if( 40<=px ) break ;
		       Str[px++]=t1 ; Str[px]=0 ;
		       i=Buch->find( Str );
		       if( i<0 ) Str[--px]=0 ;
			   else  i_word=i ;
		       break ;
      case 0 :
         if( (ZZ&(S_Shift_L|S_Shift_R))==0 )
	 {  switch( t2 )
	    {
	       case S_key_Up:    if( 0<i_word ) i_word-- ; break ;
	       case S_key_Down:  if( i_word<Buch->n_Word ) i_word++ ; break ;
	       case S_key_PgUp:  i_word-=size1_y-3 ; 
	                	 if( i_word<0 ) 
			             i_word=0 ; 
				 break ;
	       case S_key_PgDn:  i_word+=size1_y-3 ;
				 if( Buch->n_Word<=i_word )
				     i_word=Buch->n_Word-1 ;
				 break ;
               case S_key_F1 : 
		     Window_help( (void *)"slowo1.html") ;
		     break ;
	       case S_key_F10:   return WM_ESC ;
	    }
         }
	 if( (ZZ&(S_Shift_L|S_Shift_R))!=0 )
	 {
	    switch( t2 )
	    {  case S_key_PgUp : x=y=0 ; return WM_P_WORD ;
	       case S_key_PgDn : x=y=0 ; return WM_N_WORD ;
	       case S_key_Up:    y=max(     0,y-1); break ;
	       case S_key_Down:  y=min(size_y,y+1); break ;
	       case S_key_Left:  x=max(     0,x-1); break ;
	       case S_key_Right: x=min(size_x,x+1); break ;
	    }
	 }
	 break ;
      }
      if( i_word<first_word )          first_word=i_word ;
      if( first_word+size1_y-3<i_word) first_word=i_word-size1_y+3 ;      
   }
   return WM_ESC ;
}
/***************************************************************************/
void t_Slowo1 :: main_loop1_1( void )
{
    short size1_y=min( 16,rect1.y2-rect1.y1 ) ;
    
    i_word=Buchs[i_Buch].find( Str );
    if( i_word<first_word )          first_word=i_word ;
    if( first_word+size1_y-4<i_word) first_word=i_word-(size1_y-4) ;
//    paint( i_word, first_word );      
}
# ifdef NIE
/***************************************************************************/
//        реакция на кнопки когда смотрится перевод выбранного слова       //
/***************************************************************************/
void t_Slowo1 :: antwort( long i_word1 )
{
   short x=0,y=0 ;
   short t1,t2 ;

   while( 1 )
   {
      paint1( i_word1,x,y );
      s_redraw( );
      s_goto_xy( rect1.y2-1,rect1.x1+px+1 );
      s_getch( &t1,&t2 );
      switch( t1 )
      {
      case S_key_Esc : case S_key_Enter :
	 paint1( -1,x,y ); return ;
      case 0 :
	 switch( t2 )
	 {
	 case S_key_Up:    y=max(     0,y-1); break ;
	 case S_key_Down:  y=min(size_y,y+1); break ;
	 case S_key_Left:  x=max(     0,x-1); break ;
	 case S_key_Right: x=min(size_x,x+1); break ;
	 case S_key_F10:   paint1( -1,x,y ); return ;
	 }
      }
   }
}
# endif
/***************************************************************************/
//              отображение списка слов (главного словаря)
/***************************************************************************/
void t_Slowo1 :: paint( long i_word1, long first_word1 )
{  long   i,i1,L ;
   short  size1_y ;
   char  *Str1,f ;

   if( fDirect==0 )
   {
      t_Win::paint();
   }
   else
   {
      if( 1==f_2Rame )
      {   s_rame2_F ( y1,x1,y2,      x2 ,0x0f );
	  s_rame1_f ( y1,x1,y2,rect1.x2 ,0x0f );
	  s_rame2_f ( y1,x1,y2,      x2 ,0x0f );
      }
      else 
      {   s_rame1_F ( y1,x1,y2,      x2 ,0x0f );
          s_rame1_f ( y1,x1,y2,rect1.x2 ,0x0f );
          s_rame1_f ( y1,x1,y2,      x2 ,0x0f );
      }
      s_text_yxf( y1,x1+3 ,titl_Color, Name );
   }
//   if( 0<=first_word1 )
//       first_word=first_word1 ;

   size1_y=rect1.y2-rect1.y1 ;
   L=rect1.x2-rect1.x1-2 ;

//   if( first_word<(i_word1-size1_y-5) ) 
//       first_word=(i_word1-size1_y-5);

   for( i=0 ; i<size1_y ; i++ )
   {
       if( first_word+i==i_word1 )
                f=cur_item_Color ;
	   else f=    text_Color ;

       Str1=Buchs[i_Buch].wort( first_word+i );
       s_text_yxfl( rect1.y1+i+1,rect1.x1+1,f,L,emptystr(L) );
       s_text_yxfl( rect1.y1+i+1,rect1.x1+1,f,L,Str1 ); // Вот тут преобразование koi9 -> Unicode
   }

   if( 1==f_2Rame )
            s_rame2_F ( rect1.y2-2,rect1.x1,rect1.y2,rect1.x2,0x0f );
       else s_rame1_F ( rect1.y2-2,rect1.x1,rect1.y2,rect1.x2,0x0f );
   s_text_yxf( rect1.y2-1,rect1.x1+1,0x07,Str );
   paint_cursor( rect1.y2-1, rect1.x1+px+1 );
   paint_hotkey( n_Line-1, 0, hk_Slowo );
}
/***************************************************************************/
//            отобразить перевод выбранного слова (новая)
/***************************************************************************/
void t_Slowo1 :: paint2( char *Ans, t_Rect rect, short x, short y )
{
   short i,j,j1,s,L ;
   short size_x,size_y ;
   char  Str1[100] ;

   size_x=rect.x2-rect.x1 ;
   size_y=rect.y2-rect.y1 ;
   for( i=0 ; i<size_y ; i++ )
       s_text_yxf( rect.y1+i+1,rect.x1,0x0f,emptystr(size_x) );

   L=strlen( Ans );
   for( i=0,s=j=j1=0 ; i<L ; i++ )
   {  if( Ans[i]=='\n' || i==L-1 )
      {  Str1[j++]=0;
	 if( y<=s && s-y<size_y )
	     s_text_yxf( rect.y1+s-y,rect.x1,0x0f,Str1 );
	 j=j1=0 ; s++ ;
      }
      else
      {  if( (j1++)<x || size_x-3<j ) continue ;
	 Str1[j++]=Ans[i] ;
      }
   }
}
/***************************************************************************/
//                 прочесть словарь из файла _Name
/***************************************************************************/
/*
char t_Slowo1 :: read_buch( char *Name )
{  char r=0 ;
   if( 0!=strcmp( Buch.NameFile.c_str(), Name ) )
   {   r=Buch.read( Name );
       Buch.set_divider( "=" );
       Buch.set_sort( 0 ) ;
       i_word=0 ;
   }
   return r ;
}
/***************************************************************************/
//                 прочесть словари
/***************************************************************************/
void t_Slowo1 :: read_buchs(  )
{  long i,j ;

   for( i=0 ; i<n_Buch ; i++ )
      Buchs[i].del();
   Free(Buchs);

   Buchs=(t_Slowo *)Calloc( Grammar.File2[0].FileName.j, sizeof(t_Slowo) );

   for( i=j=0 ; i<Grammar.File2[0].FileName.j ; i++ )
   {
      Buchs[j].set_divider( "=" );
      char r=Buchs[j].read( Grammar.File2[0].FileName[i] );
      Buchs[j].set_sort( 0 ) ;
      if( r!=0 )
      {  t_String Str1="Не могу прочесть словарь \n" ;
         Str1+=Str ;
         figna( Str1.c_str() );
         continue ;
      }
      j++ ;
   }
   n_Buch=j ;
}
/***************************************************************************/
void t_Slowo1 :: select_buch( )
{  long    i,i1,z ;
   t_View  Item ;
   t_ViewListSelect Select ;

   for( i=0 ; i<n_Buch ; i++ )
   {  char *s=Buchs[i].NameFile.c_str();
      for( z=i1=0 ; s[i1]!=0 ; i1++ )
         if( s[i1]=='/'  ) z=i1+1 ;
      Strcpy( Item.Str,s+z,100 );
      Select.add( Item );
   }
   Select.calc_pos();
   Select.set_pos( Select.x1, y1+3, Select.x2, y2-2 );
   s_text_yxf( y1+1,Select.x1-1 ,regim_Color, emptystr( Select.x2-Select.x1+3) );
   s_text_yxf( y1+1,Select.x1 ,regim_Color, "Словари" );
   i=Select.loop( i_Buch );
   if( 0<=i )
       i_Buch=i ;
}
/***************************************************************************/
//                  установить переводимое слово
/***************************************************************************/
void t_Slowo1 :: set_str( char *_Str )
{  long i,j ;
   char c,*s ;

   Strcpy( Str,_Str,40 );
   // ------ исключение лишних символов ---------
   for( j=i=0 ; i<40 ; i++ )
   {  c=Str[i] ;
      if( c=='\"' || c=='`' || c=='\'' || c=='(' || c==')' 
       || c=='['  || c==']' || c=='{'  || c=='}' || c=='<' || c=='>'
       || c=='@'  || c=='#' || c=='$'  || c=='%' || c=='^' || c=='&'
       || c=='\\' || c=='/' || c=='*' ) continue ;
      Str[j++]=c ;
   }   
   px=strlen( Str );
   i_word=Buchs[i_Buch].find( Str );
}
/***************************************************************************/
//
/***************************************************************************/
void t_Slowo1 :: set_i_word( long _i_word )
{
   long size1_y=rect1.y2-rect1.y1 ;

   i_word=_i_word ;
   if( i_word<first_word )          first_word=i_word ;
   if( first_word+size1_y-4<i_word) first_word=i_word-(size1_y-4) ;
   strcpy( Str,Buchs[i_Buch].wort(i_word) );
   px=strlen(Str);
   if( Str[px-1]==' ' ) px-- ;
}
/***************************************************************************/
long t_Slowo1 :: get_i_word()
{  return i_word ;  }
/***************************************************************************/
long t_Slowo1 :: get_first_word()
{  return first_word ;  }
/***************************************************************************/
char * t_Slowo1 :: answer( )
{  return Buchs[i_Buch].answer( i_word );  }
/***************************************************************************/
long t_Slowo1 :: n_word( )
{  return Buchs[i_Buch].n_Word ;  }
/***************************************************************************/
void t_Slowo1 :: key_back()
{  if( 0<px ) Str[--px]=0 ;  }
void t_Slowo1 :: key_add( char key1 )
{  Str[px++]=key1 ; Str[px]=0 ;
}


