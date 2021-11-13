#ifndef BOARD_H
#define BOARD_H

#include <istream>
#include <vector>
#include "cell.h"
#include "position.h"
#include "orientation.h"
#include "word.h"
#include "hand.h"

// Represents a Scrabble board.
class Board {
    // The width of the 'Board'.
    unsigned int width;
    // The height of the 'Board'.
    unsigned int height;

    // The matrix of cells that form this 'Board'.
    std::vector<std::vector<Cell>> grid;

    // The total number of letters (non-empty cells) 
    // that this 'Board' contains.
    unsigned int total_letters;
    // The total number of letters that have been covered
    // in this board.
    unsigned int total_covered;
    
    // Internal method to unlock the next 'Cell' in a 'Word'
    // after covering a previous 'Cell'.
    //
    // Given the position of the new covered 'Cell' and the
    // 'Orientation' of the 'Word', tries to find the next
    // uncovered 'Cell' in that 'Word', making it coverable.
    //
    // Returns whether a 'Cell' was found and made coverable.
    bool propagate(Position position, Orientation orientation);
    // Internal method to find what would be the next coverable 'Cell' in a 'Word'
    // if current coverable 'Cell' was coverable.
    //
    // If no such cell is found, returns 'nullptr'.
    //
    // This method is mainly useful for the 'mustPlayTwiceEdgeCase'.
    const Cell* getNextUncoveredCell(Position position, Orientation orientation) const;

    public:
    // Constructs a 'Board' with given width and height.
    Board(unsigned int width, unsigned int height);
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
    
    // Get the list of letters used in this board.
    // Useful for constructing a 'Pool'.
    std::vector<char> getLettersInBoard() const;
    // Returns the total number of letters (non-empty cells) 
    // that this 'Board' contains.
    unsigned int countLetters() const;
    // Returns whether all letters in this 'Board' have been covered.
    bool isFullyCovered() const;
    // Returns the height of this 'Board'.
    unsigned int getHeight() const;
    // Returns the 'width' of this 'Board'.
    unsigned int getWidth() const;
    // Returns the 'Cell' at the given 'Position'.
    const Cell& getCell(Position position) const;

    // Adds given 'Word' to the 'Board'.
    //
    // Word is assumed to be valid and to be placed at a
    // valid position of the 'Board'.
    void addWord(Word &word);
    // Finds the word that has a letter at given 'Position'
    // and has the given 'Orientation'.
    //
    // Must only be called if such 'Position' is known to belong
    // to a 'Word' with that 'Orientation'.
    Word findWord(Position position, Orientation orientation) const;
    
    // Covers 'Cell' at given position. Should only be called after
    // checking if it 'isCoverable'.
    //
    // By covering the 'Cell' at given positon, this function is also 
    // unlocking the next 'Cell' in the same 'Word's, if applicable. 
    // If any 'Word' has been completed, it is pushed to 'completed_words'. 
    void cover(Position position, std::vector<Word> &completed_words);
    // Returns whether the given 'Hand' can make a move.
    bool hasMove(const Hand &hand) const;    

    // Checks for the specific edge case where otherwise legal moves
    // can't be made by the 'Player' because it would unallow them
    // to make a second move in the same turn.
    //
    // Returns true if the edge case is applicable. In that case,
    // pushes the list of legal positions to 'legal_positions'.
    bool mustPlayTwiceEdgeCase(const Hand &hand, std::vector<Position> &legal_positions);
};

#endif