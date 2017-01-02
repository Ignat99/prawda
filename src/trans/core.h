/*******************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//             Структуры, описывающие ядро перевода
//     Анисимов Д.В.                               сегодня
/*******************************************************************/

# ifndef CORE_H
# define CORE_H

# include "grammar.h"
# include "slowo3.h"

const int LWORD =  50 ; // длинна строки слова
const int LFRASA=1000 ; // длинна строки фразы

struct t_Core ;

// ---------------- чем закончился перевод фразы -----------------------
enum e_Result
{  E_NULL,   // программа не вызывалась вообще
   E_NO,     // не нужно транслировать
   E_OK,     // перевелось
   E_FIG,    // частично перевелось
   E_FIGNA,  // не перевелось
   E_ERROR   // внутренняя ошибка программы
};
// ------------------ представление для слова источника или приемника ---
struct t_Item
{  char     *Str1 ;  // ссылка на то место фразы, где начинается это слово
   char     *Str  ;  // отдельно это слово
   char      exist ; // существует ли это слово в словаре (0-нет 1-да)
   short     i_word ;
   short     z_str ; // где это слово начинается в исходной строке
   long      i_slowo ; // индекс в словаре переводов
};
DECLARE_LIST_TYPE( t_Item,t_ItemList );

// ------------------ узел дерева разбора фразы и -----------------------
// ------------------ узел дерева вариантов перевода --------------------
struct t_Tree
{  e_Type type ;        // тип узла
   long   up ;          // ссылка вверх
   long   down,n_down ; // ссылка вниз, и число таких ссылок
                        // если down=-1 то это конец дерева разбора
   short  i_word,       // с какого слова начинается
	  i_last_word,  // с какого слова начинается следующая
	  i_struct ;    // что за структура
   long   i_variant ;   // номер варианта структуры
   long   link ;        // ссылка источника на приемник или наоборот
   t_Form Form ;        // параметры сией структуры в соответствии с Struct->Param
   short  select ;      // выбранный вариант в TSELECT
   short  index ;       // номер слова в структуре t_Struct->Word
   char   i_dic ;       // 0-словарь перевода, 1-словарь догадок (Переводик)
   long   i_slowo ;     // индекс в словаре переводов
   short  i_slowo1 ;    // способ перевода
   short  i_var_irreg ; // индекс варианта вводного слова (new)
   short  n_slowo  ;    // число вариантов перевода
   char   Str [LWORD] ; // значение в нужной форме
   char   Str1[LWORD] ; // значение в начальной форме

	 t_Tree( void );
	~t_Tree( void );
   void  init( void );
   void  del( void );
   void  set( char _type, long up, short iw, short i_struct );
} ;
DECLARE_LIST_TYPE( t_Tree, t_TreeList );

// ----------- вариант реализации слова ----------------------
struct t_rWord  
{
   e_Type  type ;
   short   i_word,      // номер слова, с которого начинается
	   i_last_word, // номер слова, с которого начинается следующий
	   i_struct;    // номер структуры в t_Grammar->Trans[]
   long	   i_variant;   // номер варианта перевода 
   short   index ;      // индекс в массиве t_Struct->Word[]
   char    i_dic ;       // 0-словарь перевода, 1-словарь догадок (Переводик)
   long    i_slowo ;    // индекс в словаре переводов
   short   i_slowo1 ;   // способ перевода
   short   n_slowo ;    // число вариантов перевода (в словаре)

   t_rWord( );
};
DECLARE_LIST_TYPE( t_rWord,t_rWordList );

