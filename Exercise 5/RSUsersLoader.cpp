#include "RSUsersLoader.h"

std::vector<RSUser>
RSUsersLoader::create_users_from_file
    (const std::string &users_file_path,
     std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  sp_rec_system rss = std::move (rs);
  std::vector<RSUser> users_vec;
  std::istringstream input_file (*read_once (users_file_path));
  std::string line;
  std::getline (input_file, line);
  std::vector<Movie> movie_arr = extract_movies_list (line);
  while (std::getline (input_file, line))
    {
      sp_rec_system prs = rss;  // shared
      size_t pos = line.find (SEP);
      std::string user_name = line.substr (0, pos);
      line.erase (0, pos + 1);
      std::vector<double> user_ranks = extract_user_ranks (line);
      rank_map user_map = create_rank_map (movie_arr, user_ranks);
      users_vec.emplace_back (RSUser (user_name, user_map, prs));
    }
  return users_vec;
}

std::vector<Movie> extract_movies_list (std::string &movies_line)
{
  std::vector<Movie> movie_arr;
  std::vector<std::string> string_movies;
  size_t pos;
  while ((pos = movies_line.find (SEP)) != std::string::npos)
    {
      string_movies.push_back (movies_line.substr (0, pos));
      movies_line.erase (0, pos + 1);
    }
  pos = movies_line.find (SEP);
  string_movies.push_back (movies_line.substr (0, pos));
  movies_line.erase (0, pos + 1);
  for (const auto &str: string_movies)
    {
      size_t diff = str.find (MAKAF);
      std::string movie_year = str.substr (diff + 1);
      const std::string movie_name = str.substr (0, diff);
      Movie new_movie (movie_name, std::stoi (movie_year));
      movie_arr.push_back (new_movie);
    }
  return movie_arr;
}

std::vector<double> extract_user_ranks (std::string &movies_ranking)
{
  std::vector<std::string> user_movies_ranking;
  std::vector<double> final;
  size_t pos;
  while ((pos = movies_ranking.find (SEP)) != std::string::npos)
    {
      user_movies_ranking.push_back (movies_ranking.substr (0, pos));
      movies_ranking.erase (0, pos + 1);
    }
  movies_ranking.erase (movies_ranking.size ());
  user_movies_ranking.push_back (movies_ranking);
  for (const auto &str: user_movies_ranking)
    {
      if (is_number (str))
        {
          double rank = std::stod (str);
          if(check_input (rank))
            {
              final.push_back (rank);
            }
          else
            {
              throw std::length_error(LENGTH);
            }
        }
      else
        {
          final.push_back (NOT_FOUND);
        }
    }
  return final;
}

rank_map
create_rank_map (const std::vector<Movie> &movie_arr,
                 const std::vector<double> &user_ranks)
{
  rank_map new_map (0, sp_movie_hash, sp_movie_equal);
  for (int i = 0; i < (int) movie_arr.size (); i++)
    {
      sp_movie new_movie = std::make_shared<Movie> (movie_arr[i]);
      new_map[new_movie] = user_ranks[i];
    }
  return new_map;
}

bool is_number (const std::string &str)
{
  return (std::any_of (str.begin (), str.end (), [] (char const &c)
  {
      return std::isdigit (c) == 1;
  }));
}
