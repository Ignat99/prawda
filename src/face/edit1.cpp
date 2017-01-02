/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                      ��������, ���������� ��� ������� 
//     �������� �.�.                                         �������
/***************************************************************************/

# include <stdio.h>
# include <malloc.h>
# include <string.h>

# include "mylib.h"
# include "video.h"
# include "common.h"
# include "menu.h"
# include "edit1.h"
# include "macrostruct.h"
# include "windows.h"

void  format_make( void *V );
void  format_clr ( void *V );
void  format_frasa ( void *V );
void  format_absatz( void *V );

/* ----------------------------------------------------------------------- */
extern t_Menu file_menu1[] ;
t_Menu file_menu[] =
{
   { F_FUNK  ,0,WW_create    ,(void *)"k" ,"�������          " },
   { F_FUNK  ,0,WW_save_src  ,(void *)"k" ,"��������� ��������" },
   { F_SCHREI,0,NULL         ,NULL        ,"��������� �������" },
   { F_FUNK  ,0,WW_save_as   ,(void *)"k" ,"��� ������       " },
   { F_SCHREI,0,NULL         ,NULL        ,"-----------------" },
   { F_FUNK  ,0,WW_create    ,(void *)"w" ,"�������    cp1251" },
   { F_FUNK  ,0,WW_save_src  ,(void *)"w" ,"����. ����.cp1251" },
   { F_SCHREI,0,NULL         ,NULL        ,"��������� �������" },
   { F_FUNK  ,0,WW_save_as   ,(void *)"w" ,"��� ������ cp1251" },
   { F_SCHREI,0,NULL         ,NULL        ,"-----------------" },
   { F_FUNK  ,0,WW_read_langtxt ,NULL     ,"�������� lang.txt" },
   { F_FUNK  ,0,WW_read_alldata ,NULL     ,"�������� ��� �����"},
   { F_NUMER ,0,NULL         ,(void *)"x" ,"�����            " },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};
t_Menu edit_menu[] =
{  { F_FUNK  ,0,WW_replace3  ,NULL   ,"�����-������       F3" },
   { F_FUNK  ,0,WW_goto_str  ,NULL   ,"������ �����         " },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};
t_Menu *lang_menu = NULL ;
t_Menu trans_menu[] =
{  { F_FUNK  ,0,WW_trans          ,NULL   ,"������� �����  F7" },
   { F_FUNK  ,0,Sorry             ,NULL   ,"������� �����    " },
   { F_FUNK  ,0,WW_trans_frasa    ,NULL   ,"������� �����  F8" },
   { F_FUNK  ,0,WW_trans_bilingvo ,NULL   ,"������� �������� " },
   { F_ENDE  ,0,NULL              ,NULL   ,"" }
};
t_Menu window_menu[] =
{  { F_NUMER ,0,NULL         ,(void *)"1"   ,"�������� �����           " },
   { F_NUMER ,0,NULL         ,(void *)"2"   ,"����� ��������           " },
   { F_NUMER ,0,NULL         ,(void *)"s"   ,"�������                F2" },
   { F_NUMER ,0,NULL         ,(void *)"b"   ,"�������                  " },
   { F_NUMER ,0,NULL         ,(void *)"c"   ,"�������2               F9" },
   { F_NUMER ,0,NULL         ,(void *)"e"   ,"������ ����������        " },
   { F_SCHREI,0,NULL         ,NULL          ,"-------------------------" },
   { F_NUMER ,0,NULL         ,(void *)"3"   ,"������ ������       ^S F2" },
   { F_NUMER ,0,NULL         ,(void *)"4"   ,"��������� ��������� ^S F3" },
   { F_NUMER ,0,NULL         ,(void *)"5"   ,"��������� ��������� ^S F4" },
   { F_NUMER ,0,NULL         ,(void *)"6"   ,"��������            ^S F5" },
   { F_NUMER ,0,NULL         ,(void *)"f"   ,"�������� ����������������" },
   { F_NUMER ,0,NULL         ,(void *)"7"   ,"���� ����������          " },
   { F_NUMER ,0,NULL         ,(void *)"8"   ,"���� ��������            " },
   { F_NUMER ,0,NULL         ,(void *)"9"   ,"������� ������� ���������" },
   { F_NUMER ,0,NULL         ,(void *)"a"   ,"������� �������          " },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};
