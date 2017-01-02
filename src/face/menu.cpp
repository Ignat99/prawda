/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                             менюшка
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <stdio.h>
# include <malloc.h>
# include <string.h>

# include "mylib.h"
# include "video.h"
# include "option.h"
# include "menu.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

# define stop() {  ushort key1,key2 ; s_getch(&key1,&key2); }
 
/* ----------------------------------------------------------------------- */
struct t_menuForm
{  short   size_x,size_y,
	   n_punkt,
	  *igrek ;
   t_Menu *menu ;
} ;

static short   Main_menu_y ;
static short   mass_iks[25], mass_igrek[25], Ret ;

/* ----------------------------------------------------------------------- */

short  hor_menu( short y, t_Menu *menu, short mouse_x=-1 );
void   s_hor_menu( short y, short *x, t_Menu *menu, short pos=-1 );
void   s_ver_menu( short y, short x, t_menuForm F, short pos=-1 );
short  ver_menu( short y, short x, t_Menu *menu, short *mouse_x );
void   ver_menu_creat( t_menuForm *f, t_Menu *menu );
short  sehen_hor_menu( short **x, t_Menu *menu );
static short  schnell_taste( t_Menu *menu, uchar taste );
void   sorry( short y, short x );

/* *********************************************************************** */
short menu( t_Menu *Menu, short y )
{
    Ret=0 ;
    Main_menu_y=y ;
    hor_menu( y, Menu );
    null_menu( Menu );
    return Ret ;
}
/* ----------------------------------------------------------------------- */
short menu_mouse( t_Menu *Menu, short y, short mouse_x )
{
    Ret=0 ;
    Main_menu_y=y ;
    hor_menu( y, Menu, mouse_x );
    null_menu( Menu );
    return Ret ;
}
/* ----------------------------------------------------------------------- */
void null_menu( t_Menu *Menu, short y )
{  short *x ;

   sehen_hor_menu( &x,Menu ) ;
   s_hor_menu( y,x,Menu ) ;
}
/* ----------------------------------------------------------------------- */
/*             реакция на кнопки внутри горизонтального меню               */
/* ----------------------------------------------------------------------- */
short hor_menu( short y, t_Menu *menu, short mouse_x )
{  short    p ;        // текущий пункт меню
   short    flag=0 ;
   short    size_Menu ;
   short    *x,i ;
   e_sEvent Event ;
   static ushort key1,key2,key3 ;

M_Begin:
   p=0 ;
   size_Menu=sehen_hor_menu( &x,menu ) ;
   for( i=0 ; i<size_Menu ; i++ )
      if( x[i]<=mouse_x )
          p=i ;
   s_hor_menu( y,x,menu,p );
   if( 0<=mouse_x ) goto M_ENTER ;
   
   while( 1 )
   {
       s_hor_menu( y,x,menu,p );
       Event=s_getch( &key1,&key2 );
       
       if( MOUSE_PRESS==Event )
       {   short xx,yy ;
           s_get_mouse( &xx, &yy );
	   if( yy!=y )
	       return S_key_Esc ;
	   for( i=0 ; i<size_Menu ; i++ )
	      if( x[i]<=xx )
                  p=i ; 
           s_hor_menu( y,x,menu,p );
	   goto M_ENTER ;
       }

       switch( key2 )
       {   case S_key_Left: M_LINKS :
		if( p<=0 ) p=size_Menu-1 ; else p-- ;
		if( flag==1 && menu[p].flag==F_MENU )
		{   s_hor_menu( y,x,menu,p );
		    key3=ver_menu( y+1,x[p], (t_Menu *)menu[p].data, &mouse_x ) ;
		}
		else
		{  key3=0 ; }
		break ;
	   case S_key_Right: M_RECHTS :
		if( p>=size_Menu-1 ) p=0 ; else p++ ;
		if( flag==1 && menu[p].flag==F_MENU )
		{   s_hor_menu( y,x,menu,p );
		    key3=ver_menu( y+1,x[p], (t_Menu *)menu[p].data, &mouse_x ) ;
		}
		else
		{  key3=0 ; }
		break ;
	   case S_key_Down: goto M_ENTER ;
	   case S_key_F10 : return S_key_Esc ;
	   case 0 :
	   switch( key1 )
	   {
		schnell :
		    s_hor_menu( y,x,menu,p );
		case '\r' : M_ENTER :
		   if( menu[p].flag==F_NUMER )
		   {   Ret=((char *)menu[p].data)[0] ;
		       return S_key_Esc ;
		   }
		   if( menu[p].flag==F_FUNK )
		   {   menu[p].funk( menu[p].data ) ;
		       return S_key_Esc ;
		   }
		   if( menu[p].flag==F_MENU )
		   {   flag=1 ;
		       key3=ver_menu( y+1,x[p], (t_Menu *)menu[p].data, &mouse_x );
		   }
		   break ;
	       case S_key_Esc : return S_key_Esc ;
	       default :
		   {   short p1 ;
		       p1=schnell_taste( menu,key1 ) ;
		       if( 0<=p1 ) {  p=p1 ; goto schnell ;  }
		   }
		   break ;
	   }
       }
       switch( key3 )
       {   case S_key_Left : goto M_LINKS ;
	   case S_key_Right: goto M_RECHTS ;
	   case S_key_F9   : goto M_Begin ;
	   case S_key_Esc  : return S_key_Esc ;
       }
   }
}
/* ----------------------------------------------------------------------- */
/*              реакция на кнопки внутри вертикального меню                */
/* ----------------------------------------------------------------------- */
short ver_menu( short y1, short x1, t_Menu *menu, short *mouse_x )
{  short       p=0,i,ret ;
   t_menuForm  f ;
   e_sEvent     Event ;
   static ushort key1,key2 ;

  *mouse_x=-1 ;
   ver_menu_creat( &f,menu ) ;
   s_save( y1,x1,y1+f.size_y+1,x1+f.size_x+1 );
   while( 1 )
   {
       s_ver_menu( y1,x1,f,p );
       Event=s_getch( &key1,&key2 );
       
       if( MOUSE_PRESS==Event )
       {   short x,y ;
           s_get_mouse( &x, &y );
	   if( x<x1 || (x1+f.size_x+1)<x || y<y1 || (y1+f.size_y+1)<y )
	   {   if( y==Main_menu_y )
	       {   *mouse_x=x ;
	           ret = S_key_F9 ; // безобразие - коды должны быть Enum
	       }else
	       {   ret = S_key_Esc ;
	       }
	       goto ENDE ;
           }
	   for( short i1=0 ; i1<f.n_punkt ; i1++ )
	      if( f.igrek[i1]==(y-y1-1) )
              {   p=i1 ; goto schnell ;  }
       }

       switch( key2 )
       {   case S_key_Up:
		if( p<=0 ) p=f.n_punkt-1 ; else p-- ; break ;
	   case S_key_Down:
		if( p>=f.n_punkt-1 ) p=0 ; else p++ ; break ;
	   case S_key_Left : ret = S_key_Left ;  goto ENDE ;
	   case S_key_Right: ret = S_key_Right ; goto ENDE ;
	   case S_key_F10  : ret = S_key_Esc ;   goto ENDE ;
	   case 0 :
	       switch( key1 )
	       {   case '\r' :
	   schnell :   i=f.igrek[p] ;
		       if( menu[i].flag==F_NUMER )
		       {
		           Ret=((char *)menu[i].data)[0] ;
			   ret = S_key_Esc ;
			   goto ENDE ;
		       }
		       if( menu[i].flag==F_FUNK )
		       {   if( menu[i].ret!=1 ) s_restore( ) ;
			   menu[i].funk( menu[i].data ) ;
			   ret = S_key_Esc ;  
			   if( menu[i].ret!=1 ) goto ENDE1 ;
			   break ;
		       }
		       if( menu[i].flag==F_MENU )
		       {   short mouse_x ;
		           ver_menu( y1+i+1,x1+f.size_x, (t_Menu *)menu[i].data,&mouse_x );
			   break ;
		       }
	           case S_key_Esc : ret = S_key_Esc ; goto ENDE ;
		   default :
		       {   short p1 ;
			   p1=schnell_taste( menu,key1 ) ;
			   if( 0<=p1 ) {  p=p1 ; goto schnell ;  }
		       }   break ;
	       }
	       break ;
       }
   }
ENDE :  s_restore( ) ;
ENDE1 :
   return ret ;
}
/* ----------------------------------------------------------------------- */
//              определяет, не нажата ли горячая клавиша
/* ----------------------------------------------------------------------- */
short schnell_taste( t_Menu *menu, uchar taste )
{  short i,j ;
   uchar c ;

   for( i=0 ; menu[i].flag!=0 ; i++ )
   {   for( j=0 ; ; j++ )
       {   c=menu[i].name[j] ;
	   if( c==0 ) break ;
	   if( (uchar)'А'<=c && c<=(uchar)'Я' )
	   {   if( c==taste ) return i ;
	       //if( XLAT[c]==XLAT[taste] ) return i ;
	       break ;
	   }
       }
   }
   return -1 ;
}
/* ----------------------------------------------------------------------- */
//        расставит иксы для рисования выпадающих меню
/* ----------------------------------------------------------------------- */
short sehen_hor_menu( short **iks, t_Menu *menu )
{  short i,j, *a ;

   *iks=a = mass_iks ;
   for( j=2,i=0 ; ; i++ )
   {  if( menu[i].flag==0 ) break ;
      a[i]=j ;
      j+=strlen( menu[i].name )+2 ;
   }
   return i ;
}
/* ----------------------------------------------------------------------- */
/*                     изобразить горизонтальное меню                      */
/* ----------------------------------------------------------------------- */
void s_hor_menu( short y, short *x, t_Menu *menu, short pos )
{  short i ;

   s_text_yxf( y,0,menu_Color,emptystr( s_get_sx() ) ) ;
   for( i=0 ; i<25 ; i++ )
   {  if( menu[i].flag==0 ) break ;
      s_text_yxf( y,x[i],menu_Color,menu[i].name ) ;
   }
   // ------- изобразить текущую позицию горизонтального меню -------
   if( 0<=pos )
       s_text_yxf( y,x[pos],curs_menu_Color,menu[pos].name ) ;
}
/* ----------------------------------------------------------------------- */
/*                     изобразить вертикальное меню                        */
/* ----------------------------------------------------------------------- */
void s_ver_menu( short y, short x, t_menuForm F, short pos )
{  short i,c ;

   s_rame1_F( y,x,y+F.size_y+1,x+F.size_x+1,menu_Color ) ;
   for( i=0 ; i<F.size_y ; i++ )
   {  if( i==mass_igrek[pos] )
      {   s_text_yxf( y+1+i,x+1,curs_menu_Color,emptystr(F.size_x) ) ;
	  c=curs_menu_Color ;
      }
      else c=menu_Color ;
      s_text_yxf( y+1+i,x+1,c,F.menu[i].name ) ;
   }
}
/* ----------------------------------------------------------------------- */
/*                       конструктор формы меню                            */
/* ----------------------------------------------------------------------- */
void ver_menu_creat( t_menuForm *f, t_Menu *menu )
{  short i,j,s,*igrek ;

   for( i=0 ; i<25 ; i++ ) if( menu[i].flag==0 ) break ;
   f->size_y=i ;

   igrek = mass_igrek ;
   for( s=j=0,i=0 ; i<f->size_y ; i++ )
   {  if( menu[i].flag==0 ) break ;
      s=max( (unsigned)s, strlen( menu[i].name ) );
      if( menu[i].flag==2 || menu[i].flag==3 || menu[i].flag==4 )
	  igrek[j++]=i ;
   }
   f->size_x=s ;     f->n_punkt=j ;
   f->igrek=igrek ;  f->menu=menu ;
}
/* ----------------------------------------------------------------------- */
//          переобозвать пункт меню (поставить в него галку)
/* ----------------------------------------------------------------------- */
char menu_set( t_Menu *Menu, char *sFind, char *sReplace )
{  short i ;

   for( i=0 ; i<100 ; i++ )
   {  if( Menu[i].flag==F_ENDE ) break ;
      if( 0==strcmp(Menu[i].name,sFind) )
      {   strcpy( Menu[i].name,sReplace );
          return 1 ;
      }	  
      if( Menu[i].flag==F_MENU )
          if( 1==menu_set( (t_Menu *)Menu[i].data,sFind, sReplace ) ) return 1;
   }
   return 0 ;
}
/* ----------------------------------------------------------------------- */
//            переобозвать пункт меню (поставить в него галку)
/* ----------------------------------------------------------------------- */
/*
void menu_set( char *sFind, char *sReplace )
{
   menu_set( Main_menu, sFind, sReplace );
}
*/
/* ----------------------------------------------------------------------- */
//                    найти пункт меню по его имени
/* ----------------------------------------------------------------------- */
t_Menu * menu_find( t_Menu *Menu, char *sFind )
{  short i ;
   t_Menu *R ;

   for( i=0 ; i<100 ; i++ )
   {  if( Menu[i].flag==F_ENDE ) break ;
      if( 0==strcmp(Menu[i].name,sFind) )
          return &Menu[i] ;
      if( Menu[i].flag==F_MENU )
      {   R=menu_find( (t_Menu *)Menu[i].data,sFind );
          if( R!=NULL ) return R ;
      }
   }
   return NULL ;
}
/* ----------------------------------------------------------------------- */
/*
t_Menu * menu_find( char *sFind )
{
   return menu_find( Main_menu, sFind );
}
/* ----------------------------------------------------------------------- */
void Sorry( void *d ){   sorry( 10,30 ) ;  }
/* ----------------------------------------------------------------------- */
void sorry( short y, short x )
{
   s_save( y,x,y+4,x+20 ) ;
   s_rame1_f( y,x,y+4,x+20,0x70 ) ;
   s_text_yxf( y+1,x+1,0x70,"      Извините     ") ;
   s_text_yxf( y+2,x+1,0x70,"  эта функция еще  ") ;
   s_text_yxf( y+3,x+1,0x70,"  не  реализована  ") ;

   stop( ) ;
   s_restore( ) ;
}



