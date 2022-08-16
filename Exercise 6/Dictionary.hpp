#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
#include "HashMap.hpp"

#define INVALID_KEY "Invalid key"
using std::string;
typedef std::vector<string> string_vec;

class InvalidKey : public std::invalid_argument {
 public:
  explicit InvalidKey (const string &e) : std::invalid_argument (e)
  {}
  explicit InvalidKey () : std::invalid_argument ("")
  {}

};

class Dictionary : public HashMap<string, string> {

 private:
  /**
   * The function force an insert of the given pair inside the HashMap
   * also if the key is already inside the map
   * @param key The given key
   * @param value The given Value
   */
  void force_insert (const string &key, const string &value)
  {
    (*this)[key] = value;
  }

 public:
  Dictionary () = default;
  Dictionary (const Dictionary &other) : HashMap (other)
  {}
  Dictionary (const string_vec &key, const string_vec &val)
      : HashMap (key, val)
  {}

  /**
   * The function erase the key from the HashMap and throw exception of
   * InvalidKey if not be able to delete the given key
   * @param key The given key
   * @returns True if success anf False otherwise
   */
  bool erase (const string &key) override
  {
    if (!(*this).contains_key (key))
      {
        throw InvalidKey (INVALID_KEY);
      }
    HashMap::erase (key);
    return true;
  }
  /**
   * The function updates the HashMap according to given start / end
   * iterators, it will force an insert of iterator pairs to the map
   * @tparam Iterator A Forward Iterator class given
   * @param start The begin iterator
   * @param end The end iterator
   */
  template<class Iterator>
  void update (Iterator start, Iterator end)
  {
    if (start == end)
      {
        return;
      }
    for (; start != end; start++)
      {
        if (!contains_key (start->first))
          {
            HashMap::insert (start->first, start->second);
          }
        else
          {
            this->force_insert (start->first, start->second);
          }
      }
  }

};

#endif //_DICTIONARY_HPP_
