
//
// Created by Luecx on 26.09.2021.
//
#include "Element.h"
#include "../system/LoadCase.h"


//Precision Element::compliance(LoadCase* load_case) {
//    auto disp = this->getNodalData(DISPLACEMENT, load_case, this->nodeDOF());
//    auto stif = this->computeLocalStiffness(load_case);
//
//    auto comp = (!disp) * (stif * disp);
//    return comp(0,0);
//}
