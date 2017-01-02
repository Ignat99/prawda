/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                  Простенький такой текстовый редактор
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# include <string.h>

# include "mylib.h"
# include "video.h"
# include "common.h"
# include "menu.h"
# include "option.h"
# include "edit.h"
# include "windows.h"

# define  size_Tab 8
# define  max_Length   1000000L
# define  max_Clip     100000L

# define T_FRASAEND  1
# define T_ABSATZEND 2
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

void      set_cursor( int Reg );
t_myChar *emptystr32( short n );
char      str_recht( t_myChar *str );

static t_Find     Find ;
static t_myChar  *Clip=NULL ;  // клипборд
static long       l_Clip=0 ;   // текущая длинна клипборда
static char       f_Clip=0 ;   // признак, что в буфере больше одной строки
extern t_Menu     Main_menu[] ;

/***************************************************************************/
//  
/***************************************************************************/
t_Edit :: t_Edit( void )
{
   f_2Rame=0 ;
   x1=1 ; x2=78 ;
   y1=2 ; y2=n_Line-2 ;
   size_y=y2-y1 ;
   size_x=x2-x1 ;
   Begin_X=0 ;
   PX=0 ;
   PY=0 ;
   otm1=0 ; otm2=-1 ;
   Otm1=0 ; Otm2=-1 ;
   str1=0 ; str2=-1 ;
   Str1=0 ; Str2=-1 ;
   fOvertype =0 ;
   Flag_b=0 ;

   if( Clip==NULL )
       Clip=(t_myChar *)Calloc( max_Clip,sizeof(t_myChar) );
   strcpy32( Str,emptystr32(255) );
}
/***************************************************************************/
e_WinMsg t_Edit :: main( void )
{
   ushort  ZZ,key1,key2 ;    /* нажатая клавиша */
   e_WinMsg r ;

   if( fview!=1 )
       null_menu( Main_menu ) ;

   while( 1 )
   {
      paint( );

      s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      r=loop( ZZ,key1,key2 );
      if( r!=WM_NULL1 ) return r ;
   }
}
/***************************************************************************/
//                    один шаг реакции на кнопки
/***************************************************************************/
e_WinMsg t_Edit :: loop( short ZZ, ushort key1, ushort key2 )
{
   E_Function Funk ;
   short r ;
   char  f_MarkText ;

   Funk=translate( ZZ,key1,key2 );

   f_MarkText = F_MLeft==Funk || F_MRight==Funk || F_MUp    ==Funk || F_MDown    ==Funk || 
                F_MHome==Funk || F_MEnd  ==Funk || F_MPageUp==Funk || F_MPageDown==Funk ;
   
   if( f_MarkText && Flag_b==0 )
   {  
       Flag_b=1 ; str2=str1=PY ; otm2=otm1=PX ;	  
   }

   switch( Funk )
   {
   case 0 :
	     switch( key1 )
	     {   case '\r': 
			    PX=key_WK( ) ; break ;
		 case S_key_Esc:
			    put_str( ) ;
			    r=menu( Main_menu ) ;
			    null_menu( Main_menu ) ;
			    if( r!=0 ) return int_to_msg( r );
			    break ;
                 case  0 :  break ;
		 default :
			    if( fview==1 ) break ;
			    if( fOvertype==0 ) insert_str1( PX,key1 ) ;
				else Str[PX]=(char)key1 ;
			    if(PX<255) PX++ ;
			    break ;
		 case S_key_Back:
			    if( PX>0 ) delet_str1( --PX ) ;
			    break ;
	     } break ;
   case F_Menu:	     put_str( ) ;
		     r=menu( Main_menu ) ;
		     null_menu( Main_menu ) ;
		     if( r!=0 ) return int_to_msg( r );
		     break ;

   /*------------ обработка СТРЕЛОК -------------*/
   case F_MLeft  :   if(PX>0  ) {  PX--; otm2=PX;  } break ;
   case F_MRight :   if(PX<255) {  PX++; otm2=PX;  } break ;
   case F_MUp    :   if( PY>0 ) PY-- ; goto M_key1 ;
   case F_MDown  :   if( PY<Text->n_string()-1 ) PY++ ;
          M_key1 :   str2=PY ; otm1=otm2=0 ;
		     break ;
   case F_MHome  :   PX=beg_str(); goto M_key2 ;
   case F_MEnd   :   PX=end_str() ;
	  M_key2 :   otm2=PX ;
		     break ;
   case F_MPageUp:   PY=max( PY-size_y,0 ) ; goto M_key3 ;
   case F_MPageDown: PY=min( PY+size_y,Text->n_string()-1 ) ;
	    M_key3 : str2=PY ; otm1=otm2=0 ;
		     break ;
   case F_Paste    : if( f_Clip )
		     {   Text->put_text1( PY, Clip,l_Clip );
		         get_str( );
		     }
		     else
		     {  insert_str( PX ) ; }
		     break ;
   case F_Cat      :
   case F_Copy     : if( Str1<Str2 )
		     {
			if( Funk==F_Cat )
			     l_Clip=Text->get_text1( Str1,Str2, Clip );
			else l_Clip=Text->get_text2( Str1,Str2, Clip );
			f_Clip=1 ;
			str2=str1 ;
			PY=min( Str1,Text->n_string()-1 );
			get_str( );
		     } 
		     else
		     {  if( Funk==F_Cat )
				 delet_str( Otm1,Otm2 ) ;
			    else delet_str( Otm1,Otm2,1 ) ;
			otm2=otm1 ; PX=Otm1 ;
			f_Clip=0 ;
		     } break ;
   case F_IncCopy :  if( Str1<Str2 )
		     {  
			l_Clip=Text->get_text2( Str1,Str2, Clip );
			str2=str1 ;			
		     }
		     else
		     {  
			for( short i=Otm1 ; i<=Otm2 ; i++ )
			   Clip[l_Clip++]=Str[i] ;
			Clip[l_Clip++]='\n' ;
			otm2=otm1 ;
		     } break ;

   case F_BeginText : PY=0 ; break ;
   case F_EndText   : PY=Text->n_string()-1 ; break ;
   case F_ShiftLeft : str_shift(-1) ; break ;
   case F_ShiftRight: str_shift( 1) ; break ;
   case F_Left      : if( PX>0   ) PX-- ; break ;
   case F_Right     : if( PX<255 ) PX++ ; break ;
   case F_Up        : if( PY>0   ) PY-- ; break ;
   case F_Down      : if( PY<Text->n_string()-1 ) PY++ ; break ;
   case F_ScrollUp  : if( Begin_Y>0   ) {  Begin_Y-- ; PY-- ;  } break ;
   case F_ScrollDown: if( Begin_Y<Text->n_string()-(y2-y1-1) )
                      {   Begin_Y++ ; PY++ ; }
		      break ;
   case F_Delete    : if( 0==char_exist( PX ) )
		          key_Delet( ) ;
		      else
			  delet_str1( PX ) ;
		      break ;
   case F_Insert    : fOvertype=!fOvertype ;
		      break ;
   case F_Home      : PX=beg_str() ; break ;
   case F_End       : PX=end_str(); break ;
   case F_PageUp    : PY=max( PY-size_y,0 ) ; break ;
   case F_PageDown  : PY=min( PY+size_y,Text->n_string()-1 ) ; break ;
   case F_GoToLine  : goto_str(); break ;
   case F_Replace3  : replace3(); break ;

   }  /* switch(key1) */

   if( f_MarkText==0 )
   {  //if( Flag_b==1 )
      {  Flag_b=0 ;
	 str2=str1 ; Str2=Str1-1 ;
	 otm2=otm1 ; Otm2=Otm1-1 ;
      }
   }

   if( f_MarkText )
   {  Otm1=min(otm1,otm2) ; Otm2=max(otm1,otm2) ;
      Str1=min(str1,str2) ; Str2=max(str1,str2) ;
   }

   raden_Y( PY ) ;
   raden_X( PX ) ;
   return WM_NULL1 ;
}
/***************************************************************************/
//           нарисовать рамки и прочие причиндалы окна
/***************************************************************************/
void t_Edit :: paint( void )
{  char s1[10] ;

   if( f_2Rame==1 ) s_rame2_F( y1,x1,y2,x2,0x0f );
       else         s_rame1_F( y1,x1,y2,x2,red_rame_Color );
   s_text_yxf( y1,x1+3,titl_Color,Name );

   paint_body( );

   sprintf( s1,"%4ld:%3d",PY,PX );
   s_text_yxf( n_Line-2,n_Col-10,0x70,s1 ) ;

   if( fOvertype==0 ) s_text_yxf( n_Line-2,1,0x70," <Вставка> " ) ;
       else           s_text_yxf( n_Line-2,1,0x70," <Замена > " ) ;

   // ------- вот тут этот курсор ---------
   uchar t[4]={ 0x9f,0xef,0xcf };
   short Flag=Str[PX].Flag ;
   short px=x1+PX-Begin_X+1 ;
   short py=y1+PY-Begin_Y+1 ;
   s_color_yxt( py,px,t,"0" );
   if( Flag==T_FRASAEND  ) 
       s_color_yxt( py,px,t,"1" );
   if( Flag==T_ABSATZEND ) 
       s_color_yxt( py,px,t,"2" );
   //s_goto_xy( y1+PY-Begin_Y,x1+PX-Begin_X );

   if( fview==1 )
            paint_hotkey( n_Line-1, 0, hk_Help );
       else paint_hotkey( n_Line-1, 0, hk_Edit ); 
}
/***************************************************************************/
//           нарисовать основную внутреннюю область (по новому)
//  _Otm1, _Otm2 - с какой строки по какую происходит выделение
/***************************************************************************/
void t_Edit :: paint_body( )
{   short i,i1,xx1,Flag ;
    char  f ;
    uchar t[2]={0x0a,0xa0 };
    t_myChar *Str3 ;

    Text->set_reper( Begin_Y );
    if( fview==0 )
    {  
       for( i=0 ; i<size_y-1 ; i++ )
       {   if( Str1<=Begin_Y+i && Begin_Y+i<=Str2 && Str1<Str2 )
		    f=buf_Color ;
	       else f=text_Color ;
	   if( Begin_Y+i==PY )
		    Str3=Str ;
	       else Str3=Text->get_str( Begin_Y+i,Buf );
	   s_text32_yxfl( y1+i+1,x1+1,f,x2-x1-1,Str3+Begin_X );           
	   // -------- текущая строка -----------------------
	   if( Begin_Y+i==PY )
	   {   short x11=x1+1+Otm1-Begin_X ;
	       short L=min( Otm2-Otm1,x2-x11-1 );
	       s_text32_yxfl( y1+i+1,x11,buf_Color,L,Str+Otm1 );
	   }
           // --------- границы фраз и абзацев --------------
           for( i1=Begin_X ; i1<Begin_X+x2-x1 ; i1++ )
	   {  Flag=Str3[i1].Flag ;
	      xx1=i1-Begin_X ;
	      if( Flag==T_FRASAEND )
	          s_text32_yxfl( y1+i+1,x1+1+xx1,0x67,1,Str3+i1 );
	      if( Flag==T_ABSATZEND )
	          s_text32_yxfl( y1+i+1,x1+1+xx1,0x47,1,Str3+i1 );
 	      if( Flag==T_THISFRASA )
	          s_text32_yxfl( y1+i+1,x1+1+xx1,0x0e,1,Str3+i1 );
           }
       }
    }
    else
    {  short i1,xx1,z,begin,end ;

       for( i=0 ; i<size_y-1 ; i++ )
       {  Str3=Text->get_str( Begin_Y+i,Buf );
	  s_text32_yxfl( y1+i+1,x1+1,help_Color,x2-x1-1,Str3+Begin_X );
          for( i1=0 ; i1<256 ; i1++ )
	  {  if( Str3[i1]==0x200 ) {  z=i1+1 ; f=link_help_Color ; Str3[i1]=' '; }
	     if( Str3[i1]==0x300 ) {  z=i1+1 ; f=bold_help_Color ; Str3[i1]=' '; }
	     if( Str3[i1]==0x100 )
	     {   Str3[i1]=' ';
	         if( z<Begin_X+x2-x1 && Begin_X<i1 )
	         {   begin=max( z-1 , Begin_X );
		     end  =min( i1+1, Begin_X+x2-x1 );
		     xx1=begin-Begin_X ;
		     s_text32_yxfl( y1+i+1,x1+1+xx1,f,end-begin,Str3+begin );
		 }	      
	     }	      
	  }
       }
    }
    Text->reset_reper();
}
/***************************************************************************/
//          сдвинуть строчки буфера вправо или влево
/***************************************************************************/
void t_Edit :: str_shift( char R )
{  long i,py ;
   t_myChar *S ;

   if( Str1<Begin_Y || Begin_Y+size_y<=Str2 ) return ;
   for( py=Str1 ; py<=Str2 ; py++ )
   {
      S=Text->get_str( py-Begin_Y,Buf );
      if( 0<R )
      {   for( i=255 ; 0<i ; i-- )
	     S[i]=S[i-1] ;
	  S[0]=' ';
      }
      else
      {   for( i=0 ; i<255 ; i++ )
	     S[i]=S[i+1] ;
	  S[255]=' ';
      }
      Text->put_str_istr( py,Buf );
   }
   get_str( );
}
/***************************************************************************/
//              удалить символ из текущей строки
/***************************************************************************/
void t_Edit :: delet_str1( short px )
{  short i ;
   for( i=px ; i<255 ; i++ )
       Str[i]=Str[i+1] ;
   Str[255]=' ';
}
/***************************************************************************/
//              вставить символ в текущую строку
/***************************************************************************/
void t_Edit :: insert_str1( short px, ushort key )
{  short i ;

   for( i=255 ; px<i ; i-- )
      Str[i]=Str[i-1] ;
   Str[px].Char=key ;
   Str[px].Flag=0 ;
}
/***************************************************************************/
//           вычислить первый ненулевой символ текущей строки
/***************************************************************************/
short t_Edit :: beg_str( void )
{  short i ;
   for( i=0 ; i<255 ; i++ )
      if( Str[i]!=' ') return i ;
   return 0 ;
}
/***************************************************************************/
//           вычислить последний ненулевой символ текущей строки
/***************************************************************************/
short t_Edit :: end_str( void )
{  short i ;
   for( i=255 ; 0<=i ; i-- )
      if( Str[i]!=' ' || Str[i].Flag!=0 ) return i+1 ;
   return 0 ;
}
/***************************************************************************/
//                  есть ли символы в текущей строке
/***************************************************************************/
short t_Edit :: char_exist( short px )
{  short i ;
   for( i=px ; i<256 ; i++ )
      if( Str[i]!=' '|| Str[i].Flag!=0 ) return 1 ;
   return 0 ;
}
/***************************************************************************/
e_WinMsg t_Edit :: int_to_msg( short i )
{
   switch( i )
   {  case '1' : return WM_EDIT1 ;
      case '2' : return WM_EDIT2 ;
      case '3' : return WM_FIRST ;
      case '4' : return WM_SRC ;
      case '5' : return WM_DST ;
      case '6' : return WM_DEBUG ;
      case 'f' : return WM_DEBUG1 ;
      case '7' : return WM_GRAMMAR ;
      case '8' : return WM_SLOWO ;
      case '9' : return WM_BASE ;
      case 'a' : return WM_BASE1 ;
      case 's' : return WM_SLOWO1 ;
      case 'e' : return WM_ERROR ;
      case 'x' : return WM_ESC ;
   }
   return WM_NULL1 ;
}
/***************************************************************************/
void t_Edit :: put_str( void )
{
   Text->put_str_istr( old_PY,Str );
}
/***************************************************************************/
void  t_Edit :: get_str( void )
{
   Text->get_str( PY,Str );
   old_PY = PY ;
}
/***************************************************************************/
//       между прочим, функция уничтожающая возмутительную ошибку
/***************************************************************************/
void t_Edit :: raden_y3(  )
{
   if( PY<Begin_Y ) Begin_Y=PY ;
   if( Begin_Y+(y2-y1)<PY+3 ) Begin_Y=PY+3-(y2-y1) ;
}
/***************************************************************************/
//       сдвигание окна при изменении PY
/***************************************************************************/
short t_Edit :: raden_Y( long _PY )
{
   if( _PY==old_PY ) return 0 ;
   put_str( );
   get_str( );
   if( _PY<Begin_Y ) Begin_Y=_PY ;
   if( Begin_Y+(y2-y1-2)<_PY ) Begin_Y=_PY-(y2-y1-2) ;
   
   return 1 ;
}
/***************************************************************************/
//       сдвигание окна при изменении px
/***************************************************************************/
short t_Edit :: raden_X( short px )
{
   if( px<Begin_X )
   {   Begin_X=px ;
       return 1 ;
   }
   if( Begin_X+size_x-2<px )
   {   Begin_X=max(0,px-size_x+2) ;
       return 1 ;
   }
   return 0 ;
}
/***************************************************************************/
//       преобразование текста при нажатии Enter
/***************************************************************************/
short t_Edit :: key_WK( void )
{  short ret_x ;

   ret_x=beg_str() ;
   if( PX<ret_x ) ret_x=PX ;
   if( size_y<=PY-Begin_Y )
       Begin_Y++ ;

   Str[255]=0 ;
   Text->insert_str( PY );
   strcpy32( Buf,emptystr32(ret_x) );
   strncpy32( Buf+ret_x,Str+PX,256-PX );
   Text->put_str_istr( PY+1, Buf );
   Str[PX]=0 ;
   Text->put_str_istr( PY, Str );
   PY++ ;
   get_str( );
   return ret_x ;
}
/***************************************************************************/
//       преобразование текста при нажатии Delete
/***************************************************************************/
void t_Edit :: key_Delet( void )
{  short i,px1 ;

   Text->get_str( PY+1,Buf );
   for( px1=0 ; px1<256 ; px1++  )
      if( Buf[px1]!=' ' ) break ;
   if( px1==256 ) px1=0 ;
   for( i=0 ; PX+i<256 ; i++  )
      Str[PX+i]=Buf[px1+i] ;
   Text->delete_str( PY+1 );
}
/***************************************************************************/
//       удаление (и помещение в буфер) выделенного куска строки
/***************************************************************************/
void t_Edit :: delet_str( short _Otm1, short _Otm2, char f )
{  short  i,j ;

   l_Clip=_Otm2-_Otm1 ;

   for( i=0 ; i<l_Clip ; i++ )
      Clip[i]=Str[_Otm1+i] ;

   if( f==0 )
   {  for( j=_Otm1,i=_Otm2 ; i<256 ; i++ )
	  Str[j++]=Str[i] ;
      for( ; j<256 ; j++ )
	  Str[j]=' ' ;
   }
   f_Clip=0 ;
}
/***************************************************************************/
//       вставка в строку значения буфера
/***************************************************************************/
void t_Edit :: insert_str( short px )
{  short i,L ;

   L=(short)l_Clip ;
   for( i=255 ; px+L<=i ; i-- )
      Str[i]=Str[i-L] ;
   for( i=0 ; i<L ; i++ )
      Str[px+i]=Clip[i] ;
}
/***************************************************************************/
//       вставка в строку значения буфера
/***************************************************************************/
void t_Edit :: insert_str( t_myChar *_Str )
{  short i,L ;

   L=strlen32( _Str );
   for( i=255 ; PX+L<=i ; i-- )
      Str[i]=Str[i-L] ;
   for( i=0 ; i<L && PX+i<256 ; i++ )
      Str[PX+i]=_Str[i] ;
}
/***************************************************************************/
void t_Edit :: cursor_right( )
{  if( PX<255 ) PX++ ;
}
/***************************************************************************/
void t_Edit :: screen_up()
{  if( 0<Begin_Y ) Begin_Y-- ; 
}
/***************************************************************************/
void t_Edit :: screen_down()
{  if( Begin_Y<Text->n_string() ) Begin_Y++ ;
}
/***************************************************************************/
void t_Edit :: screen_set_y( long _Begin_Y )
{  Begin_Y= _Begin_Y ;
   if( Begin_Y<0 ) Begin_Y=0 ;
   if( Text->n_string()<=Begin_Y ) Begin_Y=Text->n_string()-1 ;
}
/***************************************************************************/

