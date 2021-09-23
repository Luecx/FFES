
#include "Set.h"

#include <iomanip>
std::ostream& operator<<(std::ostream& os, const Set& set) {

    os << "Set<" << set.name << ">\n";
    for(auto h:set.ids){
        os << std::right;
        os << std::setw(10) << h << "\n";
    }

    return os;
}
