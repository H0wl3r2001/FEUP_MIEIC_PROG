#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "cell.h"
#include "position.h"
#include "word.h"

// Represents a Scrabble board.
class Board {
    // The width of the 'Board'.
    unsigned int width;
    // The height of the 'Board'.
    unsigned int height;

    // The matrix of cells that form this 'Board'.
    std::vector<std::vector<Cell>> grid;
    // A list of all words inserted in this 'Board'.
    std::vector<Word> words;

    // The total number of letters (non-empty cells) 
    // that this 'Board' contains.
    unsigned int total_letters;
    
    public:
    // Constructs a 'Board' with given width and height.
    Board(unsigned int width, unsigned int height);

    // Returns the total number of letters (non-empty cells) 
    // that this 'Board' contains.
    unsigned int countLetters() const;
    // Returns the total number of words inserted to this
    // 'Board'.
    unsigned int countWords() const;
    // Returns the height of this 'Board'.
    unsigned int getHeight() const;
    // Returns the 'width' of this 'Board'.
    unsigned int getWidth() const;
    // Returns the 'Cell' at the given 'Position'.
    const Cell& getCell(Position position) const;

    // Returns whether the given 'Word' may currently be 
    // placed in the 'Board'.
    //
    // If not, an user-friendly error message explaining 
    // why it can't be placed is inserted in the 'error_messages'
    // stream. 
    bool isWordValid(const Word &word, std::ostream &error_messages) const;
    // Adds given 'Word' to the 'Board'.
    //
    // Should only be done after making sure this 'Word' may be
    // added to 'Board' (see 'isWordValid').
    void addWord(const Word &word);

    // Loads 'Words' from a stream until either the stream ends or
    // a line can't be parsed.
    //
    // The stream is assumed to start immediately with the list of 
    // words (doesn't have a line for board dimensions, for example)
    // and this list forms a valid 'Board' (so 'Word's are valid and
    // don't appear in invalid positions). Each line must contain the 
    // information of a 'Word', following the specification of a board file.
    //
    // The stream may have a 2D representation of the board at the end
    // because 'Word's stop being loaded as soon as a line can't be
    // parsed as a 'Word'.
    void loadWords(std::istream &save);
    // Writes the data from this 'Board' to stram 'out', obeying
    // the specification of a board file.
    void writeData(std::ostream &out) const;
};

#endif