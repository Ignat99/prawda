/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//          Структуры, описывающие грамматику (словообразование)
//       Анисимов Д.В.                                    сегодня
/***************************************************************************/
# ifndef GRAMMAR1_H
# define GRAMMAR1_H

# include "list.h"

// ------------- шаг алгоритма ----------------------------------
struct t_Step1
{
   char   type ;  //  тип 0-шаг 1-алгоритм 2-анал шаг
   short  label ; //  метка
   short  up ;    //  ссылка на вышестоящий шаг
   short  i_step ;//  индекс шага или алгоритма
   char  *Name ;  //  имя шага

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
