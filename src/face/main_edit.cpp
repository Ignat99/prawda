/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                                 main()
//     �������� �.�.                                         �������
/***************************************************************************/

# include <string.h>
# include <unistd.h>
# include <time.h>
# include <ctype.h>
# include <sys/types.h>
# include "mylib.h"
# include "video.h"
# include "face.h"
# include "menu.h"
# include "menu.h2"
# include "edit.h"
# include "edit1.h"
# include "debug.h"
# include "resource1.h"
# include "hypertext.h"
# include "hypertext2.h"

char hk_Edit[]  ="[F1]��������� [F3 F4] ����� ������ � ����� [F10]����" ;
char hk_Help[]  ="[Enter] ������� �� ������ [F10] ������� � �������� �����" ;
char hk_Path[]  ="[F1] ��������� [Tab] ������� ����� ������ [�������] �������� �� ������ ��������� [F10] �����" ;
char hk_Slowo[] ="[F1] ��������� [�������] ������� [Shift+PgDn/PgUp] ����/���� ����� [Esc] ������� ����� [F10] �����" ;
char hk_Word[]  ="[F1] ��������� [^C <-] ���� ����� [^C ->] ���� ����� [->] ����� ��������� [F10] �����" ;
char hk_Word1[] ="[F1] ��������� [<- ->] ����/���� ����� [Enter] ������ ������� [F10] �����" ;

char hk_First[] ="[F1] ��������� [F3] �������� [F4] �������� [F5] �������� [Enter] ��������� [F10]�����" ;
char hk_Sou[]   ="[F1] ��������� [F2] ������ ������ [F4] �������� [F5] �������� [F10] �����" ;
char hk_Dst[]   ="[F1] ��������� [F2] ������ ������ [F3] �������� [F5] �������� [F10] �����" ;
char hk_Debug1[]="[F1] ��������� [<- ->] ����/���� ����� [������] �����/���� ������ [Enter] ������ ��������� [F10] �����";
char hk_Debug2[]="[F1] ��������� [Enter] ��������� �������� [<-|] ������� � ������� ������� [F10] �����" ;
char hk_Debug3[]="[F1] ��������� [Enter] ��� ���� [<-|] ��� ����� �� ������ ����������� [F10] �����" ;
char hk_Gramm[] ="[F1]���������[Enter]����[<-|]����� �� ������ ����������[Tab]��������/��������[F8]���������[F10]�����" ;
char hk_Perev[] ="[F1]��������� [Enter]��� ���� [<-|]��� ����� �� ������ ������� [F10]�����" ;
char hk_Dicts[] ="[F1]��������� [Enter] ����������� ������ ���� ������� [F10]�����" ;

extern t_Option    Option ;
// ------- �����, ��� ����� ����������� ������� � ��. ������ ----------
char *MercuryDir00[]={ "/usr/share/prawda",
                       "/usr/local/share/prawda/",
                        NULL
		     };
// ------- �����, ��� ��� ������������� ����������� -------------------
char *MercuryDir0 ;

// ------- �����, ��� ����� ����������� ��������� ----------
char *HelpDir00[]={ "/usr/share/doc/prawda/help",
                    "/usr/local/share/doc/prawda/help",
                     NULL
                  };
// ------- �����, ��� ��� ������������� ����������� -------------------
char  HelpDir0[200] ;
char *HelpDir ;

t_Debug      Debug ;
t_Option     Option ;
t_Dir        Dir ;
t_HelpData   HelpData ;
t_Text       Text1,HelpText ;
t_EditTrans  Edit ;
t_Edit       HelpView ;
char         MercuryDir[200]="../" ;
t_Hypertext  Hypertext ;
t_Hypertext2 Hypertext2( &Hypertext );
t_Kontext    Kontext ;
t_Kontext2   Kontext2( &Kontext );

void null_menu( void ) ;
char is_number( char *Str );
void preproc_rc( char *File );
void language_rc( char *File );
void set_mercury_dir( char *h );

/***************************************************************************/

