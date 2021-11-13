#ifndef BOARD_BUILDER_DISPLAYER_H
#define BOARD_BUILDER_DISPLAYER_H

#include <string>
#include <sstream>
#include <ostream>
#include "board.h"
#include "word.h"
#include "cmd.h"

// Helps to display and animate the state of the program to the user.
class BoardBuilderDisplayer {
    // Number of milliseconds between adding each
    // letter when adding a new 'Word' to the board.
    static const int ADD_LETTER_TO_BOARD_DELAY;

    // The stream of error messages printed immediately
    // before prompting user for input.
    std::ostringstream error_messages;
    // Horizontal offset before starting the zone of the
    // screen that shows board properties.
    unsigned int board_info_x_offset;
    // Vertical offset before starting the zone of the
    // screen where the player is prompted for input.
    unsigned int prompt_y_offset;

    public:
    // Normal text color.
    static const Color TEXT_COLOR;
    // Darkened text color.
    static const Color TEXT_COLOR_DARK;
    // Color for erros.
    static const Color ERROR_COLOR;
    // Color for warnings.
    static const Color WARNING_COLOR;
    // Color for success.
    static const Color SUCCESS_COLOR;
    // Color of a letter in the board.
    static const Color LETTER_COLOR;
    // Color of the background of the board.
    static const Color BOARD_BACKGROUND;

    // Constructs a displayer for a 'Board' with given width and height.
    BoardBuilderDisplayer(unsigned int board_width, unsigned int board_height);

    // Returns the stream of error messages printed immediately
    // before prompting user for input.
    std::ostream& getErrorStream();
    // Clears all errors in the stream of error messages.
    void clearErrors();

    // Prints the given 'Board' to screen.
    void printBoard(const Board &board) const;
    // Prints the properties of given 'Board' to screen, along with
    // its name and the maximum number of players.
    void printBoardInfo(const std::string &board_name, const Board &board, unsigned int max_players) const;
    // Prints the prompt for user to enter another word.
    void printPrompt(unsigned int max_players, unsigned int total_letters) const;
    // Prints a new word, letter by letter (with a short delay inbetween)
    // that is being added to the board. 
    void printNewWord(const Word &word, const Board &board) const;
};

#endif