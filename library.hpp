#include <stdio.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <allocator>

template <class T, class Allocator = std::allocator<T> >
class Vector_base{
    public:
        using allocator_type  = Allocator;
        using value_type      = typename Allocator::value_type;
        using size_type       = typename Allocator::size_type;
        using pointer         = typename Allocator::pointer;
        using reference       = typename Allocator::reference;
        using const_reference = typename Allocator::const_reference;
        
        //TODO implement const iterator
        class Iterator friend Vector{
        public:
            using allocator_type  = Allocator;
            using value_type      = typename Allocator::value_type;
            using size_type       = typename Allocator::size_type;
            using pointer         = typename Allocator::pointer;
            using reference       = typename Allocator::reference;
            using const_reference = typename Allocator::const_reference;
        
        private:
            pointer iteratorP_ = nullptr;
        
        public:
            Iterator() = default;
            
            Iterator(pointer inputIterator){
                iteratorP_ = inputIterator;
            }
            
            Iterator & operator++(){ 
                iteratorP_++;
                return *this;
            }
            
            Iterator operator++(int){ 
                auto temp = *this;
                iteratorP_++;
                return temp;
            }
            
            Iterator & operator--(){ 
                iteratorP_--;
                return *this;
            }
            
            Iterator operator--(int){ 
                auto temp = *this;
                iteratorP_--;
                return temp;
            }

            value_type& operator[](int index) const{ 
                return *(iteratorP_ + index);
            }
            
            value_type& operator*() const{ 
                return *iteratorP_; 
            }
            
            // T& operator->() const { 
            //     return *iteratorP_; 
            // }
            
            Iterator & operator+=(int offset){ 
                start_ += offset; 
                return *this; 
            }

            Iterator & operator-=(int offset){ 
                start_ -= offset; 
                return *this; 
            }

            // bool operator<(RingIter const&sibling) const { return off < sibling.off;}
            // bool operator<=(RingIter const&sibling) const { return off <= sibling.off; }
            // bool operator>(RingIter const&sibling) const { return off > sibling.off; }
            // bool operator>=(RingIter const&sibling) const { return off >= sibling.off; }
            
        };
        
        using reverse_iterator = std::reverse_iterator<Iterator>;
        
    private:
        using allocatorTraits = std::allocator_traits<allocator_type>;

        pointer start_        = nullptr;    // points to first element
        pointer end_capacity_ = nullptr;    // points to one past the allocated storage
        pointer end_          = nullptr;    // points to one past last element
        size_type capacity_;
        size_type size_;
        Allocator vectorAllocator_;

        /*
        * Allocates space without initializing 
        */
        pointer allocate(size_type capacity){
            return capacity != 0 ? allocatorTraits::allocate(vectorAllocator_, capacity) : nullptr;
        }
        
        void deallocate(pointer data, size_type capacity){
            if(data)
                allocatorTraits::deallocate(vectorAllocator_, data, capacity);
        }
        
        /*
        * Changes the amount of allocated memory
        */
        void reallocate(size_type oldCapacity, size_type newCapacity){ 
            std::cout << "REALLOCATING from " << oldCapacity << " TO " << newCapacity << "\n";
            this->printContents();
            std::cout << "----------- \n";
            // Check if newCapacity is greater than largest possible allocation size
            if (newCapacity >  vectorAllocator_.max_size()) {
                throw std::length_error("vector::reallocate() newCapacity > max_size");
            }
            pointer newDataBegin = allocate(newCapacity);
            pointer newDataEnd = newDataBegin;
            
            auto temp = start_;
            while(temp != end_){
                allocatorTraits::construct(vectorAllocator_, newDataEnd, std::move(*temp));
                allocatorTraits::destroy(vectorAllocator_, temp);
                ++temp;
                ++newDataEnd;
            }
            
            deallocate(start_, capacity());
            
            start_ = newDataBegin;
            end_ = newDataEnd;
            end_capacity_ = start_ + newCapacity;
        }
        
