#include <stdio.h>
#include <iostream>

#define GROWTHFACTOR 2//rename GROWTHFACTOR

using namespace std;

template <class T, class Allocator = std::allocator<T> >
class Vector{
    public:
        using allocator_type  = Allocator;
        using value_type      = typename Allocator::value_type;
        using size_type       = typename Allocator::size_type;
        using pointer         = typename Allocator::pointer;
        
        // TODO: implement iterator
        // class iterator friend class Vector{
            
        // };
        
    private:
        using allocatorTraits = std::allocator_traits<allocator_type>;

        pointer data_         = nullptr;         // points to first element
        pointer end_capacity_ = nullptr;         // points to one past internal storage
        pointer end_          = nullptr;         // points to one past last element
        size_type capacity_;
        size_type size_;
        Allocator  allocator_;
        Allocator vectorAllocator_;
        
        pointer allocate(size_type capacity){
            return allocatorTraits::allocate(vectorAllocator_, capacity);
        }
        
        void deallocate(pointer data, size_type capacity){
            allocatorTraits::deallocate(vectorAllocator_, data, capacity);
        }
        
        // Changes the amount of allocated memory if necessary
        void reallocate(size_type oldCapacity, size_type newCapacity){ 
            // Check if newCapacity is greater than largest possible allocation size
            if (newCapacity >  vectorAllocator_.max_size()) {
                throw std::length_error("vector::reallocate() newCapacity > max_size");
            }
            pointer newDataBegin = allocate(newCapacity);
            pointer newDataEnd = newDataBegin;
            
            auto temp = data_;
            while(temp != end_){
                allocatorTraits::construct(vectorAllocator_, newDataEnd, std::move(*temp));
                allocatorTraits::destroy(vectorAllocator_, temp);
                ++temp;
                ++newDataEnd;
            }
            
            deallocate(data_, end_capacity_ - data_);
            
            data_ = newDataBegin;
            end_ = newDataEnd;
            end_capacity_ = newDataBegin + newCapacity;
        }
        
        void ReallocateIfFullCapacity(){
            const auto GROWTH_FACTOR = 2;
            size_type newCap = capacity() * GROWTH_FACTOR; //what if newCap zero?
            reallocate(capacity(), newCap);
        }
        
    public:
    
        Vector(){  //10 is the default number of elements
            const auto defaultSize = 10;
            data_ = allocate(defaultSize);
            end_capacity_ = data_ + defaultSize;
            end_ = data_;
         }

        Vector(size_type capacity){
            if(capacity < 0){
                throw std::length_error("invalid capacity");
            }
            data_ = allocate(capacity);
            end_capacity_ = data_ + capacity;
            end_ = data_;
        }
        
        // Vector(const Vector &arg){ 
        //     capacity_ = arg.getCapacity();
        //     data_ = allocator_.allocate(capacity_);
        //     for(int i = 0; i < arg.getCapacity(); i++){ //should probably be end_;
        //         data_[i] = arg[i];
        //     }
        //     end_capacity_ = data_ + capacity_;
        //     end_ = data_ + arg.getSize();
        // }
        
        size_type capacity(){
            return (end_capacity_ - data_);
        }
        
        size_type size(){
            return (end_ - data_);
        }
        
        // Vector &operator=(const Vector &arg){//check if this equals arg
        //     //if(arg != this){
        //         reallocate(capacity_, arg.getCapacity());
        //         for(int i = 0; i < arg.getSize(); i++){ 
        //             data_[i] = arg[i];
        //         }
        //         end_ = data_ + arg.getSize();
        //     //}
        // }
        
        T &operator[](int index){
                if(index >= capacity() || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    if(index > size())
                        end_ = data_ + index + 1;
                        
                    return data_[index];
                }
            }
        
        const T &operator[](int index) const{ 
                if(index >= capacity() || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    return data_[index];
                }
            }
        
        void push_back(T input){
            if(end_ >= end_capacity_){
                ReallocateIfFullCapacity();
            }
            else{
                *end_ = input;
                 ++end_;
            }
        }
        // DELETE LATOR
        void printContents(){
            auto it = data_;
            while(it < end_){
                std::cout << *it << "\n";
                ++it; 
            }
            std::cout << "capacity is:" << capacity() << "\n";
            std::cout << "size is:" << size() << "\n";
        }
};

int main()
{
    Vector<int> x(50);
     x.push_back(23);
     
    for(int i = 1; i < 50; i++)
    x[i] = i;
    x.printContents();
//      Vector<char> y(2);
//     y.push_back(51);
//     y.push_back(11);
//     y.push_back(21);
//     y.push_back(31);
//     y.push_back(41);
//     y.push_back(99);
   

//     y = x;
//     std::cout << y[0] << endl;
//     std::cout << y[1] << endl;
//   /* std::cout << y[2] << endl;
//     std::cout << y[3] << endl;
//     std::cout << y[4] << endl;
//     std::cout << y[5] << endl;
//     std::cout << y[6] << endl;
//     std::cout << y[8] << endl;*/
    
    return 0;
}

