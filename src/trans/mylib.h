/***************************************************************************/
//              "Меркурий"-"Правда" - open source переводчик
//          распространяется в соответсвии с лицензией GNU v 2.0
//
//              Всякие мелкие, но очень полезные штучки
//       Анисимов Д.В.                                    сегодня
/***************************************************************************/
# ifndef MYLIB_H
# define MYLIB_H

# include <stdio.h>
# include <malloc.h>
# include <stdlib.h>


# define Err_Memory -2
# define Err_Fopen  -3
# define Err_Fread  -4
# define Err_Fwrite -5
# define Err_Data   -6
# define Err_List   -7

# define uchar       unsigned char
# define ushort      unsigned short
# define USPACE      0x2000        // юникодный пробел
# define UTAB        0x0900        // юникодная табуляция
# define UNEWLINE    0x0A00        // юникодная новая строка '\n'
# define UCARRET     0x0D00        // юникодная '\r'

# define UPOINT      0x2E00        // точка
# define UCOMMA      0x2C00        // запятая
# define UQUEST      0x3F00        // вопросительный знак
# define UEXCLAM     0x2100        // восклицательный знак
# define UCOLON      0x3A00        // двоеточие

# define    max(a,b)      (((a) > (b)) ? (a) : (b))
# define    min(a,b)      (((a) < (b)) ? (a) : (b))
# define    Free( a )     if( a!=NULL ) {  free(a);   a=NULL ;  }
# define    Fclose( f )   if( f!=NULL ) {  fclose(f); f=NULL ;  }
# define    is_space(c)   ((c)==' ' || (c)=='\t' || (c)=='\n' || (c)=='\r')
# define    is_space16(c) ((c)     ==USPACE || (c)     ==UTAB || (c)     ==UNEWLINE || (c)     ==UCARRET)
# define    is_space32(c) ((c.Char)==USPACE || (c.Char)==UTAB || (c.Char)==UNEWLINE || (c.Char)==UCARRET)

# define    is_point32(c) ((c.Char)==UPOINT || (c.Char)==UQUEST || (c.Char)==UEXCLAM || (c.Char)==UCOLON)

void  *Calloc( long N, long S );
void  *Malloc( long N, long S );
void  *Realloc( void *M, long L );

FILE  *Fopen( char *Name, char *a );
void   Fread( void *M, long S, long N, FILE *File );
void   Fwrite( void *M, long S, long N, FILE *File );
void   Fseek( FILE *fw, long Offset, long Mode );
long   FileLength( FILE *fw );

short  Strcmp( char *Str1, char *Str2 );
short  Strcmp1( char *Str1, char *Str2 );
short  Strncmp( char *Str1, char *Str2, short L );
void   Strcpy( char *Str1, char *Str2, int Lmax );
void   Strcat( char *Str1, char *Str2, int Lmax );
char  *emptystr( short n );
uchar *SortChar( void );
uchar *Koi_to_Win( void );
uchar *Win_to_Koi( void );
long   remark( char *Mass, long l_Max );
long   file_to_mass( char *File, char **_Mass );

void   error_set( const char *s );
void   error_add( const char *s );
char  *error_get( void );
void   provokator( );

struct t_String
{
    char *Str ;
    short Length ;

    t_String();
    t_String( const char *Str1 );
    t_String( const t_String &S );
   ~t_String();
    void       init( );
    void       del( );
    t_String&  operator =  ( const char *Str1 );
    t_String&  operator =  ( const t_String &Str1 );
    void       operator += ( char c );
    void       operator += ( const char *Str1 );
    void       operator += ( const t_String &Str1 );
//    t_String&  operator +  ( char *Str );
//    t_String&  operator +  ( t_String &Str1 );
    char      *c_str();
    int        length();
    char       operator [] (int i);
    void       resize( int n );
};

t_String operator + ( const t_String &Str1, const char *Str2 );
t_String operator + ( const t_String &Str1, const t_String &Str2 );

# endif
