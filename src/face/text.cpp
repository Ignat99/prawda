/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                 Часть моего текстового редактора
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <string.h>
# include <stdlib.h>
# include <iconv.h>

# include "mylib.h"
# include "koi9.h"
# include "edit.h"
# include "video.h"

# define size_Tab    8
# define max_Length  1000000L

static char     *Empty="" ;
static t_myChar  Empty32[1]={ 0,0 } ;

/***************************************************************************/
/*                                                                         */
/***************************************************************************/
t_Text :: t_Text( void )
{
   Length = 0 ;
   Text = (t_myChar *)Calloc( max_Length,sizeof(t_myChar) );

   i_str=0 ; i_pos=0 ;
   n_String =1 ;
   is_const=NULL ;
}
/***************************************************************************/
t_Text :: ~t_Text( void )
{
   Free( Text );
}
/***************************************************************************/
void t_Text :: init( char *_Text )
{  long i,j,L ;

   ushort *Unicod=(ushort *)Calloc( max_Length,sizeof(ushort) );
   L=koi9_to_Unicod( _Text, Unicod, max_Length, max_Length );

   for( i=0 ; i<max_Length ; i++ )
      Text[i]=(ushort)0 ;
   n_String=1 ;
   for( i=j=0 ; i<L ; i++ )
   {  //if( _Text[i].Char==0 ) break ;
      if( Unicod[i]!='\r' ) Text[j++] = Unicod[i];
      if( Unicod[i]==UNEWLINE ) n_String++ ;
   }
   Length=j ;
   i_str=i_pos=0 ;
   Free( Unicod );
}
/***************************************************************************/
void t_Text :: set_reper( long _i_str )
{  if( 0<=_i_str && _i_str<n_String )
   {   i_pos=pos_str( _i_str );
       i_str=_i_str ;
   }
}
/***************************************************************************/
void t_Text :: reset_reper( )
{  i_str=i_pos=0 ;
}
/***************************************************************************/
//                    или из Юникда инициировать?
/***************************************************************************/
void t_Text :: init32( t_myChar *_Text )
{  long i,j ;

   for( i=0 ; i<max_Length ; i++ )
      Text[i]=(ushort)0 ;
   n_String=1 ;
   for( i=j=0 ; i<max_Length ; i++ )
   {  if( _Text[i].Char==0 ) break ;
      if( _Text[i].Char!='\r' ) Text[j++] = _Text[i] ;
      if( _Text[i].Char==UNEWLINE ) n_String++ ;
   }
   Length=j ;
   i_str=i_pos=0 ;
}
/***************************************************************************/
void t_Text :: clear( void )
{
   Length  = 0 ;
   i_str   = 0 ;
   i_pos   = 0 ;
   n_String= 1 ;
}
/***************************************************************************/
//
/***************************************************************************/
void t_Text :: write_file( char *Name, e_Latin CodePage )
{  
   long    i ;
   size_t  L1,L2 ;
   FILE   *fw=NULL ;
   ushort *T1 ;
   uchar  *T2 ;

try
{  fw=Fopen( Name,"wb" );

   ushort *T1=(ushort *)Calloc(Length,sizeof(ushort));
   for( long i=0 ; i<Length ; i++ )
      T1[i]=Text[i].Char ;

   if( CodePage!=UCS2BE )
   {  char *CP ;

      switch( CodePage )
      {
	  case LATIN1 : CP="iso8859-1" ; break ;
	  case LATIN2 : CP="iso8859-2" ; break ;
	  case LATIN3 : CP="iso8859-3" ; break ;
	  case LATIN4 : CP="iso8859-4" ; break ;
	  case KOI8   : CP="KOI8-R"    ; break ;      
      }
      L1=L2=Length ;
      iconv_t cd=iconv_open( CP,"UCS-2BE" ) ;
      iconv( cd, (char **)&T1, &L1, (char **)&T2, &L2 );
      iconv_close( cd );

      Fwrite( T2,Length,sizeof(char),fw );
   }
   else
   {  Fwrite( T1,Length,sizeof(ushort),fw );
   }
   Fclose( fw );
}
catch( int E )
{  Fclose( fw );  }
}
/***************************************************************************/
char t_Text :: read_file( char *Name, e_Latin CodePage )
{  long   i,i1,j,L ;
   FILE  *fr=NULL ;
   ushort *m=NULL ;

try
{
   fr=Fopen( Name,"rb" );
   if( CodePage==UCS2BE ) // нужен случай для USC2LE
   {   // ------ случай с двухбайтной кодировкой ---------------
       L=FileLength( fr )/sizeof(ushort);
       if( max_Length<L )
       {  error_set( "Файл слишком велик.\nВсе равно за один раз столько не переведете." );
	  throw(-1) ;
       }
       ushort *m=(ushort *)Calloc(L,sizeof(ushort));
       Fread( m,L,sizeof(ushort),fr );
   }
   else
   {   // ------ случай с однобайтными кодировками -------------
       L=FileLength( fr );
       if( max_Length<L )
       {  error_set( "Файл слишком велик.\nВсе равно за один раз столько не переведете." );
	  throw(-1) ;
       }
       char    *m0 ;
       iconv_t  cd ;
       
       m0=(char   *)Calloc(L,sizeof(char));
       m =(ushort *)Calloc(L,sizeof(ushort));
       Fread( m0 ,L,sizeof(char),fr );
       
       switch( CodePage )
       {   case   KOI8 : cd=iconv_open("UCS-2BE","KOI8-R" ) ; break ;
           case CP1251 : cd=iconv_open("UCS-2BE","CP1251" ) ; break ;
           case LATIN1 : cd=iconv_open("UCS-2BE","iso8859-1" ) ; break ;
           case LATIN2 : cd=iconv_open("UCS-2BE","iso8859-2" ) ; break ;
	   case LATIN3 : cd=iconv_open("UCS-2BE","iso8859-3" ) ; break ;
	   case LATIN4 : cd=iconv_open("UCS-2BE","iso8859-4" ) ; break ;
       }
       char   *d1=&m0[0] ;
       char   *d2=(char *)&m [0] ;
       size_t  L1=L ;
       size_t  L2=L*sizeof(short);
       iconv( cd, &d1, &L1, &d2, &L2 );
       iconv_close( cd );
       Free(m0);
   }
   // -------- преобразовать табуляции и границы строк -------------------
   n_String=1 ;
   for( i=j=0 ; i<L ; i++ )
   {  if( m[i]=='\t' )
      {   for( i1=0 ; i1<size_Tab ; i1++ )
             Text[j++]=' ';
	  continue ;
      }
      if( m[i]!=UCARRET ) Text[j++]=m[i] ;
      if( m[i]==UNEWLINE ) n_String++ ;
   }
   Length=j ;
   for( i=j ; i<max_Length ; i++ )
      Text[i]=(ushort)0 ;

   Free(m);
   Fclose( fr );
   i_str=i_pos=0 ;
   return 0 ;
}
catch( int E )
{  Free( m );
   Fclose( fr );   
   throw( E ) ;
}
}
/***************************************************************************/
t_myChar * t_Text :: get_str( long i_Str, t_myChar *Buf )
{  t_myChar *s ;
   short i ;
   
   s=get_str( i_Str );
   for( i=0 ; i<256 ; i++ )
      Buf[i]=' ' ;
   for( i=0 ; i<256 ; i++ )
   {  if( s[i]==(ushort)0 || s[i]=='\n' ) break ;
      Buf[i]=s[i] ;
   }      
   return Buf ;
}
/***************************************************************************/
t_myChar * t_Text :: get_str( long i_Str )
{  long p ;

   if( i_Str<0 || n_String<=i_Str ) return Empty32 ;
   p=pos_str( i_Str );   
   if( Length<=p ) return Empty32 ;
   return Text+p ;
}
/***************************************************************************/
long t_Text :: get_pos_xy( long py, long px )
{  long i,p ;

   if( py<0 ) return 0 ;
   if( n_String<=py ) return Length ;
   p=pos_str( py );
   for( i=0 ; i<px ; i++ )
   {  if( Length<=p+i ) break ;
      if( Text[p+i]=='\n' ) {  i++ ; break ;  }
   }
   return p+i ;
}
/***************************************************************************/
void t_Text :: get_xy_pos( long pos1, long *_py, long *_px )
{  long i ;
   long px,py ;

   if( pos1<0 ) {  *_py=0; *_px=0 ; return ;  }
   if( Length<=pos1 ) 
   {  *_py=n_String-1;
      for( i=1 ; i<Length ; i++ )
         if( Text[Length-i]=='\n' ) break ;
      *_px=i ;
      return ;
   }
   px=pos1 ; py=0 ;
   for( i=0 ; i<=pos1 ; i++ )
      if( Text[i]=='\n' ) {  py++ ; px=pos1-i-1 ;  }
   *_py=py ;
   *_px=px ;
}
/***************************************************************************/
//                    вернуть индекс начала строки
/***************************************************************************/
long t_Text :: pos_str( long i_Str )
{
   long i,i_str1 ;

   if( i_Str==0 ) return 0 ;
   if( i_Str==i_str ) return i_pos ;
   //for( i=i_str1=0 ; i<Length ; i++ )
   for( i=i_pos,i_str1=i_str ; i<Length ; i++ )
   {
      if( Text[i]=='\n' )
      {  i_str1++ ;
	 if( i_str1==i_Str ) return i+1 ;
      }
   }
   return Length ;
}
/***************************************************************************/
//    вставить строку в позицию (предыдущая строка исчезает)
/***************************************************************************/
void t_Text :: put_str_ipos( long i1_pos, t_myChar *str )
{  long l ;
   long i,j,i1,i2 ;

   // ---- вычислить l - длинну строки
   for( l=0 ; l<256 ; l++ )
      if( (str[l]==(ushort)0 && str[l].Flag==0) || str[l]=='\n' ) break ;
   j=l ;
   for( i=l=0 ; i<j ; i++ )
      if( str[i]!=' ' || str[i].Flag!=0 ) l=i+1 ;

   // ---- проверить позицию на легальность
   if( i1_pos<0 || Length<i1_pos ) throw(-1);

   // ---- найти i1 - начало строки
   for( i1=i1_pos-1 ; 0<=i1 ; i1-- )
      if( Text[i1]=='\n' ){  i1++ ; break ;  }
   if( i1<0 ) i1=0 ;

   // ---- найти i2 - конец строки
   for( i2=i1_pos ; i2<Length ; i2++ )
      if( Text[i2]=='\n' ){  i2++ ; break ;  }

   if( l+1 <= i2-i1 )
   {  // ---- если вставляемая строка короче предыдущей
      for( i=0,j=i1 ; i<l ; i++ )
	 Text[j++]=str[i] ;
      Text[j++]='\n' ;
      for( i=i2 ; i<Length ; i++ )
	 Text[j++]=Text[i] ;
   }
   else
   {  // ---- если вставляемая строка длиннее предыдущей
      for( i=Length-i2-1 ; 0<=i ; i-- )
	 Text[i1+l+1+i]=Text[i2+i] ;
      for( i=0,j=i1 ; i<l ; i++ )
	 Text[i1+i]=str[i] ;
      Text[i1+l]='\n' ;
   }
   Length+=(l+1)-(i2-i1);
   if( i1_pos<i_pos ) i_pos+=(l+1)-(i2-i1) ;
}
/***************************************************************************/
void t_Text :: put_str_istr( long i_Str, t_myChar *str )
{  long pos ;

   pos=pos_str( i_Str );
   put_str_ipos( pos, str );
}
/***************************************************************************/
//                 выдать слово в текущей позиции
/***************************************************************************/
t_myChar * t_Text :: get_word_punkt32( long z )
{  long i,j ;
   ushort c ;
   static t_myChar Str[1000]={0} ;

   if( Length<=z ) return Str ;

   // ---- найти  начало слова
   for(  ; 0<=z ; z-- )
      if( is_space32(Text[z]) )  break ;

   z++ ;
   // ---- заполнить возвращаемую строку
   for( j=0,i=z ; i<Length ; i++ )
   {  c=Text[i].Char ;
      if( is_space16(c) ) break ;
      Str[j++]=Text[i] ;
   }
   Str[j++]=0 ;
   return Str ;
}
/***************************************************************************/
char * t_Text :: get_word_punkt( long z )
{  static char Str1[1000]={0} ;

   myChar_to_koi9( get_word_punkt32(z), (uchar *)Str1, 1000 );
   return Str1 ;
}
/***************************************************************************/
long t_Text :: prev_word( long z )
{
   for(  ; 0<=z ; z-- )
      if( is_space32(Text[z]) )  break ;
   for(  ; 0<=z ; z-- )
      if( !is_space32(Text[z]) )  break ;

   if( z<0 ) z=0 ;
   return z ;
}
/***************************************************************************/
long t_Text :: next_word( long z )
{
   for(  ; z<Length ; z++ )
      if( is_space32(Text[z]) )  break ;
   for(  ; z<Length ; z++ )
      if( !is_space32(Text[z]) )  break ;

   if( Length<=z ) z=Length-1 ;
   return z ;   
}
/***************************************************************************/
//                 выдать фразу в текущей позиции
/***************************************************************************/
t_myChar * t_Text :: get_frasa32( long pos )
{  long i,j,z1,z2 ;
   static t_myChar Str[1000]={0} ;

   if( Length<=pos ) return Str ;

   z1=begin_frasa( pos );
   z2=end_frasa( pos ) ;
   for( i=z1,j=0 ; i<=z2 && j<999 ; i++ )
      Str[j++]=Text[i] ;
   Str[j++]=(ushort)0 ;
   return Str ;
}
/***************************************************************************/
//                 выдать фразу в текущей позиции
/***************************************************************************/
char * t_Text :: get_frasa( long pos )
{  static char Str1[1000]={0} ;

   myChar_to_koi9( get_frasa32(pos), (uchar *)Str1, 1000 );
   return Str1 ;
}
/***************************************************************************/
//                 вставить фразу в текущую позицию
/***************************************************************************/
void t_Text :: put_frasa32( long z, t_myChar *str )
{  long  i,j,i1,i2,l ;

   l=strlen32( str );
   if( Length<=z )
   {   Text[Length++]='\n' ;
       for( i=0 ; i<l ; i++ )
          Text[Length++]=str[i] ;
       goto M_End ;
   }
   // ---- найти i1 - начало фразы
   i1=begin_frasa( z ) ;
   i2=end_frasa( z );
   if( i2<z ) i1=i2=z ; // только что вставил (вставка после последней фразы)
   if( i1<0 ) i1=0 ;
   if( Text[i1]=='\n' ) i1++ ;

   if( l <= i2-i1 )
   {  // ---- если вставляемая строка короче предыдущей
      for( i=0,j=i1 ; i<l ; i++ )
	 Text[j++]=str[i] ;
      for( i=i2 ; i<Length ; i++ )
	 Text[j++]=Text[i] ;
      Text[j]=(ushort)0 ;
   }
   else
   {  // ---- если вставляемая строка длиннее предыдущей
      for( i=Length-i2-1 ; 0<=i ; i-- )
	 Text[i1+l+i]=Text[i2+i] ;
      for( i=0,j=i1 ; i<l ; i++ )
	 Text[i1+i]=str[i] ;
   }
   Length+=l-(i2-i1);
   Text[Length]=(ushort)0 ;
   
   M_End :
   for( n_String=1,i=0 ; i<Length ; i++ )   
      n_String+=(Text[i]=='\n') ;
}
/***************************************************************************/
//                 выдать фразу в текущей позиции
/***************************************************************************/
void t_Text :: put_frasa( long z, char *str )
{  static t_myChar Str1[1000]={0} ;

   koi9_to_myChar( str, Str1, 1000 );
   put_frasa32( z, Str1 );
}
/***************************************************************************/
//                          предыдущая фраза
/***************************************************************************/
long t_Text :: prev_frasa( long pos )
{  long  i ;

   i=begin_frasa( pos );
   i=begin_frasa( i-1 );
   if( i<0 ) i=0 ;
   return i ;
}
/***************************************************************************/
//                       последующая фраза
/***************************************************************************/
long t_Text :: next_frasa( long pos )
{  long i,z ;

   z=end_frasa( pos ) ;
   return z+1 ;
}
/***************************************************************************/
//                          начало фразы
/***************************************************************************/
long t_Text :: begin_frasa( long pos )
{  long      i ;
   t_myChar  c,c1 ;

   for( i=pos ; 0<=i ; i-- )
   {  c=Text[i] ;
      if( c=='.' || c=='!' || c=='?' || c==':' )
      {  c1=Text[i+1] ;
	 if( is_space(c1) ) return i+1 ;
      }
   }
   return 0 ;   
}
/***************************************************************************/
//                          конец фразы
/***************************************************************************/
long t_Text :: end_frasa( long pos )
{  long      i,z ;
   char      f=0 ;
   t_myChar  c,c1 ;

   for( i=pos ; i<Length ; i++ )
   {  c=Text[i] ;
      if( c=='.' || c=='!' || c=='?' || c==':' )
      {  c1=Text[i+1] ;
	 if( is_space(c1) ) return i+1 ;
      }
   }
   return Length ;
}
/***************************************************************************/
//     извлечь из текста строки с Str1 по Str2 с удалением из текста
/***************************************************************************/
long t_Text :: get_text1( long Str1, long Str2, t_myChar *List )
{
   long  i,pos1,pos2,L=0 ;

   pos2=pos_str( Str2+1 );
   pos1=pos_str( Str1 );

   L=pos2-pos1 ;
   for( i=0 ; i<L ; i++ )
      List[i]=Text[pos1+i] ;

   Length-=L ;
   for( i=pos1 ; i<Length ; i++ )
      Text[i]=Text[i+L] ;

   for( i=0,n_String=1 ; i<Length ; i++ )
      if( Text[i]=='\n' ) n_String++ ;
   return pos2-pos1 ;
}
/***************************************************************************/
//     извлечь из текста строки с Str1 по Str2 без удаления из текста
/***************************************************************************/
long t_Text :: get_text2( long Str1, long Str2, t_myChar *List )
{
   long  i,pos1,pos2,L=0 ;

   pos2=pos_str( Str1 );
   pos1=pos_str( Str2 );

   L=pos2-pos1 ;
   for( i=0 ; i<L ; i++ )
      List[i]=Text[pos1+i] ;

   return L ;
}
/***************************************************************************/
//                
/***************************************************************************/
void t_Text :: put_text1( long Str, t_myChar *List, long l_List )
{
   long  i,pos1 ;

   pos1 =pos_str( Str );

   for( i=Length-1 ; pos1<=i ; i-- )
      Text[i+l_List]=Text[i] ;
   Length+=l_List ;
   for( i=0 ; i<l_List ; i++ )
      Text[pos1+i]=List[i] ;
   for( i=0,n_String=1 ; i<Length ; i++ )
      if( Text[i]=='\n' ) n_String++ ;
}
/***************************************************************************/
//                вставить фразу взамен cуществующей
/***************************************************************************/
void t_Text :: put_text1p( long Pos1, long Pos2, t_myChar *Str ) 
{
   long i,L,D ;
   char f ;
   
   L=strlen32( Str );
   if( Str[L-1]==' ' ) Str[L-1]='.' ;
   D=L-(Pos2-Pos1)+1 ;

   if( 0<D )
   {   // ---------- раздвинуть текст --------------
       for( i=Length ; Pos1<=i ; i-- )
	  Text[i+D]=Text[i] ;
       for( i=0 ; i<D ; i++ )
          Text[Pos1+i]=' ' ;
       Length+=D ;
   }
   if( D<0 )
   {   // ---------- сдвинуть текст ----------------
       for( i=Pos1 ; i<Length ; i++ )
	  Text[i]=Text[i-D] ;
       Length+=D ;
   }
   // ----------- вставить фразу ----------------
   // ---- танцы с f - чтобы сохранить переводы строк ----
   for( i=0,f=0 ; i<L ; i++ )
   {  //if( Text[Pos1+i]=='\n' ) f=1 ;
      Text[Pos1+i]=Str[i] ;
      //if( f==1 && Str[i]==' ' )
      //{   Text[Pos1+i]='\n' ; f=0 ;  }
   }
   
}
/***************************************************************************/
//                       вставить пустую строку
/***************************************************************************/
void t_Text :: insert_str( long PY )
{  long i,j ;

   j=pos_str( PY );
   if( max_Length<Length )
   {  printf("\n t_Text :: insert_str Length>=max_Length !!!" ); exit(-1) ; }
   for( i=Length ; j<i ; i-- )
	 Text[i]=Text[i-1] ;
   Text[j].Char=UNEWLINE ;
   Text[j].Flag=0 ; 
   Length++ ;
   n_String++ ;
}
/***************************************************************************/
void t_Text :: delete_str( long PY )
{  long j,j1 ;

   j=pos_str( PY );
   if( Length<=j ) return ;
   for( j1=j ; j1<Length ; j1++ )
      if( Text[j1]=='\n' ) break ;
   for( j1++ ; j1<Length ; j1++ )
      Text[j++]=Text[j1] ;   
   Length=j ;
   n_String-- ;
}
/***************************************************************************/
short t_Text :: max_length_str( void )
{  short i,j,L=0 ;

   for( i=j=0 ; i<Length ; i++ )
      if( Text[i]=='\n' )
      {  L=max( L,i-j ); j=i;  }
   L=max( L,i-j );
   return L ;
}
/***************************************************************************/
long t_Text :: length( void )
{  return Length ;  }
/************************************************************************/
long t_Text :: n_string( )
{  return n_String ;  }
/***************************************************************************/
t_myChar & t_Text :: text( long i )
{  return Text[i] ; }
/***************************************************************************/
t_myChar * t_Text :: text_all( )
{  Text[Length]=0 ;
   return Text ; 
}
