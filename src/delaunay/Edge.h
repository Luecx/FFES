

//
// Created by Luecx on 23.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_EDGE_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_EDGE_H_

#include <ostream>
#include "defs.h"
#include "Node.h"

namespace delaunay {
struct Edge {
    // two nodes spanning the edge
    Node* n1 = nullptr;
    Node* n2 = nullptr;
    // the connected edge to this edge. nullptr if this edge is a boundary edge
    Edge* other = nullptr;
    // the triangle which this edge belongs to
    Triangle* triangle = nullptr;
    // the index of this edge within the given triangle
    Index edgeIndex = 0;
    // the flip age of this edge. this is relevant when this edge might be flipped to ensure
    // no edge is flipped twice. Since Age is unsigned, setting flipAge to -1, will result in no flips ever occuring
    Age flipAge = 0;

    // checks for the intersection between two edges.
    // returns true if the edges intersect
    // intersections are not counted if only the nodes touch
    [[nodiscard]] bool intersects(Edge* other) const;

    // computes the length of the edge
    // the length is computed according to the euclidean norm
    [[nodiscard]] Precision length() const;

    // checks if the given node is left of the edge. It returns true if this is the case
    // In case of the node sitting exactly on the edge, it will also return true
    [[nodiscard]] bool isLeft(Node* node) const;

    // checks if the given node is left of the edge. It returns true if this is the case
    // In case of the node sitting exactly on the edge, it will return false
    [[nodiscard]] bool isStrictlyLeft(Node* node) const;

    // checks if the given node is on the edge. It returns true if this is the case
    // the solution is exact
    [[nodiscard]] bool isOnEdge(Node* node) const;

    // checks if the edge satisfies the delaunay constraint
    // if the edge is not linked, this returns true
    // otherwise it checks if the opposite node of the associated triangle is within the linked triangles circumcircle
    [[nodiscard]] bool isDelaunay() const;

    // checks if the quadrilateral defined by the two associated triangles forms a strictly convex shape
    // this implies that no three points are collinear.
    [[nodiscard]] bool isStrictlyConvex() const;

    // gets the next edge which comes after this edge for the associated triangle
    [[nodiscard]] Edge& next() const;

    // gets the previous edge which comes before this edge for the associated triangle
    [[nodiscard]] Edge& prev() const;

    // gets the node which is opposite to this edge in the given triangle
    [[nodiscard]] Node* opposite() const;

    // check if its a restricted edge
    [[nodiscard]] bool isRestricted() const;

    // restricts flipping for the edge. This leads to no flips ever occurring again for this edge or its linked edge
    void restrict();

    // links the edge to another edge. Both edges will also share the flipAge
    // when linking the first time, both will receive get their flipAge updated to the higher flipAge value.
    void link(Edge* other=nullptr);

    // sets the flipAge of the edge. If the edge is linked to a second edge, the second edge will also
    // have its flipAge updated
    void setAge(Age age);


    // flips the edge if it does not fulfill the delaunay constraint and creates two new triangles
    // increases the history score for both associated triangles
    // does nothing if the linked edge does not exist
    // if recursion is enabled, it will invoke the flip algorithm for all new edges which need to be checked
    // for flipping
    void flip(CDT& cdt, bool recursive=false, Age age=0);

    // flips the edge and reuses the two triangles adjacent.
    // It does not check if the flip should be performed in the first place.
    // it has no impact on the history scores and assumes that no new nodes will be added which may require
    // triangle finding using the histories since this function invalidates the histories.
    void flip();

    // stream output operator
    friend std::ostream &operator<<(std::ostream &os, const Edge &edge);

    bool operator==(const Edge &rhs) const;
    bool operator!=(const Edge &rhs) const;
};
}

#endif //EXACTCONSTRAINEDDELAUNAY_SRC_EDGE_H_