/***************************************************************************/
int main( short argc, char *argv[] )
{  char  r,r1 ;
   char  *h,Str[200]="" ;
   short sx=80,sy=50 ;

try
{  
   h=getenv( "HOME" );
   if( h!=NULL ) {  Strcpy( Str,h,200 );  }
   Strcat( Str,"/.prawdarc",200 );

   if( 0>Option.read( Str ) )
   {   printf("\n ���� %s ����������� ��� �����������.",Str ); exit(-1) ;  }

   set_mercury_dir( getenv( "HOME" ) );
   // --------- ���������� ������� � � -------------
   Strcpy( Str,Option.get("size_x"),200 );
   if( is_number( Str ) && Str[0]!=0 ) sx=atoi(Str);
   Strcpy( Str,Option.get("size_y"),200 );
   if( is_number( Str ) && Str[0]!=0 ) sy=atoi(Str);
   // --------- ���������� ������������ ------------
   Strcpy( Str,Option.get("rus"),200 );
   if( Str[0]!=0 && 0!=strcmp(Str,"No") )
   {  short Komb=0 ;
      if( NULL!=strstr(Str,"Shift_R") ) Komb|=S_Shift_R ;
      if( NULL!=strstr(Str,"Shift_L") ) Komb|=S_Shift_L ;
      if( NULL!=strstr(Str,"Ctrl_R")  ) Komb|=S_Ctrl_R ;
      if( NULL!=strstr(Str,"Ctrl_L")  ) Komb|=S_Ctrl_L ;
      if( NULL!=strstr(Str,"Alt_R")  )  Komb|=S_Alt_R ;
      if( NULL!=strstr(Str,"Alt_L")  )  Komb|=S_Alt_L ;
   
      s_set_ruskomb( Komb );   
   }   
   // --------- ���������� ����� -------------------
   Strcpy( Str,Option.get("font"),200 );
   if( Str[0]!=0 ) s_set_font( Str );
   s_begin_schone( );
   s_set_size( sx,sy );
   s_nacht( );

   // --------- ���������� HelpDir -----------------
   HelpDir=HelpDir0 ;
   Strcpy( HelpDir0,MercuryDir ,200 );
   Strcat( HelpDir0,"/doc/help",200 );
   for( long i=0 ; HelpDir00[i]!=NULL ; i++ )
   {  if( 0==access( HelpDir,R_OK ) ) break ;
      HelpDir=HelpDir00[i] ;
   }
   // --------- ���������� ������ ------------------
   null_menu( );
   s_refresh();

//   HelpData.make();

   HelpView.set_text( &HelpText );
   HelpView.set_name("���������");
   HelpView.set_view( 1 ) ;
   HelpView.set_pos( 1, 2, sx-2, sy-3 );

   Hypertext.Text=&Text1 ;
   Hypertext2.set_pos( 1, 2, sx-2, sy-3 );
   Edit.Text=&Text1 ;
   Edit.set_name("�����");
   Edit.begin( );
   Edit.set_pos( 0, 1, sx-1, sy-2 );
   Edit.main_loop( ) ;

   return 0 ;
}
catch( int E )
{
   s_end_schone();
   return -1 ;
}
}
/***************************************************************************/
void w_create( char *Name )
{  char f ;
   char Buf[200] ;

try
{  if( Name==NULL )
	{  f=Dir.direktor( Buf,0 );  }
   else {  strcpy(Buf,Name); f=0;  }
   if( f>=0 )
   {
      if( f==0 )
	 Edit.read( Buf, KOI8 );
      Edit.set_name( Buf );
   }
   else
      w_create( "new.txt" );
   
//   paint( );
}
catch( ... )
{   //paint();
    figna( error_get() );
}
}
/***************************************************************************/
//                      ���������� MercuryDir
/***************************************************************************/
void set_mercury_dir( char *h )
{
   short i,j ;
   char Str[200] ;
   
   Strcpy( Str,Option.get("path"),200 );
   if( Str[0]!=0 )
   {  for( i=j=0 ; i<199 && Str[i]!=0 ; i++ )
      {  if( Str[i]=='~' )
         {   MercuryDir[j++]=0 ;
             if( h!=NULL ) 
	     {  Strcat( MercuryDir,h,200 ); j=strlen( MercuryDir );  }
         }
         else
             MercuryDir[j++]=Str[i] ;
      }
      MercuryDir[j++]=0 ;
   }
   else
   {
      Strcpy( MercuryDir,h,200 );
      Strcat( MercuryDir,"/prawda",200 );
   }

}
/***************************************************************************/
void w_save_as( void )
{  char f ;
   char Buf[200] ;

   f=Dir.direktor( Buf,1 );   
   if( f<0 )
      strcpy( Buf,"mercury_out.txt" );
   Edit.set_name( Buf );
   Edit.write( Buf, KOI8 );
//   paint( );
}
/***************************************************************************/
void w_save( void )
{  char Buf[200] ;
   Edit.write( Edit.Name,KOI8 );
   sprintf( Buf,"���� \"%s\" ��������",Edit.Name );
   figna( Buf );
}
/***************************************************************************/
void w_exit( void )
{
   if( 1==fragen( "��������� ���� ��������? (1-�� 0-���)"  ) ) 
       w_save( ) ;

   s_nacht( ) ;
   s_redraw( );
   s_end_schone( );
   exit(0);
}
/***************************************************************************/
void  Window_help( void *Void )
{  char *Str=(char *)Void ;
   short sx,sy ;

   s_get_size( sx,sy );
   s_save( 1,1,sy-1,sx-1 );
   HelpView.init( HelpData.get_help( Str ) );
   HelpView.main( );
   s_restore();
}
/***************************************************************************/
void  Window_menu( void *Void )
{
   char d0,d1=0 ;

   d0=((char *)Void)[0] ;
   switch( d0 )
   {
      case '1' : w_create( NULL );     break ;
      case '2' : w_save(  );           break ;
      case '3' : w_save_as(  );        break ;
      case '4' : w_exit( );            break ;
      case '5' : Edit.replace3( );     break ;
      case '6' : Edit.replace2( );     break ;
      case '7' : Edit.replace4( );     break ;     
      case '8' : Edit.goto_str( );     break ;
   }
}
/**************************************************************************/
//            ���������, ��� ������ ������ �������� ������
/**************************************************************************/
/*
char is_number( char *Str )
{
   for( short i=0 ; i<99 && Str[i]!=0 ; i++ )
      if( Str[i]<'0' || '9'<Str[i] ) return 0 ;
   return 1 ;
}
*/
