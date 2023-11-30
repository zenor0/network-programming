#include <stdlib.h> 
#include <time.h>
#include <stdbool.h>


typedef struct _GameSession {
    int number;
    int punish_score;
    double bonus_factor;
    int dealer_fd;
} GameSession;


typedef struct _PlayerSession {
    int fd;
    int score;
    int guess_count;
    int last_guess;
} PlayerSession;



void init_game_session(GameSession *game_session);
void init_player_session(PlayerSession *player_session, int fd);
int guess_number(GameSession *game_session, PlayerSession *player_session, int guess);
int calc_score(GameSession *game_session, PlayerSession *player_session);

void set_game_arguments(GameSession *game_session, int punish_score, int bonus_factor);

int find_last_score(PlayerSession **player_session, int cnt);
PlayerSession *find_player_session(PlayerSession **player_session, int cnt, int fd);