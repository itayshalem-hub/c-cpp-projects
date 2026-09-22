//
// Created by idan alperin on 04/09/2022.
//

#ifndef _DEATH_NEW_H_
#define _DEATH_NEW_H_

// this is to negate any invalid access
#define NEW_SLACK_FACTOR 2

int counter = 0;

void *operator new[](size_t size)
{
    std::cout << "new[] operator allocating " << size << " bytes (counter=" << counter << ")" << std::endl;
    if(!counter--)
    {
        throw std::bad_alloc();
    }

    return calloc(size, NEW_SLACK_FACTOR);;
}

void operator delete[](void *p) noexcept
{
    std::cout << "delete[] operator overloading " << std::endl;
    free(p);
}

#endif //_DEATH_NEW_H_
