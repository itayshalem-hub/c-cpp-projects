//
// Created by 24565 on 2/22/2022.
//

#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"

#define EXIT_FAIL_TEST 2
#define EXIT_SUCCESS_TEST 0


int main(){


    auto rs = RecommendationSystemLoader::create_rs_from_movies("TestComparison_4.in_m");
    std::vector<User> user = UsersLoader::create_users("TestComparison_4.in_u", std::move(rs));
    const User user1 = user[0];
    if (user1.get_name() != "Amani" || user[1].get_name() != "Lauren" || user[2].get_name() != "Karsyn" || user[3].get_name() != "Michelle")
        return EXIT_FAIL_TEST;

    return EXIT_SUCCESS_TEST;
}
