#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <string>
#define ERROR "The ranking value is out of range [1,10]"
#define RUN_TIME "CANNOT OPEN FILE"
#define MAKAF '-'
#define TEN 10
#define SPACE ' '

/**
 * The function checks all input in the valid range [1,10]
 * @param a
 * @return True if all valid and false otherwise
 */
bool check_input (double a);
std::shared_ptr<std::string> read_once (const std::string &path);

class RecommenderSystemLoader {
 private:
 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movie
   * @return shared pointer to a RecommenderSystem which
   * was created with those movies
   */
  static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file
      (const std::string &movies_file_path) noexcept (false);

};

#endif // RECOMMENDERSYSTEMLOADER_H
