/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFES is free software: you can redistribute it and/or modify                  *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFES is distributed in the hope that it will be useful,                    *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

//
// Created by Luecx on 13.04.2022.
//

#ifndef TOPO_PY_SRC_DELAUNEY_TRIANGLE_H_
#define TOPO_PY_SRC_DELAUNEY_TRIANGLE_H_

#include <tuple>
#include <ostream>
#include <iostream>
#include "Node.h"
#include "Edge.h"
namespace delaunay{

struct Triangle {

    Edge e1{};
    Edge e2{};
    Edge e3{};

    // history data
    Triangle* history[3]{};
    int       historyDegree = 0;

    // sets the parent of the edges
    void linkEdges(){
        e1.triangle = this;
        e2.triangle = this;
        e3.triangle = this;
        e1.edge_index = 0;
        e2.edge_index = 1;
        e3.edge_index = 2;
    }

    // sets the edges to form a triangle for the given 3 nodes
    void createEdges(Node* n1, Node* n2, Node* n3){
        e1.n1 = n1;
        e1.n2 = n2;
        e2.n1 = n2;
        e2.n2 = n3;
        e3.n1 = n3;
        e3.n2 = n1;
        linkEdges();
    }

    void checkIntegrity(){

        // check edge nodes
        if( getEdge(0).n1 != getN1() ||
            getEdge(1).n1 != getN2() ||
            getEdge(2).n1 != getN3()){
            std::cout << "edge nodes do not match with triangle" << std::endl;
            std::cout << *this << std::endl;
            exit(-1);
        }

        if( getEdge(0).n2 != getN2() ||
            getEdge(1).n2 != getN3() ||
            getEdge(2).n2 != getN1()){
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
        if( getN1() == getN2() ||
            getN2() == getN3() ||
            getN1() == getN3()){
            std::cout << "triangle has duplicate vertex" << std::endl;
            std::cout << *this << std::endl;
            exit(-1);
        }

        // check that if an edge has a connected edge, the nodes are the same
        for(int i = 0; i < 3; i++){
            if(getEdge(i).other){
                if(getEdge(i).n1 != getEdge(i).other->n2 ||
                   getEdge(i).n2 != getEdge(i).other->n1 ){
                    std::cout << "edge has wrong link" << std::endl;
                    std::cout << *this << std::endl;
                    exit(-1);
                }

                // check that two connected triangles cannot have one of them with a history index
                // of 0
                if( getEdge(i).triangle->historyDegree != 0 && historyDegree == 0){
                    std::cout << "connected triangle has different history degree where one has a history degree of 0" << std::endl;
                    std::cout << *this << std::endl;
                    exit(-1);
                }
            }
        }
    }

    // algorithm from
    // https://www.newcastle.edu.au/__data/assets/pdf_file/0017/22508/
    // 13_A-fast-algorithm-for-constructing-Delaunay-triangulations-in-the-plane.pdf
    bool imscribed(Node* nP) {
        Precision x13 = e1.n1->x - e3.n1->x;
        Precision x23 = e2.n1->x - e3.n1->x;
        Precision x1P = e1.n1->x - nP->x;
        Precision x2P = e2.n1->x - nP->x;

        Precision y13 = e1.n1->y - e3.n1->y;
        Precision y23 = e2.n1->y - e3.n1->y;
        Precision y1P = e1.n1->y - nP->y;
        Precision y2P = e2.n1->y - nP->y;

        return   (x13 * x23 + y13 * y23) * (x2P * y1P - x1P * y2P)
               < (y13 * x23 - x13 * y23) * (x2P * x1P + y1P * y2P);
    }

    // splits the given triangle into three smaller triangles
    void splitIntoThreeTriangles(Triangle& t1, Triangle &t2, Triangle &t3, Node* np){

        // assign edges
        t1.createEdges(getN1(), getN2(), np);
        t2.createEdges(getN2(), getN3(), np);
        t3.createEdges(getN3(), getN1(), np);

        // link outwards
        if(e1.other)
            t1.e1.link(e1.other);
        if(e2.other)
            t2.e1.link(e2.other);
        if(e3.other)
            t3.e1.link(e3.other);

        // link inner
        t1.e2.link(&t2.e3);
        t2.e2.link(&t3.e3);
        t3.e2.link(&t1.e3);

        historyDegree = 3;
        history[0] = &t1;
        history[1] = &t2;
        history[2] = &t3;
    }

    // checks if the node is contained within the triangle
    bool contains(Node* nP){
        return e1.isLeft(nP) &&
               e2.isLeft(nP) &&
               e3.isLeft(nP);
    }

    // return the node which is opposite of the given edge
    Node* opposite(Edge* edge){
        if(edge == &e1) return getN3();
        if(edge == &e2) return getN1();
        if(edge == &e3) return getN2();
        exit(-1);
        return nullptr;
    }

    // return the given edge
    Edge& getEdge(int idx){
        switch (idx){
            case 0: return e1;
            case 1: return e2;
            case 2: return e3;
            default: exit(-1);
        }
    }

    // some getters
    Node* getN1() const { return e1.n1; }
    Node* getN2() const { return e2.n1; }
    Node* getN3() const { return e3.n1; }
    Edge& getE1() { return e1; }
    Edge& getE2() { return e2; }
    Edge& getE3() { return e3; }

    friend std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
        os << "Triangle: " << &triangle  << " " << triangle.historyDegree << std::endl;
        os << "    N1: " << *triangle.getN1() << "\n";
        os << "    N2: " << *triangle.getN2() << "\n";
        os << "    N3: " << *triangle.getN3() << "\n";
        os << "    E1: " << &triangle.e1 << " " << triangle.e1 << "\n";
        os << "    E2: " << &triangle.e2 << " " << triangle.e2 << "\n";
        os << "    E3: " << &triangle.e3 << " " << triangle.e3 << "\n";
        return os;
    }
};

}

#endif    // TOPO_PY_SRC_DELAUNEY_TRIANGLE_H_
