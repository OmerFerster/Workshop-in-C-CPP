#include "RSUsersLoader.h"
#include "RecommenderSystemLoader.h"
#include <iostream>

int main ()
{
  std::unique_ptr<RecommenderSystem> rs
      = RecommenderSystemLoader::create_rs_from_movies_file (
          "../presubmit.in_m");

  std::vector<RSUser> users = RSUsersLoader::create_users_from_file (
      "../RSUsersLoader_input.txt", std::move (rs));

  std::cout << users[0];
  std::vector<double> d = {9, 10, 1, 1};
  users[0].add_movie_to_rs ("Avatar", 2000, d, 2);
  RecommenderSystem a;
  a.add_movie ("s", 2, d);
  std::cout << a;
  return 0;
}