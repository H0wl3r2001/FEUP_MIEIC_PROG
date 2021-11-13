#include "orientation.h"

using namespace std;

ostream& operator<<(ostream &out, Orientation orientation) {
    if(orientation == Horizontal) out << 'H';
    else out << 'V';
    return out;
}
