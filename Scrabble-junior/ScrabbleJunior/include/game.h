#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <ostream>
#include <random>
#include "player.h"
#include "board.h"
#include "pool.h"
#include "position.h"
#include "gameDisplayer.h"

// Represents what the current player must do
// in that turn.
enum TurnState {
    // Must choose a cell to cover.
    MUST_MOVE,
    // Must choose two letters to exchange with the pool.
    MUST_EXCHANGE_TWO,
    // Must choose one letter to exchange with the pool.
    MUST_EXCHANGE_ONE,
    // They can't move anymore. Their turn is over.
    MUST_END_TURN,
    // They can't move nor exchange at the start of the turn. Their turn is skipped.
    MUST_SKIP_TURN,
};

// Manages the game itself.
class Game {
    // The board of the game.
    Board &board;
    // The pool of the game.
    Pool pool;
    // The players of the game.
    std::vector<Player> players;
    // Helper class to help display and animate the state of the game.
    GameDisplayer displayer;
    // The index of current player.
    unsigned int current_player_index;
    // Number of moves left this turn.
    unsigned int moves_left;

    // Returns whether this game is over.
    bool isOver() const;
    // Returns the current player.
    const Player& getCurrentPlayer() const;
    // Returns the current player.
    Player& getCurrentPlayer();
    // Returns the ID of the winners. Must only be called after
    // game is over. Assumes that the players have already been
    // sorted for the leaderboard.
    std::vector<int> getWinnersId() const;

    // Returns what current player must do in current turn.
    TurnState getTurnState() const;
    // Starts the game loop until it is over.
    bool playLoop(std::default_random_engine &rng);

    // Checks whether this move must be restricted by the edge case of forcing to play twice
    // (see 'Board::mustPlayTwiceEdgeCase'), returning the correct checker for whether a move
    // is legal or not.
    //
    // Passes whether the edge case is detected to 'must_play_twice'.
    GameDisplayer::CheckLegalMove getCheckLegalMove(bool &must_play_twice) const;
    // Tries to parse 'position' from 'input', returning whether it was successful.
    bool parsePosition(std::istream &input, Position &position);
    // Returns whether a move to 'position' by the current player would be valid.
    //
    // If 'must_play_twice', checks 'is_legal' for the edge case.
    bool validateMove(Position position, bool must_play_twice, GameDisplayer::CheckLegalMove is_legal);
    // Covers the 'Cell' at 'position'. Should only be called if that move has been validated.
    void move(Position position);
    
    // Tries to parse 'letter' from 'input', returning whether it was successful.
    bool parseLetter(std::istream &input, char &letter);
    // Returns whether current player may exchange 'letter' with the 'Pool'.
    bool validateExchange(char letter);
    // Exchanges 'letter' with the 'Pool'. Should only be called if that exchange has been validated.
    void exchange(char letter, std::default_random_engine &rng);
    
    // Tries to parse two letters from 'input', returning whether it was successful.
    bool parseLetters(std::istream &input, char &letter1, char &letter2);
    // Returns whether current player may exchange 'letter1' and 'letter2' with the 'Pool'.
    bool validateExchange(char letter1, char letter2);
    // Exchanges 'letter1' and 'letter2' with the 'Pool'. Should only be called if that exchange has been validated.
    void exchange(char letter1, char letter2, std::default_random_engine &rng);

    // Advance to the next turn.
    void nextTurn();

    public:
    // Constructs a game with given 'Board' and number of players.
    Game(Board &board, unsigned int num_players);

    // Starts the game and plays it. Returns true if it ended successfuly,
    // and false if stdin has failed.
    bool play(std::default_random_engine &rng);
};

#endif