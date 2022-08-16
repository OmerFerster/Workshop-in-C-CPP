#include <iostream>
#include "Dictionary.hpp"

using std::string;
using std::cout;
using std::endl;

int main ()
{
  HashMap<string, string> *arr = new Dictionary[TWO];
  for (int i = 0; i < 2; i++)
    {
      for (int j = 1; j <= 10; j++)
        {
          arr[i].insert (std::to_string (j), std::to_string (j * 10));
        }
    }
  for (int k = 1; k <= 9; k++)
    {
      arr[ZERO].erase (std::to_string (k));
    }
  for (const auto &it: arr[ZERO])
    {
      cout << "key: " << it.first << " - value: " << it.second << endl;
    }
  for (const auto &it: arr[ONE])
    {
      cout << "key: " << it.first << " - value: " << it.second << endl;
    }
  return 0;
}

/*
 * if ((bucket_index != END_OF_MAP) && (pair_index != END_OF_MAP)
        && (it_ptr != nullptr))
      {
        return *it_ptr;  //itertor* check this
      }
 */

/*
  void print_map () const
  {
    std::cout << "Map size - " << _size << ", Map capa - " << _capacity
              << std::endl;
    for (int i = 0; i < _capacity; i++)
      {
        Bucket b = (*this).table[i];
        if (!b.empty ())
          {
            pair_vec c = b.get_array ();
            std::cout << "Bucket num - " << i << std::endl;
            for (int k = 0; k < c.size (); k++)
              {
                std::cout << c[k].first << "-" << c[k].second << " ";
              }
            std::cout << std::endl;
          }
      }
  }
   */
