cmake_minimum_required(VERSION 4.1)
project(ex5)

set(CMAKE_CXX_STANDARD 20)

# ריכוז קבצי המקור של המחלקות (ללא פונקציית main)
set(CORE_FILES
        Movie.cpp
        User.cpp
        RecommendationSystem.cpp
        RecommendationSystemLoader.cpp
        UsersLoader.cpp
)

# הגדרת מטרת הרצה (Target) נפרדת עבור כל טסט
add_executable(test_1 Test_1.cpp ${CORE_FILES})
add_executable(test_2 Test_2.cpp ${CORE_FILES})
add_executable(test_3 Test_3.cpp ${CORE_FILES})
add_executable(test_comp_1 TestComparison_1.cpp ${CORE_FILES})
add_executable(test_comp_2 TestComparison_2.cpp ${CORE_FILES})
add_executable(test_comp_3 TestComparison_3.cpp ${CORE_FILES})
add_executable(test_comp_4 TestComparison_4.cpp ${CORE_FILES})
add_executable(test_comp_8 TestComparison_8.cpp ${CORE_FILES})