/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//          ���������, ����������� ���������� (����������������)
//       �������� �.�.                                    �������
/***************************************************************************/
# ifndef GRAMMAR1_H
# define GRAMMAR1_H

# include "list.h"

// ------------- ��� ��������� ----------------------------------
struct t_Step1
{
   char   type ;  //  ��� 0-��� 1-�������� 2-���� ���
   short  label ; //  �����
   short  up ;    //  ������ �� ����������� ���
   short  i_step ;//  ������ ���� ��� ���������
   char  *Name ;  //  ��� ����

         t_Step1();
   void  init();
};
DECLARE_LIST_TYPE( t_Step1, t_Step1List );

struct t_Algorithm
{
   char *Name ;
   t_Step1List Step ;
   
	 t_Algorithm();
	~t_Algorithm();
   void  init();
   void  del();
};

DECLARE_LIST_TYPE( t_Algorithm, t_AlgorithmList );

# endif
