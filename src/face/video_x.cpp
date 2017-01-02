/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//     библиотека интерфейса с текстовым терминалом (X-Window)
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <malloc.h>
# include <string.h>
# include <stdlib.h>
# include <locale.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xos.h>
# include <X11/keysym.h>
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>

# include "mylib.h"
# include "koi9.h"
# include "video.h"
# include "slowo.xbm"

# define ACS_ULCORNER  0x10001
# define ACS_LLCORNER  0x10002
# define ACS_URCORNER  0x10003
# define ACS_LRCORNER  0x10004
# define ACS_HLINE     0x10005
# define ACS_VLINE     0x10006

# define ACS_ULCORNER2 0x10007
# define ACS_LLCORNER2 0x10008
# define ACS_URCORNER2 0x10009
# define ACS_LRCORNER2 0x1000a
# define ACS_HLINE2    0x1000b
# define ACS_VLINE2    0x1000c

# define DELTA          5  // величина "увеличения" окна

extern ushort ToUnicode[5][256] ;

long  *Symbol ;
uchar *Color ;
short  max_x,max_y ;          // чем они друг т друга отличаются?
short  size_x,size_y ;
int    Symbol_W,Symbol_H;     // Ширина и высота буковки
int    Shiftstatus ;          // Какие нажаты спецклавиши 
short  Mouse_x,Mouse_y ;      // Координаты мышки (в буковках)
short  MouseButton ;          // Нажатая кнопка мышки
char   f_mouse_move ;         // Отдавать ли события движения
char        *FontName ;
Display     *Dis ;
int          i_Scr ;
Window       Win ;
Colormap     Colormap1 ;
XColor       xColor[16] ;
XFontStruct *FontInfo ;
Pixmap       Pixmap1 ;
char         F_Rus=0 ;
static short T_ang_rus[256] ;
short        RussKomb=0 ;

// -------------------- графический контекст -------------------------------
class t_GC
{
public :
   Window Win ;
   GC     GC1 ;

	  t_GC( Window Win );
	  t_GC( Window Win, ulong Color1, ulong Color2 );
	 ~t_GC( void );
   void   SetForeground( ulong Color );
   void   SetBackground( ulong Color );
   void   DrawImageString( short x, short y, char *Text );
   void   DrawImageString16( short x, short y, unsigned short *Text );
};

class t_Save_Screen
{  long   *stek ;  // запоминание областей экрана
   long   l_stek ; // длинна массива Stek
   long   j_stek ; // указатель последнего свободного байта
public:
   t_Save_Screen( long L );
   ~t_Save_Screen( void );
   void   save   ( short y1, short x1, short y2, short x2 );
   void   save_rame( short y1, short x1, short y2, short x2 );
   void   restore( void );
} ;
                   // что это тут вторая секция переменных ?
short       Cur_X=0,Cur_Y=0 ;
short       n_Line,n_Col ;
char        Run=0 ; // признак того, что графика запущена
static char    TextFont[100]="" ;
unsigned short Text16[256] ; // буфер для преобразования koi9->Unicode
t_Save_Screen  Save( 60000 ) ;

void s_rame( short y1, short x1, short y2, short x2, uchar f );
void s_rame2( short y1, short x1, short y2, short x2, uchar f );
void s_quadro( short y1, short x1, short y2, short x2, uchar f );
void make_ang_rus( void );

