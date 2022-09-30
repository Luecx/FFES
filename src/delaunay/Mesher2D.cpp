

//
// Created by Luecx on 21.04.2022.
//

#include "Mesher2D.h"

#include <random>
#include <queue>
#include <algorithm>
#include "CDT.h"

delaunay::Mesher2D::Mesher2D(const std::vector<Loop> &loops) : loops{loops}{
    // extract the nodes from the loops. this assumes that no loops cross or share nodes
    std::vector<Node> nodes{};
    for(const Loop &l:loops){
        for(const Border &b:l.borders){
            // insert all the nodes except the last one
            nodes.insert(nodes.end(), b.nodes.begin(), b.nodes.end()-1);
        }
    }
    std::shuffle(nodes.begin(), nodes.end(), std::mt19937());

    CDT cdt{static_cast<int>(nodes.size() * 2)};
    for(const Node& n:nodes){
        cdt.addNode(n);
    }

    cdt.finish();

    // add the border constraints
    for(const Loop &l:loops){
        for(Border b:l.borders){
            // go through each division and add the constraint
            for(int i = 0; i < b.divisions; i++){
                Edge e = Edge{};
                e.n1 = &b.nodes[i];
                e.n2 = &b.nodes[i+1];
                cdt.addSegment(e);
            }
        }
    }

    // remove all triangles outside the borders
    for(Triangle& t:cdt.triangles){
        if(!t.isValid()) continue;
        for(const Loop &l:loops) {
            Node n = t.center();
            if(!l.includes(&n)){
                t.invalidate();
            }
        }
    }

    std::cout << cdt << std::endl;
//
//    bool bad_triangles_found = true;
//    while(bad_triangles_found){
//        bad_triangles_found = false;
//        for(Triangle& t:cdt.all_triangles) {
//            if (!t.isValid())
//                continue;
//            if(t.area() > 1e-3 || (t.aspectRatio() > 100 && !std::isnan(t.aspectRatio()) && std::isfinite(t.aspectRatio()))){
//                Triangle* t1 = cdt.newTriangle();
//                Triangle* t2 = cdt.newTriangle();
//                Triangle* t3 = cdt.newTriangle();
//                cdt.all_nodes.push_back(t.center());
//                t.splitIntoThreeTriangles(*t1,*t2,*t3,&cdt.all_nodes.back());
//                int age = cdt.all_triangles.size();
//                t1->getE1().flipIfRequired(age, cdt);
//                t2->getE1().flipIfRequired(age, cdt);
//                t3->getE1().flipIfRequired(age, cdt);
//                bad_triangles_found = true;
//            }
//        }
//    }
//
//
//    std::cout << cdt << std::endl;
}
delaunay::Precision delaunay::Mesher2D::densityFunction(delaunay::Node node) {
    for(const Loop &l:loops){
        for(Border b:l.borders){
            // go through each division and add the constraint
            for(int i = 0; i < b.divisions; i++){
                Edge e = Edge{};
                e.n1 = &b.nodes[i];
                e.n2 = &b.nodes[i+1];
//                cdt.addSegment(e);
            }
        }
    }
}
