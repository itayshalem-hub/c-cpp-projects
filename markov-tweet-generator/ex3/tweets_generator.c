#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"
#include "linked_list.h"

#define MAX_LINE_LENGTH 1000
#define MAX_TWEET_WORDS 20

//Don't change the macros!
#define FILE_PATH_ERROR "Error: incorrect file path\n"
#define NUM_ARGS_ERROR  "Usage: tweets_generator <seed> <num_tweets> "\
"<file_path> [words_to_read] [seed_word]\n"

#define DELIMITERS " \n\t\r"

// הוספת המאקרו שחסר עבור שגיאת הקצאת זיכרון
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate new memory\n"

/**
 * מילוי מבנה הנתונים מקובץ הטקסט
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char line[MAX_LINE_LENGTH + 2]; // תמיכה באורך שורה כפי שמוגדר בהנחות [cite: 118]
    int words_read = 0;
    MarkovNode *prev_node = NULL;

    // קריאת שורות מתוך הקורפוס [cite: 83]
    while (fgets(line, sizeof(line), fp) != NULL) {
        // פירוק השורה למילים בעזרת strtok והמקרואים שהוגדרו [cite: 83, 121]
        char *token = strtok(line, DELIMITERS);

        while (token != NULL) {
            // עצירה במידה והגענו למספר המילים המבוקש [cite: 98, 123]
            if (words_to_read != -1 && words_read >= words_to_read) {
                return EXIT_SUCCESS;
            }

            // הוספת המילה הנוכחית ל-database [cite: 169, 189]
            Node *node = add_to_database(markov_chain, token);
            if (node == NULL) {
                return EXIT_FAILURE;
            }
            MarkovNode *curr_node = node->data;

            // עדכון רשימת התדירויות של המילה הקודמת [cite: 191, 192]
            if (prev_node != NULL) {
                if (add_node_to_frequency_list(prev_node, curr_node) == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
            }

            // בדיקה האם המילה הנוכחית היא מילה מסיימת משפט (תו אחרון הוא נקודה) [cite: 87, 123]
            int len = strlen(curr_node->data);
            if (len > 0 && curr_node->data[len - 1] == '.') {
                prev_node = NULL; // מאפסים את המילה הקודמת לקראת משפט חדש
            } else {
                prev_node = curr_node;
            }

            words_read++;
            token = strtok(NULL, DELIMITERS); // מעבר למילה הבאה בשורה [cite: 79]
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    // 1. בדיקת תקינות כמות הארגומנטים והדפסת שגיאה במקרה הצורך [cite: 114, 115]
    if (argc < 4 || argc > 6) {
        printf("%s", NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }

    // 2. שליפת הארגומנטים ואתחול מחולל המספרים הפסאודו-אקראיים [cite: 92, 93, 94]
    unsigned int seed = atoi(argv[1]);
    srand(seed); // אתחול ה-seed של המחולל [cite: 71, 92]

    int num_tweets = atoi(argv[2]);
    char *file_path = argv[3];

    int words_to_read = -1;
    if (argc >= 5) {
        words_to_read = atoi(argv[4]); // קריאת כמות מילים מוגבלת אם סופקה [cite: 98, 107]
    }

    char *seed_word = NULL;
    if (argc == 6) {
        seed_word = argv[5]; // קריאת מילת התחלה אם סופקה [cite: 100]
    }

    // 3. פתיחת קובץ הקורפוס ובדיקת תקינות הנתיב [cite: 112, 113]
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf("%s", FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }

    // 4. הקצאה ואתחול של שרשרת המרקוב ומבני הנתונים [cite: 141, 148]
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (markov_chain == NULL) {
        // במקרה של שגיאת הקצאה מדפיסים את ההודעה המובנית בשלד ויוצאים
        printf("%s", ALLOCATION_ERROR_MASSAGE);
        fclose(fp);
        return EXIT_FAILURE;
    }

    markov_chain->database = malloc(sizeof(LinkedList));
    if (markov_chain->database == NULL) {
        printf("%s", ALLOCATION_ERROR_MASSAGE);
        free(markov_chain);
        fclose(fp);
        return EXIT_FAILURE;
    }
    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    markov_chain->database->size = 0;

    // 5. מילוי ה-database מתוך הקובץ [cite: 241, 242]
    if (fill_database(fp, words_to_read, markov_chain) == EXIT_FAILURE) {
        printf("%s", ALLOCATION_ERROR_MASSAGE);
        free_database(&markov_chain); // שחרור כל המשאבים שהוקצו עד כה [cite: 244, 332]
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    // 6. יצירת והדפסת הציוצים ל-stdout [cite: 87, 321]
    for (int i = 1; i <= num_tweets; i++) {
        printf("Tweet %d: ", i); // הדפסת פורמט תחילת השורה [cite: 322, 324]

        MarkovNode *first_node = NULL;
        if (seed_word != NULL) {
            // אם סופקה מילת התחלה, נשתמש בה [cite: 100, 109]
            Node *node = get_node_from_database(markov_chain, seed_word);
            if (node != NULL) {
                first_node = node->data;
            }
        }

        // אם לא סופקה מילת התחלה או שהמילה לא נמצאה, מגרילים מילה ראשונה [cite: 101, 268]
        if (first_node == NULL) {
            first_node = get_first_random_node(markov_chain);
        }

        // יצירת הציוץ באורך מקסימלי של 20 מילים [cite: 319, 325]
        generate_tweet(first_node, MAX_TWEET_WORDS);
    }

    // 7. שחרור כלל המשאבים בסיום הריצה התקינה [cite: 235, 331]
    free_database(&markov_chain);
    return EXIT_SUCCESS;
}