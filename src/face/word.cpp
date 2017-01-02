/*******************************************************************/
//           "Меркурий"-"Правда" - open source переводчик
//       распространяется в соответсвии с лицензией GNU v 2.0
//
//             Ввод новых слов и выражений (по-новому)
//      Анисимов Д.В.                               сегодня
/*******************************************************************/

# include <malloc.h>
# include <string.h>
# include "mylib.h"
# include "grammar.h"
# include "koi9.h"
# include "video.h"
# include "common.h"
# include "option.h"
# include "menu.h"
# include "slowo2.h"
# include "slowo3.h"
# include "word.h"
# include "windows.h"
# include "window.h"

/* =============================================================
Дальнейшее движение
   1) Undo-Redo
============================================================= */

void  word_loop( void *V );

# ifdef NIE
t_Menu Wizard_menu1[] = // редактор
{
   { F_FUNK  ,0,Sorry, NULL       ,"Отмена" },
   { F_FUNK  ,0,Sorry, NULL       ,"Примена" },
   { F_NUMER ,0,NULL, (void *)"w" ,"Выход с запоминаеием Enter" },
   { F_NUMER ,0,NULL, (void *)"x" ,"Выход с отменой      Esc  " },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_menu2[] = // выражение
{
   { F_FUNK  ,0,word_loop, (void *)"2"  ,"Обструктурить   F2" },
   { F_FUNK  ,0,word_loop, (void *)"3"  ,"Расструктурить  F3" },
   { F_FUNK  ,0,word_loop, (void *)"6"  ,"Cтруктуры1      F6" },
   { F_FUNK  ,0,word_loop, (void *)"7"  ,"Cтруктуры2      F7" },
   { F_FUNK  ,0,word_loop, (void *)"9"  ,"Дублировать ^S Ins" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_menu3[] = // слово
{
   { F_FUNK  ,0,word_loop, (void *)"4"  ,"Части речи      F4" },
   { F_FUNK  ,0,word_loop, (void *)"5"  ,"Части речи все  F5" },
   { F_FUNK  ,0,word_loop, (void *)"8"  ,"Параметры       F8" },
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_help[] = // подсказка
{
   { F_FUNK  ,0,Window_help ,(void *)"index.html "       ,"Содержание    "},
   { F_FUNK  ,0,Window_help ,(void *)"k_konstruktor.html","Об этом режиме"},
   { F_ENDE  ,0,NULL,NULL         ,"" }
};
t_Menu Wizard_menu[] =
{
   { F_MENU  ,0,NULL ,Wizard_menu1, "Редактор"},
   { F_MENU  ,0,NULL ,Wizard_menu2, "Выражение "},
   { F_MENU  ,0,NULL ,Wizard_menu3, "Слово" },
   { F_MENU  ,0,NULL ,Wizard_help , "Подсказка"},
   { F_ENDE  ,0,NULL ,NULL        ,"" }
};
# endif

extern t_Slowo3    *Perevod ;
extern t_Slowo3     Dogadka ;
extern t_Slowo2    *SlowoF,*SlowoT ;
extern t_SetPerevod  SetPerevod ;

/******************************************************************/
void mouse_message1( )
{  char *Str=
   "Этот режим намеренно оставлен не  мышефицированным.\n"
   "                  МЫШЬ ЭТО ЗЛО.                    \n"
   "Стандартный  компьютерный  интерфейс  рассчитан  на\n"
   "обыкновенного человека с четырьмя руками - две руки\n"
   "для обычной клавиатуры,  одной  (третей)  руки  для\n"
   "спецклавиш - стрелок, Home, End и т.п. и одной руки\n"
   "(четвертой) для мышки. Если у Вас нет четырех  рук,\n"
   "то учитесь работать без  мышки.  Тем более,  что  в\n"
   "этом режиме Вам надо сформировать словарную статью,\n"
   "по-этому  нет  никакого  смысла  отрывать  руки  от\n"
   "клавиатуры.                                        \n" ;

   figna( Str );
}
/******************************************************************/
t_WordWizard :: t_WordWizard() : t_Win()
{
   ErrStr="" ;
}
/******************************************************************/
void t_WordWizard :: set_pos( t_Rect &R )
{   set_pos( R.x1, R.y1, R.x2, R.y2 );
}
/******************************************************************/
void t_WordWizard :: set_pos( short _x1, short _y1, short _x2, short _y2 )
{
    t_Win::set_pos( _x1, _y1, _x2, _y2 );
    EditWord.set_pos( x1,y1,x2 );
}
/******************************************************************/
void t_WordWizard :: paint(  )
{
   //s_nacht();

   s_rame1_F( EditWord.y1+1,EditWord.x1,EditWord.y1+3,EditWord.x2, 0x07 );
   if( 1==f_2Rame )
            s_rame2_F( EditWord.y1-1,EditWord.x1,EditWord.y1+1,EditWord.x2, 0x07 );
       else s_rame1_F( EditWord.y1-1,EditWord.x1,EditWord.y1+1,EditWord.x2, 0x07 );
   EditWord.paint();
   s_text_yxf( EditWord.y1+2,EditWord.x1+1,0xce,ErrStr );
   s_text_yxf( y1,x1+3 ,titl_Color, Name );
   //paint_hotkey( n_Line-1, 0, "[F1]Подсказка[F2]Обструктурить[F3]Расструктурить[F4]Слово[F5]Слово все[F6]Struct1[F7]Struct2[F8]Param[F9]Меню" );
   //paint_header( "Конструктор словарной статьи" );
   //null_menu( Wizard_menu,y1 );
}
/******************************************************************/
e_WinMsg t_WordWizard :: main_loop(  )
{
   ushort ZZ,key1,key2,k ;
   e_WinMsg  r ;
   e_sEvent  Event ;

   ErrStr="" ;
   EditWord.Cursor=0 ;
   while( 1 )
   {
      paint( );

      Event=s_getch16( &key1, &key2 );
      ZZ=s_shiftstatus();
      if( MOUSE_PRESS==Event )
          mouse_message1( );

      r=loop( ZZ, key1, key2 );
      if( r==WM_ESC || r==WM_OK ) return r ;
   }
}
/******************************************************************/
e_WinMsg t_WordWizard :: loop( short ZZ, ushort key1, ushort key2 )
{
   short  r1 ;
   char   Str2[100] ;
   t_sRecord1 Rec ;

   switch( key1 )
   {
       case S_key_Esc: return WM_ESC ;
       case '\r'     :
	    myChar_to_koi9( EditWord.Str,(uchar *)Str2,100,100 );
	    ErrStr=Rec.parse2( Str2,&Grammar.format1() );
	    if( ErrStr[0]!=0 ) break ;
 	    return WM_OK ;
   }

   if( 0==(ZZ&(S_Shift_L|S_Shift_R)) )
   {   switch( key2 )
       {
	   case S_key_F1: Window_help( (void *)"k_konstruktor.html");break ;
	   case S_key_F2: make_struct();       break ;
	   case S_key_F3: EditWord.to_plain(); break ;
	   case S_key_F4: part( );             break ;
	   case S_key_F5: part_all( );         break ;
	   case S_key_F6: struct1( );          break ;
	   case S_key_F7: struct2( );          break ;
	   case S_key_F8: param();             break ;
/*
	   case S_key_F9: 
        	r1=menu( Wizard_menu,y1-1 );
        	if( 'x'==r1 ) return WM_ESC ;
        	if( 'w'==r1 ) return WM_OK ;
        	break ;
*/
	   case S_key_F10:
		myChar_to_koi9( EditWord.Str,(uchar *)Str2,100,100 );
		ErrStr=Rec.parse2( Str2,&Grammar.format1() );
		if( ErrStr[0]!=0 ) break ;
 		return WM_OK ;
	   case 0: case S_key_Left: case S_key_Right:
        	   case S_key_Del : case S_key_Ins  :
		   case S_key_Home: case S_key_End  :
        	EditWord.loop( ZZ, key1, key2 );    
        	break ;
       }
   }
   else
   {   switch( key2 )
       {   case S_key_Ins :
        	EditWord.double_str( );
		break ;
	   case 0:// case S_key_Left: case S_key_Right:
        	  // case S_key_Del : case S_key_Ins  :
        	EditWord.loop( ZZ, key1, key2 );    
        	break ;
       }
   } 
   return WM_NULL1 ;
}
/******************************************************************/
void  word_loop( void *V )
{  char *Func=(char *)V ;

   switch( Func[0] )
   {
      case '1' : SetPerevod.WordWizard.loop( 0,0,S_key_F1 ); break ;
      case '2' : SetPerevod.WordWizard.loop( 0,0,S_key_F2 ); break ;
      case '3' : SetPerevod.WordWizard.loop( 0,0,S_key_F3 ); break ;
      case '4' : SetPerevod.WordWizard.loop( 0,0,S_key_F4 ); break ;
      case '5' : SetPerevod.WordWizard.loop( 0,0,S_key_F5 ); break ;
      case '6' : SetPerevod.WordWizard.loop( 0,0,S_key_F6 ); break ;
      case '7' : SetPerevod.WordWizard.loop( 0,0,S_key_F7 ); break ;
      case '8' : SetPerevod.WordWizard.loop( 0,0,S_key_F8 ); break ;
      case '9' : SetPerevod.WordWizard.loop( S_Shift_L,0,S_key_Ins ); break ;
      case 'a' : SetPerevod.WordWizard.loop( 0,0,S_key_F10); break ;
   }   
}
/******************************************************************/
//        вставить в строку тег структуры (TSTRUCT1)
/******************************************************************/
void t_WordWizard :: struct1( )
{  short  i ;
   t_View S ;
   t_String Ant ;

   // -------- сформировать список структур -----------
   char fTo=in_dst( );
   Select.clr();
   if( fTo==0 )
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].From.type==TSTRUCT1 )
          {   S.index=i ;
	      word_of_struct( fTo, i, Ant );
	      strcpy( S.Str, Ant.c_str() );
              Select.add( S );
          }
   }
   else
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].To.type==TSTRUCT1 )
          {   S.index=i ;
	      word_of_struct( fTo, i, Ant );
	      strcpy( S.Str, Ant.c_str() );
              Select.add( S );
          }
   }
   Select.calc_pos();
   i=Select.loop();
   if( i<0 ) return ;
   // --------- вставить тег структуры ---------------
   t_Format1  *FF=&Grammar.format1() ;
   EditWord.insert_struct( fTo,FF->get_tag( TO,Select.ViewList[i].index ) );
}
/******************************************************************/
//        вставить в строку тег структуры (TSTRUCT2)
/******************************************************************/
void t_WordWizard :: struct2( )
{  short  i ;
   t_View S ;
   t_String Ant ;

   // -------- сформировать список структур -----------
   char fTo=in_dst( );
   Select.clr() ;
   if( fTo==0 )
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].From.type==TSTRUCT2 )
          {   S.index=i ;
	      word_of_struct( fTo, i, Ant );
	      strcpy( S.Str, Ant.c_str() );
              Select.add( S );
          }
   }
   else
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].To.type==TSTRUCT2 )
          {   S.index=i ;
	      word_of_struct( fTo, i, Ant );
	      strcpy( S.Str, Ant.c_str() );
              Select.add( S );
          }
   }
   Select.calc_pos();
   i=Select.loop();
   if( i<0 ) return ;
   // --------- вставить тег структуры ---------------
   t_Format1  *FF=&Grammar.format1() ;
   EditWord.insert_struct( fTo,FF->get_tag( TO,Select.ViewList[i].index ) );
}
/******************************************************************/
//    cформировать строку вида
//    S1 гр_сущ1  S1:a[прил]s[сущ]
/******************************************************************/
void t_WordWizard :: word_of_struct( char fTo, short i_struct, t_String &Ant )
{
   char        Str1[20] ;
   t_Format1  *FF=&Grammar.format1() ;
   t_Struct   *SS ;
   if( fTo==0 ) SS=&Grammar[i_struct].From ;
       else     SS=&Grammar[i_struct].To ;
      
   sprintf( Str1,"%3s ",FF->get_tag( fTo,i_struct ) );
   Ant =Str1 ;
   Ant+=SS->Name ;
   Ant+=" " ;
   Ant+=Str1 ;
   Ant+=":"  ;
   for( short i=0 ; i<SS->Word.j ; i++ )
   {  
      Ant+=FF->get_tag(fTo,SS->Word[i].i_struct);
      Ant+="[" ;
      Ant+=SS->Word[i].Name ;
      Ant+="] " ;
   }
}
/*******************************************************************/
//      вставить текст части речи v[вставить] (все части речи)
/*******************************************************************/
void t_WordWizard :: part_all( )
{  short  i ;
   t_View S ;

   // -------- сформировать список структур -----------
   char        fTo=in_dst( );
   t_String Str ;
   char        Str1[20] ;
   t_Format1  *FF=&Grammar.format1() ;

   Select.clr() ;
   if( fTo==0 )
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].From.type==TWORD )
          {   S.index=i ;
	      sprintf( Str1,"%3s ",FF->get_tag( fTo,i ) );
	      Str =Str1 ;
	      Str+=Grammar[i].From.Name ;
	      strcpy( S.Str, Str.c_str() );
              Select.add( S );
          }
   }
   else
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
          if( Grammar[i].To.type==TWORD )
          {   S.index=i ;
	      sprintf( Str1,"%3s ",FF->get_tag( fTo,i ) );
	      Str =Str1 ;
	      Str+=Grammar[i].To.Name ;
	      strcpy( S.Str, Str.c_str() );
              Select.add( S );
          }
   }
   Select.calc_pos();
   i=Select.loop();
   if( i<0 ) return ;
   // --------- вставить тег структуры ---------------
   EditWord.insert_part( FF->get_tag( fTo,Select.ViewList[i].index ) );
}
/***************************************************************************/
//      вставить текст части речи v[вставить] (интересные части речи)
/***************************************************************************/
void t_WordWizard :: part( )
{  short     i,i_unknown ;
   t_View    S ;
   t_Struct *SS ;

   // -------- сформировать список структур -----------
   char fTo=in_dst( );
   i_unknown=Grammar.i_unknown( fTo ) ;
   if( i_unknown<0 )
   {   part_all( );
       return ;
   } 
   if( fTo==0 ) SS=&Grammar[i_unknown].From ;
       else     SS=&Grammar[i_unknown].To ;

   t_String Str ;
   char        Str1[20] ;
   t_Format1  *FF=&Grammar.format1() ;

   Select.clr() ;
   for( i=0 ; i<SS->Word.j ; i++ )
   {   S.index=SS->Word[i].i_struct ;
       sprintf( Str1,"%3s ",FF->get_tag( fTo,S.index ) );
       Str =Str1 ;
       Str+=SS->Word[i].Name ;
       strcpy( S.Str, Str.c_str() );
       Select.add( S );
   }
   Select.calc_pos();
   i=Select.loop();
   if( i<0 ) return ;
   // --------- вставить тег структуры ---------------
   EditWord.insert_part( FF->get_tag( fTo,Select.ViewList[i].index ) );
}
/***************************************************************************/
//                вставить параметры данного слова
/***************************************************************************/
void t_WordWizard :: param( )
{  t_String Tag,sParam ;
   t_Struct   *SS ;
   t_View      S ;
   short       i,i1 ;
   char        r ;

   r=EditWord.get_tag( Tag );
   if( r<0 ) return ;
   
   char        fTo=in_dst( );
   t_Format1  *FF =&Grammar.format1() ;
   short       i_struct=FF->find_struct( TO,(char *)Tag.c_str() );

   if( i_struct<0 ) return ;

   if( fTo==0 ) SS=&Grammar[i_struct].From ;
       else     SS=&Grammar[i_struct].To ;

   if( SS->Param.j<=0 ) return ;

   sParam="(" ;
   for( i=0 ; i<SS->Param.j ; i++ )
   {  
      if( 0<i ) sParam+=",";
      if( SS->Param[i].Dir==1 )
      {   t_Param *Param ;
          if( fTo==0 ) Param =&Grammar.from().Param[SS->Param[i].param] ;
	      else     Param =&Grammar.to  ().Param[SS->Param[i].param] ;
	  Select.clr() ;
          for( i1=0 ; i1<Param->Value.j ; i1++  )
          {  S.index=i1 ;
	     strcpy( S.Str, Param->Value[i1].Name );
	     Select.add( S );
	  }
	  Select.calc_pos();
	  i1=Select.loop();
	  if( 0<=i1 )
	  {   sParam+=Param->Value[i1].Name ;
	  }
      }
      else
      {   sParam+=SS->Param[i].Name ;
      }
   }
   sParam+=")";
   EditWord.insert_param( (char *)sParam.c_str() );
}
/******************************************************************/
//                   расструктурить строку
/******************************************************************/
void t_EditLine2 :: to_plain(  )
{  short i,j ;
   char  f ;
   t_myChar c ;

   for( i=j=0,f=0 ; i<Length ; i++ )
   {  c=Str[i] ;
      if( c==']' ) { f=0 ; Str[j++]=' ' ; }
      if( f==1   ) Str[j++]=c ;
      if( c=='=' ) Str[j++]='=' ;
      if( c=='[' ) f=1 ;
   }
   Length=Cursor=j ;
   Str[j]=0 ;
}
/******************************************************************/
//       в приемнике или источнике находится курсор
/******************************************************************/
char t_WordWizard :: in_dst( )
{  short i,z ;

   for( z=i=0 ; i<EditWord.Length ; i++ )
      if( EditWord.Str[i]=='=' )
      {  z=i ; break ; }
   if( z<EditWord.Cursor ) return 1 ;
   return 0 ;
}
/******************************************************************/
//                  Сформировать массив From
/******************************************************************/
void t_Wizard1 :: make_from( char *Str )
{  char c,c1 ;
   long i,j,L ;

   L=strlen( Str );
   // ------- исключить лишние пробелы ----------
   for( i=j=0 ; i<L ; i++ )
   {  c=Str[i] ;
      if( c==' ' && c1==' ' ) continue ;
      Str[j++]=c ;
   }
   L=j ;
   Str[L]=0 ;
   if( Str[L-1]==' ' ) 
   {  L-- ; Str[L]=0 ;  }
   
   From.j=0 ; 
   From.add( Str );
   for( i=0 ; i<L ; i++ )
   {  if( Str[i]==' ' )
      {   Str[i]=0 ;
          char *s=Str+i+1 ;
          From.add( s );
      }
   }
   n_word  =From.j ;
   n_struct=Grammar.n_trans();
}
/******************************************************************/
//                    Сделать главную таблицу
/******************************************************************/
void t_Wizard1 :: make_main_table( char fTo )
{
   short   i,i_word,i_part,N ;
   char   *Str ;
   long    Ant1[20] ;
   short   part[20] ;
   t_Form  Form[20] ;

   Free( Variants );
   Variants=(char *)Calloc( From.j*Grammar.n_trans(), sizeof(char) );

   for( i_word=0 ; i_word<From.j ; i_word++ )
   {
      Str=From[i_word] ;

      if( fTo==0 )
      {	  // ----------- найти в словарях форм -----------------
	  for( i=0 ; i<Grammar.from().File.j ; i++ )
	  {  i_part=SlowoF[i].Format->i_part ;
             N=SlowoF[i].quest( Str, Ant1, Form );
             if( 0<N )
        	 Variants[n_struct*i_word+i_part]=1 ;
	  }
	  // ----------- найти в главном словаре ---------------
	  N=Perevod->part( Str, part );
	  for( i=0 ; i<N ; i++ )
              Variants[n_struct*i_word+part[i]]=1 ;
      }
      else
      {   // ----------- найти в словарях форм -----------------
	  for( i=0 ; i<Grammar.to().File.j ; i++ )
	  {  i_part=SlowoT[i].Format->i_part ;
             N=SlowoT[i].quest( Str, Ant1, Form );
             if( 0<N )
        	 Variants[n_struct*i_word+i_part]=1 ;
	  }
      }
   }
}
/******************************************************************/
//      построить дерево словосочетаний для этой структурой (рекурсивная)
//      *S    - структура
//      up    - вышестоящая вершина
//      i_word- индекс в строке from
//      index - индекс слова (в структуре)
/******************************************************************/
void t_Wizard1 :: make_tree( t_Struct *S, long up, short i_word, short index )
{  short i,i_struct ;

   t_WordTree W ;
 
   if( n_word   <=i_word ) return ;
   if( S->Word.j<=index ) return ;
   i_struct=S->Word[index].i_struct ;

   W.up      =up ;
   W.index   =index ;
   W.i_word  =i_word ;
   W.i_struct=i_struct ;
   W.end     =0 ;

   if( 0<=i_struct )
   {   if( 0==Variants[n_struct*i_word+i_struct] ) return ;
       W.i_last_word=i_word+1 ;
       if( W.i_last_word==n_word && index+1==S->Word.j )
           W.end=1 ;
       WordTree.add( W );
       make_tree( S, WordTree.j-1, i_word+1, index+1 );
   }
   else
   {   for( i=i_word+1 ; i<n_word ; i++ )
       {   W.i_last_word=i ;
	   if( W.i_last_word==n_word && index==S->Word.j-1 )
               W.end=1 ;
	   WordTree.add( W );
	   make_tree( S, WordTree.j-1, i, index+1 );
       }
   }
}
/******************************************************************/
//  построить дерево словосочетаний для этой структуры (главная)
/******************************************************************/
void t_Wizard1 :: make_tree_main( t_Struct *S, short i_struct )
{
    WordTree.j=0 ;
    if( S->type==TWORD )
    {   if( 1==Variants[n_struct*0+i_struct] && 1==n_word )
	{   t_WordTree W ;
	    W.up          =-1 ;
	    W.index       = 0 ;
	    W.i_word      = 0 ;
	    W.i_last_word = 1 ;
	    W.i_struct    = i_struct ;
	    W.end         = 1 ;   
	    WordTree.add( W );
	}
    }
    else
        make_tree( S, -1, 0, 0 );
}
/******************************************************************/
//    обструктурить словосочетание (для одной вершины дерева вариантов
//    i_tree - вершина в дереве (концевая)
/******************************************************************/
void t_Wizard1 :: string_of_variant( char fTo, short i_struct, long i_tree, t_String &Ant )
{   long i,i1,j,z_tree ;
    long Stack[30] ;

    z_tree=i_tree ;
    j=0 ;
    while( 0<=z_tree )
    {   
        Stack[j++]=z_tree ;
        z_tree=WordTree[z_tree].up ;
    }

    Ant="" ;
    Ant+=Grammar.format1().get_tag( fTo, i_struct );
    Ant+=":" ;
    for( i=j-1 ; 0<=i ; i-- )
    {  t_WordTree *T=&WordTree[Stack[i]] ;
       Ant+=Grammar.format1().get_tag( fTo,T->i_struct );
       Ant+="[" ;
       for( i1=T->i_word ; i1<T->i_last_word ; i1++ )
       {  if( i1!=T->i_word ) 
              Ant+=" ";
          Ant+=From[i1] ;
       }
       Ant+="]" ;
    }
}
/******************************************************************/
//                      обструктурить
/******************************************************************/
void t_WordWizard :: make_struct()
{
   short i,i1,j ;

   // --- располовинить строку (а еще ее можно по пути расструктурить) ----
   char fTo=in_dst( );
   t_myChar *Str0=EditWord.Str ;
   t_myChar  Str1[100] ;
   char      Str2[100] ;
  
   for( j=i=0 ; Str0[i]!=0 ; i++ )
   {  if( Str0[i]=='=' ) break ;
      Str1[j++]=Str0[i];
   }
   if( fTo==1 && Str0[i]!=0 )
   for( j=0,i++ ; Str0[i]!=0 ; i++ )
      Str1[j++]=Str0[i];
   Str1[j]=0 ;
      
   // --- построить список структур ------------------------------
   t_String Ant ;
   t_View      V ;

   myChar_to_koi9( Str1,(uchar *)Str2,100,100 );
   make_from( Str2 );
   make_main_table( fTo );
   
   Select.clr() ;
   if( fTo==0 )
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
       {  if( Grammar[i].From.type==TSTRUCT1 )
	  {   make_tree_main( &Grammar[i].From, i );
	      for( i1=0 ; i1<WordTree.j ; i1++ )
	         if( WordTree[i1].end==1 )
		 {   string_of_variant( fTo,i,i1,Ant );
		     strcpy( V.Str,Ant.c_str() );
		     Select.add(V);
		 }
	  }
       }
   }
   else
   {
       for( i=0 ; i<Grammar.n_trans() ; i++ )
       {  if( Grammar[i].To.type==TSTRUCT1 || Grammar[i].To.type==TWORD )
	  {   make_tree_main( &Grammar[i].To, i );
	      for( i1=0 ; i1<WordTree.j ; i1++ )
	         if( WordTree[i1].end==1 )
		 {   string_of_variant( fTo,i,i1,Ant );
		     strcpy( V.Str,Ant.c_str() );
		     Select.add(V);
		 }
	  }
       }
   }
   // ------- достать и вставить -------------------------
   Select.calc_pos();
   i=Select.loop();
   if( i<0 ) return ;
   EditWord.insert_all( fTo,Select.ViewList[i].Str );
}
/***************************************************************************/
//              вставить тег структуры
/***************************************************************************/
void t_EditLine2 :: insert_struct( char TO, char *Str1 )
{  short i,z,z1 ;

   // ------- найти место куда вставлять ----------
   z=0 ;
   if( TO==1 )
   {   for( i=0 ; i<Length ; i++ )
          if( Str[i]=='=' ) 
          {   z=i+1 ; goto M1 ;  }
       return ;
   }
M1:
   // ----- найти двоеточие ----------------------
   for( i=z1=z ; i<Length ; i++ )
   {  if( Str[i]==':' )
      {   z1=i ; break ;  }
      if( Str[i]=='=' ) break ;
   }
   // ----- убрать и вставить то, что надо ------
   fdelete( z,z1 );
   insert_text( z,":" );
   insert_text( z,Str1 );
}
/***************************************************************************/
//              вставить тег части речи
/***************************************************************************/
void t_EditLine2 :: insert_part( char *Str1 )
{  short i,z,z1 ;
   z=z1=-1 ;
   // ------- найти место куда вставлять ----------
   for( i=Cursor ; 0<i ; i-- )
      if( Str[i]=='[' || Str[i-1]==' ' || Str[i-1]=='=' )
      {   z1=i ; break ;  }
   for( i=z1 ; 0<i ; i-- )
      if( Str[i]==']' || Str[i]==' ' || Str[i]=='=' )
      {   z=i ; break ;  }
   // ----- убрать и вставить то, что надо ------
   fdelete( z+1,z1 );
   insert_text( z+1,Str1 );
   // ----- заключить в квадратные скобки -------
   if( Str[Cursor]!='[' )
       insert_text( Cursor,"[" );
   for( i=Cursor ; i<Length ; i++ )
   {  if( Str[i]==']' ) return ;
      if( Str[i]==' ' || Str[i]=='=' ) break ;
   }
   insert_text( i,"]" );
}
/***************************************************************************/
//                     выдать тег части речи
/***************************************************************************/
char t_EditLine2 :: get_tag( t_String &Tag )
{  short i,z,z1 ;

   z=0 ; z1=-1 ;
   for( i=Cursor ; 0<=i ; i-- )
      if( Str[i]==' ' || Str[i]==']' || Str[i]=='=' )
      {   z=i ; break ;  }
   for( i=z+1 ; i<Length ; i++ )
      if( Str[i]=='[' )
      {   z1=i ; break ;  }

   if( z1<0 ) return -1 ; // тег не найден
   Cursor=z1 ;

   Tag="" ;
   for( i=z ; i<z1 ; i++ )
   {  if( Str[i]==' ' || Str[i]==']' || Str[i]=='=' ) continue ;
      Tag+=Unicod_to_koi8( Str[i].Char );
   }
   return 0 ;
}
/***************************************************************************/
//                   вставить обструктуренную строку
/***************************************************************************/
void t_EditLine2 :: insert_all( char fTo, char *Str1 )
{  short i,z ;

   for( i=0 ; i<Length ; i++ )
      if( Str[i]=='=' ) 
      {  z=i ; break ;  }

   if( fTo==0 )
   {   fdelete( 0,z );
       insert_text( 0,Str1 );
   }
   else
   {   fdelete( z+1,Length );
       insert_text( z+1,Str1 );
   }
}
/******************************************************************/
//               вставить список параметров
/******************************************************************/
void t_EditLine2 :: insert_param( char *Str1 )
{  short i,z ;
   z=-1 ;
   for( i=Cursor ; i<Length ; i++ )
      if( Str[i]==']' ) 
      {  z=i ; break ;  }
   if( z<0 ) return ;
   insert_text( z+1,Str1 );
}
/******************************************************************/
//                дублировать словарную статью
/******************************************************************/
void t_EditLine2 :: double_str(  )
{  short i,z ;
   z=-1 ;
   for( i=0 ; i<Length ; i++ )
      if( Str[i]=='=' ) 
      {  z=i ; break ;  }
   for( i=0 ; i<z ; i++ )
      Str[i+z+1]=Str[i];
   Str[Length=2*z+1]=0 ;
}
/******************************************************************/
//           назначить единственно верный перевод слову
/******************************************************************/
void t_EditLine2 :: set_perevod( char *Str1 )
{  short i ;

   for( i=0 ; i<Length ; i++ )
      if( Str[i]=='=' ) 
      {  Length=i+1 ; break ;  }
   Str[Length]=0 ;   
   insert_text( Length,Str1 );  
}
/******************************************************************/
//           назначить единственно верный перевод слову
/******************************************************************/
void t_EditLine2 :: insert_perevod( char *Str1 )
{  short i ;

   if( Cursor!=Length )   
   {  for( i=Cursor ; 0<i ; i-- )
	 if( Str[i]=='=' || Str[i]=='=' ) 
	 {  Cursor=i+1 ; break ;  }
      insert_text( Cursor,";" );
      Cursor-- ;
   }
   else
   {  insert_text( Cursor,";" );
   }
   insert_text( Cursor,Str1 );
}
/******************************************************************/
DEFINE_LIST_BTYPE( t_WordTree,t_WordTreeList )
DEFINE_LIST_BTYPE( t_View ,t_ViewList  )
