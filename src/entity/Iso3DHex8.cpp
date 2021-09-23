
#include "Iso3DHex8.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso3DHex8::Iso3DHex8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6,
                     int node_7, int node_8)
    : node_ids {node_1, node_2, node_3, node_4, node_5, node_6, node_7, node_8} {}

int         Iso3DHex8::nodeCount() { return 8; }
int*        Iso3DHex8::nodeIDS() { return node_ids; }
int         Iso3DHex8::nodeDOF() { return 3; }

DenseMatrix Iso3DHex8::computeLocalStiffness() {

    QuickMatrix<24, 24> stiffness {};

    ERROR(material, MATERIAL_HAS_NO_ELASTICITY, 0);
    ERROR(material->getElasticity(), NO_MATERIAL_ASSIGNED, 0);

    DenseMatrix       integration_points = getIntegrationScheme();
    auto              mat_matrix         = material->getElasticity()->getMaterialMatrix3D();

    QuickMatrix<8, 3> node_coords {};

    for (int i = 0; i < 8; i++) {
        node_coords(i, 0) = (*node_data)[POSITION][node_ids[i]][0];
        node_coords(i, 1) = (*node_data)[POSITION][node_ids[i]][1];
        node_coords(i, 2) = (*node_data)[POSITION][node_ids[i]][2];
    }

    for (int i = 0; i < integration_points.getM(); i++) {
        Precision r   = integration_points(i, 0);
        Precision s   = integration_points(i, 1);
        Precision t   = integration_points(i, 2);

        Precision det = 0;
        auto      B   = computeStrainDisplacementRelation(node_coords, r, s, t, det);
        stiffness += (!B * mat_matrix * B) * (integration_points(i, 3) * det);
    }

    return DenseMatrix {stiffness};
}
DenseMatrix Iso3DHex8::computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) {

    DenseMatrix stresses {6, evaluation_points.getM()};

    ERROR(material, MATERIAL_HAS_NO_ELASTICITY, 0);
    ERROR(material->getElasticity(), NO_MATERIAL_ASSIGNED, 0);

    auto               mat_matrix = material->getElasticity()->getMaterialMatrix3D();

    QuickMatrix<8, 3>  node_coords {};
    QuickMatrix<24, 1> node_vector {};

    for (int i = 0; i < 8; i++) {
        node_coords(i, 0)         = (*node_data)[POSITION][node_ids[i]][0];
        node_coords(i, 1)         = (*node_data)[POSITION][node_ids[i]][1];
        node_coords(i, 2)         = (*node_data)[POSITION][node_ids[i]][2];
        node_vector(i * 3 + 0, 0) = load_case->node_data[DISPLACEMENT][node_ids[i]][0];
        node_vector(i * 3 + 1, 0) = load_case->node_data[DISPLACEMENT][node_ids[i]][1];
        node_vector(i * 3 + 2, 0) = load_case->node_data[DISPLACEMENT][node_ids[i]][2];
    }

    for (int i = 0; i < evaluation_points.getM(); i++) {
        Precision r      = evaluation_points.get(i, 0);
        Precision s      = evaluation_points.get(i, 1);
        Precision t      = evaluation_points.get(i, 2);
        Precision det    = 0;

        auto      B      = computeStrainDisplacementRelation(node_coords, r, s, t, det);
        auto      strain = B * node_vector;
        auto      stress = mat_matrix * strain;

        for (int j = 0; j < stress.getM(); j++) {
            stresses(j, i) = stress(j, 0);
        }
    }
    return stresses;
}
DenseMatrix        Iso3DHex8::getIntegrationScheme() { return integrate<ISO_HEX, LINEAR>(); }
QuickMatrix<6, 24> Iso3DHex8::computeStrainDisplacementRelation(QuickMatrix<8, 3>& node_coords,
                                                                Precision r, Precision s, Precision t,
                                                                Precision& det) {
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

    // compute jacobian
    QuickMatrix<3, 3> jacobian {};
    for (int j = 0; j < 3; j++) {
        for (int n = 0; n < 3; n++) {
            Precision sum = 0;
            for (int b = 0; b < 8; b++) {
                sum += node_coords(b, n) * local_shape_derivative(j, b);
            }
            jacobian(j, n) = sum;
        }
    }

    // compute inverse jacobian + determinant
    auto inv = inverse3x3(jacobian, det);

    // error handling
    ERROR(det > 0, NEGATIVE_JACOBIAN, det);

    // compute derivatives with respect to x,y
    auto               B_help = inv * local_shape_derivative;
    // formulating Strain-displacement matrix
    QuickMatrix<6, 24> B {};
    for (int j = 0; j < 8; j++) {
        int r1   = j * 3;
        int r2   = r1 + 1;
        int r3   = r1 + 2;
        B(0, r1) = B_help(0, j);
        B(1, r2) = B_help(1, j);
        B(2, r3) = B_help(2, j);

        B(3, r1) = B_help(1, j);
        B(3, r2) = B_help(0, j);

        B(4, r1) = B_help(2, j);
        B(4, r3) = B_help(0, j);

        B(5, r2) = B_help(2, j);
        B(5, r3) = B_help(1, j);
    }
    return B;
}
