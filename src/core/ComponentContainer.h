//
// Created by Luecx on 01.09.2021.
//

#ifndef FEM_SRC_CORE_COMPONENTCONTAINER_H_
#define FEM_SRC_CORE_COMPONENTCONTAINER_H_

#include <iomanip>
#include <iostream>
template<typename C> struct ComponentContainerSub{
    C* data;
    explicit ComponentContainerSub(C* p_data) : data(p_data) {}

    C& operator[](int sub_index){
        return data[sub_index];
    }
};

template<typename C> struct ComponentContainer {
    C*   data            = nullptr;
    int* indices         = nullptr;
    int  index_count     = 0;
    int  data_count      = 0;

    ComponentContainer() = default;

    virtual ~ComponentContainer() {
        cleanUp();
    }

    void init(int p_data_count, int p_index_count) {
        this->cleanUp();
        this->data        = new C[p_data_count]{};
        this->indices     = new int[p_index_count]{};
        this->data_count  = p_data_count;
        this->index_count = p_index_count;
    }

    void cleanUp() {
        if (this->data_count > 0) {
            delete[] this->data;
            this->data = nullptr;
            this->data_count = 0;
        }
        if (this->index_count > 0) {
            this->data = nullptr;
            delete[] this->indices;
            this->index_count = 0;
        }
    }

    uint64_t mem(){
        return 2 * sizeof(int)
            + sizeof(C*)
            + sizeof(int*)
            + sizeof(C) * data_count
            + sizeof(int) * index_count;
    }

    C& at(int index, int sub_index){
        return data[indices[index] + sub_index];
    }

    ComponentContainerSub<C> operator[](int index){
        return ComponentContainerSub<C>{&data[indices[index]]};
    }

    friend std::ostream& operator<<(std::ostream& os, const ComponentContainer& container) {
        os << "Component Container<" << typeid(C).name() << ">\n";
        os << std::setw(10) << "#" << std::setw(10) << "data"<< "\n";
        for(int i = 0; i < container.index_count; i++){
            int lower = container.indices[i];
            int upper = (i == (container.index_count - 1))
                ? container.data_count
                : container.indices[i+1];
            os << std::right;
            os << std::setw(10) << i << std::setw(10) << container.data[lower] << "\n";
            for(int n = lower+1; n < upper; n++){
                os << std::setw(20) << container.data[n] << "\n";
            }
        }
        return os;
    }

    void setIndexIncremental(int id, int size){
        if(id + 1 >= index_count) return;
        this->indices[id+1] = this->indices[id] + size;
    }

    bool isInitialised(){
        return this->data_count > 0 && this->index_count > 0;
    }
};


#endif    // FEM_SRC_CORE_COMPONENTCONTAINER_H_
