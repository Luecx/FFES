

//
// Created by Luecx on 23.04.2022.
//

#include "Triangle.h"
delaunay::Precision delaunay::Triangle::area() const {
    Coordinate x1 = edges[0].n1->x;
    Coordinate x2 = edges[1].n1->x;
    Coordinate x3 = edges[2].n1->x;
    Coordinate y1 = edges[0].n1->y;
    Coordinate y2 = edges[1].n1->y;
    Coordinate y3 = edges[2].n1->y;

    Coordinate double_area =
        ((x1 * y2 + y1 * x3 + x2 * y3)
            - (x1 * y3 + y1 * x2 + y2 * x3));

    return static_cast<Precision>(double_area) / 2.0;
}
delaunay::Precision delaunay::Triangle::aspectRatio() const {
    Precision a = edges[0].length();
    Precision b = edges[1].length();
    Precision c = edges[2].length();

    Precision s = 0.5 * (a + b + c);

    return (a * b * c) / ( 8 * (s-a) * (s-b) * (s-c));
}
delaunay::Edge &delaunay::Triangle::getEdge(delaunay::Index idx) {
    return edges[idx];
}
delaunay::Node *delaunay::Triangle::getNode(delaunay::Index idx) const {
    return edges[idx].n1;
}
delaunay::Node delaunay::Triangle::center() const {
    Node result = *getNode(0) + *getNode(1) + *getNode(2);
    result.x /= 3;
    result.y /= 3;
    return result;
}
delaunay::Node *delaunay::Triangle::opposite(const delaunay::Edge &e) {
    return getNode(prevIndex(e.edgeIndex));
}
delaunay::Index delaunay::Triangle::getNodeIndex(delaunay::Node *node) {
    if(*node == *edges[0].n1) return 0;
    if(*node == *edges[1].n1) return 1;
    if(*node == *edges[2].n1) return 2;
    return -1;
}
bool delaunay::Triangle::contains(delaunay::Node *nP) const {
    return edges[0].isLeft(nP) &&
           edges[1].isLeft(nP) &&
           edges[2].isLeft(nP);
}
bool delaunay::Triangle::isCircumscribed(delaunay::Node *n) const {
    auto x13 = static_cast<Precision>(edges[0].n1->x - edges[2].n1->x);
    auto x23 = static_cast<Precision>(edges[1].n1->x - edges[2].n1->x);
    auto x1P = static_cast<Precision>(edges[0].n1->x - n->x);
    auto x2P = static_cast<Precision>(edges[1].n1->x - n->x);

    auto y13 = static_cast<Precision>(edges[0].n1->y - edges[2].n1->y);
    auto y23 = static_cast<Precision>(edges[1].n1->y - edges[2].n1->y);
    auto y1P = static_cast<Precision>(edges[0].n1->y - n->y);
    auto y2P = static_cast<Precision>(edges[1].n1->y - n->y);

    return   (x13 * x23 + y13 * y23) * (x2P * y1P - x1P * y2P)
           < (y13 * x23 - x13 * y23) * (x2P * x1P + y1P * y2P);
}
bool delaunay::Triangle::isValid() const {
    return historyDegree == 0;
}
void delaunay::Triangle::invalidate() {
    historyDegree = -1;
}
void delaunay::Triangle::assignNodes(delaunay::Node *n1, delaunay::Node *n2, delaunay::Node *n3) {
    Node* nodes[3]{n1,n2,n3};
    for(int i = 0; i < 3; i++){
        // assign the nodes
        edges[i].n1 = nodes[i];
        edges[i].n2 = nodes[nextIndex(i)];
        // assign the triangle
        edges[i].triangle = this;
        // assign the edge index
        edges[i].edgeIndex = i;
    }
}
void delaunay::Triangle::splitIntoThreeTriangles(CDT& cdt,
                                                 delaunay::Node *np) {
    // assign edges
    Triangle* t1 = cdt.newTriangle();
    Triangle* t2 = cdt.newTriangle();
    Triangle* t3 = cdt.newTriangle();

    t1->assignNodes(getNode(0), getNode(1), np);
    t2->assignNodes(getNode(1), getNode(2), np);
    t3->assignNodes(getNode(2), getNode(0), np);

    // link outwards
    if(edges[0].other)
        t1->edges[0].link(edges[0].other);
    if(edges[1].other)
        t2->edges[0].link(edges[1].other);
    if(edges[2].other)
        t3->edges[0].link(edges[2].other);

    // link inner
    t1->edges[1].link(&t2->edges[2]);
    t2->edges[1].link(&t3->edges[2]);
    t3->edges[1].link(&t1->edges[2]);

    historyDegree = 3;
    history[0] = t1;
    history[1] = t2;
    history[2] = t3;
}
void delaunay::Triangle::splitIntoFourTriangles(delaunay::Edge &e, delaunay::CDT &cdt, delaunay::Node *node) {

    /**
     *
     *           *                                      *  N1
     *         / | \                                  /2|3\
     *       /   |   \                     e_o4     /   |   \    e_o1
     *     /     |     \                          /  T4 | T1  \
     *   /       |       \                      /3     1|1     2\
     *  *  L   node   R   *     ----->      N4 *--------+--------*  N3
     *   \       |T      /                      \2     1|1     3/
     *     \     |H    /                          \ T3  |  T2 /
     *       \   |I  /                     e_o3     \   |   /    e_o2
     *         \ |S/                                  \3|2/
     *           *                                      *  N2
     *
     *
     */
    Triangle* t1  = cdt.newTriangle();
    Triangle* t2  = cdt.newTriangle();
    Triangle* t3  = cdt.newTriangle();
    Triangle* t4  = cdt.newTriangle();

    Triangle* right = this;
    Triangle* left  = e.other->triangle;

    Node* n0  = node;
    Node* n1  = e.n1;
    Node* n2  = e.n2;
    Node* n3  = e.opposite();
    Node* n4  = e.other->opposite();

    Edge* eo1 = e.prev().other;
    Edge* eo2 = e.next().other;
    Edge* eo3 = e.other->prev().other;
    Edge* eo4 = e.other->next().other;

    // create triangles
    t1->assignNodes(n0, n3, n1);
    t2->assignNodes(n0, n2, n3);
    t3->assignNodes(n0, n4, n2);
    t4->assignNodes(n0, n1, n4);

    // link edges together
    t1->edges[0].link(&t2->edges[2]);
    t2->edges[0].link(&t3->edges[2]);
    t3->edges[0].link(&t4->edges[2]);
    t4->edges[0].link(&t1->edges[2]);

    t1->edges[1].link(eo1);
    t2->edges[1].link(eo2);
    t3->edges[1].link(eo3);
    t4->edges[1].link(eo4);

    // histories
    right->historyDegree = 2;
    right->history[0] = t1;
    right->history[1] = t2;
    left ->historyDegree = 2;
    left ->history[0] = t3;
    left ->history[1] = t4;
}
std::ostream &delaunay::operator<<(std::ostream &os, const delaunay::Triangle &triangle) {
    os << "Triangle: " << &triangle  << " " << triangle.historyDegree << std::endl;
    os << "    N1: " << *triangle.getNode(0) << "\n";
    os << "    N2: " << *triangle.getNode(1) << "\n";
    os << "    N3: " << *triangle.getNode(2) << "\n";
    os << "    E1: " << &triangle.edges[0] << " " << triangle.edges[0] << "\n";
    os << "    E2: " << &triangle.edges[1] << " " << triangle.edges[1] << "\n";
    os << "    E3: " << &triangle.edges[2] << " " << triangle.edges[2] << "\n";
    return os;
}

