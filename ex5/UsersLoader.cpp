#include "UsersLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include "User.h"

std::vector<User> UsersLoader::create_users(const std::string& users_file_path, std::shared_ptr<RecommendationSystem> rs) {
    std::ifstream infile(users_file_path);
    if (!infile) {
        throw std::invalid_argument("File format error or file not found");
    }

    std::vector<User> users_list;
    std::string line;

    if (!std::getline(infile, line)) {
        return users_list;
    }

    std::istringstream movies_iss(line);
    std::string movie_token;
    std::vector<sp_movie> movies;

    while (movies_iss >> movie_token) {
        size_t hyphen_pos = movie_token.rfind('-');
        std::string name = movie_token.substr(0, hyphen_pos);
        int year = std::stoi(movie_token.substr(hyphen_pos + 1));

        movies.push_back(rs->get_movie(name, year));
    }

    while (std::getline(infile, line)) {
        // מניעת קריסה משורות ריקות או תווי ירידת שורה עודפים
        if (line.find_first_not_of(" \r\n\t") == std::string::npos) continue;

        std::istringstream iss(line);
        std::string username;
        iss >> username;

        // אתחול מאובטח עם פונקציות הגיבוב
        rank_map ranks(0, sp_movie_hash, sp_movie_equal);

        std::string rate_str;
        int movie_idx = 0;
        while (iss >> rate_str) {
            if (rate_str != "NA") {
                double rating = std::stod(rate_str);
                ranks[movies[movie_idx]] = rating;
            }
            movie_idx++;
        }

        users_list.push_back(User(username, ranks, rs));
    }

    return users_list;
}