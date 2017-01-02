# ifndef KOI9_H
# define KOI9_H
# include <limits.h>
# include <sys/types.h>

# define uchar  unsigned char
# define ushort unsigned short

enum e_Latin
{  KOI8  =0,
   LATIN1=1,
   LATIN2=2,
   LATIN3=3,
   LATIN4=4,
   UCS2BE=5, // Эту кодировку мы используем внутри себя
   UCS2LE=6,
   CP1251=7
};

// ------------------ символ Юникодного текста с дополнительными флагами -----
struct t_myChar
{  ushort Char ; // сам символ
   ushort Flag ; // флаги
   t_myChar operator = ( char C );
   t_myChar operator = ( ushort C );
   t_myChar operator = ( int C );
   char     operator ==( char C );
   char     operator !=( char C );
   char     operator ==( ushort C );
   char     operator ==( int C );
   char     operator !=( t_myChar C );
};

void   init_unicod( );
long   koi9_to_Unicod ( char *Koi9,    ushort   *Unicod, long LKoi9=LONG_MAX, long LUnicod=LONG_MAX );
long   koi9_to_myChar ( char *Koi9,    t_myChar *myChar, long LKoi9=LONG_MAX, long LUnicod=LONG_MAX );
long   Unicod_to_koi9( ushort *Unicode, uchar *Koi8, long LUnicode=LONG_MAX, long LKoi8=LONG_MAX );
long   myChar_to_koi9( t_myChar *Char, uchar *Koi8, long LChat=LONG_MAX, long LKoi8=LONG_MAX );
ushort koi9_to_Unicod ( uchar Koi8 );
ushort   Unicode_toUp  ( ushort Uc );
char     Unicod_to_koi8( ushort Uc );
t_myChar myChar_toUp( t_myChar C );

long  strlen32   ( t_myChar *str );
void  strcpy32   ( t_myChar *Str1, t_myChar *Str );
void  strncpy32  ( t_myChar *Str1, t_myChar *Str,  long L );
int   strncmp32  ( t_myChar *Str1, t_myChar *Str2, long L );
int   strncmp32_8( t_myChar *Str1, char     *Str2, long L );

# endif
