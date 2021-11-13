#ifndef POSITION_H
#define POSITION_H

#include <ostream>
#include <utility>
#include "orientation.h"

// Represents a position in the board.
class Position {
    // Overload of the insertion operator.
    // Prints this position in board coordinates. (For example, 'Ab')
    friend std::ostream& operator<<(std::ostream &out, const Position &pos);

    // The x coordinate.
    int x;
    // The y coordinate.
    int y;

    public:
    // Default constructor. Constructs a 'Position' at the origin
    // (top left corner of the board).
    Position();
    // Constructs a 'Position' with given numeric coordinates.
    Position(int x, int y);
    // Constructs a 'Position' with given board coordinates.
    // (For example, 'Ab' is ('b', 'A'))
    //
    // Should be constructed with valid board coordinates ('x' must
    // be a letter in range 'a-z', and 'y' must be a letter in range
    // 'A-Z').
    Position(char x, char y); 

    // Returns the numeric x coordinate.
    int getX() const;
    // Returns the numeric y coordinate.
    int getY() const;

    // Changes 'this' by walking one step in the positive direction
    // of given 'Orientation'.
    //
    // This means that 'Horizontal' increases 'x', while 'Vertical'
    // increases 'y'.
    Position& stepForward(Orientation orientation);
    // Changes 'this' by walking one step in the negative direction
    // of given 'Orientation'.
    //
    // This means that 'Horizontal' decreases 'x', while 'Vertical'
    // decreases 'y'.
    Position& stepBackwards(Orientation orientation);
    // Returns the two positions that are adjacent and lateral to 'this',
    // assuming front and back are in the given 'Orientation'.
    //
    // For example, if 'Orientation' is 'Horizontal', left and right
    // is back and front, respectively. This means that, in this case,
    // the 'laterals' are the positions that are immediately above and 
    // below 'this'.
    std::pair<Position, Position> laterals(Orientation orientation) const;

    // Checks whether this position is within limits of a board
    // with given width and height (boards are assumed to always
    // start at the origin).
    bool inLimits(unsigned int width, unsigned int height) const;
};

std::ostream& operator<<(std::ostream &out, const Position &pos);

#endif