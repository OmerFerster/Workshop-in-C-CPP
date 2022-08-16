#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "RecommenderSystemLoader.h"
#define SEP ' '
#define NOT_FOUND -999
#define LENGTH "The ranking is out of range [1,10]"

std::vector<Movie> extract_movies_list (std::string &movies_line);
std::vector<double> extract_user_ranks (std::string &movies_ranking);
/**
 * The function checks if the given str is a valid number or not
 * @param str to get
 * @return true if is num and false otherwise
 */
bool is_number (const std::string &str);
/**
 * The function creates A rank map for the user by given movies and ranks
 * @param movie_arr The movies array
 * @param user_ranks The given user ranking array
 * @return A new rank_map type for the user
 */
rank_map create_rank_map (const std::vector<Movie> &movie_arr,
                          const std::vector<double> &user_ranks);

class RSUsersLoader {
 private:

 public:
  RSUsersLoader () = delete;
  /**
   *
   * loads users by the given format with their movie's ranks
   * @param users_file_path a path to the file of the
   * users and their movie ranks
   * @param rs RecommendingSystem for the Users
   * @return vector of the users created according to the file
   */
  static std::vector<RSUser> create_users_from_file
      (const std::string &users_file_path,
       std::unique_ptr<RecommenderSystem> rs)
  noexcept (false);

};

#endif // USERFACTORY_H
