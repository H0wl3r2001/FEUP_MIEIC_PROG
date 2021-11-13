#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <ostream>

// Represents the orientation of a 'Word'.
// May either be 'Horizontal' or 'Vertical'.
enum Orientation {
    Vertical,
    Horizontal,
};

// Overload of the insertion operator.
// Inserts this 'Orientation', with 'H' standing for 'Horizontal'
// and 'V' standing for vertical.
std::ostream& operator<<(std::ostream &out, Orientation orientation);

#endif