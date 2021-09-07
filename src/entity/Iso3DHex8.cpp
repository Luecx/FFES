//
// Created by Luecx on 06.09.2021.
//
#include "Iso3DHex8.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"

Iso3DHex8::Iso3DHex8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6,
                     int node_7, int node_8)
    : node1(node_1), node2(node_2), node3(node_3), node4(node_4), node5(node_5), node6(node_6),
      node7(node_7), node8(node_8) {}

DenseMatrix Iso3DHex8::computeLocalStiffness() {

    QuickMatrix<24, 24> stiffness {};

    DenseMatrix         integration_points = integrate<ISO_HEX, LINEAR>();
    auto                mat_matrix         = material->getMaterialMatrix3D();

    QuickMatrix<8, 3>   node_coords {};

    node_coords(0, 0) = (*node_data)[POSITION][node1][0];
    node_coords(0, 1) = (*node_data)[POSITION][node1][1];
    node_coords(0, 2) = (*node_data)[POSITION][node1][2];

    node_coords(1, 0) = (*node_data)[POSITION][node2][0];
    node_coords(1, 1) = (*node_data)[POSITION][node2][1];
    node_coords(1, 2) = (*node_data)[POSITION][node2][2];

    node_coords(2, 0) = (*node_data)[POSITION][node3][0];
    node_coords(2, 1) = (*node_data)[POSITION][node3][1];
    node_coords(2, 2) = (*node_data)[POSITION][node3][2];

    node_coords(3, 0) = (*node_data)[POSITION][node4][0];
    node_coords(3, 1) = (*node_data)[POSITION][node4][1];
    node_coords(3, 2) = (*node_data)[POSITION][node4][2];

    node_coords(4, 0) = (*node_data)[POSITION][node5][0];
    node_coords(4, 1) = (*node_data)[POSITION][node5][1];
    node_coords(4, 2) = (*node_data)[POSITION][node5][2];

    node_coords(5, 0) = (*node_data)[POSITION][node6][0];
    node_coords(5, 1) = (*node_data)[POSITION][node6][1];
    node_coords(5, 2) = (*node_data)[POSITION][node6][2];

    node_coords(6, 0) = (*node_data)[POSITION][node7][0];
    node_coords(6, 1) = (*node_data)[POSITION][node7][1];
    node_coords(6, 2) = (*node_data)[POSITION][node7][2];

    node_coords(7, 0) = (*node_data)[POSITION][node8][0];
    node_coords(7, 1) = (*node_data)[POSITION][node8][1];
    node_coords(7, 2) = (*node_data)[POSITION][node8][2];


    for (int i = 0; i < integration_points.m ; i++) {
        Precision        r = integration_points(i, 0);
        Precision        s = integration_points(i, 1);
        Precision        t = integration_points(i, 2);

        Precision       rp = r + 1;
        Precision       sp = s + 1;
        Precision       tp = t + 1;
        Precision       rm = r - 1;
        Precision       sm = s - 1;
        Precision       tm = t - 1;

        // shape function evaluation. 4 entries for 1) sign, 2) r 3) s 4) t
        QuickMatrix<8,4> shape_function{};
        for(int n = 0; n < 8; n++){
            shape_function(n,0) = (n == 0 || n == 2 || n == 5 || n == 7) ? -1:1;
            shape_function(n,1) = ((n + 1) / 2) % 2 == 1 ? rp : rm;
            shape_function(n,2) = ((n    ) / 2) % 2 == 1 ? sp : sm;
            shape_function(n,3) = n >= 4                 ? tp : tm;
        }

        // containing derivatives of the shape functions h1 -> h8 with respect to r/s/t
        QuickMatrix<3,8> local_shape_derivative{};
        for(int n = 0; n < 8; n++){
            local_shape_derivative(0,n) = shape_function(n,0) * shape_function(n,2) * shape_function(n,3);
            local_shape_derivative(1,n) = shape_function(n,0) * shape_function(n,1) * shape_function(n,3);
            local_shape_derivative(2,n) = shape_function(n,0) * shape_function(n,1) * shape_function(n,2);
        }
        local_shape_derivative *= 0.125;

        // compute jacobian
        QuickMatrix<3,3> jacobian_transposed{};
        for(int n = 0; n < 3; n++){
            for(int j = 0; j < 3; j++){
                Precision sum = 0;
                for(int b = 0; b < 8; b++){
                    sum += node_coords(b,n) * local_shape_derivative(j,b);
                }
                jacobian_transposed(n,j) = sum;
            }
        }


        // compute inverse jacobian + determinant
        Precision det;
        auto inv = inverse3x3(jacobian_transposed, det);

        // error handling
        ERROR(det > 0, NEGATIVE_JACOBIAN);
        ERROR(material, NO_MATERIAL_ASSIGNED);

        // compute derivatives with respect to x,y
        auto B_help = inv * local_shape_derivative;
        // formulating Strain-displacement matrix
        QuickMatrix<6,24> B{};
        for(int j = 0; j < 8; j++){
            int r1 = j * 3;
            int r2 = r1 + 1;
            int r3 = r1 + 2;
            B(0,r1) = B_help(0,j);
            B(1,r2) = B_help(1,j);
            B(2,r3) = B_help(2,j);


            B(3,r1) = B_help(1,j);
            B(3,r2) = B_help(0,j);

            B(4,r1) = B_help(2,j);
            B(4,r3) = B_help(0,j);

            B(5,r2) = B_help(2,j);
            B(5,r3) = B_help(1,j);
        }

        stiffness += (!B * mat_matrix * B) * (integration_points(i, 3) * det);
    }

    return DenseMatrix{stiffness};
}
