

//
// Created by Luecx on 24.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_CDT_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_CDT_H_

#include <vector>
#include <iostream>
#include "defs.h"

namespace delaunay{

struct CDT {
    std::vector<Triangle> triangles{};
    std::vector<Node>     nodes{};

    // constructor
    // reserves the triangles and nodes vectors above
    CDT(int max_nodes);

    // creates a new triangle
    // fails and exits if no more triangles can be created
    Triangle* newTriangle();

    // returns the base triangle spanning the entire range of values
    Triangle* getBaseTriangle();

    // finds the triangles using the history entries stored in each triangle
    // it starts at the base triangle and searches for triangles as long as they are invalid.
    // if a valid triangle is found (historyDegree == 0), the triangle will be propagated back
    Triangle* findTriangle(Triangle* base, Node* node);

    // adds the node to the triangulation
    // first it finds the valid triangle which contains the given node
    // depending on if the node lies on the edge or is within the triangle, it splits the triangle into 3 or 4
    // smaller triangles. The edges are flipped to recover the delaunay constraints
    void addNode(Node node);

    // adds a segment to the triangulation
    // in the first step it gathers all edges which intersect the given constraint.
    // in the second step, it rearranges and flips the edges so that no edge crosses the constraint
    // lastly, the delaunay triangulation will be restored
    void addSegment(Edge edge);

    // removes all the triangles sharing an edge with the base triangle
    // this results in the convex hull of the triangles remaining as valid triangles
    void finish();

    friend std::ostream &operator<<(std::ostream &os, const CDT &cdt);
};

}
#endif //EXACTCONSTRAINEDDELAUNAY_SRC_CDT_H_
