

//
// Created by Luecx on 23.04.2022.
//

#include <algorithm>
#include <cmath>
#include "Edge.h"
#include "Triangle.h"

bool delaunay::Edge::intersects(delaunay::Edge *other) const {
    Coordinate r1  = n1->x;
    Coordinate r2  = n1->y;
    Coordinate s1  = n2->x - r1;
    Coordinate s2  = n2->y - r2;

    Coordinate u1  = other->n1->x;
    Coordinate u2  = other->n1->y;
    Coordinate v1  = other->n2->x - u1;
    Coordinate v2  = other->n2->y - u2;

    Coordinate b1  = r1 - u1;
    Coordinate b2  = r2 - u2;

    Coordinate det = -s1 * v2 + s2 * v1;

    // they do not intersect if they are parallel
    if (std::abs(det) == 0)
        return false;

    // compute inverse. Skip division by determinant and include that later in the range checks
    Coordinate A_inv_11 =  v2;
    Coordinate A_inv_12 = -v1;
    Coordinate A_inv_21 =  s2;
    Coordinate A_inv_22 = -s1;

    // compute x1 and x2 which is scaled by the determinant.
    Coordinate x1_nt = A_inv_11 * b1 + A_inv_12 * b2;
    Coordinate x2_nt = A_inv_21 * b1 + A_inv_22 * b2;

    if(det > 0){
        return x1_nt > 0   &&
               x1_nt < det &&
               x2_nt > 0   &&
               x2_nt < det;
    }else{
        return x1_nt < 0   &&
               x1_nt > det &&
               x2_nt < 0   &&
               x2_nt > det;
    }

}
delaunay::Precision delaunay::Edge::length() const {
    Node temp = *n1 - *n2;
    return std::sqrt(temp.x * temp.x + temp.y * temp.y);
}
bool delaunay::Edge::isLeft(delaunay::Node *node) const {
    auto h = (  (n2->x - n1->x)*(node->y - n1->y)
              - (n2->y - n1->y)*(node->x - n1->x));
    return h >= 0;
}
bool delaunay::Edge::isStrictlyLeft(delaunay::Node *node) const {
    auto h = (  (n2->x - n1->x)*(node->y - n1->y)
              - (n2->y - n1->y)*(node->x - n1->x));
    return h > 0;
}
bool delaunay::Edge::isOnEdge(delaunay::Node *node) const {
    auto h = (  (n2->x - n1->x)*(node->y - n1->y)
              - (n2->y - n1->y)*(node->x - n1->x));
    return h == 0 
        && std::max(n1->x, n2->x) >= node->x 
        && std::min(n1->x, n2->x) <= node->x
        && std::max(n1->y, n2->y) >= node->y
        && std::min(n1->y, n2->y) <= node->y;
}
bool delaunay::Edge::isDelaunay() const {
    if(other){
        Node* node = triangle->opposite(*this);
//        if(node->x <= MIN_SOFT_COORDINATE || node->x >= MAX_SOFT_COORDINATE) return true;
//        if(node->y <= MIN_SOFT_COORDINATE || node->y >= MAX_SOFT_COORDINATE) return true;
//        if(n1->x <= MIN_SOFT_COORDINATE || n1->x >= MAX_SOFT_COORDINATE) return true;
//        if(n1->y <= MIN_SOFT_COORDINATE || n1->y >= MAX_SOFT_COORDINATE) return true;
//        if(n2->x <= MIN_SOFT_COORDINATE || n2->x >= MAX_SOFT_COORDINATE) return true;
//        if(n2->y <= MIN_SOFT_COORDINATE || n2->y >= MAX_SOFT_COORDINATE) return true;

        return !other->triangle->isCircumscribed(triangle->opposite(*this));
    }
    return true;
}

bool delaunay::Edge::isStrictlyConvex() const {
    if(other){
        Edge& e1 = this->next();
        Edge& e2 = this->prev();
        Edge& e3 = other->next();
        Edge& e4 = other->prev();

        if(!e1.isStrictlyLeft(e2.n2))
            return false;
        if(!e2.isStrictlyLeft(e3.n2))
            return false;
        if(!e3.isStrictlyLeft(e4.n2))
            return false;
        if(!e4.isStrictlyLeft(e1.n2))
            return false;
    }
    return true;
}

