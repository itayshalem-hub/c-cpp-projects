#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"
#include "linked_list.h"

#define MAX_LINE_LENGTH 1000
#define MAX_TWEET_WORDS 20
#define EXPECTED_MIN_ARGS 4
#define EXPECTED_MAX_ARGS 6

#define SEED_ARG_IDX 1
#define TWEETS_ARG_IDX 2
#define PATH_ARG_IDX 3
#define WORDS_ARG_IDX 4
#define SEED_WORD_ARG_IDX 5

//Don't change the macros!
#define FILE_PATH_ERROR "Error: incorrect file path\n"
#define NUM_ARGS_ERROR  "Usage: tweets_generator <seed> <num_tweets> "\
"<file_path> [words_to_read] [seed_word]\n"

#define DELIMITERS " \n\t\r"

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate new memory\n"

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char line[MAX_LINE_LENGTH + 2];
    int words_read = 0;
    MarkovNode *prev_node = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        char *token = strtok(line, DELIMITERS);

        while (token != NULL) {
            if (words_to_read != -1 && words_read >= words_to_read) {
                return EXIT_SUCCESS;
            }

            Node *node = add_to_database(markov_chain, token);
            if (node == NULL) {
                return EXIT_FAILURE;
            }
            MarkovNode *curr_node = node->data;

            if (prev_node != NULL) {
                if (add_node_to_frequency_list(prev_node, curr_node) == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
            }

            int len = strlen(curr_node->data);
            if (len > 0 && curr_node->data[len - 1] == '.') {
                prev_node = NULL;
            } else {
                prev_node = curr_node;
            }

            words_read++;
            token = strtok(NULL, DELIMITERS);
        }
    }
    return EXIT_SUCCESS;
}

static void generate_all_tweets(MarkovChain *chain, int num_tweets, char *seed_word) {
    for (int i = 1; i <= num_tweets; i++) {
        printf("Tweet %d: ", i);

        MarkovNode *first_node = NULL;
        if (seed_word != NULL) {
            Node *node = get_node_from_database(chain, seed_word);
            if (node != NULL) {
                first_node = node->data;
            }
        }

        if (first_node == NULL) {
            first_node = get_first_random_node(chain);
        }

        generate_tweet(first_node, MAX_TWEET_WORDS);
    }
}

static MarkovChain* init_markov_chain(void) {
    MarkovChain *chain = malloc(sizeof(MarkovChain));
    if (chain == NULL) {
        return NULL;
    }

    chain->database = malloc(sizeof(LinkedList));
    if (chain->database == NULL) {
        free(chain);
        return NULL;
    }

    chain->database->first = NULL;
    chain->database->last = NULL;
    chain->database->size = 0;
    return chain;
}

int main(int argc, char *argv[]) {
    if (argc < EXPECTED_MIN_ARGS || argc > EXPECTED_MAX_ARGS) {
        printf("%s", NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }

    unsigned int seed;
    sscanf(argv[SEED_ARG_IDX], "%u", &seed);
    srand(seed);

    int num_tweets;
    sscanf(argv[TWEETS_ARG_IDX], "%d", &num_tweets);

    int words_to_read = -1;
    if (argc >= EXPECTED_MIN_ARGS + 1) {
        sscanf(argv[WORDS_ARG_IDX], "%d", &words_to_read);
    }

    char *seed_word = NULL;
    if (argc == EXPECTED_MAX_ARGS) {
        seed_word = argv[SEED_WORD_ARG_IDX];
    }

    FILE *fp = fopen(argv[PATH_ARG_IDX], "r");
    if (fp == NULL) {
        printf("%s", FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }

    MarkovChain *markov_chain = init_markov_chain();
    if (markov_chain == NULL) {
        printf("%s", ALLOCATION_ERROR_MASSAGE);
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (fill_database(fp, words_to_read, markov_chain) == EXIT_FAILURE) {
        printf("%s", ALLOCATION_ERROR_MASSAGE);
        free_database(&markov_chain);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    generate_all_tweets(markov_chain, num_tweets, seed_word);

    free_database(&markov_chain);
    return EXIT_SUCCESS;
}