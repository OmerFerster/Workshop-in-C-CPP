#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_
#include <iostream>
#include <vector>

#define ZERO 0
#define ONE 1
#define TWO 2
#define END_OF_MAP -1
#define INITIAL_SIZE 16
#define UPPER_BOUND 0.75
#define LOWER_BOUND 0.25
#define LENGTH_ERROR "The vector sizes are not the same"
#define NOT_FOUND "Key not found"

template<class KeyT, class ValueT>
class HashMap {

 private:
  typedef std::vector<std::pair<KeyT, ValueT>> pair_vec;
  class Bucket {
   private:
    pair_vec array;
   public:
    /**
     * Default Copy & Regular constructors
     */
    Bucket () = default;
    Bucket (const Bucket &other) = default;
    /**
     * @return The Bucket pairs vector
     */
    pair_vec &get_array ()
    {
      return array;
    }
    /**
     * @returns True if Bucket has an empty vector and False otherwise
     */
    bool empty () const
    {
      return array.empty ();
    }
    /**
     * The function pushes a given pair to the pairs vector
     * @param elem The given pair to add to the Bucket
     */
    void push (std::pair<KeyT, ValueT> elem)
    {
      array.push_back (elem);
    }
    /**
     * The function checks if the given key is in the Bucket
     * @param key The given key to check if existed in the Bucket
     * @return True if key inside Bucket and False otherwise
     */
    bool find (const KeyT &key) const
    {
      for (const auto &pair: array)
        {
          if (pair.first == key)
            {
              return true;
            }
        }
      return false;
    }
  };

  int _capacity;
  int _size;
  Bucket *table;
  /**
   * The function delete the Dynamic Allocated table of Buckets
   */
  void destruct_table ()
  {
    if ((_capacity > ONE) && (table != nullptr))
      {
        delete[] table;
        table = nullptr;
      }
    else if ((_capacity == ONE) && (table != nullptr))
      {
        delete table;
        table = nullptr;
      }
  }
  /**
   * @returns The current load factor of the HashMap
   */
  double load_factor () const
  {
    return ((double) _size / _capacity);
  }
  /**
   * The function checks if the load factor is larger then 0.75 (UPPER BOUND)
   * @returns True if larger and False otherwise
   */
  bool is_heavy_map () const
  {
    return load_factor () > UPPER_BOUND;
  }
  /**
   * The function checks if the load factor is smaller then 0.25 (LOWER BOUND)
   * @returns True if smaller and False otherwise
   */
  bool is_easy_map () const
  {
    return load_factor () < LOWER_BOUND;
  }
  /**
   * The function hash the given key to a fitting index in the table
   * @param x The given key
   * @returns An int represent the index of the table of "x"
   */
  int hash_function (const KeyT &x) const
  {
    int a = std::hash<KeyT>{} (x);
    return a & (_capacity - 1);
  }
  /**
   * @returns The needed new rehashing table size
   */
  int extract_needed_capacity () const
  {
    int needed = 1;
    if (_size == ZERO)
      {
        return ONE;
      }
    while ((double) _size / needed >= UPPER_BOUND)
      {
        needed *= TWO;
      }
    return needed;
  }
  /**
   * The function reduce the table and rehash it according to the new size
   */
  void rehash_smaller ()
  {
    int old_capa = _capacity;
    _capacity = extract_needed_capacity ();
    HashMap<KeyT, ValueT> new_map (_capacity);
    auto new_table = new_map.table;
    for (int i = 0; i < old_capa; i++)
      {
        if (!table[i].empty ())
          {
            pair_vec next_arr = table[i].get_array ();
            for (int k = 0; k < (int) next_arr.size (); k++)
              {
                int index = hash_function (next_arr[k].first);
                new_table[index].push (std::make_pair
                                           (next_arr[k].first,
                                            next_arr[k].second));
                new_map._size++;
              }
          }
      }
    *this = new_map;
  }
  /**
   * The function enlarge the table and rehash it according to the new size
   */
  void rehash_bigger ()
  {
    HashMap<KeyT, ValueT> new_map (_capacity * TWO);
    auto new_table = new_map.table;
    for (int j = 0; j < _capacity; j++)
      {
        pair_vec copy_me = table[j].get_array ();
        if (!copy_me.empty ())
          {
            for (int k = 0; k < (int) copy_me.size (); k++)
              {
                int index = hash_function (copy_me[k].first);
                new_table[index].push (std::make_pair
                                           (copy_me[k].first,
                                            copy_me[k].second));
                new_map._size++;
              }
          }
      }
    *this = new_map;
  }
  /**
   * The function rehash the table according to the new size of the table
   * larger or smaller depends if table is heavy or easy
   */
  void rehash ()
  {
    if (is_easy_map ())
      {
        this->rehash_smaller ();
      }
    else if (is_heavy_map ())
      {
        this->rehash_bigger ();
      }
  }

