/**************************************************************************/
//              Построение догадок по родственным словам
//
/**************************************************************************/
# include <string.h>
# include "grammar.h"
# include "semantika.h"
# include "core.h"

extern  t_Slowo3     Dogadka ;   // словарь догадок
extern  t_Semantika  Semantika ; // словарь смысловых связей (приемника)

/**************************************************************************/
//           Догадаться об одном слове по одному алгоритму
//
/**************************************************************************/
/**************************************************************************/
//     Построить дерево догадок о значении слова
//     Str0 - исходное слово
/**************************************************************************/
void t_Core1 :: make_dogadka( char *_Str0, t_longList &_Antwort )
{
   Str0=_Str0 ;
   Tree.j=0 ;
   Antwort.j=0 ;
   make_dogadka( Str0, &Grammar.Algorithm[Grammar.i_Algorithm], -1 );
//   Dogadka.make_tree();
   _Antwort=Antwort ;
}
/**************************************************************************/
//     Построить дерево догадок о значении слова
//     Str0 - исходное слово
//     A    - алгоритм
//     z_Tree - стартовая вершина дерева
/**************************************************************************/
void t_Core1 :: make_dogadka( char *_Str0, t_Algorithm *A, long z_Tree )
{  
   long    i,i1,i2,N1,i_Tree ;   
   short   i_struct ;
   t_mTree T ;   
   char    Str1[100] ;
   t_longList work_index ;
   
   Str0=_Str0 ;
   work_index.Calloc( A->Step.j );

   for( i=0 ; i<A->Step.j ; i++ )
   {  t_Step1 *St=&A->Step[i] ;
   
      if( St->up<0 )
           i_Tree=z_Tree ;
      else i_Tree=work_index[St->up] ;
   
      if( 1==St->type )
      {   // ----- очередной шаг - алгоритм ------------
          t_Algorithm *A1=&Grammar.Algorithm[St->i_step] ;
	  if( i_Tree<0 ) make_dogadka( Str0            , A1, i_Tree );
              else       make_dogadka( Tree[i_Tree].Str, A1, i_Tree );
      }
      else
      {   // ----- очередной шаг - обычный -------------
          t_Shift *Shift=&Grammar.Step[St->i_step] ;
          if( i_Tree<0 ) make_step( Str0            , Str1, Shift, 0 );
              else       make_step( Tree[i_Tree].Str, Str1, Shift, 0 );

          if( 0==Str1[0] )
	      continue ;
	  
	  i_struct=Shift->To.i_struct ;
	  //i_struct=Shift->From.i_struct ;
	  
	  T.up       =i_Tree ;
	  T.type     =St->type ;
	  T.type_link=St->i_step ;
	  strcpy( T.Str, Str1 );
	  work_index[i]=Tree.j ;
	  Tree.add( T );
	  
          t_longList  Ant2 ;
          Perevod->find( Str1, i_struct, &Ant2 );

          Tree[Tree.j-1].i_dogadka=Dogadka.n_record();
	  for( i1=0 ; i1<Ant2.j ; i1++ )
	  {  
	     N1=Perevod->get_n_perevod( Ant2[i1] );
	     for( i2=0 ; i2<N1 ; i2++ )
             {  
	        t_sStruct *S=Perevod->get_to( Ant2[i1], i2 );
        	long i_semantika=Semantika.find_word( i_struct,S->Word[0].str );
		if( 0<=i_semantika )
		    reverse_path( Tree.j-1, i_semantika, Shift->From.i_struct );
             }
          }
          Tree[Tree.j-1].i_last_dogadka=Dogadka.n_record();
      }
   }
}
/**************************************************************************/
//      Проход по дереву догадок в обратном направлении (аналитический)
//      Tree   - само дерево
//      z_Tree - вершина дерева, от которой надо идти
//      i_semantika - слово, от которого надо идти
//      i_struct    - структура, которой должно быть переводимое слово
/**************************************************************************/
void t_Core1 :: reverse_path( long z_Tree, long i_semantika, short i_struct )
{
   long   i,i1,i2,i_Tree ;
   short  i_main,i_down ;
   t_Struct1List Var,Var1 ;
   t_longList    Ant ;
   t_Struct1     V ;

   i_Tree=z_Tree ;

   mk_struct( V,i_semantika );
   Var.add( V ) ;

   while( 1 )
   {
      t_Shift *Shift=&Grammar.Step[Tree[i_Tree].type_link] ;
      if( Tree[i_Tree].type==0 )
      {  // ----- синтетический шаг ------------------
         Var1.j=0 ;
	 
         for( i=0 ; i<Var.j ; i++ )
         {  i_main=grammar_use( Var[i] );
            for( i1=0 ; i1<Shift->sLink.j ; i1++ )
	    {  Semantika.link_word( Var[i].i_semantika, Shift->sLink[i1].type_link, Ant );
	       for( i2=0 ; i2<Ant.j ; i2++ )
	       {  V=Var[i] ;
        	  V.i_struct_from=Shift->i_struct_from ;
        	  V.i_struct_to  =Shift->i_struct_to ;
		  set_word_semantika( V, i_main, Ant[i2] );
		  Var1.add( V );
               }
	    }
	 }
      }
      else
      {  // ----- аналитический шаг ------------------
         Var1.j=0 ;
	 i_down=meaning_use( Shift->Trans.To );
	 i_main=grammar_use( Shift->Trans.To );
         for( i=0 ; i<Var.j ; i++ )
         {  t_Word &W=Shift->Trans.To.Word[i_main] ;

            V.i_struct_from=Shift->i_struct_from ;
            V.i_struct_to  =Shift->i_struct_to ;
	    insert( V, Shift->Trans.To, Var[i], i_down );
	    V.i_semantika=Semantika.find_word( W.i_struct, W.literal );
	    Var1.add( V );
	 }
      }
      Var=Var1 ;
      i_Tree=Tree[i_Tree].up ;
      if( i_Tree==-1 )
          break ;
   }
   t_String STmp,tag1,tag2 ;

   for( i=0 ; i<Var.j ; i++ )
   {
      tag1=Dogadka.format()->get_tag( 0, i_struct ) ;
      tag2=Dogadka.format()->get_tag( 0, Var[i].i_struct_from ) ;

      if( 1<Var[i].Word.j )
	       STmp=tag2+":"+tag1+"["+Str0+"]=" ;
          else STmp=         tag1+"["+Str0+"]=" ;
      struct_print( STmp, Var[i], i_struct );
      // long z=Dogadka.add_new1( (char *)STmp.c_str(), 1 ); // Нету сейчас такой функции
      // Antwort.add( z );
   }
}
/**************************************************************************/
//         Сформировать исходное слово
//         Str  - исходная строка
//         Str1 - результирующая строка
//         Step - тип шага
//         Flag - 0-прямое 1-обратное преобразование
/**************************************************************************/
void t_Core1 :: make_step( char *Str, char *Str1, t_Shift *Shift, char Flag )
{
   long  L,Lsuff1,Lpref1,Lsuff2,Lpref2 ;
   t_Shift1 *P1,*P2 ;

   if( 0==Flag )
   {   P1=&Shift->From ;
       P2=&Shift->To ;
   }else
   {   P2=&Shift->From ;
       P1=&Shift->To ;
   }
   Str1[0]=0 ;
   
   Lpref1 = strlen(P1->Prefix);
   Lsuff1 = strlen(P1->Suffix);
   Lpref2 = strlen(P2->Prefix);
   Lsuff2 = strlen(P2->Suffix);

   // ----- проверить возможен ли такой шаг ------------
   if( 0!=strncmp( Str, P1->Prefix, Lpref1 ) ) return ;
   if( 0!=strcmp(  Str+strlen(Str)-Lsuff1, P1->Suffix ) ) return ;

   strcpy( Str1, P2->Prefix );
   strcat( Str1, Str+Lpref1 );
   L=Lpref2-Lpref1+strlen(Str)-Lsuff1 ;
   Str1[L]=0 ;
   strcat( Str1, P2->Suffix );
}
/**************************************************************************/
//            Вставка структуру в другую структуру
/**************************************************************************/
void t_Core1 :: insert( t_Struct1 &Result, t_Struct &Up, t_Struct1 &Down, long i_up )
{  long i,i1 ;

   // ----- слова скомпоновать --------------
   Result.del();
   for( i=0 ; i<Up.Word.j ; i++ )
   {  if( i==i_up )
      {  for( i1=0 ; i1<Down.Word.j ; i1++ )
            Result.Word.add( Down.Word[i1] );
         // ---- переименовать параметры вставляемой структуры ----
         for( i1=0 ; i1<Down.Relation.j ; i1++ )
	 {  t_Relation &R=Down.Relation[i1] ;
	    if( R.s1==0 && R.s2!=0 )
	        strcpy( Result.Word[i+R.s2-1].Param[R.p2].Name, Up.Word[i_up].Param[R.p1].Name );
	    if( R.s2==0 && R.s1!=0 )
	        strcpy( Result.Word[i+R.s1-1].Param[R.p1].Name, Up.Word[i_up].Param[R.p2].Name );
	 }
      }
      else
      {  Result.Word.add( Up.Word[i] );
      }
   }
   // ----- передача параметров -------------
   Result.Param=Grammar[Result.i_struct_to].To.Param ;
   Result.trans_relation();
   
   for( i=0 ; i<Result.Relation.j ; i++ )
   {  t_Relation R=Result.Relation[i] ;
      if( 0==R.s2 )
          Result.Param[R.p2].value=Result.Word[R.s1].Param[R.p1].value ;
   }
}
/**************************************************************************/
//                 Сформировать слово из семантики
/**************************************************************************/
void t_Core1 :: mk_struct( t_Struct1 &Struct, long i_semantika )
{
   short i ;
   t_mWord  &W=Semantika.Word[i_semantika] ;
   t_Struct &S=Grammar[W.i_struct].To ;
   t_Word   Word ;
   
   Struct.Word.del();
   Struct.Relation.del();
   
   // ----- сформировать слово -------------
   Word.type    =TWORD ;
   Word.Param   =S.Param ;
   Word.literal =W.str ;
   Word.i_struct=W.i_struct ;
   Word.grammar_use=1 ;
   Word.meaning_use=1 ;

   for( i=0 ; i<Word.Param.j ; i++ )
      if( Word.Param[i].Dir==1 )
      {   Word.Param[i].value=W.Param.value[i] ;
          strcpy( Word.Param[i].Name, S.Param[i].Name );
      }

   // ----- сформировать заголовок структуры -------------
   Struct.type   =TSTRUCT ;
   Struct.Param  =S.Param ;
   Struct.i_semantika  =i_semantika ;
   Struct.i_struct_from=W.i_struct ;
   Struct.i_struct_to  =W.i_struct ;
   Struct.Word.add( Word );

   // ----- сформировать передачу параметров -------------
   for( i=0 ; i<Struct.Param.j ; i++ )
   {  t_Relation R ;

      R.p1=i ; R.p2=i ;
      Struct.Param[i].Dir=!Word.Param[i].Dir ;
      if( Word.Param[i].Dir==1 )
      {   R.s1=1 ; R.s2=0 ;  }
      else
      {   R.s2=1 ; R.s1=0 ;  }
      Struct.Relation.add( R );
   }
   // ----- задать параметры заголовку структуры --------
   for( i=0 ; i<Struct.Param.j ; i++ )
      Struct.Param[i].value=Word.Param[i].value ;
}
/**************************************************************************/
//                сделать синтетический шаг для структуры
/**************************************************************************/
void t_Core1 :: set_word_semantika( t_Struct1 &S1, short i_main, long i_semantika )
{
   S1.i_semantika=i_semantika ;
   S1.Word[i_main].i_struct=Semantika.Word[i_semantika].i_struct ;
   S1.Word[i_main].literal =Semantika.Word[i_semantika].str ;
}
/**************************************************************************/
//                    главное грамматическое слово
/**************************************************************************/
short t_Core1 :: grammar_use( t_Struct &S1 )
{
   for( short i=0 ; i<S1.Word.j ; i++ )
      if( S1.Word[i].grammar_use!=0 )
          return i ;
   return -1 ;
}
/**************************************************************************/
//                      главное смысловое слово
/**************************************************************************/
short t_Core1 :: meaning_use( t_Struct &S1 )
{
   for( short i=0 ; i<S1.Word.j ; i++ )
      if( S1.Word[i].meaning_use!=0 )
          return i ;
   return -1 ;
}
/**************************************************************************/
//                      сравнение структур
/**************************************************************************/
char t_Core1 :: struct_cmp( t_Struct &S1, t_Struct1 &S2 )
{
   if( S1.Word.j != S2.Word.j ) 
      return -1 ;
   for( short i=0 ; i<S1.Word.j ; i++ )
      if( S1.Word[i].i_struct!=S2.Word[i].i_struct )
          return -1 ;
   return 0 ;
}
/**************************************************************************/
//           напечатать (стандартную) структуру для словаря 
//    STmp - строка в которую печатают (к моменту функции уже частично заполнена)
/**************************************************************************/
void t_Core1 :: struct_print( t_String &STmp, t_Struct1 &S, short i_struct0 )
{  long  i,i1,i_struct,i1_semantika ;
   short i_struct1 ;
   char  STmp1[100],STmp2[100] ; // сделать это и STmp как string

   if( 1<S.Word.j )
   {  // ------ заголовок структуры -------------------
      for( i=0 ; i<Grammar.n_trans() ; i++ )
      {  if( Grammar[i].To.type==TSTRUCT1 )
             if( 0==struct_cmp( Grammar[i].To,S ) )
	     {   i_struct1=i ; goto M_Ok1 ; }
      }
      i_struct1=S.i_struct_to ;
   M_Ok1 :
      char *tag2=Dogadka.format()->get_tag( 1, i_struct1 );
      // параметры структуры не печатаются
      //Semantika.print_param( STmp2, &Semantika.Word[S.i_semantika], 1 );
      sprintf( STmp1,"%s:",tag2 );
      STmp+=STmp1 ;
   }
   // --------- составляющие структуры ----------------
   for( i=0 ; i<S.Word.j ; i++ )
   {
      i_struct=S.Word[i].i_struct ;
      char *tag2=Dogadka.format()->get_tag( 1, i_struct );
      char *Str2=S.Word[i].literal ;
      sprintf( STmp1,"%s[%s] ",tag2,Str2 );
      STmp+=STmp1 ;
      if( S.Word[i].meaning_use!=0 )
      {   i1_semantika=Semantika.find_word( S.Word[i].i_struct, S.Word[i].literal );
          Semantika.print_param( STmp2, &Semantika.Word[i1_semantika], 1 );
          STmp+=STmp2 ;
      }
   }
   if( S.Word.j==1 )
   {  // Semantika.print_param сделать через string
      Semantika.print_param( STmp1, &Semantika.Word[S.i_semantika], 1 );
      STmp+=STmp1 ;
   }
}
/**************************************************************************/
t_mTree :: t_mTree()
{  init();  }
/**************************************************************************/
void t_mTree :: init( void )
{
   up=-1 ;
   type=0 ;
   Str[0]=0 ;
   i_dogadka=0 ;
   i_last_dogadka=0 ;
}
/**************************************************************************/
DEFINE_LIST_BTYPE( t_mTree,   t_mTreeList )
DEFINE_LIST_TYPE ( t_Struct1, t_Struct1List )
