#include "Movie.h"

Movie::Movie (const std::string &name, int year) : _name (name), _year (year)
{}

const std::string &Movie::get_name () const
{
  return this->_name;
}

int Movie::get_year () const
{
  return this->_year;
}

std::ostream &operator<< (std::ostream &os, const Movie &movie)
{
  os << movie.get_name () << " (" << movie.get_year () << ")" << std::endl;
  return os;
}

bool Movie::operator< (const Movie &other) const
{
  if (this->_year < other._year)
    {
      return true;
    }
  else if (this->_year == other._year)
    {
      return (this->_name < other._name);
    }
  return false;
}

/**
 * hash function used for a unordered_map STL
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash (const sp_movie &movie)
{
  std::size_t res = HASH_START;
  res = res * RES_MULT + std::hash<std::string> () (movie->get_name ());
  res = res * RES_MULT + std::hash<int> () (movie->get_year ());
  return res;
}

/**
 * equal function used for an unordered_map STL to
 * @param m1 first movie sp
 * @param m2 second movie sp
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2)
{
  return !(*m1 < *m2) && !(*m2 < *m1);
}
