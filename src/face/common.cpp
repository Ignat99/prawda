/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                       удобные штучки для интерфейса
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <malloc.h>
# include "mylib.h"
# include "video.h"
# include "option.h"
# include "window.h"
# include "common.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

extern t_Option Option ;

/***************************************************************************/
//     установить курсор
/***************************************************************************/
void paint_cursor( short y, short x )
{
   uchar t[2]={ 0x9f,0 };
   s_color_yxt( y,x,t,"0" );
}
/***************************************************************************/
//                      Выдать сообщение или ошибку
//   flag 0-ошика 1-сообщение
/***************************************************************************/
short my_figna( char *str, char flag )
{  short    i,j,x,y,y_i,n_str,l_str,r1,sx,sy ;
   e_sEvent Event ;
   ushort   key1,key2 ;
   char     str1[80] ;

   j=0 ; l_str=0 ;
   for( n_str=1,i=0 ; i<1024 ; i++ )
   {   if( str[i]==0 )
       {  if( i-j>l_str ) l_str=i-j ;
	  break ;
       }
       if( str[i]=='\n' )
       {  if( i-j>l_str ) l_str=i-j ;
	  n_str++ ; j=i ;
       }
   }
   s_get_size( sx, sy );
   x=sx/2-l_str/2-1 ; y=sy/2-n_str/2-2 ;

   if( flag!=1 )
       s_save( y,x,y+n_str+1,x+l_str+2 ) ;

   s_rame1_F( y,x,y+n_str+1,x+l_str+2,0x70 ) ;
   for( y_i=0,r1=0 ; y_i<n_str ; y_i++ )
   {   for( i=r1,j=0 ; i<1024 ; i++ )
       {   if( str[i]==0 || str[i]=='\n' )
           {  str1[j]=0 ; r1=i+1 ; break ;  }
	   str1[j++]=str[i] ;
       }
       s_text_yxf( y+y_i+1,x+1,0x70,str1 ) ;
   }
   if( flag==0 )
   {
M2:   Event=s_getch( &key1,&key2 );
      if( MOUSE_RELEASE==Event ) goto M2 ;
      s_restore( ) ;
   }
   if( flag==1 )
   {  s_redraw();
      s_refresh();
   }
   if( flag==2 )
   {   short ret ;
    M1:s_getch( &key1,&key2 );
       switch( key1 )
       {  case '1': ret = 1 ; break ;
	  case '0': ret = 0 ; break ;
	  case S_key_Esc: ret = S_key_Esc ; break ;
	  default : goto M1 ;
       }
       s_restore( ) ;
       return ret ;
   }
   return 0 ;
}
/***************************************************************************/
void mouse_message( )
{  char *Str="Этот режим еще не <<мышефицирован>>\n"
             "Пользуйтесь меню и клавиатурой" ;
   figna( Str );
}
/***************************************************************************/
//                          Выдать сообщение
/***************************************************************************/
void message( char *str )
{  my_figna( str, 1 );  }
/***************************************************************************/
//                          Выдать сообщение
/***************************************************************************/
void figna( char *str )
{  my_figna( str, 0 );  }
/***************************************************************************/
//                   Задать вопрос и получить ответ
/***************************************************************************/
short fragen( char *str )
{  return  my_figna( str, 2 );  }
/***************************************************************************/
//                        Ввести строку (t_myChar)
//   x y - координаты на экране
//   f   - цвет
//   str - заполняемая строка
//   length - максимальная длинна строки
//   EIN    - можно ли ходить вверх-вниз (в соседние виджеты)
//   str1   - предыдущие введенные строки
//   n_str1 - число предыдущих строк
/***************************************************************************/
e_WinMsg kommand32_yxf( short y, short x, char f, t_myChar *str, short lenght, char EIN,
		   t_myChar **str1, short n_str1 )
{  short  i,p=0,ZZ,REG=1,L ;
   ushort key1,key2 ;
   t_myChar alt_str[80] ;

strcpy32( alt_str,str ) ;
L=strlen32( str ) ;

while( 1==1 )
{
   s_text32_yxf1( y,x,f,str ) ;
   s_text_yxf( y,x+L+2,f,emptystr(lenght-L-2) ) ;
   paint_cursor( y, x+p+1 );
   //s_goto_xy( y,x+p+1 );

   s_getch16( &key1,&key2 );
   ZZ=s_shiftstatus();
   if( key1==0 )
   {   switch(key2)
       {  case S_key_Left : if( ZZ&(S_Ctrl_L|S_Ctrl_R) ) return WM_PREV ;
                            if( p>0 ) p-- ; break ;
	  case S_key_Right: if( ZZ&(S_Ctrl_L|S_Ctrl_R) ) return WM_NEXT ;
	                    if( p<L && str[p]!=0 ) p++ ; break ;
	  case S_key_Up   : if( (EIN&0x01)==0 ) break ;
			    return WM_PREV ;
	  case S_key_Down : if( (EIN&0x01)==0 ) break ;
			    return WM_NEXT ;
	  case S_key_Del  :
		    if( str[p]==0 || p>=L ) break ;
		    for( i=p ; i<L && str[i]!=0 ; i++ )
		       str[i]=str[i+1] ;
		    str[L]=0 ; L-- ; break ;
	  case S_key_Ins :
		    if(REG==0) {  REG=1 ;  }
		       else    {  REG=0 ;  }
		    break ;
	  case S_key_Home: p=0 ; break ;
	  case S_key_End:
		   for( p=0 ; str[p]!=0 && p<lenght ; p++ ) ;
		   for(     ; str[p-1]==' ' ; p-- ) ;
		   break ;
	  case S_key_F1:
		       return WM_HELP ;
/*          case S_key_F2:
	           if( (EIN&0x02)!=0 )
		   {   short i ;
		       i=kommand_yxf1( y,x,str1,n_str1 );
		       if( 0<=i )
		       {  strcpy32( str,str1[i] );
			  L=strlen32( str );
		       }
		   }
		   break ;
*/
	  case S_key_F10:
		  strcpy32( str,alt_str ) ;
		  return WM_ESC ;
	 default: ;
       }
   }
   else
   {  switch( key1 )
      {  case  8: if(p>0)
    /* Zuruck  */ {  for( i=(--p) ; i<L && str[i]!=0 ; i++ )
		       str[i]=str[i+1];
		     L-- ;
		  }  break ;
	 case '\r': goto AUSGANG ;
	 case S_key_Esc:
		  strcpy32( str,alt_str ) ;
		  return WM_ESC ;
	 default: if( REG==1 )
		  {  if( L<lenght )
		     {   for( i=L ; i>=p ; i-- ) str[i+1]=str[i] ;
			 str[p]=key1 ; p++ ; L++ ;
		     }else
		     if( p<lenght )
		     {   for( i=lenght-2 ; i>=p ; i-- ) str[i+1]=str[i] ;
			 str[p]=key1 ; p++ ;
		     }
		  }
		  if( REG==0 && p<lenght )
		  {   str[p]=key1 ;
		      if( p>=L ){ str[p+1]=0 ; L=p+1 ; }
		      p++ ;
		  }
       }
   }
}
AUSGANG:
return WM_OK ;
}
/***************************************************************************/
//                          Ввести строку (koi9)
//   x y - координаты на экране
//   f   - цвет
//   str - заполняемая строка
//   length - максимальная длинна строки
//   EIN    - можно ли ходить вверх-вниз (в соседние виджеты)
/***************************************************************************/
e_WinMsg kommand_yxf( short y, short x, char f, char *str, short lenght, char EIN )
{  e_WinMsg Ret ;
   static t_myChar str1[100] ;

   koi9_to_myChar( str, str1, lenght, 100 );
   Ret=kommand32_yxf( y, x, f, str1, lenght, EIN );
   myChar_to_koi9( str1, (uchar *)str, lenght, 100 );

   return Ret ;
}
/***************************************************************************/
//             выбор строки из массива предыдущих строк
/***************************************************************************/
short kommand_yxf1( short y, short x, char **str, short n_str )
{  short  i,PY=0 ;
   ushort key1,key2 ;
   char   c ;

   s_save(    y,x,y+n_str+1,x+40 ) ;
   s_rame1_F( y,x,y+n_str+1,x+40,0x70 ) ;

   while( 1 )
   {
      for( i=0 ; i<n_str ; i++ )
      {   if( i==PY ) c=curs_menu_Color ;
	      else    c=     menu_Color ;
	  s_text_yxf( y+i+1,x+1,c,emptystr(38) ) ;
	  s_text_yxf( y+i+1,x+1,c,str[i] ) ;
      }
      s_getch( &key1,&key2 );
      if( key1==0 )
      {   switch( key2 )
	  {   case S_key_Up   :
		   PY-- ; if( PY<0  ) PY=n_str-1 ;
		   break ;
	      case S_key_Down :
		   PY++ ; if( n_str<PY ) PY=0 ;
		   break ;
	  }
      }
      else
      {   switch( key1 )
	  {
	     case '\r':      goto Ende ;
	     case S_key_Esc: PY=-1 ; goto Ende ;
	  }
      }
   }
   Ende:
   s_restore( );
   return PY ;
}
/************************************************************************/
//       напечатать список горячих клавиш (в формате "[F1]Help" )
/************************************************************************/
void paint_hotkey( short y, short x, const char *Str, short x2 )
{  short i,j,f,sx,sy ;
   uchar t[2] ={ 0xb0, 0x3f };
   static char Str1[200] ;
   static char Color[200] ;
   t[0]=hotkey1_Color ;
   t[1]=hotkey2_Color ;

   for( i=j=0,f='0' ; Str[i]!=0 && i<200 ; i++ )
   {  if( Str[i]=='[' ) {  f='1' ; continue ;  }
      if( Str[i]==']' ) {  f='0' ; continue ;  }
      Str1[j] =Str[i] ;
      Color[j]=f ;
      j++ ;
   }
   s_get_size( sx,sy );
   for( i=j ; i<sx && i<200 ; i++ )
   {  Str1[i]=' ' ; Color[i]='0' ; }
   Str1[i]=0 ; Color[i]=0 ;
   
   if( 0<=x2 )
   {  i=x2-x ;
      Str1[i]=0 ; Color[i]=0 ;
   }
   
   s_text_yx( y,x,Str1 );
   s_color_yxt( y,x,t,Color );
}
/************************************************************************/
//                         заголовок окна
/************************************************************************/
void paint_header( char *Str )
{
   short sx=s_get_sx(  );
   char *s =emptystr( sx );
   s_text_yxf( 0, 0, regim_Color, s );
   s_text_yxf( 0, (sx-strlen(Str))/2, regim_Color, Str );
}

