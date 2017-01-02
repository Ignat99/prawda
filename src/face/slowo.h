/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                     "������������" �������
//     �������� �.�.                                         �������
/***************************************************************************/
# ifndef SLOWO_H
# define SLOWO_H

struct t_slowoWord
{  char *Str ;   // ���� ����� (����������� � WordStr)
   long  begin,  // ������ � ����� � ����� ��������
         end ;
};

struct t_Slowo
{
   t_String    Name ;        // ��� ������� �������� "bokarev"
   t_String    NameFile ;    // ��� ����� � ������� ����� ������� "eo_ru/bokarev"
   char       *WordStr ;     // ������ � ������� ����� ����������� �����
   long        L_WordStr ;   // ������ ������� WordStr
   t_slowoWord *Word  ;      // �����
   long        n_Word ;      // ����� ����

   char        Divider[10] ;
   short       l_Divider ;

         t_Slowo( void );
	~t_Slowo( void );
   void  del();
   char  read( char *File );
   long  find( char *Str );
   char *wort( long i );        // ���� �����
   char *trans( long i_wort );  // ������������
   char *answer( long i_wort ); // ����� ��������� ������
   void  set_divider( char * _Divider );
   void  set_sort( char f );
};

# endif





