

#ifndef FEM_SRC_INTEGRATION_QUADRATURE_H_
#define FEM_SRC_INTEGRATION_QUADRATURE_H_

enum Order{
    LINEAR,
    QUADRATIC,
    CUBIC
};

enum Domain{
    ISO_TRI,
    ISO_QUAD,
    ISO_HEX
};

template<Domain domain, Order order>
inline DenseMatrix integrate(){
    if constexpr (domain == ISO_TRI){

        if constexpr (order == LINEAR){
            return DenseMatrix{1,2+1,
                1.0/3.0, 1.0/3.0, 1.0/2.0};
        }

        if constexpr (order == QUADRATIC){
            return DenseMatrix{3,2+1,
                1.0/6.0, 1.0/6.0, 1.0/6.0,
                2.0/3.0, 1.0/6.0, 1.0/6.0,
                1.0/6.0, 2.0/3.0, 1.0/6.0};
        }

        if constexpr (order == CUBIC){
            return DenseMatrix{4,2+1,
                1.0/3.0, 1.0/3.0, -27.0/48.0,
                1.0/5.0, 3.0/5.0,  25.0/48.0,
                1.0/5.0, 1.0/5.0,  25.0/48.0,
                3.0/5.0, 1.0/5.0,  25.0/48.0};
        }
    }

    if constexpr (domain == ISO_QUAD){

        if constexpr (order == LINEAR){
            return DenseMatrix{4,2+1,
                 sqrt(3.0)/3.0,  sqrt(3.0)/3.0, 1.0,
                -sqrt(3.0)/3.0,  sqrt(3.0)/3.0, 1.0,
                -sqrt(3.0)/3.0, -sqrt(3.0)/3.0, 1.0,
                 sqrt(3.0)/3.0, -sqrt(3.0)/3.0, 1.0};
        }

        if constexpr (order == QUADRATIC){
            return DenseMatrix{9,2+1,
                -sqrt(0.6), -sqrt(0.6), 25.0 / 81.0,
                 sqrt(0.0), -sqrt(0.6), 40.0 / 81.0,
                 sqrt(0.6), -sqrt(0.6), 25.0 / 81.0,
                -sqrt(0.6),  sqrt(0.0), 40.0 / 81.0,
                 sqrt(0.0),  sqrt(0.0), 64.0 / 81.0,
                 sqrt(0.6),  sqrt(0.0), 40.0 / 81.0,
                -sqrt(0.6),  sqrt(0.6), 25.0 / 81.0,
                 sqrt(0.0),  sqrt(0.6), 40.0 / 81.0,
                 sqrt(0.6),  sqrt(0.6), 25.0 / 81.0
            };
        }
    }


    if constexpr (domain == ISO_HEX){

        if constexpr (order == LINEAR){
            return DenseMatrix{8,3+1,
                 1.0/sqrt(3.0),  1.0/sqrt(3.0),  1.0/sqrt(3.0), 1.0,
                 1.0/sqrt(3.0),  1.0/sqrt(3.0), -1.0/sqrt(3.0), 1.0,
                 1.0/sqrt(3.0), -1.0/sqrt(3.0),  1.0/sqrt(3.0), 1.0,
                 1.0/sqrt(3.0), -1.0/sqrt(3.0), -1.0/sqrt(3.0), 1.0,
                -1.0/sqrt(3.0),  1.0/sqrt(3.0),  1.0/sqrt(3.0), 1.0,
                -1.0/sqrt(3.0),  1.0/sqrt(3.0), -1.0/sqrt(3.0), 1.0,
                -1.0/sqrt(3.0), -1.0/sqrt(3.0),  1.0/sqrt(3.0), 1.0,
                -1.0/sqrt(3.0), -1.0/sqrt(3.0), -1.0/sqrt(3.0), 1.0};
        }

        if constexpr (order == QUADRATIC){
            DenseMatrix res{27,3+1};

            Precision coords[3] {-sqrt(3.0 / 5.0), 0, sqrt(3.0 / 5.0)};
            Precision weight[3] {5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0};

            int c = 0;
            for (int x = 0; x < 3; x++){
                for (int y = 0; y < 3; y++){
                    for (int z = 0; z < 3; z++){
                        res(c, 0) = coords[x];
                        res(c, 1) = coords[y];
                        res(c, 2) = coords[z];
                        res(c, 3) = weight[x] * weight[y] * weight[z];
                        c ++;
                    }
                }
            }
            return res;
        }
    }
}

#endif    // FEM_SRC_INTEGRATION_QUADRATURE_H_