delaunay::Edge& delaunay::Edge::next() const {
    return triangle->getEdge(nextIndex(edgeIndex));
}
delaunay::Edge& delaunay::Edge::prev() const {
    return triangle->getEdge(prevIndex(edgeIndex));
}

delaunay::Node *delaunay::Edge::opposite() const {
    return triangle->opposite(*this);
}

bool delaunay::Edge::isRestricted() const {
    return this->flipAge == static_cast<Age>(-1);
}


void delaunay::Edge::restrict() {
    setAge(-1);
}


void delaunay::Edge::link(delaunay::Edge *other) {
    this->other = other;
    if(other) {
        other->other = this;
        this->flipAge = other->flipAge = std::max(this->flipAge, other->flipAge);
    }
}

void delaunay::Edge::setAge(Age age) {
    this->flipAge = age;
    if(other)
        other->flipAge = age;
}



void delaunay::Edge::flip(delaunay::CDT &cdt, bool recursive, Age age) {

    if(other == nullptr) return;
    if(triangle == nullptr) return;
    if(other->triangle == nullptr) return;

    // don't do anything if it should not be flipped or was already flipped
    if(flipAge >= age) return;

    // don't do anything if it fulfills the delaunay constraint
    if(isDelaunay()) {
        // mark this edge as checked and return
        setAge(age);
        return;
    };

    // get the opposite nodes for each edge
    Node* our_opp = this->opposite();
    Node* oth_opp = other->opposite();

    // create two new triangles
    Triangle* t1 = cdt.newTriangle();
    Triangle* t2 = cdt.newTriangle();
    t1->assignNodes(our_opp, oth_opp, n2);
    t2->assignNodes(oth_opp, our_opp, n1);

    // get original neighbours
    // link neighbours
    Edge* other_edge_next = &other->next();
    Edge* other_edge_prev = &other->prev();
    Edge*       edge_next = &next();
    Edge*       edge_prev = &prev();

    // link
    t1->edges[0].link(&t2->edges[0]);
    t1->edges[1].link(other_edge_prev->other);
    t1->edges[2].link(      edge_next->other);
    t2->edges[1].link(      edge_prev->other);
    t2->edges[2].link(other_edge_next->other);

    // set histories
    triangle->historyDegree = 2;
    triangle->history[0] = t1;
    triangle->history[1] = t2;
    other->triangle->historyDegree = 2;
    other->triangle->history[0] = t1;
    other->triangle->history[1] = t2;

    // set the age to this edge and the linked edge
    setAge(age);

    // recursion
    if(recursive){
        t1->getEdge(1). flip(cdt, true, age);
        t2->getEdge(2). flip(cdt, true, age);
    }
}

void delaunay::Edge::flip() {
    if (other == nullptr)
        return;
    if (triangle == nullptr)
        return;
    if (other->triangle == nullptr)
        return;

    Node* other_node = other->opposite();
    Node* our_node   = opposite();

    Edge* other_edge_next = other->next().other;
    Edge* other_edge_prev = other->prev().other;
    Edge* edge_next       = next().other;
    Edge* edge_prev       = prev().other;

    Node* n1_temp = this->n1;
    Node* n2_temp = this->n2;

    Triangle* t1 = this->triangle;
    Triangle* t2 = this->other->triangle;
    // create two new triangles
    t1->assignNodes(our_node, other_node, n2_temp);
    t2->assignNodes(other_node, our_node, n1_temp);
    // link neighbours
    t1->edges[0].link(&t2->edges[0]);
    t1->edges[1].link(other_edge_prev);
    t1->edges[2].link(edge_next);
    t2->edges[1].link(edge_prev);
    t2->edges[2].link(other_edge_next);
}


std::ostream &delaunay::operator<<(std::ostream &os, const delaunay::Edge &edge) {
    os << "Edge: " << &edge
       << " n1: " << *edge.n1
       << " n2: " << *edge.n2
       << " other: " << edge.other
       << " triangle: " << edge.triangle
       << " edgeIndex: " << edge.edgeIndex
       << " flipAge: " << edge.flipAge;
    return os;
}
bool delaunay::Edge::operator==(const delaunay::Edge &rhs) const {
    return *n1 == *rhs.n1 &&
           *n2 == *rhs.n2 ||
           *n2 == *rhs.n1 &&
           *n1 == *rhs.n2 ;
}
bool delaunay::Edge::operator!=(const delaunay::Edge &rhs) const {
    return !(rhs == *this);
}






