// **************************************************************************
//
//                 ����� ���������� �����������
//
// **************************************************************************

// --------- ����� ��� ������ ����� ������������� -------------

@�������
{
   @�����  ���� = ���� @0[,] ;
   @�����  ���� = ���� @0[,] ;
}

@�������
{
   @��������� �����_����������_���� = ���� ���_��� ;
   @��������� �����_����������_���� = ���� ���_��� ;
}

@�������
{
   @�����  ��������������1 =
           �������_���
	   ��_������� // ��������������� ���� �� ����� ������� ����� ��� ��������������
           but_do_������
	   �����_that
	   �����_whether
	   �����_��������������
	   �����_����������_����
	   ;
   @�����  ��������������1 =
           �������_���
	   ��_�������
	   but_do_������
           �����_that
	   �����_whether
	   �����_��������������
	   �����_����������_����
	   ;
}
@�������
{
   @�����2 _����_�������������� =
           �������_���
	   ��_�������
	   �����_��������������
	   ��������������_��
	   @00
	   ;
   @�����2 _����_�������������� =
           �������_���
	   ��_�������
	   �����_��������������
	   ��������������_��
	   @00
	   ;
}
@�������
{
   @��������� ��������������2 = ��������������1 _�������������� ;
   @��������� ��������������2 = ��������������1 _�������������� ;
}
@�������
{
   @�����     _�������������� = ��������������2 ��_������1 @00 ;
   @�����     _�������������� = ��������������2 ��_������1 @00 ;
}

// --------- ������, ������� ����� � ������ ����������
// PP:@0[to]=PP(�):@0[_]
// PP:@0[for]=PP(�):@0[���]
@�������
{  // He cut his finger. = �� ����� ��� ����� (test01)
   @��������� �����_1 = ��_���_� ;
   @��������� �����_1 = ��_���_�( %� ) ;
}
@�������
{  // They told me that I was too early. = ��� ������� ���, ��� � ��� ������� ���� (test25)
   @��������� �����_2 = ��_���_�       ;
   @��������� �����_2 = ��_���_�( %� ) ;
}
@�������
{  // She said to him that... = ��� ������� ���, ���... (test26)
   @��������� �����_3 = @0[to] ��_���_�       ;
   @��������� �����_3 =        ��_���_�( %� ) ;
}
@�������
{  // We heard from Jones all. = �� �������� �� ����� ��� (test38)
   @��������� �����_4 = �������_���   ��_���_� ;
   @��������� �����_4 = �������_���   ��_���_�( %� ) ;
}
@�������
{  // to search for manual pages. = 
   @��������� �����_4� = �������_��� ;
   @��������� �����_4� = �������_��� ;
}
@�������
{  // You must write her a letter. = �� ������ �������� �� ������ (test40)
   @��������� �����_5 = ��_���_�       ��_���_� ;
   @��������� �����_5 = ��_���_�( %� ) ��_���_�( %� ) ;
}
@�������
{  // We appointed him manager =  �� ��������� ��� ���������� (test16)
   @��������� �����_6 = ��_���_�       ��_���_� ;
   @��������� �����_6 = ��_���_�( %� ) ��_���_�( %� ) ;
}
// --------- ������, ������� ����� � ������ ���������� � ������
@�������
{  // I found the box empty. = � ����� ���� ���� ������ (test15)
   @��������� �����_7 = ��_���_�  ��_���� ;
   @��������� �����_7 = ��_���_�( &�����,&����,%�,&��� ) ��_����( ���,%�,%�,����� ) ;
}
@�������
{  // ------- ��� ������ ����� ������� �������� �������������� ��������������� ----
   @��������� �����_8 = ��_���_�  ������( &S1, %Form2 ) ;
   @��������� �����_8 = 
              ��_���_�( &�����,&����,%�,&��� )
	      ������( %���, %���������, @0, @0, @0, ���, %������, %� );
}
// --------- ������, ������� �� ����� � ������ ���������� ----------
@�������
{  // Do you think it odd that I should live by myself? (test07)
   // �� ������� ��� �������, ��� � ���� ���� 
   @��������� �����_21 = @0[it]      ��_�����_������� ;
   @��������� �����_21 = @0[���_���] ��_�����_������� ;
}
@�������
{  // You wouldn't want there to be war. = 
   // �� �� ������, ����� ���� ������ ����� (test05)
   @��������� �����_22 = 
              @0[there] @0[to] @0[be] _No ��_���_� ;
   @��������� �����_22 = 
              @0[�����] 
	      _No ������[����]( %���,%������,@0,�����,%�������,���,%�����,@0 )
	      ��_���_�( &�����, &�, %�, &��� );
}
@�������
{  // They proved him to be wrong. = ��� ������� ��� �� ��� ������� (test06)
   @��������� �����_23_1 =         ��_���_�                   to_inf2( &S1 ) ;
   @��������� �����_23_1 = @0[���] ��_���_�( &�, &�, %�, &� ) to_inf2( �����, ��� ) ;
}
@�������
{  //  I sent Tom to buy some stamps. = � ������ ���� ������ ����� (test23)
   @��������� �����_23_2 = ��_���_�               to_inf( &S1 ) ;
   @��������� �����_23_2 = ��_���_�( &�, %�, &� ) to_inf( �, � ) ;
}
@�������
{
   @��������� �����_23_3 = ��_���_�                         to_inf( &S1 ) ;
   @��������� �����_23_3 = ��_���_�( &�, %�, &� ) @0[�����] to_inf( �, � ) ;
}
@�������
{  // I do not want anyone to know. = � �� ���� ����� ���-������ ���� (test04)
   @��������� �����_23_4 =           ��_���_�                   to_inf2( &S1 ) ;
   @��������� �����_23_4 = @0[�����] ��_���_�( &�, &�, %�, &� ) to_inf2( �, � ) ;
}
@�������
{     
   @����� �����_� =
          �����_1    �����_2    �����_3    �����_4    �����_5    
	  �����_6    �����_7    �����_8    �����_4�
	  �����_21   �����_22   �����_23_1 �����_23_2 �����_23_3 �����_23_4
	  @00 ;
   @����� �����_� =
          �����_1    �����_2    �����_3    �����_4    �����_5    
	  �����_6    �����_7    �����_8    �����_4�
	  �����_21   �����_22   �����_23_1 �����_23_2 �����_23_3 �����_23_4
	  @00 ;
}
@�������
{
   @��������� ����� = �����_� _�������������� ;
   @��������� ����� = �����_� _�������������� ;
}
