#include "markov_chain.h"

#include <string.h>

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}
int is_end_of_sentence(MarkovNode *markov_node) {
    if (markov_node == NULL || markov_node->data == NULL) {
        return false;
    }
    int len = strlen(markov_node->data);
    if ((markov_node->data)[len-1] == '.') {
        return true;
    }
    return false;

}
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *curr = markov_chain->database->first;
    while (curr) {
        MarkovNode *current_markov_node = curr->data;
        if (strcmp((current_markov_node->data),data_ptr)==0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    Node *temp = get_node_from_database(markov_chain,data_ptr);
    if (temp != NULL) {
        return temp;
    }
    MarkovNode *new_markov_node = malloc((sizeof(MarkovNode)));

    if (new_markov_node==NULL) {
        return NULL;
    }
    new_markov_node->data = malloc(strlen(data_ptr) + 1);
    if (new_markov_node->data == NULL) {
        free(new_markov_node);
        return NULL;
    }

    strcpy(new_markov_node->data, data_ptr);
    new_markov_node->frequency_list = NULL;

    if (add(markov_chain->database, new_markov_node) == 1) {
        free(new_markov_node->data);
        free(new_markov_node);
        return NULL;
    }
    new_markov_node->list_size = 0;

    return markov_chain->database->last;
}

int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node) {
    for (int i = 0; i<first_node->list_size;i++) {
        if (second_node == first_node->frequency_list[i].markov_node) {
            first_node->frequency_list[i].frequency += 1;
            return EXIT_SUCCESS;
        }
    }
    MarkovNodeFrequency *temp_list = realloc(first_node->frequency_list, (first_node->list_size + 1) * sizeof(MarkovNodeFrequency));
    if (temp_list==NULL) {
        return EXIT_FAILURE;
    }
    temp_list[first_node->list_size].markov_node = second_node;
    temp_list[first_node->list_size].frequency = 1;
    first_node->list_size+=1;
    first_node->frequency_list =  temp_list;
    return EXIT_SUCCESS;
}
void free_database(MarkovChain ** ptr_chain) {
    Node *curr = (*ptr_chain)->database->first;

    while (curr) {
        free(curr->data->frequency_list);
        free(curr->data->data);
        free(curr->data);
        Node *next = curr->next;
        free(curr);
        curr=next;
    }
    free((*ptr_chain)->database);
    free(*ptr_chain);
    *ptr_chain = NULL;
}
MarkovNode* get_first_random_node(MarkovChain *markov_chain) {
    Node *curr = markov_chain->database->first;
    while (true){
        int num = get_random_number(markov_chain->database->size);
        for (int i = 0;i<num;i++) {
            curr = curr->next;
        }
        int len = strlen(curr->data->data);
        if ((curr->data->data)[len-1] != '.') {
            break;
        }
        curr = markov_chain->database->first;
    }
    return curr->data;
}
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node) {
    int total_num = 0;
    for (int i = 0;i<cur_markov_node->list_size;i++) {
        total_num+=cur_markov_node->frequency_list[i].frequency;
    }
    int rand_num = get_random_number(total_num);
    for (int i = 0;i<cur_markov_node->list_size;i++) {
        rand_num-=cur_markov_node->frequency_list[i].frequency;
        if (rand_num<0) {
            return cur_markov_node->frequency_list[i].markov_node;
        }
    }
    return NULL;
}
void generate_tweet(MarkovNode *first_node, int max_length) {
    if (first_node == NULL || max_length <= 0) {
        return;
    }
    printf("%s " , first_node->data);
    int i = 1;
    while (i<max_length) {
        MarkovNode *next =  get_next_random_node(first_node);
        if (next == NULL) {
            break;
        }
        if (is_end_of_sentence(next) == true) {
            printf("%s", next->data);
            break;

        }
        printf("%s ", next->data);

        first_node = next;
        i++;
    }
    printf("\n");
}
