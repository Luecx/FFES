
//
// Created by Luecx on 24.04.2022.
//

#include <queue>
#include "CDT.h"
#include "Node.h"
#include "Triangle.h"
#include "Edge.h"

delaunay::CDT::CDT(int max_nodes) {
    // reserve data for the triangles
    // technically the upper bound is incredibly large since every edge flip will create two new triangles
    // every inserted node will also create 3 new triangles. We reserve space for 13 triangles per inserted node
    // which corresponds to 3 triangles for inserting the node itself and 10 further triangles for flipping
    // If on average, there are more than 5 flips, the triangulation will interrupt
    triangles.reserve(max_nodes * 13 + 1);
    nodes    .reserve(max_nodes      + 3);

    // add the first 3 nodes which span the parent triangle
    nodes.emplace_back(MIN_HARD_COORDINATE, MIN_HARD_COORDINATE / 2);
    nodes.emplace_back(MAX_HARD_COORDINATE, MIN_HARD_COORDINATE / 2);
    nodes.emplace_back(                  0, MAX_HARD_COORDINATE    );

    // create the base triangle
    Triangle* base = newTriangle();
    base->assignNodes(&nodes[0], &nodes[1], &nodes[2]);
}

delaunay::Triangle *delaunay::CDT::newTriangle() {
    if(triangles.size() >= triangles.capacity()-2){
        std::cout << "Cannot allocate any more triangles" << std::endl;
        std::exit(-1);
    }else{
        triangles.emplace_back(Triangle{});
        return &triangles.back();
    }
}

delaunay::Triangle *delaunay::CDT::getBaseTriangle() {
    return &triangles[0];
}

delaunay::Triangle *delaunay::CDT::findTriangle(delaunay::Triangle *base, delaunay::Node *node) {
    bool contains = base->contains(node);
    // return null if not contained
    if (!contains)
        return nullptr;
    // if there is no child, use that triangle
    if (base->isValid()) {
        return base;
    }

    // else go through the other triangles and check them
    for (int i = 0; i < base->historyDegree; i++) {
        delaunay::Triangle* tri = findTriangle(base->history[i], node);
        if (tri != nullptr)
            return tri;
    }


    return nullptr;
}

void delaunay::CDT::addNode(delaunay::Node node) {
    // insert nodes into node list
    nodes.push_back(node);

    // retrieve the triangle
    Triangle* tri = findTriangle(getBaseTriangle(), &nodes.back());

    // exit if the triangle could not be found
    if (tri == nullptr) {
        std::cout << "did not find a triangle" << std::endl;
        exit(-2);
    }

    // check if the node lies on the edge or is within the triangle
    bool on_edge_1 = tri->getEdge(0).isOnEdge(&node);
    bool on_edge_2 = tri->getEdge(1).isOnEdge(&node);
    bool on_edge_3 = tri->getEdge(2).isOnEdge(&node);

    if(on_edge_1 + on_edge_2 + on_edge_3 > 1) return;

    // age can be set to the amount of triangles since thats always increasing
    int age = triangles.size();
    if(on_edge_1 || on_edge_2 || on_edge_3){
        if(on_edge_1){
            tri->splitIntoFourTriangles(tri->getEdge(0), *this, &nodes.back());
        }else if(on_edge_2){
            tri->splitIntoFourTriangles(tri->getEdge(1), *this, &nodes.back());
        }else {
            tri->splitIntoFourTriangles(tri->getEdge(2), *this, &nodes.back());
        }
        Edge& e1 = triangles[triangles.size()-1].getEdge(1);
        Edge& e2 = triangles[triangles.size()-2].getEdge(1);
        Edge& e3 = triangles[triangles.size()-3].getEdge(1);
        Edge& e4 = triangles[triangles.size()-4].getEdge(1);

        e1.flip(*this, true, age);
        e2.flip(*this, true, age);
        e3.flip(*this, true, age);
        e4.flip(*this, true, age);


    }else{
        // add three new triangles
        tri->splitIntoThreeTriangles(*this, &nodes.back());
        // flip vertices

        Edge& e1 = triangles[triangles.size()-1].getEdge(0);
        Edge& e2 = triangles[triangles.size()-2].getEdge(0);
        Edge& e3 = triangles[triangles.size()-3].getEdge(0);

        e1.flip(*this, true, age);
        e2.flip(*this, true, age);
        e3.flip(*this, true, age);
    }

}

