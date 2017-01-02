/***************************************************************************/
//              "��������"-"������" - open source ����������
//          ���������������� � ����������� � ��������� GNU v 2.0
//
//                 ������� ��������� ���� � ��������������
//       �������� �.�.                                    �������
/***************************************************************************/
# ifndef SLOWO3_H
# define SLOWO3_H

# ifdef __WIN__
   # include <dir.h>
#else
   # include <unistd.h>
#endif
# include <sys/types.h>
# include <sys/stat.h>

# include "mylib.h"
# include "grammar.h"

struct t_sForm
{
   unsigned int v0 : 4 ;
   unsigned int v1 : 4 ;
   unsigned int v2 : 4 ;
   unsigned int v3 : 4 ;
   unsigned int v4 : 4 ;
   unsigned int v5 : 4 ;
   unsigned int v6 : 4 ;
   unsigned int v7 : 4 ;

   void init( );
   t_sForm();
   void operator = ( t_Form &F );
   t_Form form( );
};

// ------- ����� ��������� ----------------------------
struct t_sWord
{  long  sy ;        // ������ �� �����       t_Slowo3->Mass
   char *str ;       // ������ �� �����       t_Slowo3->Mass
   short i_struct ;  // ������ ��������� �    t_Grammar->Trans
   short order  ;    // ��������� ������ ����<1>
   t_sForm Param ;    // ����������� ���������
} ;
DECLARE_LIST_TYPE( t_sWord,t_sWordList );

// -------- ��������� ��������� ��� �������� ----------
struct t_sStruct
{  long     sy_word ;  // ������ ������� ����� ��������� � t_Slowo3->Word
		       // ���� �� ����� ��������� ����� n_From ��� ����������
   t_sWord *Word ;     // ������ �� �� �� �����
   short    n_Word ;   // ����� ���� � ���� ���������
   short    i_struct ; // ������ ��������� �    t_Grammar->Trans
   t_sForm  Param ;    // ����������� ���������
   long     i_relation ;// ������ ������� �������� ���������� t_Slowo3->Relation
} ;
DECLARE_LIST_TYPE( t_sStruct,t_sStructList );

// -------- ���� ������ �������� (�������) ------------------------
struct t_sRecord
{  long       sy_struct ;    // ������ ������ ��������� � t_Slowo3->Struct
   t_sStruct *Struct ;
   short      n_struct ;     // ����� ����� �������� (������� ���)
   short      i_dict ;       // ������ �������
};

// -------- ���� ������ �������� (����������) ---------------------
struct t_sRecord1
{
   char           *Mass ;    // ��� �������� ������
   char            f_Malloc ;// ������� ����, ��� ������ ��������� ��������
   t_RelationList  Relation ;
   t_sWordList     Word ;
   t_sStructList   Struct ;
   
	 t_sRecord1();
	~t_sRecord1();
   void  clr();
   char *parse2( char *Str, t_Format1 *Format );
   void  parse1( char *Str, t_Format1 *Format, char _f_Malloc );
   void  make_relation2( char fdst, t_sStruct *S, long i_word, char *Str );
};

// --------- ������� ������ ������ ------------------------
struct t_sTree
{  long  up,down,n_down ;
   long  first,last ;   // ������ ������ � ��������� (������������) Record, 
                        // �������� � ��� ���������
   short rang ;         // ����� ����� � ������� ���������
   char  empty ;        // �������, � ��������� ������ ������������
                        // ���������� �����
   char  unused ;
} ;

// ---------- ������� ��������� ---------------------------------------
class t_Slowo3
{
private:
   char        FileName[40] ;
   char        i_dic ;   // 0-������� ���������, 1-������� �������
   t_Format1  *Format ;  // ������ �������
   uchar      *Mass ;    // ������ ����, ������ ���� "S[word]=S[�����]"
   t_sWord    *Word ;    // ������ �� ���� ������ ���� "S<1>(�)[������]"
   t_sStruct  *Struct ;  // ������ �� ���� ������ ���� "GS:@0[a hot]S<1>[dog];"
   t_sRecord  *Record ;  // ������ Struct, � ������� ���������� ���������
			 // ������ ���� S[word]=S[�����]
   t_sTree    *Tree ;    // ������ ��� ������ ��������
   t_Relation *Relation ;// ������� �������� ����������
   
   long       n_Mass    ,l_Mass ;    // ������ ������� Mass
   long       n_Word    ,l_Word ;
   long       n_Struct  ,l_Struct ;  // ����� � ������ ������� ��������
   long       n_Record  ,l_Record ;  // ����� -----// -------  �������
   long       n_Tree    ,l_Tree ;    // ����� -----// -------  Tree
   long       n_Relation,l_Relation; // ����� -----// -------  Relation

private:
   t_longList Antwort ;  // ������ �������
public:
   char       Type[256] ;
   time_t     mtime ;// ��������� ����� ����������� ��������

public:
	      t_Slowo3( char i_dic );
   void       clr( void );
   void       del( void );
   void       read( char *Dir, t_Grammar *Gr );
   void       read( char *File, t_Format1 *Format );
   char       read_bin( char *File );
   void       write_bin( char *File );
   void       make( char *_Mass, t_Format1 *Format );
   void       make_tree( void );
   void       s_universe( short i_word, short i_struct );
   void       find( char *From, short i_struct, t_longList *Antwort );
   char      *translate_word_i( char *From, long  i_slowo,  short i_slowo1=0 );
   char      *translate_word_s( char *From, short i_struct, short i_slowo1=0 );
   short      part( char *From, short *i_Part );
   t_Form     struct_param( long i_slowo, long i_variant=0 );
   t_Form     word_param( long i_slowo, long i_variant, long i_word );
   t_Format1       *format( void );
   void             set_format( t_Format1 *F );
   t_sRecord       *record( long i_record );   
   long             n_record( void );
   char             is_atom( long i_record );
   t_sTree         *tree( long i_tree );
  inline t_sStruct *sstruct( long i_struct );
   t_sStruct       *get_from( long i_slowo );
   t_sStruct       *get_to( long i_slowo, long i_slowo1=0 );
   t_RelationList1  get_relation( long i_slowo, long i_variant );
   short            get_n_perevod( long i_slowo );
   char            *word_src(  long i_tree );
   char            *word_src1( long i_slowo );
   void             print( char *File );
   long             print_mass( char **Mass );
   void             print_record( FILE *fw, long i_record );
   void             print_param( FILE *fw, char to, t_sStruct *S, short i_word );
   void             print_record( t_String &STmp, long i_record );
   void             print_struct( t_String &STmp, long i_record, long i_struct );
   void             print_param(  t_String &STmp, char to, t_sStruct *S, short i_word );
   t_String         get_strerr();

//private:
   void      read0( char *File );
   void      make0( char *Mass );
   void      parse( void );
   char      is_struct( long z_begin );
static void  make_relation1( char fdst, t_sStruct *S, char *Str );
   void      make_relation2( char fdst, t_sStruct *S, long i_word, char *Str, long &j_Relation );
   void      sort0( void );
   void      split_unorder( void );
   void      s_universe1( short i_word, long i_tree );
   void      s_universe2( char *From, short i_word, long i_tree );
   char      find_bin(  char *From1, long i_tree );
   char      find_bin1(  char *From1, long i_tree );
   long      find_mutt( long i_tree );
   long      root_of_struct( short i_struct );
// long      find_insert( t_sRecord &R );
friend class t_Core ;
};

# endif