 public:
  class ConstIterator;
  /**
   * Default Constructors
   */
  HashMap () : _capacity (INITIAL_SIZE), _size (ZERO)
  {
    table = new Bucket[INITIAL_SIZE];
  }

  explicit HashMap (int capacity) : _capacity (capacity), _size (ZERO)
  {
    table = new Bucket[capacity];
  }

  HashMap (std::vector<KeyT> keys_vec, std::vector<ValueT> values_vec) :
      _capacity (INITIAL_SIZE), _size (ZERO)
  {
    if (keys_vec.size () != values_vec.size ())
      {
        throw std::length_error (LENGTH_ERROR);
      }
    this->table = new Bucket[INITIAL_SIZE];
    for (int i = 0; i < (int) keys_vec.size (); i++)
      {
        (*this)[keys_vec[i]] = values_vec[i];
      }
  }
  /**
   * Copy Constructor
   * @param other The other HashMap to copy from
   */
  HashMap (const HashMap<KeyT, ValueT> &other) :
      _capacity (other.capacity ()), _size (other.size ())
  {
    this->table = new Bucket[other._capacity];
    for (int i = 0; i < other._capacity; i++)
      {
        pair_vec copy = other.table[i].get_array ();
        if (!copy.empty ())
          {
            for (int k = 0; k < (int) copy.size (); k++)
              {
                this->table[i].push (std::make_pair
                                         (copy[k].first,
                                          copy[k].second));
              }
          }
      }
  }
  /**
   * Destructor function
   */
  virtual ~HashMap ()
  {
    destruct_table ();
  }
  /**
   * @returns The size of the HashMap - actual elements inside it
   */
  int size () const
  {
    return _size;
  }
  /**
   *@returns The capacity of the HashMap - The number of Buckets in the map
   */
  int capacity () const
  {
    return _capacity;
  }
  /**
   * The function checks if The HashMap is empty from pairs
   * @returns True if empty and False otherwise.
   */
  bool empty () const
  {
    return (_size == ZERO);
  }
  /**
   * The function Insert a new pair to the HashMap, if key already inside the
   * map then The map wont insert the new value
   * @param key The given key
   * @param val The given value of the key
   * @returns True if success and False otherwise
   */
  bool insert (const KeyT &key, const ValueT &val)
  {
    if (this->contains_key (key))
      {
        return false;
      }
    int index = hash_function (key);
    this->table[index].push (std::make_pair (key, val));
    this->_size++;
    if (is_heavy_map ())
      {
        (*this).rehash ();
      }
    return true;
  }
  /**
   * The function checks if the given key is already inside the map or not
   * @param key The given key
   * @returns True if key inside map and False otherwise
   */
  bool contains_key (const KeyT &key) const
  {
    for (int i = 0; i < _capacity; i++)
      {
        if (table[i].find (key))
          {
            return true;
          }
      }
    return false;
  }
  /**
   * The function returns The value of the given key inside the map
   * @param key The given key
   */
  ValueT &at (const KeyT &key)
  {
    if (!contains_key (key))
      {
        throw std::runtime_error (NOT_FOUND);
      }
    else
      {
        return (*this)[key];
      }
  }

