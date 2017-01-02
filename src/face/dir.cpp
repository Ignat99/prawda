/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                            ЛАЗАЛКА ПО ДИСКУ
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# include <time.h>
# include <dirent.h>
# ifdef __WIN__
   # include <dir.h>
#else
   # include <unistd.h>
#endif

# include <sys/types.h>
# include <sys/stat.h>

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "mylib.h"
# include "dir1.h"
# include "option.h"
# include "common.h"
# include "video.h"
# include "window.h"
# include "grammar.h" // для t_DirTrans 

# ifdef __WIN__
   #define SLASH   '\\'
   #define SLASH1  "\\"
# else
   #define SLASH   '/'
   #define SLASH1  "/"
# endif
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

/*
   Дальнейшее движение
   1) Окошки, которые рисуют текущие значения
   2) Выпадающие списочки функции
      1) заполняют списочки
      2) организуют цикл обработки
   3) Горячие клавиши, которые направляют поток управления в окошки
   4) области, которые направляют поток в окошки
   --------------------------------------------------------
   3) Узкие места и переполнения
      3) Длинные имена файлов (не знаю как сделать)
*/

char   schablon( char *s, char *n );
char   name_test( char *str );
void   name_zerstoren( char *sou, char *dst1, char *dst2 );
char   schablon1( char *s, char *n);

extern t_DirTrans    *DirFormat ;
extern short          n_DirFormat ;

t_Rect RPath  ={  0, 1, 79, 3 };
t_Rect RFile  ={  0, 4, 50,25 };
t_Rect RDisk  ={ 51, 4, 79, 6 };
t_Rect RShabl ={ 51, 7, 79, 9 };
t_Rect RNew   ={ 51,10, 79,12 };
t_Rect RCode  ={ 51,19, 79,21 };
t_Rect RFormat={ 51,22, 79,24 };

t_View Code[]=
{
   { 0,"  koi8r" },
   { 1," cp1251" },
   { 2,"Latin_1" },
   { 3,"Latin_2" },
   { 4,"Latin_3" },
   { 5,"Latin_4" },
};