/***************************************************************************/
void t_Edit :: goto_str( void )
{  short i ;
   char  Name1[40]="" ;

   s_save( 10,15,14,65 ) ;
   s_rame1_F( 10,15,14,65,menu_Color ) ;
   s_text_yxf( 11,20,menu_Color,"Введите номер строки" ) ;
   s_text_yxf( 12,20,curs_menu_Color,emptystr( 40 ) ) ;
   if( WM_OK!=kommand_yxf( 12,20,curs_menu_Color,Name1,40,0 ) ) return ;

   for( i=0 ; i<8 && Name1[i]!=0 ; i++ )
      if( Name1[i]<'0' || '9'<Name1[i] ) goto M_Err ;
   if( i==0 || 6<i ) goto M_Err ;
   PY=atoi( Name1 );
M_Err:
   s_restore( ) ;
}
/***************************************************************************/
//     вычислить положение диалога в зависимости от положения курсора
//     size_y - размер диалога
/***************************************************************************/
short t_Edit :: dialog_position( short size_y )
{
   if( (y2-y1)/2<PY-Begin_Y )
             return y1+(y2-y1-size_y)/4 ;
	else return y2-6-(y2-y1-size_y)/4 ;
}
/***************************************************************************/
char t_Edit :: find1( void )
{  t_myChar  *S1 ;

   S1=Find.sfind ;
   if( Otm1<Otm2 )
   {
       strncpy32( S1,Str+Otm1,Otm2-Otm1 );
       S1[Otm2-Otm1]=0 ;
       Otm2=Otm1-1 ;
   }
   else
       S1[0]=0 ;

   e_WinMsg r ;
   r=Find.loop( dialog_position( 7 ) );
   if( r!=WM_OK ) return 0 ;
   return find( 0,Find.sfind );
}
/***************************************************************************/
char t_Edit :: find( char Richt, char *_sFind )
{
   t_myChar  Stmp[256] ;
   
   koi9_to_myChar( _sFind, Stmp, 256, 256 );
   return find( Richt, Stmp );
}
/***************************************************************************/
//                          повторный поиск
//   return  1 - нашла 0 - не нашла
/***************************************************************************/
char t_Edit :: find( char Richt, t_myChar *sFind )
{  short     i,y,px,px1,py,size,L,n_Str ;
   t_Find   *f ;
   t_myChar *S ;
   t_myChar  strtmp[256] ;

   f=&Find ;
   size = strlen32( sFind ) ;
   if( size==0 ) return 0 ;
   n_Str=Text->n_string() ;

   if( f->fxlat==1 )
   {  for( i=0 ; i<size ; i++ )
	 strtmp[i] = myChar_toUp( sFind[i] ) ;
   }
   else strcpy32( strtmp,sFind );

   for( y=0 ; y<n_Str ; y++ )
   {  if( Richt==0 ) py=PY+y ;
	 else        py=PY-y ;
      if( py>=n_Str )
	 if( f->fcontinue!=0 ) py-=n_Str ; else break ;
      if( py<0 )
	 if( f->fcontinue!=0 ) py+=n_Str ; else break ;

      S = Text->get_str( py ) ;
      for( i=0 ; i<256 ; i++ )
	 if( S[i]==0 || S[i]=='\n' ) break ;
      L=i ;
      if( y==0 ) px=PX+1 ; else px=0 ;
      for( ; px+size<L ; px++ )
      {
	 if( f->fxlat==1 )
	 {  for( i=0 ; i<size ; i++ )
	       if( myChar_toUp( S[px+i] ) != strtmp[i] ) goto M1 ;
	 }
	 else
	 {  for( i=0 ; i<size ; i++ )
	       if( S[px+i] != strtmp[i] ) goto M1 ;
	 }
	 for( i=0,px1=0 ; i<px ; i++ )
	    if( S[i]=='\t' ) px1+=size_Tab-1 ;
	 PY=py ; PX=px+px1  ;
	 return 1 ;
      M1: ;
      }
   }
   figna("Строка не найдена");
   return 0 ;
}
/***************************************************************************/
//                        поиск-замена
/***************************************************************************/
void t_Edit :: replace( void )
{  t_Find *f ;
   char    r ;
   short   taste,L ;

   f=&Find ;
   Find.loop( dialog_position( 7 ) );
   if( f->sfind[0]==0 || f->sreplace[0]==0 ) return ;
   L=strlen32( f->sfind );

   while( 1 )
   {  
      r=find( f->fdirect,f->sfind );
      if( r==0 ) break ;
      raden_Y( PY );
      raden_X( PX );
      if( f->falles==0 )
      {
	 Otm1=PX ; Otm2=PX+L ;
	 paint( );
     M3: taste=fragen( "Менять ? (1-Да 0-Нет Esc-Выход)");
	 switch( taste )
	 {  case S_key_Esc: goto M1 ;
	    case   1: break ;
	    case   0: goto M2 ;
	    default : goto M3 ;
	 }
      }
      replace1( f->sfind, f->sreplace );
   M2: ;
   }
M1:
    Otm1=0 ; Otm2=-1 ;
}
/***************************************************************************/
//   заменить что-то в текущей строке и позиции f->sfind на f->sreplace
/***************************************************************************/
void t_Edit :: replace1( char *sFind, char *sReplace )
{  
   t_myChar sFind1[256], sReplace1[256] ;

   koi9_to_myChar ( sFind   , sFind1   , 256, 256 );
   koi9_to_myChar ( sReplace, sReplace1, 256, 256 );
   replace1( sFind1, sReplace1 );
}
/***************************************************************************/
//   заменить что-то в текущей строке и позиции f->sfind на f->sreplace
/***************************************************************************/
void t_Edit :: replace1( t_myChar *sFind, t_myChar *sReplace )
{  t_myChar  *s,Buf1[256] ;
   short      i,l1,l2,d ;
   t_Find    *f ;

   f=&Find ;
   s=Text->get_str( PY );
   for( i=0 ; i<256 ; i++ )
   {  if( s[i]==0 || s[i]=='\n' ) {  Buf1[i]=0 ; break ;  }
      Buf1[i]=s[i] ;
   }
   l1 = strlen32( sFind );
   l2 = strlen32( sReplace );
   d = l2-l1 ;
   if( d>0 )
      for( i=256-d-1 ; PX<=i ; i-- )
	 Buf1[i+d] = Buf1[i] ;
   for( i=0 ; i<l2 ; i++ )
      Buf1[PX+i] = sReplace[i] ;
   if( d<0 )
   {  for( i=PX+l2 ; i<256-l2 ; i++ )
	 Buf1[i] = Buf1[i-d] ;
      for( i=256-d-1 ; i<256 ; i++ )
	 Buf1[i] = ' ' ;
   }
   Text->put_str_istr( PY,Buf1 );
   get_str( );
}
/***************************************************************************/
//                           поиск-замена 
//   F3 0 - искать следующую
//   F4 1 - искать предыдущую
//   F5 2 - заменить и искать следующую
//   Надо приделать "заменить всех"
/***************************************************************************/
void t_Edit :: replace3( void )
{  t_Find  *f ;
   e_WinMsg r ;
   char     direct=0 ;
   ushort   key1,key2,L ;

M_Begin:
   f=&Find ;
   r=Find.loop( dialog_position( 8 ) );
   if( WM_ESC==r ) goto M1 ;
   goto M2 ;

   while( 1 )
   {
       paint( );
       Find.paint( dialog_position( 7 ),-5 );
       s_getch( &key1,&key2 );
       switch( key1 )
       {
           case 0 : 
	          switch( key2 )
	          {
		     case S_key_F3  : direct=0 ; goto M2 ;
     		     case S_key_F4  : direct=1 ; goto M2 ;
		     case S_key_F10 : goto M1 ;
		  }
		  break ;
           case S_key_Esc: goto M_Begin ;
	   case '1' : direct=1 ; goto M2 ;
	   case '2' : if( f->sreplace[0]!=0 )
	              {   replace1( f->sfind,f->sreplace ); 
		          direct=0 ; goto M2 ;  
		      }
		      break ;
	   case '0' :
               direct=0 ;
M2:            L=strlen32( f->sfind );
	       if( 0==find( direct,f->sfind ) ) 
	           break ;
	       raden_Y( PY );
	       raden_X( PX );
	       Otm1=PX ; Otm2=PX+L ;
               break ;
       }
   }
M1: 
    Otm1=0 ; Otm2=-1 ;

}
/***************************************************************************/
void t_Edit :: set_text( t_Text *_Text )
{
   PX=0 ; Begin_X=0 ;
   PY=0 ; Begin_Y=0 ;
   Text=_Text ;
   get_str( );
}
/***************************************************************************/
void t_Edit :: init( char *_Text )
{
   Text->init( _Text );
   PX=0 ; Begin_X=0 ;
   PY=0 ; Begin_Y=0 ;
   get_str( );
}
/***************************************************************************/
void t_Edit :: init32( t_myChar *_Text )
{
   Text->init32( _Text );
   PX=0 ; Begin_X=0 ;
   PY=0 ; Begin_Y=0 ;
   get_str( );
}
/***************************************************************************/
void t_Edit :: write( char *_Name, e_Latin CodePage )
{
   Text->write_file( _Name, CodePage );
}
/***************************************************************************/
void t_Edit :: read( char *_Name, e_Latin CodePage )
{
   Text->read_file( _Name, CodePage );
   PX=0 ; PY=0 ;
   get_str( );
}
/***************************************************************************/
void t_Edit :: set_view( char f )
{  fview=f ;  }
/***************************************************************************/
//          выдать функцию редактора по комбинации нажатых клавиш
/***************************************************************************/
E_Function t_Edit :: translate( short shift, ushort key1, ushort key2 )
{
   if( shift==0 )
   {
       switch( key1 )
       {  case 0:
	       switch( key2 )
	       {
		  case S_key_Ins  : return F_Insert ;
		  case S_key_Del  : return F_Delete ;
		  case S_key_Home : return F_Home ;
		  case S_key_End  : return F_End ;
		  case S_key_PgUp : return F_PageUp ;
		  case S_key_PgDn : return F_PageDown ;
		  case S_key_Left : return F_Left ;
		  case S_key_Right: return F_Right ;
		  case S_key_Up   : return F_Up ;
		  case S_key_Down : return F_Down ;
		  
		  case S_key_F3   : return F_Replace3 ;		  
		  case S_key_F10  : return F_Menu ;
	       }
	  break ;
       }
   }
   if( (shift&(S_Shift_L|S_Shift_R))!=0 )
   {
       switch( key1 )
       {  case 0:
	       switch( key2 )
	       {
		  case S_key_Ins  : return F_Paste ;
		  case S_key_Del  : return F_Cat ;
		  case S_key_Home : return F_MHome ;
		  case S_key_End  : return F_MEnd ;
		  case S_key_PgUp : return F_MPageUp ;
		  case S_key_PgDn : return F_MPageDown ;
		  case S_key_Left : return F_MLeft ;
		  case S_key_Right: return F_MRight ;
		  case S_key_Up   : return F_MUp ;
		  case S_key_Down : return F_MDown ;
	       }
	  break ;
       }
   }
   if( (shift&(S_Ctrl_L|S_Ctrl_R))!=0 )
   {
       switch( key1 )
       {  case 0:
	       switch( key2 )
	       {
		  case S_key_Left : return F_PrevFrasa ;
		  case S_key_Right: return F_NextFrasa ;
		  case S_key_Up   : return F_ScrollUp ;
		  case S_key_Down : return F_ScrollDown ;
                  case S_key_Ins  : return F_Copy ;
               }       
          break ;
       }
   }
   return F_Nop ;
}
/***************************************************************************/
t_Find :: t_Find( void )
{  short i ;
//   char *x=xlat ;

   for( i=0 ; i<256 ; i++ )
   {  sfind[i]=0 ; sreplace[i]=0 ;  }
   for( i=0 ; i<2560 ; i++ )
      Mass[i]=0 ;
   for( i=0 ; i<10 ; i++ )
      prev_find[i]=Mass+i*256 ;

//   xlat=(char *)SortChar() ;

   fxlat     = 0 ;
   fcontinue = 0 ;
   fdirect   = 0 ;
   falles    = 0 ;
}
/***************************************************************************/
//                   установить флаги поиска и замены
/***************************************************************************/
e_WinMsg t_Find :: loop( short y0 )
{  t_Find Alt ;
   short  x0,punkt=0 ;
   ushort key1,key2 ;

   x0=(s_get_sx( )-56)/2 ;
   Alt=*this ;

while( 1 )
{
   x0=(s_get_sx( )-56)/2 ;
   paint( y0,punkt ) ;

   key1=WM_NULL1 ;
   if( punkt==0 ) key1=kommand32_yxf( y0+1,x0+12,curs_menu_Color,sfind  ,39,3,prev_find,10 ) ;
   if( punkt==1 ) key1=kommand32_yxf( y0+2,x0+12,curs_menu_Color,sreplace,39,1 ) ;

   switch( key1 )
   {  case WM_OK     : {  short i ;
			  for( i=9 ; 0<i ; i-- )
			     strcpy32( prev_find[i],prev_find[i-1] );
			  strcpy32( prev_find[0],sfind );
			  if( !str_recht(sfind) )
			  {  *this=Alt ; return WM_ESC ;  }
			  return WM_OK ;
		       }
      case WM_ESC  :  return WM_ESC ; //key2=0 ; break ;
      case WM_NEXT :  key2=S_key_Down ; key1=0 ; break ;
      case WM_PREV :  key2=S_key_Up   ; key1=0 ; break ;
      case WM_NULL1 : s_getch( &key1,&key2 ) ;  break ;
      case WM_HELP :  Window_help( (void *)"find.html") ; break ;
   }

   switch( key1 )
   {   case 0 :
	   switch( key2 )
	   {   case S_key_Up   : if(punkt<=0) punkt=2 ; else punkt-- ; break ;
	       case S_key_Down : if(punkt>=2) punkt=0 ; else punkt++ ; break ;
	       case S_key_F1   : Window_help( (void *)"find.html") ; break ;
	   } break ;
       case '\r' :
	   if( !str_recht(sfind) ) 
	   {  *this=Alt ; return WM_ESC ;  }
	   return WM_OK ;
       case S_key_Esc :
	   *this=Alt ;
	   return WM_ESC ;
       case ' ' :
	   switch( punkt )
	   {   case 2 : fxlat   = !fxlat ;   break ;
	       case 3 : falles  = !falles ;  break ;
	       case 4 : fdirect = !fdirect ; break ;
	   }
	   break ;
   }
}
}
/***************************************************************************/
//                нарисовать диалог поиска и замены
/***************************************************************************/
void t_Find :: paint( short y0, short punkt )
{  char f ;
   short x0,l1,l2 ;

   x0=(s_get_sx( )-56)/2 ;

   s_rame1_F( y0,x0,y0+5,x0+56,menu_Color ) ;

   s_text_yxf( y0  ,x0+20,menu_Color,"Поиск-замена" ) ;
   s_text_yxf( y0+1,x0+1 ,menu_Color,"Найти      :" ) ;
   s_text_yxf( y0+2,x0+1 ,menu_Color,"Заменить на:" ) ;
   
   s_text32_yxf1( y0+1,x0+12,menu_Color,sfind ) ;
   s_text32_yxf1( y0+2,x0+12,menu_Color,sreplace ) ;
   l1=strlen32(sfind); l2=strlen32(sreplace);
   s_text_yxf( y0+1,x0+14+l1,menu_Color,emptystr(37-l1) ) ;
   s_text_yxf( y0+2,x0+14+l2,menu_Color,emptystr(37-l2) ) ;

   if( punkt==2 ) f=curs_menu_Color ; else f=menu_Color ;
   s_text_yxf( y0+3,x0+1,f,"[ ] Различать большие и малые" ) ;
   if( fxlat==0 )  s_text_yxf( y0+3,x0+2,f,"X" ) ;

   if( punkt<0 )
      paint_hotkey( y0+4,x0+1, "[F3]вперед [F4]назад [F5]замена [F8]замена всех",x0+56 );
   else
      paint_hotkey( y0+4,x0+1, "[Enter] начать поиск-замену [Esc] выход",x0+56 );
}
/***************************************************************************/
char str_recht( char *str )
{  short i ;
   for( i=0 ; str[i]!=0 && i<80 ; i++ )
      if( str[i]!=' ' && str[i]!='\t' ) return 1 ;
   return 0 ;
}
/***************************************************************************/
char str_recht( t_myChar *str )
{  short i ;
   for( i=0 ; str[i]!=0 && i<80 ; i++ )
      if( str[i]!=' ' && str[i]!='\t' ) return 1 ;
   return 0 ;
}
/***************************************************************************/
void t_Edit :: find_forward( void )
{  e_WinMsg r ;

   r=Find.loop( dialog_position( 7 ) );
   if( r!=WM_OK ) return ;
   find( 0,Find.sfind );
}
/***************************************************************************/
void t_Edit :: find_backward( void )
{  e_WinMsg r ;

   Find.loop( dialog_position( 7 ) );
   if( r!=WM_OK ) return ;
   find( 1,Find.sfind );
}
/***************************************************************************/
t_Text * t_Edit :: text( void )
{
   return Text ;
}
/***************************************************************************/
t_myChar * t_Edit :: get_Buf()
{  Clip[l_Clip]=0 ;
   return Clip ;
}
/***************************************************************************/
void t_Edit :: set_Buf( t_myChar *Str )
{
   strncpy32( Clip,Str,256 );
   l_Clip=strlen32( Str );
}
/***************************************************************************/
t_myChar * t_Edit :: text_all( )
{  
   return Text->text_all() ; 
}
/***************************************************************************/
t_myChar *emptystr32( short n )
{
   static t_myChar Str[256] ;
   for( short i=0 ; i<n && i<255 ; i++ )
   {  Str[i].Char=' ' ;
      Str[i].Flag= 0  ;
   }
   Str[n].Char=0 ;
   Str[n].Flag=0 ;  
   return Str ;
}