void delaunay::Triangle::checkIntegrity() {

    // check that there area is not negative or 0
    if(area() < 1e-16){
        std::cout << "triangle seems to be malformed" << std::endl;
        std::cout << *this << std::endl;
        exit(-1);
    }

    // check edge nodes
    if( getEdge(0).n1 != getNode(0) ||
        getEdge(1).n1 != getNode(1) ||
        getEdge(2).n1 != getNode(2)){
        std::cout << "edge nodes do not match with triangle" << std::endl;
        std::cout << *this << std::endl;
        exit(-1);
    }

    if( getEdge(0).n2 != getNode(1)||
        getEdge(1).n2 != getNode(2) ||
        getEdge(2).n2 != getNode(0)){
        std::cout << "edge nodes do not match with triangle" << std::endl;
        std::cout << *this << std::endl;
        exit(-1);
    }

    // check that each edge has this triangle
    if( getEdge(0).triangle != this ||
        getEdge(1).triangle != this ||
        getEdge(2).triangle != this){
        std::cout << "edge has wrong reference to triangle" << std::endl;
        std::cout << *this << std::endl;
        exit(-1);
    }

    // check that no two nodes are the same
    if( getNode(0) == getNode(1) ||
        getNode(1) == getNode(2) ||
        getNode(0) == getNode(2)){
        std::cout << "triangle has duplicate vertex" << std::endl;
        std::cout << *this << std::endl;
        exit(-1);
    }

    // check that if an edge has a connected edge, the nodes are the same
    for(int i = 0; i < 3; i++){
        if(getEdge(i).other){
            if(getEdge(i).n1 != getEdge(i).other->n2 ||
                getEdge(i).n2 != getEdge(i).other->n1 ){
                std::cout << "edge << [" << (i+1) <<  "] has wrong link" << std::endl;
                std::cout << "edge : " << getEdge(i) << std::endl;
                std::cout << "other: " << *getEdge(i).other << std::endl;
                std::cout << *this << std::endl;
                exit(-1);
            }

            // check that two connected triangles cannot have one of them with a history index
            // of 0
            if( !getEdge(i).triangle->isValid() && isValid()){
                std::cout << "connected triangle has different history degree where one has a history degree of 0" << std::endl;
                std::cout << *this << std::endl;
                exit(-1);
            }
        }
    }
}
