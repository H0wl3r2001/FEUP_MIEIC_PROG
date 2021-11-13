#include "cell.h"

using namespace std;

// As an implementation detail, an empty cell is
// represented by an empty space (' '). This is
// convenient to overload 'operator<<'.
const char Cell::EMPTY = ' ';

Cell::Cell(char letter): 
    letter(letter), 
    covered(false),
    coverable(false),
    propagates_horizontally(false),
    propagates_vertically(false)
{}

void Cell::allowMove(Orientation orientation) {
    coverable = true;
    if(orientation == Horizontal) {
        propagates_horizontally = true;
    } else {
        propagates_vertically = true;
    }
}

void Cell::cover() {
    covered = true;
    coverable = false;
}

char Cell::getLetter() const {
    return letter;
}

Cell& Cell::setLetter(char l) {
    letter = l;
    return *this;
}

bool Cell::isCovered() const {
    return covered;
}

bool Cell::isCoverable() const {
    return coverable;
}

bool Cell::isEmpty() const {
    return letter == EMPTY;
}

bool Cell::propagatesHorizontally() const {
    return propagates_horizontally;
}

bool Cell::propagatesVertically() const {
    return propagates_vertically;
}

ostream& operator<<(ostream &out, const Cell &cell) {
    return out << cell.letter;
}
