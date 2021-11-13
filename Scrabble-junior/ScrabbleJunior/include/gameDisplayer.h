#ifndef GAME_DISPLAYER_H
#define GAME_DISPLAYER_H

#include <string>
#include <sstream>
#include <ostream>
#include <vector>
#include "board.h"
#include "position.h"
#include "cell.h"
#include "player.h"
#include "word.h"
#include "cmd.h"

// Helps to display and animate the state of the game to the user.
class GameDisplayer {
    // Color of each player, in the order of id's.
    static const Color PLAYERS_COLOR[];
    // Label for each place in the leaderboard, from best to worst.
    static const char* WINNER_LABELS[];
     
    // Number of milliseconds between highlighting each
    // letter when completing a word to the board.
    static const int WORD_COMPLETED_ANIMATION_DELAY;
    // Number of milliseconds of delay after increasing
    // the score because a word has been completed.
    static const int SCORE_INCREASE_DELAY;
    // Number of milliseconds of delay after swaping a
    // letter between a 'Hand' and a 'Pool'.
    static const int SWAP_LETTER_DELAY;
    // Number of milliseconds after printing a notice to 
    // the screen.
    static const int NOTICE_DELAY;
    // Number of milliseconds after printing a notice to 
    // the screen that doesn't take as much time to read.
    static const int SHORT_NOTICE_DELAY;
    // Number of seconds of delay after 'Hand' is refilled.
    static const int AFTER_REFILL_DELAY;

    // Horizontal offset until the column where player's hand is printed.
    // This is needed to know where to update letters in the player hand.
    static const unsigned int current_player_hand_x_offset;

    // The stream of error messages printed immediately
    // before prompting user for input.
    std::ostringstream error_messages;
    // Horizontal offset before starting the zone of the
    // screen that shows the scoreboard (or leaderboard,
    // when game is over).
    const unsigned int scoreboard_x_offset;
    // Vertical offset before starting the zone of the
    // screen where the information of the turn is printed.
    const unsigned int turn_info_y_offset;

    // Auxiliary method to print the given id in the respective
    // player's color. Optionally, a prefix can be added, which will
    // also be colored.
    //
    // This function guarantees to set the terminal color to 
    // 'TEXT_COLOR' at the end.
    static void printColoredId(int id, const char *prefix = "");

    // Auxiliary method to print the given word, possibly with delay
    // between each letter. This is used to highlight a completed word
    // and turn it back to normal.
    static void printWord(const Word &word, bool delay_each_letter);

    public:
    // Normal text color.
    static const Color TEXT_COLOR;
    // Color for erros.
    static const Color ERROR_COLOR;
    // Color for warnings.
    static const Color WARNING_COLOR;
    // Color of letters that were swapped in 'Hand'.
    static const Color SWAP_LETTER_COLOR;
    // Color of uncovered letters in board.
    static const Color LETTER_UNCOVERED_COLOR;
    // Color of covered letters in board.
    static const Color LETTER_COVERED_COLOR;
    // Color for score increasing.
    static const Color SCORE_COLOR;
    // Color of the background of the board.
    static const Color BOARD_BACKGROUND;
    // Color of the background of the board when highlighted.
    static const Color BOARD_HIGHLIGHTED_BACKGROUND;
    
    // A callback to check whether a move to the given Position
    // (where exists the given 'Cell') is legal.
    //
    // This is used to find out whether the board must highlight
    // such 'Cell' as being playable.
    typedef std::function<bool (Position, Cell)> CheckLegalMove;

    // Constructs a displayer for a 'Board' with given width and height.
    GameDisplayer(unsigned int board_width, unsigned int board_height); 

    // Returns the stream of error messages printed immediately
    // before prompting user for input.
    std::ostream& getErrorStream();
    // Clears all errors in the stream of error messages.
    void clearErrors();

    // Prints the given 'Board' to screen.
    // If 'check_legal_move' is passed, highlights 'Cell's recognized as legal.
    static void printBoard(const Board &board, CheckLegalMove check_legal_move = nullptr);
    // Prints the scoreboard.
    void printScoreboard(const std::vector<Player> &players) const;
    // Prints the information of the turn.
    void printTurnInfo(const Player &current_player, unsigned int moves_left) const;
    // Print the leaderboard. Should only be called when game is over.
    // Players are assumed to be sorted.
    void printLeaderboard(const std::vector<Player> &players) const;
    // Declares the winners of the game, given their ids and the total number of players.
    void declareWinners(const std::vector<int> &winners_id, int num_players) const;

    // Returns a callback to animate a letter being exchanged or inserted in 'Hand'.
    Hand::SwapLetterAnimator getSwapLetterCallback() const;
    // Animates the given words ('words_completed') being completed by the given 'player'.
    void animateWordComplete(const Player &player, const std::vector<Word> &words_completed) const;

    // Prints a notice to the screen, in warning colors delaying for some time.
    // The duration of the delay is shorter if 'short_delay' is true.
    static void notice(const std::string &information, bool short_delay = false);
    // Prints the necessary notices and delays after a refill has been finished.
    // This depend on whether the pool has been depleted (passed with 'depleted_pool').
    void afterRefill(bool depleted_pool) const;
};

#endif