#ifndef BOARD_BUILDER_H
#define BOARD_BUILDER_H

#include <string>
#include <istream>
#include "board.h"
#include "boardBuilderDisplayer.h"
#include "position.h"
#include "orientation.h"

// Manages the creation of the board.
class BoardBuilder {
    // The name of the board being created.
    std::string board_name;
    // The 'Board' being created.
    Board &board;
    // Helper class to display and animate the state of the program to the user.
    BoardBuilderDisplayer displayer;
    // The current maximum number of players.
    unsigned int max_players;

    // Try to parse a 'Position' from 'input' to 'position'.
    // Returns whether the parsing was successful.
    bool parsePosition(std::istream &input, Position &position);
    // Try to parse an 'Orientation' from 'input' to 'orientation'.
    // Returns whether the parsing was successful.
    bool parseOrientation(std::istream &input, Orientation &orientation);
    // Try to parse a valid string (for a word) from 'input' to 'word_str'.
    // Returns whether the parsing was successful.
    bool parseWordStr(std::istream &input, std::string &word_str);
    // Returns whether 'word' is found in dictionary stream 'dict'.
    static bool inDict(std::istream &dict, const std::string &word);

    public:
    // Name of the words dictionary.
    static const char* DICTIONARY;

    // Constructs a builder for the given 'board' with name 'board_name'.
    BoardBuilder(std::string &board_name, Board &board);

    // Runs this builder.
    void run();
    // Saves the content of 'board' to file.
    void saveToFile() const;
};

#endif