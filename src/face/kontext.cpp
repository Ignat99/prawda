/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//              Структуры для построения контекстного словаря
//     t_Kontext - все данные о контексте
//     t_Serial  - класс для записи-чтения образов
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# include <string.h>
# include <string>

# include "core.h"
# include "macrostruct.h"
# include "kontext.h"

extern t_Macrostruct Macrostruct ;
extern t_Kontext     Kontext ;
extern t_Slowo3      Perevod1 ;
extern t_Slowo3      Perevod2 ;
extern t_Core        Core ;

void message( char *Str );
char s_break( );
int find_i_slowo( char *Str );

/************************************************************************/
t_Kontext :: t_Kontext()
{
   exist=0 ;
   Word00=NULL ;
   Word0.init();
   Word1.init();
   Exemplar.init();
   Type.init();
   i_Exemplar.init();
}
/************************************************************************/
void t_Kontext :: make( )
{
   Word0.j=0 ;
   Word1.j=0 ;
   Exemplar.j=0 ;
   Type.j=0 ;
   i_Exemplar.j=0 ;

   make_exemplar( );
   make_word1( );
   make_type( );
   test( );
   exist=1 ;
}
/************************************************************************/
void t_Kontext :: del( )
{
   Word0.del();
   Word1.del();
   Exemplar.del();
   Type.del();
   i_Exemplar.del();
   exist=0 ;
}
/************************************************************************/
int word1cmp( const void *a, const void *b )
{
    return strcmp( ((t_wWord*)a)->str,((t_wWord*)b)->str );
}
/************************************************************************/
//                    построение массива word1
/************************************************************************/
void t_Kontext :: make_word1( )
{  long i,j,j_Word ;

   Word1=Word0 ;
   for( i=0 ; i<Word1.j ; i++ )
      Word1[i].i_word=i ;
   qsort( Word1.list,Word1.j,sizeof(t_wWord),word1cmp );
   for( j=0,i=1 ; i<Word1.j ; i++ )
   {  if( 0!=strcmp( Word1[j].str,Word1[i].str ) )
               Word1[++j]=Word1[i] ;
	  else Word1[j].num++ ;
      Word0[Word1[i].i_word].i_word=j ; // ссылка Word0->Word1
   }
   Word1.j=j ;
}
/*************************************************************************/
//       построение массива Interes ("Интересные" конструкции)
/*************************************************************************/
void t_Kontext :: make_interes_struct()
{  long i,z_interes=-1 ;

   Interes.Calloc(Grammar.n_trans());

   for( i=0 ; i<Grammar.n_trans() ; i++ )
      if( 0==strcmp( Grammar[i].From.Name,"@интерес") )
      {   z_interes=i ; break ;  }
   if( z_interes<0 ) return ;

   for( i=0 ; i<Grammar.n_trans() ; i++ )
      Interes[i]=1 ;   
   for( i=0 ; i<Grammar.n_trans() ; i++ )
      if( Grammar[i].From.type==TWORD )
          Interes[i]=0 ;
   for( i=0 ; i<Grammar[z_interes].From.Word.j ; i++ )
      if( 0<Grammar[z_interes].From.Word[i].i_struct )
          Interes[Grammar[z_interes].From.Word[i].i_struct]=0 ;
}
/************************************************************************/
//                   построение массива Exemplar
/************************************************************************/
void t_Kontext :: make_exemplar( )
{
   long i,i_struct,i_word,i4,i5,j_Word,j_Word00,n_word ;
   char f_VarRep ;
   t_HT_Sort *S ;
   char present[10] ; // есть ли у нас конструкция такй длинны
   char Str[1000],Str1[1000] ;

   make_interes_struct();
   // ------ цикл по фразам ----------------------------------------

   f_VarRep=Core.f_VarRep ;
   Core.f_VarRep=1 ;

   Free( Word00 );
   Word00=(char *)Calloc( Macrostruct.Text->length(),sizeof(char) );

   for( long i=j_Word00=0 ; i<Macrostruct.Sort.j ; i++ )
   {
      if( i%10==0 )
      {   sprintf( Str1,"Построено %d/%d",i,Macrostruct.Sort.j );
          message( Str1 );
      }
      S=&Macrostruct.Sort[i] ;
      if( S->i_frasa<0 ) continue ;
      t_Frasa &Frasa=Macrostruct.Absatz[S->i_absatz].Frasa[S->i_frasa] ;
      Macrostruct.get_frasa ( S->i_absatz, S->i_frasa, Str );

      Core.translate( Str,Str1 ); // можно заменить на построение главной таблицы      
      t_ItemList &From=Core.from();

      // ------- построение Word0 ---------------------------
      t_wWord W ;
      j_Word=Word0.j ;
      for( i_word=0 ; i_word<From.j ; i_word++ )
      {  strcpy( Word00+j_Word00, From[i_word].Str );
         W.str     =Word00+j_Word00 ;
	 W.i_slowo =From[i_word].i_slowo ;
         W.num     =1 ;
	 Word0.add( W );
         j_Word00+=strlen( From[i_word].Str )+1;
      }
      // -------- построение Exemplar -----------------------
      for( i_struct=0 ; i_struct<Core.fn_struct(); i_struct++ )
      {
         if( 1==Interes[i_struct] ) continue ;
         for( i_word=0 ; i_word<Core.fn_word(); i_word++ )
         {  t_Variants *VV=Core.variants( i_word, i_struct );
	    for( i4=0 ; i4<10 ; i4++ )
	       present[i4]=0 ;
	    for( i4=0 ; i4<VV->Variant.j ; i4++ )
	    {  t_rStruct *V=&VV->Variant[i4] ;
	       n_word=V->i_last_word-V->i_word ;
	       if( n_word<6 && present[n_word]==0 )
	       {   // Внести очередной вариант
		   t_wExemplar E ;
		   E.i_slowo      =V->i_slowo ;
		   E.i_struct     =V->i_struct ;
	           E.i_word       =j_Word+V->i_word ;
		   E.i_last_word  =j_Word+V->i_last_word ;
	           E.i_word1      =       V->i_word ;
		   E.i_absatz     =S->i_absatz ;
		   E.i_frasa      =S->i_frasa ;
		   E.i_type       =0 ;
		   t_String Str ;
        	   for( i5=E.i_word ; i5<E.i_last_word ; i5++ )
		   {  if( E.i_word<i5 ) Str+=" " ;
		      Str+=Word0[i5].str ;
		   }
		   strncpy( E.Str, Str.c_str(), 100 );
		   Exemplar.add( E );
	           present[n_word]=1 ;
	       }
	    }
	 }
      }
   }
   Core.f_VarRep=f_VarRep ;
}
/************************************************************************/
//         сравнение экземпляров для построения массива типов
/************************************************************************/
int exemplar_cmp( const void *a, const void *b )
{
   t_wExemplar *A=(t_wExemplar *)a ;
   t_wExemplar *B=(t_wExemplar *)b ;
   long r,i1,i2 ;

   if( 0<=A->i_slowo && 0<=B->i_slowo )
       return A->i_slowo - B->i_slowo ;
   if( A->i_slowo<0 && 0<=B->i_slowo ) return  1 ;
   if( B->i_slowo<0 && 0<=A->i_slowo ) return -1 ;

   for( i1=A->i_word,i2=B->i_word ; i1<A->i_last_word && i2<B->i_last_word ; i1++,i2++ )
   {  
      if( 0<=Kontext.Word0[i1].i_slowo && 0<=Kontext.Word0[i2].i_slowo )
      {   r=Kontext.Word0[i1].i_slowo-Kontext.Word0[i2].i_slowo ;
          if( r!=0 ) return r ;
      }
      else
      {	  if( Kontext.Word0[i1].i_slowo<0 && 0<=Kontext.Word0[i2].i_slowo ) return 1 ;
	  if( Kontext.Word0[i2].i_slowo<0 && 0<=Kontext.Word0[i1].i_slowo ) return -1 ;

	  r=Kontext.Word0[i1].i_word-Kontext.Word0[i2].i_word ;      
	  if( r!=0 ) return r ;
      }
   }
   return (A->i_last_word-A->i_word) - (B->i_last_word-B->i_word) ;
}
/************************************************************************/
//      сравнение экземпляров для построения Overlap
/************************************************************************/
int exemplar1_cmp( const void *a, const void *b )
{
   t_wExemplar *A=*((t_wExemplar **)a) ;
   t_wExemplar *B=*((t_wExemplar **)b) ;

   return A->i_word - B->i_word ;
}
/************************************************************************/
//                       построение массива Type
//         можно отработать более прямо Exemplar1 (он без повторений)
//         может заменить собой Exemplar
/************************************************************************/
void t_Kontext :: make_type( )
{  long     i,i1,j,w1,w2 ;
   t_wExemplarList Exemplar1 ;

   Exemplar1=Exemplar ;
   for( i=0 ; i<Exemplar1.j ; i++ )
   {  Exemplar [i].index=i ;
      Exemplar1[i].index=i ;
   }
   qsort( Exemplar1.list, Exemplar1.j, sizeof(t_wExemplar), exemplar_cmp );

   // ------- исключить полностью накладывающиеся варианты --------------
   w1=w2=-1 ;
   for( i=j=0 ; i<Exemplar1.j ; i++ )
   {  if( Exemplar1[i].i_word!=w1 || Exemplar1[i].i_last_word!=w2 )
          Exemplar1[j++]=Exemplar1[i] ;
      w1=Exemplar1[i].i_word ;
      w2=Exemplar1[i].i_last_word ;
   }
   Exemplar1.j=j ;
   
   for( i=0 ; i<Exemplar1.j ; i++ )
   {
      if( i==0 || 0!=exemplar_cmp( &Exemplar1[i-1],&Exemplar1[i] ) )
      {   t_wExemplar *E=&Exemplar1[i] ;
          t_wType      T ;
          t_String  Str ;

          T.used    =0 ;
	  T.i_struct=E->i_struct ;
          T.i_slowo =E->i_slowo ;
	  T.i_exemplar=i ;
	  T.select  =0 ;
	  T.n_hole    =0 ;
          for( i1=E->i_word ; i1<E->i_last_word ; i1++ )
	  {  if( E->i_word<i1 ) Str+=" " ;
	     Str+=Word0[i1].str ;
	  }
	  Type.add(T) ;
	  strncpy( Type[Type.j-1].Str, Str.c_str(), 100 );
      }
      i_Exemplar.add( Exemplar1[i].index );
      if( 0<Type.j )
	  Type[Type.j-1].i_last_exemplar=i ;
   }
   if( 0<Type.j )
       Type[Type.j-1].i_last_exemplar=i ;
   for( i=0 ; i<Type.j ; i++ )
      Type[i].num=Type[i].i_last_exemplar-Type[i].i_exemplar ;
   // -------- проставить i_slowo типам ---------------------
   for( i=0 ; i<Type.j ; i++ )
      for( i1=Type[i].i_exemplar ; i1<Type[i].i_last_exemplar ; i1++ )
      {  Exemplar[Exemplar1[i1].index].i_type=i ;
         if( 0<=Exemplar1[i1].i_slowo && Type[i].i_slowo<0 )
	     Type[i].i_slowo=Exemplar1[i1].i_slowo ;
      }
}
/************************************************************************/
//   построение первого (компьютерного) варианта контекстного перевода
/************************************************************************/
void t_Kontext :: make_default_perevod( )
{  long i,i1 ;

   for( i=0 ; i<Type.j ; i++ )
   {  t_wType &T=Kontext.Type[i] ;
      if( 0<=T.i_slowo )
      {	  t_String STmp ;
	  Perevod->print_record( STmp, T.i_slowo );
	  const char *Str=STmp.c_str() ;
	  for( i1=0 ; Str[i1]!=0 ; i1++ )
	     if( Str[i1]==';' )
		 break ;
	  strncpy( T.Str, Str, min(100,i1) );
      }
   }
}
/************************************************************************/
//   сделать текущий словарь (название функции неудачное) make_current_dict
/************************************************************************/
void t_Kontext :: make_current_perevod( )
{  long i,L ;

try
{
   // -------- захватить память ---------------
   for( L=i=0 ; i<Type.j ; i++ )
   {  t_wType &T=Kontext.Type[i] ;
      if( NULL!=strstr( T.Str,"=") )
         L+=strlen( T.Str )+1 ;
   }
   // -------- заполнить массив ---------------
   char *Mass=(char *)Malloc( L,sizeof(char) );
   for( L=i=0 ; i<Type.j ; i++ )
   {  t_wType &T=Kontext.Type[i] ;
      if( NULL!=strstr( T.Str,"=") )
      {  strcpy( Mass+L,T.Str );
         L+=strlen( T.Str );
	 Mass[L++]='\n';
      }
   }
   if( 0<L ) Perevod2.make( Mass,&Grammar.format1() );
       else  Perevod2.make( ""  ,&Grammar.format1() );
   Free( Mass );
}
catch( int E )
{  error_add( "t_Kontext :: make_current_perevod\n" );
   throw( E );
}
}
/************************************************************************/
void t_Kontext :: test( )
{  long i,i1 ;

   for( i=0 ; i<Type.j ; i++ )
   {  t_wType *T=&Type[i] ;
      for( i1=T->i_exemplar ; i1<T->i_last_exemplar ; i1++ )
         if( Exemplar[i_Exemplar[i1]].i_type!=i )
	     printf( "\nError t_Kontext :: test 1" );
   }
   for( i=0 ; i<Exemplar.j ; i++ )
      if( Type.j<=Exemplar[i].i_type )
          printf("\nError t_Kontext :: test 2");
}
/************************************************************************/
//                     дублировать выражение
/************************************************************************/
void t_Kontext :: double_type( long i_Type )
{
   long i ;
   t_wType T ;

   Type.add( T );
   for( i=Type.j-2 ; i_Type<=i ; i-- )
      Type[i+1]=Type[i] ;
   for( i=0 ; i<Exemplar.j ; i++ )
      if( i_Type<=Exemplar[i].i_type )
          Exemplar[i].i_type++ ;
}
/************************************************************************/
//                     удалить выражение
/************************************************************************/
void t_Kontext :: delete_type( long i_Type )
{  long i,i1 ;

   test( );
	  
   for( i=i_Type+1 ; i<Type.j ; i++ )
      Type[i-1]=Type[i] ;
   Type.j-- ;
   correct_i_type();
   test( );
}
/************************************************************************/
//                скорректировать Exemplar[].i_type
/************************************************************************/
void t_Kontext :: correct_i_type()
{  long i,i1 ;

   for( i=0 ; i<Exemplar.j ; i++ ) // только что вставил
      Exemplar[i].i_type=-1 ;

   for( i=0 ; i<Type.j ; i++ )
   {  t_wType *T=&Type[i] ;
      for( i1=T->i_exemplar ; i1<T->i_last_exemplar ; i1++ )
         Exemplar[i_Exemplar[i1]].i_type=i ;
   }
}
/************************************************************************/
//       подсчитать число слов непокрытых переводами ("дырок")
/************************************************************************/
void t_Kontext :: calc_n_hole( )
{  long i,i1,t ;

   // ---- обнулить у слов флаг "переведено" ----------
   for( i=0 ; i<Word0.j ; i++ )
      Word0[i].f_perevod=0 ;

   // ---- проставить у слов флаг "переведено" ----------
   for( t=0 ; t<Type.j ; t++ )
   {  
      if( NULL==strstr( Type[t].Str,"=" ) ) continue ;
      for( i=Type[t].i_exemplar ; i<Type[t].i_last_exemplar ; i++ )
      {  t_wExemplar *E=&Exemplar[i_Exemplar[i]] ;
         for( i1=E->i_word ; i1<E->i_last_word ; i1++ )
	    Word0[i1].f_perevod=1 ;
      }
   }
   // ---- посчитать количество "дырок" каждого выражения -----
   for( t=0 ; t<Type.j ; t++ )
   {  Type[t].n_hole=0 ;
      if( NULL!=strstr( Type[t].Str,"=" ) ) continue ;
      for( i=Type[t].i_exemplar ; i<Type[t].i_last_exemplar ; i++ )
      {  t_wExemplar *E=&Exemplar[i_Exemplar[i]] ;
         for( i1=E->i_word ; i1<E->i_last_word ; i1++ )
	    Type[t].n_hole+=(Word0[i1].f_perevod==0) ;
      }
   }
}
/************************************************************************/
void cat_perevod_string( t_String &STmp )
{
   int L=STmp.length();
   for( int i=0 ; i<L ; i++ )
      if( STmp[i]=='=' )
      {   STmp.resize( i ) ; break ; }
}
/************************************************************************/
//            проверить Type[].i_slowo при чтении из образа
/************************************************************************/
void t_Kontext :: restore_i_slowo( )
{  long     i ;
   t_String STmp1,STmp2 ;

   // ------ проверка не уплыли ли i_slowo ---------------
   for( i=0 ; i<Type.j ; i++ )
   {  if( Type[i].i_slowo<0 ) continue ;
      // --------- проверить на соответствие с текущим словарем -------
      Perevod1.print_record( STmp1, Type[i].i_slowo );
      STmp2=Type[i].Str ;
      cat_perevod_string( STmp1 );
      cat_perevod_string( STmp2 );
      if( 0!=strcmp( STmp1.c_str(),STmp2.c_str() ) )
	  Type[i].i_slowo=-1 ;
   }
}
/************************************************************************/
//            проставить Type[].i_slowo при чтении из образа
/************************************************************************/
void t_Kontext :: restore_i_slowo1( )
{
   long  i,i1,i2,t,L,L1,L2,z_begin,z_end ;
   char  f_trans ;
   t_HT_Sort *S ;
   static char     Str1[1000],Str2[1000] ;

   char f_VarRep=Core.f_VarRep ;
   Core.f_VarRep=1 ;

   z_begin=0 ;
   for( i=0 ; i<Macrostruct.Sort.j ; i++ )
   {  
      S=&Macrostruct.Sort[i] ;
      if( S->i_frasa<0 ) continue ;
      // ------- определить экземпляры, относящиеся к этой фразе -------
      z_end=Exemplar.j ;
      for( i1=z_begin ; i1<Exemplar.j ; i1++ )
         if( S->i_absatz<Exemplar[i1].i_absatz ||
	     S->i_frasa <Exemplar[i1].i_frasa )
         {  z_end=i1 ; break ;  }

      // ------ определить переводить фразу или нет ---------
      f_trans=0 ;
      for( i1=z_begin ; i1<z_end ; i1++ )
      {  t=Exemplar[i1].i_type ;
         if( Type[t].i_struct<0 ) continue ;
         if( Type[t].i_slowo<0 && 0==Interes[Type[t].i_struct] )
	 {   f_trans=1 ; break ;  }
      }
      if( f_trans!=0 )
      {
	 // ------ сделать перевод -----------------------------
	 t_Frasa   *F=&Macrostruct.Absatz[S->i_absatz].Frasa[S->i_frasa] ;
	 if( F->flag1==0 || F->perevod==HAND  ) continue ;

	 t_myChar *Str=Macrostruct.Text->text_all()+F->begin ;
	 long      L=F->end-F->begin ;
	 myChar_to_koi9( Str, (uchar *)Str1, L, 1000 );
	 Core.translate( Str1,Str2 );

	 // ------- таки проставить i_slowo --------------------
	 for( i1=z_begin ; i1<z_end ; i1++ )
	 {  t=Exemplar[i1].i_type ;
            if( Type[t].i_struct<0 ) continue ;
            if( 0<=Type[t].i_slowo || 0!=Interes[Type[t].i_struct] )
		continue ;
            t_Variants *V=Core.variants( Exemplar[i1].i_word1, Exemplar[i1].i_struct );
	    for( i2=0 ; i2<V->Variant.j ; i2++ )
	    {  L1=Exemplar  [i1].i_last_word-Exemplar  [i1].i_word ;
	       L2=V->Variant[i2].i_last_word-V->Variant[i2].i_word ;	 
	       if( L1==L2 )
	       {   Type[t].i_slowo=V->Variant[i2].i_slowo ;
	           break ;
	       }
	    }
	 }
      }
      z_begin=z_end ;
   }
   Core.f_VarRep=f_VarRep ;
}
/************************************************************************/
//              есть ли такое слово в главном словаре
/************************************************************************/
int find_i_slowo( char *Str )
{
   long        i,N ;
   short       part[20] ;
   t_longList  Ant2 ;

   N=Perevod->part( Str, part );
   for( i=0 ; i<N ; i++ )
   {  Perevod->find( Str, part[i], &Ant2 );
      if( 0<Ant2.j ) return Ant2[0] ;
   }
   return -1 ;
}
/**********************************************************************/
//               записывание образа данных в файл
/**********************************************************************/
void t_Kontext :: print_word( t_wWord &Word )
{
   fprintf( fw,"@word{" );
   print_str( "str"    , Word.str );
   print_int( "i_slowo", Word.i_slowo );
   print_int( "num"    , Word.num );
   fprintf( fw,"}\n" );
}
/**********************************************************************/
void t_Kontext :: print_exemplar( t_wExemplar &E )
{
   fprintf( fw,"@exemplar{" );
   print_int( "i_word"        , E.i_word );
   print_int( "i_last_word"   , E.i_last_word );
   print_int( "i_word1"       , E.i_word1 );
   print_int( "i_absatz"      , E.i_absatz );
   print_int( "i_frasa"       , E.i_frasa );
   print_int( "i_slowo"       , E.i_slowo );
   print_int( "i_type"        , E.i_type );
   print_str( "Str"           , E.Str );
   fprintf( fw,"}\n" );
}
/**********************************************************************/
void t_Kontext :: print_type( t_wType &T )
{
   t_Format1 *Format=&Grammar.format1() ;

   fprintf( fw,"@type1{" );
   print_int( "type"            , T.used );
   print_str( "i_struct"        , Format->get_tag(0,T.i_struct) );
   print_int( "i_slowo"         , T.i_slowo );
   print_int( "i_exemplar"      , T.i_exemplar );
   print_int( "i_last_exemplar" , T.i_last_exemplar );
   print_int( "num"             , T.num );
   print_int( "num1"            , T.n_hole );
   print_str( "Str"             , T.Str );
   fprintf( fw,"}\n" );
}
/**********************************************************************/
void t_Kontext :: print_all( char *File )
{
   FILE *fw1=fopen( File,"w");
   print_all( fw1 );
   Fclose( fw1 );
}
/**********************************************************************/
void t_Kontext :: print_all( FILE *fw1 )
{  long i ;

   fw=fw1 ;
   fprintf( fw,"@kontext{\n" );
   print_int( "exist",exist );

   fprintf( fw,"@Word0{\n" );
   for( i=0 ; i<Word0.j ; i++ )
      print_word( Word0[i] );
   fprintf( fw,"}\n" );

   fprintf( fw,"@Word1{\n" );
   for( i=0 ; i<Word1.j ; i++ )
      print_word( Word1[i] );
   fprintf( fw,"}\n" );

   fprintf( fw,"@Exemplar{\n" );
   for( i=0 ; i<Exemplar.j ; i++ )
      print_exemplar( Exemplar[i] );
   fprintf( fw,"}\n" );

   fprintf( fw,"@Type2{\n" );
   for( i=0 ; i<Type.j ; i++ )
      print_type( Type[i] );
   fprintf( fw,"}\n" );

   fprintf( fw,"@i_Exemplar{\n" );
   for( i=0 ; i<i_Exemplar.j ; i++ )
   {
      if( (i%10)==0 ) fprintf( fw,"\n" );
      print_int( "i",i_Exemplar[i] );
      fprintf( fw," " );
   }
   fprintf( fw,"}\n" );
   fprintf( fw,"}\n" );
}
/**********************************************************************/
//            считывание образа данных из файла
/**********************************************************************/
void t_Kontext :: scanf_all( char *File )
{  long i ;
   struct stat stat1 ;
try
{
   make_interes_struct();

   stat( File, &stat1 );
   mtime = stat1.st_mtime ;

   L=file_to_mass( File,&Mass );
   Free(Word00);
   Word00=(char *)Calloc( L,sizeof(char) );

   Word0.j=0 ;
   Word1.j=0 ;
   Exemplar.j=0 ;
   Type.j=0 ;
   i_Exemplar.j=0 ;
   j_Word00=0 ;

   z_Mass=find_tag( 0,"@kontext" );
   z_Mass=find_tag( 0,"@exist" );
   scanf_int( &i ); exist=i ;
   
   scanf_word( Word0 );
   scanf_word( Word1 );
   scanf_exemplar( );
   scanf_type( );
   scanf_i_exemplar( );

   for( i=0 ; i<Exemplar.j ; i++ )
   {  Exemplar[i].i_struct=Type[Exemplar[i].i_type].i_struct ;
      Exemplar[i].i_slowo =Type[Exemplar[i].i_type].i_slowo  ;
   }
   Free(Mass);
   if( mtime<Perevod1.mtime )
   {  restore_i_slowo( );
      restore_i_slowo1( );
   }
   test();
}
catch( int E )
{
   Free(Mass);
   throw( E );
}
}
/**********************************************************************/
void t_Kontext :: scanf_word( t_wWordList &WL )
{  t_wWord W ;

//   if( 0==strncmp( Mass+find_at( z_Mass ),"@Word",5 ) )
//       printf("\nError in scanf_word");

   while( 0==is_end_block() )
   {  
      if( 0!=strncmp( Mass+z_Mass,"@str",4 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      W.str=Word00+j_Word00 ;
      scanf_str( W.str,L );
      j_Word00+=strlen( W.str )+1 ;
      scanf_int( &W.i_slowo );
      scanf_int( &W.num );
      WL.add( W );
      z_Mass=find_end(z_Mass)+1 ;
   }
   z_Mass=find_end(z_Mass)+1 ;
}
/**********************************************************************/
void t_Kontext :: scanf_exemplar( )
{  t_wExemplar E ;

//   if( 0==strncmp( Mass+find_at( z_Mass ),"@Exemplar",9 ) )
//       printf("\nError in scanf_exemplar");

   while( 0==is_end_block() )
   {
      if( 0!=strncmp( Mass+z_Mass,"@i_word",7 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      scanf_int( &E.i_word );
      scanf_int( &E.i_last_word );
      scanf_int( &E.i_word1 );
      scanf_int( &E.i_absatz );
      scanf_int( &E.i_frasa );
      scanf_int( &E.i_slowo );
      scanf_int( &E.i_type );
      scanf_str(  E.Str, 100 );
      Exemplar.add(E);
      z_Mass=find_end(z_Mass)+1 ;
   }
   z_Mass=find_end(z_Mass)+1 ;
}
/**********************************************************************/
void t_Kontext :: scanf_type( )
{  t_wType T ;
   long    n ;
   char    Str[10] ;

   t_Format1 *Format=&Grammar.format1() ;

//   if( 0==strncmp( Mass+find_at( z_Mass ),"@Type",5 ) )
//       printf("\nError in scanf_type");

   while( 0==is_end_block() )
   {
      if( 0!=strncmp( Mass+z_Mass,"@type{",6 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      scanf_int( &n ); T.used=n ;
      scanf_str( Str,10 );
      T.i_struct=Format->find_struct( 0, Str );
      scanf_int( &T.i_slowo );
      scanf_int( &T.i_exemplar );
      scanf_int( &T.i_last_exemplar );
      scanf_int( &T.num );
      scanf_int( &T.n_hole );
      scanf_str(  T.Str, 100 );
      T.select=0 ;
      Type.add(T);
      z_Mass=find_end(z_Mass)+1 ;
   }
   z_Mass=find_end(z_Mass)+1 ;
}
/**********************************************************************/
void t_Kontext :: scanf_i_exemplar( )
{
//   if( 0==strncmp( Mass+find_at( z_Mass ),"@Exemplar",9 ) )
//       printf("\nError in scanf_i_exemplar");

   while( 0==is_end_block() )
   {
      if( 0!=strncmp( Mass+z_Mass,"@i{",3 ) )
      {   z_Mass=find_at(z_Mass+1); continue ;  }
      long ii ;
      scanf_int( &ii );
      i_Exemplar.add(ii);
   // z_Mass=find_end(z_Mass)+1 ;
   }
   z_Mass=find_end(z_Mass)+1 ;
}
/**********************************************************************/
//        класс t_Serial для чтения и записи образов файла
/**********************************************************************/
void t_Serial :: file_to_mass1( char *File )
{
   L=file_to_mass( File, &Mass );
}
/**********************************************************************/
void t_Serial :: print_int( char *tag, long i )
{
   fprintf( fw,"@%s{%d}",tag,i );
}
/**********************************************************************/
void t_Serial :: print_hex( char *tag, long i )
{
   fprintf( fw,"@%s{%h}",tag,i );
}
/**********************************************************************/
void t_Serial :: print_str( char *tag, char *s )
{
   long L=strlen( s );
   if( 0<L && s[L-1]=='\\' ) // Заплатка, чтобы можно было печатать строчки типа "\}"
            fprintf( fw,"@%s{%s }",tag,s ); 
       else fprintf( fw,"@%s{%s}" ,tag,s );
}
/**********************************************************************/
void t_Serial :: print_str32( char *tag, t_myChar *s )
{
   long L1,L2 ;
   L1=2*strlen32(s) ;
   uchar *s1 =(uchar *)Calloc( L1+1 ,sizeof(uchar) );
   L2=myChar_to_koi9( s, s1, L1 );
   s1[L2]=0 ;
   fprintf( fw,"@%s{%s}",tag,s1 );
   Free(s1);
}
/**********************************************************************/
void t_Serial :: scanf_int( long *n )
{  long i,i1,j ;
   char Str[100] ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1,j=0 ; i1<L && Mass[i1]!='}' ; i1++ )
      Str[j++]=Mass[i1] ;
   Str[j]=0 ;
   *n=atoi(Str);
   z_Mass=i1+1 ;
}
/**********************************************************************/
void t_Serial :: scanf_hex( long *n )
{  long i,i1,j,m ;
   char Str[100] ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1,j=0 ; i1<L && Mass[i1]!='}' ; i1++ )
      Str[j++]=Mass[i1] ;
   Str[j]=0 ;

   if( '0'!=Str[0] || 'x'!=Str[1] )
   {   *n=-1 ; return ;  }
   for( i=2,m=0 ; i<j ; i++ )
   {  switch( Str[i] )
      {   case '0' : case '1' : case '2' : case '3' : case '4' :
          case '5' : case '6' : case '7' : case '8' : case '9' :
                m=(m<<4)+Str[i]-'0' ; break ;
          case 'a' : case 'b' : case 'c' : case 'd' : case 'e' :  case 'f' :
	        m=(m<<4)+Str[i]-'a'+10 ; break ;
          default: 
	  {  *n=-1 ; return ;  }
      }
   }   
   *n=m;
   z_Mass=i1+1 ;
}
/**********************************************************************/
void t_Serial :: scanf_str( char *str, long Lmax )
{  long i,i1,L1 ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1 ; i1<L ; i1++ )
      if( Mass[i1]=='}' && Mass[i1-1]!='\\' ) break ;

   L1=min( i1-i-1,Lmax );
   strncpy( str,Mass+i+1,L1 );
   str[L1]=0 ;
   z_Mass=i1+1 ;
}
/**********************************************************************/
void t_Serial :: scanf_str32( t_myChar *str, long Lmax )
{  long i,i1,L1 ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1 ; i1<L && Mass[i1]!='}' ; i1++ ) ;

   L1=min( i1-i-1,Lmax );
   koi9_to_myChar( Mass+i+1,str,L1 );
   str[L1]=0 ;
   z_Mass=i1+1 ;
}
/**********************************************************************/
void t_Serial :: scanf1_str( char **_str )
{  long i,i1,j,L1 ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1 ; i1<L ; i1++ )
      if( Mass[i1]=='}' && Mass[i1-1]!='\\' ) break ;

   L1=i1-i-1 ;
   char *str=(char *)Calloc( L1+1,sizeof(char) );
   strncpy( str,Mass+i+1,L1 );
   for( i=j=0 ; str[i]!=0 ; i++ )
   {   if( str[i]=='\\' && str[i+1]=='}' ) continue ;
       str[j++]=str[i] ;
   }
   str[j]=0 ;
   *_str=str ;
   z_Mass=i1+1 ;
}
/**********************************************************************/
void t_Serial :: scanf1_str32( t_myChar **str )
{  long i,i1,L1 ;

   for( i=z_Mass; i<L && Mass[i]!='{' ; i++ ) ;
   for( i1=i+1 ; i1<L && Mass[i1]!='}' ; i1++ ) ;

   L1=i1-i-1 ;
   *str=(t_myChar *)Calloc( L1+1,sizeof(t_myChar) );
   koi9_to_myChar( Mass+i+1,*str,L1 );
   z_Mass=i1+1 ;
}
/**********************************************************************/
char t_Serial :: is_end_block()
{
   for( long i=z_Mass ; i<L ; i++ )
   {  if( Mass[i]=='}' ) return 1 ;
      if( Mass[i]=='@' ) return 0 ;
   }
   return 1 ;
}
/**********************************************************************/
long t_Serial :: find_at( long z )
{
   for( long i=z ; i<L ; i++ )
      if( Mass[i]=='@' ) return i ;
   return L ;
}
/**********************************************************************/
long t_Serial :: find_tag( long z, char *tag )
{
   long L1=strlen( tag );
   for( long i=z ; i<L ; i++ )
      if( Mass[i]=='@' ) 
          if( 0==strncmp( Mass+i,tag,L1 ) ) return i ;
   return L ;
}
/**********************************************************************/
long t_Serial :: find_end( long z )
{
   for( long i=z ; i<L ; i++ )
      if( Mass[i]=='}' ) return i ;
   return L ;
}

/************************************************************************/
DEFINE_LIST_BTYPE( t_wWord    , t_wWordList )
DEFINE_LIST_BTYPE( t_wExemplar, t_wExemplarList )
DEFINE_LIST_BTYPE( t_wType    , t_wTypeList )