        // void ReallocateIfFullCapacity(){
        //     const auto GROWTH_FACTOR = 2;
        //     size_type newCap = capacity() * GROWTH_FACTOR; //what if newCap zero?
        //     reallocate(capacity(), newCap);
        // }

        void create_storage(size_type n){
            start_ = allocate(n);
            end_ = start;
            end_capacity_ = start_ + n;
        }


        /*
        * Changes the number of initialized objects????/
        */
        // void resize(size_type newSize){
        //     if(size() >= newSize)
        //         return;
        // }
        
    public:

        /*
        * Constructors
        */
        Vector(){  
            const auto default_size = 10;
            create_storage(default_size);
         }

        Vector(size_type capacity){
            if(capacity < 0){
                throw std::length_error("invalid capacity");
            }

            create_storage(capacity);
        }

        Vector(const allocator_type a) {
            allocator_ = a;
        }

        Vector(size_type s, const allocator_type a) {
            allocator_ = a;
            create_storage(s);
        }

        Vector(size_type s, const value_type& v){
            // TODO check for right data type
            create_storage(s);
            for(auto i = start_; i < end_capacity_; i++){
                *i = v;
            }
        }
        
        // Vector(const Vector &arg){ 
        //     capacity_ = arg.getCapacity();
        //     start_ = allocator_.allocate(capacity_);
        //     for(int i = 0; i < arg.getCapacity(); i++){ //should probably be end_;
        //         start_[i] = arg[i];
        //     }
        //     end_capacity_ = start_ + capacity_;
        //     end_ = start_ + arg.getSize();
        // }

        ~Vector(){
            deallocate(start_, capacity());
        }
        
        size_type capacity() const{
            return (end_capacity_ - start_);
        }
        
        size_type size() const{
            return (end_ - start_);
        }
        
        // Vector &operator=(const Vector &arg){//check if this equals arg
        //     //if(arg != this){
        //         reallocate(capacity_, arg.getCapacity());
        //         for(int i = 0; i < arg.getSize(); i++){ 
        //             start_[i] = arg[i];
        //         }
        //         end_ = start_ + arg.getSize();
        //     //}
        // }
        
        reference &operator[](size_type index){
                if(index >= capacity() || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    if(index > size())
                        end_ = start_ + index + 1;
                        
                    return start_[index];
                }
            }
        
        const reference &operator[](size_type index) const{ 
                if(index >= capacity() || index < 0) {
                    throw std::out_of_range("invalid pos");
                }
                else{
                    return start_[index];
                }
            }
            
        reference at(size_type index){
            if(index >= capacity() || index < 0) {
                throw std::out_of_range("invalid pos");
            }
            else{
                return start_[index];
            }
        }
        
        void push_back(value_type input){
            if(end_ >= end_capacity_){
                ReallocateIfFullCapacity();
                *end_ = input;
                ++end_;
            }
            else{
                *end_ = input;
                ++end_;
            }
        }

        void insert(){

        }

        void erase(){

        }

        // void swap(vector ){}

        void clear(){
            for(int i = 0; i < size(); i++){
                end_--;
                allocatorTraits::destroy(vectorAllocator_, end_);
            }   
        }
        
        bool empty() const{
            return (start_ == end_);
        }
        
        // DELETE LATOR
        void printContents(){
            auto it = start_;
            while(it < end_){
                std::cout << *it << "\n";
                ++it; 
            }
            std::cout << "capacity is:" << capacity() << "\n";
            std::cout << "size is:" << size() << "\n";
        }
    
        pointer begin(){
            return start_;
        }
        
        pointer end(){
            return end_;
        }
        
        pointer rbegin(){
            return reverse_iterator(begin());
        }
        
        pointer rend(){
            return reverse_iterator(end());
        }
    
    
};

int main()
{
    Vector<char> x(1);
    
    for(int i = 0; i < 26; i++)
        x.push_back(i + 'a');
        
    //x.printContents();
    for(auto i = x.rend(); i != x.rbegin(); i++){
        std::cout<< *i;
    }
    return 0;
}

