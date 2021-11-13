#include "word.h"

using namespace std;

Word::Word(Position start, Orientation orientation, string &word): 
    start(start),
    orientation(orientation),
    word(move(word)) 
{}

Position Word::getStart() const {
    return start;
}

Orientation Word::getOrientation() const {
    return orientation;
}

Word::const_iterator Word::begin() const {
    return word.begin();
}

Word::const_iterator Word::end() const {
    return word.end();
}

ostream& operator<<(ostream &out, const Word &word) {
    out << word.start << ' ' << word.orientation << ' ' << word.word;
    return out;
}
