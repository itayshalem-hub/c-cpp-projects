#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include "Movie.h"
#include <map>
#include <vector>
#include <iostream>

class User;

struct sp_movie_compare {
    bool operator()(const sp_movie& a, const sp_movie& b) const {
        return *a < *b;
    }
};

class RecommendationSystem {
private:
    std::map<sp_movie, std::vector<double>, sp_movie_compare> _movies;

    // מכפלה סקלארית בין שני וקטורים
    double dot_product(const std::vector<double>& a, const std::vector<double>& b)const;

    // חישוב נורמה של וקטור
    double norm(const std::vector<double>& a)const;

    // חישוב דמיון קוסינוס לפי הנוסחה
    double cosine_similarity(const std::vector<double>& a, const std::vector<double>& b)const;
public:
    RecommendationSystem();
    
    sp_movie add_movie_to_rs(const std::string& name, int year, const std::vector<double>& features);
    sp_movie get_movie(const std::string& name, int year) const;
    sp_movie recommend_by_content(const User& user) const;
    double predict_movie_score(const User& user, const sp_movie& movie, int k) const;
    sp_movie recommend_by_cf(const User& user, int k) const;
    
    friend std::ostream& operator<<(std::ostream& os, const RecommendationSystem& rs);
};

#endif // RECOMMENDATIONSYSTEM_H