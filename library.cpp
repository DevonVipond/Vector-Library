#include "library.h"

#include <iostream>

template <typename T>
    class Vector{
        private:
            unsigned int capacity, size;
            T *dataPtr;
        public:
        Vector(){
            size = 0;
            dataPtr = new T[size];
        }
        Vector(int capacity){
            this->capacity = capacity;
            for(int i = 0; i < capacity; i++){
                dataPtr = new T();
            }
        }
        Vector(const Vector &arg){

        }
        ~Vector(){
            delete []dataPtr;
        }

        Vector &operator[](const int index){
            if(index >= capacity) {
                throw "Out of Range";
            }
            else{

            }
        }

        Vector &operator=(const Vector &arg){

        }

        void push_back(T inputData){
            size++;
            if(size >= capacity){
                T* tempPtr = dataPtr;
                dataPtr = new T[capacity * 2]; //2 is the grown factor

                for(int i = 0; i < size - 1; i++){
                    dataPtr[i] = tempPtr[i];
                }
                dataPtr[size - 1] = inputData;
                delete []tempPtr;
                capacity *= 2;
            }

        }
        int getSize() const{
            return size;
        }
        int getCapacity() const{
            return capacity;
        }

    };