# ifndef SERIAL_H
# define SERIAL_H

# include "koi9.h"

// ----------------- класс записи и чтения из файла ---------------------
class t_Serial
{
protected:
   FILE *fw,*fr ;
   char *Mass ;
   long  z_Mass,L ;
public:
   void  file_to_mass1( char *File );
   void  print_int( char *tag, long n );
   void  print_hex( char *tag, long n );
   void  print_str( char *tag, char *s );
   void  print_str32( char *tag, t_myChar *s );
   void  scanf_int( long *n );
   void  scanf_hex( long *n );
   void  scanf_str( char *str, long Lmax );
   void  scanf_str32( t_myChar *str, long Lmax );
   void  scanf1_str( char **str );
   void  scanf1_str32( t_myChar **str );
   long  find_at( long z );
   long  find_end( long z );
   char  is_end_block();
   long  find_tag( long z, char *tag );
};
#endif
