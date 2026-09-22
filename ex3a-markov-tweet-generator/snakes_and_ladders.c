#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {
    {13, 4},
    {85, 17},
    {95, 67},
    {97, 58},
    {66, 89},
    {87, 31},
    {57, 83},
    {91, 25},
    {28, 50},
    {35, 11},
    {8, 30},
    {41, 62},
    {81, 43},
    {69, 32},
    {20, 39},
    {33, 70},
    {79, 99},
    {23, 76},
    {15, 47},
    {61, 14}
};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to; // cell which ladder leads to, if there is one
    int snake_to; // cell which snake leads to, if there is one
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/**
 * allocates memory for cells on the board and initalizes them
 * @param cells Array of pointer to Cell, represents game board
 * @return EXIT_SUCCESS if successful, else EXIT_FAILURE
 */
int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(cells[j]);
            }
            printf(ALLOCATION_ERROR_MESSAGE);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell){i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        } else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

int add_cells_to_database(MarkovChain *markov_chain, Cell *cells[BOARD_SIZE])
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        Node *tmp = add_to_database(markov_chain, cells[i]);
        if (tmp == NULL)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int set_nodes_frequencies(MarkovChain *markov_chain, Cell *cells[BOARD_SIZE])
{
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain, cells[i])->data;
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain,
                                             cells[index_to])->data;
            int res = add_node_to_frequency_list(from_node, to_node);
            if (res == EXIT_FAILURE)
            {
                return EXIT_FAILURE;
            }
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell *) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])->data;
                int res = add_node_to_frequency_list(from_node, to_node);
                if (res == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int fill_database_snakes(MarkovChain *markov_chain)
{
    Cell *cells[BOARD_SIZE];
    if (create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (add_cells_to_database(markov_chain, cells) == EXIT_FAILURE)
    {
        for (size_t i = 0; i < BOARD_SIZE; i++)
        {
            free(cells[i]);
        }
        return EXIT_FAILURE;
    }

    if(set_nodes_frequencies(markov_chain, cells) == EXIT_FAILURE)
    {
        for (size_t i = 0; i < BOARD_SIZE; i++)
        {
            free(cells[i]);
        }
        return EXIT_FAILURE;
    }

    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static void print_cell(void *data) {
    Cell *cell = (Cell *)data;
    if (cell->number == 100) {
        printf("[%d]", cell->number);
    } else if (cell->snake_to != EMPTY) {
        printf("[%d] -snake to->", cell->number);
    } else if (cell->ladder_to != EMPTY) {
        printf("[%d] -ladder to->", cell->number);
    } else {
        printf("[%d] ->", cell->number);
    }
}

static int comp_cells(void *data1, void *data2) {
    Cell *c1 = (Cell *)data1;
    Cell *c2 = (Cell *)data2;
    return c1->number - c2->number;
}

static void free_cell(void *data) {
    free(data);
}

static void *copy_cell(void *data) {
    Cell *new_cell = malloc(sizeof(Cell));
    if (!new_cell) return NULL;
    *new_cell = *(Cell *)data;
    return new_cell;
}

static bool is_last_cell(void *data) {
    Cell *cell = (Cell *)data;
    return cell->number == 100;
}

static MarkovChain *initialize_chain(void) {
    MarkovChain *chain = malloc(sizeof(MarkovChain));
    if (!chain) {
        return NULL;
    }
    chain->database = malloc(sizeof(LinkedList));
    if (!chain->database) {
        free(chain);
        return NULL;
    }
    chain->database->first = NULL;
    chain->database->last = NULL;
    chain->database->size = 0;

    chain->print_func = print_cell;
    chain->comp_func = comp_cells;
    chain->free_data = free_cell;
    chain->copy_func = copy_cell;
    chain->is_last = is_last_cell;
    return chain;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(NUM_ARGS_ERROR "\n");
        return EXIT_FAILURE;
    }

    int seed = (int)strtol(argv[1], NULL, 10);
    int num_routes = (int)strtol(argv[2], NULL, 10);
    srand(seed);

    MarkovChain *chain = initialize_chain();
    if (!chain) {
        printf(ALLOCATION_ERROR_MESSAGE "\n");
        return EXIT_FAILURE;
    }

    if (fill_database_snakes(chain) == EXIT_FAILURE) {
        printf(ALLOCATION_ERROR_MESSAGE "\n");
        free_database(&chain);
        return EXIT_FAILURE;
    }

    Cell start_cell = {1, EMPTY, EMPTY};
    Node *start_node = get_node_from_database(chain, &start_cell);

    for (int i = 1; i <= num_routes; i++) {
        printf("Random Walk %d: ", i);
        generate_random_sequence(chain, start_node->data, MAX_GENERATION_LENGTH);
        printf("\n");
    }

    free_database(&chain);
    return EXIT_SUCCESS;
}