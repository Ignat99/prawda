/*******************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                      ����������� ����� ����
//     �������� �.�.                               �������
/*******************************************************************/

# include <string.h>

# include "video.h"
# include "option.h"
# include "window.h"

/***************************************************************************/
//                       ����������� ����� ����                            //
/***************************************************************************/
t_Win :: t_Win( void )
{
   Name[0]=0 ;
   f_2Rame=0 ;
   set_pos( 0, 79, 1, 24 );
}
/***************************************************************************/
t_Win :: ~t_Win( void )
{ ; }
/***************************************************************************/
void t_Win :: set_pos( short _x1, short _y1, short _x2, short _y2 )
{
   x1=_x1 ; y1=_y1 ; x2=_x2 ; y2=_y2 ;
   size_x = x2-x1 ;
   size_y = y2-y1 ;
}
/***************************************************************************/
void t_Win :: set_pos( t_Rect &Rect )
{
   set_pos( Rect.x1, Rect.y1, Rect.x2, Rect.y2 );
}
/***************************************************************************/
t_Rect t_Win :: get_pos( )
{  t_Rect R ;
   R.x1=x1 ; R.y1=y1 ; R.x2=x2 ; R.y2=y2 ; 
   return R ;
}
/***************************************************************************/
void t_Win :: set_name( char *S )
{  strcpy(Name,S) ;
}
/***************************************************************************/
void t_Win :: paint( char S )
{  char c ;

   if( S )  c=0x0f ;
       else c=0x07 ;
   if( 1==f_2Rame )
        s_rame2_F( y1, x1, y2, x2, c );
   else s_rame1_F( y1, x1, y2, x2, c );
   s_text_yxf( y1,x1+3 ,titl_Color, Name );
}
/**************************************************************************/
//    �������������� ���� ������ (������ ������� ��� ������ �������)
/**************************************************************************/
void t_Win :: loop_rect( short yy )
{
   short    x,*z ;
   ushort   ZZ,key1,key2 ; // ������� �������
   e_sEvent Event ;

   short xx1=x1-1,yy1=y1-1,xx2=x2+1,yy2=y2+1 ;

   if( yy==yy1 ) z=&yy1 ;
   if( yy==yy2 ) z=&yy2 ;

   s_save_rame( yy1,xx1,yy2,xx2 );
   s_set_mouse_move( 1 );

   while( 1 )
   {
      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();

      if( MOUSE_MOVE==Event )
      {   s_get_mouse( &x, z );

          s_restore( );
	  s_save_rame( yy1,xx1,yy2,xx2 );
	  s_rame1_f  ( yy1,xx1,yy2,xx2, 0x0b );
      }
      if( MOUSE_RELEASE==Event )
      {   break ;
      }
   }
   s_set_mouse_move( 0 );
   x1=xx1+1 ; y1=yy1+1 ; x2=xx2-1 ; y2=yy2-1 ;
}
/***************************************************************************/
char t_Rect :: inside( short x, short y )
{
   return ( x1<=x && x<=x2 && y1<=y && y<=y2 );
}
