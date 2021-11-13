#ifndef POOL_H
#define POOL_H

#include <vector>
#include <random>

// Represents a 'bag' of letters 'Player's can draw from.
class Pool {
    // Letters in 'Pool'. 
    std::vector<char> letters;

    public:
    // Creates a 'Pool' by moving the given vector of 'letters'.
    //
    // Letters should be in range 'A-Z'.
    Pool(std::vector<char> &&letters);

    // Removes and returns a letter from the 'Pool'.
    char popLetter();
    // Swaps a letter in the 'Pool' with the given letter.
    //
    // This exchange is not randomized. 'Pool' should be shuffled
    // afterwards.
    void exchange(char *letter);
    // Swaps two different letters in the 'Pool' with given letters. 
    //
    // This exchange is not randomized. 'Pool' should be shuffled
    // afterwards.
    void exchange(char *letter1, char *letter2);

    // Shuffles the letters inside the pool.
    void shuffle(std::default_random_engine &rng);

    // Returns whether 'Pool' is empty (has no letters left).
    bool isEmpty() const;
    // Returns how many letters the 'Pool' still has.
    int size() const;
};

#endif