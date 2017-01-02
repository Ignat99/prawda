/****************************************************************************


*****************************************************************************/

# include <string.h>
# include <stdio.h>
# include <malloc.h>
# include <X11/Xlib.h>

# include <iconv.h>
# include "mylib.h"
# include "koi9.h"

struct t_Koi9Kod
{
   char   *from ;
   ushort  to ;
};

ushort ToUnicode[5][256] ;
uchar  UnicodeToKoi[0x0ffff];

// ------------- Koi9(латинская часть)->Unicode -----------
t_Koi9Kod Koi9_Unicode[]=
{
   "\\`A"  , 0xC000, "\\`a"  , 0xE000,
   "\\'A"  , 0xC100, "\\'a"  , 0xE100,
   "\\^A"  , 0xC200, "\\^a"  , 0xE200,
   "\\~A"  , 0xC300, "\\~a"  , 0xE300,
   "\\\"A" , 0xC400, "\\\"a" , 0xE400,
   "\\rA"  , 0xC500, "\\ra"  , 0xE500,

   "\\,C"  , 0xC700, "\\,c"  , 0xE700,

   "\\`E"  , 0xC800, "\\`e"  , 0xE800,
   "\\'E"  , 0xC900, "\\'e"  , 0xE900,
   "\\^E"  , 0xCA00, "\\^e"  , 0xEA00,
   "\\\"E" , 0xCB00, "\\\"e" , 0xEB00,

   "\\`I"  , 0xCC00, "\\`i"  , 0xEC00,
   "\\'I"  , 0xCD00, "\\'i"  , 0xED00,
   "\\^I"  , 0xCE00, "\\^i"  , 0xEE00,
   "\\\"I" , 0xCF00, "\\\"i" , 0xEF00,

   "\\~N"  , 0xD100, "\\~n"  , 0xF100,
   "\\`O"  , 0xD200, "\\`o"  , 0xF200,

   "\\'O"  , 0xD300, "\\'o"  , 0xF300,
   "\\^O"  , 0xD400, "\\^o"  , 0xF400,
   "\\~O"  , 0xD500, "\\~o"  , 0xF500,
   "\\\"O" , 0xD600, "\\\"o" , 0xF600,

   "\\`U"  , 0xD900, "\\`u"  , 0xF900,
   "\\'U"  , 0xDA00, "\\'u"  , 0xFA00,
   "\\^U"  , 0xDB00, "\\^u"  , 0xFB00,
   "\\\"U" , 0xDC00, "\\\"u" , 0xFC00,
   "\\'Y"  , 0xDD00, "\\'y"  , 0xFD00,
                     "\\\"y" , 0xFF00,

   "\\=A"  , 0x0001, "\\=a"  , 0x0101,
   "\\uA"  , 0x0201, "\\ua"  , 0x0301,
   "\\'C"  , 0x0601, "\\'c"  , 0x0701,
   "\\^C"  , 0x0801, "\\^c"  , 0x0901,
   "\\.C"  , 0x1001, "\\.c"  , 0x1101,
   "\\'C"  , 0x0601, "\\'c"  , 0x0701,
   "\\=E"  , 0x1201, "\\=e"  , 0x1301,
   "\\uE"  , 0x1401, "\\ue"  , 0x1501,
   "\\.E"  , 0x1601, "\\.e"  , 0x1701,
   "\\^G"  , 0x1C01, "\\^g"  , 0x1D01,
   "\\uG"  , 0x1E01, "\\ug"  , 0x1E01,
   "\\.G"  , 0x2001, "\\.g"  , 0x2101,
   "\\^H"  , 0x2401, "\\^h"  , 0x2501,
   "\\=H"  , 0x2601, "\\=h"  , 0x2701,
   "\\~I"  , 0x2801, "\\~i"  , 0x2901,
   "\\=I"  , 0x2A01, "\\=i"  , 0x2B01,
   "\\uI"  , 0x2C01, "\\ui"  , 0x2D01,
   "\\.I"  , 0x3001, "\\.i"  , 0x3101,
   "\\^J"  , 0x3401, "\\^j"  , 0x3501,
   "\\cK"  , 0x3601, "\\ck"  , 0x3701,
   "\\'L"  , 0x3901, "\\'l"  , 0x3A01,
   "\\cL"  , 0x3B01, "\\cl"  , 0x3C01,
   "\\'N"  , 0x4301, "\\'n"  , 0x4401,
   "\\cN"  , 0x4501, "\\cn"  , 0x4601,
   "\\=O"  , 0x4C01, "\\=o"  , 0x4D01,
   "\\uO"  , 0x4E01, "\\uo"  , 0x4F01,
   "\\HO"  , 0x5001, "\\Ho"  , 0x5101,
   "\\'R"  , 0x5401, "\\'r"  , 0x5501,
   "\\cR"  , 0x5601, "\\cr"  , 0x5701,
   "\\'S"  , 0x5A01, "\\'s"  , 0x5B01,
   "\\^S"  , 0x5C01, "\\^s"  , 0x5D01,
   "\\cS"  , 0x5E01, "\\cs"  , 0x5F01,
   "\\cT"  , 0x6201, "\\ct"  , 0x6301,
   "\\~U"  , 0x6801, "\\~u"  , 0x6901,
   "\\=U"  , 0x6A01, "\\=u"  , 0x6B01,
   "\\uU"  , 0x6C01, "\\uu"  , 0x6D01,
   "\\rU"  , 0x6E01, "\\ru"  , 0x6F01,
   "\\HU"  , 0x7001, "\\Hu"  , 0x7101,
   "\\^W"  , 0x7401, "\\^w"  , 0x7501,
   "\\^Y"  , 0x7601, "\\^y"  , 0x7701,
   "\\\"Y" , 0x7801,
   "\\'Z"  , 0x7901, "\\'z"  , 0x7A01,
   "\\.Z"  , 0x7B01, "\\.z"  , 0x7C01,
   NULL
};
uchar Koi8_ToUp[256]=
{
   0x0  , 0x1  , 0x2  , 0x3  , 0x4  , 0x5  , 0x6  , 0x7  ,
   0x8  , 0x9  , 0xa  , 0xb  , 0xc  , 0xd  , 0xe  , 0xf  ,
   0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 ,
   0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f ,
   0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 ,
   0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f ,
   0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 ,
   0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f ,
   //'@'    'A'    'B'    'C'    'D'    'E'    'F'    'G'
   0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 ,
   //'H'    'I'    'J'    'K'    'L'    'M'    'N'    'O'
   0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f ,
   //'P'    'Q'    'R'    'S'    'T'    'U'    'V'    'W'
   0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 ,
   //'X'    'Y'    'Z'
   0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f ,
   //'`'    'a'    'b'    'c'    'd'    'e'    'f'    'g'
   0x60 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 ,
   //'h'    'i'    'j'    'k'    'l'    'm'    'n'    'o'
   0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f ,
   //'p'    'q'    'r'    's'    't'    'u'    'v'    'w'
   0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 ,
   //'x'    'y'    'z'
   0x58 , 0x59 , 0x5a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f ,

   0x80 , 0x81 , 0x82 , 0x83 , 0x84 , 0x85 , 0x86 , 0x87 ,
   0x88 , 0x89 , 0x8a , 0x8b , 0x8c , 0x8d , 0x8e , 0x8f ,
   0x90 , 0x91 , 0x92 , 0x93 , 0x94 , 0x95 , 0x96 , 0x97 ,
   0x98 , 0x99 , 0x9a , 0x9b , 0x9c , 0x9d , 0x9e , 0x9f ,
   0xa0 , 0xa1 , 0xa2 , 0xb3 , 0xa4 , 0xa5 , 0xa6 , 0xa7 ,
   0xa8 , 0xa9 , 0xaa , 0xab , 0xac , 0xad , 0xae , 0xaf ,
   0xb0 , 0xb1 , 0xb2 , 0xb3 , 0xb4 , 0xb5 , 0xb6 , 0xb7 ,
   0xb8 , 0xb9 , 0xba , 0xbb , 0xbc , 0xbd , 0xbe , 0xbf ,
   //'ю'    'а'    'б'    'ц'    'д'    'е'    'ф'    'г'
   0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 ,
   //'х'    'и'    'й'    'к'    'л'    'м'    'н'    'о'
   0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 0xed , 0xee , 0xef ,
   //'п'    'я'    'р'    'с'    'т'    'у'    'ж'    'в'
   0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 ,
   //'ь'    'ы'    'з'    'ш'    'э'    'щ'    'ч'    'ъ'
   0xf8 , 0xf9 , 0xfa , 0xfb , 0xfc , 0xfd , 0xfe , 0xff ,
   //'Ю'    'А'    'Б'    'Ц'    'Д'    'Е'    'Ф'    'Г'
   0xe0 , 0xe1 , 0xe2 , 0xe3 , 0xe4 , 0xe5 , 0xe6 , 0xe7 ,
   //'Х'    'И'    'Й'    'К'    'Л'    'М'    'Н'    'О'
   0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 0xed , 0xee , 0xef ,
   //'П'    'Я'    'Р'    'С'    'Т'    'У'    'Ж'    'В'
   0xf0 , 0xf1 , 0xf2 , 0xf3 , 0xf4 , 0xf5 , 0xf6 , 0xf7 ,
   //'Ь'    'Ы'    'З'    'Ш'    'Э'    'Щ'    'Ч'    'Ъ'
   0xf8 , 0xf9 , 0xfa , 0xfb , 0xfc , 0xfd , 0xfe , 0xff
};
uchar Latin1_ToUp[256]=
{
   0x0  , 0x1  , 0x2  , 0x3  , 0x4  , 0x5  , 0x6  , 0x7  ,
   0x8  , 0x9  , 0xa  , 0xb  , 0xc  , 0xd  , 0xe  , 0xf  ,
   0x10 , 0x11 , 0x12 , 0x13 , 0x14 , 0x15 , 0x16 , 0x17 ,
   0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f ,
   0x20 , 0x21 , 0x22 , 0x23 , 0x24 , 0x25 , 0x26 , 0x27 ,
   0x28 , 0x29 , 0x2a , 0x2b , 0x2c , 0x2d , 0x2e , 0x2f ,
   0x30 , 0x31 , 0x32 , 0x33 , 0x34 , 0x35 , 0x36 , 0x37 ,
   0x38 , 0x39 , 0x3a , 0x3b , 0x3c , 0x3d , 0x3e , 0x3f ,
   0x40 , 0x41 , 0x42 , 0x43 , 0x44 , 0x45 , 0x46 , 0x47 ,
   0x48 , 0x49 , 0x4a , 0x4b , 0x4c , 0x4d , 0x4e , 0x4f ,
   0x50 , 0x51 , 0x52 , 0x53 , 0x54 , 0x55 , 0x56 , 0x57 ,
   0x58 , 0x59 , 0x5a , 0x5b , 0x5c , 0x5d , 0x5e , 0x5f ,
   0x60 , 0x61 , 0x62 , 0x63 , 0x64 , 0x65 , 0x66 , 0x67 ,
   0x68 , 0x69 , 0x6a , 0x6b , 0x6c , 0x6d , 0x6e , 0x6f ,
   0x70 , 0x71 , 0x72 , 0x73 , 0x74 , 0x75 , 0x76 , 0x77 ,
   0x78 , 0x79 , 0x7a , 0x7b , 0x7c , 0x7d , 0x7e , 0x7f ,
   0x80 , 0x81 , 0x82 , 0x83 , 0x84 , 0x85 , 0x86 , 0x87 ,
   0x88 , 0x89 , 0x8a , 0x8b , 0x8c , 0x8d , 0x8e , 0x8f ,
   0x90 , 0x91 , 0x92 , 0x93 , 0x94 , 0x95 , 0x96 , 0x97 ,
   0x98 , 0x99 , 0x9a , 0x9b , 0x9c , 0x9d , 0x9e , 0x9f ,
   0xa0 , 0xa1 , 0xa2 , 0xa3 , 0xa4 , 0xa5 , 0xa6 , 0xa7 ,
   0xa8 , 0xa9 , 0xaa , 0xab , 0xac , 0xad , 0xae , 0xaf ,
   0xb0 , 0xb1 , 0xb2 , 0xb3 , 0xb4 , 0xb5 , 0xb6 , 0xb7 ,
   0xb8 , 0xb9 , 0xba , 0xbb , 0xbc , 0xbd , 0xbe , 0xbf ,
   0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 ,
   0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce , 0xcf ,
   0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 ,
   0xd8 , 0xd9 , 0xda , 0xdb , 0xdc , 0xdd , 0xde , 0xdf ,
   0xc0 , 0xc1 , 0xc2 , 0xc3 , 0xc4 , 0xc5 , 0xc6 , 0xc7 ,
   0xc8 , 0xc9 , 0xca , 0xcb , 0xcc , 0xcd , 0xce , 0xcf ,
   0xd0 , 0xd1 , 0xd2 , 0xd3 , 0xd4 , 0xd5 , 0xd6 , 0xd7 ,
   0xe8 , 0xe9 , 0xea , 0xeb , 0xec , 0xed , 0xee , 0xef ,
};
uchar *Latin2_ToUp=&Latin1_ToUp[0] ;
uchar *Latin3_ToUp=&Latin1_ToUp[0] ;
uchar *Latin4_ToUp=&Latin1_ToUp[0] ;

