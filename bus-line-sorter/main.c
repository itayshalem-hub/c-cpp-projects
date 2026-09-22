#include <stdio.h>
#include <stdlib.h>
#include "sort_bus_lines.h"
#include "test_bus_lines.h"

int test(BusLine *start, int num_of_lines) {
    BusLine *copy = malloc(num_of_lines * sizeof(BusLine));
    if (copy == NULL) {
        return EXIT_FAILURE;}
    for (int i = 0; i < num_of_lines; i++) {
        copy[i] = start[i];}
    bus_quick_sort(copy, copy + num_of_lines - 1, DISTANCE);
    if (is_sorted_by_distance(copy, copy + num_of_lines - 1)) {
        printf("TEST 1 PASSED: Sorted by distance\n");
    } else {
        printf("TEST 1 FAILED: Not sorted by distance\n");}
    if (is_equal(copy, copy + num_of_lines - 1, start, start + num_of_lines - 1)) {
        printf("TEST 2 PASSED: Array elements match original\n");
    } else {
        printf("TEST 2 FAILED: Array elements do not match original\n");}
    bus_quick_sort(copy, copy + num_of_lines - 1, DURATION);
    if (is_sorted_by_duration(copy, copy + num_of_lines - 1)) {
        printf("TEST 3 PASSED: Sorted by duration\n");
    } else {
        printf("TEST 3 FAILED: Not sorted by duration\n");}
    if (is_equal(copy, copy + num_of_lines - 1, start, start + num_of_lines - 1)) {
        printf("TEST 4 PASSED: Array elements match original\n");
    } else {
        printf("TEST 4 FAILED: Array elements do not match original\n");}
    bus_quick_sort(copy, copy + num_of_lines - 1, FREQUENCY);
    if (is_sorted_by_frequency(copy, copy + num_of_lines - 1)) {
        printf("TEST 5 PASSED: Sorted by frequency\n");
    } else {
        printf("TEST 5 FAILED: Not sorted by frequency\n");}
    if (is_equal(copy, copy + num_of_lines - 1, start, start + num_of_lines - 1)) {
        printf("TEST 6 PASSED: Array elements match original\n");
    } else {
        printf("TEST 6 FAILED: Array elements do not match original\n");}
    bus_bubble_sort(copy, copy + num_of_lines - 1);
    if (is_sorted_by_name(copy, copy + num_of_lines - 1)) {
        printf("TEST 7 PASSED: Sorted by name\n");
    } else {
        printf("TEST 7 FAILED: Not sorted by name\n");
    }
    if (is_equal(copy, copy + num_of_lines - 1, start, start + num_of_lines - 1)) {
        printf("TEST 8 PASSED: Array elements match original\n");
    } else {
        printf("TEST 8 FAILED: Array elements do not match original\n");}
    free(copy);
    return EXIT_SUCCESS;}
int get_num_of_lines() {
    int num_of_lines = 0;
    char input[60];
    while (1) {
        printf("Enter number of lines. Then enter\n");
        if (fgets(input, 60, stdin) == NULL) {
            return -1;
        }
        int result = sscanf(input, "%d", &num_of_lines);
        if (result == 1 && num_of_lines >= 1) {
            return num_of_lines;
        }
        printf("Error: one number bigger then 1 (includes)\n");
    }
}
int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: illegal number of arguments\n");
        return EXIT_FAILURE;}
    if (strcmp(argv[1], "by_name") != 0 && strcmp(argv[1], "by_frequency") != 0 &&
        strcmp(argv[1], "by_distance") != 0 && strcmp(argv[1], "by_duration") != 0 &&
        strcmp(argv[1], "test") != 0) {
        printf("Usage: wrong input\n");
        return EXIT_FAILURE;}
    int num_of_lines = get_num_of_lines();
    if (num_of_lines == -1) {
        return EXIT_FAILURE;}
    BusLine *lines_arr = malloc(num_of_lines * sizeof(BusLine));
    if (lines_arr == NULL) {
        return EXIT_FAILURE;}
    for (int i = 0; i < num_of_lines; i++) {
        int flag = 0;
        printf("Enter line info. Then enter\n");
        char line_input[60];
        fgets(line_input, 60, stdin);
        int line_info = sscanf(line_input, "%20[^,],%d,%d,%d",lines_arr[i].name,&lines_arr[i].distance,&lines_arr[i].duration,&lines_arr[i].frequency);
        if (line_info != 4) {
            printf("Error: enter 4 arguments\n");
            i--;
            continue;}
        for (int j = 0; j < strlen(lines_arr[i].name); j++) {
            if (!(lines_arr[i].name[j]<='9' && lines_arr[i].name[j]>='0' || lines_arr[i].name[j]>='a' && lines_arr[i].name[j]<='z')) {
                printf("Error: bus name should contains only digits and small chars\n");
                flag = 1;
                break;}}
        if (flag == 1) {
            i--;
            continue;}
        if (lines_arr[i].distance < 0 || lines_arr[i].distance > 1000) {
            printf("Error: distance should be between 0 to 1000\n");
            i--;
            continue;}
        if (lines_arr[i].duration < 10 || lines_arr[i].duration > 100) {
            printf("Error: duration should be between 10 to 100\n");
            i--;
            continue;}
        if (lines_arr[i].frequency < 1 || lines_arr[i].frequency > 50) {
            printf("Error: frequency should be between 1 to 150\n");
            i--;
            continue;}}
    if (strcmp(argv[1], "test") == 0) {
        test(lines_arr, num_of_lines);
    } else {
        if (strcmp(argv[1], "by_name") == 0) {
            bus_bubble_sort(lines_arr, lines_arr + num_of_lines - 1);}
        else if (strcmp(argv[1], "by_distance") == 0) {
            bus_quick_sort(lines_arr, lines_arr + num_of_lines - 1, DISTANCE);}
        else if (strcmp(argv[1], "by_duration") == 0) {
            bus_quick_sort(lines_arr, lines_arr + num_of_lines - 1, DURATION);}
        else if (strcmp(argv[1], "by_frequency") == 0) {
            bus_quick_sort(lines_arr, lines_arr + num_of_lines - 1, FREQUENCY);}
        for (int k = 0; k < num_of_lines; k++) {
            printf("%s,%d,%d,%d\n", lines_arr[k].name, lines_arr[k].distance, lines_arr[k].duration, lines_arr[k].frequency);}}
    free(lines_arr);
    return EXIT_SUCCESS;}

