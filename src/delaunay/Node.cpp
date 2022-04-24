

#include "Node.h"
delaunay::Node::Node(delaunay::HalfCoordinate x, delaunay::HalfCoordinate y) :
    x{static_cast<Coordinate>(x)},
    y{static_cast<Coordinate>(y)} {}

delaunay::Node::Node(delaunay::Coordinate x, delaunay::Coordinate y) :
    x{static_cast<Coordinate>(x)},
    y{static_cast<Coordinate>(y)} {}

delaunay::Node::Node(const delaunay::Node &other) {
    x = other.x;
    y = other.y;
}
delaunay::Node::Node(delaunay::Node &&other) noexcept {
    x = other.x;
    y = other.y;
}
delaunay::Node delaunay::Node::operator+(const delaunay::Node &other) const {
    return Node{x + other.x, y + other.y};
}
delaunay::Node delaunay::Node::operator-(const delaunay::Node &other) const {
    return Node{x - other.x, y - other.y};
}
bool delaunay::Node::operator==(const delaunay::Node &rhs) const {
    return x == rhs.x &&
        y == rhs.y;
}
bool delaunay::Node::operator!=(const delaunay::Node &rhs) const {
    return !(rhs == *this);
}
std::ostream &delaunay::operator<<(std::ostream &os, const delaunay::Node &node) {
    os << "Node(" << node.x << "," << node.y << ")";
    return os;
}

