#include <iostream>
#include <memory>
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

class Dog {
public:
    Dog(const std::string& name) : name_(name) {}

    void getToy(std::shared_ptr<Toy> toy) {
        if (toy_ == toy) {
            std::cout << "I already have this toy." << std::endl;
        }
        else if (toy.use_count() > 1) {
            std::cout << "Another dog is playing with this toy." << std::endl;
        }
        else {
            toy_ = toy;
            std::cout << name_ << " picked up the toy " << toy->getName() << "." << std::endl;
        }
    }

    void dropToy() {
        if (toy_) {
            std::cout << name_ << " dropped the toy " << toy_->getName() << "." << std::endl;
            toy_.reset();
        }
        else {
            std::cout << "Nothing to drop." << std::endl;
        }
    }

private:
    std::string name_;
    std::shared_ptr<Toy> toy_;
};

int main() {
    auto toy = std::make_shared<Toy>("Bone");
    Dog dog1("Rex");
    Dog dog2("Max");

    dog1.getToy(toy);
    dog2.getToy(toy);

    dog1.dropToy();
    dog2.getToy(toy);

    return 0;
}
