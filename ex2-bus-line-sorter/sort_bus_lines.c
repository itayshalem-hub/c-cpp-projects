#include "sort_bus_lines.h"



//TODO add implemen+ dtation here

void bus_bubble_sort (BusLine *start, BusLine *end)
{
    BusLine *current_end = end;
    for ( BusLine *i = start;i< end ; i++) {
        for ( BusLine *j = start; j<current_end ; j++ ) {
            if (strcmp((j)->name, (j+1)->name) > 0) {
                BusLine temp = *(j+1);
                *(j+1) = *j;
                *j = temp;
            }
        }
        current_end -=1;
    }
}
void bus_quick_sort (BusLine *start, BusLine *end, SortType sort_type)
{
    if (start >= end)
    {
        return;
    }
    BusLine *pivot_ptr = partition(start, end, sort_type);
    bus_quick_sort(start, pivot_ptr - 1, sort_type);
    bus_quick_sort(pivot_ptr + 1, end, sort_type);
}
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type)
{
    BusLine *boundary = start;
    int should_swap = 0;
    for (BusLine *i = start; i < end; i++)
    {
        should_swap = 0;
        if (sort_type == DISTANCE && i->distance < end->distance){
            should_swap = 1;}
        else if (sort_type == DURATION && i->duration < end->duration){
            should_swap = 1;}
        else if (sort_type == FREQUENCY && i->frequency < end->frequency){
            should_swap = 1;}
        if (should_swap){
            BusLine temp = *boundary;
            *boundary = *i;
            *i = temp;
            boundary++;
        }
    }
    BusLine temp = *boundary;
    *boundary = *end;
    *end = temp;
    return boundary;
}