ushort  *UnicodeToUp ;
ushort n_UnicodeToUp ;

/***************************************************************************/
//                 Преобразование в большие буквы
/***************************************************************************/
unsigned char Latin_toUp( e_Latin Page, uchar c )
{
   switch( Page )
   {
      case LATIN1 :
      case LATIN2 :
      case LATIN3 :
      case LATIN4 : return Latin1_ToUp[(uchar)c] ;
      case KOI8   : return Koi8_ToUp  [(uchar)c] ;
      default :  printf("\nError in Latin_to_Unicod!!!\n"); return c ;
   }
}
/***************************************************************************/
ushort Unicode_toUp( ushort Uc )
{
   if( n_UnicodeToUp<=Uc )
       return Uc ;
   return UnicodeToUp[Uc] ;
}
/***************************************************************************/
void make_UnicodeToUp( )
{  short i,i1,up ;
   // -------- определить размер ---------------------
   n_UnicodeToUp=0 ;
   for( i=0 ; i<5 ; i++ )
      for( i1=0 ; i1<256 ; i1++ )
         n_UnicodeToUp=max( n_UnicodeToUp,ToUnicode[i][i1] );

   UnicodeToUp=(ushort *)Calloc( n_UnicodeToUp+1,sizeof(ushort) );
   // -------- заполнить таблицу ---------------------
   for( i=0 ; i<5 ; i++ )
   {  for( i1=0 ; i1<256 ; i1++ )
      {  e_Latin Page ;
         switch( i )
	 {   case 0 : Page=KOI8   ; break ;
	     case 1 : Page=LATIN1 ; break ;
	     case 2 : Page=LATIN2 ; break ;
	     case 3 : Page=LATIN3 ; break ;
	     case 4 : Page=LATIN4 ; break ;
	 }
         up=(unsigned char)Latin_toUp( Page, i1 );
         UnicodeToUp[ToUnicode[i][i1]]=ToUnicode[i][up];
      }
   }
}
/***************************************************************************/
//           Преобразование из одной кодовой страницы в другую
/***************************************************************************/
void  koi9_to_byte( t_Koi9Kod *Table, char *From, char *To )
{  long i,i1,j ;

   for( j=i=0 ; From[i]!=0 ; i++ )
   {  if( From[i]=='\\' )
      {
          for( i1=0 ; Table[i1].from!=NULL ; i1++ )
	     if( 0==strncmp( From+i,Table[i1].from,3 ) )
	     {  To[j++]=Table[i1].to ; i+=2 ; goto M1 ;  }
          To[j++]=From[i] ;
	  M1 :;
      }
      else
      {  To[j++]=From[i] ;
      }
   }
   To[j++]=0 ;
}
/***************************************************************************/
ushort koi8_to_Unicod( uchar Koi8 )
{
   return ToUnicode[KOI8][Koi8] ;
}
/***************************************************************************/
char   Unicod_to_koi8( ushort Uc )
{
    return UnicodeToKoi[ Uc ] ;
}
/***************************************************************************/
//                        новая функция
/***************************************************************************/
long   koi9_to_Unicod( char *Koi8, ushort *Unicode, long LKoi8, long LUnicode )
{  long  i,i1,j ;
   uchar c ;

   for( j=i=0 ; i<LKoi8 ; i++ )
   {
      c=Koi8[i] ;
      if( c==0 )
      {   Unicode[j]=0 ; break ;  }
      if( c=='\\' )
      {   for( i1=0 ; Koi9_Unicode[i1].from!=NULL ; i1++ )
             if( 0==strncmp( (const char *)(Koi8+i),Koi9_Unicode[i1].from,3 ) )
	     {   Unicode[j++]=Koi9_Unicode[i1].to ; i+=2 ; goto M1 ; }
      }
      if( c<128 ) Unicode[j++]=c<<8 ;
          else    Unicode[j++]=ToUnicode[KOI8][c] ;
      M1:;
   }
   return j ;
}
/***************************************************************************/
//                        новая функция
/***************************************************************************/
long   koi9_to_myChar( char *Koi9, t_myChar *myChar, long LKoi9, long LmyChar )
{  long  i,i1,j ;
   uchar c ;

   for( j=i=0 ; i<LKoi9 ; i++ )
   {  
      c=Koi9[i] ;
      if( c==0 )
      {   myChar[j]=0 ; break ;  }
      if( c=='\\' )
      {   for( i1=0 ; Koi9_Unicode[i1].from!=NULL ; i1++ )
             if( 0==strncmp( (const char *)(Koi9+i),Koi9_Unicode[i1].from,3 ) )
	     {   myChar[j++]=Koi9_Unicode[i1].to ; i+=2 ; goto M1 ; }
      }
      if( c<128 ) myChar[j++]=(ushort)(c<<8) ;
          else    myChar[j++]=(ushort)(ToUnicode[KOI8][c]) ;
      M1:;
   }
   return j ;
}
/***************************************************************************/
//                        новая функция
/***************************************************************************/
long   Unicod_to_koi9( ushort *Unicode, uchar *Koi8, long LUnicode, long LKoi8 )
{  long i,i1,j ;
   ushort c,f ;

   for( j=i=0 ; i<LUnicode && j<LKoi8 ; i++ )
   {
      c=Unicode[i] ;
      if( c==0 ) break ;
      f=c&0x00ff ;
      if( f==0 )
      {   Koi8[j++]=c>>8 ; goto M1 ;  }
      if( f==0x4 )
      {   Koi8[j++]=UnicodeToKoi[c] ; goto M1 ;  }
      if( 0x1<f && f<0x4 )
      {   for( i1=0 ; Koi9_Unicode[i1].from!=NULL ; i1++ )
             if( c==Koi9_Unicode[i1].to )
	     {   Koi8[j++]=Koi9_Unicode[i1].from[0] ;
	         Koi8[j++]=Koi9_Unicode[i1].from[1] ;
		 Koi8[j++]=Koi9_Unicode[i1].from[2] ;
	         goto M1 ;
	     }
      }
      Koi8[j++]=' ' ;
      M1:;
   }
   Koi8[j]=0 ;
   return j ;
}
/***************************************************************************/
long   myChar_to_koi9( t_myChar *myChar, uchar *Koi8, long LmyChar, long LKoi8 )
{  long i,i1,j ;
   ushort c,f ;

   for( j=i=0 ; i<LmyChar && j<LKoi8 ; i++ )
   {  
      c=myChar[i].Char ;
      if( c==0 ) break ;
      f=c&0x00ff ;
      if( f==0 )
      {   Koi8[j++]=c>>8 ; goto M1 ;  }
      if( f==0x4 )
      {   Koi8[j++]=UnicodeToKoi[c] ; goto M1 ;  }
      if( 0x1<f && f<0x4 )
      {   for( i1=0 ; Koi9_Unicode[i1].from!=NULL ; i1++ )
             if( c==Koi9_Unicode[i1].to )
	     {   Koi8[j++]=Koi9_Unicode[i1].from[0] ;
	         Koi8[j++]=Koi9_Unicode[i1].from[1] ;
		 Koi8[j++]=Koi9_Unicode[i1].from[2] ;
	         goto M1 ;
	     }
      }
      Koi8[j++]=' ' ;
      M1:;
   }
   Koi8[j]=0 ;
   return j ;
}
/***************************************************************************/
//                  функции работы со строками
/***************************************************************************/
void strcpy32( t_myChar *Str1, t_myChar *Str )
{
   for( long i=0 ;  ; i++ )
   {  Str1[i]=Str[i] ;
      if( Str[i].Char==0 ) break ;
   }
}
/***************************************************************************/
void strncpy32( t_myChar *Str1, t_myChar *Str, long L )
{
   for( long i=0 ;  i<L ; i++ )
   {  Str1[i]=Str[i] ;
      if( Str[i].Char==0 ) break ;
   }
}
/***************************************************************************/
int strncmp32( t_myChar *Str1, t_myChar *Str2, long L )
{  int r ;
   for( long i=0 ;  i<L ; i++ )
   {  r=Str1[i].Char-Str2[i].Char ;
      if( r!=0 ) return r ;
      if( Str2[i].Char==0 ) break ;
   }
   return 0 ;
}
/***************************************************************************/
int strncmp32_8( t_myChar *Str1, char *Str2, long L )
{  int r ;
   for( long i=0 ;  i<L ; i++ )
   {  r=Str1[i].Char-koi8_to_Unicod(Str2[i]) ;
      if( r!=0 ) return r ;
      if( Str2[i]==0 ) break ;
   }
   return 0 ;
}
/***************************************************************************/
long strlen32( t_myChar *str )
{  long i ;

   for( i=0 ; str[i].Char!=0 ; i++ )
      ;
   return i ;
}
/***************************************************************************/
//                        работа с t_myChar
/***************************************************************************/
t_myChar t_myChar :: operator = ( char c )
{
   Flag=0 ;
   Char=koi8_to_Unicod( c );
   return *this ;
}
/***************************************************************************/
t_myChar t_myChar :: operator = ( ushort C )
{
   Flag=0 ;
   Char=C ;
   return *this ;
}
/***************************************************************************/
t_myChar t_myChar :: operator = ( int C )
{
   Flag=0 ;
   Char=(ushort)C ;
   return *this ;
}
/***************************************************************************/
char t_myChar :: operator == ( char c )
{
   return ( Char==koi8_to_Unicod( c ) );
}
/***************************************************************************/
char t_myChar :: operator != ( char c )
{
   return ( Char!=koi8_to_Unicod( c ) );
}
/***************************************************************************/
char t_myChar :: operator == ( ushort C )
{
   return (Char==C) ;
}
/***************************************************************************/
char t_myChar :: operator == ( int C )
{
   return (Char==(ushort)C) ;
}
/***************************************************************************/
char t_myChar :: operator != ( t_myChar C )
{
   return (Char!=C.Char) ;
}
/***************************************************************************/
t_myChar  myChar_toUp( t_myChar C )
{  t_myChar D ;

   D.Char=Unicode_toUp( C.Char );
   D.Flag=C.Flag ;
   return D ;
}
/***************************************************************************/
void init_unicod()
{
   char    *T1, *T2, T[256] ;
   size_t   N1,N2 ;
   iconv_t  cd ;
   char    *U1 ;

   for( short i=0 ; i<256 ; i++ )
      T[i]=i ;

   cd=iconv_open("UCS-2BE","KOI8-R" ) ;
   T1 =&T[0] ;
   U1 =(char *)&ToUnicode[0][0] ;
   N1=256,N2=512 ;
   iconv( cd, &T1, &N1, &U1, &N2 );
   iconv_close( cd );

   cd=iconv_open("UCS-2BE","iso8859-1" ) ;
   T1 =&T[0] ;
   U1 =(char *)&ToUnicode[1][0] ;
   N1=256,N2=512 ;
   iconv( cd, &T1, &N1, &U1, &N2 );
   iconv_close( cd );

   cd=iconv_open("UCS-2BE","iso8859-2" ) ;
   T1 =&T[0] ;
   U1 =(char *)&ToUnicode[2][0] ;
   N1=256,N2=512 ;
   iconv( cd, &T1, &N1, &U1, &N2 );
   iconv_close( cd );

   cd=iconv_open("UCS-2BE","iso8859-3" ) ;
   T1 =&T[0] ;
   U1 =(char *)&ToUnicode[3][0] ;
   N1=256,N2=512 ;
   iconv( cd, &T1, &N1, &U1, &N2 );
   iconv_close( cd );

   cd=iconv_open("UCS-2BE","iso8859-4" ) ;
   T1 =&T[0] ;
   U1 =(char *)&ToUnicode[4][0] ;
   N1=256,N2=512 ;
   iconv( cd, &T1, &N1, &U1, &N2 );
   iconv_close( cd );

   // --------- построение обратных таблиц --------------
   for( short i=0 ; i<256 ; i++ )
      UnicodeToKoi[ ToUnicode[0][i] ]=i ;
   UnicodeToKoi[ 8]= 8 ;
   UnicodeToKoi[ 9]= 9 ;
   UnicodeToKoi[13]=13 ;
   UnicodeToKoi[27]=27 ;

   make_UnicodeToUp( );
}
