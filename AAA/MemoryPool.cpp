#include <bits/stdc++.h>

class MemoryPool
{
public:
    MemoryPool(int blockSize, int slotSize)
        : blockSize_(blockSize)
        , slotSize_(slotSize)
        , firstBlock_(nullptr)
        , freeList_(nullptr)
        , curSlot_(nullptr)
        , lastSlot_(nullptr)
    {}

    ~MemoryPool()
    {
        // 写法一, 我临时想出来的新写法, 逻辑一样, 这样写也挺好, 随缘了
        // while(firstBlock_)
        // {
        //     Slot* curBlock = firstBlock_;
        //     firstBlock_ = firstBlock_->next;
        //     operator delete(reinterpret_cast<void*>(curBlock));
        // }
        
        // 写法二, 保持和之前一样的写法
        Slot* curBlock = firstBlock_;
        size_t cnt = 0;
        while (curBlock)
        {
            Slot* nextBlock = curBlock->next;
            operator delete(curBlock);
            curBlock = nextBlock;
            cnt ++;
        }
        std::cout << "clear " << cnt << " blocks" << std::endl;
    }

    void* allocate()
    {
        if (freeList_)
        {
            Slot* cur = freeList_;
            freeList_ = freeList_->next;
            return cur;
        }
        if (curSlot_ >= lastSlot_)
        {
            allocateNewBlock();
        }
        Slot* cur = curSlot_;
        curSlot_ = reinterpret_cast<Slot*>(reinterpret_cast<char*>(curSlot_) + slotSize_); // reinterpret_cast, 我一开始用成了static_cast
        return cur;
    }
    void deallocate(void* ptr)
    {
        if (ptr == nullptr) return ; // 特判一下
        Slot* cur = reinterpret_cast<Slot*>(ptr);
        cur->next = freeList_;
        freeList_ = cur;
    }
    void allocateNewBlock()
    {
        std::cout << "allocateNewBlock()" << std::endl;
        void* newBlock = operator new(blockSize_);
        Slot* curBlock = reinterpret_cast<Slot*>(newBlock);
        curBlock->next = firstBlock_;
        firstBlock_ = curBlock;
        // 然后重新确定curSlot_和lastSlot_的位置.
        size_t body = reinterpret_cast<size_t>(newBlock) + 8;
        size_t remain = body % slotSize_; // 对齐到slotSize_;
        size_t paddingSize = remain == 0 ? 0 : slotSize_ - remain;
        curSlot_ = reinterpret_cast<Slot*>(body + paddingSize);
        lastSlot_ = reinterpret_cast<Slot*>(reinterpret_cast<size_t>(newBlock) + blockSize_ - slotSize_ + 1);
        // 我直接统一成用size_t来计算padding了, 涉及%运算, 如果单纯是加减我会用char*.
    }
private:
    struct Slot {
        Slot* next;
    };
    size_t blockSize_;
    size_t slotSize_;
    Slot* firstBlock_; // 也可以在这里初始化为nullptr, C++11就支持, 与C++17支持inline变量不是一个概念.
    Slot* freeList_;
    Slot* curSlot_;
    Slot* lastSlot_;
};

struct Person {
    std::string name_; // 32字节哦!!!
    int age_;
    Person(std::string name, int age) : name_(name), age_(age)
    {
        // std::cout << "Person Created" << std::endl;
    } 
    ~Person()
    {
        // 不用写, 编译器会自动处理std::string的清理?
    }
};
int main()
{
    MemoryPool* pool = new MemoryPool(4096, sizeof(Person));
    // 简单示例
    // Person* p1 = new (pool.allocate()) Person("zq", 20);
    // p1->~Person();
    // pool.deallocate(p1);

    // 示例二, 用vector
    std::vector<Person*> people;
    for (int i = 0; i < 1000; i ++) {
        Person* p = new (pool->allocate()) Person("zq", 20);
        people.push_back(p);
    }

    for (int i = 0; i < 1000; i ++) {
        Person* p = people[i];
        p->~Person();
        pool->deallocate(p);
    }
    pool->~MemoryPool(); // 手动调用
    return 0;
}