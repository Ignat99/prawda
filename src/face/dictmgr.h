# ifndef DICTMGR_H

# include "list.h"

struct t_dvWord
{
   long i_kontext ;   // индекс в Kontext.Type
   char select ;
};
DECLARE_LIST_TYPE( t_dvWord,t_dvWordList );

class t_DictVisard : public t_Win
{
public:
   t_dvWordList  Extern ;

   long          Cursor ; 
   long          Begin ;
   short         BeginX ;
   t_Rect        rect_Current  ,rect_Extern ;
   char          f_Half ;       // 0-значит работаем с левой панелью
   t_String      DictFile ;
public:
             t_DictVisard( );

   void      paint( );
   e_WinMsg  loop( );
   void      set_pos( short x1, short y1, short x2, short y2 );
   void      select_new_word( );
   void      select_new_perevod( );
   void      select_old_word( );
   void      deselect_current( );
   void      deselect_extern( );
   void      delete_selected( );
   void      add_selected_word( );
   void      ugoday_perevod( void );
   void      read_dict( );
   void      make_reflection( );
   short     dialog_owerwrite( char *Old, char *New );
   short     exist_new_perevod( t_sRecord1 *Record1, t_sRecord *Record );
   void      print_new_perevod( t_String &Str, t_sRecord1 *Record1, long i_record );
   void      find();
};

# endif
