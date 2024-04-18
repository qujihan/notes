#include "utils/util.h"
class Airplane {
public:
    unsigned long getMiles() { return rep.miles; }
    char getType() { return rep.type; }
    void set(unsigned long m, char t) {
        rep.miles = m;
        rep.type = t;
    }

    static void* operator new(size_t size);
    static void operator delete(void* ptr);

    ~Airplane() { std::cout << "Airplane destructor called\n" << std::endl; }

private:
    struct AirplaneRep {
        unsigned long miles{10};
        char type{'A'};
    };

    union {
        AirplaneRep rep{};
        Airplane* next;
    };

    static const int BLOCK_SIZE;
    static Airplane* headOfFreeList;
};

const int Airplane::BLOCK_SIZE = 512;
Airplane* Airplane::headOfFreeList{nullptr};

void* Airplane::operator new(size_t size) {
    Airplane* p = headOfFreeList;
    if (p) {
        headOfFreeList = p->next;
    } else {
        // ::operator 就是全局的new, 相当于 malloc
        Airplane* newBlock = static_cast<Airplane*>(::operator new(BLOCK_SIZE* size));
        for (int i = 1; i < BLOCK_SIZE; ++i) {
            newBlock[i].next = &newBlock[i + 1];
        }
        newBlock[BLOCK_SIZE - 1].next = nullptr;
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}

void Airplane::operator delete(void* ptr) {
    if (!ptr) {
        return;
    }
    Airplane* del = static_cast<Airplane*>(ptr);
    del->next = headOfFreeList;
    headOfFreeList = del;
}

void overload_new_delete() {
    util::subtitle("重写new和delete的意义");
    Airplane* p1 = new Airplane();
    std::cout << p1 << std::endl;

    Airplane* p2 = new Airplane();
    std::cout << p2 << std::endl;

    Airplane* p3 = new Airplane();
    std::cout << p3 << std::endl;

    delete p1;
    delete p2;
    delete p3;
}

int main() {
    overload_new_delete();
}
