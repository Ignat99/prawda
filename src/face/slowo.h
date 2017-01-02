/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                     "Человеческий" словарь
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/
# ifndef SLOWO_H
# define SLOWO_H

struct t_slowoWord
{  char *Str ;   // само слово (размещается в WordStr)
   long  begin,  // начало и конец в файле подкачки
         end ;
};

struct t_Slowo
{
   t_String    Name ;        // имя словаря например "bokarev"
   t_String    NameFile ;    // имя файла в котором лежит словарь "eo_ru/bokarev"
   char       *WordStr ;     // массов в котором лежат прочитанные слова
   long        L_WordStr ;   // длинна массива WordStr
   t_slowoWord *Word  ;      // слова
   long        n_Word ;      // число слов

   char        Divider[10] ;
   short       l_Divider ;

         t_Slowo( void );
	~t_Slowo( void );
   void  del();
   char  read( char *File );
   long  find( char *Str );
   char *wort( long i );        // само слово
   char *trans( long i_wort );  // транскрипция
   char *answer( long i_wort ); // ответ словарной статьи
   void  set_divider( char * _Divider );
   void  set_sort( char f );
};

# endif