void delaunay::CDT::addSegment(delaunay::Edge e) {
    std::queue<Edge*> cut_edgdes {};

    // age which can be used to determine if edges pass
    Age age = triangles.size();

    // go through all triangles and check if its intersected by the edge.
    for (Triangle& t : triangles) {
        if (!t.isValid())
            continue;

        for(int n = 0; n < 3; n++){
            // restrict if the edge equals the constrained edge and return
            if(t.getEdge(n) == e){
                t.getEdge(n).restrict();
                return;
            }
        }

        // go through each edge and check if it intersects and has not yet been added to the list of edges
        for(int n = 0; n < 3; n++) {
            if(t.getEdge(n).flipAge >= age) continue;
            if(t.getEdge(n).intersects(&e)) {
                t.getEdge(n).setAge(age);
                cut_edgdes.push(&t.getEdge(n));
            }
        }
    }
    // nothing needs to be done if there are no triangles affected. This is the case
    // if for example, the segment equals an already existing edge.
    if (cut_edgdes.empty())
        return;

    std::vector<Edge*> new_edges{};
    // remove intersection edges as long as some edges still cross the constrained edge
    while(!cut_edgdes.empty()){
        // remove an edge
        Edge* edge = cut_edgdes.front();
        cut_edgdes.pop();

        // check if the two triangles that share this edge form a strictly convex quad
        // if not, put it back at the end of the queue
        if(!edge->isStrictlyConvex()){
            cut_edgdes.push(edge);
            continue;
        }

        // swap the edge
        Triangle* t = edge->triangle;

        // flipping will invalidate certain edges. these need to be fixed
        // for this purpose, we first store the triangles that may change
        Edge* other_edge_next = &edge->other->next();
        Edge* other_edge_prev = &edge->other->prev();
        Edge*       edge_next = &edge->next();
        Edge*       edge_prev = &edge->prev();

        // fix broken edges
        std::queue<Edge*> new_queue{};
        while(!cut_edgdes.empty()){
            Edge* edge_to_check = cut_edgdes.front();
            if(   edge_to_check == other_edge_next
                || edge_to_check == other_edge_prev
                || edge_to_check == edge_next
                || edge_to_check == edge_prev){
                new_queue.push(edge_to_check->other);
            }else{
                new_queue.push(edge_to_check);
            }
            cut_edgdes.pop();
        }
        cut_edgdes = new_queue;
        // flip the edge
        edge->flip();

        Edge* new_edge = &t->getEdge(0);

        Precision intersection_dist = new_edge->intersects(&e);
        if(intersection_dist > 0 && intersection_dist < 1){
            // still intersects
            cut_edgdes.push(new_edge);
        }else{
            new_edges.push_back(new_edge);
        }
    }

    // restore triangulation
    int swaps_happened = true;
    while(swaps_happened){
        swaps_happened = false;
        for(Edge* &edge:new_edges){
            // dont swap an edge if it lays right on the constrained edge
            if(*edge == e){
                edge->restrict();
                continue;
            }
            // check if delaunay is satisfied
            if(!edge->isDelaunay()){
                Triangle* t = edge->triangle;
                edge->flip();
                Edge* new_edge = &t->getEdge(0);
                edge = new_edge;
                swaps_happened = true;
            }
        }
    }
}

void delaunay::CDT::finish() {
    for (Triangle& t : triangles) {
        for(int i = 0; i < 3; i++){
            Node* n = t.getNode(i);
            if(*n == nodes[0] ||
               *n == nodes[1] ||
               *n == nodes[2]){
                t.invalidate();
                break;
            }
        }
    }
}

std::ostream &delaunay::operator<<(std::ostream &os, const delaunay::CDT &cdt) {
    for (const Node& n : cdt.nodes) {
        os << "Node " << &n << " " << n << std::endl;
    }
    int idx = 1;
    int edx = 1;
    for (const Triangle& t : cdt.triangles) {
        if (t.isValid()) {
            os << "Triangle "
               << (idx++) << " "
               << t.getNode(0) << ";"
               << t.getNode(1) << ";"
               << t.getNode(2) << std::endl;
        }
        for(Edge e:t.edges){
            if(e.isRestricted()){
                os << "Edge "
                   << (edx++) << " "
                   << e.n1 << ";"
                   << e.n2 << std::endl;
            }
        }
    }
    return os;
}

