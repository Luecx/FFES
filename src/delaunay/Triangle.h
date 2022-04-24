

//
// Created by Luecx on 23.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_TRIANGLE_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_TRIANGLE_H_

#include <ostream>
#include "defs.h"
#include "Edge.h"
#include "CDT.h"

namespace delaunay{
struct Triangle {
    // Edges spanning the triangle
    Edge edges[3]{};

    // history data for finding a triangle during construction
    // the most amount of triangles which could be created within this triangle is 3.
    // 3 triangles are spawned when the triangle is split with a new node in the middle
    // if the new node lies on the edge, only two new triangles are created within this triangle.
    Triangle* history[3]{};
    // the history degree equals the amount of triangles set in the history array above.
    // there is also the option to invalidate a triangle without it having any history entries.
    // this is used when e.g. the triangles lies outside the boundary and needs to be excluded from the final mesh
    // the options for the history degree are the following:
    // -1 : invalidated
    //  0 : valid
    // 1-3: not part of the active set (history entries exist)
    int historyDegree = 0;

    // compute the area of the triangle. small rounding offsets may occur but no exact arithmetic is required
    [[nodiscard]] Precision area() const;

    // compute the aspect ratio of the triangle. if all sides have the same length, the aspect ratio
    // equals 1. Any other combination of side lengths results in an aspect ratio > 1.
    [[nodiscard]] Precision aspectRatio() const;

    // returns the edge at the given index. edges are sorted counterclockwise
    [[nodiscard]] Edge& getEdge(Index idx);

    // returns a pointer to the node at the given index
    // the node equals the first node of the edge with the given index
    [[nodiscard]] Node* getNode(Index idx) const;

    // returns a new node which is the average of all the other nodes
    // due to integer rounding this may not be exact and may even result in a node OUTSIDE the triangle
    // this should be checked afterwards and is a potential problem when the problem is very small.
    [[nodiscard]] Node center() const;

    // returns the node which is opposite to the given triangle. It assumes that the edge is one of the edges
    // above
    [[nodiscard]] Node* opposite(const Edge& e);

    // returns the index of the given node. if the node is not part of this triangle, it returns -1
    [[nodiscard]] Index getNodeIndex(Node* node);

    // check if the node is inside the triangle. It returns true if the node is within the triangle.
    // it also returns true if the node lies on the edge
    [[nodiscard]] bool contains(Node* nP) const;

    // check if the given node is within the circumscribed circle
    // the circumscribed circle is defined by the 3 nodes which also define this triangle.
    // if the node lies exactly on the circumscribed triangle, it will return FALSE.
    // this operation is not exact since it uses floating points operations
    [[nodiscard]] bool isCircumscribed(Node* n) const;

    // check if its part of the active set
    // this is the case if the history degree equals 0. a negative degree would be an invalidated triangle.
    // a positive degree would indicate children which have replaced this triangle
    [[nodiscard]] bool isValid() const;

    // invalidate the element. useful to disable it when it does not satisfy some constraint and
    // should therefor be removed from the set of active triangles
    void invalidate();

    // assigns nodes to this triangle.
    // this includes the generation of the edges which will in return get the node pointers
    // furthermore, the edges will receive data like their edge index and the triangle they are connected to.
    void assignNodes(Node* n1, Node* n2, Node *n3);
    
    // split the triangle into three triangles
    // the three new triangles are given as well as the new node to be inserted
    void splitIntoThreeTriangles(CDT& cdt, Node* np);

    // split the given edge into four new triangles if the edge is linked to a second edge.
    void splitIntoFourTriangles(Edge& e, CDT& cdt, Node* np);

    // checks that no error has occured
    void checkIntegrity();


    friend std::ostream &operator<<(std::ostream &os, const Triangle &triangle);
};
}


#endif //EXACTCONSTRAINEDDELAUNAY_SRC_TRIANGLE_H_
