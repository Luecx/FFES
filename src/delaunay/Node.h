

//
// Created by Luecx on 23.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_NODE_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_NODE_H_

#include <ostream>
#include "defs.h"
namespace delaunay{

struct Node {
    Coordinate x;
    Coordinate y;

public:
    Node(HalfCoordinate x, HalfCoordinate y);
    Node(const Node& other);
    Node(Node&& other) noexcept;
private:
    Node(Coordinate x, Coordinate y);
public:
    // simple operator overloading for adding two nodes together
    Node operator+(const Node& other) const;
    // simple operator overloading for subtracting two nodes
    Node operator-(const Node& other) const;
    // assignment operator
    Node& operator=(const Node& other)= default;
    // assignment operator
    Node& operator=(Node&& other)= default;

    // equality check
    bool operator==(const Node &rhs) const;
    // inequality check
    bool operator!=(const Node &rhs) const;

    // stream output; displays "Node(${x},${y})"
    friend std::ostream &operator<<(std::ostream &os, const Node &node);

};

}

#endif //EXACTCONSTRAINEDDELAUNAY_SRC_NODE_H_
