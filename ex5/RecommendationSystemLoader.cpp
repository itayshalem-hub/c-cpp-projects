#include "RecommendationSystemLoader.h"
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <sstream>

const double MIN_FEATURE = 1.0;
const double MAX_FEATURE = 10.0;

std::unique_ptr<RecommendationSystem> RecommendationSystemLoader::create_rs_from_movies(const std::string& file) {
    std::ifstream infile(file);
    if (!infile) {
        throw std::invalid_argument("File format error or file not found");
    }

    auto rs = std::make_unique<RecommendationSystem>();
    std::string line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string movie_token;
        iss >> movie_token;

        size_t hyphen_pos = movie_token.rfind('-');
        std::string movie_name = movie_token.substr(0, hyphen_pos);
        int year = std::stoi(movie_token.substr(hyphen_pos + 1));

        std::vector<double> features;
        double feature;

        while (iss >> feature) {
            if (feature < MIN_FEATURE || feature > MAX_FEATURE) {
                throw std::invalid_argument("Feature score must be between 1 and 10");
            }
            features.push_back(feature);
        }

        rs->add_movie_to_rs(movie_name, year, features);
    }

    return rs;
}