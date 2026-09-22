
#ifndef EX5_MOVIE_H
#define EX5_MOVIE_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#define HASH_START 17



class Movie;

typedef std::shared_ptr<Movie> sp_movie; // define your smart pointer

/**
 * those declartions and typedefs are given to you and should be used in the ex
 */
typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);

std::size_t sp_movie_hash(const sp_movie& movie);

bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

class Movie
{
    std::string _name;
    int _year;
public:
    std::string get_name() const;
    int get_year() const;
    Movie(std::string name,int year);
    friend std::ostream& operator<<(std::ostream& os, const Movie& m);
    bool operator<(const Movie& other)const;

};


#endif //EX5_MOVIE_H
