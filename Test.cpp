template <class T, class A = std::allocator<T> >
class vector {
public:
    // public member functions
    Vector(): size_(0), capacity_(10){	//10 is the default number of elements

 		data_ = allocator_.allocate(10);
    }
    Vector(int capacity): size_(0), capacity_(capacity){
    	data_ = allocator_.allocate(capacity * sizeof(T));
    }

    ~Vector(){}

    T &operator[](int index){
            if(index >= size_ || index < 0) {
                throw std::out_of_range("invalid pos");
            }
            else{
            		//questionable
                return data_[index];
            }
        }

    const T &operator[](int index) const{ //problem, this may or may not increment size;
            if(index > end_ - data_) {
                throw std::out_of_range("invalid pos");
            }
            else{
                return T[index];
            }
        }
    void push_back(int input){
    	size_++;
        if(size_ >= capacity_){
        	capacity_ *= 2;		//2 is the growth factor
            T* tempPtr = data_;
            data_ = A.allocate(capacity_);	
            for(int i = 0; i < size_ - 1; i++){
                data_[i] = tempPtr[i];
            }
            data_[size - 1] = input;
            A.deallocate(tempPtr, capacity_);
        }
        else{
            dataPtr[size_ - 1] = inputData;
        }
    }

    Vector &operator=(const Vector &arg){}
private:
    T* data_;         // points to first element
    T* end_capacity_; // points to one past internal storage
    T* end_;          // points to one past last element
    int size_;
    int capacity_;
    A  allocator_;
};