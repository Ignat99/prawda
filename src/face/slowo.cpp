/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                    "Человеческий" словарь (ядро)
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# include <stdio.h>
# include <malloc.h>
# include <string.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdlib.h>

# include "mylib.h"
# include "slowo.h"

static uchar  *SC,
        SC1[256], // сортировка по-русски
	SC2[256] ;// сортировка по-европейски

# define  is_divider(SS)  (0==strncmp( SS,Divider,l_Divider ))

extern t_String   MercuryDir ;

short my_strncmp( char *Str1, char *Str2, short L );
void  SC_init( void );

/***************************************************************************/
t_Slowo :: t_Slowo( void )
{
   Name     ="" ;
   NameFile ="" ;
   WordStr  =NULL ;
   Word     =NULL ;
   L_WordStr=0 ;
   n_Word   =0 ;

   strcpy( Divider,"=" );
   l_Divider=strlen( Divider );
}
/***************************************************************************/
t_Slowo :: ~t_Slowo( void )
{  del();
}
/***************************************************************************/
void t_Slowo :: del( void )
{
   Name     ="" ;
   NameFile ="" ;
   Free( WordStr );
   Free( Word );
   L_WordStr=0 ;
   n_Word   =0 ;
}
/***************************************************************************/
int word_cmp( const void *a, const void *b )
{   t_slowoWord *a1=(t_slowoWord *)a ;
    t_slowoWord *b1=(t_slowoWord *)b ;
    return my_strncmp( a1->Str,b1->Str, 1000 );
}
/***************************************************************************/
//        прочесть словарь из файла и построить все его структуры          //
/***************************************************************************/
char t_Slowo :: read( char *File )
{  long    i,i1,j,f,z,z_begin,L,n_Mass,j_Str ;
   char   *Mass=NULL ;
try
{
   NameFile=File ;
   char *s=NameFile.c_str();
   for( z=i=0 ; s[i]!=0 ; i++ )
      if( s[i]=='/'  ) z=i+1 ;
   Name=s+z ;

   SC_init();

   Free(WordStr);
   Free(Word);

   t_String Str ;
   Str=MercuryDir+"/"+File ;
   n_Mass=file_to_mass( Str.c_str(),&Mass );
   
   // ------- сосчитать число слов и общую длинну слов -------
   n_Word=1 ;
   L_WordStr=0 ;
   f=1 ;
   for( z_begin=i=0 ; i<n_Mass ; i++ )
   {  if( Mass[i]=='\n' ) 
      {   if( 1==f ) L_WordStr+=i-z_begin+1 ;
          n_Word++ ; z_begin=i ; f=1 ;
      }
      if( 1==f && is_divider( Mass+i ) )
      {   L_WordStr+=i-z_begin+1 ; f=0 ; }
   }

   Word    =(t_slowoWord *)Calloc( n_Word   ,sizeof(t_slowoWord) );
   WordStr =(char        *)Calloc( L_WordStr,sizeof(char) );

   // ------- построить массив слов ---------------------------
   for( j=j_Str=0,i=0 ; i<n_Mass ; i++ )
   {  if( Mass[i]=='\n' || i==0 ) 
      {   Word[j].Str=WordStr+j_Str ;
          Word[j].begin=i+1 ;
	  if( 0<j ) Word[j-1].end=i ;
          for( i1=i+1 ; i1<n_Mass ; i1++ )
	     if( Mass[i1]=='\n' || is_divider( Mass+i1 ) )
	         break ;
          L=i1-i-1 ;
          strncpy( WordStr+j_Str,Mass+i+1, L );
	  WordStr[j_Str+L]=0 ;
	  j_Str+=L+1 ;
	  j++ ;
      }
   }
   Word[j-1].end=i ;
   n_Word=j ;
   Free( Mass );
   
   qsort( Word,n_Word,sizeof(t_slowoWord),word_cmp );

   return 0 ;
}
catch( int E )
{
   return -1 ;
}
}
/***************************************************************************/
//  найти индекс строки, в которой есть слово, максимально похожее на Str  //
/***************************************************************************/
long t_Slowo :: find( char *_Str )
{  long b,e,i,i1,i2,L ;
   char Str2[80] ;

   L=strlen(_Str);
   b=0 ; e=n_Word-1 ;

   while( 1 )
   {  if( e-b<=10 ) break ;
      i=(b+e)/2 ;
      if( 0<my_strncmp( _Str,Word[i].Str,L ) ) b=i ; else e=i ;
   }

   // ------------- поиск точно такой строки ---------------
   for( i1=b ; i1<=e ; i1++ )
   {
       Strcpy( Str2,Word[i1].Str,80 );
       for( i2=0 ; i2<79 ; i2++ )
	   if( is_divider( Str2+i2 ) ) Str2[i2]=0 ;
       if( 0==my_strncmp( Str2,_Str,L ) )
           return i1 ;
   }
   // --- если не найдена точно такая строк, найти похожую ----
   for( i=L-1 ; 0<i ; i-- )
   {  for( i1=b ; i1<=e ; i1++ )
      {
 	  Strcpy( Str2,Word[i1].Str,80 );
	  for( i2=0 ; i2<79 ; i2++ )
	      if( is_divider( Str2+i2 ) ) Str2[i2]=0 ;
	  if( 0==my_strncmp( Str2,_Str,i ) )
	      return i1 ;
      }
   }
   return -1 ;
}
/***************************************************************************/
//                     вернуть слово с индексом i                          //
/***************************************************************************/
char * t_Slowo :: wort( long z_word )
{  
   if( 0<=z_word && z_word<=n_Word )
       return Word[z_word].Str ;
   return "" ;
}
/***************************************************************************/
//                вернуть транскрипцию слова с индексом i                  //
/***************************************************************************/
char * t_Slowo :: trans( long z_word )
{  long   i ;
   static t_String StrTrans ;

return "" ;
/*
   StrTrans="" ;
   if( 0<=z_word && z_word<=n_Word )
   {  char *Str=Word[z_word] ;
      for( i=0 ; Str[i]!=0 ; i++ )
	 if( Str[i]=='[' ) break ;
      for( ; Str[i]!=0 ; i++ )
      {  StrTrans+=Str[i] ;
	 if( Str[i]==']' ) break ;
      }
   }
   return StrTrans.c_str() ;
*/
}
/***************************************************************************/
//                   вернуть перевод слова с индексом i                    //
/***************************************************************************/
char * t_Slowo :: answer( long z_word )
{  long  i,j,off,end,z,L ;
   char  f ;
   char  *Str,c,c1,c2 ;
   static t_String StrAnswer ;
   static char *Buf =NULL ; // локальный буфер для словарной статьи
   static long LBuf =0 ;    // его длинна

   StrAnswer="" ;
   if( 0<=z_word && z_word<=n_Word )
   {
      t_String File = MercuryDir+"/"+NameFile ;
      FILE *fr=Fopen( File.c_str(),"r" );
      L=Word[z_word].end-Word[z_word].begin ;
      if( LBuf<L+1 )
      {   Buf=(char *)Realloc( Buf,(L+1)*sizeof(char) );
          LBuf=L+1 ;
      }
      fseek( fr,Word[z_word].begin, SEEK_SET );
      fread( Buf,L,sizeof(char),fr );      
      Buf[L]=0 ;
      
      fclose(fr);
      
      Str=Buf ;

      for( z=i=0 ; i<Str[i]!=0 ; i++ )
	 if( is_divider( Str+i ) )
	 {   z=i+l_Divider ; break ;  }

      for( i=z,f=0 ; Str[i]!=0 ; i++ )
      {  if( Str[i]=='[' ) f=1 ;
	 if( Str[i]==']' ) { f=0 ; continue ; }
	 if( f==1 ) continue ;
	 c=Str[i] ; c1=Str[i+1] ; c2=Str[i+2] ;
	 if( (c==' ' || '0'<=c1 && c1<='9') &&
	     '0'<=c1 && c1<='9' && (c2==')' || c2=='>') )
	     StrAnswer+='\n' ;
	 StrAnswer+=Str[i] ;
	 if( Str[i]==';' ) StrAnswer+='\n' ;
      }
   }
   return StrAnswer.c_str() ;
}
/***************************************************************************/
//       установить какой разделитель будет между словом и переводом       //
/***************************************************************************/
void t_Slowo :: set_divider( char * _Divider )
{
   short  i,j ;
   char  *c ;

   c=Divider ;
   strcpy( c,_Divider );
   for( i=j=0 ; i<10 ; i++ )
   {  if( c[i]!='"' ) c[j++]=c[i] ;
      if( c[i]==0 ) break ;
   }
   l_Divider=strlen( Divider ) ;
}
/***************************************************************************/
//          установить, как будут сортироваться строки словаря             //
/***************************************************************************/
void t_Slowo :: set_sort( char f )
{  if( f==1 ) SC=SC2 ; else SC=SC1 ;  }
/***************************************************************************/
//                         сравнить две строки                             //
/***************************************************************************/
short my_strncmp( char *Str1, char *Str2, short L )
{  short i,c ;

   for( i=0 ; i<L ; i++ )
   {  if( Str1[i]==0 && Str2[i]!=0 ) return -1 ;
      if( Str2[i]==0 && Str1[i]!=0 ) return  1 ;
      c = SC[(uchar)Str1[i]] - SC[(uchar)Str2[i]] ;
      if( c!=0 ) return c ;
   }
   return 0 ;
}
/***************************************************************************/
void SC_init( void )
{  short i ;

   SC=SC1 ;

   for( i=0 ; i<256 ; i++ )
      SC[i]=i ;
   // ----- соответствие латинских больших и маленьких ----
   for( i=65 ; i<91 ; i++ )
      SC[i]=i+32 ;
   // ----- соответствие русских больших и маленьких в КОИ8 ----
   SC[193]=SC[225]=128 ;
   SC[194]=SC[226]=129 ;
   SC[215]=SC[247]=130 ;
   SC[199]=SC[231]=131 ;
   SC[196]=SC[228]=132 ;
   SC[197]=SC[229]=133 ;
   SC[214]=SC[246]=134 ;
   SC[218]=SC[250]=135 ;
   SC[201]=SC[233]=136 ;
   SC[202]=SC[234]=137 ;
   SC[203]=SC[235]=138 ;
   SC[204]=SC[236]=139 ;
   SC[205]=SC[237]=140 ;
   SC[206]=SC[238]=141 ;
   SC[207]=SC[239]=142 ;
   SC[208]=SC[240]=143 ;
   SC[210]=SC[242]=144 ;
   SC[211]=SC[243]=145 ;
   SC[212]=SC[244]=146 ;
   SC[213]=SC[245]=147 ;
   SC[198]=SC[230]=148 ;
   SC[200]=SC[232]=149 ;
   SC[195]=SC[227]=150 ;
   SC[222]=SC[254]=151 ;
   SC[219]=SC[251]=152 ;
   SC[221]=SC[253]=153 ;
   SC[216]=SC[248]=154 ;
   SC[217]=SC[249]=155 ;
   SC[223]=SC[255]=156 ;
   SC[220]=SC[252]=157 ;
   SC[192]=SC[224]=158 ;
   SC[209]=SC[241]=160 ;

   SC=SC2 ;

   for( i=0 ; i<256 ; i++ )
      SC[i]=i ;
   // ----- соответствие латинских больших и маленьких ----
   for( i=65 ; i<91 ; i++ )
      SC[i]=i+32 ;
   // ----- соответствие латинских умляутов, крышечек и тд ----
   SC[192]=SC[193]=SC[194]=SC[195]=SC[196]=SC[197]=97 ; //A
   SC[224]=SC[225]=SC[226]=SC[227]=SC[228]=SC[229]=97 ; //a
   SC[200]=SC[201]=SC[202]=SC[203]=101  ; //E
   SC[232]=SC[233]=SC[234]=SC[235]=101  ; //e
   SC[204]=SC[205]=SC[206]=SC[207]=SC[208]=111  ; //O
   SC[242]=SC[243]=SC[244]=SC[245]=SC[246]=111  ; //o
   SC[223]=115 ; //s
   SC[217]=SC[218]=SC[219]=SC[220]=117  ; //U
   SC[249]=SC[250]=SC[251]=SC[252]=117  ; //u
   SC[221]=SC[253]=SC[255]=121 ; // y
   
   SC=SC1 ;
}