t_Menu help_menu[] =
{  { F_FUNK  ,0,Window_help  ,(void *)"help.html"     ,"� ���������  " },
   { F_FUNK  ,0,Window_help  ,(void *)"index.html"    ,"����������   " },
   { F_FUNK  ,0,Window_help  ,(void *)"howto.html"    ,"��� ������   " },
   { F_FUNK  ,0,Window_help  ,(void *)"menu.html"     ,"����         " },
   { F_FUNK  ,0,Window_help  ,(void *)"windows.html"  ,"������       " },
   { F_FUNK  ,0,Window_help  ,(void *)"edit.html"     ,"��������     " },
   { F_FUNK  ,0,Window_help  ,(void *)"path.html"     ,"�������      " },
   { F_FUNK  ,0,Window_help  ,(void *)"first.html"    ,"������ ������" },
   { F_FUNK  ,0,Window_help  ,(void *)"src.html"      ,"���������    " },
   { F_FUNK  ,0,Window_help  ,(void *)"debug.html"    ,"��������     " },
   { F_FUNK  ,0,Window_help  ,(void *)"grammar.html"  ,"���� ����������" },
   { F_FUNK  ,0,Window_help  ,(void *)"slowo1.html"   ,"������� �����."  },
   { F_FUNK  ,0,Window_help  ,(void *)"base_enru.html","������� �������" },
   { F_FUNK  ,0,Window_help  ,(void *)"tip0.html"     ,"�������� ������" },
   { F_FUNK  ,0,Sorry        ,(void *)"���������"     ,"���������" },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};
t_Menu Main_menu[] =
{  { F_MENU  ,0,NULL,file_menu1  ,"�����"    },
   { F_MENU  ,0,NULL,edit_menu   ,"��������" },
   { F_SCHREI,0,NULL,NULL        ,"�����������" },
   { F_MENU  ,0,NULL,trans_menu  ,"�������"  },
   { F_MENU  ,0,NULL,window_menu ,"������"   },
   { F_MENU  ,0,NULL,help_menu   ,"���������"},
   { F_ENDE  ,0,NULL,NULL        ,"" }
};
// ----------- ���� ��� �������������� -------------------------------------
extern t_Menu file_menu[] ;
extern t_Menu edit_menu[] ;

t_Menu format_menu[] =
{  { F_FUNK  ,0,format_make  ,NULL   ,"�������  �ӣ" },
   { F_FUNK  ,0,format_clr   ,NULL   ,"������   �ӣ" },
   { F_FUNK  ,0,format_frasa ,NULL   ,"����� ����� " },
   { F_FUNK  ,0,format_absatz,NULL   ,"����� ������" },
   { F_ENDE  ,0,NULL,NULL        ,"" }
};

t_Menu format_help[] =
{  { F_FUNK  ,0,Window_help  ,(void *)"index.html"    ,"����������    " },
   { F_FUNK  ,0,Window_help  ,(void *)"m_format.html" ,"�� ���� ������" },
   { F_ENDE  ,0,NULL         ,NULL  ,"" }
};

static t_Menu Format_menu[] =
{  { F_MENU  ,0,NULL,file_menu1  ,"�����"    },
   { F_MENU  ,0,NULL,edit_menu   ,"��������" },
   { F_MENU  ,0,NULL,format_menu ,"��������������" },
   { F_MENU  ,0,NULL,format_help ,"���������"},
   { F_ENDE  ,0,NULL,NULL        ,"" }
};

extern t_Macrostruct Macrostruct ;
extern t_Kontext     Kontext ;
extern char          hk_Format[] ;
char f_do=0 ; // ���� ������� �������� ;

char data_obsolete( );

