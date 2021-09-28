/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

#ifndef FEM_SRC_CORE_COMPONENTCONTAINER_H_
#define FEM_SRC_CORE_COMPONENTCONTAINER_H_

#include <cstring>
#include <iomanip>
#include <iostream>

template<typename C> struct ComponentContainerSub{
    C* data;

    ComponentContainerSub(C* p_data) : data(p_data) {}

    C& operator[](int sub_index){
        return data[sub_index];
    }

    ComponentContainerSub<C>& operator=(C value){
        data[0] = value;
        return *this;
    }
};

template<typename C> struct ComponentContainer {
    C*   data             = nullptr;
    C*   data_permanent   = nullptr;

    int* indices          = nullptr;
    int  index_count      = 0;
    int  data_count       = 0;

    ComponentContainer() = default;

    virtual ~ComponentContainer() {
        cleanUp();
    }

    ComponentContainer& init(int p_data_count, int p_index_count) {
        this->cleanUp();
        this->data           = new C  [p_data_count ]{};
        this->data_permanent = new C  [p_data_count ]{};
        this->indices        = new int[p_index_count]{};
        this->data_count     = p_data_count;
        this->index_count    = p_index_count;
        return *this;
    }

    ComponentContainer& even(int spacing=0){
        if(spacing == 0) spacing = this->data_count / this->index_count;
        for(int i = 0; i < index_count; i++){
            setSizeIncremental(i, spacing);
        }
        return *this;
    }

    ComponentContainer& fill(C value){
        for(int i = 0; i < data_count; i++){
            data          [i] = value;
            data_permanent[i] = value;
        }

        return *this;
    }

    void cleanUp() {
        if (this->data_count > 0) {
            delete[] this->data;
            this->data = nullptr;
            delete[] this->data_permanent;
            this->data_permanent = nullptr;
            this->data_count = 0;
        }
        if (this->index_count > 0) {
            delete[] this->indices;
            this->indices = nullptr;
            this->index_count = 0;
        }
    }

    uint64_t mem(){
        return 2 * sizeof(int)
            + sizeof(C*) * 2
            + sizeof(int*)
            + sizeof(C) * data_count * 2
            + sizeof(int) * index_count;
    }

    C& at(int index, int sub_index){
        return data[indices[index] + sub_index];
    }

    void set(int index, int sub_index, C value, bool permanent=true){
        if(permanent)
            data_permanent[indices[index]+sub_index] = value;
        data[indices[index]+sub_index] = value;
    }

    ComponentContainerSub<C> operator[](int index){
        return ComponentContainerSub<C>{&data[indices[index]]};
    }

    ComponentContainerSub<C> operator()(int index){
        return ComponentContainerSub<C>{data_permanent[indices[index]]};
    }

    ComponentContainer<C>& operator=(const ComponentContainer<C>& other){
        return this->loadFromPrevious(other);
    }

    ComponentContainer<C>& loadFromPrevious(const ComponentContainer<C>& other){
        this->cleanUp();
        if(!other.permanentIsNotZero() || !other.isInitialised()) return *this;
        this->init(other.data_count, other.index_count);
        std::memcpy(this->indices       , other.indices       , this->index_count * sizeof(int));
        std::memcpy(this->data          , other.data_permanent, this->data_count  * sizeof(C));
        std::memcpy(this->data_permanent, other.data_permanent, this->data_count  * sizeof(C));
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const ComponentContainer& container) {
        os << "Component Container<" << typeid(C).name() << ">\n";
        os << std::setw(18) << "#" << std::setw(18) << "data"<< "\n";
        for(int i = 0; i < container.index_count; i++){
            int lower = container.indices[i];
            int upper = (i == (container.index_count - 1))
                ? container.data_count
                : container.indices[i+1];
            os << std::right;
            os << std::setw(18) << i << std::setw(18) << container.data[lower] << "\n";
            for(int n = lower+1; n < upper; n++){
                os << std::setw(36) << container.data[n] << "\n";
            }
        }
        return os;
    }

    void setSizeIncremental(int id, int size){
        if(id + 1 >= index_count) return;
        this->indices[id+1] = this->indices[id] + size;
    }

    bool isInitialised() const{
        return this->data_count > 0 && this->index_count > 0;
    }

    bool permanentIsNotZero() const{
        for(int i = 0; i < data_count; i++){
            if(data_permanent[i] != 0) return true;
        }
        return false;
    }
};


#endif    // FEM_SRC_CORE_COMPONENTCONTAINER_H_