  const ValueT &at (const KeyT &key) const
  {
    if (!contains_key (key))
      {
        throw std::runtime_error (NOT_FOUND);
      }
    else
      {
        return (*this)[key];
      }
  }
  /**
   * The function erase a given pair from the HashMap
   * @param key The given key to delete the elem from
   * @returns True if success and False otherwise
   */
  virtual bool erase (const KeyT &key)
  {
    if (!(*this).contains_key (key))
      {
        return false;
      }
    for (int i = 0; i < _capacity; i++)
      {
        for (int k = 0; k < (int) table[i].get_array ().size (); k++)
          {
            if (table[i].get_array ()[k].first == key)
              {
                table[i].get_array ().erase (
                    table[i].get_array ().begin () + k);
                _size--;
              }
          }
      }
    if (_size == ZERO)
      {
        if (_capacity > ONE)
          {
            delete[] this->table;
          }
        else if (_capacity == ONE)
          {
            delete this->table;
          }
        this->_capacity = ONE;
        this->table = new Bucket;
        return true;
      }
    if (is_easy_map ())
      {
        (*this).rehash ();
      }
    return true;
  }
  /**
   * @returns The load factor of the HashMap
   */
  double get_load_factor () const
  {
    return load_factor ();
  }
  /**
   * The function checks the Buckets size of the given key index
   * @param key The given key
   * @returns The size of the key current Bucket
   */
  int bucket_size (const KeyT &key) const
  {
    if (!contains_key (key))
      {
        throw std::runtime_error (NOT_FOUND);
      }
    return (int) this->table[this->bucket_index (key)].get_array ().size ();
  }
  /**
  * The function checks the Buckets index of the given key index
  * @param key The given key
  * @returns The index of the key current Bucket
  */
  int bucket_index (const KeyT &key) const
  {
    if (!contains_key (key))
      {
        throw std::runtime_error (NOT_FOUND);
      }
    return hash_function (key);
  }
  /**
   * The function clears all HashMap from all elements, delete them all
   */
  void clear ()
  {
    for (int i = 0; i < _capacity; i++)
      {
        table[i].get_array ().clear ();
      }
    _size = 0;
  }

  // ~~ The needed operators ~~ //

  HashMap &operator= (const HashMap<KeyT, ValueT> &other)
  {
    if (this == &other)
      {
        return *this;
      }
    if (other.capacity () == ONE)
      {
        this->table = new Bucket;
        this->_capacity = ONE;
        this->_size = ZERO;
        return *this;
      }
    destruct_table ();
    this->table = new Bucket[other._capacity];
    for (int i = 0; i < other._capacity; i++)
      {
        pair_vec copy = other.table[i].get_array ();
        if (!copy.empty ())
          {
            for (int k = 0; k < (int) copy.size (); k++)
              {
                this->table[i].push (std::make_pair
                                         (copy[k].first,
                                          copy[k].second));
              }
          }
      }
    this->_capacity = other._capacity;
    this->_size = other._size;
    return *this;
  }

  ValueT &operator[] (const KeyT &key)
  {
    if (!contains_key (key))
      {
        ValueT new_val = ValueT ();
        this->insert (key, new_val);
        return this->at (key);
      }
    for (int i = 0; i < _capacity; i++)
      {
        if (table[i].find (key))
          {
            for (auto &it: table[i].get_array ())
              {
                if (it.first == key)
                  {
                    return it.second;
                  }
              }
          }
      }
    return this->at (key);
  }

  const ValueT &operator[] (const KeyT &key) const
  {
    for (int i = 0; i < _capacity; i++)
      {
        if (table[i].find (key))
          {
            for (const auto &it: table[i].get_array ())
              {
                if (it.first == key)
                  {
                    return it.second;
                  }
              }
          }
      }
    return this->at (key);
  }

  bool operator== (const HashMap<KeyT, ValueT> &other) const
  {
    if (_size != other._size)
      {
        return false;
      }
    for (const auto &first_iter: *this)
      {
        if (!other.contains_key (first_iter.first))
          {
            return false;
          }
        else
          {
            if (other[first_iter.first] != first_iter.second)
              {
                return false;
              }
          }
      }
    for (const auto &second_iter: other)
      {
        if (!(*this).contains_key (second_iter.first))
          {
            return false;
          }
        else
          {
            if ((*this)[second_iter.first] != second_iter.second)
              {
                return false;
              }
          }
      }
    return true;
  }

