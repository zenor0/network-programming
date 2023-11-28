#include "game.h"
#define BASE_SCORE 100

void init_game_session(GameSession *game_session) {
    game_session->number = rand() % 100 + 1;
    game_session->guess_count = 0;
    game_session->last_guess = 0;
    game_session->score = BASE_SCORE;
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
    return (game_session->guess_count * precession_factor) + 10;
}