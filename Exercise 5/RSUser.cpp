#include "RSUser.h"
#include "RecommenderSystem.h"

RSUser::RSUser (std::string &name, rank_map &user_map,
                sp_rec_system &user_system)
    : _username (name), _user_map (user_map), _user_system (user_system)
{}

std::string RSUser::get_name () const
{
  return this->_username;
}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                        const std::vector<double> &features,
                        double rate)
{
  if((rate < 1) || (rate > TEN))
    {
      throw std::length_error(LENGTH_ERROR);
    }
  sp_movie new_p = std::make_shared<Movie> (name, year);
  this->_user_system->add_movie (name, year, features);
  this->_user_map[new_p] = rate;
}

rank_map RSUser::get_ranks () const
{
  return this->_user_map; // rank_map
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return this->_user_system->get_recommendation_by_content(*this);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return this->_user_system->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie
    (const std::string &name, int year, int k) const
{
  sp_movie new_p = std::make_shared<Movie> (name, year);
  return this->_user_system->predict_movie_score (*this, new_p, k);
}

std::ostream &operator<< (std::ostream &os, RSUser &other)
{
  os << "name: " << other._username << std::endl;
  os << *(other._user_system);
  os << std::endl;
  return os;
}
