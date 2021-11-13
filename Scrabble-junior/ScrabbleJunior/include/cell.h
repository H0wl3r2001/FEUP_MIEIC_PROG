#ifndef CELL_H
#define CELL_H

#include <ostream>
#include "orientation.h"

// Represents a single space for a letter in the board.
class Cell {
    // Overload of the insertion operator.
    // Inserts the letter of this 'Cell', or an empty space (' ')
    // if 'Cell' is empty.
    friend std::ostream& operator<<(std::ostream &out, const Cell &cell);

    // 'char' that represents a 'Cell' with no letter.
    static const char EMPTY;

    // The letter of the 'Cell'. 'EMPTY' if the cell doesn't 
    // have a letter.
    char letter;
    // Whether 'Cell' has already been covered by a 'Player'.
    bool covered;
    // Whether 'Cell' can be covered by a 'Player' (so it's not
    // empty and it's the next uncovered letter in a word).
    bool coverable;
    // Whether covering this 'Cell' may unlock other 'Cell's 
    // in the 'Horizontal' orientation.
    bool propagates_horizontally;
    // Whether covering this 'Cell' may unlock other 'Cell's 
    // in the 'Vertical' orientation.
    bool propagates_vertically;
    
    public:
    // Constructs a 'Cell' with given letter.
    //
    // Argument passed must be a letter in range 'A-Z'. If
    // none is passed, an empty 'Cell' is constructed.
    Cell(char letter = EMPTY);
    // Allows this 'Cell' to be covered, as a part of a word
    // in given 'Orientation'.
    //
    // 'Cell' will remember the 'Orientation' so when it is
    // covered, it may unlock another 'Cell' of the same word
    // in that orientation.
    void allowMove(Orientation word_orientation);
    // Marks this 'Cell' as covered. Should only be called after
    // checking if it 'isCoverable'.
    void cover();
    
    // Returns the letter of this cell. Should only be called
    // after making sure that this cell is not empty (see 'isEmpty').
    char getLetter() const;
    // Sets the letter of this cell to the given letter.
    Cell& setLetter(char l);

    // Returns whether 'Cell' has already been covered by a 'Player'.
    bool isCovered() const;
    // Returns whether 'Cell' can be covered by a 'Player' (so it's not
    // empty and it's the next uncovered letter in a word).
    bool isCoverable() const;
    // Returns whether this cell has a letter.
    bool isEmpty() const;
    // Returns whether covering this 'Cell' may unlock other 
    // 'Cell's in the 'Horizontal' orientation
    bool propagatesHorizontally() const;
    // Returns whether covering this 'Cell' may unlock other 
    // 'Cell's in the 'Vertical' orientation.
    bool propagatesVertically() const;
};

std::ostream& operator<<(std::ostream &out, const Cell &cell);

#endif