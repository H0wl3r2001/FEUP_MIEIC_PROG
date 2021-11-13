#ifndef HAND_H
#define HAND_H

#include <functional>
#include <ostream>
#include "pool.h"

// Represents the letters a 'Player' holds in the game.
class Hand {
    // Overload of the insertion operator.
    // Inserts this 'Hand', with a space after every letter.
    // When a slot is empty, it is represented by an underscore ('_').
    friend std::ostream& operator<<(std::ostream& out, const Hand& hand); 
    
    // Players may have up to 7 letters in their 'Hand'.
    static const int HAND_SIZE = 7;
    // 'char' that represents a slot without a letter.
    static const char EMPTY;

    // The letters in 'Hand'.
    char hand[HAND_SIZE];

    // Auxiliary function to calculate the index of an element
    // based on its position in memory.
    int indexOf(char* element) const;

    public:
    // A callback to animate a letter being exchanged or inserted in 'Hand'.
    //
    // The first argument is the index of the letter being changed.
    // The second argument is the new letter.
    typedef std::function<void (int, char)> SwapLetterAnimator;

    // Constructs a new 'Hand' with every slot empty.
    Hand();

    // Returns whether this 'Hand' is full (has no empty slot).
    bool isFull() const;
    // Refills every empty slot in 'Hand' with a letter from 'Pool',
    // until 'Hand' is full or 'Pool' is empty.
    //
    // If a 'SwapLetterAnimator' is given, it is called every time
    // a new letter is put in 'Hand'.
    void refill(Pool &pool, SwapLetterAnimator swap_hand = nullptr);
    // Finds the first letter in 'Hand' that matches the given letter
    // and exchanges it with the 'Pool'.
    //
    // Should only be called if 'Hand' has given letter and Pool is not empty.
    //
    // If a 'SwapLetterAnimator' is given, it is called every time
    // a new letter is put in 'Hand'.
    void exchange(Pool &pool, char letter, SwapLetterAnimator swap_hand = nullptr);
    // Finds the first letters in 'Hand' that match the given letters
    // and exchanges them with the 'Pool'.
    //
    // Should only be called if 'Hand' has both letters and Pool has at least two letters.
    //
    // If a 'SwapLetterAnimator' is given, it is called every time
    // a new letter is put in 'Hand'.
    void exchange(Pool &pool, char letter1, char letter2, SwapLetterAnimator swap_hand = nullptr);

    // Returns whether player has given letter.
    bool hasLetter(char letter) const;
    // Returns the amount of letters this 'Hand' has that are equal to given letter.
    int countLetter(char letter) const;
    // Uses the given letter (i.e. removes one instance of it from 'Hand').
    //
    // Should only be called if 'Hand' has given letter.
    void useLetter(char letter);
};

std::ostream& operator<<(std::ostream& out, const Hand& hand); 

#endif