/* ----------------------------------------------------------------------- */
void   s_begin_schone( int argc, char *argv[] )
{  short i,f=0,Size_X,Size_Y ;
   Window        Root ;

   setlocale( LC_ALL,"" );
   max_x=256 ; max_y=100 ;
   Symbol=(long  *)Calloc( max_x*max_y,sizeof(long) );
   Color =(uchar *)Calloc( max_x*max_y,1 );
   for( i=0 ; i<max_x*max_y ; i++ )
      Symbol[i]=' ';

   size_y=n_Line=50 ;
   size_x=n_Col =100 ;
   Shiftstatus  =0 ;
   make_ang_rus( );

   XSizeHints    SizeHints ;
   XWMHints      WMHints ;
   XClassHint    ClassHint ;
   XTextProperty WinName ;

   // --------------- открыть дисплей ---------------------------
   Dis   = XOpenDisplay(NULL) ;
   if( Dis == NULL ) { printf("Can not open Display"); exit(-1) ; }
   i_Scr = DefaultScreen(Dis) ;
   Root  = RootWindow( Dis,i_Scr) ;

   // -------------- каким шрифтом писать будем ----------------
   for( i=0 ; i<argc-1 ; i++ )
      if( 0==strcmp( argv[i], "-fn" ) )
      {  FontName=(char *)Calloc( strlen(argv[i+1])+1,sizeof(char) );
	 strcpy( FontName,argv[i+1] );
	 f=1 ;
      }
   if( f==0 && TextFont[0]!=0 )
   {   FontName=TextFont ; f=1 ;  }
   //FontName="-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso10646-1" ;

   if((FontInfo = XLoadQueryFont( Dis, FontName )) == NULL)
   {  printf( "\n Error load font:%s",FontName ); exit(-1);  }
   Symbol_W = FontInfo->max_bounds.width;
   Symbol_H = FontInfo->ascent + FontInfo->descent;
   Size_X=100*Symbol_W+5 ; Size_Y=50*Symbol_H+DELTA ;

   // --------------- открыть окошко --------------------------
   Win   = XCreateSimpleWindow( Dis,RootWindow( Dis,i_Scr),
	   0,0,Size_X,Size_Y,5, BlackPixel(Dis,i_Scr), BlackPixel(Dis,i_Scr) );

   // --------------- захватить цвета --------------------------
   Colormap1 = DefaultColormap( Dis,DefaultScreen(Dis) );
   for( i=0 ; i<16 ; i++ )
   {  XColor Color1 ;
      unsigned short V ;

      if( i>=8 ) V=0x8000 ; else V=0 ;
      if( (i&0x01)!=0 ) Color1.blue  = V+0x7FFF ; else Color1.blue  = 0 ;
      if( (i&0x02)!=0 ) Color1.green = V+0x7FFF ; else Color1.green = 0 ;
      if( (i&0x04)!=0 ) Color1.red   = V+0x7FFF ; else Color1.red   = 0 ;
      if( i==7 ) Color1.red=Color1.green=Color1.blue=0xBFFF ;
      if( i==8 ) Color1.red=Color1.green=Color1.blue=0xBFFF ;
      Color1.flags = DoRed|DoGreen|DoBlue ;
      XAllocColor( Dis,Colormap1,&Color1 );
      xColor[i] =Color1 ;
   }
   // ------------ сделать иконку чтоб красиво было --------------

   XWindowAttributes Attr ;
   XGetWindowAttributes( Dis,Root,&Attr );

   Pixmap Pix1 = XCreatePixmapFromBitmapData( Dis,RootWindow( Dis,i_Scr),
		 lingvo_bits,lingvo_width,lingvo_height,xColor[0x0e].pixel,xColor[0x0c].pixel,Attr.depth );

   char *sSlowo="Prawda" ;
   if( !XStringListToTextProperty( &sSlowo,1,&WinName ) ) exit(1);

   // -------------- назначить атрибуты окна -----------------------
   SizeHints.flags       = 0 ;

   WMHints.flags         = StateHint | IconPixmapHint | InputHint ;
   WMHints.initial_state = NormalState ;
   WMHints.input         = True ;
   WMHints.icon_pixmap   = Pix1 ;

   ClassHint.res_name    = argv[0] ;
   ClassHint.res_class   = "Mercury" ;

   XSetWMProperties ( Dis,Win, &WinName,&WinName, argv,argc,
		      &SizeHints, &WMHints, &ClassHint );

   XSelectInput( Dis,Win,ExposureMask | KeyPressMask | KeyReleaseMask
                    | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );
   XMapWindow( Dis,Win );

   Pixmap1=XCreatePixmap( Dis,Root,Attr.width,Attr.height,Attr.depth );

   Run=1 ;
   Mouse_x=Mouse_y=0 ;
   f_mouse_move=0 ;
   init_unicod();
}
/* ----------------------------------------------------------------------- */
void   s_begin_schone( void )
{  char *argv[1] ;
   argv[0]="mercury" ;
   s_begin_schone( 1,argv );
}
/* ----------------------------------------------------------------------- */
void   s_end_schone( void )
{
   XDestroyWindow( Dis,Win );
   XCloseDisplay( Dis );
   Run=0 ;
}
/* ----------------------------------------------------------------------- */
void   s_nacht( void )
{  long i ;

   for( i=0 ; i<max_x*max_y ; i++ )
   {  Symbol[i]=' ' ; Color[i]=0 ;  }
}
/* ----------------------------------------------------------------------- */
void   s_text32_yxf(  short y, short x, uchar f, t_myChar *str )
{  long i,r1 ;

   r1=max_x*y+x ;
   for( i=0 ; str[i]!=0 && str[i]!='\r' && str[i]!='\n' ; i++ )
   {  Symbol[ r1+i ]=str[i].Char ;
      Color [ r1+i ]=f ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_text_yxf(  short y, short x, uchar f, char *str )
{  long i,r1,L ;

   L=koi9_to_Unicod( str, Text16 );
   r1=max_x*y+x ;
   for( i=0 ; i<L ; i++ )
   {
      Symbol[ r1+i ]=Text16[i] ;
      Color [ r1+i ]=f ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_text32_yxf1( short y, short x, uchar f, t_myChar *str )
{  long i,r ;

   r = max_x*y+x ;
   Symbol[ r ] = '\"'<<8 ; Color[ r ] = f ;
   for( i=0 ; str[i].Char!=0 ; i++ )
   {  r=max_x*y+x+i+1 ;
      Symbol[ r ] = str[i].Char ; Color[ r ] = f ;
   }
   r=r+1 ;
   Symbol[ r ] = '\"'<<8 ; Color[ r ] = f ;
}
/* ----------------------------------------------------------------------- */
void   s_text_yxf1( short y, short x, uchar f, char *str )
{  long i,r,L ;

   L=koi9_to_Unicod( str, Text16 );
   r = max_x*y+x ;
   Symbol[ r ] = '\"'<<8 ; Color[ r ] = f ;
   for( i=0 ; i<L ; i++ )
   {  r=max_x*y+x+i+1 ;
      Symbol[ r ] = Text16[i] ; Color[ r ] = f ;
   }
   r=r+1 ;
   Symbol[ r ] = '\"'<<8 ; Color[ r ] = f ;
}
/* ----------------------------------------------------------------------- */
void   s_text32_yxfl( short y, short x, uchar f, short L, t_myChar *str ) 
{  long i,r1 ;

   r1=max_x*y+x ;
   for( i=0 ; str[i].Char!=0 && i<L ; i++ )
   {  Symbol[ r1+i ]=str[i].Char ;
      Color [ r1+i ]=f ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_text_yxfl( short y, short x, uchar f, short L, char *str )
{  long i,r1,L1 ;

   L1=koi9_to_Unicod( str, Text16, L, 256 );
   r1=max_x*y+x ;
   for( i=0 ; str[i]!=0 && i<L1 ; i++ )
   {  Symbol[ r1+i ]=Text16[i] ;
      Color [ r1+i ]=f ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_text_yx( short y, short x, char *str )
{  long i,r1,L ;

   L=koi9_to_Unicod( str, Text16 );
   r1=max_x*y+x ;
   for( i=0 ; i<L ; i++ )
      Symbol[ r1+i ]=Text16[i] ;
}
/* ----------------------------------------------------------------------- */
void   s_color_yxt( short y, short x, uchar *t, char *str )
{  long i,r1,r2,f ;

   r1=max_x*y+x ;
   for( i=0 ; str[i]!=0 ; i++ )
   {  f=0x07 ; r2=str[i] ;
      if( '0'<=r2 && r2<= '9' ) f=t[r2-'0'] ;
      if( 'a'<=r2 && r2<= 'z' ) f=t[r2-'a'+10] ;
      Color[ r1+i ]=f ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_foreground_yxt( short y, short x, uchar *t, char *str )
{  long i,r1,r2,f ;

   r1=max_x*y+x ;
   for( i=0 ; str[i]!=0 ; i++ )
   {  f=0x07 ; r2=str[i] ;
      if( '0'<=r2 && r2<= '9' ) f=t[r2-'0'] ;
      if( 'a'<=r2 && r2<= 'z' ) f=t[r2-'a'+10] ;
      Color[ r1+i ] = (0xf0 & Color[ r1+i ]) + (0x0f & f) ;
   }
}
/* ----------------------------------------------------------------------- */
void   s_rame1_f( short y1, short x1, short y2, short x2, uchar f )
{
   s_rame( y1, x1, y2, x2, f );
}
/* ----------------------------------------------------------------------- */
void   s_rame1_F( short y1, short x1, short y2, short x2, uchar f )
{
   s_quadro( y1, x1, y2, x2, f );
   s_rame  ( y1, x1, y2, x2, f );
}
/* ----------------------------------------------------------------------- */
void   s_rame2_f( short y1, short x1, short y2, short x2, uchar f )
{
   s_rame2 ( y1, x1, y2, x2, f );
}
/* ----------------------------------------------------------------------- */
void   s_rame2_F( short y1, short x1, short y2, short x2, uchar f )
{
   s_quadro( y1, x1, y2, x2, f );
   s_rame2 ( y1, x1, y2, x2, f );
}
/* ----------------------------------------------------------------------- */
void s_rame( short y1, short x1, short y2, short x2, uchar f )
{  long i,r ;

   for( i=x1+1 ; i<x2 ; i++ )
   {  r=max_x*y1+i ; Symbol[ r ]=ACS_HLINE ; Color[ r ]=f ;
      r=max_x*y2+i ; Symbol[ r ]=ACS_HLINE ; Color[ r ]=f ;
   }
   for( i=y1+1 ; i<y2 ; i++ )
   {  r=max_x*i+x1 ; Symbol[ r ]=ACS_VLINE ; Color[ r ]=f ;
      r=max_x*i+x2 ; Symbol[ r ]=ACS_VLINE ; Color[ r ]=f ;
   }

   r=max_x*y1+x1 ; Symbol[ r ]=ACS_ULCORNER ; Color[ r ]=f ;
   r=max_x*y2+x1 ; Symbol[ r ]=ACS_LLCORNER ; Color[ r ]=f ;
   r=max_x*y1+x2 ; Symbol[ r ]=ACS_URCORNER ; Color[ r ]=f ;
   r=max_x*y2+x2 ; Symbol[ r ]=ACS_LRCORNER ; Color[ r ]=f ;
}
/* ----------------------------------------------------------------------- */
void s_rame2( short y1, short x1, short y2, short x2, uchar f )
{  long i,r ;

   for( i=x1+1 ; i<x2 ; i++ )
   {  r=max_x*y1+i ; Symbol[ r ]=ACS_HLINE2 ; Color[ r ]=f ;
      r=max_x*y2+i ; Symbol[ r ]=ACS_HLINE2 ; Color[ r ]=f ;
   }
   for( i=y1+1 ; i<y2 ; i++ )
   {  r=max_x*i+x1 ; Symbol[ r ]=ACS_VLINE2 ; Color[ r ]=f ;
      r=max_x*i+x2 ; Symbol[ r ]=ACS_VLINE2 ; Color[ r ]=f ;
   }

   r=max_x*y1+x1 ; Symbol[ r ]=ACS_ULCORNER2 ; Color[ r ]=f ;
   r=max_x*y2+x1 ; Symbol[ r ]=ACS_LLCORNER2 ; Color[ r ]=f ;
   r=max_x*y1+x2 ; Symbol[ r ]=ACS_URCORNER2 ; Color[ r ]=f ;
   r=max_x*y2+x2 ; Symbol[ r ]=ACS_LRCORNER2 ; Color[ r ]=f ;
}
/* ----------------------------------------------------------------------- */
void s_quadro( short y1, short x1, short y2, short x2, uchar f )
{  long i,j,r1 ;

   for( j=y1+1 ; j<y2 ; j++ )
   {  for( i=x1+1 ; i<x2 ; i++ )
      {   r1=max_x*j+i ;
	  Symbol[ r1 ]=' ' ;
	  Color [ r1 ]=f ;
      }
   }
}
/* ----------------------------------------------------------------------- */
void s_save_rame( short s1, short p1, short s2, short p2 )
{  Save.save_rame( s1, p1, s2, p2 );  }

void s_save( short s1, short p1, short s2, short p2 )
{  Save.save( s1, p1, s2, p2 );  }

void s_restore( void )
{  Save.restore( );  }
/* ----------------------------------------------------------------------- */
t_Save_Screen :: t_Save_Screen( long L )
{
   stek=(long *)Calloc( L,sizeof(long) );
   j_stek=0 ;
   l_stek=L ;
}
/* ----------------------------------------------------------------------- */
t_Save_Screen :: ~t_Save_Screen( void )
{
   if( stek!=NULL ){  free( stek ); stek=NULL ;  }
   j_stek=0 ;
   l_stek=0 ;
}
/* ----------------------------------------------------------------------- */
void t_Save_Screen :: save( short y1, short x1, short y2, short x2 )
{  long i,j,r ;

   for( i=y1 ; i<=y2 ; i++ )
   {  for( j=x1 ; j<=x2 ; j++ )
      {   r=max_x*i+j ;
	  stek[j_stek++] = Color [ r ] ; // атрибут
	  stek[j_stek++] = Symbol[ r ] ; // символ
      }
   }
   stek[j_stek++]=(uchar )x1 ; stek[j_stek++]=(uchar )y1 ;
   stek[j_stek++]=(uchar )x2 ; stek[j_stek++]=(uchar )y2 ;
   stek[j_stek++]=0 ;
}
/* ----------------------------------------------------------------------- */
void t_Save_Screen :: save_rame( short s1, short p1, short s2, short p2 )
{  long i,r,r1,r2 ;

   r1=max_x*s1 ; r2=max_x*s2 ;
   for( i=p1 ; i<=p2 ; i++ )
   {  r=r1+i ;
      stek[j_stek++]=Symbol[ r ] ;
      stek[j_stek++]=Color [ r ] ;
      r=r2+i ;
      stek[j_stek++]=Symbol[ r ] ;
      stek[j_stek++]=Color [ r ] ;
   }
   for( i=s1 ; i<=s2 ; i++ )
   {  r1=max_x*i+p1 ;
      stek[j_stek++]=Symbol[ r1 ] ;
      stek[j_stek++]=Color [ r1 ] ;
      r2=max_x*i+p2 ;
      stek[j_stek++]=Symbol[ r2 ] ;
      stek[j_stek++]=Color [ r2 ] ;
   }
   stek[j_stek++]=(uchar )p1 ; stek[j_stek++]=(uchar )s1 ;
   stek[j_stek++]=(uchar )p2 ; stek[j_stek++]=(uchar )s2 ;
   stek[j_stek++]=1 ;
}
/* ----------------------------------------------------------------------- */
void t_Save_Screen :: restore( void )
{  long i,j,p1,p2,s1,s2,
	 r,r1,r2 ;
   uchar c,f ;

   f =stek[--j_stek] ;
   s2=stek[--j_stek] ; p2=stek[--j_stek] ;
   s1=stek[--j_stek] ; p1=stek[--j_stek] ;
   if( f==1 )
   {  for( i=s2 ; s1<=i ; i-- )
      {  r=max_x*i+p2 ;
	 f=Color [ r ] = stek[--j_stek] ;
	 c=Symbol[ r ] = stek[--j_stek] ;
	 r=max_x*i+p1 ;
	 f=Color [ r ] = stek[--j_stek] ;
	 c=Symbol[ r ] = stek[--j_stek] ;
      }
      r1=max_x*s1 ; r2=max_x*s2 ;
      for( i=p2 ; p1<=i ; i-- )
      {  r=r2+i ;
	 f=Color [ r ] = stek[--j_stek] ;
	 c=Symbol[ r ] = stek[--j_stek] ;
	 r=r1+i ;
	 f=Color [ r ] = stek[--j_stek] ;
	 c=Symbol[ r ] = stek[--j_stek] ;
      }
   }
   else
   {  for( i=s2 ; s1<=i ; i-- )
	 for( j=p2 ; p1<=j ; j-- )
	 {   r=max_x*i+j ;
	     c=Symbol[ r ] = stek[--j_stek] ; // символ 
	     f=Color [ r ] = stek[--j_stek] ; // атрибут
	 }
   }
}
/* ----------------------------------------------------------------------- */
void s_redraw_rame( char fRame );

void s_redraw( void )
{  long  i,i1,j,r ;
   char  f,f1 ;
   unsigned short Text[256] ;

   t_GC GC( Pixmap1, 15, 0 );
   XSetFont( Dis, GC.GC1, FontInfo->fid );

   f=Color[0] ;
   GC.SetForeground( f&0x0f );
   GC.SetBackground( (f>>4)&0x0f );

   for( i=0 ; i<n_Line ; i++ )
   {  for( i1=0,j=0 ; i1<n_Col ; i1++ )
      {  r=max_x*i+i1 ;
         f1=Color[r] ;
	 if( f1!=f )
	 {   Text[j]=0 ;
	     GC.DrawImageString16( (i1-j)*Symbol_W, (i+1)*Symbol_H, Text );
	     f=f1 ; j=0 ;
	     GC.SetForeground( f&0x0f );
	     GC.SetBackground( (f>>4)&0x0f );
	 }
	 if( 0xffff<Symbol[r] ) Text[j++] = ' ' ;//koi9_to_Unicod(' ') ;
	     else               Text[j++] = Symbol[r] ;
      }
      Text[j]=0 ;
      GC.DrawImageString16( (i1-j)*Symbol_W, (i+1)*Symbol_H, Text );
   }
/* ------ курсор ---------------------
   GC.SetForeground( 0 );
   GC.SetBackground( 9 );
   Text[0] = Symbol[max_x*Cur_Y+Cur_X] ;
   Text[1] = 0 ;
   GC.DrawImageString16( Cur_X*Symbol_W ,(Cur_Y+1)*Symbol_H, Text );
*/
   s_redraw_rame( 1 );
   s_redraw_rame( 2 );
//   s_debug();
}
/************************************************************************/
void s_redraw_rame( char fRame )
{
   long  ULCORNER, LLCORNER, URCORNER, LRCORNER, HLINE, VLINE ;
   long  x,y,x1,x2,y1,y2 ;
   short draw,j,r ;
   long  c ;
   char  f,f1 ;

   t_GC GC ( Pixmap1, 15, 0 );
   t_GC GC2( Pixmap1,  0, 0 );

   if( fRame==1 )
   {   ULCORNER=ACS_ULCORNER ;
       LLCORNER=ACS_LLCORNER ;
       URCORNER=ACS_URCORNER ;
       LRCORNER=ACS_LRCORNER ;
       HLINE   =ACS_HLINE ;
       VLINE   =ACS_VLINE ;
       XSetLineAttributes( Dis, GC.GC1 , 1, LineSolid,CapRound,JoinMiter );
   }
   if( fRame==2 )
   {   ULCORNER=ACS_ULCORNER2 ;
       LLCORNER=ACS_LLCORNER2 ;
       URCORNER=ACS_URCORNER2 ;
       LRCORNER=ACS_LRCORNER2 ;
       HLINE   =ACS_HLINE2 ;
       VLINE   =ACS_VLINE2 ;
       XSetLineAttributes( Dis, GC.GC1 , 3, LineSolid,CapRound,JoinMiter );
       XSetLineAttributes( Dis, GC2.GC1 , 1, LineSolid,CapRound,JoinMiter );
   }

   for( y=0 ; y<n_Line ; y++ )
   {  draw=0 ;
      f=Color[max_x*y] ;
      y1=y2=Symbol_H*y+Symbol_H/2 +2 ;
      for( x=0,j=0 ; x<n_Col ; x++ )
      {  r=max_x*y+x ;
         f1=Color[r] ;
	 c=Symbol[r] ;

	 if( draw==1 && c!=HLINE && c!=URCORNER && c!=LRCORNER )
	 {   x2=Symbol_W*x ;
	     GC.SetForeground( f&0x0f );
	     XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	     if( fRame==2 )
	         XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	     draw=0 ;
	 }
	 if( draw==0 )
	 {  if( c==ULCORNER || c==LLCORNER )
	    {   draw=1 ; x1=Symbol_W*x + Symbol_W/2 -1 ;  }
	    if( c==HLINE )
	    {   draw=1 ; x1=Symbol_W*x -1 ; }
         }
	 if( c==URCORNER || c==LRCORNER )
	 {   if( draw==0 ) x1=Symbol_W*x ;
	     x2=Symbol_W*x + Symbol_W/2 -1 ;
	     draw=0 ;
	     GC.SetForeground( f&0x0f );
	     XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	     if( fRame==2 )
	         XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	 }
	 if( draw==1 && f1!=f )
	 {   x2=Symbol_W*x ;
	     GC.SetForeground( f&0x0f );
	     if( x1<x2 )
	     {   XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	         if( fRame==2 )
	             XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	     }
	     x1=x2 ;
	     if( c==ULCORNER || c==LLCORNER )
	         x1+=Symbol_W/2 -1 ;
	 }
	 f=f1 ;
      }
   }
   for( x=0 ; x<n_Col ; x++ )
   {  draw=0 ;
      f=Color[x] ;
      x1=x2=Symbol_W*x+Symbol_W/2 -1 ;
      for( y=0 ; y<n_Line ; y++ )
      {  r=max_x*y+x ;
         f1=Color[r] ;
	 c=Symbol[r] ;
	 if( draw==1 && c!=VLINE && c!=LLCORNER && c!=LRCORNER )
	 {   y2=Symbol_H*y +2 ;
	     GC.SetForeground( f&0x0f );
	     XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	     if( fRame==2 )
	         XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	     draw=0 ;
	 }
	 if( draw==0 )
	 {  if( c==ULCORNER || c==URCORNER )
	    {   draw=1 ; y1=Symbol_H*y + Symbol_H/2 +2 ;  }
	    if( c==VLINE )
	    {   draw=1 ; y1=Symbol_H*y +2 ; }
         }
	 if( c==LLCORNER || c==LRCORNER )
	 {   if( draw==0 ) y1=Symbol_H*y ;
	     y2=Symbol_H*y + Symbol_H/2 +2 ;
	     draw=0 ;
	     GC.SetForeground( f&0x0f );
	     XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	     if( fRame==2 )
	         XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	 }
	 if( draw==1 && f1!=f )
	 {   y2=Symbol_H*y ;
	     GC.SetForeground( f&0x0f );
	     if( y1<y2 )
	     {   XDrawLine( Dis, GC.Win, GC.GC1, x1, y1, x2, y2 );
	         if( fRame==2 )
 	             XDrawLine( Dis, GC2.Win, GC2.GC1, x1, y1, x2, y2 );
	     }
	     y1=y2 ;
	     if( c==ULCORNER || c==URCORNER )
	         y1+=Symbol_H/2 +2 ;
	 }
	 f=f1 ;
      }
   }
}
/* ----------------------------------------------------------------------- */
void   s_goto_xy( short y, short x )
{
   Cur_X=x ; Cur_Y=y ;
}
/***************************************************************************/
/*       принять событие и сделать из него Сканкод (юникодная версия)
/***************************************************************************/
e_sEvent  s_getch( unsigned short *taste1, unsigned short *taste2 )
{  e_sEvent  type ;
   unsigned short taste1a ;

   type=s_getch16( &taste1a, taste2 );
   *taste1=Unicod_to_koi8( taste1a );
   return type ;
}
/***************************************************************************/
e_sEvent  s_getch16( unsigned short *taste1, unsigned short *taste2 )
{
   int     Symbol,Symbol1 ;
   char    Symb[3] ;
   KeySym  RetSym ;
   XEvent  Event1 ;
   ushort  t1,t2,t3,t4 ;

   s_redraw( );
   t_GC GC1( Pixmap1,0,0 ) ;
M2:XCopyArea( Dis,Pixmap1,Win,GC1.GC1,0,0,n_Col*Symbol_W,n_Line*Symbol_H+DELTA,0,0 );
M_Begin:
   XNextEvent( Dis, &Event1 );
   if( Event1.type == Expose ) goto M2 ;
   if( Event1.type != KeyPress && Event1.type != KeyRelease && 
       Event1.type != ButtonPress && Event1.type != ButtonRelease && 
       Event1.type!=MotionNotify ) goto M_Begin ;

   Symbol=0 ; Symbol1=0 ;
   *taste1=0 ;
   *taste2=0 ;

   /* ------------------------------------ */
   if( Event1.type == KeyPress )
   {  
      XLookupString(&(Event1.xkey), Symb, 1, &RetSym, NULL);

      t1=(0xff00&RetSym)>>8 ;
      t2=0x00ff&RetSym ;
      if( t1==6 ) t3=0 ;
          else    t3=t1+1 ;
      t4=t3+t2 ;
      if( t3<5 )
          t4=ToUnicode[t3][t2] ;

      switch( RetSym )
      {
	 case  XK_Caps_Lock :
	 case  XK_ISO_Next_Group :  goto M_Begin ;
	 
	 case  XK_Shift_L   : Shiftstatus |= (S_Shift_L); goto M_Begin ;
	 case  XK_Control_L : Shiftstatus |= (S_Ctrl_L);  goto M_Begin ;
	 case  XK_Alt_L     : Shiftstatus |= (S_Alt_L);   goto M_Begin ;
	 case  XK_Alt_R     : Shiftstatus |= (S_Alt_R);   goto M_Begin ;
	 case  XK_Control_R : Shiftstatus |= (S_Ctrl_R);  goto M_Begin ;
	 case  XK_Shift_R   : Shiftstatus |= (S_Shift_R); goto M_Begin ;

	 case  XK_F1:            Symbol1 = S_key_F1;break;
	 case  XK_F2:            Symbol1 = S_key_F2;break;
	 case  XK_F3:            Symbol1 = S_key_F3;break;
	 case  XK_F4:            Symbol1 = S_key_F4;break;
	 case  XK_F5:            Symbol1 = S_key_F5;break;
	 case  XK_F6:            Symbol1 = S_key_F6;break;
	 case  XK_F7:            Symbol1 = S_key_F7;break;
	 case  XK_F8:            Symbol1 = S_key_F8;break;
	 case  XK_F9:            Symbol1 = S_key_F9;break;
	 case  XK_F10:           Symbol1 = S_key_F10;break;

	 case  XK_L1:            Symbol1 = S_key_F1;break;
	 case  XK_L2:            Symbol1 = S_key_F2;break;
	 case  XK_L3:            Symbol1 = S_key_F3;break;
	 case  XK_L4:            Symbol1 = S_key_F4;break;
	 case  XK_L5:            Symbol1 = S_key_F5;break;
	 case  XK_L6:            Symbol1 = S_key_F6;break;
	 case  XK_L7:            Symbol1 = S_key_F7;break;
	 case  XK_L8:            Symbol1 = S_key_F8;break;
	 case  XK_L9:            Symbol1 = S_key_F9;break;
	 case  XK_L10:           Symbol1 = S_key_F10;break;

	 case  XK_Return:        Symbol  = '\r';  break;
	 case  XK_Tab:        t4=Symbol  = S_key_TabR; break;
	 case  XK_ISO_Left_Tab: t4=Symbol= S_key_TabR; break;
	 case  XK_Escape:        Symbol  = S_key_Esc;  break;
	 case  XK_BackSpace:     Symbol  = S_key_Back; break;
	 case  XK_Left:          Symbol1 = S_key_Left; break;
	 case  XK_Right:         Symbol1 = S_key_Right;break;
	 case  XK_Down:          Symbol1 = S_key_Down; break;
	 case  XK_Up:            Symbol1 = S_key_Up;   break;
	 case  XK_Insert:        Symbol1 = S_key_Ins;  break;
	 case  XK_Delete:        Symbol1 = S_key_Del;  break;
	 case  XK_Home:          Symbol1 = S_key_Home; break;
	 case  XK_End:           Symbol1 = S_key_End;  break;
	 case  XK_Prior:         Symbol1 = S_key_PgUp; break;
	 case  XK_Next:          Symbol1 = S_key_PgDn; break;
      }
      *taste1 = t4 ;
      if( Symbol == S_key_Back ) *taste1=S_key_Back ;
      if( Symbol == S_key_Esc  ) *taste1=S_key_Esc ;
      if( Symbol == '\r' )       *taste1='\r' ;

      *taste2 = Symbol1 ;
      if( Symbol1!=0 ) *taste1=0 ;     
   }
   /* -------------------------------------- */
   if( Event1.type == KeyRelease )
   {  
      XLookupString(&(Event1.xkey), Symb, 1, &RetSym, NULL);

      switch( RetSym )
      {
	 case  XK_Caps_Lock :  goto M_Begin ;
	 case  XK_Control_L : Shiftstatus &= (~S_Ctrl_L);  goto M_Begin ;
	 case  XK_Alt_L     : Shiftstatus &= (~S_Alt_L);   goto M_Begin ;
	 case  XK_Alt_R     : Shiftstatus &= (~S_Alt_R);   goto M_Begin ;
	 case  XK_Control_R : Shiftstatus &= (~S_Ctrl_R);  goto M_Begin ;
	 case  0xfe0a:
	 case  XK_Shift_L   : Shiftstatus &= (~S_Shift_L); goto M_Begin ;
//	 case  XK_ISO_Next_Group :
	 case  0xfe0e:
	 case  XK_Shift_R   : Shiftstatus &= (~S_Shift_R); goto M_Begin ;
      }
      goto M_Begin ;
   }
   if( Event1.type==ButtonPress || Event1.type==ButtonRelease )
   {
       Mouse_x= Event1.xbutton.x/Symbol_W ;
       Mouse_y= Event1.xbutton.y/Symbol_H ;
       MouseButton=Event1.xbutton.button ;
       //printf( "Button %d\n",Event1.xbutton.button );
   }
   if( Event1.type==MotionNotify )
   {   
       Mouse_x= Event1.xmotion.x/Symbol_W ;
       Mouse_y= Event1.xmotion.y/Symbol_H ;
       if( 0==f_mouse_move ) goto M_Begin ;
   }

//   if( (Event1.type != KeyPress) && (Event1.type != KeyRelease) )
//      goto M_Begin ;

   if( Event1.type==KeyPress      ) return KEY_PRESS ;
   if( Event1.type==KeyRelease    ) return KEY_RELEASE ;
   if( Event1.type==ButtonPress   ) return MOUSE_PRESS ;
   if( Event1.type==ButtonRelease ) return MOUSE_RELEASE ;
   if( Event1.type==MotionNotify  ) return MOUSE_MOVE ;
   goto M_Begin ;
}
/***************************************************************************/
int    s_shiftstatus( void )
{
   return Shiftstatus ;
}
/***************************************************************************/
short  s_get_mouse( short *x, short *y )
{
   *x=Mouse_x ;
   *y=Mouse_y ;
   return MouseButton ;
}
/***************************************************************************/
char   s_break( )
{   Bool T ;
    char    Symb[3] ;
    KeySym  RetSym ;
    XEvent  Event1 ;

    T=XCheckTypedEvent( Dis, KeyPress, &Event1 );
    if( 0==T ) return 0 ;
    XLookupString(&(Event1.xkey), Symb, 1, &RetSym, NULL);
    if( XK_Escape==RetSym ) return 1 ;
    return 0 ;
} 
/***************************************************************************/
t_GC :: t_GC( Window Win1, ulong i1, ulong i2 )
{
   if( 16<=i1 ) i1=7 ;
   if( 16<=i2 ) i2=7 ;
   Win=Win1 ;
   GC1=XCreateGC( Dis,Win,0,NULL );
   XSetFunction(  Dis, GC1, GXcopy );
   XSetForeground( Dis,GC1, xColor[i1].pixel );
   XSetBackground( Dis,GC1, xColor[i2].pixel );
}
/* ----------------------------------------------------------------------- */
t_GC :: ~t_GC( void )
{  XFreeGC( Dis,GC1 ); }
/* ----------------------------------------------------------------------- */
void t_GC :: SetForeground( ulong i )
{  XSetForeground( Dis,GC1, xColor[i].pixel );  }
/* ----------------------------------------------------------------------- */
void t_GC :: SetBackground( ulong i )
{  XSetBackground( Dis,GC1, xColor[i].pixel );  }
/* ----------------------------------------------------------------------- */
void t_GC :: DrawImageString( short x, short y, char *Text )
{  XDrawImageString( Dis,Win,GC1,x,y,Text,strlen(Text) );  }
/* ----------------------------------------------------------------------- */
void t_GC :: DrawImageString16( short x, short y, unsigned short *Text )
{
   long L ;
   for( L=0 ; Text[L]!=0 ; L++ ) ;

   XDrawImageString16( Dis,Win,GC1,x,y,(XChar2b *)Text,L );  
}
/***************************************************************************/
void   s_set_size( short sx, short sy )
{
   n_Col=sx ; n_Line=sy ;
   XResizeWindow( Dis, Win, sx*Symbol_W, sy*Symbol_H+DELTA );
}
/***************************************************************************/
//        сделать таблицу перекодирования кнопок на русский шрифт
/***************************************************************************/
void make_ang_rus( void )
{  short *T=T_ang_rus ;

   for( short i=0 ; i<256 ; i++ )
      T[i]=i ;

   T['q'] ='й'; T['w'] ='ц'; T['e'] ='у'; T['r'] ='к';
   T['t'] ='е'; T['y'] ='н'; T['u'] ='г'; T['i'] ='ш';
   T['o'] ='щ'; T['p'] ='з'; T['['] ='х'; T[']'] ='ъ';
   T['a'] ='ф'; T['s'] ='ы'; T['d'] ='в'; T['f'] ='а';
   T['g'] ='п'; T['h'] ='р'; T['j'] ='о'; T['k'] ='л';
   T['l'] ='д'; T[';'] ='ж'; T['\'']='э'; T['z'] ='я';
   T['x'] ='ч'; T['c'] ='с'; T['v'] ='м'; T['b'] ='и';
   T['n'] ='т'; T['m'] ='ь'; T[','] ='б'; T['.'] ='ю';

   T['Q'] ='Й'; T['W'] ='Ц'; T['E'] ='У'; T['R'] ='К';
   T['T'] ='Е'; T['Y'] ='Н'; T['U'] ='Г'; T['I'] ='Ш';
   T['O'] ='Щ'; T['P'] ='З'; T['{'] ='Х'; T['}'] ='Ъ';
   T['A'] ='Ф'; T['S'] ='Ы'; T['D'] ='В'; T['F'] ='А';
   T['G'] ='П'; T['H'] ='Р'; T['J'] ='О'; T['K'] ='Л';
   T['L'] ='Д'; T[':'] ='Ж'; T['"'] ='Э'; T['Z'] ='Я';
   T['X'] ='Ч'; T['C'] ='С'; T['V'] ='М'; T['B'] ='И';
   T['N'] ='Т'; T['M'] ='Ь'; T['<'] ='Б'; T['>'] ='Ю';

   T['^'] =','; T['&'] ='.'; T['$'] ='"'; T['$'] =':';
}
/***************************************************************************/
void   s_refresh( void )
{  s_redraw();
   t_GC GC1( Pixmap1,0,0 );
   XCopyArea( Dis,Pixmap1,Win,GC1.GC1,0,0,n_Col*Symbol_W,n_Line*Symbol_H,0,0 );
}
/***************************************************************************/
void   s_get_size( short &sx, short &sy ){  sx=n_Col ; sy=n_Line ;  }
void   s_get_size( short *sx, short *sy ){  *sx=n_Col ; *sy=n_Line ;  }

short  s_get_sx( void ) {  return n_Col ; }
short  s_get_sy( void ) {  return n_Line ; }

void   x_set_fonts( void ){;}
void   s_set_font( char *F ){ Strcpy( TextFont,F,100 ); }
char  *s_get_font( void ){ return TextFont ; }
short  s_clear_cursor( void ){ return 0; }
void   s_set_ruskomb( short Komb ){  RussKomb=Komb ;  }
char   s_run( ){  return Run ;  }
void   s_set_mouse_move( char f ){  f_mouse_move=f ;  }
char   s_x_version( ){  return 1 ;  }
