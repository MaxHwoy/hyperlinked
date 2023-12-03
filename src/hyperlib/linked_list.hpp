#pragma once

#include <cstdint>
#include <algorithm>

namespace hyper
{
    template <typename T> class linked_list;

    template <typename T> class linked_node
    {
    public:
        inline linked_node() : next_(nullptr), prev_(nullptr)
        {
        }

        inline auto next() -> T*
        {
            return static_cast<T*>(this->next_);
        }

        inline auto next() const -> const T*
        {
            return static_cast<const T*>(this->next_);
        }

        inline auto prev() -> T*
        {
            return static_cast<T*>(this->prev_);
        }

        inline auto prev() const -> const T*
        {
            return static_cast<const T*>(this->prev_);
        }

        inline auto next_node() -> linked_node*&
        {
            return this->next_;
        }

        inline auto prev_node() -> linked_node*&
        {
            return this->prev_;
        }

        inline void disconnect()
        {
            if (this->prev_ != nullptr)
            {
                this->prev_->next_ = this->next_;
            }

            if (this->next_ != nullptr)
            {
                this->next_->prev_ = this->prev_;
            }
        }

        inline auto next_ref() const -> linked_node<T>* const*
        {
            return &this->next_;
        }

        inline auto prev_ref() const -> linked_node<T>* const*
        {
            return &this->prev_;
        }

    private:
        linked_node* next_;
        linked_node* prev_;

    private:
        friend class linked_list<T>;
    };

    template <typename T> class linked_list
    {
    public:
        inline linked_list()
        {
            this->head_.next_ = &this->head_;
            this->head_.prev_ = &this->head_;
        }

        inline void null()
        {
            this->head_.next_ = nullptr;
            this->head_.prev_ = nullptr;
        }

        inline auto head() -> T*
        {
            return static_cast<T*>(&this->head_);
        }

        inline auto head() const -> const T*
        {
            return static_cast<const T*>(&this->head_);
        }

        inline auto begin() -> T*
        {
            return this->head_.next();
        }

        inline auto begin() const -> const T*
        {
            return this->head_.next();
        }

        inline auto tail() -> T*
        {
            return this->head_.prev();
        }

        inline auto tail() const -> const T*
        {
            return this->head_.prev();
        }

        inline auto end() -> T*
        {
            return static_cast<T*>(&this->head_);
        }

        inline auto end() const -> const T*
        {
            return static_cast<const T*>(&this->head_);
        }

        inline bool empty() const
        {
            return this->head_.next_ == &this->head_;
        }

        inline void add(T* val)
        {
            linked_node<T>* node = static_cast<linked_node<T>*>(val);

            node->prev_ = this->head_.prev_;

            this->head_.prev_->next_ = node;

            this->head_.prev_ = node;

            node->next_ = &this->head_;
        }

        inline void remove(T* val)
        {
            linked_node<T>* node = static_cast<linked_node<T>*>(val);

            node->prev_->next_ = node->next_;
            node->next_->prev_ = node->prev_;
        }

        inline auto remove_first() -> T*
        {
            linked_node<T>* curr = this->head_.next_;

            curr->prev_->next_ = curr->next_;
            curr->next_->prev_ = curr->prev_;

            return static_cast<T*>(curr);
        }

        inline auto remove_last() -> T*
        {
            linked_node<T>* curr = this->head_.prev_;

            curr->prev_->next_ = curr->next_;
            curr->next_->prev_ = curr->prev_;

            return static_cast<T*>(curr);
        }

        inline void add_after(T* val, T* node)
        {
            linked_node<T>* val_node = static_cast<linked_node<T>*>(val);
            linked_node<T>* node_node = static_cast<linked_node<T>*>(node);

            val_node->prev_ = node_node;

            val_node->next_ = node_node->next_;

            node_node->next_->prev_ = val_node;

            node_node->next_ = val_node;
        }

        inline void add_before(T* val, T* node)
        {
            linked_node<T>* val_node = static_cast<linked_node<T>*>(val);
            linked_node<T>* node_node = static_cast<linked_node<T>*>(node);

            val_node->next_ = node_node;

            val_node->prev_ = node_node->prev_;

            node_node->prev_->next_ = val_node;

            node_node->prev_ = val_node;
        }

        inline void clear()
        {
            this->head_.prev_ = &this->head_;
            this->head_.next_ = &this->head_;
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
            for (T* i = this->begin(); i != this->end(); i = i->next())
            {
                action(i);
            }
        }

        template <typename F> inline void foreach_reverse(F action)
        {
            for (const T* i = this->tail(); i != this->end(); i = i->prev())
            {
                action(i);
            }
        }

        template <typename P> inline void sort(P predicate)
        {
            if (this->head_.next_ != &this->head_)
            {
                linked_node<T>* root = this->head_.next_;

                linked_node<T>* curr = root->next_;

                root->prev_ = &this->head_;
                root->next_ = &this->head_;

                this->head_.prev_ = root;
                this->head_.next_ = root;

                while (curr != &this->head_)
                {
                    linked_node<T>* next = curr->next_;

                    linked_node<T>* temp = root;

                    while (temp != &this->head_)
                    {
                        if (predicate(static_cast<T*>(temp), static_cast<T*>(curr)) >= 0)
                        {
                            break;
                        }

                        temp = temp->next_;
                    }

                    curr->next_ = temp;

                    curr->prev_ = temp->prev_;

                    temp->prev_->next_ = curr;

                    temp->prev_ = curr;

                    curr = next;

                    root = this->head_.next_;
                }
            }
        }

    private:
        linked_node<T> head_;
    };
}
