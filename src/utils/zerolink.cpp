/**************************************************************************/
//         Найти все конструкции, на которые нет ссылок
//
/**************************************************************************/
# include <stdlib.h>
# include <grammar.h>

int main( int argc, char **argv )
{  short i,i1,j,z,*m,*n ;
   t_Struct *S ;

   if( argc<2 )
   {   printf("\nЗадайте файл описания грамматики вот так:\nzerolink <../grammar/lang.txt>");
       exit(-1);
   }
   if( 0>Grammar.make_all( argv[1] ) )
   {   printf("\nГрамматика не прочитана\n");
       exit(-1);
   }
   m=(short *)calloc( Grammar.n_trans(),sizeof(short) );
   n=(short *)calloc( Grammar.n_trans(),sizeof(short) );
   // ------------------------------------------------------
   m[0]=Grammar.i_all() ; n[0]=1 ;
   for( i=0,j=1 ; i<j ; i++ )
   {  S=&Grammar[m[i]].From ;
      for( i1=0 ; i1<S->Word.j ; i1++ )
      {  z=S->Word[i1].i_struct ;
         if( 0<=z && n[z]==0 )
	 {  m[j++]=z ; n[z]=1 ; }
      }
   }
   printf("\n // ------ Источник ------");
   for( i=0 ; i<Grammar.n_trans() ; i++ )
   {  if( n[i]==0 )
          printf("\n %s",Grammar[i].From.Name );
   }
   
   for( i=0 ; i<Grammar.n_trans() ; i++ )
      m[i]=n[i]=0 ;
   // ------------------------------------------------------
   m[0]=Grammar.i_all() ; n[0]=1 ;
   for( i=0,j=1 ; i<j ; i++ )
   {  S=&Grammar[m[i]].To ;
      for( i1=0 ; i1<S->Word.j ; i1++ )
      {  z=S->Word[i1].i_struct ;
         if( 0<=z && n[z]==0 )
	 {  m[j++]=z ; n[z]=1 ; }
      }
   }
   printf("\n // ------ Приемник ------");
   for( i=0 ; i<Grammar.n_trans() ; i++ )
   {  if( n[i]==0 )
          printf("\n %s",Grammar[i].To.Name );
   }
      
   return 0 ;
}
