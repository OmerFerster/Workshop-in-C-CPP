#include "cipher.h"

const int a = 97;
const int z = 122;
const int A = 65;
const int Z = 90;

char edit_right (int c, int k);
char edit_left (int c, int k);
/*
 * The function shift C k steps to the left,
 * handle only Chars between a - z
 */
int handle_a_to_z (int c, int k);
/*
 * The function shift C k steps to the left,
 * handle only Chars between A - Z
 */
int handle_A_to_Z (int c, int k);

char edit_right (int c, int k)
{
  k %= 26;
  int new_val;
  if (c >= a && c <= z)
    {
      if (c + k >= a && c + k <= z)
        {
          new_val = c + k;
        }
      else
        {
          new_val = ((c + k) % z) + a - 1;
        }
    }
  else if (c >= A && c <= Z)
    {
      if (c + k >= A && c + k <= Z)
        {
          new_val = c + k;
        }
      else
        {
          new_val = ((c + k) % Z) + A - 1;
        }
    }
  else
    {
      return (char) c;
    }
  return (char) new_val;
}

char edit_left (int c, int k)
{
  k %= 26;
  int new_val;
  if (c >= a && c <= z)
    {
      new_val = handle_a_to_z (c, k);
    }
  else if (c >= A && c <= Z)
    {
      new_val = handle_A_to_Z (c, k);
    }
  else
    {
      return (char) c;  // returns non-alphabet char
    }
  return (char) new_val;
}

void encode (char s[], int k)
{
  if (k >= 0)
    {
      int i = 0;
      while (s[i] != 0)
        {
          s[i] = edit_right (s[i], k);
          i++;
        }
    }
  else if (k < 0)
    {
      int i = 0;
      while (s[i] != 0)
        {
          s[i] = edit_left (s[i], -k);
          i++;
        }
    }
}

void decode (char s[], int k)
{
  if (k >= 0)
    {
      int i = 0;
      while (s[i] != 0)
        {
          s[i] = edit_left (s[i], k);
          i++;
        }
    }
  else if (k < 0)
    {
      int i = 0;
      while (s[i] != 0)
        {
          s[i] = edit_right (s[i], -k);
          i++;
        }
    }
}

int handle_a_to_z (int c, int k)
{
  if (c - k >= a && c - k <= z)
    {
      return ((int) c - k);
    }
  else
    {
      int temp = k;
      while (temp != 0)
        {
          if (c != a)
            {
              c--;
            }
          else
            {
              c = z;
            }
          temp--;
        }
      return (int) c;
    }
}

int handle_A_to_Z (int c, int k)
{
  if (c - k >= A && c - k <= Z)
    {
      return ((int) c - k);
    }
  else
    {
      int temp = k;
      while (temp != 0)
        {
          if (c != A)
            {
              c--;
            }
          else
            {
              c = Z;
            }
          temp--;
        }
      return (int) c;
    }
}
