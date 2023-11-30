#include "game.h"
#define BASE_SCORE 100

void init_game_session(GameSession *game_session) {
    game_session->number = rand() % 100 + 1;
    game_session->guess_count = 0;
    game_session->last_guess = 0;
    game_session->score = BASE_SCORE;

    game_session->bonus_factor = 1;
    game_session->punish_score = 10;
}
int guess_number(GameSession *game_session, int guess) {
    game_session->guess_count++;
    game_session->last_guess = guess;

    // add score depend by guess and precision of this guess
    int score = calc_score(game_session);
    game_session->score -= score;

    if (guess == game_session->number) {
        return 0;
    } else if (guess < game_session->number) {
        return -1;
    } else {
        return 1;
    }
}
//calc result by guesscount
int calc_score(GameSession *game_session) {
    // calc by last guess and random number, depend by the distance
    double precession_factor = (double)abs(game_session->last_guess - game_session->number) / 10;
    return ((game_session->guess_count * precession_factor) + game_session->punish_score) * game_session->bonus_factor;
}

void set_game_arguments(GameSession *game_session, int punish_score, int bonus_factor) {
    game_session->punish_score = punish_score;
    game_session->bonus_factor = bonus_factor;
}