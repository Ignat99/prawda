/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                                 main()
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# include <string.h>
# include <unistd.h>
# include <time.h>
# include <ctype.h>
# include "mylib.h"
# include "video.h"
# include "core.h"
# include "menu.h"
# include "common.h"
# include "windows.h"

extern t_Menu      Main_menu[] ;
extern t_String    MercuryDir ;
// ------- места, где могут размещаться словари и пр. данные ----------
char *MercuryDir00[]={ "/usr/share/prawda",
                       "/usr/local/share/prawda/",
                        NULL
		     };
// ------- место, где они действительно размещаются -------------------
char *MercuryDir0 ;

// ------- места, где могут размещаться подсказки ----------
char *HelpDir00[]={ "/usr/share/doc/prawda/help",
                    "/usr/local/share/doc/prawda/help",
                     NULL
                  };
// ------- место, где они действительно размещаются -------------------
t_String HelpDir ;

char is_number( char *Str );
void preproc_rc( char *File );
void language_rc( char *File );
void out_format_rc( char *File );
void select_format( void *S );
void select_lang( void *S );

extern t_Kontext Kontext ;

/***************************************************************************/
void spez_figna( char *Str1 )
{  char *Str ;

   Str=Option.get("font");
   if( Str[0]!=0 ) s_set_font( Str );
   s_begin_schone( );

   figna( Str1 );
}
/***************************************************************************/
void spez_figna1( char *Str )
{  
   printf( Str );
}
/***************************************************************************/
//               есть ли более старая версия "Правды"
/***************************************************************************/
char old_version_present( char *Path, long Ver )
{
   char *Mass ;
   long Ver1 ;
   t_String Str ;
   
   Str=Path ;
   if( 0>access( Str.c_str(),R_OK ) ) return 0 ; //существует ли каталог "Правды"
   
   Str+="/version" ;
   if( 0>access( Str.c_str() ,R_OK ) ) return 1 ; //существует ли файл версий
   
   // ---- старее ли каталог данных чем программа? -----
   FILE *fr=Fopen( Str.c_str(),"r" );
   if( EOF==fscanf( fr,"%ld",&Ver1 ) ) return 1 ;
   Fclose( fr );
   if( 0<(Ver-Ver1)/100 ) return 1 ;
   
   return 0 ;
}
/***************************************************************************/
//         скопировать каталог Меркурия в домашний каталог
/***************************************************************************/
char copy_mercury_dir( char *h, char Clear, char rc_file )
{  int   r0,r1,r2,r3 ;
   char Cmd[1000] ;

   spez_figna1("\n Подготовка к первому запуску \"Правды\". Ждите...\n" );
   if( Clear )
   {   sprintf( Cmd,"rm -r %s",MercuryDir.c_str() );
       system( Cmd );
   }
   sprintf( Cmd,"cp -r %s %s",MercuryDir0,MercuryDir.c_str() );
   r1=system( Cmd );
   if( rc_file )
   {  sprintf( Cmd,"cp %s/prawdarc.local %s/.prawdarc",MercuryDir0,h );
      r2=system( Cmd );
   }
   if( r1<0 || r2<0 )
   {   spez_figna1("\n Копирование не произошло" );
       return -1 ;
   }
   return 0 ;
}
/***************************************************************************/
//                      установить MercuryDir
/***************************************************************************/
void set_mercury_dir( char *h )
{
   short i ;
   char  *Str ;
   
   Str=Option.get("path");
   if( Str[0]!=0 )
   {  MercuryDir="" ;
      for( i=0 ; i<199 && Str[i]!=0 ; i++ )
      {  if( Str[i]=='~' )
         {   MercuryDir="" ;
             if( h!=NULL ) 
	     {  MercuryDir+=h ; }
         }
         else
             MercuryDir+=Str[i] ;
      }
   }
   else
   {
      MercuryDir=h ;
      MercuryDir+="/prawda" ;
   }
}
/***************************************************************************/
//                проверки присутствия нужных данных 
//               (пример того, как НЕ НАДО писать программу)
/***************************************************************************/
void check_install( )
{   
   int  i ;
   int r0 ; // присутствует настроечный файл "~/.prawdarc"
   int r1 ; // присутствует "каталог пользователя" "~/prawda"
   int r2 ; // присутствует "исходный каталог" "/usr/share/prawda"
   int r3 ; // присутствует УСТАРЕВШИЙ "каталог пользователя" "~/prawda"
   char *h,Str[200]="" ;
      
   h=getenv( "HOME" );
   if( h!=NULL ) {  Strcpy( Str,h,200 );  }
   Strcat( Str,"/.prawdarc",200 );

   // ----- узнать, а где у нас "/usr/share/prawda" ---------
   MercuryDir0=MercuryDir00[0] ;
   for( i=0 ; MercuryDir00[i]!=NULL ; i++ )
   {  if( 0==access( MercuryDir00[i],R_OK ) )
      {   MercuryDir0=MercuryDir00[i] ; break ;  }
   }
   
   r0=access( Str,R_OK );

   if( 0==r0 ) 
   {  // присутствует настроечный файл "~/.prawdarc
      // --------- прочесть настроечный файл ----------
      int r=Option.read_all( Str );
      if( 0>r )
      {   printf("\n Файл %s отсутствует или некорректен.",Str ); exit(-1) ;  }

      set_mercury_dir( h );
      r1=access( MercuryDir.c_str(),R_OK );
      r2=access( MercuryDir0,R_OK );

      if( 0==r1 ) 
      {  // присутствует "~/prawda"
         r3=old_version_present( MercuryDir.c_str(),300 );
	 if( 1==r3 ) // присутствует УСТАРЕВШИЙ "каталог пользователя" "~/prawda"
	 {   
             if( r2<0 ) // НЕ присутствует "исходный каталог" "/usr/share/prawda"
                 goto M_OldVersion ;
	     Strcpy( Str,Option.get("font"),200 );
	     if( Str[0]!=0 ) s_set_font( Str );
	     s_begin_schone( );

             if( 1==fragen("Обнаружена старая версия Правды\n Переписать ее? 1-Да(рекомендуется) 0-Нет ") )
	        copy_mercury_dir( h, 1, 0 );
	     else goto M_OldVersion ;
	 }
      }
      else
      {  // не присутствует "~/prawda"
         if( 0==r2 ) // присутствует "исходный каталог" "/usr/share/prawda" 
	      copy_mercury_dir( h, 0, 0 ); // в каталог MercuryDir без ~/.prawdarc
	 else goto M_Dir0_Absend ;
      }
   }
   else
   {  // НЕ присутствует настроечный файл "~/.prawdarc      
      set_mercury_dir( h );
      r1=access( MercuryDir.c_str(),R_OK );
      r2=access( MercuryDir0,R_OK );
      if( 0>r2 ) // НЕ присутствует "/usr/share/prawda"
          goto M_Dir0_Absend ;
      copy_mercury_dir( h, 0==r1, 1 ); // вместе с ~/.prawdarc
      int r=Option.read( Str );
      if( 0>r )
      {   printf("\n Файл %s отсутствует или некорректен.",Str ); exit(-1) ;  }
      set_mercury_dir( h );
   }
   return ;

   M_Dir0_Absend:
      sprintf( Str,"Нет данных (словарей) в каталоге\n %s",MercuryDir0 );
      spez_figna( Str );
      exit(-1);
   M_OldVersion:
      sprintf( Str,"Версия данных в каталоге \n%s\n не соответствует версии программы\n",MercuryDir.c_str() );
      printf("\n%s",Str );
      spez_figna( Str );
      exit(-1);
}
/***************************************************************************/
void main_test( char *File )
{
   Edit1.read( File, KOI8 );
   Macrostruct.make();
   Kontext.make();
   Kontext.make_default_perevod( );
   Macrostruct2.translate_all();
   Macrostruct.print( "perevod.txt" );
}
/***************************************************************************/

