#include <iterator>
#include <algorithm>
#include "hand.h"

// As an implementation detail, an empty slot is
// represented by an underscore ('_'). This is
// convenient to overload 'operator<<'.
const char Hand::EMPTY = '_';

Hand::Hand() {
    std::fill(std::begin(hand), std::end(hand), EMPTY);
}

int Hand::indexOf(char *element) const {
    return (int) (element - std::begin(hand));
}

bool Hand::isFull() const {
    // 'Hand' is full if it is not possible to find an empty slot.
    return std::find(std::begin(hand), std::end(hand), EMPTY) == std::end(hand);
}

void Hand::refill(Pool &pool, SwapLetterAnimator swap_hand) {
    for(auto &letter: hand) {
        // Refill must stop as soon as the 'Pool' is empty
        if(pool.isEmpty()) break;
        
        if(letter == EMPTY) {
            letter = pool.popLetter();
            if(swap_hand) swap_hand(indexOf(&letter), letter);
        }
    }
}

void Hand::exchange(Pool &pool, char letter, SwapLetterAnimator swap_hand) {
    char *element = std::find(std::begin(hand), std::end(hand), letter);
    pool.exchange(element);
    if(swap_hand) swap_hand(indexOf(element), *element);
}

void Hand::exchange(Pool &pool, char letter1, char letter2, SwapLetterAnimator swap_hand) {
    char *element1, *element2;
    element1 = std::find(std::begin(hand), std::end(hand), letter1);

    if(letter1 == letter2) {
        // When given letters are the same, we must be careful
        // not to choose the same letter twice. To avoid that,
        // search only after the first element.
        element2 = std::find(element1+1, std::end(hand), letter2);
    } else {
        // When given letters are different, finding the second
        // element is very similar to finding the first one.
        element2 = std::find(std::begin(hand), std::end(hand), letter2);
    }

    pool.exchange(element1, element2);
    if(swap_hand) {
        swap_hand((int) (element1 - std::begin(hand)), *element1);
        swap_hand((int) (element2 - std::begin(hand)), *element2);
    }
}

bool Hand::hasLetter(char letter) const {
    return std::find(std::begin(hand), std::end(hand), letter) != std::end(hand);
}

int Hand::countLetter(char letter) const {
    return (int) std::count(std::begin(hand), std::end(hand), letter);
}

void Hand::useLetter(char letter) {
    char *l = std::find(std::begin(hand), std::end(hand), letter);
    if(l != std::end(hand)) *l = EMPTY;
}

std::ostream& operator<<(std::ostream& out, const Hand& hand) {
    for(char letter: hand.hand) {
        out << letter << " ";
    }
    
    return out;
}
