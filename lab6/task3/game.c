#include "game.h"
#define BASE_SCORE 100

void init_game_session(GameSession *game_session) {
    game_session->number = rand() % 100 + 1;
    game_session->bonus_factor = 1;
    game_session->punish_score = 10;
}
int guess_number(GameSession *game_session, PlayerSession *player_session, int guess) {
    player_session->guess_count++;
    player_session->last_guess = guess;

    // add score depend by guess and precision of this guess
    int score = calc_score(game_session, player_session);
    player_session->score -= score;

    if (guess == game_session->number) {
        return 0;
    } else if (guess < game_session->number) {
        return -1;
    } else {
        return 1;
    }
}

void init_player_session(PlayerSession *player_session, int fd) {
    player_session->fd = fd;
    player_session->score = BASE_SCORE;
    player_session->guess_count = 0;
    player_session->last_guess = 0;

}
int calc_score(GameSession *game_session, PlayerSession *player_session) {
    // calc by last guess and random number, depend by the distance
    double precession_factor = (double)abs(player_session->last_guess - game_session->number) / 10;
    return ((player_session->guess_count * precession_factor) + game_session->punish_score) * game_session->bonus_factor;
}

void set_game_arguments(GameSession *game_session, int punish_score, int bonus_factor) {
    game_session->punish_score = punish_score;
    game_session->bonus_factor = bonus_factor;
}

int find_last_score(PlayerSession *player_session[], int cnt) {
    // return the minimal score's fd
    int min_score = player_session[0]->score;
    int min_fd = -1;
    for (int i = 0; i < cnt; i++) {
        if (player_session[i]->score < min_score) {
            min_score = player_session[i]->score;
            min_fd = player_session[i]->fd;
        }
    }
    return min_fd;
}
PlayerSession *find_player_session(PlayerSession **player_session, int cnt, int fd) {
    // return the player session by fd
    for (int i = 0; i < cnt; i++) {
        if (player_session[i]->fd == fd) {
            return player_session[i];
        }
    }
    return NULL;
}