/***************************************************************************/
int main( short argc, char *argv[] )
{  char  r,r1 ;
   char     *str ;
   t_String  Str ;
   short sx=80,sy=50 ;

try
{
   check_install( );

 
   // --------- установить размеры х у -------------
   str=Option.get("size_x");
   if( is_number( str ) && str[0]!=0 ) sx=atoi(str);
   str=Option.get("size_y");
   if( is_number( str ) && str[0]!=0 ) sy=atoi(str);
   // --------- установить руссификацию ------------
   str=Option.get("rus");
   if( str[0]!=0 && 0!=strcmp(str,"No") )
   {  short Komb=0 ;
      if( NULL!=strstr(str,"Shift_R") ) Komb|=S_Shift_R ;
      if( NULL!=strstr(str,"Shift_L") ) Komb|=S_Shift_L ;
      if( NULL!=strstr(str,"Ctrl_R")  ) Komb|=S_Ctrl_R ;
      if( NULL!=strstr(str,"Ctrl_L")  ) Komb|=S_Ctrl_L ;
      if( NULL!=strstr(str,"Alt_R")  )  Komb|=S_Alt_R ;
      if( NULL!=strstr(str,"Alt_L")  )  Komb|=S_Alt_L ;
   
      s_set_ruskomb( Komb );   
   }   
   // --------- установить шрифт -------------------
   str=Option.get("font");
   if( str[0]!=0 ) s_set_font( str );
   s_begin_schone( );
   s_set_size( sx,sy );
   s_nacht( );

   // --------- установить HelpDir -----------------
   HelpDir= MercuryDir ;
   HelpDir+="/doc/help" ;
   for( long i=0 ; HelpDir00[i]!=NULL ; i++ )
   {  if( 0==access( HelpDir.c_str(),R_OK ) ) break ;
      HelpDir=HelpDir00[i] ;
   }

   // --------- установить направления перевода ----
   Str= MercuryDir ;
   Str+="/language.rc" ;
   language_rc( Str.c_str() );

   // --------- установить выходной формат ---------
   Str= MercuryDir ;
   Str+="/format.rc" ;
   out_format_rc( Str.c_str() );
   if( 0<n_DirFormat )
        select_format( &DirFormat[0] );

   // --------- собственно работа ------------------
   WW_init( );
   WW_set_pos( );

# ifndef __WIN__
   HelpData.make();
# endif

   // ------ если prawda -lang <язык> то переводить тестовый файл ------
   for( short i=0 ; i<argc ; i++ )
      if( 0==strcmp( argv[i],"-lang" ) && i<argc-1 )
      {   for( short i1=0 ; i1<argc ; i1++ )
             if( 0==strcmp( DirTrans[i1].Name, argv[i+1]) )
	     {   select_lang( &DirTrans[i1] ); break ;  }
          goto M_LangSelected ;
      }
   select_lang( &DirTrans[0] );
M_LangSelected:

   // ------ если prawda -test <файл> то переводить тестовый файл ------
   for( short i=0 ; i<argc ; i++ )
      if( 0==strcmp( argv[i],"-test" ) && i<argc-1 )
      {   main_test( argv[i+1] ); exit(0);  }
   
   // ------ показать надоедающую подсказку ----------------------------
   if( 0==strcmp( "Yes",Option.get("RTFM") ) )
   {   time_t      tm0=time(NULL);
       struct tm  *tm1=localtime( &tm0 );
       char        Str2[40] ;
       sprintf( Str2,"tip%d.html",1+(tm1->tm_yday)%6 );

       null_menu( Main_menu );
       Window_help( Str2 );
   }
   if( 0==strcmp( "Yes",Option.get("Figna") ) )
       Core.f_Figna=1 ;
   null_menu( Main_menu );
   
   // ------ главный цикл работы ----------------------
   WW_trans_besser( NULL );

   return 0 ;
}
catch( int E )
{
   s_end_schone();
   return -1 ;
}
}