// ----------- вариант реализации структуры ------------------
struct t_rStruct
{
   e_Type  type ;
   short   i_word,       // с какого слова начинается
	   i_last_word,  // с какого слова начинается следующая
	   i_struct ;    // номер структуры в t_Grammar->Trans[]
   long    r_word ;      // ссылка на t_Core->rWord
   short   nr_word ;     // число r_word (new)
   char    i_dic ;       // 0-словарь перевода, 1-словарь догадок (Переводик)
   long    i_slowo ;     // индекс в словаре переводов
   short   i_slowo1 ;    // способ перевода
   short   i_var_irreg ; // индекс варианта вводного слова (new)
   t_Form  Form ;
   long    i_meaning ;   // смысл этой конструкции на языке оригинала
			 // (указывает на t_Core->Meaning)
   t_rStruct( );
};
DECLARE_LIST_TYPE( t_rStruct,t_rStructList );

// ---------- все возможные варианты реализации структуры ----
struct t_Variants
{  char  absend ;          // признак того, что эта конструкция в 
                           // этой позиции стоять не может
   short i_struct,i_word ;
   t_rStructList Variant ; // сами варианты разбора
};

// ---------- слова переведенной фразы (t_Antwort) ------------
struct t_aWord
{  short  i_word,       // номер слова в исходной фразе
          i_last_word ; // номер следующего слова в исходной фразе
   short  i_struct ;
   char   i_dic ;       // 0-словарь перевода, 1-словарь догадок (Переводик)
   long   i_slowo ;     // номер t_Record в главном словаре перевода
   short  n_slowo ;     // число вариантов перевода
   long   meaning ;
   t_Form Form ;
   char   Str[LWORD] ;
};
DECLARE_LIST_TYPE( t_aWord, t_aWordList );

// ----------- возможный перевод фразы ------------------------
class t_Antwort
{
// char         Str[LFRASA] ;  // собственно ответ
   t_String     Str ;
   t_TreeList   Tree1, Tree2 ; // структура фразы-источника и фразы приемника
   t_aWordList  aWord ;        // фраза-приемник
public:
                t_Antwort( );
   void         del( void );
   void         make( short i_word, short i_struct, long i_variant, t_Form Form );
   void         make_sou( short i_word, short i_struct, long i_Variant );
   char        *translate_simple( short Begin );
   char        *translate_simple1( short Begin );
   char        *str( void );
   t_aWordList *aword( void );
   t_TreeList  &tree1( void );
   t_TreeList  &tree2( void );
private:
   void  set_tree( t_Tree *T, t_rStruct  *pS, long i );
   void  make_dst( void );
   void  make_dst1( long up, long i2, t_Trans *Tr );
   void  make_dst2( long up, long i2, t_sStruct *S1, t_sStruct *S2 );
   void  make_dst3( long up, long i2 );
   void  make_to( long i_tree );
   void  param_from( void );
   void  param_transmit( void );
   void  param_transmit1( t_Table *Table, long i_Tree1, long i_Tree2 );
   void  param_to( void );
   void  make_vision( char Halb, long i_tree );
   void  insert_tag();
   void  print_antwort( long i_tree, char fword );
   void  add_word( t_Tree &T, char fword  );
   void  post_proc( void );
};

