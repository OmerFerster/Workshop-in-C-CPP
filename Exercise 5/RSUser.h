#ifndef USER_H
#define USER_H

#include "Movie.h"
#include <unordered_map>
#define TEN 10
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;
class RecommenderSystem;
typedef std::shared_ptr<RecommenderSystem> sp_rec_system;

class RSUser {
 private:
  std::string _username;
  rank_map _user_map;
  sp_rec_system _user_system;

 public:
  RSUser (std::string &name, rank_map &user_map, sp_rec_system &user_system);
  /**
   * a getter for the user's name
   * @return the username
   */
  std::string get_name () const;
  /**
   * function for adding a movie to the DB
   * @param movie a Movie object
   * @param features a vector of the movie's features
   * @param rate the user rate for this movie
   */
  void add_movie_to_rs (const std::string &name, int year,
                        const std::vector<double> &features,
                        double rate);
  /**
   * a getter for the ranks map
   * @return
   */
  rank_map get_ranks () const;
  /**
   * returns a recommendation according to the movie's content
   * @return recommendation
   */
  sp_movie get_recommendation_by_content () const;
  /**
   * returns a recommendation according to the similarity
   * recommendation method
   * @param k the number of the most similar movies to calculate by
   * @return recommendation
   */
  sp_movie get_recommendation_by_cf (int k) const;
  /**
   * predicts the score for a given movie
   * @param name the name of the movie
   * @param year the year the movie was created
   * @param k the parameter which represents the number of the most
   * similar movies to predict the score by
   * @return predicted score for the given movie
   */
  double
  get_prediction_score_for_movie (const std::string &name,
                                  int year, int k) const;
  /**
   * output RS from the users
   * @param os the output stream
   * @param user the user
   * @return output streams
   */
  friend std::ostream &operator<< (std::ostream &os, RSUser &other);

};

#endif // USER_H
