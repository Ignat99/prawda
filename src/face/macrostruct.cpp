// ****************************************************************************
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                 Работа с "крупной структурой" текста
//     Анисимов Д.В.                               сегодня
// ****************************************************************************
# include <string.h>
# include "mylib.h"
# include "macrostruct.h"
# include "kontext.h"

# define T_FRASAEND  1
# define T_ABSATZEND 2

extern t_Macrostruct Macrostruct ;
extern t_Kontext     Kontext ;

void select_lang( void *S );

/***********************************************************************/
//
/***********************************************************************/
t_Macrostruct :: t_Macrostruct()
{
   Absatz.init();
   Sort.init();
   exist=0 ;
}
/***********************************************************************/
//        построить список абзацев и фраз по тексту с границами
/***********************************************************************/
void t_Macrostruct :: make( )
{
   if( 0==was_maked() )
       automake1();
   automake2();
   make_ht_sort();
   exist=1 ;
}
/***********************************************************************/
void t_Macrostruct :: del( )
{
   Absatz.del();
   Sort.del();
   exist=0 ;
}
/************************************************************************/
//         было ли сделано разбиение текста на абзацы и фразы
/************************************************************************/
char t_Macrostruct :: was_maked( void )
{  long      L=Text->length();
   t_myChar *T=Text->text_all() ;

   for( long i=0 ; i<L ; i++ )
      if( T[i].Flag!=0 ) return 1;
   return 0 ;
}
/************************************************************************/
//        корректно ли разбиение текста на абзацы и фразы
/************************************************************************/
char t_Macrostruct :: format_is_correct( void )
{  long      L=Text->length();
   t_myChar *T=Text->text_all() ;
   long      nabsatz,nfrasa ;

   nabsatz=Absatz.j ;
   for( long i=nfrasa=0 ; i<Absatz.j ; i++ )
   {  for( long i1=0 ; i1<Absatz[i].Frasa.j ; i1++ )
      {  t_Frasa   *F=&Absatz[i].Frasa[i1] ;
         if( T[F->end].Flag!=T_FRASAEND && T[F->end].Flag!=T_ABSATZEND )
	     return -1 ;
         if( T[F->end].Flag==T_FRASAEND )
	     nfrasa++ ;
      }
   }
   for( long i2=0 ; i2<L ; i2++ )
   {  if( T[i2].Flag==T_FRASAEND  ) 
          nfrasa-- ;
   }
   if( nfrasa <-1 || 1<nfrasa  ) return -1 ;
   return 1 ;
}
/************************************************************************/
//           убрать разбиение текста на абзацы и фразы 
/************************************************************************/
void t_Macrostruct :: clear( void )
{  long      L=Text->length();
   t_myChar *T=Text->text_all() ;

   for( long i=0 ; i<L ; i++ )
      T[i].Flag=0 ;
}
/************************************************************************/
//  разбить текст на абзацы и фразы (проставить в текст предположительные границы фраз)
/************************************************************************/
void t_Macrostruct :: automake1( void )
{  t_myChar *T ;
   long      i,z,L ;
   char      f_char,f_string,f_string1 ;

   L=Text->length();
   T=Text->text_all() ;

   f_char   =0 ;
   f_string =0 ;
   f_string1=0 ;

   for( i=0 ; i<L ; i++ )
   {  // ------- определение конца фразы --------------------
      if( is_point32(T[i]) && is_space32(T[i+1]) )
          T[i].Flag=T_FRASAEND ;

      // ------- определение конца абзаца --------------------
      if( T[i]==UNEWLINE ) // пустая строка
      {   if( 0==f_char ) f_string=0 ;
              else        f_string=1 ;
          if( 1==f_string1 && 0==f_string )
	      for( z=i-1 ; 0<=z ; z-- )
	         if( T[z]==UNEWLINE )
		 {   T[z-1].Flag=T_ABSATZEND ; break ;  }// не всегда видно		 
	  f_string1=f_string ;
	  f_char=0 ;
      }
      if( 0==is_space(T[i]) ) f_char=1 ;
   }
}
/************************************************************************/
//   разбить текст на абзацы и фразы (сделать массивы фраз и абзацев)
/************************************************************************/
void t_Macrostruct :: automake2( void )
{  t_myChar *T ;
   long      i,L,zFrasa ;
   t_Frasa   Frasa1 ;
   t_Absatz  Absatz1 ;

   L=Text->length();
   T=Text->text_all() ;

   zFrasa  =0 ;
   Absatz.del();

   for( i=0 ; i<L ; i++ )
   {  // ------- определение конца фразы --------------------
      if( T[i].Flag==T_FRASAEND || T[i].Flag==T_ABSATZEND )
      {
	  Frasa1.begin=zFrasa ;
	  Frasa1.end  =i ;
	  zFrasa      =i+1 ;
	  Absatz1.Frasa.add( Frasa1 );
      }
      // ------- определение конца абзаца --------------------
      if( T[i].Flag==T_ABSATZEND )
      {
	  Absatz.add( Absatz1 );
	  Absatz1.Frasa.del();
      }
   }
   Absatz.add( Absatz1 );
}
/***********************************************************************/
//     вспомогательная функция, определяющая конец фразы по знакам пунктуации
/***********************************************************************/
char is_point( char16 T )
{   if( T=='.' || T=='?' || T=='!' ) return 1 ;
    return 0 ;
}
/***********************************************************************/
//     вспомогательная функция, определяющая пробел для конца фразы
/***********************************************************************/
char is_space1( char16 T )
{  if( T==' ' || T==UNEWLINE ) return 1 ;
   return 0 ;
}
/***********************************************************************/
//                  напечатать образ гипертекста
/***********************************************************************/
void t_Macrostruct :: print( char *File )
{
   FILE *fw1=Fopen( File,"w" );
   print( fw1 );
   Fclose( fw1 );
}
/***********************************************************************/
//                  напечатать образ гипертекста
/***********************************************************************/
void t_Macrostruct :: print( FILE *fw1 )
{
   long  i,i1 ;
   fw=fw1;

   fprintf( fw,"@hypertext\n" );
   print_int( "exist",exist );
   print_str( "direct_trans",z_DirTrans->Name );

   print_str32( "Text", Text->text_all() );

   for( i=0 ; i<Absatz.j ; i++ )
   {
      fprintf( fw,"\n@Абзац{");
      print_int( "Тип",Absatz[i].type );
      fprintf( fw,"\n" );

      for( i1=0 ; i1<Absatz[i].Frasa.j ; i1++ )
         print_frasa( Absatz[i].Frasa[i1] );
      fprintf( fw,"}");
   }
}
/**********************************************************************/
//                  напечатать фразу в образ
/**********************************************************************/
void t_Macrostruct :: print_frasa( t_Frasa &Frasa )
{
    fprintf( fw,"\n@Фраза{");
    print_int( "perevod", Frasa.perevod );
    print_int( "flag1"  , Frasa.flag1 );
    print_int( "begin"  , Frasa.begin );
    print_int( "end"    , Frasa.end );
    if( Frasa.Str.Str!=NULL )
         print_str32( "Str", Frasa.Str.Str );
    else print_str  ( "Str", "" );
    	
    fprintf( fw,"}");
}

