template <class T, class A = std::allocator<T> >
class vector {
public:
    // public member functions
private:
    T* data_;         // points to first element
    T* end_capacity_; // points to one past internal storage
    T* end_;          // points to one past last element
    A  allocator_;
};