/***************************************************************************/
t_EditTrans :: t_EditTrans( void ) : t_Edit()
{
   Name[0]=0 ;
   set_text( &Text1 );
}
/***************************************************************************/
t_EditTrans :: ~t_EditTrans( void )
{
   ;
}
/***************************************************************************/
void t_EditTrans :: set_name( char *S )
{  strcpy(Name,S) ;
}
/***************************************************************************/
//                        ���� ��������� ������
/***************************************************************************/
e_WinMsg t_EditTrans :: main_loop( void )
{  
   ushort   ZZ,key1,key2 ; // ������� ������� 
   short    Funk ;
   e_WinMsg r ;

   null_menu( Main_menu );
   while( 1 )
   {
      paint( );

      s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( key1==S_key_TabR ) return WM_NEXT ;

      if( (ZZ&(S_Shift_L|S_Shift_R))!=0 )
      {	  // -------- Shift ������� ----------------
	  switch( key2 )
	  {
	     case S_key_F2 : return WM_FIRST ;
	     case S_key_F3 : return WM_SRC ;
	     case S_key_F4 : return WM_DST ;
	     case S_key_F5 : return WM_DEBUG ;
	     case S_key_F6 : return WM_DEBUG1 ;
	  }	  
      }
      else
      {  // -------- Shift ������� ----------------
	 if( key1==0 )
	 {  switch( key2 )
	    {  case S_key_F1 : Window_help( (void *)"edit.html") ; continue ;
	       case S_key_F2 : return WM_SLOWO1 ;
	       case S_key_F7 : WW_trans( NULL ); continue ;
	       case S_key_F8 : WW_trans_frasa( NULL ); continue ;		  
	    }
	 }
      }

      r=loop( ZZ,key1,key2 );
      if( r!=WM_NULL1 ) return r ;
      
      Funk=translate( ZZ,key1,key2 );
      switch( Funk )
      {   /*------------ ��������� ������� -------------*/
	  case 0           : 
	  case F_Paste     :
	  case F_Cat       :
	  case F_IncCopy   :
	  case F_ShiftLeft :
	  case F_ShiftRight:
	  case F_Delete    : put_str( ); break ;
      }
   }
}
/***************************************************************************/
//                        ���� ������ ��������������
/***************************************************************************/
# define DO if( data_obsolete() ) continue ;

