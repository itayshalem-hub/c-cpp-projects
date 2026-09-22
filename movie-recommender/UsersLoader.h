//
// Created on 2/21/2022.
//
#include "User.h"
#ifndef USERFACTORY_H
#define USERFACTORY_H
#include <vector>
#include <string>
#include "RecommendationSystem.h"


class UsersLoader {
public:
    static std::vector<User>create_users(const std::string& users_file_path, std::shared_ptr<RecommendationSystem> rs);

};
#endif //USERFACTORY_H
