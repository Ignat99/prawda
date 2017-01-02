/*************************************************************************/
//                 ������ ��� ������� ���������
//
/*************************************************************************/

# include <string.h>
# include "mylib.h"
# include "semantika.h"
# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))

short find_value( t_ValueList  *List, char *Str );

/************************************************************************/
t_Semantika :: t_Semantika( void )
{
   Format=NULL ;
   Mass  =NULL ;
   Mass1 =NULL ;
   n_Mass=n_Mass1=j_Mass1=0 ;
   Shabl =NULL ;
   EmptyStr[0]=0 ;
//   TypeLink.init();
   Prefix.init();
   Suffix.init();
   Root.init();
   Link.init();
   Word.init();
}
/************************************************************************/
t_Semantika :: ~t_Semantika( void )
{
   Free(Mass) ;
   Free(Mass1) ;
   Free(Shabl);
//   TypeLink.del();
   Prefix.del();
   Suffix.del();
   Root.del();
   Link.del();
   Word.del();
}
/************************************************************************/
//             �������� ����� �����
/************************************************************************/
void t_Semantika :: add_word( t_mWord &_Word )
{  Word.add( _Word );  }

/************************************************************************/
//        ���������� ����� ����������� (��� ���������� ��������)
/************************************************************************/
void t_Semantika :: set_remark( long i_Word, char *Remark )
{  long L ;

   L=strlen( Remark );
   if( n_Mass1-1<=j_Mass1+L ) return ;
   strcpy( Mass1+j_Mass1,Remark );
   Word[i_Word].remark=Mass1+j_Mass1 ;
   j_Mass1+=L+1 ;
}

