
//
// Created by Luecx on 24.04.2022.
//

#include "Border.h"

#include <utility>
#include <numeric>
#include <cmath>

delaunay::Border::Border(delaunay::Node n_1, delaunay::Node n_2, int divisions)
    : n1(std::move(n_1)), n2(std::move(n_2)), divisions(divisions) {
    nodes.push_back(n1);

    // compute the greatest common divisor in order to know how many possible divisions are possible
    Coordinate gcd = std::gcd(n1.x - n2.x, n1.y - n2.y);

    // correct division count
    divisions = static_cast<int>(std::min(static_cast<Coordinate>(divisions), gcd));

    Coordinate x_step = (n2.x - n1.x) / gcd;
    Coordinate y_step = (n2.y - n1.y) / gcd;

    for(int i = 0; i < divisions-1; i++){
        Coordinate multiple = std::round(gcd * (static_cast<Precision>(i+1) / divisions));

        auto x = static_cast<HalfCoordinate>(n1.x + x_step * multiple);
        auto y = static_cast<HalfCoordinate>(n1.y + y_step * multiple);
        nodes.emplace_back(x,y);
    }
    nodes.push_back(n2);
}
