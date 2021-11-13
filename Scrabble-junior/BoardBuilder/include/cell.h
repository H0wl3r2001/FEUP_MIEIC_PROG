#ifndef CELL_H
#define CELL_H

#include <ostream>

// Represents a single space for a letter in the board.
class Cell {
    // Overload of the insertion operator.
    // Inserts the letter of this 'Cell', or an empty space (' ') if
    // 'Cell' is empty.
    friend std::ostream& operator<<(std::ostream &out, const Cell &cell);

    // Char that represents a 'Cell' with no letter.
    static const char EMPTY;

    // The letter of the 'Cell'. 'EMPTY' if the
    // cell doesn't have a letter.
    char letter;
    
    public:
    // Constructs a 'Cell' with given letter.
    //
    // Argument passed must be a letter in range 'A-Z'. If
    // none is passed, an empty 'Cell' is constructed.
    Cell(char letter = EMPTY);

    // Returns the letter of this cell. Should only be called
    // after making sure that this cell is not empty (see 'isEmpty').
    char getLetter() const;
    // Sets the letter of this cell to the given letter
    Cell& setLetter(char l);

    // Returns whether this cell is empty (has no letter).
    bool isEmpty() const;
};

std::ostream& operator<<(std::ostream &out, const Cell &cell);

#endif