/************************************************************************/
// void  t_Semantika :: add_typelink( t_TypeLink &_TypeLink )
// {  TypeLink.add( _TypeLink ); }
/************************************************************************/
//            �������� ����� ����� ����� �������
/************************************************************************/
void  t_Semantika :: add_link( t_mLink &_Link )
{   long i,z ;

    // ----------- ���������� ������ ������ ------------------
    z=Word[_Link.i0_word].i_link ;
    Link.add( _Link );
    for( i=Link.j-2 ; z<=i ; i-- )
       Link[i+1]=Link[i] ;
    Word[_Link.i0_word].n_link++ ;

    // --------- ����������� Word[i].i_link ------------------
    // �� ������������ ������, ����� i_link=i_link
    long z1=Word.list[_Link.i0_word].i_link ;

    for( i=0 ; i<Word.j ; i++ )
       if( _Link.i0_word !=i && z1<=Word[i].i_link )
           Word[i].i_link++ ;

    // --------- ���������� �������� ������ ------------------
    Link[z]=_Link ;
    if( _Link.str1[0]==0 )
        Link[z].str1=EmptyStr ;
    else
    {  long L=strlen( _Link.str1 );
       if( n_Mass1-1<=j_Mass1+L ) return ;
       strcpy( Mass1+j_Mass1,_Link.str1 );
       Link[z].str1=Mass1+j_Mass1 ;
       j_Mass1+=L+1 ;       
    }
}
/************************************************************************/
//               �������� ������ �����
/************************************************************************/
void  t_Semantika :: add_root( t_Root &_Root )
{   Root.add( _Root );  }
/************************************************************************/
//               �������� ���������
/************************************************************************/
void  t_Semantika :: add_prefix( t_Affix &_Affix )
{   Prefix.add( _Affix );  }
/************************************************************************/
//               �������� �������
/************************************************************************/
void  t_Semantika :: add_suffix( t_Affix &_Affix )
{   Suffix.add( _Affix );  }   
/************************************************************************/
//               �������� ��� �����
/************************************************************************/
int mwordcmp( const void *a, const void *b )
{  t_mWord *W1=(t_mWord *)a ;
   t_mWord *W2=(t_mWord *)b ;

   if( W1->i_struct!=W2->i_struct )
       return W1->i_struct - W2->i_struct ;
   return strcmp( W1->str,W2->str );
}
/************************************************************************/
//               ����� ������ �����
//              (��� ������� �������!!!)
/************************************************************************/
long t_Semantika :: find_word( short i_struct, char *Str )
{
   int cur;
   int cmp_res;
   int top,bottom ;
   t_mWord  W;
   
   if( Word.j<=0 ) return -1 ;
   W.i_struct=i_struct ;
   W.str     =Str ;

   bottom=0 ;
   top   =Word.j ;

   while( bottom <= top )
   {
      cur=(top+bottom)>>1 ;

      // ��������� ��������
      if((cmp_res=mwordcmp( &W,&Word[cur])) == 0) 
          return cur;

      if( cmp_res>0 )
           bottom=cur+1 ;
      else top=cur-1;
   }
   return -1; // Not found.
}
/************************************************************************/
//                    �������� � ����
/************************************************************************/
void  t_Semantika :: write( char *File )
{  long i,i1,i2 ;
   char *ss,*s1 ;

   FILE *fw=Fopen( File,"w" );

   // ------- ������� -------------------------------------
   fprintf( fw,"\n@�������" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Grammar.to().Part.j ; i++ )
   {  fprintf( fw,"\n   @�����_���� %s",Grammar.to().Part[i].Name );
      fprintf( fw,"\n   {" );
      for( i1=0 ; i1<Shabl[i].j ; i1++ )
      {
         fprintf( fw,"\n       %d 0 ", i1 );
         for( i2=0 ; i2<Shabl[i][i1].n_end ; i2++ )
	    fprintf( fw," %s",Shabl[i][i1].end[i2] );
         fprintf( fw," ;" );	    
      }
      fprintf( fw,"\n   }" );
   }
   fprintf( fw,"\n}" );
   // ------- ���� ������ ---------------------------------
   fprintf( fw,"\n@����_������" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Format->TypeLink.j ; i++ )
      fprintf( fw,"\n   @������ = %s %s ;",Format->TypeLink[i].Name,Format->TypeLink[i].remark );
   fprintf( fw,"\n}" );   

   // ------- ��������� -----------------------------------
   fprintf( fw,"\n@���������" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Prefix.j ; i++ )
   {  if( Prefix[i].i_struct<0 )
           ss="@0" ;
      else ss=Grammar.to().Part[Prefix[i].i_struct].Name ;
      fprintf( fw,"\n   @��������� = %s %s %s ;",
             Prefix[i].Name,Format->TypeLink[Prefix[i].i_link].Name,ss );
   }
   fprintf( fw,"\n}" );

   // ------- �������� ------------------------------------
   fprintf( fw,"\n@��������" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Suffix.j ; i++ )
   {  if( Suffix[i].i_struct<0 )
           ss="@0" ;
      else ss=Grammar.to().Part[Suffix[i].i_struct].Name ;
      fprintf( fw,"\n   @������� = %s %s %s ;",
             Suffix[i].Name,Format->TypeLink[Suffix[i].i_link].Name,ss );
   }
   fprintf( fw,"\n}" );   
   
   // ------- ����� --------------------------------------
   fprintf( fw,"\n@�����" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Root.j ; i++ )
      fprintf( fw,"\n## %s %s ;",Root[i].Str,Root[i].Str2 );
   fprintf( fw,"\n}" );

   // ------ ����� ---------------------------------------
   t_mWord *W ;
   t_mLink *L ;

   fprintf( fw,"\n@�����" );
   fprintf( fw,"\n{" );
   for( i=0 ; i<Word.j ; i++ )
   {  s1=Grammar.format1().get_tag(1,Word[i].i_struct );
      fprintf( fw,"\n## %s[%s] %d ",s1,Word[i].str,Word[i].i_shablon );
      print_param( fw,&Word[i] );
      if( Word[i].remark!=NULL )
          fprintf( fw," <%s> ",s1,Word[i].remark );
      // ------ ������ -----------------------------------
      for( i1=Word[i].i_link ; i1<Word[i].i_link+Word[i].n_link ; i1++ )
      {  L=&Link[i1] ;
         W=&Word[L->i_word] ;
	 s1=Grammar.format1().get_tag(1,W->i_struct );
	 if( L->str1[0]==0 )
              fprintf( fw,"\n  &%s->%s[%s]",Format->TypeLink[L->type].Name,s1,W->str );
	 else fprintf( fw,"\n  &%s[%s]->%s[%s]",Format->TypeLink[L->type].Name,L->str1,s1,W->str );
	 
         if( W->remark!=NULL )
             fprintf( fw,"<%s> ", W->remark );
         fprintf( fw,";" );
      }
   }
   fprintf( fw,"\n}" );
}
/************************************************************************/
//     ���������� ������
//     � �������� ��� ����� ������������� � ���, ��� ����
/************************************************************************/
void t_Semantika :: print_link( FILE *fw, long i_word, t_mLink *L )
{   
    t_mWord *W =&Word[i_word] ;
    t_mWord *W1=&Word[L->i_word] ;
    char    *s1=Grammar.format1().get_tag(1,W->i_struct );
    
    fprintf( fw,"\n %s ",W->str );
    if( W->remark!=NULL )
        fprintf( fw,"<%s> ", W->remark );    
    if( L->str1[0]==0 )
         fprintf( fw,"&%s->%s[%s]"    ,Format->TypeLink[L->type].Name,s1,W1->str );
    else fprintf( fw,"&%s[%s]->%s[%s]",Format->TypeLink[L->type].Name,L->str1,s1,W->str );
    if( W1->remark!=NULL )
        fprintf( fw,"<%s> ", W1->remark );
}
/************************************************************************/
//       ���������� ��������� �����
/************************************************************************/
void t_Semantika :: print_param( FILE *fw, t_mWord *Word )
{
   short  i,i1,ip,n ;
   short  i_struct ;
   char   *s ;
   t_Form      Param ;
   t_Struct   *SS ;
   
   i_struct=Word->i_struct ;
   SS=&Grammar[Word->i_struct].To ;
   Param=Word->Param ;

   for( i1=i=0 ; i<10 ; i++ )
      if( 0<=Param.value[i] ) i1++ ;
   if( i1==0 ) return ;
   
   fprintf( fw,"(" );
   
   t_Format1 *Format=&Grammar.format1() ;
   
   for( i=0 ; i<10 ; i++ )
   {  if( Grammar[i_struct].To.Param.j<=i ) break ;
      if( 0<=Param.value[i] )
      {  // ------- ���������� �������� ����� -------------
	 if( Grammar[i_struct].To.Param.j<=i ) break ;
         ip=Grammar[i_struct].To.Param[i].param ;
	 s =Grammar.to().Param[ip].Value[Param.value[i]].Name;

	 for( i1=0 ; i1<Format->Rename.j ; i1++ )
	    if( 0==strcmp(Format->Rename[i1].Full,s) ) 
	    {   s=Format->Rename[i1].Reduce ; break ;  }
      }
      else
         s="." ;
      if( 0<i ) fprintf( fw,"," );
      fprintf( fw,s );
   }
   fprintf( fw,")" );
}
/************************************************************************/
//       ���������� ��������� ����� � ������
//      (���� �������������)
/************************************************************************/
void t_Semantika :: print_param( char *Str, t_mWord *Word, char empty_par )
{
   short  i,i1,ip,n ;
   short  i_struct ;
   char   *s ;
   t_Form      Param ;
   t_Struct   *SS ;

   Str[0]=0 ;   
   i_struct=Word->i_struct ;
   SS=&Grammar[Word->i_struct].To ;
   Param=Word->Param ;

   for( i1=i=0 ; i<10 ; i++ )
      if( 0<=Param.value[i] ) i1++ ;
   if( i1==0 ) return ;
   
   strcat( Str,"(" );
   
   t_Format1 *Format=&Grammar.format1() ;
   
   for( i=0 ; i<10 ; i++ )
   {  if( Grammar[i_struct].To.Param.j<=i ) break ;
      if( 0<=Param.value[i] )
      {  // ------- ���������� �������� ����� -------------
         ip=Grammar[i_struct].To.Param[i].param ;
	 s =Grammar.to().Param[ip].Value[Param.value[i]].Name;

	 for( i1=0 ; i1<Format->Rename.j ; i1++ )
	    if( 0==strcmp(Format->Rename[i1].Full,s) ) 
	    {   s=Format->Rename[i1].Reduce ; break ;  }
      }
      else
      {  if( 0!=empty_par ) continue ;
         s="." ;
      }
      if( 0<i ) strcat( Str,"," );
      strcat( Str,s );
   }
   strcat( Str,")" );
}
/************************************************************************/
//                      �������� �� �����
/************************************************************************/
void t_Semantika :: read( char *File, t_Format4 *_Format )
{  long  i,i1 ;
   long  z_shablon,z_link,z_prefix,z_suffix,z_root,z_word ;
   FILE *fr ;
 
try
{  // ------- �������� ���� � ������ ----------------------
   Format=_Format ;

   fr=Fopen( File,"rb" );
   n_Mass=FileLength( fr );
   Mass  =(char *)Calloc( n_Mass+1,sizeof(uchar) );
   Fread( Mass,n_Mass,sizeof(char),fr );
   fclose(fr);
   n_Mass=remark( (char *)Mass, n_Mass );

   // ------- ������� �� ������ ---------------------------
   for( i=0 ; i<n_Mass ; i++ )
   {  if( Mass[i]=='@' )
      {   if( 0==strncmp( Mass+i,"@�������"    ,8 )) z_shablon=i ;
	  if( 0==strncmp( Mass+i,"@����_������",12)) z_link=i ;
          if( 0==strncmp( Mass+i,"@���������"  ,10)) z_prefix=i ;
	  if( 0==strncmp( Mass+i,"@��������"   ,9 )) z_suffix=i ;
	  if( 0==strncmp( Mass+i,"@�����"      ,6 )) z_root=i ;
	  if( 0==strncmp( Mass+i,"@�����"      ,6 )) z_word=i ;
      }
   }
   read_shablon ( Mass+z_shablon, z_link  -z_shablon );
// read_typelink( Mass+z_link   , z_prefix-z_link );
   read_affix   ( Mass+z_prefix , z_suffix-z_prefix, Prefix, 0 );
   read_affix   ( Mass+z_suffix , z_root  -z_suffix, Suffix, 1 );
   read_root    ( Mass+z_root   , z_word  -z_root );
   read_word    ( Mass+z_word,    n_Mass  -z_word );

   // ------- ���������� ������ ������ �� ������ ---------
   for( i=0 ; i<Word.j-1 ; i++ )
      Word[i].n_link=Word[i+1].i_link-Word[i].i_link ;
   Word[Word.j-1].n_link=Link.j-Word[i].i_link ;

   qsort( Word.list, Word.j, sizeof(t_mWord), mwordcmp );

   for( i=0 ; i<Word.j-1 ; i++ )
      for( i1=0 ; i1<Word[i].n_link ; i1++ )
         Link[Word[i].i_link+i1].i0_word=i ;
   
   // ------- ���������� ������� ������ �� ����� ---------
   // (�������� ��� ��� �����������, ��� �� ������)
   for( i=0 ; i<Link.j ; i++ )
      Link[i].i_word = find_word( Link[i].i_struct, Link[i].str );

}
catch( int E )
{  
   fprintf( File_Error,"\n ������, ��-�� � ������� %s �� ���� ��������.",File );
   if( E==Err_Fopen ) fprintf( File_Error,"\n � �� ������-�� ����?" );
   throw( E );
}
}
/************************************************************************/
//               �������� ���� ������ ��������������
/************************************************************************/
void t_Semantika :: read_shablon1( char *M, long L )
{
   long  i,j,j1,z,n_ends,N,j_mem ;
   short i_struct ;
   //t_Lexer &Lexer=LShablon ;
   char  *Mem ;
   t_Lexer Lexer ;
   
   Lexer.clr_breaker(  );
   Lexer.init( M,L );
   Lexer.remark();
   Lexer.make_words();
   Mem=(char *)Calloc( L,sizeof(char) );
   
   i_struct=-1 ;
   for( i=0 ; i<Lexer.n_Word ; i++ )
   {  if( 0==strcmp( Lexer.Word[i],"@�����_����" ))
          i_struct=Grammar.find_struct( TO, Lexer.Word[i+1] );
      if( Lexer.Word[i][0]=='{' ) break ;
   }
   z=i+1 ;
   for( N=0 ; i<Lexer.n_Word ; i++ )
   {  if( Lexer.Word[i][0]=='}' ) break ;
      if( Lexer.Word[i][0]==';' ) N++ ;
   }
   if( 0<N ) n_ends=((i-z)/N)-3 ;
       else  n_ends=0 ;
   
   Shabl[i_struct].Calloc( N );

   j_mem=0 ;
   for( j=j1=0,i=z ; i<Lexer.n_Word ; i++ )
   {
      if( Lexer.Word[i][0]=='}' ) break ;
      if( Lexer.Word[i][0]==';' ) {  j++ ; j1=0 ; continue ;  }
      if( 0==j1 )
      {   Shabl[i_struct][j].end=(char **)calloc( n_ends,sizeof(char *) );
          Shabl[i_struct][j].n_end=n_ends ;
      }
      if( 2<=j1 )
      {   Shabl[i_struct][j].end[j1-2]=Mem+j_mem ;
          strcpy( Mem+j_mem, Lexer.Word[i] );
	  j_mem+=strlen( Lexer.Word[i] )+1;
      }
      j1++ ;
   }
}
/************************************************************************/
//                �������� ������� ��������������
/************************************************************************/
void t_Semantika :: read_shablon( char *M, long L )
{  long i ;

   Shabl=(t_ShablList *)Calloc( Grammar.to().Part.j,sizeof(t_ShablList) );

   for( i=0 ; i<L ; i++ )
   {  if( 0==strncmp( M+i,"@�����_����",11 ) )
          read_shablon1( M+i, L-i );
   }
}
/************************************************************************/
//             �������� ������
/************************************************************************/
/*
void t_Semantika :: read_typelink( char *M, long L )
{
   long  i,N,j ;
   t_Lexer &Lexer=LTypeLink ;

   Lexer.clr_breaker(  );
   Lexer.init( M,L );
   Lexer.remark();
   Lexer.make_words();
   
   for( i=N=0 ; i<Lexer.n_Word ; i++ )
      if( Lexer.Word[i][0]==';' ) N++ ;

   TypeLink.Calloc( N );

   for( j=i=0 ; i<Lexer.n_Word ; i++ )
   {  if( 0==strcmp( Lexer.Word[i],"@������" ) )
      {   TypeLink[j].Name  =Lexer.Word[i+2] ;
          TypeLink[j].remark=Lexer.Word[i+3] ;
	  j++ ;
      }
   }
}
/************************************************************************/
//           �������� ������ ��������
/************************************************************************/
void t_Semantika :: read_affix( char *M, long L, t_AffixList &Affix, char fpostfix )
{
   long  i,N,j ;
   t_Lexer *Lexer ;

   if( fpostfix==0 ) Lexer=&LPrefix ;
       else          Lexer=&LPostfix ;

   Lexer->clr_breaker(  );
   Lexer->init( M,L );
   Lexer->remark();
   Lexer->make_words();
   
   for( i=N=0 ; i<Lexer->n_Word ; i++ )
      if( Lexer->Word[i][0]==';' ) N++ ;

   Affix.Calloc( N );

   for( j=i=0 ; i<Lexer->n_Word ; i++ )
   {  if( 0==strcmp( Lexer->Word[i],"@���������" ) ||
          0==strcmp( Lexer->Word[i],"@�������" ) )
      {   Affix[j].Name    =Lexer->Word[i+2] ;
          Affix[j].i_link  =find_TypeLink( Lexer->Word[i+3] );
	  if( 0==strcmp( "@0",Lexer->Word[i+4] ) )
	           Affix[j].i_struct=-1 ;
	      else Affix[j].i_struct=Grammar.find_struct( TO, Lexer->Word[i+4] );
          j++ ;
      }
   }
}
/************************************************************************/
//                    �������� ����� ����
/************************************************************************/
void t_Semantika :: read_root( char *M, long L )
{
   long  i,j,N ;
   t_Lexer &Lexer=LRoot ;

   Lexer.clr_breaker(  );
   Lexer.init( M,L );
   Lexer.remark();
   Lexer.make_words();

   for( i=N=0 ; i<Lexer.n_Word ; i++ )
      if( Lexer.Word[i][0]==';' ) N++ ;

   Root.Calloc( N );

   for( j=i=0 ; i<Lexer.n_Word ; i++ )
   {  if( 0==strcmp( Lexer.Word[i],"##" ) )
      {   Root[j].Str   =Lexer.Word[i+1] ;
          Root[j].Str2  =Lexer.Word[i+2] ;
          j++ ;
      }
   }
}
/************************************************************************/
//                �������� �����
/************************************************************************/
void t_Semantika :: read_word( char *M, long L )
{  long i,j,j_Link ;
   long n_Word,n_Link ;

   n_Word=n_Link=0 ;
   for( i=0 ; i<L ; i++ )
   {  if( M[i]=='#' && M[i+1]=='#' ) n_Word++ ;
      if( M[i]=='&'  ) n_Link++ ;
   }
   Word.Calloc( max(4,n_Word) );
   Link.Calloc( max(4,n_Link) );
   Link.j=n_Link ;

   j_Link=0 ;
   for( i=j=0 ; i<L ; i++ )
   {  if( M[i]=='#' && M[i+1]=='#' )
      {   read_word1( Word[j],j,M+i+2, j_Link );
          j++ ;
      }
   }
}
/************************************************************************/
//               �������� ���� �����
/************************************************************************/
void t_Semantika :: read_word1( t_mWord &Word, long i_word, char *Text, long &j_Link )
{  long i,j,f ;
   char Str[100] ;

   t_Format1 *Format=&Grammar.format1() ;

   // ------- ��� ��������� --------------------
   for( j=i=0 ; Text[i]!='['; i++ )
      if( Text[i]!=' ' && Text[i]!='\t' )
          Str[j++]=Text[i] ;
   Str[j++]=0 ;
   Word.i_struct=Format->find_struct( TO, Str );
   
   // ------- ������� --------------------------
   Word.str=&Text[i+1] ;
   for( ; Text[i]!='#'; i++ )
      if( Text[i]==']' )
      {   Text[i]=0 ; break ;  }
   i++ ;
   // ------- ������ ---------------------------
   for( j=0 ; Text[i]!='<' && Text[i]!='(' && Text[i]!='&'  && Text[i]!='#' ; i++ )
      if( Text[i]!=' ' && Text[i]!='\t' )
          Str[j++]=Text[i] ;
   Str[j++]=0 ;
   Word.i_shablon=atoi( Str );

   // ------- ���������� -----------------------
   Word.remark=NULL ;
   if( Text[i]=='<' )
   {   Word.remark=&Text[i+1] ;
       for( ; Text[i]!='#'; i++ )
	  if( Text[i]=='>' )
	  {   Text[i]=0 ; break ;  }
   }
   // ------- ��������� - ������������ ������ ------
   for( j=f=0 ; Text[i]!='&'  && Text[i]!='#' ; i++ )
   {  
      if( Text[i]==')' ) break ;
      if( Text[i]==' ' ) continue ;
      if( 1==f) Str[j++]=Text[i] ;
      if( Text[i]=='(' ) f=1 ;
   }
   Str[j]=0 ;
   // ------- ��������� - ��������� �������� ------
   t_Form  F ;
   t_mLink L ;
   long    j_param ;
   char    c ;
   char    Str1[100] ;
   
   for( i=j=j_param=0 ; i<100 ; i++ )
   {  c=Str[i] ;
      if( c==' ' ) continue ;
      if( c==0 || c==',' )
      {   Str1[j]=0 ;
          if( 0<j )
	      F.value[j_param]=find_value1( Word.i_struct, j_param, 1, Str1 ) ;
	  j=0 ;
	  j_param++ ;
      }
      if( c==0 ) break ;
      if( c==',' ) continue ;
      Str1[j++]=c ;
   }
   Word.Param=F ;
   // ------- ������ ------------------------------
   Word.i_link=j_Link ;
   char f1=0 ; // ���� ��� ������� ����� "->"
   for( ; Text[i]!='#' && Text[i]!='}' ; i++ )
   {  c=Text[i] ;
      if( c=='&' )
      {   f=f1=0 ; j=0 ; 
          L.remark =EmptyStr ;
	  L.i0_word=i_word ;
          continue ;  
      }
      if( c=='-' && Text[i+1]=='>' )
      {   // ------ ���������� ��� ������ ---------
          Str[j]=0 ;
          L.type=find_TypeLink( Str );
	  if( NULL==strstr( Str,"]" ) )
	      L.str1=EmptyStr ;
	  f1=1 ;
      }
      if( c=='[' && f1==0 ) // ---- ������� ������
          L.str1=Text+i+1 ;
      if( c=='[' && f1==1 )
      {   // ------ ���������� �����, �� ������� ���� ������ ------
          short i1,j1,z ;
          Str[j]=0 ;
	  for( j1=i1=z=0 ; i1<j ; i1++ )
	  {  if( Str[i1]!=' ' )
	         Str[j1++]=Str[i1] ;
             if( Str[i1]=='>' ) z=j1 ;
 	  }
	  Str[j1]= 0 ;
	  if( 0<j1 )       // ----- ��������� �����
              L.i_struct=Format->find_struct( TO, Str+z );
	  L.str=Text+i+1 ; // ----- ������� �����
      }
      if( c==']' || c=='>' )
          Text[i]=0 ;
      if( c=='<' )         // ----- ������� �����
      {   f=1 ;
      	  L.remark=Text+i+1 ;
      }
      if( f==0 )
          Str[j++]=c ;
      if( c==';' )  // ------ ������ �������������� ������ � ������
      {   
          Link[j_Link++]=L ;
      }
   }
}
/************************************************************************/
//                  ����� ��� ������ (�� �����)
/************************************************************************/
long t_Semantika :: find_TypeLink( char *Name )
{  long i ;
   char Str[100] ;

   for( i=0 ; i<100 && Name[i]!=0 && Name[i]!='[' ; i++ )   
      Str[i]=Name[i] ;
   Str[i]=0 ;

   for( i=0 ; i<Format->TypeLink.j ; i++ )
      if( 0==strcmp( Format->TypeLink[i].Name,Str ) )
          return i ;
   return -1 ;
}
/***************************************************************************/
//   ����� �������� ��������������� ��������� �� ����� �������� 
//   i_struct - ������ ����� ���� ��� ���������
//   i_param  - ������ ��������� � ���� ��������� t_Struct->Param[i_struct]
//   fdst     - 0-�������� 1-��������
//   Str1     - �������� ���������, �������� "���������"
/***************************************************************************/
short t_Semantika :: find_value1( short i_struct, short i_param, char fdst, char *Str1 )
{  short    v, i_rename ;
   t_Param1List *pParam ;
   t_ValueList  *value ;

   if( i_struct<0 )
   {  fprintf( File_Error,"\nt_Semantika::find_value1 i_struct<0"); throw(-1);  }

   if( fdst==0 )
   {   pParam=&Grammar[i_struct].From.Param ;
       if( i_param<0 || pParam->j<=i_param )
       {  fprintf( File_Error,"\nt_Semantika::find_value1 i_param Error"); throw(-1);  }
       value =&Grammar.from().Param[pParam->list[i_param].param].Value ;
   }
   else
   {   pParam=&Grammar[i_struct].To.Param ;
       if( i_param<0 || pParam->j<=i_param )
       {  fprintf( File_Error,"\nt_Semantika::find_value1 i_param Error"); throw(-1);  }
       value =&Grammar.to().Param[pParam->list[i_param].param].Value ;
   }

   t_Format1 *Format=&Grammar.format1() ;
   i_rename=-1 ;
   for( short i=0 ; i<Format->Rename.j ; i++ )
      if( 0==strcmp( Str1,Format->Rename[i].Reduce ) ) 
      {   i_rename=i ; break ;  }

   if( 0<=i_rename )
	    v=find_value( value, Format->Rename[i_rename].Full );
       else v=find_value( value, Str1 );
	      
   return v ;
}
/**************************************************************************/
//                    ��������� "�������� �����"
/**************************************************************************/
void t_Semantika :: reverse(  )
{
   long  i ;
   long *sy=NULL ;
   t_mLinkList Link1 ;
   t_mLink     L,L1 ;

   Link1.Calloc( 2*Link.j );
   sy=(long *)Calloc( Word.j+1,sizeof(long));

   for( i=0 ; i<Link.j ; i++ )
   {  sy[ Link[i].i_word +1 ]++ ;
      sy[ Link[i].i0_word+1 ]++ ;
   }
   for( i=0 ; i<Word.j ; i++ )
      sy[i+1]+=sy[i] ;
   for( i=0 ; i<Word.j ; i++ )
   {  Word[i].i_link=sy[i] ;
      Word[i].n_link=sy[i+1]-sy[i] ;
   }   
   for( i=0 ; i<Link.j ; i++ )
   {  L=L1=Link[i] ;
      L1.i_word =L.i0_word ;
      L1.i0_word=L.i_word ;
      Link1[ sy[L.i0_word ]++ ]=L ;
      Link1[ sy[L1.i0_word]++ ]=L1 ;
   }
   Link=Link1 ;
   free(sy);
}
/**************************************************************************/
//                     ������ �� ������
//    i_word     - �� ������ ����� ����
//    i_typelink - ��� ������
/**************************************************************************/
void t_Semantika :: link_word( long i_word, long i_typeLink, t_longList &List )
{
    List.j=0 ;
    for( long i=0 ; i<Word[i_word].n_link ; i++ )
    {  t_mLink &L=Link[Word[i_word].i_link+i] ;
       if( L.type==i_typeLink )
           List.add( L.i_word );
    }
}
/**************************************************************************/
/*
short find_value( t_ValueList  *List, char *Str )
{
   if( 0==strcmp( "@0",Str ) ) return -1 ;
   for( short i=0 ; i<List->j ; i++ )
      if( 0==strcmp(List->list[i].Name,Str) ) return i ;
   return -1 ;
}
*/
/**************************************************************************/
DEFINE_LIST_BTYPE( t_Shabl,    t_ShablList )
DEFINE_LIST_BTYPE( t_Affix,    t_AffixList )
DEFINE_LIST_BTYPE( t_Root,     t_RootList )
DEFINE_LIST_BTYPE( t_mLink,    t_mLinkList )
DEFINE_LIST_BTYPE( t_mWord,    t_mWordList )


