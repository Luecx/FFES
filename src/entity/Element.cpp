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