e_WinMsg t_EditTrans :: loop_format( void )
{  
   ushort   ZZ,key1,key2 ; // ������� �������
   short    Funk ;
   e_WinMsg r ;
   e_sEvent Event ;
   short    r1 ;
   
   f_do=0 ;

   while( 1 )
   {
      paint_header( "�������������� ������" );
      paint( );
      null_menu( Format_menu,1 );
      paint_hotkey( n_Line-1, 0, hk_Format );

      Event=s_getch16( &key1,&key2 ) ;
      ZZ   =s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short button,mouse_x,mouse_y ;
          button=s_get_mouse( &mouse_x, &mouse_y );

	  if( 3==button )  goto M_Grenze;
	  if( 4==button ){   loop( S_Ctrl_L,0,S_key_Up   ); continue ;  }
	  if( 5==button ){   loop( S_Ctrl_L,0,S_key_Down ); continue ;  }	  
	  if( 1==mouse_y )
 	  {  r1=menu_mouse( Format_menu,1 ,mouse_x );
	     if( 'x'==r1 ) return WM_ESC ;
          }
	  if( n_Line-1==mouse_y )
	      Option.get_key( &ZZ, &key1, &key2, hk_Format, mouse_x );
          if( x1<mouse_x && mouse_x<x2 && y1<mouse_y && mouse_y<y2 )
	  {   
	      PX=Begin_X+mouse_x-x1-1 ;
	      PY=Begin_Y+mouse_y-y1-1 ;
              raden_Y( PY ) ;
	  }
      }

      if( (ZZ&(S_Shift_L|S_Shift_R))==0 )
      {  // -------- Shift ������� ----------------
	 if( key1==S_key_Esc ) goto M_Menu ;
	 if( key1==0 )
	 {  switch( key2 )
	    {  case S_key_F1 :    Window_help( (void *)"m_format.html") ; continue ;
	       case S_key_F4 : DO format_clr();   continue ;
	       case S_key_F5 : DO format_make();  continue ;
	       case S_key_F7 : DO format_frasa(); continue ;
	       case S_key_F8 : DO format_absatz();continue ;
               case S_key_F9 : M_Menu :
	                       r1=menu( Format_menu,1 ); 
	                       if( 'x'==r1 ) return WM_ESC ;
	                       continue ;
	       case S_key_F10: return WM_ESC ;
	    }
	 }
      }
      if( (ZZ&(S_Ctrl_L|S_Ctrl_R))!=0 )
      {  // --------  Ctrl ����� ------------------
         switch( key2 )
M_Grenze:{   case S_key_Ins :
	          DO 
	          if( Str[PX].Flag==0 )
		  {   Str[PX].Flag=T_FRASAEND ; continue ;  }
	          if( Str[PX].Flag==T_FRASAEND )
		  {   Str[PX].Flag=T_ABSATZEND ; continue ;  }
	          if( Str[PX].Flag==T_ABSATZEND )
		  {   Str[PX].Flag=0 ; continue ;  }
	 }
      }

      Funk=translate( ZZ,key1,key2 );
      switch( Funk )
      {
	 case F_Cat:    case F_Copy:   case F_IncCopy:
	 case F_Paste:  case F_Insert: case F_BackSpase:
	 case F_Delete: case F_DelStr: case F_ShiftRight:
	 case F_ShiftLeft: case F_Nop:
              DO ;
	      break ;
      }

      r=loop( ZZ,key1,key2 );
      if( r!=WM_NULL1 ) return r ;

      switch( Funk )
      {   /*------------ ��������� ������� -------------*/
	  case 0           : 
	  case F_Paste     :
	  case F_Cat       :
	  case F_IncCopy   :
	  case F_ShiftLeft :
	  case F_ShiftRight:
	  case F_Delete    : put_str( ); break ;
      }
   }
}
/***************************************************************************/
void  format_make( void *V )  {   Edit1.format_make();  }
void  format_clr ( void *V )  {   Edit1.format_clr();   }
void  format_frasa ( void *V ){   Edit1.format_frasa(); }
void  format_absatz( void *V ){   Edit1.format_absatz();}
/***************************************************************************/
void t_EditTrans :: format_make()
{
   Macrostruct.automake1();
   get_str( );
}
/***************************************************************************/
void t_EditTrans :: format_clr()
{
   Macrostruct.clear();
   get_str( );
}
/***************************************************************************/
void t_EditTrans :: format_frasa()
{
   if( 0==Str[PX].Flag )  // ---- ���������-����� ����� �����
	    Str[PX].Flag=T_FRASAEND ;
       else Str[PX].Flag=0 ;
}
/***************************************************************************/
void t_EditTrans :: format_absatz()
{
 if( 0==Str[PX].Flag ) // ---- ���������-����� ����� ������
	  Str[PX].Flag=T_ABSATZEND ;
     else Str[PX].Flag=0 ;
}
/***************************************************************************/
//                      ���� ������ �������� �����
/***************************************************************************/
e_WinMsg t_EditTrans :: loop_frasa( void )
{  
   ushort   ZZ,key1,key2 ; // ������� �������
   short    Funk ;
   e_WinMsg r ;
   e_sEvent Event ;

   while( 1 )
   {
      paint( );
      paint_hotkey( n_Line-1, 0, hk_FrasaEdit );

      Event=s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   short x,y ;
          s_get_mouse( &x, &y );
          if( (y<y1 || y2<y) && y!=n_Line-1 ) 
	      return WM_NEXT ;
	  if( y==n_Line-1 )
	      Option.get_key( &ZZ, &key1, &key2, hk_FrasaEdit, x );
//	  continue ;
      }

      switch( key1 )
      {	 case S_key_TabR: return WM_NEXT ;
      }
      if( (ZZ&(S_Shift_L|S_Shift_R))==0 )
      {  // -------- Shift ������� ----------------
	 switch( key2 )
	 {  case S_key_F1 : Window_help( (void *)"frasa.html") ; continue ;
	    case S_key_F2 : case S_key_F3 : case S_key_F4 : case S_key_F5 :
	    case S_key_F6 : case S_key_F7 : case S_key_F8 : case S_key_F9 : continue ;	    
	    case S_key_F10: return WM_ESC ;
	 }
      }

      r=loop( ZZ,key1,key2 );
      if( r!=WM_NULL1 ) return r ;
      
      Funk=translate( ZZ,key1,key2 );
      switch( Funk )
      {   /*------------ ��������� ������� -------------*/
	  case 0           : 
	  case F_Paste     :
	  case F_Cat       :
	  case F_IncCopy   :
	  case F_ShiftLeft :
	  case F_ShiftRight:
	  case F_Delete    : put_str( ); break ;
      }
   }
}
/***************************************************************************/
e_WinMsg t_EditTrans :: loop_help( char *File )
{  
   ushort   ZZ,key1,key2 ; // ������� �������
   short    Funk ;
   short    i,j_History,button,mouse_x,mouse_y ;
   t_String Path ;
   e_sEvent Event ;
   static   t_String History[10] ;

   History[j_History=0]=File ;
   init( HelpData.get_help( File ) );

   while( 1 )
   {
      paint( );
      paint_hotkey( n_Line-1, 0, hk_Help );

      Event=s_getch16( &key1,&key2 ) ;
      ZZ=s_shiftstatus();

      if( MOUSE_PRESS==Event )
      {   button=s_get_mouse( &mouse_x, &mouse_y );
	  if( mouse_y==n_Line-1 )
	      Option.get_key( &ZZ, &key1, &key2, hk_Help, mouse_x );
	  if( 4==button ){   loop( S_Ctrl_L,0,S_key_Up   ); continue ;  }
	  if( 5==button ){   loop( S_Ctrl_L,0,S_key_Down ); continue ;  }	  
          if( x1<mouse_x && mouse_x<x2 && y1<mouse_y && mouse_y<y2 )
	  {
	      PX=Begin_X+mouse_x-x1-1 ;
	      PY=Begin_Y+mouse_y-y1-1 ;
              raden_Y( PY ) ;
	      ZZ=0 ; key1='\r' ; key2=0 ;
	  }
      }

      Funk=translate( ZZ,key1,key2 );

      switch( key2 )
      {
          case S_key_F2 :
	       init( HelpData.get_help( "index.html" ) );
 	       Begin_Y=Begin_X=PY=PX=0 ;
	       continue ;
          case S_key_F3 :
	       if( 0<j_History )
	       {   init( HelpData.get_help( History[--j_History].c_str() ) );
		   Begin_Y=Begin_X=PY=PX=0 ;
	       }
	       continue ;
      }

      switch( Funk )
      {
         case 0 :
	      switch( key1 )
	      {	 
		 case '\r': // ---- ������� �� ������ ����� -------
		      for( i=PX ; 0<=i && Str[i].Char!=0x200 ; i-- )
			 if( Str[i].Char==0x100 ) goto M_No_Link ;
		      if( i<0 ) goto M_No_Link ;
		      Path="" ;
		      for( i++ ; i<256 && Str[i].Char!=0x100 ; i++ )
			 Path+=Unicod_to_koi8(Str[i].Char);
		      init( HelpData.get_help( Path.c_str() ) );
		      Begin_Y=Begin_X=PY=PX=0 ;
		      
		      if( j_History<9 )
		          History[++j_History]=Path ;
		      else
		      {   for( i=0 ; i<9 ; i++ )
			     History[i]=History[i+1];
		          History[9]=Path ;
		      }
	M_No_Link :   break ;	  
		 case S_key_Esc: return WM_ESC ;
		 default :       break ;
	      }
	      break ;
         case F_Paste:  case F_Delete:  case F_Copy:     case F_ShiftLeft:
	 case F_Cat:    case F_Insert:  case F_IncCopy:  case F_ShiftRight:	 
	      break ;
         case F_Menu: return WM_ESC ;
	 default:
	      loop( ZZ, key1, key2 );
      }
   }
}
/***************************************************************************/
long  t_EditTrans :: current_pos( void )
{  return Text->pos_str(PY)+PX ;  }
/***************************************************************************/
char* t_EditTrans :: get_frasa( long i )
{  return Text->get_frasa( i );  }
/***************************************************************************/
char* t_EditTrans :: get_word( long i )
{  return Text->get_word_punkt( i );  }
/***************************************************************************/
//      ���������� ������ � ������� put_frasa()
/***************************************************************************/
void  t_EditTrans :: put_frasa32( long p, t_myChar *Frasa )
{  long     i,z ;
   t_myChar c ;
   t_myChar Strtmp[1000] ;

   for( i=z=0 ; Frasa[i]!=0 ; i++ )
   {  c=Frasa[i] ;
      if( 80<=(i-z) && (c==' ' || c=='\t') )
          Frasa[i]='\n' ;
      if( Frasa[i]=='\n' ) z=i ;
   }
   if( 990 <strlen32(Frasa) )
       Frasa[990]=0 ;
   strcpy32( Strtmp,Frasa );
   
   Text->put_frasa32( p,Strtmp );
   get_str();
}
/***************************************************************************/
void  t_EditTrans :: put_frasa( long p, char *Frasa )
{   static t_myChar Str[1000] ;

    koi9_to_myChar( Frasa, Str, 1000 );
    put_frasa32( p, Str );
}
/***************************************************************************/
//      ������� ��� "�������� �������" - ����/���� �����/�����
/***************************************************************************/
void  t_EditTrans :: step( short flag )
{
   long p,p1 ;
   long py,px ;

   p=current_pos();
   
   if( flag==0 ) p1=Text->next_frasa( p );
   if( flag==1 ) p1=Text->prev_frasa( p );
   if( flag==2 ) p1=Text->next_word( p );
   if( flag==3 ) p1=Text->prev_word( p );
   
   Text->get_xy_pos( p1,&py,&px );
   PY=py ; PX=px ;
   raden_Y( PY ) ;
   raden_X( PX ) ;
   raden_y3( );
}
/***************************************************************************/
void  t_EditTrans :: next_frasa( void ){  step(0) ;  }
void  t_EditTrans :: prev_frasa( void ){  step(1) ;  }
void  t_EditTrans :: next_word( void ) {  step(2) ;  }
void  t_EditTrans :: prev_word( void ) {  step(3) ;  }
/***************************************************************************/
void  t_EditTrans :: set_funk_const( char (*is_const)( char *Str ) )
{
   //Text1.is_const=is_const ;
}
/***************************************************************************/
//            ���������� ����� ������ (������� �����)
/***************************************************************************/
void t_EditTrans :: set_thisfrasa( long begin, long end )
{
   for( long i=begin ; i<end ; i++ )
      if( Text->text(i).Flag==0 )
          Text->text(i).Flag=T_THISFRASA ;
}
/***************************************************************************/
//
/***************************************************************************/
void t_EditTrans :: clr_thisfrasa( long begin, long end )   
{
   for( long i=begin ; i<end ; i++ )
      if( Text->text(i).Flag==T_THISFRASA )
          Text->text(i).Flag=0 ;
}
/***************************************************************************/
// ������������ �� "�������" �������� ������������ t_Kontext � t_Macrostruct
//  1 - ���������� �� ��������
/***************************************************************************/
char data_obsolete( )
{   
   if( f_do==1 ) return 0 ;
   if( Macrostruct.exist==0 && Kontext.exist==0 ) return 0 ;
   char *Str="�������������� ������ �������� � ����\n"
             "��� ����������� ������� ��������.\n"
	     "����������? 1-�� 0-���" ;
   if( 1==fragen( (char *)Str ) ) 
   {  f_do=1 ;
      Macrostruct.del() ;
      Kontext.del() ;
      return 0 ;  
   }
   return 1 ;
}