/******************************************************************/
/******************************************************************/
//                         t_EditLine1
/******************************************************************/
/******************************************************************/
t_EditLine1 :: t_EditLine1()
{
   REG=1 ;
   BeginX=0 ;
   Length=0 ;
   Cursor=0 ;
   MaxLength=10000 ;
   Color=0x07 ;
   fMouseExit=0 ;
}
/******************************************************************/
void t_EditLine1 :: set_pos( short _x1, short _y, short _x2 )
{
   x1=_x1 ; y1=_y+1 ; x2=_x2 ;
}
/******************************************************************/
void t_EditLine1 :: paint()
{
   s_text_yxf   ( y1,x1+1,Color,emptystr(x2-x1-1) ) ;
   s_text32_yxf1( y1,x1+1,Color,Str+BeginX ) ;
   paint_cursor ( y1,x1+Cursor-BeginX+2 );
}
/******************************************************************/
e_WinMsg t_EditLine1 :: loop( short ZZ, ushort key1, ushort key2 )
{   short i ;

    switch(key2)
    {  case S_key_Left : 
            if( 0<Cursor ) Cursor-- ;
	    break ;
       case S_key_Right: 
	    if( Cursor<Length && Str[Cursor]!=0 ) Cursor++ ;
	    break ;
       case S_key_Del  :
	    if( Str[Cursor]==0 ) break ;
	    for( i=Cursor ; Str[i]!=0 ; i++ )
	       Str[i]=Str[i+1] ;
	    Str[Length]=0 ; Length-- ; 
	    break ;
       case S_key_Ins : REG=1-REG ; break ;
       case S_key_Home: Cursor=0 ; break ;
       case S_key_End:
	    for( Cursor=0 ; Str[Cursor]!=0 && Cursor<Length ; Cursor++ ) ;
	    for(     ; Str[Cursor-1]==' ' ; Cursor-- ) ;
	    break ;
      default: ;
   }
   switch( key1 )
   {  case 0: break ;
      case S_key_Back:
           if(Cursor==0) break ;
           for( i=(--Cursor) ; i<Length && Str[i]!=0 ; i++ )
	       Str[i]=Str[i+1];
	   Length-- ;
	   break ;
      case S_key_Enter : return WM_OK ;
      case S_key_Esc:
	   strcpy32( Str,alt_Str ) ;
	   return WM_ESC ;
      default: 
           if( REG==1 )
	   {  if( Length<MaxLength )
	      {   for( i=Length ; i>=Cursor ; i-- ) 
		     Str[i+1]=Str[i] ;
		  Str[Cursor]=key1 ; Cursor++ ; Length++ ;
	      }
	      else
	      if( Cursor<MaxLength )
	      {   for( i=MaxLength-2 ; Cursor<=i ; i-- ) 
		     Str[i+1]=Str[i] ;
		  Str[Cursor]=key1 ; Cursor++ ;
	      }
	   }
	   if( REG==0 && Cursor<Length )
	   {   Str[Cursor]=key1 ;
	       if( Length<=Cursor )
	       {  Str[Cursor+1]=0 ; Length=Cursor+1 ;  }
	       Cursor++ ;
	   }
    }
    if( Cursor<BeginX ) BeginX=Cursor ;
    if( (x2-x1-3)<Cursor-BeginX ) BeginX=Cursor-(x2-x1-3) ;
    return WM_NULL1 ;
}
/******************************************************************/
e_WinMsg t_EditLine1 :: main_loop( )
{
   e_WinMsg r ;
   ushort   ZZ,key1,key2 ;
   e_sEvent Event ;
   
   REG=1 ; Cursor=BeginX=0 ;
   Length=strlen32( Str ) ;
   strcpy32( alt_Str,Str ) ;

   while( 1 )
   {
       paint();
       Event=s_getch16( &key1,&key2 );
       ZZ=s_shiftstatus();
       if( MOUSE_PRESS==Event && fMouseExit )
       {   short x,y ;
           s_get_mouse( &x, &y );
	   if( x<x1 || x2<x || y!=y1 )
	       return WM_ESC ;
       }
       
       r=loop( ZZ, key1, key2 );
       if( r!=WM_NULL1 ) return r ;
   }
}
/******************************************************************/
void t_EditLine1 :: set_text( char *Str1 )
{
   static t_myChar Str2[10000] ;
   koi9_to_myChar( Str1, Str2, 10000, 10000 );
   set_text( Str2 );
}
/******************************************************************/
void t_EditLine1 :: set_text( t_myChar *str )
{
   strcpy32( Str,str ) ;
   Length=strlen32( Str );
   Cursor=BeginX=0 ;
}
/******************************************************************/
char * t_EditLine1 :: get_ctext( )
{
   static char Str2[10000] ;
   myChar_to_koi9( Str, (uchar *)Str2, 10000, 10000 );
   return Str2 ;
}
/******************************************************************/
void t_EditLine1 :: insert_text( char *Str1 )
{
   static t_myChar Str2[10000] ;
   koi9_to_myChar( Str1, Str2, 10000, 10000 );
   insert_text( Str2 );
}
/******************************************************************/
void t_EditLine1 :: insert_text( t_myChar *Str1 )
{  short i,Length1 ;

   Length1=strlen32( Str1 );
   for( i=Length ; i>=Cursor ; i-- ) 
      Str[i+Length1]=Str[i] ;
   strncpy32( Str+Cursor,Str1,Length1 );
   Cursor+=Length1 ; Length+=Length1 ;
}
/******************************************************************/
void t_EditLine1 :: insert_text( short z, t_myChar *Str1 )
{
   Cursor=z ;
   insert_text( Str1 );
}
/******************************************************************/
void t_EditLine1 :: insert_text( short z, char *Str1 )
{
   static t_myChar Str2[10000] ;
   koi9_to_myChar( Str1, Str2, 10000, 10000 );
   insert_text( z,Str2 );
}
/******************************************************************/
t_myChar * t_EditLine1 :: get_text(  )
{
   return Str ;
}
/******************************************************************/
void t_EditLine1 :: word_left(  )
{  short i ;

   for( i=Cursor-1 ; 0<i ; i-- )
      if( Str[i]==' ' ) break ;
   for( ; 0<=i ; i-- )
      if( Str[i]!=' ' ) 
      {   Cursor=i ; break ;  }
}
/******************************************************************/
void t_EditLine1 :: word_right(  )
{  short i ;

   for( i=Cursor+1 ; i<Length ; i++ )
      if( Str[i]==' ' ) break ;
   for( ; i<Length ; i++ )
      if( Str[i]!=' ' ) 
      {   Cursor=i ; break ;  }
}
/******************************************************************/
void t_EditLine1 :: fdelete( short z, short z1 )
{  short i ;

   if( z1<=z ) return ;
   for( i=z ; i<=Length ; i++ )
      Str[i]=Str[i+z1-z] ;
   Length-=z1-z ;
   Str[Length]=0;
}
/******************************************************************/
//
/******************************************************************/
t_ViewListSelect :: t_ViewListSelect()
{
   PY=Begin=Begin_X=size_y=0 ;
   ViewList.init() ;
}
/******************************************************************/
t_ViewListSelect :: ~t_ViewListSelect()
{
   ViewList.del() ;
}
/******************************************************************/
void t_ViewListSelect :: calc_pos()
{  short i,L ;
   short sx,sy ;
   
   s_get_size( sx,sy );

   for( L=i=0 ; i<ViewList.j ; i++ )
   {  L=max( L,strlen(ViewList[i].Str) );
   }
   if( L<sx )
   {   x1=sx/2-L/2 ;
       x2=x1+L ; 
   }
   y1=7 ; 
   y2=sy-7 ;
   size_y=y2-y1 ;
}
/******************************************************************/
void t_ViewListSelect :: paint( void )
{  short i ;
   char  f ;

   s_rame2_F( y1-1, x1-1, y2+1, x2+1, 0x0f );
   s_text_yxf( y1-1,x1+3 ,titl_Color, Name );
   for( i=0 ; i<=size_y ; i++ )
   {   if( Begin+i==PY ) f=0x70 ; else f=0x07 ;
       if( Begin+i<ViewList.j )
           s_text_yxf( y1+i,x1+1 ,f,ViewList[Begin+i].Str );
   }
}
/******************************************************************/
long t_ViewListSelect :: loop( short _PY )
{
   ushort ZZ,key1,key2 ;

   Begin=0 ;
   PY=_PY ;
   //calc_pos();
   
   while( 1 )
   {
      paint( );

      s_getch( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      switch( key1 )
      {
         case S_key_Esc : return -1 ;
         case S_key_Enter: return PY ;
      }
      switch( key2 )
      {
         case S_key_Up :
              if( PY>0 ) PY-- ;
              if( PY<Begin ) Begin=PY ;
              break ;
         case S_key_Down :
              if( PY<ViewList.j-1 ) PY++ ;
              if( size_y<PY-Begin ) Begin=PY-size_y ;
              break ;
         case S_key_Left : if( 0<Begin_X ) Begin_X-- ; break ;
         case S_key_Right: if( Begin_X<80 ) Begin_X++ ; break ;
         case S_key_Home : PY=0 ; break ;
         case S_key_End  : PY=ViewList.j-1 ; break ;
         case S_key_PgUp :
              PY=max( 0,PY-=size_y-1 );
              if( size_y<PY-Begin ) Begin=PY-size_y ;
              break ;
         case S_key_PgDn :
              PY=min( ViewList.j-1,PY+=size_y-1 );
              if( size_y<PY-Begin ) Begin=PY-size_y ;
              break ;
         case S_key_F10 : return -1 ;
      }
   }
}
/******************************************************************/
void t_ViewListSelect :: clr()
{  ViewList.j=0 ;
}
/******************************************************************/
void t_ViewListSelect :: add( t_View &V )
{  ViewList.add( V );
}

