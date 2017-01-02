// **************************************************************************
//   �������� ����� ��������� � ���� ���� ������ ���
//   ��������������� ���� � ������� ������� g++ -E -o lang.txt -I. lang.cpp
// **************************************************************************

# define NO_ESTI_S

@������  00_00_00
@���������_�������

# define W_SUB  @�����{  ���    �����=�� �����=� }
# define W_ADJ  @�����{  ����   �����=�� �����=� }
# define W_VERB @�����{  ������ �����=Inf }

// **************************************************************************
//    �������� ������
// **************************************************************************
@�����_���������
{  // ----------------- ���� ��������� --------------------
   @�������� �����   {  � ,�   };
   @�������� �����   {  ��,��  };
   @�������� ���     {  �,�,�  };
   @�������� ����    {  1�,2�,3�  };
   @�������� �����   {  �������, ���������, �������  };
   @�������� �����   {  Inf,������,�������,�����,���������,����_����,������������  }; // ����� �������

   @�������� �����_������� {  �������, �������, ������������, �������, ������  }; // �� �� �� �� .

   @�����_���� ������ ( ����� �����, ����� �����, ����� �����, ����� ����� ) ;
   @�����_���� ������ ( ����� �����, ����� �����, ����� �����, ����� ����� ) ;
   @�����_���� ���    ( ����� �����, ����� ����� );
   @�����_���� ����   ( ����� �����, ����� ����� );
   @�����_���� ����11  ; // ������ �����������
   @�����_���� �������( �����_������� &����� ) ;

   @�����_���� ������������ ;   
   @�����_���� ����_���������� ;
   @�����_���� ������ ;
   @�����_���� ����( ��� &���, ���� &����, ����� &�����, ����� ����� );
   @�����_���� ����_����( ����� �����, ����� ����� );
   @�����_���� ����_���� ;
   @�����_���� �������( ����� &����� ) ;
   @�����_���� ���� ;
   @�����_���� ������� ( ����� &����� ) ;
   @�����_���� ������� ; // ����� ����, �� �������� �� � ����� �����������

   @�����_���� ������_�_�����������_��( ����� �����, ����� ����� );
   @�����_���� ������_�_����������_�� = ;
   @�����_���� ������_�_��������������_�� = ;
   @�����_���� ������_�_����_��( ����� �����, ����� ����� );

   @���������1 ��_���_�0 ( ����� �����, ����� ����� ) = ���( &�����, &����� ) ;
   @���������1 ��_���_�1( ����� �����, ����� ����� ) =
               ����( �����1, �����1 ) ���( &�����, &����� ) ;

   @���������1 ��_����_�0( ����� �, ����� � ) = ����( &�, &� ) ;

   @���������1 ��_������_�0( ����� �����, ����� �����, ����� �����, ����� ����� ) =
               ������ ( &�����, &�����, &�����, &����� );

   // -------- ��������� ��� ���������������� ------------
   @���������1 ��_���_�20( ����� �����, ����� ����� ) =
               ������ ( %Inf, @0, @0, @0 ) ;
   @���������1 ��_�������_�0( �����_������� ����� ) = �������( &����� ) ;

   @���������  @��������_�����_���� = 
               ���( &�, &� ) ����( &�, &� ) ������( &�, &�, &�, &� ) �������( &S ) ���������� ;
   @���������  @������� = 
               ��_���_�  ��_���_�  ��_����( &� )  ��_�������  ��_������( &�, &�, &�, &� )
	       ������� ;

   # include <format_es.h>

   @����� ������                   ������1  { dicts/zform/esperanto/es_verb_r };
   @����� ������                   ������11 { dicts/zform/esperanto/es_esti };
   @����� ���                      ������2  { dicts/zform/esperanto/es_sub_r  };
   @����� ����                     ������21 { dicts/zform/esperanto/es_adj_r  };
   @����� ����_����                ������22 { dicts/zform/esperanto/es_rel  };
   @����� ������_�_�����������_��  ������23 { dicts/zform/esperanto/es_qsub };
   @����� ������_�_����_��         ������24 { dicts/zform/esperanto/es_qadj };
   @����� ����                     ������3  { dicts/zform/esperanto/es_pronoun  };

   @��������� { o      @������� = o       o @���� {  ���   �����=�� �����=� } W_SUB }
   @��������� { on     @������� = on      o @���� {  ���   �����=�� �����=� } W_SUB }
   @��������� { oj     @������� = oj      o @���� {  ���   �����=�� �����=� } W_SUB }
   @��������� { ojn    @������� = ojn     o @���� {  ���   �����=�� �����=� } W_SUB }
   
   @��������� { a      @������� = a       a @���� {  ����  �����=�� �����=� } W_ADJ }
   @��������� { an     @������� = an      a @���� {  ����  �����=�� �����=� } W_ADJ }
   @��������� { aj     @������� = aj      a @���� {  ����  �����=�� �����=� } W_ADJ }
   @��������� { ajn    @������� = ajn     a @���� {  ����  �����=�� �����=� } W_ADJ }
   
   @��������� { e      @������� = e       e @���� { ������� } @�����{ ������� } }
   
   @��������� { i      @������� = i       i @���� { ������  �����=Inf                    } W_VERB }
   @��������� { as     @������� = as      i @���� { ������  �����=������ �����=��������� } W_VERB }
   @��������� { is     @������� = is      i @���� { ������  �����=������ �����=�������   } W_VERB }
   @��������� { os     @������� = os      i @���� { ������  �����=������ �����=�������   } W_VERB }
   @��������� { us     @������� = us      i @���� { ������  �����=�����                  } W_VERB }
   @��������� { u      @������� = u       i @���� { ������  �����=�������                } W_VERB }

   @��������� { anta   @������� = anta    i @���� { ������  �����=���������    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { antan  @������� = antan   i @���� { ������  �����=���������    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { antaj  @������� = antaj   i @���� { ������  �����=���������    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { antajn @������� = antajn  i @���� { ������  �����=���������    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { inta   @������� = inta    i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { intan  @������� = intan   i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { intaj  @������� = intaj   i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { intajn @������� = intajn  i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { onta   @������� = onta    i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { ontan  @������� = ontan   i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { ontaj  @������� = ontaj   i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { ontajn @������� = ontajn  i @���� { ������  �����=���������    �����=�������   �����=�� �����=� } W_VERB }

   @��������� { ata    @������� = ata     i @���� { ������  �����=����_����    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { atan   @������� = atan    i @���� { ������  �����=����_����    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { ataj   @������� = ataj    i @���� { ������  �����=����_����    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { atajn  @������� = atajn   i @���� { ������  �����=����_����    �����=��������� �����=�� �����=� } W_VERB }
   @��������� { ita    @������� = ita     i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { itan   @������� = itan    i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { itaj   @������� = itaj    i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { itajn  @������� = itajn   i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { ota    @������� = ota     i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { otan   @������� = otan    i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { otaj   @������� = otaj    i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }
   @��������� { otajn  @������� = otajn   i @���� { ������  �����=����_����    �����=�������   �����=�� �����=� } W_VERB }

   @��������� { ante   @������� = ante    i @���� { ������  �����=������������ �����=��������� �����=�� �����=� } W_VERB }
   @��������� { inte   @������� = inte    i @���� { ������  �����=������������ �����=�������   �����=�� �����=� } W_VERB }
   @��������� { onte   @������� = onte    i @���� { ������  �����=������������ �����=�������   �����=�� �����=� } W_VERB }
}
// *************************************************************************
//
// *************************************************************************
@�����_���������
{  // ------------------------- ������� ���� ------------------
   @�������� �����   {  �,�,�,�,�,�  };
   @�������� �����   {  ��,��  };
   @�������� ���     {  �,�,�,��  };
   @�������� ������� {  �������������,������������  };
   @�������� ����    {  1�,2�,3�  };
   @�������� �����   {  �������,���������,�������  };
   @�������� �����   {  �����,������,�������  };
   @�������� ������������� {  ���,�����  };
   @�������� �����   {  Inf,������,���������,������������  };
   @�������� ���     {  ������,����������  }; // ��� �����������
   @�������� ����������� { ��,��� };
   @�������� ����    {  �,�  };
   @�������� �_����� {  �������,�,���� };
   @�������� �_����� {  ��,��,���  };
   @�������� �����_������� {  �������, ������  };

   @�����_���� ������ ( ������������� ��, ����� �����, ���� ����, 
                        ����� �����, ����� �����, ��� ���, ����� �����,
			    ����� ����� ) ;
   @�����_���� ������ ( ������������� ��, ����� �����, ���� ����,
                            ����� �����, ����� �����, ��� ���, ����� ����� ) ;
   @�����_���� ���  ( ��� &���, ���� &����, ����� �����, ����� ����� );
   @�����_���� ���� ( ���  ���, ����  ����, ����� �����, ����� ����� );
   @�����_���� ����11  ; // ������ �����������
   @�����_���� �������( �����_������� &����� ) ;
   @�����_���� ������������( �_����� &������, �_����� &������, ����� ����� ) ;
   @�����_���� ����_����������( ����� ����� ) ;

   @�����_���� ������(  ��� ���, ����� �����, ����� ����� );
   @�����_���� ���� ( ��� &���, ���� &����, ����� &�����, ����� ����� ); // ����������
   @�����_���� ����_���� ( ���  ���, ����� �����, ����� ����� );
   @�����_���� ����_���� ; //( �_����� &������, �_����� &������, ����� ����� ); // ������ ���� �����
   @�����_���� ������� ( ����� &����� ) ;
   @�����_���� ���� ;
   @�����_���� ������� ( ����������� ����������� ) ;
   @�����_���� ������� ; // ����� ����, �� �������� �� � ����� �����������

   @�����_���� ������_�_�����������_��( ����� ����� ) ;
   @�����_���� ������_�_����������_�� = ;
   @�����_���� ������_�_��������������_�� = ;
   @�����_���� ������_�_����_��( ���  ���, ����  ����, ����� �����, ����� ����� ) ;

   @���������1 ��_���_�0( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ���( &���, &����, �����, ����� ) ;
   @���������1 ��_����_�0( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
               ����( ���, ����, �����, ����� ) ;

   @���������1 ��_���_�1( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����( ���, ����, �����, ����� )
	       !���( &���, &����, �����, ����� ) ;
   @���������1 ��_���_�2( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
	       ���( &���, &����, �����, ����� )
	       @1 ;
   @���������1 ��_���_�3( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����( ���,  ����, �����, ����� )
	       ���( &���, &����, �����, ����� )
	       @1 ;
   @���������1 ��_���_�4( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����( ���,  ����, �����, ����� )
               ����( ���,  ����, �����, ����� )
	       ���( &���, &����, �����, ����� ) ;
//   @���������1 ��_���_�5( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
//	       ���( &���, &����, �����, ����� )	       
//	       ��_���_�( &�����1 ) ;
   @���������1 ��_���_�6( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����( ���,  ����, �����, ����� )
	       ���( &���, &����, �����, ����� )
	       ���( &���1, &����1, &�����1, ����� ) ;
   @���������1 ��_���_�7( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����( ���,  ����, �����, ����� )
	       ���( &���, &����, �����, ����� )
	       @1
	       ���( &���1, &����1, &�����1, ����� ) ;
//   @���������1 ��_���_�8( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
//	       ���( &���, &����, �����, ����� )
//	       @1
//	       ��_���_�( &�����1 );
   @���������1 ��_���_�9( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
	       @1
	       ����( ���,  ����, �����, ����� )
	       ���( &���, &����, �����, ����� ) ;
   @���������1 ��_���_�10( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
	       ���( &���, &����, �����, ����� )
	       ����( ���,  ����, �����, ����� ) ;
   // -------- ��� �������������� �������� ------------
   @���������1 ��_���_�11( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
	       ���( &���, &����, �����, ����� )
	       ���( &���1, &����1, %�, %�� );
   @���������1 ��_���_�12( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               ����[���������]( %�, %�, �����, ����� )
	       ���[�����]( &���, &����, �����, ����� )
	       !���( &���1, &����1, %�, %�� ) ;
   @���������1 ��_���_�20( ��� &���, ���� ����, ����� &�����, ����� &����� ) =
               ?����[���]( ���, %�, �����, ����� )
               @1 @1
	       !������( %�����, %������, %3�, %��, %���������, @0, %�����, @0 ) ;
   @���������1 ��_���_�21( ��� %�, ���� %�, ����� &�����, ����� &����� ) =
               ?���[��������]( &���, &����, �����, ����� )
	       @0[����]
	       !����( %�, @0, %�, %�� );
   @���������1 ��_���_�22( ��� %�, ���� %�, ����� &�����, ����� &����� ) =
	       ?���[�������]( &���, &����, �����, ����� )
	       !���( &���1, &����1, %�, %�� ) ;
   @���������1 ��_���_�1( ��� ���, ���� ����, ����� &�����, ����� &����� ) =
               @0[��] ?!���( &���, &����, �����, ����� ) ;

   @���������1 ��_����_�1( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
               ����( ���, ����, �����, ����� )
	       @1 ;
//   @���������1 ��_����_�2( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
//               ����( ���, ����, �����, ����� )
//	       ��_���_�( &�����1 ) ;
   @���������1 ��_����_�3( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
               ����( ���, ����, �����, ����� )
	       @1
	       ���( &���1, ����, &�����1, &�����1 ) ;
   @���������1 ��_����_�4( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
   	       @1
               ����( ���, ����, �����, ����� )
	       @1 ;
   @���������1 ��_����_�5( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
   	       ����( ���, ����, �����, ����� )
               ����( ���, ����, �����, ����� ) ;
   @���������1 ��_����_�6( ��� &���, ���� &����, ����� &�����, ����� &����� ) =
   	       @1
               ����( ���, ����, �����, ����� );

   @���������1 ��_������_�0( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                          ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               @1 ������( ��, �����, ����, �����, �����, ���, �����, ����� ) @1 ;
   @���������1 ��_������_�2( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                             ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               ?������[������]( ��, �����, ����, �����, �����, ���, �����, ����� )
	       !���( &�, &�, %�, %�� ) ;
   @���������1 ��_������_�3( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                             ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               ?������[������]( ��, �����, ����, �����, �����, ���, �����, ����� )
	       !����( %�, @0, %�, %�� );
   @���������1 ��_������_�4( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                             ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               @0[�����] !?������( ��, �����, ����, �����, �����, ���, �����, ����� ) ;
   @���������1 ��_������_�5( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                             ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               ?������[������]( ��, �����, ����, �����, �����, ���, �����, ����� )
	       !������        ( %�����, %Inf, @0, @0, @0, @0, @0, @0 ) ;
   @���������1 ��_������_�1( ������������� &��, ����� &�����, ���� &����, ����� &�����,
                             ����� &�����, ��� &���, ����� &�����, ����� &����� ) =
               @0[��] ?!������( ��, �����, ����, �����, �����, ���, �����, ����� ) ;

   @���������1 ��_�������_�1( �����_������� ����� ) =
               @0[��] ?!�������( &����� ) ;


   # include <format_r.h>

   @�����  ������                   ������1   { dicts/zform/russkij/ru_verb_r  };
   @�����  ������                   ������1_� { dicts/zform/russkij/ru_esti  };
   @�����  ���                      ������2   { dicts/zform/russkij/ru_sub_r };
   @�����  ����                     ������3_1 { dicts/zform/russkij/ru_adj_r };
   @�����  ������                   ������3   { dicts/zform/russkij/ru_quest };
   @�����  ����                     ������4_1 { dicts/zform/russkij/ru_pronoun_1 };
   @�����  ������_�_�����������_��  ������4_2 { dicts/zform/russkij/ru_quest_s };
   @�����  ������_�_����_��         ������3_3 { dicts/zform/russkij/ru_quest_a };
   @�����  ����_����                ������3_2 { dicts/zform/russkij/ru_pronoun_4 };
   @�����  �������                  ������5   { dicts/zform/russkij/ru_tiny };
   @�����  ������������             ������4   { dicts/zform/russkij/ru_num  };
   @�����  ����_����������          ������3   { dicts/zform/russkij/ru_num2 };

// @�����_���������                 ������1   { dicts/zform/russkij/semantika };

   // -------- ������� �������������� -------------------------
   @��������� {   ��� @������� = ��  ��   @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {    �� @������� =  ��  ��  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {   ��� @������� = ��� ���  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }
   @��������� {    �� @������� =  ��  ��  @���� {  ����  ���=� �����=� �����=��  } @����� { ���� } [�� �� �� �� ��� �� ��� �� ��� �� ��� �� �� �� �� �� ��� �� �� �� �� �� �� ��� �� �� �� ��] }

   @��������� {  ����� @������� = ����� ����� @���� {  ����  ���=� �����=� �����=��  } @����� { ���� }
      [���� ���� ���� ���� ����� ���� ����� ���� ����� ���� ����� ����   ���� ���� ���� ���� ����� ���� ����  ���� ����  ���� ����  ����� ���� ���� ���� ����] } 

   @��������� {   ���� @������� = ���� ���� @���� {  ����  ���=� �����=� �����=��  } @����� { ���� }
      [���� ���� ���� ���� ����� ���� ����� ���� ����� ���� ����� ����   ���� ���� ���� ���� ����� ���� ����  ���� ����  ���� ����  ����� ���� ���� ���� ����] }
}
/***************************************************************************/
//    �������� �������� ��������
/***************************************************************************/
@�������_��������
{
   @������_������� ������1
   {
      @���������  �� �������
      @���������  �� ������� 
      @���������  �� ������������
      @���������  �� �������
      @���������  .  ������

      @�����_����_���������
      {
	 v  ������ ;
	 b  ������ ;
	 s  ���    ;
	 a  ����   ;
	 d  ������� ;
	 i  ������������ ;
	 ia ����_���������� ;
	 I  ��_����2 ;
	 I3 ��_����3 ;
         o1 ���� ;
	 o2 ����_���� ;
	 p  ������� ;
	 PP �������_� ;
	 c  ���� ;
	 t  ������� ;
         z  ������� ;
	 zz ���������� ;

	 AB ����11 ;

	 DD ��_�������_�� ;
 	 D0 ��_�������_�0 ;

	 S0 ��_���_�0 ;
	 S1 ��_���_�1 ;
	 Sv ��_���_�20 ;
 	 Sb ��_���_�� ;
	 A0 ��_����_�0 ;
	 AA ��_����_�� ;
//         SS ��_���_� ;
	 Sn ��_��������������_� ;
	 Sa ��_��������������_� ;
	 
	 V1 ��_������_�0 ;
	 VV �����_�� ;
	 F  �����_������ ;
	 F1 �����_���_����������� ;
	 v1 ���� ;
	 f  inf ;
	 f1 ��_��������� ;

         QS ������_�_�����������_�� ;
         QO ������_�_����������_�� ;
         QV ������_�_��������������_�� ;
         QA ������_�_����_�� ;
      }
      @�����_����_���������
      {
	 v  ������ ;
	 b  ������ ;
	 s  ���    ;
	 a  ����   ;
	 d  ������� ;
	 i  ������������ ;
	 ia ����_���������� ;
	 I  ��_����2 ;
	 I3 ��_����3 ;
         o1 ���� ;
	 o2 ����_���� ; 
	 p  ������� ;
	 PP �������_� ;
	 c  ���� ;
	 t  ������� ;
         z  ������� ;
	 zz ���������� ;

	 AB ����11 ;
	 
 	 DD ��_�������_�� ;
 	 D1 ��_�������_�1 ;
	 
	 S0  ��_���_�0 ;
	 S1  ��_���_�1 ;
	 S2  ��_���_�2 ;
	 S3  ��_���_�3 ;
	 S4  ��_���_�4 ;
///	 S5  ��_���_�5 ;
	 S6  ��_���_�6 ;
	 S7  ��_���_�7 ;
//	 S8  ��_���_�8 ;
	 S9  ��_���_�9 ;
	 SA  ��_���_�10 ;
	 S11 ��_���_�11 ;
	 S12 ��_���_�12 ;
	 Sv  ��_���_�20 ;
	 S21 ��_���_�21 ;
	 S22 ��_���_�22 ;
	 S�1 ��_���_�1 ;
	 Sb  ��_���_�� ;

	 A0 ��_����_�0 ;
	 A1 ��_����_�1 ;
//	 A2 ��_����_�2 ;
	 A3 ��_����_�3 ;
	 A4 ��_����_�4 ;
	 A5 ��_����_�5 ;
 	 A6 ��_����_�6 ;
	 AA ��_����_�� ;

//	 SS ��_���_� ;
	 Sn ��_��������������_� ;
	 Sa ��_��������������_� ;
         F  �����_������ ;
	 F1 �����_���_����������� ;
	 V1 ��_������_�0 ;
	 V2 ��_������_�2 ;
	 V3 ��_������_�3 ;
	 V4 ��_������_�4 ;
	 V5 ��_������_�5 ;
	 Vn ��_������_�1 ;
	 VV �����_�� ;
	 v1 ���� ;
	 f  inf ;
	 f1 ��_��������� ;

         QS ������_�_�����������_�� ;
         QO ������_�_����������_�� ;
         QV ������_�_��������������_�� ;
         QA ������_�_����_�� ;
      }
   }
   @�����_��������������  ������1 {  dicts/eo_ru/es_macro  };
   @�����_���������       ������1 {  dicts/eo_ru/base_esru1
                                     dicts/eo_ru/base_esru  };
// @�����_��������������� ������1 {  dicts/eo_ru/base_post  };
}
@�������_��������
{
   @������_�������  ������1
   {  @�����������  = ;
      @����������   0 ;
   }   
   @�����_��������  ������1 { dicts/eo_ru/bokarev  };
}

# define table_chislo( A )       \
   @�������( ����� A = ����� A ) \
   {  ��  = �� ;                 \
      �� = �� ;                  \
   }
# define table_lizo( A )       \
   @�������( ���� A = ���� A ) \
   {  1� = 1� ;                \
      2� = 2� ;                \
      3� = 3� ;                \
   }
# define table_rod( A )      \
   @�������( ��� A = ��� A ) \
   {  � = � ;                \
      � = � ;                \
      � = � ;                \
   }