/************************************************************************/
//           Чтение образа файла
/************************************************************************/
long t_Macrostruct :: scanf( char *File )
{  long i,i1,n,z,L1 ;
   t_Absatz A ;
   t_Frasa  F ;

try
{   
   L=file_to_mass( File, &Mass );
   Absatz.j=0 ;
   z_Mass=0 ;
   z_Mass=find_tag( 0,"@hypertext" );

   z_Mass=find_tag( z_Mass,"@exist" );
   scanf_int( &i ); exist=i ;
   {
      char *Str ;
      z_Mass=find_tag( z_Mass,"@direct_trans" );
      scanf1_str( &Str );
      if( 0!=strcmp( Str,z_DirTrans->Name ) )
      {   for( i=0 ; i<n_DirTrans ; i++ )
             if( 0==strcmp( Str,DirTrans[i].Name ) )
	     {   select_lang( &DirTrans[i] ) ; break ;  }
      }
   }
   
   // ---------- Text ---------------------------------
   while( 0==is_end_block() )
   {
      if( 0!=strncmp( Mass+z_Mass,"@Text{",6 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      z=z_Mass+6 ;
      t_myChar *Text1 ;
      scanf1_str32( &Text1 );
      Text->init32( Text1 );
      free( Text1 );
      break ;
   }
   Text->Length=z_Mass-z ;

   if( 0!=is_end_block() )
       z_Mass++ ;

   // ---------- Абзацы -------------------------------
   while( 0==is_end_block() )
   {
      if( 0!=strncmp( Mass+z_Mass,"@Абзац",6 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      scanf_int( &n );
      A.type=n ;
      Absatz.add(A);

      // ----------- Фразы ----------------------------
      while( 0==is_end_block() )
      {
	 if( 0!=strncmp( Mass+z_Mass,"@Фраза",6 ) )
	 {   z_Mass=find_at(z_Mass+1); continue ;  }

         char     str[1000] ;
         t_myChar str32[1000] ;
	 
         scanf_int( &n );  F.perevod=(e_Perevod)n ;
         scanf_int( &n );  F.flag1=n ;
         scanf_int( &F.begin );
         scanf_int( &F.end );
	 scanf_str( str,1000 );
	 koi9_to_myChar( str,str32 );
	 F.Str.set( str32 );

	 Absatz[Absatz.j-1].Frasa.add(F);
         z_Mass=find_end(z_Mass)+1 ;
      }
      z_Mass=find_end(z_Mass)+1 ;
   }   
   z_Mass=find_end(z_Mass)+1 ;
   Free( Mass );
   // ------- проставить в тексте границы фраз ----------------
   for( i=0 ; i<Absatz.j ; i++ )
   {  z=-1 ;
      for( i1=0 ; i1<Absatz[i].Frasa.j ; i1++ )
      {  z=Absatz[i].Frasa[i1].end ;
         Text->text(z).Flag=T_FRASAEND ;
      }
      if(0<=z) Text->text(z).Flag=T_ABSATZEND ;
   }
   return 0 ;
}
catch( int E )
{
   printf("\nt_Macrostruct NOT read!\n");
   return -1 ;
}
}
/************************************************************************/
//                         сделать список Sort
/************************************************************************/
void t_Macrostruct :: make_ht_sort( void )
{  long i,i1,L ;
   t_HT_Sort S ;

   Sort.del();
   for( L=i=0 ; i<Absatz.j ; i++ )
      L+=Absatz[i].Frasa.j+1 ;
   Sort.Calloc( L );
   Sort.j=0 ;
   for( i=0 ; i<Absatz.j ; i++ )
   {  S.i_absatz=i ;
      S.i_frasa =-1 ;
      Sort.add(S);
      for( i1=0 ; i1<Absatz[i].Frasa.j ; i1++ )
      {  S.i_frasa =i1 ;
         Sort.add(S);
      }
   }
}
/***********************************************************************/
//            разделить абзац на два
//       i_frasa-я фраза идет во второй абзац
/***********************************************************************/
void t_Macrostruct :: split_absatz( long i_absatz, long i_frasa )
{  long i ;

   if( i_absatz<0 || Absatz.j<=i_absatz ) return ;
   if( i_frasa<0  || Absatz[i_absatz].Frasa.j<=i_frasa ) return ;

   t_Absatz A ;
   Absatz.add( A );
   for( i=Absatz.j-2 ; i_absatz<i ; i-- )
      Absatz[i+1]=Absatz[i] ;

   Absatz[i_absatz+1].type=Absatz[i_absatz].type ;
   Absatz[i_absatz+1].Frasa.j=0 ;
   for( i=i_frasa ; i<Absatz[i_absatz].Frasa.j ; i++ )
      Absatz[i_absatz+1].Frasa.add( Absatz[i_absatz].Frasa[i] );
   Absatz[i_absatz].Frasa.j=i_frasa ;

   // Здесь
   t_wExemplarList &Exemplar=Kontext.Exemplar ;
   for( i=0 ; i<Exemplar.j ; i++ )
   {
      if( i_absatz==Exemplar[i].i_absatz && i_frasa<=Exemplar[i].i_frasa )
      {   Exemplar[i].i_absatz++ ;
          Exemplar[i].i_frasa-=i_frasa ;
      }
      else
      if( i_absatz<Exemplar[i].i_absatz )
          Exemplar[i].i_absatz++ ;
   }
}
/************************************************************************/
//        разделить фразу на две
/************************************************************************/
void t_Macrostruct :: split_frasa( long i_absatz, long i_frasa, long px )
{  long i ;

   if( i_absatz<0 || Absatz.j<=i_absatz ) return ;
   t_Absatz &A=Absatz[i_absatz] ;
   if( i_frasa<0  || A.Frasa.j-1<=i_frasa ) return ;
   t_Frasa &F=A.Frasa[i_frasa] ;
   if( px<F.begin || F.end<px ) return ;

   t_Frasa F1 ;
   A.Frasa.add( F1 );

   for( i=A.Frasa.j-1 ; i_frasa<i ; i-- )
      A.Frasa[i+1]=A.Frasa[i] ;

   A.Frasa[i_frasa].end    =px-1 ;
   A.Frasa[i_frasa+1].begin=px ;
}
/************************************************************************/
//        вставить пустой абзац
/************************************************************************/
void t_Macrostruct :: insert_absatz( long i_absatz )
{  long i ;
   t_Absatz A ;

   Absatz.add( A );
   for( i=Absatz.j-2 ; i_absatz<=i ; i-- )
      Absatz[i+1]=Absatz[i] ;
   Absatz[i_absatz].Frasa.j=0 ;

   // Здесь
   t_wExemplarList &Exemplar=Kontext.Exemplar ;
   for( i=0 ; i<Exemplar.j ; i++ )
      if( i_absatz<Exemplar[i].i_absatz )
          Exemplar[i].i_absatz++ ;
}
/************************************************************************/
//       удалить пустой абзац
/************************************************************************/
void t_Macrostruct :: delete_absatz( long i_absatz )
{  long i ;

   if( 0<Absatz[i_absatz].Frasa.j ) return ;

   for( i=i_absatz ; i<Absatz.j-1 ; i++ )
      Absatz[i]=Absatz[i+1] ;
   Absatz.j-- ;

   // Здесь
   t_wExemplarList &Exemplar=Kontext.Exemplar ;
   for( i=0 ; i<Exemplar.j ; i++ )
      if( i_absatz<Exemplar[i].i_absatz )
          Exemplar[i].i_absatz-- ;
}
/************************************************************************/
//      удалить фразу
/************************************************************************/
void t_Macrostruct :: delete_frasa( long i_absatz, long i_frasa )
{  long i ;

   for( i=i_frasa ; i<Absatz[i_absatz].Frasa.j-1 ; i++ )
      Absatz[i_absatz].Frasa[i]=Absatz[i_absatz].Frasa[i+1] ;
   Absatz[i_absatz].Frasa.j-- ;

   // Здесь
   t_wExemplarList &Exemplar=Kontext.Exemplar ;
   for( i=0 ; i<Exemplar.j ; i++ )
      if( i_absatz==Exemplar[i].i_absatz && i_frasa<Exemplar[i].i_frasa )
          Exemplar[i].i_frasa-- ;
}
/***********************************************************************/
//                  слить абзац с последующим
/***********************************************************************/
void t_Macrostruct :: merge_absatz( long i_absatz )
{  long i,n_frasa ;

   if( i_absatz<0 || Absatz.j-1<=i_absatz ) return ;

   n_frasa=Absatz[i_absatz+1].Frasa.j ;
   for( i=0 ; i<Absatz[i_absatz+1].Frasa.j ; i++ )
      Absatz[i_absatz].Frasa.add( Absatz[i_absatz+1].Frasa[i] );
   for( i=i_absatz+1 ; i<Absatz.j-1 ; i++ )
      Absatz[i]=Absatz[i+1] ;
   Absatz.j-- ;

   // Здесь
   t_wExemplarList &Exemplar=Kontext.Exemplar ;
   for( i=0 ; i<Exemplar.j ; i++ )
   {  if( i_absatz+1==Exemplar[i].i_absatz )
          Exemplar[i].i_frasa+=n_frasa ;
      if( i_absatz<Exemplar[i].i_absatz )
          Exemplar[i].i_absatz-- ;
   }
}
/***********************************************************************/
//                  слить фразу с последующей
/***********************************************************************/
void t_Macrostruct :: merge_frasa( long i_absatz, long i_frasa )
{  long  i ;
   t_Absatz &A=Absatz[i_absatz] ;

   if( i_frasa<0 || A.Frasa.j-1<=i_frasa ) return ;
   A.Frasa[i_frasa].end=A.Frasa[i_frasa+1].end ;
   for( i=i_frasa+1 ; i<A.Frasa.j-1 ; i++ )
      A.Frasa[i]=A.Frasa[i+1] ;
   A.Frasa.j-- ;

   //Здесь должна быть коррекция Exemplar[i].i_absatz и Exemplar[i].i_frasa
}
/************************************************************************/
//         удалить обьект из списка Sort (абзац или фразу)
/************************************************************************/
void t_Macrostruct :: delete_sort( long i_sort )
{  t_HT_Sort *S=&Sort[i_sort] ;

   if( S->i_frasa<0 )
   {   if( 0<Absatz[S->i_absatz].Frasa.j )
       {   if( 0<S->i_absatz )
               merge_absatz( S->i_absatz-1 );
       }
       else
           delete_absatz( S->i_absatz );
   }
   else
   {   delete_frasa( S->i_absatz, S->i_frasa );
   }
   make_ht_sort( );
}
/*************************************************************************/
//                        дать фразу оригинала
/*************************************************************************/
long t_Macrostruct :: get_frasa( long i_Absatz, long i_Frasa, char *Str )
{  long L ;

   t_Frasa &F=Absatz[i_Absatz].Frasa[i_Frasa] ;
   L=myChar_to_koi9( Text->text_all()+F.begin, (uchar *)Str, F.end-F.begin, 1000 );
   return L ;
}
/*************************************************************************/
//                        дать фразу перевода
/*************************************************************************/
long t_Macrostruct :: get_frasa2( long i_Absatz, long i_Frasa, char *Str )
{  long L ;

   t_Frasa &F=Absatz[i_Absatz].Frasa[i_Frasa] ;
   L=myChar_to_koi9( F.Str.Str, (uchar *)Str, F.Str.Length, 1000 );
   return L ;
}
/*************************************************************************/
//                     установить перевод фразе
/*************************************************************************/
void t_Macrostruct :: put_frasa( long i_Absatz, long i_Frasa, char *Str )
{
   long L ;
   t_myChar Str1[1000] ;

   L=koi9_to_myChar ( Str, Str1, strlen(Str), 1000 );
   Absatz[i_Absatz].Frasa[i_Frasa].Str.set( Str1 ) ;
}
/**********************************************************************/
//                   написать тип абзаца в образ
/**********************************************************************/
void t_Macrostruct :: print_type_absatz( char *File )
{  long i ;

   fw=fopen( File,"w");
   fprintf( fw,"@параграфы{\n" );
   fprintf( fw,"@t_TypeAbzatz{\n" );

   for( i=0 ; i<TypeAbsatz.j ; i++ )
   {
      print_hex( "Color"    , TypeAbsatz[i].Color );
      print_int( "not_trans", TypeAbsatz[i].not_trans );
      print_str( "Name"     , TypeAbsatz[i].Name );
      print_str( "begin"    , TypeAbsatz[i].begin );
      print_str( "end"      , TypeAbsatz[i].end );
   }
   fprintf( fw,"}\n" );
   Fclose( fw );
}
/**********************************************************************/
//                  прочесть тип абзаца из образа
/**********************************************************************/
void t_Macrostruct :: scanf_type_absatz( char *File )
{  long  i,z ;
   t_TypeAbsatz TA ;

   file_to_mass1( File );
   TypeAbsatz.j=0 ;

   z_Mass=0 ;
   while( 0==is_end_block() )
   {  
      if( 0!=strncmp( t_Serial::Mass+z_Mass,"@Color{",7 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      scanf_hex( &z ); TA.Color=z ;
      scanf_int( &z ); TA.not_trans=z ;
      scanf_str ( TA.Name,40 );
      scanf1_str( &TA.begin );
      scanf1_str( &TA.end );
      TypeAbsatz.add( TA );
      z_Mass=find_end(z_Mass)+1 ;
   }
   z_Mass=find_end(z_Mass)+1 ;
   Free(t_Serial::Mass);
}
// ***********************************************************************
//  Форматировать строку по правому и левому краю
//  In Out     - входная и выходная строка
//  First      - красная строка
//  Left Right - левый и правый край абзаца
// ***********************************************************************
void format( t_String &In, t_String &Out, short First, short Left, short Right )
{
   long   Width=Right-Left ;
   long   z_space,   // индекс последнего пробела
          z_return ; // индекс последнего возврата
   char   *Space,c,f ;
   long   i,i1,n,D ;   

   // -------- сделать все пробелы одинарными -------------
   const char  *Str0 =In.c_str() ;
         long   L0   =strlen( Str0 );
   t_String  In1 ;
   for( i=0,f=0 ; i<L0 ; i++ )
   {  c=Str0[i] ;
      if( '\n'==c ) c=' ' ;
      if( ' '==c )
      {  if( 0==f )
             In1+=c ;
      }
      else
      {  In1+=c ;
      }
      f=(' '==c) ;
   }
   // ------- собственно отформатировать -----------------
   const char  *Str =In1.c_str() ;
   long   L   =strlen( Str );

   Space = (char *)calloc(Width,sizeof(char) );
   Out   = "" ;

   z_space=z_return=0 ;
   for( i=0 ; i<Left ; i++ )
      Out+=' ';
   for( i=0 ; i<L ; i++ )
   {  if( Str[i]==' ' ) z_space=i ;

      if( Width<=i-z_return )
      {   // --------- проставить длинну пробелам ------------------
          D=Width-(z_space-z_return);
          while( 0<=D )
          {  for( i1=z_return ; i1<z_space && 0<=D ; i1++ )
                if( Str[i1]==' ' )
                {   Space[i1-z_return]++ ; D-- ;  }
          }
          // -------- сформировать строку ------------------------------
          for( i1=z_return ; i1<z_space ; i1++ )
          {  Out+=Str[i1] ;
             if( Str[i1]==' ' )
                 for( n=Space[i1-z_return] ; 0<n ; n-- )
		    Out+=' ' ;
          }
          // --------- проставть все на место --------------------------
          Out+='\n' ;
	  for( i=0 ; i<Left ; i++ )
	     Out+=' ';
          z_return=z_space+1 ;
          for( i1=0 ; i1<Width ; i1++ )
             Space[i1]=0 ;
      }	 
   }
   for( i1=z_return ; i1<L ; i1++ )
      Out+=Str[i1] ;
}
/***********************************************************************/
//       Напечатать итоговый текст 
/***********************************************************************/
void t_Macrostruct :: print_result( char *File )
{
   long        i,i_absatz,i_frasa ;
   long        type_text=-1 ;    // тип абзаца с именем "Text"
   t_String Str1,Str2 ;
   char        Str[1000] ;

   FILE *fw=Fopen( File,"w" );

   for( i=0 ; i<TypeAbsatz.j ; i++ )
      if( 0==strcmp( TypeAbsatz[i].Name,"Text") )
          type_text=i ;

   // ------ заголовок текста ---------------------------
   if( 0<=type_text )
       fprintf( fw, TypeAbsatz[type_text].begin );

   // ------ абзацы текста ---------------------------
   for( i_absatz=0 ; i_absatz<Absatz.j ; i_absatz++ )
   {  t_Absatz &A=Absatz[i_absatz] ;
   
      Str1="" ;
      fprintf( fw, "\n%s\n", TypeAbsatz[A.type].begin );
      for( i_frasa=0 ; i_frasa<A.Frasa.j ; i_frasa++ )
      {  get_frasa2( i_absatz,i_frasa, Str );
         Str1+=Str ;
      }
      format( Str1,Str2,4,0,75 );
      fprintf( fw, Str2.c_str() );
      fprintf( fw, "\n%s\n", TypeAbsatz[A.type].end );
   }

   // ------ конец текста ---------------------------
   if( 0<=type_text )
       fprintf( fw, TypeAbsatz[type_text].end );
   
   Fclose(fw);
}
/************************************************************************/
void t_Macrostruct :: print_debug_str32( )
{  long i_absatz,i_frasa ;
   
   for( i_absatz=0 ; i_absatz<Absatz.j ; i_absatz++ )
   {  t_Absatz &A=Absatz[i_absatz] ;
   
      for( i_frasa=0 ; i_frasa<A.Frasa.j ; i_frasa++ )
      {  printf("\n %d %d %x",i_absatz,i_frasa,A.Frasa[i_frasa].Str.Str );
      }
   }
}
/************************************************************************/
t_Frasa :: t_Frasa()
{  init();
}
/************************************************************************/
void t_Frasa :: init()
{   begin  =0 ;
    end    =0 ;
    perevod=NOT ;
    flag1  =1 ;
    Str.init();
}
/************************************************************************/
t_Frasa :: ~t_Frasa()
{   del();
}
/************************************************************************/
void t_Frasa :: del()
{
    Str.del();
}
/************************************************************************/
void t_Frasa :: operator = ( t_Frasa &F )
{
    begin  =F.begin ;
    end    =F.end ;
    perevod=F.perevod ;
    flag1  =F.flag1 ;
    Str    =F.Str ;
}
/************************************************************************/
t_Absatz :: t_Absatz()
{   init();
}
/************************************************************************/
void t_Absatz :: init()
{  type=0 ;
   Frasa.init();
}
/************************************************************************/
void t_Absatz :: del()
{  Frasa.del();
}
/************************************************************************/
void t_Absatz :: operator = ( t_Absatz &A )
{  type =A.type ;
   Frasa=A.Frasa ;
}
/************************************************************************/
t_String32 :: t_String32( )
{  init(); }
/************************************************************************/
t_String32 :: ~t_String32( )
{  del();  }
/************************************************************************/
void t_String32 :: init( )
{  Str=NULL ; Length=0 ;  }
/************************************************************************/
void t_String32 :: del( )
{
   Free(Str); Length=0 ;
}
/************************************************************************/
void t_String32 :: operator = ( t_String32 &S )
{  if( Length<S.Length )
   {   Str=(t_myChar *)Realloc( Str,S.Length*sizeof(t_myChar) );
       Length=S.Length ;
   }
   if( 0<S.Length )
       strcpy32( Str,S.Str );
}
/************************************************************************/
void t_String32 :: set( t_myChar *_Str, long L )
{
   if( L<0 ) L=strlen32( _Str )+1;
   if( Length<=L )
   {   long L1=(L/50+1)*50 ;
       Str=(t_myChar *)Realloc( Str,L1*sizeof(t_myChar) );
       Length=L1 ;
   }
   strncpy32( Str,_Str,L );
   Str[L]=0 ;
}
/************************************************************************/
long t_String32 :: strlen()
{  if( Str==NULL ) return 0 ;
       else strlen32( Str );
}
/************************************************************************/
DEFINE_LIST_TYPE ( t_Frasa  ,t_FrasaList )
DEFINE_LIST_TYPE ( t_Absatz ,t_AbsatzList )
DEFINE_LIST_BTYPE( t_HT_Sort,t_HT_SortList )
DEFINE_LIST_BTYPE( t_TypeAbsatz,t_TypeAbsatzList )