  bool operator!= (const HashMap<KeyT, ValueT> &other) const
  {
    bool okay = (*this == other);
    return !okay;
  }

  // ~~ The Const Forward Iterator begin / end functions ~~ //

  ConstIterator begin () const
  {
    return ConstIterator (this, ZERO);
  }

  ConstIterator end () const
  {
    return ConstIterator (this, END_OF_MAP);
  }

  ConstIterator cbegin () const
  {
    return ConstIterator (this, ZERO);
  }

  ConstIterator cend () const
  {
    return ConstIterator (this, END_OF_MAP);
  }

};

template<class KeyT, class ValueT>
class HashMap<KeyT, ValueT>::ConstIterator {

  typedef std::pair<KeyT, ValueT> gen_pair;
 private:
  const HashMap<KeyT, ValueT> *hash_map;
  int bucket_index;
  int pair_index;
  const gen_pair *it_ptr;
  int next_unempty_bucket (int start)
  {
    if ((start >= hash_map->_capacity) || (start == END_OF_MAP))
      {
        return END_OF_MAP;
      }
    for (int i = start; i < hash_map->capacity (); i++)
      {
        if (!(hash_map->table[i].empty ()))
          {
            return i;
          }
      }
    return END_OF_MAP;
  }

 public:
  typedef gen_pair value_type;
  typedef const gen_pair &reference;
  typedef const gen_pair *pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::forward_iterator_tag iterator_category;

  ConstIterator (const HashMap<KeyT, ValueT> *it_map, int start)
  {
    hash_map = it_map;
    bucket_index = next_unempty_bucket (start);
    if (bucket_index == END_OF_MAP)
      {
        pair_index = END_OF_MAP;
        it_ptr = nullptr;
      }
    else
      {
        pair_index = ZERO;
        pair_vec &arr = hash_map->table[bucket_index].get_array ();
        it_ptr = &(arr[pair_index]);
      }
  }

  const gen_pair &operator* () const
  {
    return *it_ptr;
  }

  const gen_pair *operator-> () const
  {
    return it_ptr;
  }

  ConstIterator &operator++ ()
  {
    pair_vec &arr = (*hash_map).table[bucket_index].get_array ();
    if (pair_index == (int) arr.size () - 1)
      {
        int new_bucket_index = next_unempty_bucket (bucket_index + 1);
        bucket_index = new_bucket_index;
        if (bucket_index == END_OF_MAP)
          {
            pair_index = END_OF_MAP;
            it_ptr = nullptr;
          }
        else
          {
            pair_index = ZERO;
            pair_vec &new_arr = (*hash_map).table[bucket_index].get_array ();
            it_ptr = &(new_arr[pair_index]);
          }
      }
    else
      {
        pair_index++;
        it_ptr = &(arr[pair_index]);
      }
    return *this;
  }

  ConstIterator operator++ (int)
  {
    ConstIterator temp = *this;
    pair_vec &arr = (*hash_map).table[bucket_index].get_array ();
    if (pair_index == (int) arr.size () - 1)
      {
        int new_bucket_index = next_unempty_bucket (bucket_index + 1);
        bucket_index = new_bucket_index;
        if (bucket_index == END_OF_MAP)
          {
            pair_index = END_OF_MAP;
            it_ptr = nullptr;
          }
        else
          {
            pair_vec &new_arr = (*hash_map).table[bucket_index].get_array ();
            pair_index = ZERO;
            it_ptr = &(new_arr[pair_index]);
          }
      }
    else
      {
        pair_index++;
        it_ptr = &(arr[pair_index]);
      }
    return temp;
  }

  friend bool operator== (const ConstIterator &a, const ConstIterator &b)
  {
    return ((a.it_ptr == b.it_ptr) && (a.hash_map == b.hash_map));
  };

  friend bool operator!= (const ConstIterator &a, const ConstIterator &b)
  {
    bool okay = (a == b);
    return !okay;
  }

};

#endif //_HASHMAP_HPP_
