#include "test_bus_lines.h"
//TODO add implementation here

int is_sorted_by_distance (const BusLine *start, const BusLine *end) {
    while (start<end) {
        if (start->distance>(start+1)->distance) {
            return 0;
        }
        start++;
    }
    return 1;
}
/**
 * TODO add documentation
 */
int is_sorted_by_duration (const BusLine *start, const BusLine *end) {
    while (start<end) {
        if (start->duration>(start+1)->duration) {
            return 0;
        }
        start++;
    }
    return 1;
}
/**
 * TODO add documentation
 */
int is_sorted_by_frequency (const BusLine *start, const BusLine *end){
    while (start<end) {
        if (start->frequency>(start+1)->frequency) {
            return 0;
        }
        start++;
    }
    return 1;
}
/**
 * TODO add documentation
 */
int is_sorted_by_name (const BusLine *start, const BusLine *end){
    while (start<end) {
        if (strcmp(start->name,(start+1)->name)>0) {
            return 0;
        }
        start++;
    }
    return 1;
}
/**
 * TODO add documentation
 */
int is_equal (const BusLine *start_sorted,
              const BusLine *end_sorted,
              const BusLine *start_original,
              const BusLine *end_original) {
    if (end_sorted-start_sorted != end_original-start_original) {
        return 0;
    }
    const BusLine *start= start_sorted;
    int flag = 0;
    while (start_original<=end_original) {
        start_sorted = start;
        while (start_sorted<=end_sorted) {
            if (strcmp(start_original->name,start_sorted->name) == 0) {
                flag = 1;
                break;
            }
            start_sorted++;
        }
        if (!(flag)) {
            return 0;
        }
        flag = 0;
        start_original++;
    }
    return 1;
}