#pragma once

#include <cstdint>
#include <algorithm>

namespace hyper
{
    template <typename T> class linked_node
    {
    public:
        inline linked_node() : prev_(nullptr), next_(nullptr)
        {
        }

        inline auto prev() -> T*&
        {
            return *&this->prev_;
        }

        inline auto prev() const -> const T*
        {
            return this->prev_;
        }

        inline auto next() -> T*&
        {
            return *&this->next_;
        }

        inline auto next() const -> const T*
        {
            return this->next_;
        }

        inline auto prev_node() -> linked_node*
        {
            return reinterpret_cast<linked_node*>(this->prev_);
        }

        inline auto next_node() -> linked_node*
        {
            return reinterpret_cast<linked_node*>(this->next_);
        }

        inline void disconnect()
        {
            if (this->prev_ != nullptr)
            {
                reinterpret_cast<linked_node<T>*>(this->prev_)->next_ = this->next_;
            }

            if (this->next_ != nullptr)
            {
                reinterpret_cast<linked_node<T>*>(this->next_)->prev_ = this->prev_;
            }
        }

    private:
        T* prev_;
        T* next_;
    };

    template <typename T> class linked_list
    {
    public:
        inline linked_list()
        {
            this->head_.prev() = this->end();
            this->head_.next() = this->end();
        }

        inline auto head() -> T*
        {
            return reinterpret_cast<T*>(&this->head_);
        }

        inline auto head() const -> const T*
        {
            return reinterpret_cast<const T*>(&this->head_);
        }

        inline auto begin() -> T*
        {
            return reinterpret_cast<T*>(this->head_.next());
        }

        inline auto begin() const -> const T*
        {
            return reinterpret_cast<const T*>(this->head_.next());
        }

        inline auto tail() -> T*
        {
            return reinterpret_cast<T*>(this->head_.prev());
        }

        inline auto tail() const -> const T*
        {
            return reinterpret_cast<T*>(this->head_.prev());
        }

        inline auto end() -> T*
        {
            return reinterpret_cast<T*>(&this->head_);
        }

        inline auto end() const -> const T*
        {
            return reinterpret_cast<const T*>(&this->head_);
        }

        inline void add(T* val)
        {
            reinterpret_cast<linked_node<T>*>(val)->prev() = this->head_.prev();

            this->head_.prev_node()->next() = val;

            this->head_.prev() = val;

            reinterpret_cast<linked_node<T>*>(val)->next() = this->end();
        }

        inline void remove(T* val)
        {
            T* prev = reinterpret_cast<linked_node<T>*>(val)->prev();
            T* next = reinterpret_cast<linked_node<T>*>(val)->next();

            reinterpret_cast<linked_node<T>*>(prev)->next() = reinterpret_cast<T*>(next);
            reinterpret_cast<linked_node<T>*>(next)->prev() = reinterpret_cast<T*>(prev);
        }

        inline auto remove_first() -> T*
        {
            linked_node<T>* curr = reinterpret_cast<linked_node<T>*>(this->head_.next());

            reinterpret_cast<linked_node<T>*>(curr->prev())->next() = curr->next();
            reinterpret_cast<linked_node<T>*>(curr->next())->prev() = curr->prev();
            
            return reinterpret_cast<T*>(curr);
        }

        inline auto remove_last() -> T*
        {
            linked_node<T>* curr = reinterpret_cast<linked_node<T>*>(this->head_.prev());

            reinterpret_cast<linked_node<T>*>(curr->prev())->next() = curr->next();
            reinterpret_cast<linked_node<T>*>(curr->next())->prev() = curr->prev();

            return reinterpret_cast<T*>(curr);
        }

        inline void add_after(T* val, T* node)
        {
            reinterpret_cast<linked_node<T>*>(val)->prev() = node;

            reinterpret_cast<linked_node<T>*>(val)->next() = reinterpret_cast<linked_node<T>*>(node)->next();

            reinterpret_cast<linked_node<T>*>(node)->next_node()->prev() = val;

            reinterpret_cast<linked_node<T>*>(node)->next() = val;
        }

        inline void add_before(T* val, T* node)
        {
            reinterpret_cast<linked_node<T>*>(val)->next() = node;

            reinterpret_cast<linked_node<T>*>(val)->prev() = reinterpret_cast<linked_node<T>*>(node)->prev();

            reinterpret_cast<linked_node<T>*>(node)->prev_node()->next() = val;

            reinterpret_cast<linked_node<T>*>(node)->prev() = val;
        }

        inline void clear()
        {
            this->head_.prev() = this->end();
            this->head_.next() = this->end();
        }

        inline void move_front(T* val)
        {
            this->remove(val);
            this->add_before(val, this->begin());
        }

        inline void move_end(T* val)
        {
            this->remove(val);
            this->add_after(val, this->tail());
        }

        template <typename F> inline void foreach(F action)
        {
            for (const T* i = this->begin(); i != this->end(); i = reinterpret_cast<const linked_node<T>*>(i)->next())
            {
                action(i);
            }
        }

        template <typename F> inline void foreach_reverse(F action)
        {
            for (const T* i = this->tail(); i != this->end(); i = reinterpret_cast<const linked_node<T>*>(i)->prev())
            {
                action(i);
            }
        }

        template <typename P> inline void sort(P predicate)
        {
            if (this->head_.next() != &this->head_)
            {
                linked_node<T>* root = this->head_.next_node();

                linked_node<T>* curr = root->next_node();

                root->prev() = reinterpret_cast<T*>(&this->head_);
                root->next() = reinterpret_cast<T*>(&this->head_);

                this->head_.prev() = reinterpret_cast<T*>(root);
                this->head_.next() = reinterpret_cast<T*>(root);

                while (curr != &this->head_)
                {
                    linked_node<T>* next = curr->next_node();

                    linked_node<T>* temp = root;

                    while (temp != &this->head_)
                    {
                        if (predicate(reinterpret_cast<T*>(temp), reinterpret_cast<T*>(curr)) >= 0)
                        {
                            break;
                        }

                        temp = temp->next_node();
                    }

                    this->add_before(reinterpret_cast<T*>(curr), reinterpret_cast<T*>(temp));

                    curr = next;

                    root = this->head_.next_node();
                }
            }
        }

    private:
        linked_node<T> head_;
    };
}
