
#include "Iso3DHex8.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso3DHex8::Iso3DHex8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6,
                     int node_7, int node_8) {
    node_ids[0] = node_1;
    node_ids[1] = node_2;
    node_ids[2] = node_3;
    node_ids[3] = node_4;
    node_ids[4] = node_5;
    node_ids[5] = node_6;
    node_ids[6] = node_7;
    node_ids[7] = node_8;
}

QuickMatrix<3, 8> Iso3DHex8::getLocalShapeDerivative(Precision r, Precision s, Precision t) {

    Precision         rp = r + 1;
    Precision         sp = s + 1;
    Precision         tp = t + 1;
    Precision         rm = r - 1;
    Precision         sm = s - 1;
    Precision         tm = t - 1;

    // shape function evaluation. 4 entries for 1) sign, 2) r 3) s 4) t
    QuickMatrix<8, 4> shape_function {};
    for (int n = 0; n < 8; n++) {
        shape_function(n, 0) = (n == 0 || n == 2 || n == 5 || n == 7) ? -1 : 1;
        shape_function(n, 1) = ((n + 1) / 2) % 2 == 1 ? rp : rm;
        shape_function(n, 2) = ((n) / 2) % 2 == 1 ? sp : sm;
        shape_function(n, 3) = n >= 4 ? tp : tm;
    }

    // containing derivatives of the shape functions h1 -> h8 with respect to r/s/t
    QuickMatrix<3, 8> local_shape_derivative {};
    for (int n = 0; n < 8; n++) {
        local_shape_derivative(0, n) =
            shape_function(n, 0) * shape_function(n, 2) * shape_function(n, 3);
        local_shape_derivative(1, n) =
            shape_function(n, 0) * shape_function(n, 1) * shape_function(n, 3);
        local_shape_derivative(2, n) =
            shape_function(n, 0) * shape_function(n, 1) * shape_function(n, 2);
    }
    local_shape_derivative *= 0.125;
    return local_shape_derivative;
}

QuickMatrix<6, 24> Iso3DHex8::computeStrainDisplacementRelationFromSource(QuickMatrix<3, 8> b_help) {

    QuickMatrix<6, 24> B {};
    for (int j = 0; j < 8; j++) {
        int r1   = j * 3;
        int r2   = r1 + 1;
        int r3   = r1 + 2;
        B(0, r1) = b_help(0, j);
        B(1, r2) = b_help(1, j);
        B(2, r3) = b_help(2, j);

        B(3, r1) = b_help(1, j);
        B(3, r2) = b_help(0, j);

        B(4, r1) = b_help(2, j);
        B(4, r3) = b_help(0, j);

        B(5, r2) = b_help(2, j);
        B(5, r3) = b_help(1, j);
    }
    return B;
}

DenseMatrix Iso3DHex8::getIntegrationScheme() { return integrate<ISO_HEX, LINEAR>(); }
