#include <iostream>
#include <vector>
#include <mutex>

#include <hyperlib/bench.hpp>

struct node
{
    inline node(int value = 0, node* next = nullptr)
    {
        this->value = value;
        this->next = next;
    }

    int value;
    node* next;
};

class stack
{
public:
    inline stack() : head_(nullptr)
    {
    }

    inline ~stack()
    {
        node* pointer = this->head_;

        if (pointer)
        {
            node* next = nullptr;

            while (pointer)
            {
                next = pointer->next;

                delete pointer;

                pointer = next;
            }
        }
    }

    int pop()
    {
        int result = 0;

        node* head = this->head_;

        if (head)
        {
            result = head->value;

            node* next = head->next;

            delete head;

            this->head_ = next;
        }

        return result;
    }

    void push(int value)
    {
        node* ptr = new node(value);

        ptr->next = this->head_;

        this->head_ = ptr;
    }

private:
    node* head_;
};

class stack_mutex
{
public:
    inline stack_mutex() : head_(nullptr), mtx_()
    {
    }

    inline ~stack_mutex()
    {
        node* pointer = this->head_;

        if (pointer)
        {
            node* next = nullptr;

            while (pointer)
            {
                next = pointer->next;

                delete pointer;

                pointer = next;
            }
        }
    }

    int pop()
    {
        int result = 0;

        const auto _ = std::lock_guard(this->mtx_);

        node* head = this->head_;

        if (head)
        {
            result = head->value;

            node* next = head->next;

            delete head;

            this->head_ = next;
        }

        return result;
    }

    void push(int value)
    {
        node* ptr = new node(value);

        const auto _ = std::lock_guard(this->mtx_);

        ptr->next = this->head_;

        this->head_ = ptr;
    }

private:
    std::mutex mtx_;
    node* head_;
};

constexpr std::uint32_t loop_count = 10000u;

void test_stack()
{
    BENCHMARK();

    stack s{};

    for (std::uint32_t i = 0u; i < loop_count; ++i)
    {
        s.push(i);
    }

    for (std::uint32_t i = 0u; i < loop_count; ++i)
    {
        s.pop();
    }
}

void test_stack_mutex()
{
    BENCHMARK();

    stack_mutex s{};

    for (std::uint32_t i = 0u; i < loop_count; ++i)
    {
        s.push(i);
    }

    for (std::uint32_t i = 0u; i < loop_count; ++i)
    {
        s.pop();
    }
}

int main()
{
    ::test_stack();
    ::test_stack_mutex();

    hyper::bench::print();

    return 0;
}
