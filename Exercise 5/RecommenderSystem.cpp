#include "RecommenderSystem.h"

RecommenderSystem::RecommenderSystem () = default;

sp_movie RecommenderSystem::add_movie
    (const std::string &name, int year, const std::vector<double> &features)
{
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  for(const auto& elem : features)
    {
      if((elem < 1) || (elem > TEN))
        {
          throw std::length_error(LENGTH_ERROR);
        }
    }
  this->_storage[*new_movie] = features;  // log n
  return new_movie;
}

sp_movie RecommenderSystem::get_recommendation_by_content (const RSUser &user)
{
  std::vector<std::tuple<Movie, double>> res;
  vec pref_vec = user_preference_vector (user, this->_storage);
  for (const auto &elem: this->_storage)
    {
      sp_movie other = std::make_shared<Movie> (elem.first);
      if (user.get_ranks ()[other] == NOT_FOUND)
        {
          double test = imagine_result (pref_vec, elem.second);
          Movie final (elem.first.get_name (), elem.first.get_year ());
          std::tuple<Movie, double> new_tup = std::make_tuple (final, test);
          res.push_back (new_tup);
        }
    }
  std::sort (res.begin (), res.end (), sort_by_sec);
  std::tuple<Movie, double> last = res[0];
  return std::make_shared<Movie> (std::get<0> (last));
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  std::vector<std::tuple<Movie, double>> res;
  for (const auto &elem: this->_storage)
    {
      sp_movie other = std::make_shared<Movie> (elem.first);
      if (user.get_ranks ()[other] == NOT_FOUND)
        {
          double new_rank = predict_movie_score (user, other, k);
          std::tuple<Movie, double> new_tup = std::make_tuple (*other,
                                                               new_rank);
          res.push_back (new_tup);
        }
    }
  std::sort (res.begin (), res.end (), sort_by_sec);
  std::tuple<Movie, double> last = res[0];
  return std::make_shared<Movie> (std::get<0> (last));
}

double RecommenderSystem::predict_movie_score
    (const RSUser &user, const sp_movie &movie, int k)
{
  vec root = this->_storage[*movie];
  double sum = ZERO;
  double divide = ZERO;
  std::vector<std::tuple<Movie, double>> res = k_most_imagine_films
      (user, this->_storage, root, k);
  for (int i = 0; i < k; i++)
    {
      auto a = std::make_shared<Movie> (std::get<0> (res[i]));
      sum += (std::get<1> (res[i]) * user.get_ranks ()[a]);
      divide += std::get<1> (res[i]);
    }
  if (divide != ZERO)
    {
      return sum / divide;
    }
  return ZERO;
}

sp_movie RecommenderSystem::get_movie
    (const std::string &name, int year) const
{
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  if (this->_storage.find (*new_movie) == this->_storage.end ()) // O(log n)
    {
      return nullptr; //  todo check if nullptr or shared ptr to null
    }
  return new_movie;
}

std::ostream &operator<< (std::ostream &os, RecommenderSystem &rs)
{
  for (const auto &elem: rs._storage)
    {
      os << elem.first;
    }
  return os;
}

double avg_rank (const RSUser &user)
{
  double sum = ZERO;
  int rated = 0;
  for (const auto &elem: user.get_ranks ())
    {
      if (elem.second != NOT_FOUND)
        {
          rated++;
          sum += elem.second;
        }
    }
  if (rated != 0)
    {
      return sum / rated;
    }
  return ZERO;
}

rank_map normal_map (const RSUser &user, double avg)
{
  rank_map new_map (0, sp_movie_hash, sp_movie_equal);
  for (const auto &elem: user.get_ranks ())
    {
      double new_rank = elem.second - avg;
      new_map[elem.first] = new_rank;
    }
  return new_map;
}

vec num_mult_vec (vec &vect, double num)
{
  for (double &i: vect)
    {
      i *= num;
    }
  return vect;
}

vec add_vectors (vec first, vec second)
{
  vec res (first.size (), ZERO);
  for (int i = 0; i < (int) first.size (); i++)
    {
      res[i] = (first[i] + second[i]);
    }
  return res;
}

vec user_preference_vector (const RSUser &user, std::map<Movie, vec> &storage)
{
  double user_avg_rank = avg_rank (user);
  rank_map new_user_map = normal_map (user, user_avg_rank);
  vec res (FOUR, ZERO); // TODO CHANGE THIS 4
  for (const auto &elem: new_user_map)
    {
      if (elem.second != (NOT_FOUND - user_avg_rank))
        {
          vec prop = storage[*elem.first];
          num_mult_vec (prop, elem.second);
          res = add_vectors (res, prop);
        }
    }
  return res;
}

double standart_vec_mult (const vec &vec1, const vec &vec2)
{
  double sum = ZERO;
  for (int i = 0; i < (int) vec1.size (); i++)
    {
      sum += (vec1[i] * vec2[i]);
    }
  return sum;
}

double norm (const vec &vect)
{
  return sqrt (standart_vec_mult (vect, vect));
}

double imagine_result (const vec &preference_vec, const vec &movie_prop)
{
  double prop_norm = norm (movie_prop);
  double pref_norm = norm (preference_vec);
  if (prop_norm == ZERO || pref_norm == ZERO)
    {
      return ZERO;
    }
  double me = standart_vec_mult (preference_vec, movie_prop);
  return me / (prop_norm * pref_norm);
}

std::vector<std::tuple<Movie, double>> k_most_imagine_films
    (const RSUser &user, std::map<Movie, vec> &storage,
     const vec &root, int k)
{
  std::vector<std::tuple<Movie, double>> res;
  if ((int) user.get_ranks ().size () < k)
    {
      return res;
    }
  for (const auto &elem: storage)
    {
      sp_movie other = std::make_shared<Movie> (elem.first);
      if (user.get_ranks ()[other] != NOT_FOUND)
        {
          double match = imagine_result (elem.second, root);
          std::tuple<Movie, double> new_tuple = std::make_tuple
              (elem.first, match);
          res.push_back (new_tuple);
        }
    }
  sort (res.begin (), res.end (), sort_by_sec);
  std::vector<std::tuple<Movie, double>> final;
  for (int i = 0; i < k; i++)
    {
      final.push_back (res[i]);
    }
  return final;
}

bool sort_by_sec (const std::tuple<Movie, double> &a,
                  const std::tuple<Movie, double> &b)
{
  return (std::get<1> (a)
          > std::get<1> (b));
}
