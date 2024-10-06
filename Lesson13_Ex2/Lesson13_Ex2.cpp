#include <iostream>
#include <utility>
#include <string>

class Toy {
public:
    Toy(const std::string& name) : name_(name) {}

    std::string getName() const {
        return name_;
    }

    ~Toy() {
        std::cout << "Toy " << name_ << " was dropped." << std::endl;
    }

private:
    std::string name_;
};

class shared_ptr_toy {
public:
    shared_ptr_toy() : ptr_(nullptr), count_(nullptr) {}

    explicit shared_ptr_toy(Toy* toy) : ptr_(toy), count_(new size_t(1)) {}

    shared_ptr_toy(const shared_ptr_toy& other) : ptr_(other.ptr_), count_(other.count_) {
        if (count_) {
            ++(*count_);
        }
    }

    shared_ptr_toy(shared_ptr_toy&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)),
        count_(std::exchange(other.count_, nullptr)) {}

    shared_ptr_toy& operator=(const shared_ptr_toy& other) {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            count_ = other.count_;
            if (count_) {
                ++(*count_);
            }
        }
        return *this;
    }

    shared_ptr_toy& operator=(shared_ptr_toy&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = std::exchange(other.ptr_, nullptr);
            count_ = std::exchange(other.count_, nullptr);
        }
        return *this;
    }

    ~shared_ptr_toy() {
        release();
    }

    Toy& operator*() const {
        return *ptr_;
    }

    Toy* operator->() const {
        return ptr_;
    }

    size_t use_count() const {
        return count_ ? *count_ : 0;
    }

private:
    Toy* ptr_;
    size_t* count_;

    void release() {
        if (count_ && --(*count_) == 0) {
            delete ptr_;
            delete count_;
        }
    }
};

shared_ptr_toy make_shared_toy(const std::string& name) {
    return shared_ptr_toy(new Toy(name));
}

shared_ptr_toy make_shared_toy(const shared_ptr_toy& other) {
    return shared_ptr_toy(new Toy(*other));
}

int main() {
    auto toy1 = make_shared_toy("Ball");
    std::cout << "Toy created: " << toy1->getName() << ", Use count: " << toy1.use_count() << std::endl;

    {
        auto toy2 = toy1;
        std::cout << "Toy copied: " << toy2->getName() << ", Use count: " << toy1.use_count() << std::endl;
    }

    std::cout << "After toy2 is out of scope, Use count: " << toy1.use_count() << std::endl;

    return 0;
}
