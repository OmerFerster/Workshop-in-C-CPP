#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H

#include "RSUser.h"
#include <map>
#include <cmath>
#include <algorithm>
#define ZERO 0.0
#define FOUR 4
#define NOT_FOUND -999
#define TEN 10
#define LENGTH_ERROR "The elem not in the valid range [1,10]"

typedef std::vector<double> vec;
/**
 * The function compute the avg rank from all user seen movies
 * @param user The given user
 * @return The avg value for all ranks the user did
 */
double avg_rank (const RSUser &user);
/**
 * The function normalized the user rank_map by decreasing all ranks the avg
 * @param user The given user
 * @param avg The avg user ranks
 * @return A new map being normalized
 */
rank_map normal_map (const RSUser &user, double avg);
/**
 * The function mult between vector and a given num
 * @param vec
 * @param num
 * @return A result of the multiplication
 */
vec num_mult_vec (std::vector<double> &vec, double num);
/**
 * The function compute the preference vector of the given user
 * @param user The given user
 * @param sys The RS
 * @return A new vector with preference by the user
 */
vec user_preference_vector (const RSUser &user,
                            std::map<Movie, vec> &storage);
/**
 * The function compute the standard mult between 2 vectors
 * @param vec1
 * @param vec2
 * @return A double value of the multiplication
 */
double standart_vec_mult (const vec &vec1, const vec &vec2);
/**
 * The function adds 2 vectors
 * @param first
 * @param second
 * @return A new vector equals the addition between both given vectors
 */
vec add_vectors (vec first, vec second);
/**
 * The function compute the norm of A vector
 * @param vect the vec to norm
 * @return A double value of the norm
 */
double norm (const vec &vect);
/**
 * The function returns the value of imagination of 2 vectors
 * @param preference_vec The first vec
 * @param movie_prop The second vec
 * @return A double value of the 2 vectors similarities
 */
double imagine_result (const vec &preference_vec, const vec &movie_prop);
/**
 * The function returns a tuple with the the most common imaginary rated "k"
 * movies the user saw to imagine the root movie features
 * @param user The user given
 * @param sys The RS
 * @param root The given movie features
 * @param k The number of movies to be imagine
 * @return A tuple of k most imagine movies to root
 */
std::vector<std::tuple<Movie, double>> k_most_imagine_films
    (const RSUser &user, std::map<Movie, vec> &storage,
     const vec &root, int k);
/**
 * The function sorts the tuple according the second value given
 * from largest to smallest
 * @param a first tuple
 * @param b second tuple
 * @return true if a larger from b and false otherwise
 */
bool sort_by_sec (const std::tuple<Movie, double> &a,
                  const std::tuple<Movie, double> &b);


class RecommenderSystem {
 private:
  std::map<Movie, vec> _storage;  // All movies storage

 public:
  RecommenderSystem ();  // Default constractor
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie (const std::string &name, int year,
                      const std::vector<double> &features);
  /**
   * a function that calculates the movie with highest score
   * based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie get_recommendation_by_content (const RSUser &user);
  /**
   * a function that calculates the movie with highest predicted
   * score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);
  /**
   * Predict a user rating for a movie given argument using item
   * cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);
  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year) const;
  /**
   * The function prints all RS movies in storage by order
   * @param os The output stream given
   * @param rs The RS
   */
  friend std::ostream &operator<< (std::ostream &os, RecommenderSystem &rs);

};

#endif // RECOMMENDERSYSTEM_H
