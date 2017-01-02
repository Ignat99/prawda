/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//                    t_Windows - главный класс интерфейса
//     Анисимов Д.В.                                         сегодня
/***************************************************************************/

# ifndef FACE_H
# define FACE_H

# include "dir1.h"
# include "help.h"
# include "option.h"
# include "window.h"
# include "edit.h"
# include "edit1.h"
# include "slowo1.h"
# include "macrostruct.h"
# include "macrostruct2.h"
# include "kontext2.h"

struct t_myChar ;

void   WW_init( void );
void   WW_set_pos( void );
void   WW_main_loop( void );
void   WW_create( void *Void );    // создать окно с файлом Name
void   WW_save_src( void *Void );  // запомнить оригинал
void   WW_save( void *Void );      // запомнить перевод
void   WW_save_as( void *Void );   // запомнить перевод под другим именем
void   WW_standen( void );
void   WW_exit( void *Void );      // выйти из редактора
void   WW_replace3( void *Void );  // в редакторе поиск-замена
void   WW_replace2( void *Void );  // в редакторе поиск-замена
void   WW_goto_str( void *Void );  // в редакторе перейти в заданную строку
void   WW_help( char *Str );       // подказка на ключевое слово Str
void   WW_help_exit( void );       // выход из подсказки
void   WW_paint( void );
void   WW_trans( void *Void );
void   WW_trans_frasa( void *Void );
void   WW_trans_bilingvo( void *Void );
void   WW_trans_besser( void *Void );
void   WW_win_firstlook( void );
e_WinMsg  WW_win_path( void );
e_WinMsg  WW_win_path2( void );
e_WinMsg  WW_win_slowo( void );
void   WW_prev_frasa( void );
void   WW_next_frasa( void );
void   WW_read_langtxt( void *Void );
void   WW_read_alldata( void *Void );
void   WW_set_format( void *Void );
void   WW_postproc_exe( void *Void );
char   WW_schnell_taste( short Shift, short t1, short t2 );

extern char hk_Edit[]  ;
extern char hk_Help[]  ;
extern char hk_Path[]  ;
extern char hk_Slowo[] ;
extern char hk_Word[]  ;
extern char hk_Word1[] ;

extern char hk_First[] ;
extern char hk_Sou[]   ;
extern char hk_Dst[]   ;
extern char hk_Debug1[];
extern char hk_Debug2[];
extern char hk_Debug3[];
extern char hk_Gramm[] ;
extern char hk_Perev[] ;
extern char hk_Dicts[] ;
extern char hk_FrasaEdit[];
extern char hk_DictMgr[];

extern t_String  hk_Frasa ;
extern t_String  hk_Frasa1 ;
extern t_String  hk_Frasa2 ;
extern t_String  hk_Macrostruct ;
extern t_String  hk_Kontext ;
extern t_String  hk_SetPerev ;


extern t_HelpData     HelpData ;
extern t_Option       Option ;
extern t_Slowo1       Slowo ;
extern t_Option       Option ;
extern t_EditTrans    ErrorView ;
extern t_EditTrans    Edit1 ;
extern t_Macrostruct  Macrostruct ;
extern t_Macrostruct2 Macrostruct2 ;
extern t_Kontext2     Kontext2 ;
extern t_SetPerevod   SetPerevod ;
extern t_DirTrans    *DirFormat ;
extern short          n_DirFormat ;

# ifndef __WIN__
//e_WinMsg kommand_yxf( short y, short x, char f, char *str, short lenght, char EIN );
//e_WinMsg kommand32_yxf( short y, short x, char f, t_myChar *str, short lenght,
//		   char EIN, t_myChar **str1=NULL, short n_str1=0 );
# endif

# endif
