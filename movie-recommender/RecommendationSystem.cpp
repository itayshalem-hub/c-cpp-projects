#include "RecommendationSystem.h"
#include "User.h"
#include <cmath>
#include <queue>
#include <algorithm>

RecommendationSystem::RecommendationSystem(){}
sp_movie RecommendationSystem::add_movie_to_rs(const std::string& name, int year, const std::vector<double>& features) {
    for (const auto& f: features) {
        if (f < 1 || f > 10) {
            throw std::invalid_argument("Features must be between 1 and 10");
        }
    }
    sp_movie movie = std::make_shared<Movie>(name,year);

    auto it = _movies.find(movie);
    if (it !=_movies.end()) {
        it->second = features;
        return it->first;
    }
    _movies[movie] = features;
    return movie;
}
sp_movie RecommendationSystem::get_movie(const std::string& name, int year) const {
    sp_movie movie = std::make_shared<Movie>(name,year);
    auto it = _movies.find(movie);
    if (it ==_movies.end()) {
        return nullptr;
    }
    return it->first;
}
// מכפלה סקלארית בין שני וקטורים
double RecommendationSystem::dot_product(const std::vector<double>& a, const std::vector<double>& b)const {
    double sum = 0;
    for (size_t i = 0; i < a.size(); ++i) sum += a[i] * b[i];
    return sum;
}

// חישוב נורמה של וקטור
double RecommendationSystem::norm(const std::vector<double>& a)const {
    double sum = 0;
    for (double val : a) sum += val * val;
    return std::sqrt(sum);
}

double RecommendationSystem::cosine_similarity(const std::vector<double>& a, const std::vector<double>& b)const
{
    return dot_product(a, b) / (norm(a) * norm(b)); // [cite: 81]
}
sp_movie RecommendationSystem::recommend_by_content(const User& user) const {
    const rank_map& ranks = user.get_rank();

    if (ranks.empty() || _movies.empty()) return nullptr;

    double avg_rank = 0;
    for (const auto& pair : ranks) avg_rank += pair.second;
    avg_rank /= ranks.size();

    size_t num_features = _movies.begin()->second.size();
    std::vector<double> pref_vec(num_features, 0.0);

    for (const auto& pair : ranks) {
        double normalized = pair.second - avg_rank;
        const auto& features = _movies.at(pair.first);
        for (size_t i = 0; i < num_features; ++i) {
            pref_vec[i] += normalized * features[i];
        }
    }

    sp_movie best_movie = nullptr;
    double max_sim = -2.0;

    for (const auto& pair : _movies) {
        if (ranks.find(pair.first) == ranks.end()) {
            double sim = cosine_similarity(pair.second, pref_vec);
            if (sim > max_sim) {
                max_sim = sim;
                best_movie = pair.first;
            }
        }
    }
    return best_movie;
}
#include <vector>
#include <algorithm>

double RecommendationSystem::predict_movie_score(const User& user, const sp_movie& movie, int k) const {
    const rank_map& ranks = user.get_rank();
    const auto& target_features = _movies.at(movie);

    std::vector<std::pair<double, double>> sims;

    for (const auto& pair : ranks) {
        double sim = cosine_similarity(_movies.at(pair.first), target_features);
        sims.push_back({sim, pair.second});
    }

    std::sort(sims.rbegin(), sims.rend());

    double numerator = 0;
    double denominator = 0;
    int limit = std::min(k, (int)sims.size());

    for (int i = 0; i < limit; ++i) {
        numerator += sims[i].first * sims[i].second;
        denominator += sims[i].first;
    }

    return (denominator == 0) ? 0 : (numerator / denominator);
}
sp_movie RecommendationSystem::recommend_by_cf(const User& user, int k) const {
    const rank_map& ranks = user.get_rank();
    sp_movie best_movie = nullptr;
    double max_score = -1.0;

    for (const auto& pair : _movies) {
        if (ranks.find(pair.first) == ranks.end()) {
            double score = predict_movie_score(user, pair.first, k);
            if (score > max_score) {
                max_score = score;
                best_movie = pair.first;
            }
        }
    }
    return best_movie;
}
std::ostream& operator<<(std::ostream& os, const RecommendationSystem& rs) {
    for (const auto& pair:rs._movies) {
        os<<*(pair.first);
    }
    return os;
}
