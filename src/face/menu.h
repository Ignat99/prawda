/*******************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                             �������
//     �������� �.�.                               �������
/*******************************************************************/

# ifndef MENU_H
# define MENU_H

enum e_Menu
{  F_ENDE  =0 ,  // ������� ����� ������� ����
   F_SCHREI=1 ,  // �������,
   F_MENU  =2 ,  // �������
   F_FUNK  =3 ,  // �������
   F_NUMER =4    // �����. ��������
};

struct t_Menu
{  e_Menu flag ;
   char   ret ;             /* ������� �������� 0-� ������, 1 - � ����. ���� */
   void   (*funk)(void *) ; /* ���������� ������� */
   void   *data ;           /* ��������� ��� ������� ��� ������ ���������� */
   char name[30] ;          /* �������� ���� */
} ;

void  Sorry( void *V );
void  Window_help( void *Void );

short   menu( t_Menu *Menu, short y=0 ) ;
short   menu_mouse( t_Menu *Menu, short y, short x );
void    null_menu( t_Menu *Menu, short y=0 );
//void    menu_set( char *sFind, char *sReplace );
char    menu_set( t_Menu *Menu, char *sFind, char *sReplace );
t_Menu* menu_find( t_Menu *Menu, char *sFind );
# endif



