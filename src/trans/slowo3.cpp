/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                 ������� ��������� ���� � ��������������
//       �������� �.�.                                    �������
/***************************************************************************/

# include <stdio.h>
# include <malloc.h>
# include <string.h>
# include <stdlib.h>

# include "mylib.h"
# include "grammar.h"
# include "slowo3.h"
# include "core.h"
# include "factorial.cpp"

# define DELTA_MASS   1000
# define DELTA_WORD    100
# define DELTA_STRUCT   40
# define DELTA_RECORD   10
# define DELTA_REL      50
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

extern t_Slowo3 Perevod1 ;
static t_Slowo3 *Slowo3 ;
char   Word0[100] ;
t_Factorial Factorial(5) ;
static t_String StrErr ;  // ������ ������ (���-�� ��� �� �� ���������)

char *filename( char *Path, char *DirLang, char *File );
short find_param1( t_Param1List *List, char *Str );
int   funk0( const void *a, const void *b );

/***************************************************************************/
//                          ������������ ������
/***************************************************************************/
t_Slowo3 :: t_Slowo3( char _i_dic )
{
   FileName[0]=0 ;
   i_dic   =_i_dic ;
   Format  =NULL ;

   Mass    =NULL ;
   Word    =NULL ;
   Struct  =NULL ;
   Record  =NULL ;
   Tree    =NULL ;
   Relation=NULL ;

   n_Mass    = l_Mass    = 0 ;
   n_Word    = l_Word    = 0 ;
   n_Struct  = l_Struct  = 0 ;
   n_Record  = l_Record  = 0 ;
   n_Tree    = l_Tree    = 0 ;
   n_Relation= l_Relation= 0 ;

   short i ;
   for( i=0 ; i<256 ; i++ )
      Type[i]=0 ;
   Type['=']=Type[';']=Type[':']=Type['~']=1 ;
   Type['(']=Type[')']=Type['[']=Type[']']=Type['<']=Type['>']=1 ;

   mtime=0 ;
}
/***************************************************************************/
//                       ��������
/***************************************************************************/
void t_Slowo3 :: clr( void )
{
   n_Mass=n_Word=n_Struct=n_Record=n_Tree=n_Relation = 0 ;
}
/***************************************************************************/
//                       ���������� ������
/***************************************************************************/
void t_Slowo3 :: del( void )
{
   clr( );
   Free( Mass );
   Free( Word );
   Free( Struct );
   Free( Record );
   Free( Tree );
   Free( Relation );
}
/***************************************************************************/
//                     �������� �������� �����
/***************************************************************************/
void t_Slowo3 :: write_bin( char *File )
{  FILE *fw ;

   fw=Fopen( File,"wb" );
   
   Fwrite( &n_Mass    ,1,sizeof(long),fw );
   Fwrite( &n_Word    ,1,sizeof(long),fw );
   Fwrite( &n_Struct  ,1,sizeof(long),fw );
   Fwrite( &n_Record  ,1,sizeof(long),fw );
   Fwrite( &n_Tree    ,1,sizeof(long),fw );
   Fwrite( &n_Relation,1,sizeof(long),fw );

   Fwrite( Mass    ,n_Mass+1    ,sizeof(char)      ,fw );
   Fwrite( Word    ,n_Word+1    ,sizeof(t_sWord)   ,fw );
   Fwrite( Struct  ,n_Struct+1  ,sizeof(t_sStruct) ,fw );
   Fwrite( Record  ,n_Record+1  ,sizeof(t_sRecord) ,fw );
   Fwrite( Tree    ,n_Tree      ,sizeof(t_sTree)   ,fw );
   Fwrite( Relation,n_Relation+1,sizeof(t_Relation),fw );

   Fclose( fw );
}
/***************************************************************************/
//                     ��������� �������� �����
/***************************************************************************/
char t_Slowo3 :: read_bin( char *File )
{  FILE *fr ;
   long  i ;
try
{
   fr=Fopen( File,"rb" );
   del( );
   
   Fread( &n_Mass    ,1,sizeof(long),fr );
   Fread( &n_Word    ,1,sizeof(long),fr );
   Fread( &n_Struct  ,1,sizeof(long),fr );
   Fread( &n_Record  ,1,sizeof(long),fr );
   Fread( &n_Tree    ,1,sizeof(long),fr );
   Fread( &n_Relation,1,sizeof(long),fr );

   Mass    =(uchar      *)Malloc( n_Mass+1    ,sizeof(uchar));
   Word    =(t_sWord    *)Malloc( n_Word+1    ,sizeof(t_sWord));
   Struct  =(t_sStruct  *)Malloc( n_Struct+1  ,sizeof(t_sStruct));
   Record  =(t_sRecord  *)Malloc( n_Record+1  ,sizeof(t_sRecord));
   Tree    =(t_sTree    *)Malloc( n_Tree      ,sizeof(t_sTree));
   Relation=(t_Relation *)Malloc( n_Relation+1,sizeof(t_Relation));

   Fread( Mass    ,n_Mass+1    ,sizeof(char)      ,fr );
   Fread( Word    ,n_Word+1    ,sizeof(t_sWord)   ,fr );
   Fread( Struct  ,n_Struct+1  ,sizeof(t_sStruct) ,fr );
   Fread( Record  ,n_Record+1  ,sizeof(t_sRecord) ,fr );
   Fread( Tree    ,n_Tree      ,sizeof(t_sTree)   ,fr );
   Fread( Relation,n_Relation+1,sizeof(t_Relation),fr );

   Fclose( fr );
   for( i=0 ; i<n_Struct ; i++ )
      Struct[i].Word=Word+Struct[i].sy_word ;

   for( i=0 ; i<n_Word ; i++ )
      Word[i].str=(char *)Mass+Word[i].sy ;

   l_Mass=n_Mass ;
   return 0 ;
}
catch( ... )
{
   return -1 ;
}
}
/***************************************************************************/
//              ��������� �� ���������� ������� ��� �������, 
//              � ������� �� ��� ������ �������
/***************************************************************************/
void t_Slowo3 :: read( char *Dir, t_Grammar *Gr )
{  long        i,i1,L ;
   FILE       *fr ;
   t_File     *FF ;
   t_FileList &File1=Gr->file1() ;
   t_String    FileName1 ;
   struct stat stat1 ;

try
{
   del( );
   Format=&Gr->format1() ;
   
   Mass=NULL ;
   n_Mass=0 ;
   for( i=0 ; i<File1.j ; i++ )
   {  FF=&File1[i] ;
      for( i1=0 ; i1<FF->FileName.j ; i1++ )
      {  // -------- ������� ��� -------------------
       	 FileName1=Dir ;
         if( FileName1[FileName1.length()-1]!='/' )
	     FileName1+="/" ;
	 FileName1+=FF->FileName[i1] ;
         
	 // -------- �������� ��������� ������ --------
	 fr=Fopen( FileName1.c_str(),"rb" );
	 L=FileLength( fr );
	 stat( FileName1.c_str(), &stat1 );
	 mtime = max( mtime, stat1.st_mtime );
	 Mass  =(uchar *)Realloc( Mass,(n_Mass+L+1)*sizeof(uchar) );
	 Fread( Mass+n_Mass,L,sizeof(uchar),fr );
	 fclose(fr);
	 Mass[n_Mass+L]='\n' ;
	 n_Mass+=L+1 ;
      }
   }
   Mass[n_Mass-1]=0 ;

   n_Mass=remark( (char *)Mass, n_Mass );
   parse( );
   sort0( );
   make_tree( );
}
catch( int E )
{
   fprintf( File_Error,"\n ������, ��-�� � ������� %s �� ���� ��������.",FileName1.c_str() );
   if( E==Err_Fopen ) fprintf( File_Error,"\n � �� ������-�� ����?" );
   throw( E );
}
}
/***************************************************************************/
//             �������� �� ����� � ��������� ��� ���������
/***************************************************************************/
void t_Slowo3 :: read( char *File, t_Format1 *_Format )
{
   Format = _Format ;

   read0( File );
   parse( );
   split_unorder( );
   sort0( );
   make_tree( );
}
/***************************************************************************/
//             �������� �� ������� � ��������� ��� ���������
/***************************************************************************/
void t_Slowo3 :: make( char *_Mass, t_Format1 *_Format )
{
   Format = _Format ;

   make0( _Mass );
   parse( );
   sort0( );
   make_tree( );
}
/***************************************************************************/
//                     �������� �� �����
/***************************************************************************/
void t_Slowo3 :: read0( char *File )
{  FILE *fr ;

try
{
   fr=Fopen( File,"rb" );
   n_Mass=FileLength( fr );
   Mass  =(uchar *)Calloc( n_Mass+1,sizeof(uchar) );
   Fread( Mass,n_Mass,sizeof(uchar),fr );
   fclose(fr);
   n_Mass=remark( (char *)Mass, n_Mass );
}
catch( int E )
{  
   fprintf( File_Error,"\n ������, ��-�� � ������� %s �� ���� ��������.",File );
   if( E==Err_Fopen ) fprintf( File_Error,"\n � �� ������-�� ����?" );
   throw( E );
}
}
/***************************************************************************/
//                     �������� �� �������
/***************************************************************************/
void t_Slowo3 :: make0( char *_Mass )
{
   n_Mass=strlen( _Mass );
   Mass  =(uchar *)Calloc( n_Mass+1,1 );
   for( long i=0 ; i<n_Mass ; i++ )
      Mass[i]=_Mass[i] ;
   n_Mass=remark( (char *)Mass, n_Mass );
}
/***************************************************************************/
//                     ��������� ��� ���������
/***************************************************************************/
void t_Slowo3 :: parse( void )
{  long  i,i1,i2,j,j1 ;
   char  c ;
   long  j_Word,j_Struct,j_Record,j_Relation ;
   char  fformat ;
   t_String Msg ;
   t_sStruct *S ;
   t_sWord   *W ;

try
{
   // ------------ ��������� ����� ���� � ��������� -----------------
   for( i=0,fformat=1 ; i<n_Mass ; i++ )
   {  c=Mass[i] ;
      if( c=='[' || c=='~' )
      {   n_Word++ ;
	  n_Struct+=fformat ;
	  continue ;
      }
      if( c=='\n' ) {  n_Record++ ; fformat=1 ; }
      if( c==':'  ) {  n_Struct++ ; fformat=0 ; }
      if( c==';' || c=='=' ) {      fformat=1 ; }
   }

   n_Record++ ;
   // ------------ ��������� ������ --------------------------------
   Word    =(t_sWord    *)Calloc( n_Word+1  ,sizeof(t_sWord) );
   Struct  =(t_sStruct  *)Calloc( n_Struct+1,sizeof(t_sStruct) );
   Record  =(t_sRecord  *)Calloc( n_Record+1,sizeof(t_sRecord) );
   Relation=(t_Relation *)Calloc( 400000    ,sizeof(t_Relation) );

   for( i=0 ; i<n_Struct ; i++ )
      Struct[i].Param.init();
   for( i=0 ; i<n_Word ; i++ )
      Word[i].Param.init();

   // ---------- ���������� ������ � ���������� ����� ���� ---------------
   j_Word    =0 ; W=Word ;
   j_Struct  =0 ; S=Struct ;
   j_Record  =0 ;
   j_Relation=0 ;

   for( i=0 ; i<256 ; i++ )
      Type[i]=0 ;
   Type['=']=Type[';']=Type[':']=Type['~']=1 ;
   Type['(']=Type[')']=Type['[']=Type[']']=Type['<']=Type['>']=1 ;

   t_sRecord1 Record1 ;

   for( i=-1 ; i<n_Mass-1 ; i++ )
   {  if( i!=-1 && Mass[i]!='\n' ) continue ;

      // -------- ������������ ������ ��������� ------------
      Msg="" ;
      for( i1=i+1,j=0 ; i1<n_Mass && Mass[i1]!='\n' ; i1++ )
          Msg+=Mass[i1] ;

      Record1.parse1( (char *)Mass+i+1, Format, 0 );

      Record[j_Record++].sy_struct=j_Struct ;
      for( i1=0 ; i1<Record1.Struct.j ; i1++ )
      {  Struct[j_Struct]           =Record1.Struct[i1] ;
         Struct[j_Struct].sy_word   =j_Word ;
         Struct[j_Struct].i_relation=j_Relation ;
	 
         for( i2=0 ; i2<Record1.Struct[i1].n_Word ; i2++ )
	 {  Word[j_Word]   =Record1.Struct[i1].Word[i2] ;
	    Word[j_Word].sy=Record1.Struct[i1].Word[i2].sy+i+1 ;
	    j_Word++ ;
	 }
	 short n_Rel=Record1.Relation[Record1.Struct[i1].i_relation].s1+1 ;
         for( i2=0 ; i2<n_Rel ; i2++ )
	    Relation[j_Relation++]=Record1.Relation[Record1.Struct[i1].i_relation+i2] ;
	 j_Struct++ ;
      }
   }   
   if( n_Struct<j_Struct )
       printf( "\n Error n_Struct<=j_Struct" );
   // ----- ��������� �������� �������� --------------
   Struct[j_Struct].sy_word   =j_Word ;
   Struct[j_Struct].i_relation=j_Relation ;
   Record[j_Record].sy_struct =j_Struct ;

   // ----- ��������� ������ Struct[].Word -----------
   for( i=0 ; i<n_Struct ; i++ )
   {  Struct[i].Word=Word+Struct[i].sy_word ;
      Struct[i].n_Word=Struct[i+1].sy_word-Struct[i].sy_word ;
      Relation[Struct[i].i_relation].s1=Struct[i+1].i_relation-Struct[i].i_relation-1 ;
   }

   // ----- ��������� Record[].n_struct --------------
   for( i=0 ; i<n_Record ; i++ )
   {  Record[i].Struct  =Struct+Record[i].sy_struct ;
      Record[i].n_struct=Record[i+1].sy_struct-Record[i].sy_struct ;
   }
   // ------ ��������� TSTRUCT1 ----------------------
{
   char f_error=0 ;
   for( i=0 ; i<n_Record ; i++ )
   {  t_Struct *S0 ;
      for( i1=0 ; i1<Record[i].n_struct ; i1++ )
      {  S=&Struct[Record[i].sy_struct+i1] ;
      
         if( S->i_struct<0 ) continue ;
         if( i1==0 ) S0=&Grammar[S->i_struct].From ;
	     else    S0=&Grammar[S->i_struct].To ;
         if( S0->type!=TSTRUCT1 ) continue ; // ABR
         if( S0->Word.j!=S->n_Word )
	 {   fprintf( File_Error,"\n �������������� ����� ���� � ���������1");
	     fprintf( File_Error,"\n ������ %ld",i+1 );	     
	     f_error=1 ;
	 }
	 for( i2=0 ; i2<S0->Word.j ; i2++ )
	 {  if( S0->Word[i2].i_struct!=S->Word[i2].i_struct )
	    {   fprintf( File_Error,"\n �������������� ����� ���� � ���������1");
	        fprintf( File_Error,"\n ������ %ld",i+1 );
		f_error=1 ;
	    }
	 }
      }
   }
   if( f_error==1 ) throw(-1); // ��� ������ �� ���������������
}
   
   // ----- ������� ������ Record --------------------
   for( i=j_Record=0 ; i<n_Record ; i++ )
      if( 0<Record[i].n_struct ) Record[j_Record++]=Record[i] ;
   n_Record=j_Record ;
   
   // ------ ���������� ������ Word[].str ------------
   for( i=0 ; i<n_Word ; i++ )
      Word[i].str=(char *)Mass+Word[i].sy ;

   // ------ ��������� ������� ������ ----------------
   for( i=0 ; i<n_Mass ; i++ )
      if( Mass[i]=='\n' ) Mass[i]=0 ;     

   Relation=(t_Relation *)Realloc( Relation,j_Relation*sizeof(t_Relation) );
   n_Relation=j_Relation ;
}
catch( int E )
{
   if( NULL==File_Error )
       File_Error=Fopen("mercury.err","w+");
   fprintf( File_Error,"\n%s\n",StrErr.c_str() );
   fprintf( File_Error,"\nSlowo3::������=\"%s\"\n",Msg.c_str() );
   error_add( "t_Slowo3 :: parse\n" );
   Read_Error=-1 ;
   throw( E ); // ��� ������ �� ���������������
}
}
/************************************************************************/
// ���������� �������� �� ������, ������������ � Mass+z_begin ����������
/************************************************************************/
char t_Slowo3 :: is_struct( long z_begin )
{  long i1 ;
   for( i1=z_begin+1 ; i1<n_Mass && Mass[i1]!='[' ; i1++ )
      if( Mass[i1]==':' ) return 1 ;
   return 0 ;
}
char is_struct( char *Mass )
{  long i1 ;
   for( i1=0 ; Mass[i1]!=0 && Mass[i1]!='[' ; i1++ )
      if( Mass[i1]==':' ) return 1 ;
   return 0 ;
}
/***************************************************************************/
//           ������ ���������� ��������� ��������� ���������
//    fdst   - 0-�������� 1-��������
//    S      - ����������� ���������
//    Str    - ������, ���������� ���������, �������� "�����������,�������������"
/***************************************************************************/
void t_Slowo3 :: make_relation1( char fdst, t_sStruct *S, char *Str )
{
   short  i,j,j_param ;
   char   c,Str1[20] ;
   t_Form F ;

   for( i=j=j_param=0 ; i<100 ; i++ )
   {  c=Str[i] ;
      if( c==' ' ) continue ;
      if( c==0 || c==',' )
      {   Str1[j]=0 ; j=0 ;
          F.value[j_param]=Grammar.find_value1( S->i_struct, j_param, fdst, Str1 ) ;
	  j_param++ ;
      }
      if( c==0 ) break ;
      if( c==',' ) continue ;
      Str1[j++]=c ;
   }
   S->Param=F ;
}
/***************************************************************************/
//           ������� ������� �������� ���������� ��� ���������,
//           � ������ ���������� ���������
//    fdst   - 0-�������� 1-��������
//    S      - ����������� ���������
//    i_word - ������ ����� ���������  t_sStruct->Word[i_word]
//    Str    - ������, ���������� ���������, �������� "�����������,�������������"
//    j_Relation - ������ �� ������ ��������� ������� � ������� Relation
/***************************************************************************/
void t_Slowo3 :: make_relation2( char fdst, t_sStruct *S, long i_word, 
                  char *Str, long &j_Relation )
{
   t_sWord      *W=Word+S->sy_word+i_word ;
   t_Param1List *pParam ;
   t_Relation    R ;
   t_Form        F ;
   short  i,i1,j,j_param,v ;
   char   c,Str1[20] ;

try
{
   if( i_word<0 )
   {  StrErr="make_relation i_word<0 !!!" ; 
      error_set( StrErr.c_str() );
      throw(-1);  
   }

   F.init();
   for( i=j=j_param=0 ; i<100 ; i++ )
   {  c=Str[i] ;
      if( c==' ' ) continue ;
      if( c==0 || c==',' )
      {   Str1[j]=0 ; j=0 ;

          v=Grammar.find_value1( W->i_struct, j_param, fdst, Str1 ) ;

          if( 0<=v || 0==strcmp(Str1,"@0") )
	      F.value[j_param]=v ;
	  else
	  {
             if( S->i_struct<0 )
	     {  StrErr="make_relation1::S->i_struct<0" ;
	        error_set( StrErr.c_str() );
		throw(-1);
	     }

	     if( fdst==0 )
	              pParam=&Grammar[S->i_struct].From.Param ;
		 else pParam=&Grammar[S->i_struct].To.Param ;

	     i1=find_param1( pParam, Str1 );

	     if( i1<0 )
	     {  StrErr="make_relation2::�������� ��������� �� ������" ;
	        error_set( StrErr.c_str() );
	        throw(-1);
	     }

	     if( pParam->list[i1].Dir==1 )
	     {  // ----- �������� ���������� �� ��������� � �� �����
	     	R.s1=0 ;
		R.p1=i1 ;
		R.s2=i_word+1 ;
		R.p2=j_param ;
	     }
	     else
	     {  // ----- �������� ���������� �� ����� � ���������
	     	R.s2=0 ;
		R.p2=i1 ;
		R.s1=i_word+1 ;
		R.p1=j_param ;
	     }
	     Relation[j_Relation++]=R ;
	  }
	  j_param++ ;
      }
      if( c==0 ) break ;
      if( c==',' ) continue ;
      Str1[j++]=c ;
   }
   W->Param=F ;
}
catch( int E )
{  error_add( "t_Slowo3 :: make_relation2\n" );
   throw( E );
}
}
/********************************************************************************/
void t_sRecord1 :: make_relation2( char fdst, t_sStruct *S, long i_word, char *Str )
{
   t_sWord      *W=&Word[S->sy_word+i_word] ;
   t_Param1List *pParam ;
   t_Relation    R ;
   t_Form        F ;
   short  i,i1,j,j_param,v ;
   char   c,Str1[20] ;

try
{
   if( i_word<0 )
   {  StrErr="make_relation i_word<0 !!!" ; 
      error_set( StrErr.c_str() );
      throw(-1);  
   }

   F.init();
   for( i=j=j_param=0 ; i<100 ; i++ )
   {  c=Str[i] ;
      if( c==' ' ) continue ;
      if( c==0 || c==',' )
      {   Str1[j]=0 ; j=0 ;

          v=Grammar.find_value1( W->i_struct, j_param, fdst, Str1 ) ;

          if( 0<=v || 0==strcmp(Str1,"@0") )
	      F.value[j_param]=v ;
	  else
	  {
             if( S->i_struct<0 )
	     {  StrErr="make_relation1::S->i_struct<0" ;
	        error_set( StrErr.c_str() );
		throw(-1);
	     }

	     if( fdst==0 )
	              pParam=&Grammar[S->i_struct].From.Param ;
		 else pParam=&Grammar[S->i_struct].To.Param ;

	     i1=find_param1( pParam, Str1 );

	     if( i1<0 )
	     {  StrErr="make_relation2::�������� ��������� �� ������" ;
	        error_set( StrErr.c_str() );
	        throw(-1);
	     }

	     if( pParam->list[i1].Dir==1 )
	     {  // ----- �������� ���������� �� ��������� � �� �����
	     	R.s1=0 ;
		R.p1=i1 ;
		R.s2=i_word+1 ;
		R.p2=j_param ;
	     }
	     else
	     {  // ----- �������� ���������� �� ����� � ���������
	     	R.s2=0 ;
		R.p2=i1 ;
		R.s1=i_word+1 ;
		R.p1=j_param ;
	     }
	     Relation.add(R) ;
	  }
	  j_param++ ;
      }
      if( c==0 ) break ;
      if( c==',' ) continue ;
      Str1[j++]=c ;
   }
   W->Param=F ;
}
catch( int E )
{  error_add( ":: make_relation2\n" );
   throw( E );
}
}
/***************************************************************************/
t_String t_Slowo3 :: get_strerr()
{  return StrErr ;
}
/***************************************************************************/
//               ��������� ������ ������ ���� � ���������
/***************************************************************************/
void t_Slowo3 :: make_tree( void )
{
   long  i,i1,j ;
   short s,s1,n ;
   char  *Str,*Str1 ;
   t_sStruct *S,*S1 ;
   t_sTree   *T ;

   Free(Tree);
   n_Tree=0 ;
   if( n_Record<=0 ) return ;

   Tree=(t_sTree *)Calloc( 2+4*n_Word,sizeof(t_sTree) );

   Tree[0].up=-1 ;
   Tree[0].down=1 ;
   Tree[0].first =0 ;
   Tree[0].last  =n_Record ;
   Tree[0].rang  =-2 ;
   // ----------- ������ ������� - �� �������� �������� --------
   Tree[1].first=0 ;
   Tree[1].rang =-1 ;
   s=Struct[Record[0].sy_struct].i_struct ;

   for( i=0,j=2 ; i<n_Record ; i++ )
   {  s1=Struct[Record[i].sy_struct].i_struct ;
      if( s1!=s )
      {   Tree[j-1].last = i-1 ;
	  Tree[j].first  = i ;
	  Tree[j].rang   =-1 ;
	  j++ ;
	  s=s1 ;
      }
   }
   Tree[j-1].last=n_Record-1 ; // n_Record ; (2 ������ 2005�)
   Tree[0].n_down=j-1 ;
   // ----------- ��������� ������ - �� �������� ���� --------
   for( i=1 ; i<j ; i++ )
   {  T=&Tree[i] ;
      n=T->rang+1 ;

      S=&Struct[Record[T->first].sy_struct] ;
      if( (T->last-T->first)<=1 && S->n_Word<=n )
      {
         T->down=-1 ;
	 T->n_down=0 ;
         continue ;
      }
      T->down=j ;
      S =&Struct[Record[T->first].sy_struct] ;
      S1=&Struct[Record[T->last ].sy_struct] ;
      if( S->n_Word<=n  ) s =-2 ; else s =S->Word[n].i_struct ;
      if( S1->n_Word<=n ) s1=-2 ; else s1=S1->Word[n].i_struct ;
      if( s!=s1 )
      {   // ---- ��������� � ������������� ��������� ----
          T->empty=2 ;
	  s=-10 ;
          for( i1=T->first ; i1<=T->last ; i1++ )
	  {  S=&Struct[Record[i1].sy_struct] ;
	     if( S->n_Word<=n ) s1=-2 ;
		 else           s1=S->Word[n].i_struct ;
	     if( s1!=s )
	     {   if( i1!=0 && s!=-10 ) Tree[j-1].last =i1-1 ;
		 Tree[j].up     =i ;
		 Tree[j].first  =i1 ;
		 Tree[j].rang   =n-1 ;
		 Tree[j].empty  =1 ;
		 s=s1 ;
		 j++ ;
	      }
	  }
	  Tree[j-1].last=T->last ;
	  T->n_down     =j-T->down ;
      }
      else
      {   // ---- ��������� � ����������� ��������� ----
          Str="\7" ;
	  for( i1=T->first ; i1<=T->last ; i1++ )
	  {  S=&Struct[Record[i1].sy_struct] ;
	     if( S->n_Word<=n ) Str1="" ;
		 else           Str1=(char *)Mass+S->Word[n].sy ;
	     if( 0!=Strcmp( Str1,Str ) || S->n_Word==(n+1) )
	     {   if( i1!=T->first ) Tree[j-1].last =i1-1 ;
		 Tree[j].up     =i ;
		 Tree[j].first  =i1 ;
		 Tree[j].rang   =n ;
		 Str=Str1 ;
		 j++ ;
	     }
	  }
	  Tree[j-1].last=T->last ;
	  T->n_down     =j-T->down ;
       }
   }
   n_Tree=j ;
   Tree=(t_sTree *)Realloc( Tree,j*sizeof(t_sTree));
}
/***************************************************************************/
//             ���������� ��� c�������� ���� "����������"
/***************************************************************************/
void t_Slowo3 :: split_unorder( void )
{  long i,i1,i2,f,ns,z,N ;
   long j_Record,  j_Struct,  j_Word,  j_Relation ;
   long n_Record1, n_Struct1, n_Word1, n_Relation1 ;
   e_Type     t ;
   t_sRecord *R ;
   t_sStruct *S ;

   // --------- ��������� ��������� �������������� ������ -------------   
   n_Record1=n_Struct1=n_Word1=n_Relation1=0 ;

   for( f=i=0 ; i<n_Record ; i++ )
   {  S=&Struct[Record[i].sy_struct] ;
      if( S->i_struct<0 ) continue ;
      t=Grammar[S->i_struct].From.type ;
      if( t==TUNORDER1 || t==TUNORDER2 )
      {   N=Factorial.fak[S->n_Word]-1 ;
          n_Record1  +=N ;
	  n_Struct1  +=N*Record[i].n_struct ;
	  n_Word1    +=N*S->n_Word ;
	  n_Relation1+=N*(Relation[S->i_relation].s1+1) ;
      }
   }
   if( n_Record1==0 ) return ;

   long NN   =n_Record ;
   // --------- ������� ������ ----------------------------------------
   j_Record  =n_Record   ; n_Record  +=n_Record1 ;
   j_Struct  =n_Struct   ; n_Struct  +=n_Struct1 ;
   j_Word    =n_Word     ; n_Word    +=n_Word1 ;
   j_Relation=n_Relation ; n_Relation+=n_Relation1 ;
   Record  =(t_sRecord  *)Realloc( Record  ,(n_Record+1)* sizeof( t_sRecord  ) );
   Struct  =(t_sStruct  *)Realloc( Struct  ,(n_Struct+1)* sizeof( t_sStruct  ) );
   Word    =(t_sWord    *)Realloc( Word    ,n_Word      * sizeof( t_sWord    ) );
   Relation=(t_Relation *)Realloc( Relation,n_Relation  * sizeof( t_Relation ) );

   // ---- ���������� ����������� �������� -----------------
   // ---- c Relation �� ������� ��� ������
   for( f=i=0 ; i<NN ; i++ )
   {  R=&Record[i] ;
      S=&Struct[Record[i].sy_struct] ;
      if( S->i_struct<0 ) continue ;
      t=Grammar[S->i_struct].From.type ;
      if( t==TUNORDER1 || t==TUNORDER2 )
      {   N=Factorial.fak[S->n_Word] ;
          for( i1=1 ; i1<N ; i1++ )
	  {  Record[j_Record]=*R ;
	     Record[j_Record].sy_struct=j_Struct ;
	     Struct[j_Struct]=*S ;
	     Struct[j_Struct].sy_word=j_Word ;
	     for( i2=0 ; i2<S->n_Word ; i2++ )
	     {  z=Factorial.value[i1*Factorial.n+i2] ;
	        Word[j_Word++]=S->Word[z] ;
	     }
	     Relation[j_Relation++].s1=ns=Relation[S->i_relation].s1 ;
	     for( i2=0 ; i2<ns ; i2++ )
	     {  t_Relation R1=Relation[S->i_relation+i2+1] ;
	        Relation[j_Relation]   =R1 ;
	        Relation[j_Relation].s1=Factorial.use(i1,R1.s1) ;
		Relation[j_Relation].s2=Factorial.use(i1,R1.s2) ;
		j_Relation++ ;
	     }
	     j_Struct++ ;
	     for( i2=1 ; i2<R->n_struct ; i2++ )
	        Struct[j_Struct++]=S[i2] ;
	     j_Record++ ;
	  }
      }
      if( n_Record<j_Record || n_Struct<j_Struct || n_Word<j_Word || n_Relation<j_Relation )
      {   printf("\n���������� ������ � t_Slowo3 :: split_unorder\n"); 
          exit(-1);  
      }
   }
   // ----- ��������� �������� �������� ---------------------------
   Struct[j_Struct].sy_word   =j_Word ;
   Struct[j_Struct].i_relation=j_Relation ;
   Record[j_Record].sy_struct =j_Struct ;

   // ----- ��������� ������ ���� ���� ----------------------------
   for( i=0 ; i<n_Struct ; i++ )
      Struct[i].Word=Word+Struct[i].sy_word ;

   for( i=0 ; i<n_Word ; i++ )
      Word[i].str=(char *)Mass+Word[i].sy ;
}
/***************************************************************************/
//              ������� �������������� �������� (������-������)
/***************************************************************************/
int funk0( const void *a, const void *b )
{  t_sStruct *S1,*S2 ;
   short f,iw,nw,is1,is2 ;

   S1=Slowo3->sstruct((*(t_sRecord **)a)->sy_struct) ;
   S2=Slowo3->sstruct((*(t_sRecord **)b)->sy_struct) ;

   if( S1->i_struct!=S2->i_struct ) return S1->i_struct-S2->i_struct ;

   nw=min( S1->n_Word,S2->n_Word ) ;
   for( iw=0 ; iw<nw ; iw++ )
   {
      is1=S1->Word[iw].i_struct ;
      is2=S2->Word[iw].i_struct ;
      if( is1 != is2 ) return is1-is2 ;
      f=Strcmp( S1->Word[iw].str,S2->Word[iw].str );
      if( f!=0 ) return f ;
   }
   if( S1->n_Word != S2->n_Word ) return S1->n_Word - S2->n_Word ;
   return (*(t_sRecord **)a)->i_dict - (*(t_sRecord **)b)->i_dict ;
}
/***************************************************************************/
//            ������������� ��������� � ������� �������
/***************************************************************************/
void t_Slowo3 :: sort0( void )
{  long i ;
   t_sRecord *R, **RR ;
   
   Slowo3=this ;
   RR=(t_sRecord **)Malloc( n_Record,sizeof(t_sRecord *) );
   for( i=0 ; i<n_Record ; i++ )
      RR[i]=Record+i ;

   qsort( RR,n_Record,sizeof(t_sRecord *),funk0 );

   R =(t_sRecord *)Calloc( n_Record+1,sizeof(t_sRecord) );
   for( i=0 ; i<n_Record ; i++ )
      R[i]=*RR[i] ;
   Free( Record ); Free( RR );
   Record=R ;
}
/***************************************************************************/
//         ��������� ������������ �������� (��� ��������-2)
/***************************************************************************/
char * t_sRecord1 :: parse2( char *Str, t_Format1 *Format )
{  short i,f1,f2,f3 ;   
   char  c,f_struct1,f_struct2 ;

   try
   {
      f_struct1=f_struct2=0 ;
      //n_Mass = n_Word = n_Struct = n_Record = n_Tree = n_Relation = 0 ;
      for( i=0 ; Str[i]!=0 ; i++ )
         if( Str[i]=='=' ) goto M1;
      return "� ������ ��� ����� �����" ;
M1:   for( i=0 ; Str[i]!='=' ; i++ )
      {  if( Str[i]==':' ) f_struct1++ ;
         if( Str[i]=='[' ) goto M2;
      }
      return "� ������ ��� ���������� ������ (�� ����� �����)" ;
M2:   for( i=0 ; Str[i]!=0 ; i++ )
      {  if( Str[i]==':' ) f_struct2++ ;
         if( Str[i]=='[' ) goto M3;
      }
      return "� ������ ��� ���������� ������ (����� ����� �����)" ;
M3:   f1=f2=f3=0 ;
      for( i=0 ; Str[i]!=0 ; i++ )
      {  c=Str[i] ;
	 if( '['==c ) f1++ ;
	 if( ']'==c ) f1-- ;
	 if( '('==c ) f2++ ;
	 if( ')'==c ) f2-- ;
	 if( '<'==c ) f3++ ;
	 if( '>'==c ) f3-- ;
	 if( '='==c && ( f1!=0 || f2!=0 || f3!=0) )
             break ;
      }      
      if( f1!=0 || f2!=0 || f3!=0 )
	  return "������ ���������� ������" ;
      if( 1<f_struct1 || 1<f_struct2 )
	  return "��! � ����� �� ��� ����� ��������� ���������?" ;
      if( f_struct1!=f_struct2 )
	  return "����� ������ ������������ ������, ��������� - ����������" ;

      parse1( Str, Format, 1 );

      short    i_struct1 = Struct[0].i_struct ;
      short    i_struct2 = Struct[1].i_struct ;
      e_Type   type1 = Grammar[i_struct1].From.type ;
      e_Type   type2 = Grammar[i_struct2].To.type ;
 /*
      if( type1==TWORD && i_struct1!=i_struct2 )
	  return "�������������� ����� ���� ��������� � ��������" ;
      if( (type1==TSTRUCT1 || type1==TSTRUCT2) && !(type2==TSTRUCT1 || type2==TSTRUCT2) )
	  return "�������������� ����� ���� ��������� � ��������" ;
*/
      return "" ;
   }
   catch( ... )
   {  return (char *)StrErr.c_str() ;
   }
}
/***************************************************************************/
//          ��������� ����� �� ��������� ����� (������������)
/***************************************************************************/
void t_Slowo3 :: s_universe( short i_word, short i_struct )
{
   long  i,i1,i2,j,z,N1,i_tree ;
   char *Str ;
   t_sTree    *T ;
   t_sStruct  *S ;
   t_sWord    *W ;
   t_Variants *VV ;
   t_rStruct   SS ;
   t_rWord     R,*pR ;
   t_Tree      N,*pN ;
   t_TreeList  Node ;

   if( n_Tree<=0 ) return ;
   if( Core.From.j<=i_word ) return ;
   i_tree=root_of_struct( i_struct );
   if( i_tree<0 ) return ;

   // ---- ���������� ������ ��������� ------
   N.link=i_tree ;
   N.i_last_word=i_word ;
   Node.add(N);

   for( i=0 ; i<Node.j ; i++ )
   {  i_tree=Node[i].link ;
      T=&Tree[i_tree] ;

      if( Node[i].type==TNULL ) continue ; //  ��������� �������

      if( T->empty==2 )
      {   // ---- ������ � ������������� ����������� � ������� ��������� ----
	  for( i1=0 ; i1<T->n_down ; i1++ )
	  {  z=T->down+i1 ;
	     N.type       = TSTRUCT ;
	     N.up         = i ;
	     N.i_word     = Node[i].i_last_word ;
	     N.i_last_word= Node[i].i_last_word ;
	     N.link       = z ;
	     N.i_struct   =-1 ;
	     N.i_variant  = 0 ;
	     N.index      = T->rang ; // T->rang+1 
	     N.i_slowo    =-1 ;
	     Node.add(N);
	  }
	  continue ;
      }

      // --------- ������������� ������ --------------
      S=Struct+Record[T->first].sy_struct ;
      W=&S->Word[T->rang+1] ;

      if( W->i_struct==-1 )
      {  // ------ ����� �������� ���������� ---------
         if( Core.n_word<=Node[i].i_last_word )
	     Antwort.j=0 ;
	 else
	     find_bin( Core.From[Node[i].i_last_word].Str1, i_tree );
	 for( i1=0 ; i1<Antwort.j ; i1++ )
	 {  z=Antwort[i1] ;
	    S=Struct+Record[Tree[z].first].sy_struct ;
	    W=&S->Word[Tree[z].rang] ;
	    for( i2=j=1 ; W->str[i2]!=0 ; i2++ )
	       if( W->str[i2]==' ' && W->str[i2-1]!=' ' ) j++ ;
	    N.type       = TSTRUCT ; // ������������
	    N.up         = i ;
	    N.i_word     = Node[i].i_last_word ;
	    N.i_last_word= Node[i].i_last_word+j ;
	    N.link       = z ;
	    N.i_struct   =-1 ;
	    N.i_variant  = i1 ;
	    N.index      = T->rang+1 ;
	    N.i_slowo    =-1 ;
	    if( Tree[z].n_down<=0 )
		N.type=TNULL ;
	    Node.add(N);
	 }
      }
      else
      {  // ------ ����� ����� ����� ���������� ����� ------
         if( Core.From.j<=Node[i].i_last_word ) 
	     continue ;
	 Core.universe( Node[i].i_last_word,W->i_struct );
	 VV=Core.variants( Node[i].i_last_word,W->i_struct );
	 // ---- ������� ���� ����� --------
	 // ---- ��� ����� ��������� ������� ���������� �������� ���������
	 // ---- (��������� � ���, ��� ��-������ ��� ����� 10-� ��������� �����������)
	 //for( i1=0 ; i1<VV->Variant.j ; i1++ )
	 //if( 0<VV->Variant.j )
	 long NN ;
	 if( IF_WORD( Grammar[W->i_struct].From.type ) )
	          NN=min( 1,VV->Variant.j ) ;
	     else NN=VV->Variant.j ;
	 for( i1=0 ; i1<NN; i1++ )	 
	 {  // ------- ���� �� ��������� -------------
	    if( 0==W->str[0] ) // ������ ��� ����
	             Str="" ;
	        else Str=Core.get_meaning( &VV->Variant[i1] ) ; // ����� ��������
	    find_bin( Str, i_tree );
	    for( i2=0 ; i2<Antwort.j ; i2++ )
	    {  z=Antwort[i2] ;
	       N.type       = TSTRUCT ; // ������������
	       N.up         =i ;
	       N.i_word     =VV->Variant[i1].i_word ;
	       N.i_last_word=VV->Variant[i1].i_last_word ;
	       N.link       =z ;
	       N.i_struct   =W->i_struct ;
	       N.i_variant  =i1 ;
	       N.index      =T->rang+1 ;
	       N.i_slowo    =VV->Variant[i1].i_slowo ;
	       if( Tree[z].n_down<=0 )
		   N.type=TNULL ;
	       Node.add(N);
	    }
	 }
      }
   }
   // ---- ��������� ��������� ----------------
   t_Struct *S1=&Grammar[i_struct].From ;
   VV=Core.variants( i_word, i_struct );
   for( i=0 ; i<Node.j ; i++ )
   {  if( Node[i].type!=TNULL ) continue ;
      // ----- ���������� ��������� ��������� ---------
      T=&Tree[Node[i].link] ;
      SS.type       =S1->type ;
      SS.i_word     =i_word ;
      SS.i_last_word=Node[i].i_last_word ;
      SS.i_struct   =i_struct ;
      SS.r_word     =Core.rWord.j ;
      SS.i_dic      =i_dic ;
      SS.i_slowo    =T->first ;
      SS.Form       =Struct[Record[T->first].sy_struct].Param.form() ;
      N1=get_n_perevod( SS.i_slowo );

      if( 0==is_atom(SS.i_slowo) ) // � ����� ���� �� ������� �������� ��������
          N1=min( 1,N1 );          // �� IF_WORD � �� ���, ��� ��� ������ �����

      // ---- ���� �� ��������� �������� ���� ��������� -----
      for( i1=0 ; i1<N1 ; i1++)
      {  
         SS.i_slowo1=i1 ;
         // ----- ��������, �� �� ���������� ������������ ��������� ---
	 for( i2=0 ; i2<=T->rang ; i2++ )
	    Core.rWord.add(R);

         // ----- ���������� ������������ ��������� -----------------------
	 // ----- (� ������� ����� ������� �� ����� ������ � ��� �����) ---
	 z=i ;
	 while( 1 )
	 {  pN=&Node[z] ;
	    if( Tree[pN->link].empty!=1 )
	    {   pR =&Core.rWord[SS.r_word+Tree[pN->link].rang] ;
		if( pN->i_struct<0 )
	        	 pR->type =TCONST ;
                    else pR->type =Grammar[pN->i_struct].From.type ;
		pR->i_word     =pN->i_word ;
		pR->i_last_word=pN->i_last_word ;
		pR->i_struct   =pN->i_struct ;
		pR->i_variant  =pN->i_variant ;
		pR->index      =pN->index ;
		pR->i_dic      =i_dic ;
		pR->i_slowo    =pN->i_slowo ;
		pR->i_slowo1   =0 ;           // �� ������
	    }
	    z=pN->up ;
	    if( z==0 ) break ;
	 }
	 VV->Variant.add(SS);
	 Core.real_param_up( &VV->Variant[VV->Variant.j-1] );
      }
   }
}
/**************************************************************************/
//                      �������������� ������� �����
//   From     - ����������� �����
//   i_slowo  - ������ ������ � ������� t_Slowo3->Record[i_slowo]
//   i_slowo1 - ������� ��������
/**************************************************************************/
char * t_Slowo3 :: translate_word_i( char *From, long i_slowo, short i_slowo1 )
{
   strcpy( Word0,From );
   if( i_slowo<0 ) return Word0 ;
   
   t_sStruct *S=&Struct[Record[i_slowo].sy_struct+1+i_slowo1] ;

   Word0[0]=0 ;   
   for( short i=0 ; i<S->n_Word ; i++ )
   {  strcat( Word0,S->Word[i].str ); 
      if( i<S->n_Word-1 ) strcat( Word0," " );  
   }
   //strcpy( Word0,(char *)Mass+Struct[Record[i_slowo].sy_struct+1+i_slowo1].Word[0].sy );
   return Word0 ;
}
/**************************************************************************/
//                      �������������� ������� �����
//   From     - ����������� �����
//   i_struct - ����� ����, ������� ������ ���� ��� �����
//   i_slowo1 - ������� ��������
/**************************************************************************/
char * t_Slowo3 :: translate_word_s( char *From, short i_struct, short i_slowo1 )
{
   t_longList Ant ;
   e_Type t ;
   long   a ;
   
   if( 0<=i_struct )
   {   t=Grammar[i_struct].To.type ;
       if( t==TSTRUCT1 || t==TSTRUCT2 )
       {   strcpy( Word0,"" );  return Word0 ;  }
   
       if( Grammar[i_struct].To.type!=TWORD )
           return NULL ; // ������� ������������� ������
   }
   strcpy( Word0,From );
   if( n_Tree<=0 ) return Word0 ;
   
   find( From, i_struct, &Ant );
   if( Antwort.j<=0 ) return Word0 ; //  ������ ����� ��� � �������

   a=Antwort[0] ;
   if( Record[a].n_struct<=i_slowo1+1 ) i_slowo1=0 ;
   strcpy( Word0,(char *)Mass+Struct[Record[a].sy_struct+1+i_slowo1].Word[0].sy );
   return Word0 ;
}
/***************************************************************************/
//     ����� ����� � ������
//     From1  - ������� �����
//     i_struct - ��������� ����� ����
//     *_a, *_b  �������� ������� � t_sRecord ����������� ��� �����
/***************************************************************************/
void t_Slowo3 :: find( char *From1, short i_struct, t_longList *Ant )
{
   long   i,zz ;

   Antwort.j=0 ;
   Ant->j=0 ;
   if( From1[0]==0 ) return ;
   if( n_Tree<=0 ) return ;
   
   zz=root_of_struct( i_struct );
   if( zz<0 ) return ;

   find_bin( From1, zz );
   
   for( i=0 ; i<Antwort.j ; i++ )
   {  Antwort[i]=Tree[Antwort[i]].first ;
      Ant->add( Antwort[i] );
   }
}
/***************************************************************************/
//     ����� ��������� � ������, ������� � ������������ �����
//     ���� �� ����������, ��������� ���������, �� ��� ��� ���� �� ��������
//     From1  - ������� �����
//     zz - ����� ������ ������
/***************************************************************************/
char t_Slowo3 :: find_bin( char *From1, long zz )
{
   long   i ;
   char   From2[100] ;

   Antwort.j=0 ;
   for( i=0 ; i<100 ; i++ )
   {  if( From1[i]==0 )
      {   From2[i]=0 ; find_bin1( From2, zz ); break ;  }
      if( From1[i]==' ' )
      {   From2[i]=0 ; 
          if( 0==find_bin1( From2, zz ) ) break ;
	  From2[i]=' ' ; 
      }
      From2[i]=From1[i] ;
   }
   if( Antwort.j<=0 ) return -1 ;
   return 0 ;
}
/***************************************************************************/
//     ����� ��������� � ������, ������� � ������������ �����
//     � ��� ������ ��������� �������, ���� ��� � ������� ��� ���
//     From1  - ������� �����
//     i_tree - ����� ������ ������
/***************************************************************************/
char t_Slowo3 :: find_bin1( char *From1, long i_tree )
{
   long   a,b,b1,c,i,L,L1 ;
   char   f, *Str ;
   char   ff=0 ;     // ���� ������� ������ ��������� � ���� Form1

   L=strlen(From1);
   a=Tree[i_tree].down ;
   b=b1=a+Tree[i_tree].n_down-1 ;
   while( 1 )
   {
      if( b-a<4 )
      {   for( i=a ; i<=b ; i++ )
	     if( 0==Strncmp( From1,word_src(i),L ) ) break ;
	  a=i ;
	  break ;
      }
      c=(a+b)/2 ;
      f=Strncmp( From1,word_src( c ),L );
      if( f<0 ) b=c ;
      if( f>0 ) a=c ;
      if( f==0 )
      {	  for( i=c ; a<i ; i-- )
	     if( 0!=Strncmp( From1,word_src(i),L) ) {  a=i+1 ; break ;  }	  
	  break ;
      }
   }
   for( i=a ; i<=b && i<=b1 ; i++ )
   {  Str=word_src(i);      
      f=Strncmp( From1,Str,L );
      if( 0>f ) break ;
      L1 =strlen(Str);      
      if( L==L1 && f==0 ) Antwort.add(i) ;
          else    ff=1 ;
      if( 0==L && 0<L1 ) break ;
   }
   return ff ;
}
/***************************************************************************/
//    ����� ����� � ������, � ������� ���������� ��������� ���� i_struct
/***************************************************************************/
long t_Slowo3 :: root_of_struct( short i_struct )
{  long i ;

   for( i=0 ; i<Tree[0].n_down ; i++ )
      if( Struct[Record[Tree[1+i].first].sy_struct].i_struct==i_struct )
          return i+1 ;
   return -1 ;
}
/***************************************************************************/
//                ������ ������� ���� �������� ����� From
/***************************************************************************/
short t_Slowo3 :: part( char *From, short *i_Part )
{  short i,j ;
   t_sStruct *S ;
   t_longList Ant ;

   if( n_Tree<=0 ) return j ;
   for( i=j=0 ; i<Tree[0].n_down ; i++ )
   {
      S=Struct+Record[Tree[1+i].first].sy_struct ;
      if( S->i_struct<0 ) continue ;
      if( Grammar[S->i_struct].From.type!=TWORD ) continue ;
      find( From, S->i_struct,&Ant );
      if( 0<Ant.j )
          i_Part[j++]=S->i_struct ;
   }
   return j ;
}
/***************************************************************************/
//                ���������� ��������� ��������� i_slowo
/***************************************************************************/
t_Form t_Slowo3 :: struct_param( long i_slowo, long i_variant )
{
   t_Form     Form0 ;
   t_sStruct *S ;

   if( i_slowo<0 || n_Record<=i_slowo ) return Form0 ;
   if( i_variant==-1 )
            S=&Struct[Record[i_slowo].sy_struct] ;
       else S=&Struct[Record[i_slowo].sy_struct+1+i_variant] ;
   return S->Param.form() ;
}
/***************************************************************************/
//                ���������� ��������� ����� i_slowo
/***************************************************************************/
t_Form t_Slowo3 :: word_param( long i_slowo, long i_variant, long i_word )
{
   t_Form     Form0 ;
   t_sStruct *S ;

   if( i_slowo<0 || n_Record<=i_slowo ) return Form0 ;
   if( i_variant<-1 || Record[i_slowo].n_struct<=i_variant-1 ) return Form0 ;
   if( i_variant==-1 )
            S=&Struct[Record[i_slowo].sy_struct] ;
       else S=&Struct[Record[i_slowo].sy_struct+1+i_variant] ;
   if( i_word<0 || S->n_Word<=i_word ) return Form0 ;
   return S->Word[i_word].Param.form() ;
}
/***************************************************************************/
//
/***************************************************************************/
t_Format1 * t_Slowo3 :: format( void )
{
   return Format ;
}
/***************************************************************************/
//
/***************************************************************************/
void t_Slowo3 :: set_format( t_Format1 *F )
{
   Format=F ;
}
/***************************************************************************/
//
/***************************************************************************/
t_sRecord * t_Slowo3 :: record( long i_record )
{
   return &Record[i_record] ;
}
/***************************************************************************/
//
/***************************************************************************/
long t_Slowo3 :: n_record( void )
{
   return n_Record ;
}
/***************************************************************************/
//        ���� �� � ������ ����������� ����������� �������
//        ���������� 1 ���� ����
/***************************************************************************/
char t_Slowo3 :: is_atom( long i_record )
{
   if( i_record<0 ) return 1 ;

   t_sStruct *S=&Struct[Record[i_record].sy_struct] ;

   for( long i=0 ; i<S->n_Word ; i++ )
   {  if( S->Word[i].i_struct<0 ) continue ;
      e_Type t=Grammar[S->Word[i].i_struct].From.type ;
      if( IF_CONSTR(t) ) return 1 ;
   }
   return 0 ;
}
/***************************************************************************/
//              ������� ������ ���������
/***************************************************************************/
t_sTree * t_Slowo3 :: tree( long i_tree )
{
   return &Tree[i_tree] ;
}
/***************************************************************************/
//
/***************************************************************************/
t_sStruct * t_Slowo3 :: sstruct( long i_struct )
{
   return &Struct[i_struct] ;
}
/***************************************************************************/
//              ���� ����������� ���������
/***************************************************************************/
t_sStruct * t_Slowo3 :: get_from( long i_slowo )
{
   if( i_slowo<0 || n_Record<=i_slowo ) return NULL ;
   return &Struct[Record[i_slowo].sy_struct] ;
}
/***************************************************************************/
//               ������� ���������
//               i_slowo   - ������ ����������� ������
//               i_variant - ������� ��������
/***************************************************************************/
t_sStruct * t_Slowo3 :: get_to( long i_slowo, long i_variant )
{
   if( i_slowo<0 || n_Record<=i_slowo ) return NULL ;
   if( Record[i_slowo].n_struct<i_variant+1 ) i_variant=0 ;
   return &Struct[Record[i_slowo].sy_struct+1+i_variant] ;
}
/***************************************************************************/
//               �������� ����������
//               i_slowo   - ������ ����������� ������
//               i_variant - ������� �������� (-1 - ��������)
/***************************************************************************/
t_RelationList1 t_Slowo3 :: get_relation( long i_slowo, long i_variant )
{
   t_RelationList1  RL ;
   t_sStruct       *S ;

   if( i_slowo<0 || n_Record<=i_slowo ) return RL ;
   if( i_variant==-1 )
            S=&Struct[Record[i_slowo].sy_struct] ;
       else S=&Struct[Record[i_slowo].sy_struct+1+i_variant] ;
   RL.list  =Relation+S->i_relation+1 ;
   RL.j=Relation[S->i_relation].s1 ;
   return RL ;
}
/***************************************************************************/
//             ���� ����� ��������� ��������� �� ������� ���������
//             ���� "������" � "����"
/***************************************************************************/
short t_Slowo3 :: get_n_perevod( long i_slowo )
{
   return Record[i_slowo].n_struct-1 ; // ABR
}
/***************************************************************************/
//             ���� ����� �� ������� ������
/***************************************************************************/
char * t_Slowo3 :: word_src( long i_tree )
{
   if( i_tree<0 || n_Tree<=i_tree )
   {   printf("\n Error t_Slowo3 :: word_src") ; throw(-1) ;  }
   return (char *)Mass+Struct[Record[Tree[i_tree].first].sy_struct].Word[Tree[i_tree].rang].sy ;
}
/***************************************************************************/
//             ���� ����� �� ������� ���������
/***************************************************************************/
char * t_Slowo3 :: word_src1( long i_slowo )
{
   return (char *)Mass+Struct[Record[i_slowo].sy_struct].Word[0].sy ;
}
/***************************************************************************/
//             ���������� �������
/***************************************************************************/
void t_Slowo3 :: print( char *File )
{
   FILE *fw=Fopen( File,"w" );
   for( long i=0 ; i<n_Record ; i++ )
      print_record( fw, i );
   Fclose( fw );
}
/***************************************************************************/
long t_Slowo3 :: print_mass( char **_Mass )
{  
   char    *Mass=NULL ;
   long     i,j_Mass,l_Mass ;
   t_String Str ;
try
{
   l_Mass=40*n_Record ;
   Mass=(char *)Calloc( l_Mass,sizeof(char) );
   j_Mass=0 ;
   
   for( i=0 ; i<n_Record ; i++ )
   {
      print_record( Str, i );
      if( l_Mass<=j_Mass+Str.length() )
      {   char *Mass1=(char *)Realloc( Mass,2*l_Mass*sizeof(char) );
          Mass=Mass1 ;
	  l_Mass=2*l_Mass ;
      }
      strcpy( Mass+j_Mass,Str.c_str() );
      j_Mass+=Str.length() ;
      Mass[j_Mass++]='\n' ;
   }
   *_Mass=Mass ;
   return j_Mass ;
}
catch( int E )
{  Free( Mass );
   *_Mass=NULL ;
   return -1 ;
}
}
/***************************************************************************/
//             ���������� ������ �������
/***************************************************************************/
void t_Slowo3 :: print_record( FILE *fw, long i_record )
{  t_String Str ;
   print_record( Str, i_record );
   fprintf( fw,"\n%s",Str.c_str() ) ;
}
/***************************************************************************/
//             ���������� ������ �������
/***************************************************************************/
void t_Slowo3 :: print_record( t_String &STmp, long i_record )
{
   t_sRecord *R=&Record[i_record] ;
   t_sStruct *S ;
   t_Struct  *SS ;
   long       i,j,is ;
   t_String  STmp1 ;

   STmp="" ;
   if( i_record<0 || n_Record<=i_record ) return ;
   
   S =&Struct[R->sy_struct];
   SS=&Grammar[S->i_struct].From ;
   
   // ------- ���� ��������� ----------------------------
   if( SS->type!=TWORD )
   {  // ------ ���������� ��������� ��������� ----------
      STmp+=Format->get_tag(0,S->i_struct) ;
      print_param( STmp1,0,S, 0 );
      STmp+=STmp1 ;
      STmp+=":" ;
   }
   for( i=0 ; i<S->n_Word ; i++ )
   {  
      is=S->Word[i].i_struct ;
      if( is<0 ) STmp+="@0" ;
          else   STmp+=Format->get_tag(0,is);
      if( S->Word[i].str[0]==0 ) 
                 STmp+="~" ;
          else { STmp+="[" ; STmp+=S->Word[i].str ; STmp+="]" ;  }
      if( 0<S->Word[i].order )
      {   STmp+="<" ; STmp+='0'+S->Word[i].order ; STmp+=">" ;  }
      print_param( STmp1,0,S, i+1 );
      STmp+=STmp1 ;
      if( i<S->n_Word-1 )
          STmp+=" " ;
   }
   STmp+="=" ;
   // ------- ���� �������� ----------------------------
   for( j=1 ; j<R->n_struct ; j++ )
   {
      S=&Struct[R->sy_struct+j] ;
      if( 1<j ) STmp+=";" ;
      if( SS->type!=TWORD )
      {  // ------ ���������� ��������� ��������� ----------
         STmp+=Format->get_tag(1,S->i_struct) ;
	 print_param( STmp1,1,S, 0 );
	 STmp+=STmp1 ;
	 STmp+=":" ;
      }
      for( i=0 ; i<S->n_Word ; i++ )
      {  
	 is=S->Word[i].i_struct ;
	 if( is<0 ) STmp+="@0" ;
             else   STmp+=Format->get_tag(1,is);
	 if( S->Word[i].str[0]==0 ) 
	              STmp+="~" ;
             else  {  STmp+="[" ; STmp+=S->Word[i].str ; STmp+="]" ;  }
	 if( 0<S->Word[i].order )
	 {   STmp+="<" ; STmp+='0'+S->Word[i].order ; STmp+=">" ;  }
         print_param( STmp1,1,S, i+1 );
	 STmp+=STmp1 ;
	 if( i<S->n_Word-1 )
             STmp+=" " ;
      }
   }
}
/***************************************************************************/
//
/***************************************************************************/
void t_Slowo3 :: print_struct( t_String &STmp, long i_record, long i_struct )
{  char To ;
   long       i,is ;
   t_sRecord *R=&Record[i_record] ;
   t_sStruct *S ;
   t_Struct  *SS ;
   t_String   STmp1 ;

   if( i_struct==0 ) To=0 ; else To=1 ;
   
   S =&Struct[R->sy_struct+i_struct] ;
   if( 0==To ) SS=&Grammar[S->i_struct].From ;
       else    SS=&Grammar[S->i_struct].To ;

   if( SS->type!=TWORD )
   {  // ------ ���������� ��������� ��������� ----------
      STmp+=Format->get_tag( To,S->i_struct) ;
      print_param( STmp1,To,S, 0 );
      STmp+=STmp1 ;
      STmp+=":" ;
   }
   for( i=0 ; i<S->n_Word ; i++ )
   {  
      is=S->Word[i].i_struct ;
      if( is<0 ) STmp+="@0" ;
          else   STmp+=Format->get_tag(To,is);
      if( S->Word[i].str[0]==0 ) 
	           STmp+="~" ;
          else  {  STmp+="[" ; STmp+=S->Word[i].str ; STmp+="]" ;  }
      if( 0<S->Word[i].order )
      {   STmp+="<" ; STmp+='0'+S->Word[i].order ; STmp+=">" ;  }
      print_param( STmp1,To,S, i+1 );
      STmp+=STmp1 ;
      if( i<S->n_Word-1 )
          STmp+=" " ;
   }
}
/***************************************************************************/
//             ���������� ���������
//             i_struct0 - ��� ��������� ���������
//             i_word    - ����� ����� � ���������(���������� � 1)
/***************************************************************************/
void t_Slowo3 :: print_param( FILE *fw, char to, t_sStruct *S, short i_word )
{  t_String Str ;
   print_param( Str, to, S, i_word );
   fprintf( fw,Str.c_str() );
}
/***************************************************************************/
//             ���������� ���������
//             i_struct0 - ��� ��������� ���������
//             i_word    - ����� ����� � ���������(���������� � 1)
/***************************************************************************/
void t_Slowo3 :: print_param( t_String &Str, char to, t_sStruct *S, short i_word )
{
   short  i,i1,ip,n ;
   short  i_struct ;
   char   *s ;
   t_Form      Param ;
   t_Struct   *SS,*SS1 ;
   t_Relation *RR=NULL ;
   
   Str="" ;
   n=0 ;
   if( 0<=S->i_relation && S->i_relation<n_Relation-1 )
   {   n =Relation[S->i_relation].s1 ;
       RR=&Relation[S->i_relation+1] ;
   }
   if( i_word==0 )
   {  i_struct=S->i_struct ;
      Param   =S->Param.form() ;
   }
   else
   {  i_struct=S->Word[i_word-1].i_struct ;
      Param   =S->Word[i_word-1].Param.form() ;
   }

   if( 0<=i_struct )
   {  if( to==0 ) SS=&Grammar[i_struct].From ;
	  else    SS=&Grammar[i_struct].To ;
   }
   if( 0<=S->i_struct )
   {  if( to==0 ) SS1=&Grammar[S->i_struct].From ;
          else    SS1=&Grammar[S->i_struct].To ;
   }
   for( i1=i=0 ; i<10 ; i++ )
      if( 0<=Param.value[i] ) i1++ ;
   for( i=0 ; i<n ; i++ )
      if( RR[i].s1==i_word || RR[i].s2==i_word ) i1++ ;
   if( i1==0 ) return ;

   if( 0<=i_struct )
   {  Str+="(" ;   
      for( i=0 ; i<SS->Param.j ; i++ )
      {  if( 0<=Param.value[i] )
	 {  // ------- ���������� �������� ����� -------------
      	    if( to==0 )
	    {   if( Grammar[i_struct].From.Param.j<=i ) break ;
        	ip=Grammar[i_struct].From.Param[i].param ;
		s =Grammar.from().Param[ip].Value[Param.value[i]].Name;
	    }
	    else
	    {   if( Grammar[i_struct].To.Param.j<=i ) break ;
        	ip=Grammar[i_struct].To.Param[i].param ;
		s =Grammar.to().Param[ip].Value[Param.value[i]].Name;
	    }
	    for( i1=0 ; i1<Format->Rename.j ; i1++ )
	       if( 0==strcmp(Format->Rename[i1].Full,s) ) 
	       {   s=Format->Rename[i1].Reduce ; break ;  }

	    if( 0<i ) Str+="," ;
	    Str+=s ;
	 }
	 else
	 {  // ------- ���������� ���������, ����������� � �����������
            s="@0" ;
	    
	    for( i1=0 ; i1<n ; i1++ )
	    {  if( RR[i1].s1==i_word && RR[i1].p1==i )
	       {   s=SS1->Param[i].Name ;
	           //if( i_word==0 ) s=SS1->Param[i].Name ;
		   //    else        s=SS1->Word[i_word-1].Param[RR[i1].p2].Name ;
                   goto M_continue ;
	       }
	       if( RR[i1].s2==i_word && RR[i1].p2==i )
	       {   s=SS1->Param[i].Name ;
	           //if( i_word==0 ) s=SS1->Param[i].Name ;
		   //    else        s=SS1->Word[i_word-1].Param[RR[i1].p1].Name ;
                   goto M_continue ;
	       }
	    }
	    M_continue :;
	    if( 0<i ) Str+="," ;
	    Str+=s ;
	 }
      }
      Str+=")" ;
   }
}
/************************************************************************/
t_sRecord1 :: t_sRecord1( )
{
    Mass=NULL ;
    f_Malloc=0 ;
    Relation.init();
    Word.init();
    Struct.init();
}
/************************************************************************/
t_sRecord1 :: ~t_sRecord1( )
{
    if( 1==f_Malloc ) free( Mass ) ;
    Relation.del();
    Word.del();
    Struct.del();
}
/************************************************************************/
//                �������� ���� ������ �������
//     ���������
//     2) ������� � �������� ��������
//        make_relation2( fdst, S1, iw, Reg, j_Relation ); // ����� � Relation
//     3) ������������� Grammar->Format
/************************************************************************/
void t_sRecord1 :: parse1( char *_Mass, t_Format1 *Format, char _f_Malloc )
{
   long  i,i1,i2,j,j1,L ;
   char  c,Reg[100] ;
   char  fformat,  // �������, ��� ����������� ������� ���������
         fstr,     // �������, ��� ����������� ��������� ���������
         fdst;     // �������, ��� ����������� ���������-��������
try
{
   // ------- ��������� ������ ���� ���� --------------
   if( 1==f_Malloc ) free( Mass ) ;
   if( _f_Malloc )
   {   // ---- �� ���������� ������ ------
       for( i=0 ; ; i++ )
          if( _Mass[i]=='\n' || _Mass[i]==0 )
	  {   L=i ; break ;  }
       Mass=(char *)Calloc( L+1,sizeof(char) );
       strcpy( Mass,_Mass );
   }
   else
   {  // ---- �� ������� ��������� ------
       Mass=_Mass ;
   }
   f_Malloc=_f_Malloc ;
   
   StrErr="" ;
   Struct.j=Word.j=Relation.j=0 ;
   t_sStruct  S ;
   t_sWord    W ;
   t_Relation R={0,0,0,0} ;

   fdst=0 ;
   // -------- ����������, �������� �� ������ ������ ��� ���������� -------
   fformat=fstr=is_struct( Mass ); // ����� �������� �������

   // -------- ������ ������ ----------------------------
   for( i1=0,j=0 ; Mass[i1]!=0 && Mass[i1]!='\n' ; i1++ )
   {
      c=Mass[i1] ;
      if( j<98 ) Reg[j++]=c ;
      if( c<0 || Perevod1.Type[c]==0 ) continue ;

      if( c=='=' )
      {  fdst=1 ;
	 fstr=fformat ;
	 fformat=fstr=is_struct( Mass+i1 );
	 j=0 ;
      }
      if( c==';' )
      {  fstr=fformat ;
         fformat=fstr=is_struct( Mass+i1 );
	 j=0 ;
	 continue ;
      }

      if( fstr==1 )
      {   // -------- ���� ����������� ��������� ��������� --------
	  if( c==':' || c=='(')
	  {   short i_str ;

	      if( Mass[i1-1]!=')' )
	      {
		  Reg[j-1]=0 ;
		  for( j1=i2=0 ; i2<j ; i2++ )
		     if( Reg[i2]!=' ' ) Reg[j1++]=Reg[i2] ;
		  S.sy_word =Word.j ;
		  i_str=Format->find_struct( fdst,Reg );
		  if( i_str<=-10 )
        	  {   StrErr="����������� ���\"" ; StrErr+=Reg ; StrErr+="\"" ; throw(-1);  }
		  S.i_struct=i_str ;
		  S.i_relation=Relation.j ;
		  S.Param.init();
		  Struct.add( S );
		  Relation.add(R);
	      }
	      if( c==':' ) fstr=0 ;
	      j=0 ;
	  }
	  if( c==')' )
	  {  Reg[j-1]=0 ;
	     t_sStruct *S1=&Struct[Struct.j-1] ;
             t_Slowo3::make_relation1( fdst, S1, Reg );
	     j=0 ;
	  }
      }
      else
      {   // -------- ���� ����������� ���� ��������� --------
	  if( c=='[' || c=='~' )
	  {   short i_str ;

	      Reg[j-1]=0 ;
	      for( j1=i2=0 ; i2<j ; i2++ )
		 if( Reg[i2]!=' ' ) Reg[j1++]=Reg[i2] ;
	      i_str=Format->find_struct( fdst,Reg );
	      if( i_str<=-10 )
              {   StrErr="����������� ���\"" ; StrErr+=Reg ; StrErr+="\"" ; throw(-1);  }
              W.i_struct=i_str ;
	      W.Param.init() ;
	      W.order=-1 ;
	      j=0 ;

	      if( c=='[' )
	      {	  if( fformat==0 )
		  {   S.sy_word =Word.j ;
		      S.i_struct=W.i_struct ;
		      S.i_relation=Relation.j ;
		      S.Param.init();
		      Struct.add(S) ; // �����
		      Relation.add(R);
		  }
 	          W.sy=i1+1 ;
	      }
	      else
	      {	  if( fformat==0 )
		  {   S.sy_word =Word.j ;
		      S.i_struct=W.i_struct ;
		      S.i_relation=Relation.j ;
		      S.Param.init();
		      Struct.add(S) ; // �����
		      Relation.add(R);
		  }
	          Mass[i1]=0 ;
		  W.sy=i1 ;
		  Word.add(W); // �����
	      }
	      continue ;
	  }
	  if( c==']' )
	  {  Mass[i1]=0 ;
	     Word.add(W); // �����
	     j=0 ;
	     continue ;
	  }
	  if( c=='(' || c=='<' )
	     j=0 ;
	  if( c=='>' ) 
	  {  Reg[j-1]=0 ; Word[Word.j-1].order=atoi(Reg); j=0 ; }
	  if( c==')' )
	  {  Reg[j-1]=0 ;
	     t_sStruct *S1=&Struct[Struct.j-1] ;
	     short      iw=Word.j-S1->sy_word-1 ;
	     make_relation2( fdst, S1, iw, Reg );
	     j=0 ;
	  }
      }
   }
   // ----- ��������� ������ Struct[].Word -----------
   S.sy_word =Word.j ;
   S.i_relation=Relation.j ;
   Struct.add(S);
   for( i=0 ; i<Struct.j-1 ; i++ )
   {  Struct[i].Word=&Word[Struct[i].sy_word] ;
      Struct[i].n_Word=Struct[i+1].sy_word-Struct[i].sy_word ;
      Relation[Struct[i].i_relation].s1=Struct[i+1].i_relation-Struct[i].i_relation-1 ;
   }
   Struct.j-- ;
   // ------ ���������� ������ Word[].str ------------
   for( i=0 ; i<Word.j ; i++ )
      Word[i].str=(char *)Mass+Word[i].sy ;
}
catch( int E )
{
   error_add( "t_sRecord1 :: parse1\n" );
   throw( E );
}
}
/***************************************************************************/
void t_sForm :: init( )
{  v0=0xf ; v1=0xf ; v2=0xf ; v3=0xf ; v4=0xf ; v5=0xf ; v6=0xf ; v7=0xf ;  }
/***************************************************************************/
t_sForm :: t_sForm()
{  v0=0xf ; v1=0xf ; v2=0xf ; v3=0xf ; v4=0xf ; v5=0xf ; v6=0xf ; v7=0xf ;  }
/***************************************************************************/
void t_sForm :: operator = ( t_Form &F )
{
   v0 = 0xf & F.value[0] ;
   v1 = 0xf & F.value[1] ;
   v2 = 0xf & F.value[2] ;
   v3 = 0xf & F.value[3] ;
   v4 = 0xf & F.value[4] ;
   v5 = 0xf & F.value[5] ;
   v6 = 0xf & F.value[6] ;
   v7 = 0xf & F.value[7] ;
}
/***************************************************************************/
t_Form t_sForm :: form( )
{
   t_Form F ;

   if( v0 == 0xf ) F.value[0]=-1 ; else F.value[0]=v0 ;
   if( v1 == 0xf ) F.value[1]=-1 ; else F.value[1]=v1 ;
   if( v2 == 0xf ) F.value[2]=-1 ; else F.value[2]=v2 ;
   if( v3 == 0xf ) F.value[3]=-1 ; else F.value[3]=v3 ;
   if( v4 == 0xf ) F.value[4]=-1 ; else F.value[4]=v4 ;
   if( v5 == 0xf ) F.value[5]=-1 ; else F.value[5]=v5 ;
   if( v6 == 0xf ) F.value[6]=-1 ; else F.value[6]=v6 ;
   if( v7 == 0xf ) F.value[7]=-1 ; else F.value[7]=v7 ;

   return F ;
}
/***************************************************************************/
DEFINE_LIST_BTYPE( t_sWord,t_sWordList )
DEFINE_LIST_BTYPE( t_sStruct,t_sStructList )
