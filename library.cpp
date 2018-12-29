#include <stdio.h>
#include <iostream>

#define GROWTHFACTOR 2//rename GROWTHFACTOR

using namespace std;
/*
TODO
google unit testing
operator !=
emplace, emplace_back

*/
template <class T, class A = std::allocator<T> >
class Vector{
    private:
        T* data_;         // points to first element
        T* end_capacity_; // points to one past internal storage
        T* end_;          // points to one past last element
        int capacity_;
        int size_;
        A  allocator_;
        
        // Changes the amount of allocated memory
        void reallocate(int oldCapacity, int newCapacity){ 
            // Check if newCapacity is greater than largest possible allocation size
            if (newCapacity > allocator_.max_size()) {
                throw std::length_error("newCapacity > max_size");
            }
            T* temp = data_;
            data_ = 0;
            allocator_.deallocate(temp, oldCapacity);
            capacity_ = newCapacity;
            data_ = allocator_.allocate(newCapacity);
            end_ = data_;
            end_capacity_ = data_ + capacity_;
        }
        
        // Changes the amount of allocated memory
        void reallocate(int oldCapacity, int newCapacity, T* dataToCopy){ 
            if (newCapacity > allocator_.max_size()) {
                throw std::length_error("newCapacity > max_size");
            }
            int oldSize = end_ - data_;
            data_ = 0;
            data_ = allocator_.allocate(newCapacity);
            for(int i = 0; i < oldSize; i++)
                data_[i] = dataToCopy[i];
            allocator_.deallocate(dataToCopy, oldCapacity);
            capacity_ = newCapacity;
            end_ = data_ + oldSize;
            end_capacity_ = data_ + newCapacity;
        }
        
        void print(){
            cout << endl << "------------------------------" << endl;
            cout << "Print data_: " << endl;
            cout << "Size: ";
            cout << this->getSize() << endl;
            cout << "Capacity: ";
            cout << this->getCapacity() << endl;
            cout << "actual data: ";
            for(int i = 0; i < this->getSize(); i++)
            cout << data_[i] << " ";  
            cout << endl << "------------------------------" << endl;
        }
    public:
    
        Vector(): size_(0), capacity_(10){  //10 is the default number of elements
            data_ = allocator_.allocate(10);
            end_capacity_ = data_ + 10;
            end_ = data_;
        }

        Vector(int capacity): size_(0), capacity_(capacity){
            data_ = allocator_.allocate(capacity);
            end_capacity_ = data_ + capacity;
            end_ = data_;
        }
        
        Vector(const Vector &arg){ 
            capacity_ = arg.getCapacity();
            data_ = allocator_.allocate(capacity_);
            for(int i = 0; i < arg.getCapacity(); i++){ //should probably be end_;
                data_[i] = arg[i];
            }
            end_capacity_ = data_ + capacity_;
            end_ = data_ + arg.getSize();
        }
        
        Vector &operator=(const Vector &arg){//check if this equals arg
            //if(arg != this){
                reallocate(capacity_, arg.getCapacity());
                for(int i = 0; i < arg.getSize(); i++){ 
                    data_[i] = arg[i];
                }
                end_ = data_ + arg.getSize();
            //}
        }
        
        T &operator[](int index){
                if(index >= (end_capacity_ - data_) || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    if(data_ + index >= end_)
                        end_ = data_ + index;
                        
                    return data_[index];
                }
            }
        
        const T &operator[](int index) const{ //problem, this may or may not increment size;
                if(index >= (end_capacity_ - data_) || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    return data_[index];
                }
            }
        
        void push_back(T input){
            if(end_ >= end_capacity_){
                int newCapacity = getCapacity() * GROWTHFACTOR; 
                T* tempPtr = data_;
                data_ = allocator_.allocate(newCapacity); 
                for(int i = 0; i <= (end_ - tempPtr); i++){
                    if(i == (end_ - tempPtr))
                        data_[i] = input;
                    else
                        data_[i] = tempPtr[i];
                }
                end_ = data_ + (end_ - tempPtr) + 1;
                end_capacity_ = newCapacity + data_;
                allocator_.deallocate(tempPtr, capacity_);
            }
            else{
                *end_ = input;
                 end_++;
            }
        }

        int getCapacity() const{
            return end_capacity_ - data_;        
        }

        int getSize() const{
            return end_ - data_;
        }
};

int main()
{
    Vector<char> x(1);
    x.push_back('a');
    x.push_back('b');
    /*x.push_back(3);
    x.push_back(4);
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);*/
     Vector<char> y(2);
    y.push_back(51);
    y.push_back(11);
    y.push_back(21);
    y.push_back(31);
    y.push_back(41);
    y.push_back(99);
   

    y = x;
    std::cout << y[0] << endl;
    std::cout << y[1] << endl;
  /* std::cout << y[2] << endl;
    std::cout << y[3] << endl;
    std::cout << y[4] << endl;
    std::cout << y[5] << endl;
    std::cout << y[6] << endl;
    std::cout << y[8] << endl;*/
    
    return 0;
}

