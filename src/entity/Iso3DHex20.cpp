
#include "Iso3DHex20.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso3DHex20::Iso3DHex20(int node_1,
                       int node_2,
                       int node_3,
                       int node_4,
                       int node_5,
                       int node_6,
                       int node_7,
                       int node_8,
                       int node_9,
                       int node_10,
                       int node_11,
                       int node_12,
                       int node_13,
                       int node_14,
                       int node_15,
                       int node_16,
                       int node_17,
                       int node_18,
                       int node_19,
                       int node_20) {
    node_ids[0]  = node_1;
    node_ids[1]  = node_2;
    node_ids[2]  = node_3;
    node_ids[3]  = node_4;
    node_ids[4]  = node_5;
    node_ids[5]  = node_6;
    node_ids[6]  = node_7;
    node_ids[7]  = node_8;
    node_ids[8]  = node_9;
    node_ids[9]  = node_10;
    node_ids[10] = node_11;
    node_ids[11] = node_12;
    node_ids[12] = node_13;
    node_ids[13] = node_14;
    node_ids[14] = node_15;
    node_ids[15] = node_16;
    node_ids[16] = node_17;
    node_ids[17] = node_18;
    node_ids[18] = node_19;
    node_ids[19] = node_20;
}

QuickMatrix<3, 20> Iso3DHex20::getLocalShapeDerivative(Precision r, Precision s, Precision t) {

    Precision         rp = 1 + r;
    Precision         sp = 1 + s;
    Precision         tp = 1 + t;
    Precision         rm = 1 - r;
    Precision         sm = 1 - s;
    Precision         tm = 1 - t;

    // containing derivatives of the shape functions h1 -> h8 with respect to r/s/t
    QuickMatrix<3, 20> local_shape_derivative {};

    // first 8 nodes
    for(int i = 0; i < 8; i++){
        Precision sign_r1 = (((i+1)/2) % 2 == 0 ? -1:1);
        Precision sign_s1 = ( (   i/2) % 2 == 0 ? -1:1);
        Precision sign_t1 = ( (   i/4) % 2 == 0 ? -1:1);

        Precision sign_r2 = -sign_r1;
        Precision sign_s2 = -sign_s1;
        Precision sign_t2 = -sign_t1;

        Precision sum_1 = 2
            + r * sign_r2
            + s * sign_s2
            + t * sign_t2;

        Precision sum_r = (((i+1)/2) % 2 == 0 ? rm:rp);
        Precision sum_s = ( (   i/2) % 2 == 0 ? sm:sp);
        Precision sum_t = ( (   i/4) % 2 == 0 ? tm:tp);

        local_shape_derivative(0,i) = -1.0/8.0 * sum_s * sum_t * (sign_r1 * sum_1 + sign_r2 * sum_r);
        local_shape_derivative(1,i) = -1.0/8.0 * sum_r * sum_t * (sign_s1 * sum_1 + sign_s2 * sum_s);
        local_shape_derivative(2,i) = -1.0/8.0 * sum_r * sum_s * (sign_t1 * sum_1 + sign_t2 * sum_t);
    }

    // remaining 12 nodes
    for(int i = 8; i < 20; i++){
        Precision r1 = (((i+1)/2) % 2 == 0 ? rm:rp);
        Precision r2 = (((      i % 2 == 0 && i < 16 ? (i % 4 == 0 ? rp:rm):1)));
        Precision s1 = (((i  )/2) % 2 == 0 ? sm:sp);
        Precision s2 = (((      i % 2 == 1 && i < 16 ? (i % 4 == 1 ? sp:sm):1)));
        Precision t1 = (((i  )/4) % 2 == 0 ? tm:tp);
        Precision t2 = (            i < 16 ? 1: tp);

        Precision sign_r1 = (((i+1)/2) % 2 == 0 ? -1:+1);
        Precision sign_r2 = (((      i % 2 == 0 && i < 16 ? (i % 4 == 0 ? +1:-1):0)));
        Precision sign_s1 = (((i  )/2) % 2 == 0 ? -1:+1);
        Precision sign_s2 = (((      i % 2 == 1 && i < 16 ? (i % 4 == 1 ? +1:-1):0)));
        Precision sign_t1 = (((i  )/4) % 2 == 0 ? -1:+1);
        Precision sign_t2 = (            i < 16 ?  0:+1);

        local_shape_derivative(0,i) = 1.0/4.0 * s1 * s2 * t1 * t2 * (r1 * sign_r2 + r2 * sign_r1);
        local_shape_derivative(1,i) = 1.0/4.0 * r1 * r2 * t1 * t2 * (s1 * sign_s2 + s2 * sign_s1);
        local_shape_derivative(2,i) = 1.0/4.0 * r1 * r2 * s1 * s2 * (t1 * sign_t2 + t2 * sign_t1);
    }

    return local_shape_derivative;
}

QuickMatrix<6, 60> Iso3DHex20::computeStrainDisplacementRelationFromSource(QuickMatrix<3, 20> b_help) {

    QuickMatrix<6, 60> B {};
    for (int j = 0; j < 20; j++) {
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

DenseMatrix Iso3DHex20::getIntegrationScheme() { return integrate<ISO_HEX, QUADRATIC>(); }
Precision   Iso3DHex20::interpolate(QuickMatrix<20> matrix, Precision r, Precision s, Precision t) {

    Precision         rp = 1 + r;
    Precision         sp = 1 + s;
    Precision         tp = 1 + t;
    Precision         rm = 1 - r;
    Precision         sm = 1 - s;
    Precision         tm = 1 - t;

    Precision result = 0;

    // first 8 nodes
    for(int i = 0; i < 8; i++){
        Precision sign_r1 = (((i+1)/2) % 2 == 0 ? -1:1);
        Precision sign_s1 = ( (   i/2) % 2 == 0 ? -1:1);
        Precision sign_t1 = ( (   i/4) % 2 == 0 ? -1:1);

        Precision sign_r2 = -sign_r1;
        Precision sign_s2 = -sign_s1;
        Precision sign_t2 = -sign_t1;

        Precision sum_1 = 2
            + r * sign_r2
            + s * sign_s2
            + t * sign_t2;

        Precision sum_r = (((i+1)/2) % 2 == 0 ? rm:rp);
        Precision sum_s = ( (   i/2) % 2 == 0 ? sm:sp);
        Precision sum_t = ( (   i/4) % 2 == 0 ? tm:tp);

        result += matrix(i,0) * -1.0/8.0 * sum_r * sum_s * sum_t * sum_1;
    }
    // remaining 12 nodes
    for(int i = 8; i < 20; i++){
        Precision r1 = (((i+1)/2) % 2 == 0 ? rm:rp);
        Precision r2 = (((      i % 2 == 0 && i < 16 ? (i % 4 == 0 ? rp:rm):1)));
        Precision s1 = (((i  )/2) % 2 == 0 ? sm:sp);
        Precision s2 = (((      i % 2 == 1 && i < 16 ? (i % 4 == 1 ? sp:sm):1)));
        Precision t1 = (((i  )/4) % 2 == 0 ? tm:tp);
        Precision t2 = (            i < 16 ? 1: tp);

        result += matrix(i,0) * 1.0/4.0 * r1 * r2 * s1 * s2 * t1 * t2;
    }
    return result;
}