static t_EditLine1      EditLine ;
static t_ViewListSelect ListSelect ;
/***************************************************************************/
t_Dir :: t_Dir()
{
    i_Code=0 ; 
    i_Format=0 ;
    f_CFNeed=0 ;
    strcpy( shabl,"*.*" ) ;
/*
    getcwd( Path0,PATH_MAX );
    Path=Path0 ;
    if( SLASH!=Path0[strlen(Path0)-1] ) 
	Path+=SLASH1 ;
*/
}
/***************************************************************************/
void t_Dir :: paint( char fsave, char Reg )
{
    paint_karkas( fsave,Reg ) ;
    paint_file( Reg!=5 );
    paint_schabl();
    if( f_CFNeed )
    {  s_text_yxf( RCode.y1+1  , RCode.x1+1  , text_Color,emptystr(24) ) ;
       s_text_yxf( RCode.y1+1  , RCode.x1+1  , text_Color,Code[i_Code].Str ) ;
       s_text_yxf( RFormat.y1+1, RFormat.x1+1, text_Color,emptystr(24) ) ;
       s_text_yxf( RFormat.y1+1, RFormat.x1+1, text_Color,DirFormat[i_Format].Name ) ;
    }
}
/***************************************************************************/
//         главный цикл диалога
//    Antwort - возвращаемое имя файла
//    fsave   - если 0 - чтение 1 - запись
//    return ;
/***************************************************************************/
short t_Dir :: main_loop( t_String &Antwort, char fsave )
{   short reg ;
    static char f=0 ;
    char   Path0[PATH_MAX+2] ;

    RFile.y2=n_Line-1 ;
    Antwort="" ;
    if( f==0 )
    {  
        i_Code=0 ; i_Format=0 ;
        getcwd( Path0,PATH_MAX );
	Path=Path0 ;
	if( SLASH!=Path0[strlen(Path0)-1] ) 
	    Path+=SLASH1 ;
        f=1 ;
    }

    paint_schabl( );

    reg=5 ;  /* -------------------------------- цикл работы ---- */
    while( 1 )
    {
       paint( fsave,reg ) ;
       help( ) ;
       switch( reg )
       {  case  3 : reg=loop_schabl( ); break ;
	  case  4 : reg=loop_name( );
	            if( reg==10 )
		    {   Antwort=Path ;
		        Antwort+=Name ;
		        goto M_Ausgang ;
		    }
		    break ;
	  case  5 : reg=loop_file( );
		    if( reg==-1 ) goto M_Ausgang ;
		    if( reg==0 )
		    {  Antwort=Path ;
		       Antwort+=File[i_File].name ;
		       goto M_Ausgang ;
		    }
		    break ;
          case  6 : reg=loop_code(); break ;
          case  7 : reg=loop_format(); break ;
	  default : reg=5 ;
       }
    }
M_Ausgang :
   s_nacht( ) ;
   return reg ;
}
/***************************************************************************/
void t_Dir :: set_schabl( char *str )
{   strcpy( shabl,str ) ;  }
/***************************************************************************/
void t_Dir :: get_schabl( char *str )
{  strcpy( str,shabl ) ;  }
/***************************************************************************/
short t_Dir :: loop_schabl( void )
{
   e_WinMsg r ;
   ushort   ZZ,key1,key2 ;
   e_sEvent Event ;

   EditLine.set_text( shabl );
   EditLine.set_pos( 51,RShabl.y1,79 );

   while( 1 )
   {
       EditLine.paint();
       Event=s_getch16( &key1,&key2 );
       ZZ=s_shiftstatus();
       if( MOUSE_PRESS==Event )
       {   short mouse_x,mouse_y ;
           s_get_mouse( &mouse_x, &mouse_y );

	   if( RNew   .inside( mouse_x, mouse_y ) ) return 4 ;
           if( RFile  .inside( mouse_x, mouse_y ) ) return 5 ;
	   if( f_CFNeed )
           {   if( RCode  .inside( mouse_x, mouse_y ) ) return 6 ;
               if( RFormat.inside( mouse_x, mouse_y ) ) return 7 ;
           }
       }
       if( KEY_PRESS==Event || KEY_RELEASE==Event )
       {
           switch( key2 )
	   {   case S_key_F4 : return 4 ;
	       case S_key_F6 : if( f_CFNeed ) return 6 ;
	       case S_key_F7 : if( f_CFNeed ) return 7 ;
           }
	   if( key1=='\r' ) 
           {   char *cStr=EditLine.get_ctext( );   
               if( 0==name_test( cStr ) ) strcpy( shabl,cStr );
	       return 5 ;
           }
	   if( key1==S_key_Esc ) goto M_Exit ;
	   r=EditLine.loop( ZZ, key1, key2 );
           if( r!=WM_NULL1 ) goto M_Exit ;
       }
   }
M_Exit:
   s_text_yxf( 8,52,text_Color,emptystr(26) ) ;
   paint_schabl( );
   return 5 ;
}
/***************************************************************************/
void t_Dir :: paint_schabl( void )
{
   s_text_yxf(  8,52  ,text_Color,emptystr(24) ) ;
   s_text_yxf1( 8,52  ,text_Color,shabl ) ;
}
/***************************************************************************/
//       return - номер режима
//                если return==10 значит введено новое имя
/***************************************************************************/
short t_Dir :: loop_name( void )
{
   e_WinMsg r ;
   ushort   ZZ,key1,key2 ;
   e_sEvent Event ;

   EditLine.set_text( "" );
   char *Extention=strstr(shabl,"." );
   if( 0!=strcmp(shabl,"*.*") && 0!=strcmp(shabl,"*") && NULL!=Extention )
       EditLine.set_text( Extention );

   EditLine.set_pos( 51,RNew.y1,79 );

   while( 1 )
   {
       EditLine.paint();
       Event=s_getch16( &key1,&key2 );
       ZZ=s_shiftstatus();
       if( MOUSE_PRESS==Event )
       {   short mouse_x,mouse_y ;
           s_get_mouse( &mouse_x, &mouse_y );

	   if( RShabl .inside( mouse_x, mouse_y ) ) return 3 ;
           if( RFile  .inside( mouse_x, mouse_y ) ) return 5 ;
	   if( f_CFNeed )
           {   if( RCode  .inside( mouse_x, mouse_y ) ) return 6 ;
               if( RFormat.inside( mouse_x, mouse_y ) ) return 7 ;
           }
       }
       if( KEY_PRESS==Event || KEY_RELEASE==Event )
       {
           switch( key2 )
	   {   case S_key_F3 : return 3 ;
	       case S_key_F6 : if( f_CFNeed ) return 6 ;
	       case S_key_F7 : if( f_CFNeed ) return 7 ; 
           }
	   if( key1=='\r' ) 
           {   char *cStr=EditLine.get_ctext( );   
               if( 0==name_test( cStr ) ) strcpy( Name,cStr );
	       return 10 ;
           }
	   if( key1==S_key_Esc ) goto M_Exit ;
	   r=EditLine.loop( ZZ, key1, key2 );
           if( r!=WM_NULL1 ) goto M_Exit ;
       }
   }
M_Exit:
   s_text_yxf( 11,52,red_rame_Color,emptystr(26) ) ;
   return 5 ;
}
/***************************************************************************/
short t_Dir :: loop_code( void )
{  short i ;

   ListSelect.clr();
   for( i=0 ; i<6 ; i++)
      ListSelect.add( Code[i] );

   ListSelect.set_pos( RCode.x1+1,RCode.y1+1,RCode.x2-1,RCode.y1+6 );
   ListSelect.size_y=6 ;
   i=ListSelect.loop();
   if( 0<=i ) i_Code=i ;
   return 5 ;
}
/***************************************************************************/
short t_Dir :: loop_format( void )
{  short i ;

   ListSelect.clr();
   for( i=0 ; i<n_DirFormat ; i++)
   {  t_View V ;
      V.index=i ;
      strcpy( V.Str,DirFormat[i].Name );
      ListSelect.add( V );
   }
   ListSelect.set_pos( RFormat.x1+1,RFormat.y1+1,RFormat.x2-1,RFormat.y1+n_DirFormat );
   ListSelect.size_y=n_DirFormat ;
   i=ListSelect.loop();
   if( 0<=i ) i_Format=i ;
   return 5 ;
}
/***************************************************************************/
char name_test( char *str )
{  short i,p,l ;
   char  c ;

   p=l=strlen( str ) ;
//   if( l>12 ) goto M1 ;         // ограниченя на ДОСовские файлы
   for( i=0 ; i<l ; i++ )
      if( str[i]=='.' ){ p=i ; break ; }
//   if( p>8 || l-p>4 ) goto M1 ; // ограниченя на ДОСовские файлы
   for( i=0 ; i<p ; i++ )
   {  c=str[i] ;
      if( !(('!'<=c && c<='&') || ('0'<=c && c<='9')
	 || ('?'<=c && c<='Z') || ('a'<=c && c<='z')
	 || c=='*' || c=='_' || c=='.' ) )
       return -1 ;
   }
   return 0 ;
}
/***************************************************************************/
short t_Dir :: loop_file( void )
{  short     i ;
   static    ushort taste1,taste2 ;
   e_sEvent  Event ;
   t_String  Path1 ;

Path1=Path ;
read_file( Path ) ;

Begin=i_File=0 ;
while( 1 )
{   paint_file( 0 ) ;

    Event=s_getch( &taste1,&taste2 );

    if( MOUSE_PRESS==Event )
    {   short button,mouse_x,mouse_y ;

        button=s_get_mouse( &mouse_x, &mouse_y );
        if( 1==button )
	{   if( 0<mouse_x && mouse_x<50 && 7<=mouse_y && mouse_y<=n_Line-2 )
	    {   i_File=min( File.j, Begin+mouse_y-7 );
	    }
	    if( RShabl .inside( mouse_x, mouse_y ) ) taste2=S_key_F3 ;
	    if( RNew   .inside( mouse_x, mouse_y ) ) taste2=S_key_F4 ;
	    if( f_CFNeed )
	    {   if( RCode  .inside( mouse_x, mouse_y ) ) taste2=S_key_F6 ;
                if( RFormat.inside( mouse_x, mouse_y ) ) taste2=S_key_F7 ;
            }
	}
	if( 3==button )  taste1='\r' ;
	if( 4==button )  taste2=S_key_Up ;
	if( 5==button )  taste2=S_key_Down ;
    }
    
    switch( taste1 )
    {   case 0 :
	    switch( taste2 )
	    {   case S_key_Down : i_File++ ; break ;
		case S_key_Up   : i_File-- ; break ;
		case S_key_PgUp : i_File-=n_Line-8 ; break ;
		case S_key_PgDn : i_File+=n_Line-8 ; break ;
		case S_key_Home : i_File=0 ; break ;
		case S_key_End  : i_File=File.j-1 ; break ;
		case S_key_F3 : paint_file( 1 ) ; return 3 ;
		case S_key_F4 : paint_file( 1 ) ; return 4 ;
		case S_key_F6 : if( f_CFNeed ) {  paint_file( 1 ) ; return 6 ;  }
		case S_key_F7 : if( f_CFNeed ) {  paint_file( 1 ) ; return 7 ;  }

		case S_key_F10 : return -1 ;
	    }
	    if( i_File<0 ) i_File=0 ;
	    if( i_File<Begin ) Begin=i_File ;
	    if( i_File>=File.j ) i_File=File.j-1 ;
	    if( Begin+n_Line-8<i_File ) Begin=i_File-(n_Line-8) ;
	    break ;
	case '\r' :
	    if( File[i_File].type==0 ) return 0 ;
	    else
	    {  if( 0==strcmp( File[i_File].name,".." ) )
	       {  for( i=Path.length( )-2 ; 0<=i ; i-- )
		     if( Path[i]==SLASH ){  Path.resize(i+1) ; break ;  }
	       }
	       else
	       {  Path1=Path ;
	          Path1+=File[i_File].name ;
	          if( 0>access(Path1.c_str(),X_OK) ) break ;
	          Path+=File[i_File].name ;
		  Path+=SLASH1 ; 
	       }
	       read_file( Path ) ;
	       Begin=i_File=0 ;
	    }
	    break ;
    }
}
}
/***************************************************************************/
//                        Нарисовать диалог
//    fsave - 0-чтение 1-запись
//    Reg   - текущее подокно диалога (надо бы enum сделать)
/***************************************************************************/
void t_Dir :: paint_karkas( char fsave, char Reg )
{
   s_nacht();

   s_rame1_F( RPath.y1, RPath.x1, RPath.y2, RPath.x2, red_rame_Color );
   s_rame1_F( RDisk.y1, RDisk.x1, RDisk.y2, RDisk.x2, red_rame_Color );
   if( 3==Reg ) s_rame2_F( RShabl.y1,RShabl.x1,RShabl.y2,RShabl.x2,red_rame_Color );
       else     s_rame1_F( RShabl.y1,RShabl.x1,RShabl.y2,RShabl.x2,red_rame_Color );
   if( 4==Reg ) s_rame2_F( RNew.y1,  RNew.x1,  RNew.y2,  RNew.x2,  red_rame_Color );
       else     s_rame1_F( RNew.y1,  RNew.x1,  RNew.y2,  RNew.x2,  red_rame_Color );

   if( 5==Reg ) s_rame2_F( 4, 0, n_Line-1, 50, red_rame_Color );
       else     s_rame1_F( 4, 0, n_Line-1, 50, red_rame_Color );

   s_text_yxf( 0, 0, 0x77, emptystr(80) );
   if( fsave==0 ) s_text_yxf( 0,30,0x79,"Прочесть файл");
       else       s_text_yxf( 0,30,0x79,"Записать файл как");
   s_text_yxf( 1, 4,titl_Color,"Полный путь (path):" );
   s_text_yxf( 4,56,hotkey3_Color,"F2 ");   s_text_yxf( 4,59,titl_Color,"диски" );
   s_text_yxf( 7,56,hotkey3_Color,"F3 ");   s_text_yxf( 7,59,titl_Color,"шаблон" );
   s_text_yxf(10,56,hotkey3_Color,"F4 ");   s_text_yxf(10,59,titl_Color,"новый файл" );
   
   if( f_CFNeed )
   {
       if( 6==Reg ) s_rame2_F( RCode.y1,   RCode.x1,   RCode.y2,   RCode.x2,   red_rame_Color );
	   else     s_rame1_F( RCode.y1,   RCode.x1,   RCode.y2,   RCode.x2,   red_rame_Color );
       if( 7==Reg ) s_rame2_F( RFormat.y1, RFormat.x1, RFormat.y2, RFormat.x2, red_rame_Color );
	   else     s_rame1_F( RFormat.y1, RFormat.x1, RFormat.y2, RFormat.x2, red_rame_Color );

       s_text_yxf( RCode.y1  ,RCode.x1+6  ,hotkey3_Color,"F6 ");   
       s_text_yxf( RFormat.y1,RFormat.x1+6,hotkey3_Color,"F7 ");   
       s_text_yxf( RCode.y1  ,RCode.x1+9  ,titl_Color,"кодировка" );
       s_text_yxf( RFormat.y1,RFormat.x1+9,titl_Color,"формат" );
   }
   s_text_yxf( 4, 9,titl_Color," файлы и каталоги " );
   s_text_yxf( 5, 3,titlchen_Color,"имя                     дата      время  размер" );
   s_text_yxf( 6, 3,red_rame_Color,"---------------------------------------------" );
}
/***************************************************************************/
void t_Dir :: help( void )
{  short y=13,x=51 ;

   s_rame1_F(y,x,y+5,x+28,red_rame_Color) ;
   s_text_yxf(y+1,x+2,text_Color,"Боже, Боже!");
   s_text_yxf(y+2,x+2,text_Color,"дай мне полчаса времени");
   s_text_yxf(y+3,x+2,text_Color,"source - код вселенной");
   s_text_yxf(y+4,x+2,text_Color,"и хороший отладчик...");
}
/***************************************************************************/
void t_Dir :: paint_file( char Mode )
{  char f,date[32],str1[50] ;
   short i,i1,s1 ;

   s_text_yxf( RPath.y1+1,RPath.x1+1,text_Color,emptystr(40) ) ;
   s_text_yxf( RPath.y1+1,RPath.x1+1,text_Color,Path.c_str() ) ;

   for( i=RFile.y1+3 ; i<RFile.y2 ; i++ )
      s_text_yxf( i,RFile.x1+1,text_Color,emptystr(49) );

   for( i=Begin ; i<File.j ; i++ )
   {  if( i==i_File && Mode==0 ) f=cur_item_Color ; else f=text_Color ;
      file_date( date, File[i] ) ;
      s1=RFile.y1+i+3-Begin ;
      if( RFile.y2<=s1 ) break ;
      strcpy( str1,emptystr(49) ) ;
      strncpy( str1,File[i].name,min(15,strlen(File[i].name)) );
      strncpy( str1+26,date,23 );
      s_text_yxf( s1,RFile.x1+1,f,str1 ) ;
   }
}
/***************************************************************************/
//          прочитать список файлов данного каталога
/***************************************************************************/
char t_Dir :: read_file( t_String &path )
{
   short j ;
   t_String path1 ;
   t_File1 File1 ;

   DIR *my_dir ;
   struct dirent *ent ;
   static struct stat stat1 ;

   my_dir = opendir( path.c_str() );
   if( my_dir==NULL ) return 0 ;

   File.del();
   while( 1 )
   {
      ent = readdir( my_dir ) ;
      if( ent == NULL ) break ;
      path1=path ; path1+=SLASH1 ; path1+=ent->d_name ;
      stat( path1.c_str(),&stat1 ) ;
      if( 0==(S_IFDIR&stat1.st_mode) )
      {  if( 0==schablon( shabl,ent->d_name) ) continue ;
	 strncpy( File1.name,ent->d_name,32 ) ;
	 File1.type=0 ;
	 File1.date=stat1.st_mtime ;
	 File1.size=stat1.st_size ;
	 File.add( File1 );
      }
      else
      {  if( 0==strcmp( ent->d_name,".") ) continue ;
	 strncpy( File1.name,ent->d_name,32 ) ;
	 File1.type=1 ;
	 File.add( File1 );
      }
   }
   closedir( my_dir ) ;

   sort_file( );
   return 0 ;
}
/***************************************************************************/
//            Сделать строку "время модификации файла"
/***************************************************************************/
void t_Dir :: file_date( char *antwort, struct t_File1 &file )
{  short i,i1,j ;
   char  c[10] ;

   if( file.type==0 )
   {  struct tm *tm1 = localtime( &file.date );
      sprintf( antwort,"%02d:%02d:%02d %02d:%02d         ",
               tm1->tm_mday,tm1->tm_mon,(tm1->tm_year)%100, tm1->tm_hour,tm1->tm_min );
      sprintf( c,"%ld",file.size );
      i=strlen(c) ;
      for( j=23-i,i1=0 ; c[i1]!=0 ; i1++ ) antwort[j+i1]=c[i1] ;
   }
   else 
   {  strcpy( antwort,"                  <Dir>" ) ;
   }
}
/***************************************************************************/
//                           отсортировать файлы
/***************************************************************************/
int filecmp( const void *s1, const void *s2 )
{  t_File1 *File1=(t_File1 *)s1 ;
   t_File1 *File2=(t_File1 *)s2 ;
   
   int t=File2->type-File1->type ;
   if( t!=0 ) return t ;
   return strcmp( File1->name, File2->name );
}
/***************************************************************************/
void t_Dir :: sort_file( void )
{
   qsort( File.list,File.j,sizeof(t_File1),filecmp );
}
/***************************************************************************/
//        соответствует ли имя файла шаблону имени 
//        s - шаблон
//        n - имя
/***************************************************************************/
char schablon( char *s, char *n )
{  char s1[32],s2[32] ;
   char n1[32],n2[32] ;
   char f ;

   name_zerstoren( s,s1,s2 );
   name_zerstoren( n,n1,n2 );
   f=schablon1(s1,n1) & schablon1(s2,n2) ;
   return f ;
}
/***************************************************************************/
//        разделить имя файла на имя и расширение
/***************************************************************************/
void name_zerstoren( char *sou, char *dst1, char *dst2 )
{
   short n,i,p,f ;

   n=strlen(sou);
   for( f=0,i=n-1 ; 0<=i ; i-- )
      if( sou[i]=='.'){ f=1 ; p=i ; break ; }
   if( f==1 )
   {  strcpy( dst1,sou ); dst1[p]=0 ;
      strcpy( dst2,sou+p+1 );
   }
   else
   {  strcpy(dst1,sou);
      dst2[0]=0 ;
   }
}
/***************************************************************************/
//        соответствует ли имя (или расширение) файла шаблону
//        s - шаблон
//        n - имя
/***************************************************************************/
char schablon1( char *s, char *n)
{
   short i,f ;

   if( s[0]=='*' ) return 1 ;
   for( i=0 ; i<32 ; i++ )
   {  if( s[i]!='?' && s[i]!=n[i] ) { f=0 ; break ; }
      if( s[i]==0 ) { f=1 ; break ; }
   }
   return f ;
}
/***************************************************************************/
DEFINE_LIST_BTYPE( t_File1 ,t_File1List )
