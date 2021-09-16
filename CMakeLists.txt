cmake_minimum_required(VERSION 3.19)
project(FEM)

set(CMAKE_CXX_STANDARD 20)

add_executable(FEM
        main.cpp
        src/core/ComponentContainer.h
        src/core/NodeData.h
        src/entity/Element.h
        src/model/Model.h                       src/model/buildReducedStiffness.cpp
                                                src/model/buildReducedLoadVector.cpp
                                                src/model/numerateUnconstrainedNodes.cpp
                                                src/model/solidSection.cpp
                                                src/model/manageMaterials.cpp
                                                src/model/manageEntities.cpp
                                                src/model/manageSets.cpp
                                                src/model/postProcessDisplacements.cpp
        src/system/manageConstraints.cpp
        src/entity/Iso2DTri3.h                  src/entity/Iso2DTri3.cpp
        src/entity/Iso2DQuad4.h                 src/entity/Iso2DQuad4.cpp
        src/entity/Iso3DHex8.h                  src/entity/Iso3DHex8.cpp
        src/matrix/DenseMatrix.h
        src/matrix/Inverse.h
        src/matrix/QuickMatrix.h
        src/matrix/Matrix.h
        src/reader/Reader.h
        src/system/LoadCase.h
        src/system/System.h
        src/matrix/Inverse.h
        src/assert/Assert.h
        src/assert/Logger.h
        src/assert/Error.h
        src/material/Material.h
        src/material/IsotropicElasticity.h      src/material/IsotropicElasticity.cpp
        src/core/Set.h                          src/core/Set.cpp
        src/core/defs.h
        src/integration/Quadrature.h
        src/solver/CG.h                         src/solver/CG.cpp
        src/reader/LineType.h
        src/reader/LineData.h
        src/material/Elasticity.h src/assert/Warning.h
        src/system/LoadCase.h
        src/system/manageLoads.cpp
        src/system/manageConstraints.cpp src/system/reconstructBoundary.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")