#ifndef WORD_H
#define WORD_H

#include <string>
#include "position.h"
#include "orientation.h"

// Represents a word placed in the board.
class Word {
    // The 'Position' where the word starts.
    const Position start;
    // The 'Orientation' of the word.
    const Orientation orientation;
    // 'string' that contains the word itself.
    const std::string word;

    public:
    // Alias for iterator type.
    typedef std::string::const_iterator const_iterator;
    
    // Constructs a word with given 'Position', 'Orientation' and 'string'.
    // The string passed is moved.
    Word(Position start, Orientation orientation, std::string &word);

    // Returns the 'Position' where the word starts.
    Position getStart() const;
    // Returns the 'Orientation' of the word.
    Orientation getOrientation() const;

    // Returns iterator to begining of the string word.
    const_iterator begin() const;
    // Returns iterator to end of the string word.
    const_iterator end() const;
};

#endif