#include "RecommenderSystemLoader.h"

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  std::istringstream input_file (*read_once (movies_file_path));
  RecommenderSystem rs;
  std::string movie_details;
  while (std::getline (input_file, movie_details))
    {
      std::size_t diff = movie_details.find (MAKAF);
      const std::string movie_name = movie_details.substr (0, diff);
      movie_details.erase (0, diff + 1);
      std::size_t pos = movie_details.find (SPACE);
      std::string movie_year = movie_details.substr (0, pos + 1);
      movie_details.erase (0, pos + 1);
      std::vector<double> features;
      while ((pos = movie_details.find (SPACE)) != std::string::npos)
        {
          double num = std::stoi (movie_details.substr (0, pos));
          movie_details.erase (0, pos + 1);
          if (check_input (num))
            {
              features.push_back (num);
            }
          else
            {
              throw std::length_error (ERROR);
            }
        }
      double last_num = std::stoi (movie_details);
      if (check_input (last_num))
        {
          features.push_back (last_num);
        }
      else
        {
          throw std::length_error (ERROR);
        }
      rs.add_movie (movie_name, std::stoi (movie_year), features);
    }
  return std::make_unique<RecommenderSystem> (rs);
}

bool check_input (double a)
{
  return ((a >= 1) && (a <= TEN));
}

std::shared_ptr<std::string> read_once (const std::string &path)
{
  std::ifstream is (path);
  if (is)
    {
      is.seekg (0, std::ifstream::end);
      int length = (int) is.tellg ();
      is.seekg (0, std::ifstream::beg);
      char *buffer = new char[length + 1];
      is.read (buffer, length);
      buffer[length] = '\0';
      if (!is)
        {
          throw std::runtime_error (RUN_TIME);
        }
      is.close ();
      std::string new_buffer = buffer;
      delete[] buffer;
      return std::make_shared<std::string>(new_buffer);
    }
  else
    {
      throw std::runtime_error (RUN_TIME);
    }
}