// ---------- главный класс перевода --------------------------
class t_Core
{
   short        n_struct,n_word ;
   t_ItemList   From ;         // структуры From
                               // ,Tag
   t_Variants  *Variants ;     // [n_struct*n_word] все присутствующие 
                               //  варианты разбора
   t_rWordList  rWord ;        // то, на что ссылается t_Variant->variant
   char        *Meaning ;      // смыслы t_Variant->meaning
   long         j_Meaning,
                l_Meaning ;
   t_Antwort    Antwort;       // варианты переведенной фразы
   char         f_Full ;       // признак, что существуют полные варианты
   char         Sign ;         // знак в конце предложения '.' или '?'
   e_Result     Result ;       // код завершения
   char        *Sou ;          // исходная фраза
   char         Sou0[LFRASA] ; // исходная фраза разбитая на слова
   char         Sou1[LFRASA] ; // исходная фраза разбитая "от тега до тега"
   long         j_VariantMem ; // счетчик Variants[i].Variant (для учета расхода памяти)
public:
   char         f_Figna ;      // печатать ли "#фигню"
   char         f_VarRep ;     // 1-значит не повторять варианты конструкций
public:
   t_Core();
  ~t_Core();
   void        init( short n_struct, short n_word );
   void        clr( );
   void        translate( char *Sou, char *Dst );
   void        translate_from( char *Sou );
   void        make_node( t_TreeList &Node, short i_word, long i_struct );
   char        make_from1( char *Sou );  // сделано public для t_NewWord1
   char        make_from2( void );
   t_Variants *variants( short i_word, long i_struct );
   t_rStruct  *variant(  short i_word, long i_struct, long i_variant );
   t_rStruct  *variant( t_rWord R );
   t_ItemList &from( void );
   t_ItemList &tag( void );
   t_rWord    &rword( long i );
   t_Antwort  &antwort( );   
   long        n_antwort( );
   long        n_reg_antwort( );
   char        f_full();
   char        sign( void );
   long        struct_of_word( short i_word );
   short       fn_word();
   short       fn_struct();
   e_Result    result();
   char       *sou();
   char       *get_meaning( t_rStruct *V );

private:
   void        universe( short i_word, long i_struct );
   void        next_chance( char *Dst );
   void        set_meaning( t_rStruct *V, char *Str );
   void        no_brakets( char *Str, char *Str1 );
   void        add_from( short i_word, long i_struct, t_Form Form,
                         char i_dic, long i_slowo, short i1_slowo );
   char        atom( short i_word, t_Word &Word );
   void        real_param_up( t_rStruct *V );
   char        Compare_Param( t_rStruct *Variant, t_Word *Word );
   char        select2_exist( short i_word, short i_struct, short i_last_word );
   void        mem_size_test( );
   void        double_variants( char *Name );

friend class t_Slowo3 ;
};

// ----------- дерево вариантов словообразования --------------
struct t_mTree
{  long  up ;
   char  type ;       // тип шага 0-синтетический 1-аналитический
   short type_link ;  // ссылка на массив Grammar->Step
   char  Str[LWORD] ;
   long  i_dogadka,       // индекс в словаре догадок
         i_last_dogadka ; // 
   
       t_mTree();
   void init();
};
DECLARE_LIST_TYPE( t_mTree, t_mTreeList );

// ------ структура для аналитического перевода родственных слов ------
struct t_Struct1 : public t_Struct
{
   long  i_semantika ;
   short i_struct_from,
         i_struct_to ;
};
DECLARE_LIST_TYPE( t_Struct1,t_Struct1List );

// ----------- жизнь внутри слов ---------------------------
class t_Core1
{
   public:
   char        *Str0 ;  // исходное слово
   t_mTreeList  Tree ;  // дерево вариантов
   t_longList   Antwort ; // массив ответов i_record
   
   public:
   void  make_dogadka( char *_Str0, t_longList &_Antwort );
   void  make_dogadka( char *Str0, t_Algorithm *A, long z_Tree );
   void  reverse_path( long z_Tree, long i_semantika, short i_struct );
   void  make_step( char *Str, char *Str1, t_Shift *Step, char Flag );
   void  insert( t_Struct1 &Result, t_Struct &Up, t_Struct1 &Down, long i_up );
   void  mk_struct( t_Struct1 &Struct, long i_semantika );
   void  set_word_semantika( t_Struct1 &S1, short i_main, long i_semantika );
   short grammar_use( t_Struct &S1 );
   short meaning_use( t_Struct &S1 );
   void  struct_print( t_String &STmp, t_Struct1 &S, short i_struct0 );
   char  struct_cmp( t_Struct &S1, t_Struct1 &S2 );
};

extern t_Core        Core ;
extern t_Core1       Core1 ;
extern t_Slowo3     *Perevod ;
extern t_Slowo3      Dogadka ;

char read_grammar  ( char *Dir, t_DirTrans *DT );
char reread_grammar( char *Dir, t_DirTrans *DT );
void set_formats( void );
char *filename( char *Path, char *File );
void  set_main_dict( char f_kontext );
# endif
