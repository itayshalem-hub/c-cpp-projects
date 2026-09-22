#include "User.h"
#include "Movie.h"
#include "Movie.h"
#include "RecommendationSystem.h"
#include <string>


std::string User::get_name() const{
    return this->_name;
}
const rank_map& User::get_rank() const {
    return this->_map;
}
std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "name: " << u._name << "\n";
    os << *(u._rs);
    os << "\n";
    return os;
}
User::User(const std::string& name, const rank_map& map, std::shared_ptr<RecommendationSystem> rs)
    : _name(name), _map(0, sp_movie_hash, sp_movie_equal), _rs(rs)
{
    for (const auto& pair : map) {
        _map[pair.first] = pair.second;
    }
}



void User::add_movie_to_user(const std::string &name, int year,
                             const std::vector<double> &features,
                             double rate) {
    for (const auto& f: features) {
        if (f < 1 || f > 10) {
            throw std::invalid_argument("Features must be between 1 and 10");
        }
    }
    sp_movie movie_ptr = _rs->add_movie_to_rs(name, year, features);

    _map[movie_ptr] = rate;
}
sp_movie User::get_rs_recommendation_by_content()const{
    return _rs->recommend_by_content(*this);
}

double User::get_rs_prediction_score_for_movie(const std::string& name, int year, int k) const {
    sp_movie movie = _rs->get_movie(name, year);
    return _rs->predict_movie_score(*this, movie, k);
}

sp_movie User::get_rs_recommendation_by_cf(int k) const {
    return _rs->recommend_by_cf(*this, k);
}