#include <stdlib.h> 
#include <time.h>

typedef struct _GameSession {
    int number;
    int guess_count;
    int last_guess;
    int score;
} GameSession;


void init_game_session(GameSession *game_session);
int guess_number(GameSession *game_session, int guess);
int calc_score(GameSession *game_session);