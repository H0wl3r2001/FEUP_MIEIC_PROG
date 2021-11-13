#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

// Represents a player.
class Player {
    // The number that identifies this 'Player'.
    // For example, 'Player 1', 'Player 2', etc.
    //
    // Identifiers should start at 1.
    unsigned int id;
    // The score of the 'Player'.
    unsigned int score;
    // The 'Hand' of the 'Player'.
    Hand hand;

    public:
    // Constructs a player with the given id.
    Player(unsigned int id);

    // Returns the 'Hand' of this 'Player'.
    const Hand& getHand() const;
    // Returns the 'Hand' of this 'Player'.
    Hand& getHand();

    // Returns the id of given 'Player'.
    unsigned int getId() const;

    // Returns the current score of given 'Player'.
    unsigned int getScore() const;
    // Adds given points to the current score of this 'Player'.
    void addScore(unsigned int points);
};

#endif