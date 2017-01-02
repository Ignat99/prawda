/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                       лазалка по диску
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# ifndef DIR_H
# define DIR_H

# include "mylib.h"
# include "list.h"

/* ----------------------- все, что относится к режиму открытия файлов --- */
struct t_File1
{  char name[32] ;    /* имя файла с расширением */
   long size ;        // размер файла
   long date ;
   long time ;
   char type ;        /* 0-файл 1-каталог */
} ;
DECLARE_LIST_TYPE( t_File1 ,t_File1List );

class t_Dir
{  t_File1List File ;
   short    Begin,        // первый файл, видимый на экране
	    i_File;       // текущий файл
   char     shabl[32] ;   // шаблон файлов
   char     Name[32] ;    // имя нового файла
   t_String Path ;        // текущий каталог
public:
   short    i_Code ;      // индекс кодировки
   short    i_Format ;    // индекс формата
   char     f_CFNeed ;    // нужен ли код и формат
public:
          t_Dir();
   short  main_loop( t_String &Antwort, char f=0 );
   void   set_schabl( char *str );
   void   get_schabl( char *str );
private:
   short  loop_file( void );         // обработка клавиш в окне файлов
   short  loop_schabl( void );       // обработка клавиш в окне шаблона
   short  loop_name( void );         // обработка клавиш в окне нового файла
   short  loop_code( void );
   short  loop_format( void );
   void   paint( char fsave, char Reg );
   void   paint_karkas( char f, char Reg );// нарисовать все рамочки
   void   paint_file( char f );      // нарисовать список файлов
   void   paint_schabl( void );      // нарисовать шаблон
   char   read_file( t_String &Path );   // прочесть список файлов в заданном каталоге
   void   help( void );
   void   file_date( char *antwort, struct t_File1 &file );
   void   sort_file( void );
} ;

# endif
