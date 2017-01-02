# include "grammar.h"
# include "grammar1.h"

long read_shift( t_StructList &Part, t_ParamList &Param, t_Lexer &Lexer, 
                 long Begin, t_Shift &Step1 );

/************************************************************************/
//                 Прочесть t_Format4
//  Begin  - слово, с которого начинсется
//  Format - заполняемая структура
/************************************************************************/
long t_Lang :: read_format4( long Begin, t_Format4 *Format )
{  long i,i1,End,End1 ;

   Format->Name=Lexer->Word[Begin+1] ;
   Ifstr( Lexer->Word[Begin+2],"{" )
   {  End = Lexer->find_paar( Begin,"{","}" );
      if( End<0 ) 
      {  fprintf( File_Error,"\n Строка %d Ошибка непарности скобок\n%s %d",
                  Lexer->Str[Begin+1], __FILE__, __LINE__ );
         throw(-1);
      }
   }
   else 
   {  fprintf( File_Error,"\n Строка %d Ошибка формата\n%s %d",
               Lexer->Str[Begin+1], __FILE__, __LINE__ );
      throw(-1);  
   }

   for( i=Begin+3 ; i<End ; i++ )
   {
      Ifstr( Lexer->Word[i],"@Псевдоним" )
      {  // ------- строка переименование параметра -----------
	 t_Rename Ren ;
	 Ren.Reduce= Lexer->Word[i+1] ;
	 Ren.Full  = Lexer->Word[i+2] ;
	 Format->Rename.add( Ren );
	 i+=2 ;
	 continue ;
      }
      Ifstr( Lexer->Word[i],"@Части_речи" )
      {  // ------- строка часть речи -------------------------
         End1 = Lexer->find_paar( i,"{","}" );
	 for( i1=i+2 ; i1<End1 ; i1+=3 )
	 {
	    t_PartDsk PartDsk ;

	    PartDsk.Tag =Lexer->Word[i1] ;
	    PartDsk.Name=Lexer->Word[i1+1] ;
	    if( Lexer->Word[i1+2][0]!=';' )
	    {  fprintf( File_Error,"\n Строка %d Ошибка формата\n%s %d", 
	                Lexer->Str[i1+2], __FILE__, __LINE__ );
	       throw(-1);
	    }
	    Format->Part.add( PartDsk );
	 }
	 i=End1 ;
      }
      Ifstr( Lexer->Word[i],"@Типы_связок" )
      {  End1 = Lexer->find_paar( i,"{","}" );
         for( i1=i+2 ; i1<End1 ; i1++ )
         {  Ifstr( Lexer->Word[i1],"@Связка" )
	    {   t_TypeLink TL ;

		TL.Name  =Lexer->Word[i1+2] ;
        	TL.remark=Lexer->Word[i1+3] ;
		Format->TypeLink.add( TL ) ;
	    }
         }      
      }
   }
   return End ;
}
/************************************************************************/
//                 Прочесть словообразование
//   Begin  - слово, с которого начинсется
/************************************************************************/
long t_Grammar :: read_perevodik( long Begin )
{  long i,End ;

   End=Lexer.find_paar( Begin,"{","}" );
   for( i=Begin ; i<End ; i++ )
   {  Ifstr( Lexer.Word[i],"@Шаг" )
      {  t_Shift S ;
         Step.add( S );
	 i=read_shift( From.Part, From.Param, Lexer, i, Step[Step.j-1] );
      }
      Ifstr( Lexer.Word[i],"@Алгоритм" )
         i=read_algorithm( i );
   }
   return i ;
}
/************************************************************************/
//                  Прочитать половинку шага
//           Begin указывает на слово "@Было" "@Стало"
/************************************************************************/
long  read_shift1( t_StructList &Part, t_ParamList &Param, t_Lexer &Lexer, 
                   long Begin, t_Shift1 &Shift1 )
{  
   long  i,i1,i_struct,End ;
   long  z,z1,z2 ;
   t_Struct *S ;

   End = Lexer.find_paar( Begin,"{","}" );
   
   i_struct=find_part( &Part, Lexer.Word[Begin+2] );
   if( i_struct<0 )
   {  fprintf( File_Error,"\n Строка %d Ошибка типа структуры\n%s %d",
               Lexer.Str[Begin+2], Lexer.Word[Begin+2], __FILE__, __LINE__ );
      throw(-1);
   }
   Shift1.i_struct=i_struct ;

   S=&Part[i_struct] ;

   for( i=Begin+3 ; i<End ; i++ )
   {
      Ifstr( Lexer.Word[i],"}" ) break ;

      for( i1=0 ; i1<S->Param.j ; i1++ )
	 if( 0==strcmp(S->Param[i1].Name,Lexer.Word[i]) )
	 {  z1=i1 ; z=S->Param[i1].param ; goto M_Ok ; }
      fprintf( File_Error,"\n Строка %d Часть речи (%s) не имеет параметра(%s)\n%s %d",
	       Lexer.Str[i], S->Name, Lexer.Word[i], __FILE__, __LINE__ );
      Read_Error=-1 ;
      M_Ok:;

      Ifstr( Lexer.Word[i+1],"=" )
      {  // -------------- найти значение параметра -------------
	 z2=find_value( &Param[z].Value,Lexer.Word[i+2] );
	 if( z2<0 )
	 {  fprintf( File_Error,"\n Строка %d Параметр(%s) не имеет значения(%s)\n%s %d",
		     Lexer.Str[i+2], Param[z].Name, Lexer.Word[i+2], __FILE__, __LINE__ );
	    throw(-1);
	 }
	 Shift1.Form.value[z1]=z2 ;
	 i+=2 ;
      }
   }
   return End ;
}
/************************************************************************/
//               Прочесть запись типа @Шаг
//               Begin указывает на слово "@Шаг"
/************************************************************************/
long read_shift( t_StructList &Part, t_ParamList &Param, t_Lexer &Lexer, 
                 long Begin, t_Shift &Step1 )
{  long  i,z,End,L1,L2 ;
   char *s1,*s2 ;
   char  f1,f2 ;

   End=Lexer.find_paar( Begin,"{","}" );
   Step1.Name=Grammar.Lexer.Word[Begin+2] ;

   z=Begin ;
   s1=Lexer.Word[Begin+5] ;
   s2=Lexer.Word[Begin+6] ;
   Ifstr( Lexer.Word[Begin+3],"@Приставка" )
   {
      Step1.From.Prefix=s1 ;
      Step1.To.Prefix  =s2 ;
      if( s1[0]=='-' ) Step1.From.Prefix="" ;
      if( s2[0]=='-' ) Step1.To.Prefix="" ;
      z=Begin+7 ;
   }
   Ifstr( Lexer.Word[Begin+3],"@Суффикс" )
   {
      Step1.From.Suffix=s1 ;
      Step1.To.Suffix  =s2 ;
      if( s1[0]=='-' ) Step1.From.Suffix="" ;
      if( s2[0]=='-' ) Step1.To.Suffix="" ;
      z=Begin+7 ;
   }

   s1=Lexer.Word[Begin+ 9] ;
   s2=Lexer.Word[Begin+10] ;
   Ifstr( Lexer.Word[Begin+7],"@Приставка" )
   {
      Step1.From.Prefix=s1 ;
      Step1.To.Prefix  =s2 ;
      if( s1[0]=='-' ) Step1.From.Prefix="" ;
      if( s2[0]=='-' ) Step1.To.Prefix="" ;
      z=Begin+11 ;
   }
   Ifstr( Lexer.Word[Begin+7],"@Суффикс" )
   {
      Step1.From.Suffix=s1 ;
      Step1.To.Suffix  =s2 ;
      if( s1[0]=='-' ) Step1.From.Suffix="" ;
      if( s2[0]=='-' ) Step1.To.Suffix="" ;
      z=Begin+11 ;
   }
   Ifstr( Lexer.Word[z],"@Было" )
   {  z=read_shift1( Part, Param, Lexer, z, Step1.From );
      z++ ;
   }
   Ifstr( Lexer.Word[z],"@Стало" )
   {  z=read_shift1( Part, Param, Lexer, z, Step1.To );
   }
   Ifstr( Lexer.Word[z+1],"@Переводик" )
   { 
      t_semLink  SL ;

      for( i=z+3 ; i<End ; i++ )
      {  if( Lexer.Word[i][0]=='}' )
             break ;
         SL.Name=Lexer.Word[i] ;
	 Step1.sLink.add( SL );
      }
      z=i ;
   }   
   Ifstr( Lexer.Word[z+1],"@Перевод" )
   {  z=Step1.Trans.From.read( z,&Grammar.from() );
      z=Step1.Trans.To.read  ( z,&Grammar.to() );
   }
   Ifstr( Lexer.Word[z+1],"[" )
   {
      for( i=z+1 ; i<End ; i++ )
      {  if( Lexer.Word[i][0]=='[' ) continue ;
	 if( Lexer.Word[i][0]==']' || Lexer.Word[i][0]=='}' )
	     break ;
	 Step1.str.add( Lexer.Word[i] );
      }   
   }
   return End ;
}
/************************************************************************/
//                     Прочитать алгоритмы
//       Begin указывает на "@Алгоритм"
/************************************************************************/
long t_Grammar :: read_algorithm( long Begin )
{  
   long  i,i1,End ;
   char *s1 ;
   t_Algorithm Al,*Al1 ;

   End = Lexer.find_paar( Begin,"{","}" );

   Algorithm.add( Al );
   Al1=&Algorithm[Algorithm.j-1] ;
   Al1->Name=Lexer.Word[Begin+1] ;
   
   // ---- прочесть очередной Шаг алгоритма ----   
   for( i=Begin+3 ; i<End ; i+=5 )
      read_algorithm1( Al1, i );         

   // -------- установить ссылки ---------------
   for( i=0 ; i<Al1->Step.j ; i++ )
   {
      // -------- установить ссылки Up -------------
      if( 0!=Al1->Step[i].up )
      {	 for( i1=0 ; i1<Al1->Step.j ; i1++ )
	 {  if( Al1->Step[i].up==Al1->Step[i1].label )
	    {   Al1->Step[i].up=i1 ; goto M_OK ;  }
	 }
	 fprintf( File_Error,"\n Ошибочная ссылка вверх\n%s %d", __FILE__,__LINE__ );
	 throw(-1);
      }
      else
      {  Al1->Step[i].up=-1 ;
      }
      
      M_OK:;
      s1=Al1->Step[i].Name ;
      // ------ поставить ссылку на Шаг -----------
      for( i1=0 ; i1<Step.j ; i1++ )
	 if( 0==strcmp( s1,Step[i1].Name ) )
	 {   //Al1->Step[i].type=0 ; 
	     Al1->Step[i].i_step=i1 ; 
	     goto M_OK1 ;  
	 }
      // ------ поставить ссылку на Алгоритм -----------
      for( i1=0 ; i1<Algorithm.j ; i1++ )
	 if( 0==strcmp( s1,Algorithm[i1].Name ) )
	 {   Al1->Step[i].type=1 ; 
	     Al1->Step[i].i_step=i1 ;
	     goto M_OK1 ;  
	 }
      fprintf( File_Error,"\n Ошибочное имя шага %s\n%s %d",s1, __FILE__,__LINE__ );
      throw(-1);
      M_OK1:;
   }
   return End ;
}
/************************************************************************/
//        прочесть один шаг алгоритма и внести его в алгоритм
/************************************************************************/
long t_Grammar :: read_algorithm1( t_Algorithm *Al, long Begin )
{
    t_Step1 Step1 ;
    Step1.label=atoi( Lexer.Word[Begin] );
    Step1.up   =atoi( Lexer.Word[Begin+1] );
    Step1.i_step=-1 ;
    Step1.Name=Lexer.Word[Begin+2] ;
    Ifstr( Lexer.Word[Begin+3],"@Синт" )
       Step1.type=0 ;
    Ifstr( Lexer.Word[Begin+3],"@Анал" )
       Step1.type=2 ;
    if( Lexer.Word[Begin+4][0]!=';' )
    {  fprintf( File_Error,"\n Строка %d Точка с запятой отсутствует\n%s %d",
                Lexer.Str[Begin+4], __FILE__,__LINE__ );
       throw(-1);
    }
    Al->Step.add( Step1 );
    return Begin+5 ;
}
/************************************************************************/
t_Shift1 :: t_Shift1()
{  init();  }
/************************************************************************/
void t_Shift1 :: init()
{
   Prefix="" ;
   Suffix="" ;
   i_struct=-1 ;
   Form.init() ;
}
/************************************************************************/
t_semLink :: t_semLink()
{  init();  }
/************************************************************************/
void t_semLink :: init()
{  type_link=-1 ;
   Name="" ;
}
/************************************************************************/
t_Step1 :: t_Step1()
{  init();  }
/************************************************************************/
void  t_Step1 :: init()
{
   type  =-1 ;
   label =-1 ;
   up    =-1 ;
   i_step=-1 ;
   Name  ="" ;
}
/************************************************************************/
t_Algorithm :: t_Algorithm()
{  init();  }
/************************************************************************/
t_Algorithm :: ~t_Algorithm()
{  del();  }
/************************************************************************/
void t_Algorithm :: init()
{
   Name="" ;
   Step.init();
}
/************************************************************************/
void t_Algorithm :: del()
{
   Step.del();
}
/************************************************************************/
DEFINE_LIST_BTYPE( t_semLink,   t_semLinkList )
DEFINE_LIST_BTYPE( t_Step1,     t_Step1List )
DEFINE_LIST_TYPE ( t_Algorithm, t_AlgorithmList )
