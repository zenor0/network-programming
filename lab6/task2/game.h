#include <stdlib.h> 
#include <time.h>



typedef struct _GameSession {
    int number;
    int guess_count;
    int last_guess;
    int score;

    int punish_score;
    double bonus_factor;
    int dealer_fd;
} GameSession;


void init_game_session(GameSession *game_session);
int guess_number(GameSession *game_session, int guess);
int calc_score(GameSession *game_session);

void set_game_arguments(GameSession *game_session, int punish_score, int bonus_factor);