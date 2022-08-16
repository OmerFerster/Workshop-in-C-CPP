#include "tests.h"
#include <string.h>

#define K_1 3
#define K_2 2
#define K_29 29
#define K_n1 -1
#define K_n3 -3

int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);
  return strcmp (in, out) == 0;
}

int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "ab.z";
  char out[] = "cd.b";
  encode (in, K_2);
  return strcmp (in, out) == 0;
}

int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "bc.d";
  char out[] = "ab.c";
  encode (in, K_n1);
  return strcmp (in, out) == 0;
}

int test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "bcd";
  char out[] = "yza";
  encode (in, K_n3);
  return strcmp (in, out) == 0;
}

int test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "ABC";
  char out[] = "DEF";
  encode (in, K_29);
  return strcmp (in, out) == 0;
}

int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, K_1);
  return strcmp (in, out) == 0;
}

int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "ab.c";
  char out[] = "yz.a";
  decode (in, K_2);
  return strcmp (in, out) == 0;
}

int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "ab.c";
  char out[] = "bc.d";
  decode (in, K_n1);
  return strcmp (in, out) == 0;
}

int test_decode_cyclic_lower_case_negative_k ()
{
  char in[] = "abz";
  char out[] = "dec";
  decode (in, K_n3);
  return strcmp (in, out) == 0;
}

int test_decode_cyclic_upper_case_positive_k ()
{
  char in[] = "ABC";
  char out[] = "XYZ";
  decode (in, K_29);
  return strcmp (in, out) == 0;
}
