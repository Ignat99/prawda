/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                       ������� �� �����
//     �������� �.�.                                         �������
/***************************************************************************/

# ifndef DIR_H
# define DIR_H

# include "mylib.h"
# include "list.h"

/* ----------------------- ���, ��� ��������� � ������ �������� ������ --- */
struct t_File1
{  char name[32] ;    /* ��� ����� � ����������� */
   long size ;        // ������ �����
   long date ;
   long time ;
   char type ;        /* 0-���� 1-������� */
} ;
DECLARE_LIST_TYPE( t_File1 ,t_File1List );

class t_Dir
{  t_File1List File ;
   short    Begin,        // ������ ����, ������� �� ������
	    i_File;       // ������� ����
   char     shabl[32] ;   // ������ ������
   char     Name[32] ;    // ��� ������ �����
   t_String Path ;        // ������� �������
public:
   short    i_Code ;      // ������ ���������
   short    i_Format ;    // ������ �������
   char     f_CFNeed ;    // ����� �� ��� � ������
public:
          t_Dir();
   short  main_loop( t_String &Antwort, char f=0 );
   void   set_schabl( char *str );
   void   get_schabl( char *str );
private:
   short  loop_file( void );         // ��������� ������ � ���� ������
   short  loop_schabl( void );       // ��������� ������ � ���� �������
   short  loop_name( void );         // ��������� ������ � ���� ������ �����
   short  loop_code( void );
   short  loop_format( void );
   void   paint( char fsave, char Reg );
   void   paint_karkas( char f, char Reg );// ���������� ��� �������
   void   paint_file( char f );      // ���������� ������ ������
   void   paint_schabl( void );      // ���������� ������
   char   read_file( t_String &Path );   // �������� ������ ������ � �������� ��������
   void   help( void );
   void   file_date( char *antwort, struct t_File1 &file );
   void   sort_file( void );
} ;

# endif
