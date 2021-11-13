#include "cell.h"

using namespace std;

// As an implementation detail, an empty cell is
// represented by an empty space (' '). This is
// convenient to overload 'operator<<'.
const char Cell::EMPTY = ' ';

Cell::Cell(char letter): 
    letter(letter)
{}

char Cell::getLetter() const {
    return letter;
}

Cell& Cell::setLetter(char l) {
    letter = l;
    return *this;
}

bool Cell::isEmpty() const {
    return letter == EMPTY;
}

ostream& operator<<(ostream &out, const Cell &cell) {
    return out << cell.letter;
}
