#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/bstl.hpp>

namespace hyper
{
    class eastl final
    {
    public:
        template <typename T> struct use_self
        {
            typedef T result_type;

            const T& operator()(const T& x) const
            {
                return x;
            }
        };

        template <typename Pair> struct use_first
        {
            typedef Pair argument_type;
            typedef typename Pair::first_type result_type;

            const result_type& operator()(const Pair& x) const
            {
                return x.first;
            }
        };

        template <typename Pair> struct use_second
        {
            typedef Pair argument_type;
            typedef typename Pair::second_type result_type;

            const result_type& operator()(const Pair& x) const
            {
                return x.second;
            }
        };

        template <std::uint32_t N, std::uint32_t A> struct aligned_buffer
        {
            __declspec(align(A)) std::uint8_t data[N];
        };

        template <typename T, typename Allocator> struct list_base
        {
            linked_list<T> head;
            size_t size;
            Allocator allocator;
        };

        template <typename T, typename Allocator> struct list : public list_base<T, Allocator>
        {
        };

        template <typename T, typename Allocator> struct vector_base
        {
            T* begin;
            T* end;
            T* capacity;
            Allocator allocator;
        };

        template <typename T, typename Allocator> struct vector : public vector_base<T, Allocator>
        {
        };

        enum class rbtree_color : std::uint8_t
        {
            red,
            black,
        };

        enum class rbtree_side : std::uint32_t
        {
            left,
            right,
        };

        struct rbtree_node_base
        {
            rbtree_node_base* right;
            rbtree_node_base* left;
            rbtree_node_base* parent;
            rbtree_color color;
        };

        template <typename T> struct rbtree_node : public rbtree_node_base
        {
            T value;
#if defined(_MSC_VER)
            rbtree_node(const rbtree_node&) = delete;
#endif
        };

        template <typename T, typename Pointer, typename Reference> struct rbtree_iterator
        {
        public:
            rbtree_iterator();

            explicit rbtree_iterator(const rbtree_node_base* node);

            rbtree_iterator(const rbtree_iterator<T, T*, T&>& x);

            rbtree_iterator& operator=(const rbtree_iterator<T, T*, T&>& x);

            Reference operator*() const;

            Pointer operator->() const;

            rbtree_iterator& operator++();

            rbtree_iterator operator++(std::int32_t);

            rbtree_iterator& operator--();

            rbtree_iterator operator--(std::int32_t);

        private:
            template<class U, class PtrA, class RefA, class PtrB, class RefB> friend bool operator==(const rbtree_iterator<U, PtrA, RefA>&, const rbtree_iterator<U, PtrB, RefB>&);

            template<class U, class PtrA, class RefA, class PtrB, class RefB> friend bool operator!=(const rbtree_iterator<U, PtrA, RefA>&, const rbtree_iterator<U, PtrB, RefB>&);

            template<class U, class PtrA, class RefA> friend bool operator!=(const rbtree_iterator<U, PtrA, RefA>&, const rbtree_iterator<U, PtrA, RefA>&);

            template <class Key, class Value, class Comparer, class Allocator, class ExtractKey, bool MutableIterators, bool UniqueKeys> friend class rbtree;

            friend rbtree_iterator<T, T*, T&>;

            friend rbtree_iterator<T, const T*, const T&>;

        private:
            rbtree_node_base* node_;
        };

        template <typename Key, typename Value, typename Comparer, typename ExtractKey, bool UniqueKeys, typename RBTree> struct rb_base
        {
        protected:
            Comparer comparer_;
        };

        template <typename Key, typename Pair, typename Comparer, typename RBTree> struct rb_base<Key, Pair, Comparer, eastl::use_first<Pair>, true, RBTree>
        {
        protected:
            Comparer comparer_;
        };

        template <typename Key, typename Value, typename Comparer, typename Allocator, typename ExtractKey, bool MutableIterators, bool UniqueKeys> class rbtree : public rb_base<Key, Value, Comparer, ExtractKey, UniqueKeys, rbtree<Key, Value, Comparer, Allocator, ExtractKey, MutableIterators, UniqueKeys>>
        {
        public:
            typedef typename std::conditional_t<MutableIterators,
                rbtree_iterator<Value, Value*, Value&>,
                rbtree_iterator<Value, const Value*, const Value&>>                                 iterator;

            typedef rbtree_iterator<Value, const Value*, const Value&>                              const_iterator;

            typedef typename std::conditional_t<UniqueKeys, std::pair<iterator, bool>, iterator>    insert_return_type;

            typedef std::bool_constant<UniqueKeys>                                                  has_unique_keys_type;

        public:
            rbtree();

            rbtree(const Allocator& allocator);

            rbtree(const Comparer& comparer, const Allocator& allocator);
            
            rbtree(const rbtree& other);
            
            rbtree(rbtree&& other);
            
            rbtree(rbtree&& other, const Allocator& allocator);

            auto operator=(const rbtree& other) -> rbtree&;

            auto operator=(rbtree&& other) -> rbtree&;

            auto operator=(const std::initializer_list<Value>& ilist) -> rbtree&;

            ~rbtree();

            inline auto get_allocator() -> Allocator&
            {
                return this->allocator_;
            }

            inline auto get_allocator() const -> const Allocator&
            {
                return this->allocator_;
            }

            inline void set_allocator(const Allocator& allocator)
            {
                this->allocator_ = allocator;
            }

            inline auto get_comparer() -> Comparer&
            {
                return this->comparer_;
            }

            inline auto get_comparer() const -> const Comparer&
            {
                return this->comparer_;
            }

            inline auto begin() -> iterator
            {
                return iterator(this->anchor_.left);
            }

            inline auto begin() const -> const_iterator
            {
                return const_iterator(this->anchor_.left);
            }

            inline auto end() -> iterator
            {
                return iterator(&this->anchor_);
            }
            
            inline auto end() const -> const_iterator
            {
                return const_iterator(&this->anchor_);
            }
            
            inline bool empty() const
            {
                return this->size_ == 0u;
            }
            
            inline auto size() const -> size_t
            {
                return this->size_;
            }

            void clear();

            auto find(const Key& key) -> iterator;

            auto find(const Key& key) const -> const_iterator;

            auto lower_bound(const Key& key) -> iterator;

            auto lower_bound(const Key& key) const -> const_iterator;

            auto upper_bound(const Key& key) -> iterator;
            
            auto upper_bound(const Key& key) const -> const_iterator;

            template <typename... Args> auto emplace(Args&&... args) -> insert_return_type;

            auto insert(const Value& value) -> insert_return_type;

            void insert(const std::initializer_list<Value>& ilist);

            auto erase(const Key& key) -> iterator;

            auto erase(const_iterator position) -> iterator;

        protected:
            auto allocate_node() -> rbtree_node<Value>*;

            void free_node(rbtree_node<Value>* node);

            auto create_node_from_key(const Key& key) -> rbtree_node<Value>*;

            auto create_node(const Value& value) -> rbtree_node<Value>*;

            auto create_node(Value&& value) -> rbtree_node<Value>*;
            
            auto create_node(const rbtree_node<Value>* node_src, rbtree_node_base* node_parent) -> rbtree_node<Value>*;
            
            template <typename... Args> auto create_node(Args&&... args) -> rbtree_node<Value>*;

            void reset_lose_memory();

            auto copy_subtree(const rbtree_node<Value>* node_src, rbtree_node_base* node_dst) -> rbtree_node_base*;

            void nuke_subtree(rbtree_node_base* node);

            void swap(rbtree& other);

            auto insert_value(std::true_type, Value&& value) -> std::pair<iterator, bool>;

            auto insert_value(std::false_type, Value&& value) -> iterator;

            auto insert_value_impl(rbtree_node_base* parent, bool force_to_left, const Key& key, rbtree_node<Value>* new_node) -> iterator;

            template <typename... Args> auto insert_value(std::true_type, Args&&... args) -> std::pair<iterator, bool>;

            template <typename... Args> auto insert_value(std::false_type, Args&&... args) -> iterator;

            template <typename... Args> auto insert_value_impl(rbtree_node_base* parent, bool force_to_left, const Key& key, Args&&... args) -> iterator;

            auto insert_key(std::true_type, const Key& key) -> std::pair<iterator, bool>;

            auto insert_key(std::false_type, const Key& key) -> iterator;

            auto insert_key(std::true_type, const_iterator position, const Key& key) -> iterator;

            auto insert_key(std::false_type, const_iterator position, const Key& key) -> iterator;

            auto insert_key_impl(rbtree_node_base* parent, bool force_to_left, const Key& key);

            auto get_key_insertion_position_unique_keys(bool& can_insert, const Key& key) -> rbtree_node_base*;

            auto get_key_insertion_position_non_unique_keys(const Key& key) -> rbtree_node_base*;

            auto get_key_insertion_position_unique_keys_hint(const_iterator position, bool& force_to_left, const Key& key) -> rbtree_node_base*;

            auto get_key_insertion_position_non_unique_keys_hint(const_iterator position, bool& force_to_left, const Key& key) -> rbtree_node_base*;

        private:
            rbtree_node_base anchor_;
            size_t size_;
            Allocator allocator_;
        };

        template <typename Key, typename Comparer = std::less<Key>, typename Allocator = bstl::allocator> class set : public rbtree<Key, Key, Comparer, Allocator, eastl::use_self<Key>, false, true>
        {
        };

        template <typename Key, typename Value, typename Comparer = std::less<Key>, typename Allocator = bstl::allocator> class map : public rbtree<Key, std::pair<const Key, Value>, Comparer, Allocator, eastl::use_first<std::pair<const Key, Value>>, true, true>
        {
        public:
            auto operator[](const Key& key) -> Value&;

            auto at(const Key& key) -> Value*;
        };

        static auto rbtree_increment(const rbtree_node_base* node) -> rbtree_node_base*;
        
        static auto rbtree_decrement(const rbtree_node_base* node) -> rbtree_node_base*;
        
        static auto rbtree_get_min_child(const rbtree_node_base* node) -> rbtree_node_base*;
        
        static auto rbtree_get_max_child(const rbtree_node_base* node) -> rbtree_node_base*;
        
        static auto rbtree_get_black_count(const rbtree_node_base* top, const rbtree_node_base* bottom) -> size_t;
        
        static auto rbtree_rotate_left(rbtree_node_base* node, rbtree_node_base* root) -> rbtree_node_base*;

        static auto rbtree_rotate_right(rbtree_node_base* node, rbtree_node_base* root) -> rbtree_node_base*;

        static void rbtree_insert(rbtree_node_base* node, rbtree_node_base* parent, rbtree_node_base* anchor, rbtree_side side);
        
        static void rbtree_erase(rbtree_node_base* node, rbtree_node_base* anchor);
    };



    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>::rbtree_iterator() : node_(nullptr)
    {
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>::rbtree_iterator(const rbtree_node_base* node) : node_(const_cast<rbtree_node_base*>(node))
    {
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>::rbtree_iterator(const rbtree_iterator<T, T*, T&>& other) : node_(other.node_)
    {
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>& eastl::rbtree_iterator<T, P, R>::operator=(const rbtree_iterator<T, T*, T&>& other)
    {
        this->node_ = other.node_;

        return *this;
    }

    template <typename T, typename Pointer, typename Reference> Reference eastl::rbtree_iterator<T, Pointer, Reference>::operator*() const
    {
        return static_cast<rbtree_node<T>*>(this->node)->value;
    }

    template <typename T, typename Pointer, typename Reference> Pointer eastl::rbtree_iterator<T, Pointer, Reference>::operator->() const
    {
        return &static_cast<rbtree_node<T>*>(this->node_)->value;
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>& eastl::rbtree_iterator<T, P, R>::operator++()
    {
        this->node_ = eastl::rbtree_increment(this->node_);

        return *this;
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R> eastl::rbtree_iterator<T, P, R>::operator++(std::int32_t)
    {
        rbtree_iterator temp(*this);

        this->node_ = eastl::rbtree_increment(this->node_);
        
        return temp;
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>& eastl::rbtree_iterator<T, P, R>::operator--()
    {
        this->node_ = eastl::rbtree_decrement(this->node_);

        return *this;
    }

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R> eastl::rbtree_iterator<T, P, R>::operator--(std::int32_t)
    {
        rbtree_iterator temp(*this);

        this->node_ = eastl::rbtree_decrement(this->node_);
        
        return temp;
    }

    template <typename T, typename PA, typename RA, typename PB, typename RB> inline bool operator==(const eastl::rbtree_iterator<T, PA, RA>& a, const eastl::rbtree_iterator<T, PB, RB>& b)
    {
        return a.node_ == b.node_;
    }

    template <typename T, typename PA, typename RA, typename PB, typename RB> inline bool operator!=(const eastl::rbtree_iterator<T, PA, RA>& a, const eastl::rbtree_iterator<T, PB, RB>& b)
    {
        return a.node_ != b.node_;
    }

    template <typename T, typename P, typename R> inline bool operator!=(const eastl::rbtree_iterator<T, P, R>& a, const eastl::rbtree_iterator<T, P, R>& b)
    {
        return a.node_ != b.node_;
    }



    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, A, E, MI, UK>::rbtree() : anchor_(), size_(0u), allocator_()
    {
        this->reset_lose_memory();
    }

    template <typename K, typename V, typename C, typename Allocator, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, Allocator, E, MI, UK>::rbtree(const Allocator& allocator) : anchor_(), size_(0u), allocator_(allocator)
    {
        this->reset_lose_memory();
    }

    template <typename K, typename V, typename Comparer, typename Allocator, typename E, bool MI, bool UK> eastl::rbtree<K, V, Comparer, Allocator, E, MI, UK>::rbtree(const Comparer& comparer, const Allocator& allocator) : anchor_(), size_(0u), allocator_(allocator)
    {
        this->comparer_ = comparer;
        
        this->reset_lose_memory();
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, A, E, MI, UK>::rbtree(const rbtree& other) : anchor_(), size_(0u), allocator_(other.allocator_)
    {
        this->comparer_ = other.comparer_;

        this->reset_lose_memory();

        if (other.anchor_.parent)
        {
            this->anchor_.parent = this->copy_subtree(reinterpret_cast<const rbtree_node<V>*>(other.anchor_.parent), &this->anchor_);
            this->anchor_.right = eastl::rbtree_get_max_child(this->anchor_.parent);
            this->anchor_.left = eastl::rbtree_get_min_child(this->anchor_.parent);
            this->size_ = other.size_;
        }
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, A, E, MI, UK>::rbtree(rbtree&& other) : anchor_(), size_(0u), allocator_(other.allocator_)
    {
        this->comparer_ = other.comparer_;

        this->reset_lose_memory();
        
        this->swap(other);
    }

    template <typename K, typename V, typename C, typename Allocator, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, Allocator, E, MI, UK>::rbtree(rbtree&& other, const Allocator& allocator) : anchor_(), size_(0u), allocator_(allocator)
    {
        this->comparer_ = other.comparer_;

        this->reset_lose_memory();
        
        this->swap(other);
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<K, Value, C, A, E, MI, UK>::operator=(const rbtree& other) -> rbtree&
    {
        if (this != &other)
        {
            this->clear();

            this->allocator_ = other.allocator_;
            this->comparer_ = other.comparer_;

            if (other.anchor_.parent != nullptr)
            {
                this->anchor_.parent = this->copy_subtree(reinterpret_cast<const rbtree_node<Value>*>(other.anchor_.parent), &this->anchor_);
                this->anchor_.right = eastl::rbtree_get_max_child(this->anchor_.parent);
                this->anchor_.left = eastl::rbtree_get_min_child(this->anchor_.parent);
                this->size_ = other.size_;
            }
        }

        return *this;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<K, Value, C, A, E, MI, UK>::operator=(rbtree&& other) -> rbtree&
    {
        if (this != &other)
        {
            this->clear();     // to consider: are we really required to clear here? 'other' is going away soon and will clear itself in its dtor.
            this->swap(other); // member swap handles the case that x has a different allocator than our allocator by doing a copy.
        }

        return *this;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<K, Value, C, A, E, MI, UK>::operator=(const std::initializer_list<Value>& ilist) -> rbtree&
    {
        // the simplest means of doing this is to clear and insert. there probably isn't a generic
        // solution that's any more efficient without having prior knowledge of the ilist contents.
        
        this->clear();

        for (Value* i = ilist.begin(); i != ilist.end(); ++i)
        {
            this->insert_value(has_unique_keys_type(), std::move(*i));
        }

        return *this;
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> eastl::rbtree<K, V, C, A, E, MI, UK>::~rbtree()
    {
        this->nuke_subtree(this->anchor_.parent);
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> void eastl::rbtree<K, V, C, A, E, MI, UK>::clear()
    {
        this->nuke_subtree(this->anchor_.parent);
        this->reset_lose_memory();
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::find(const Key& key) -> iterator
    {
        ExtractKey extractor;

        rbtree_node_base* current = this->anchor_.parent; // start with the root node.
        rbtree_node_base* range_end = &this->anchor_;     // set it to the container end for now.

        while (current != nullptr) // do a walk down the tree.
        {
            if (!this->comparer_(extractor(reinterpret_cast<rbtree_node<Value>*>(current)->value), key)) // if current is >= key...
            {
                range_end = current;
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        if ((range_end != &this->anchor_) && !this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(range_end)->value)))
        {
            return iterator(range_end);
        }

        return iterator(&this->anchor_);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::find(const Key& key) const -> const_iterator
    {
        return const_iterator(const_cast<rbtree<Key, Value, C, A, ExtractKey, MI, UK>*>(this)->find(key));
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::lower_bound(const Key& key) -> iterator
    {
        ExtractKey extractor;

        rbtree_node_base* current = this->anchor_.parent; // start with the root node.
        rbtree_node_base* range_end = &this->anchor_;     // set it to the container end for now.

        while (current != nullptr) // do a walk down the tree.
        {
            if (!this->comparer_(extractor(reinterpret_cast<rbtree_node<Value>*>(current)->value), key)) // if current is >= key...
            {
                range_end = current;
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        return iterator(range_end);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::lower_bound(const Key& key) const -> const_iterator
    {
        return const_iterator(const_cast<rbtree<Key, Value, C, A, ExtractKey, MI, UK>*>(this)->lower_bound(key));
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::upper_bound(const Key& key) -> iterator
    {
        ExtractKey extractor;

        rbtree_node_base* current = this->anchor_.parent; // start with the root node.
        rbtree_node_base* range_end = &this->anchor_;     // set it to the container end for now.

        while (current != nullptr) // do a walk down the tree.
        {
            if (this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(current)->value))) // If key is < current...
            {
                range_end = current;
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        return iterator(range_end);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::upper_bound(const Key& key) const -> const_iterator
    {
        return const_iterator(const_cast<rbtree<Key, Value, C, A, ExtractKey, MI, UK>*>(this)->upper_bound(key));
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UniqueKeys> template <typename... Args> auto eastl::rbtree<K, Value, C, A, E, MI, UniqueKeys>::emplace(Args&&... args) -> insert_return_type
    {
        return this->insert_value(has_unique_keys_type(), std::forward<Args>(args)...);
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UniqueKeys> auto eastl::rbtree<K, Value, C, A, E, MI, UniqueKeys>::insert(const Value& value) -> insert_return_type
    {
        return this->insert_value(has_unique_keys_type(), value);
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UniqueKeys> void eastl::rbtree<K, Value, C, A, E, MI, UniqueKeys>::insert(const std::initializer_list<Value>& ilist)
    {
        for (Value* i = ilist.begin(); i != ilist.end(); ++i)
        {
            this->insert_value(has_unique_keys_type(), std::move(*i));
        }
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::erase(const Key& key) -> iterator
    {
        iterator it = this->lower_bound(key);

        if (it != this->end())
        {
            return this->erase(const_iterator(it.node));
        }

        return it;
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::erase(const_iterator position) -> iterator
    {
        const iterator ierase(position.node);
        
        --this->size_;
        
        ++position;

        eastl::rbtree_erase(ierase.node, &this->anchor_);

        this->free_node(reinterpret_cast<rbtree_node<Value>*>(ierase.node));
        
        return iterator(position.node);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value(std::true_type, Value&& value) -> std::pair<iterator, bool>
    {
        bool can_insert;

        ExtractKey extractor;
        
        Key key(extractor(value));

        rbtree_node_base* position = this->get_key_insertion_position_unique_keys(can_insert, key);

        if (can_insert)
        {
            const iterator result(this->insert_value_impl(position, false, key, std::move(value)));

            return std::pair<iterator, bool>(result, true);
        }

        return std::pair<iterator, bool>(iterator(position), false);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value(std::false_type, Value&& value) -> iterator
    {
        ExtractKey extractor;

        Key key(extractor(value));

        rbtree_node_base* position = this->get_key_insertion_position_non_unique_keys(key);

        return this->insert_value_impl(position, false, key, std::move(value));
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value_impl(rbtree_node_base* parent, bool force_to_left, const Key& key, rbtree_node<Value>* new_node) -> iterator
    {
        assert(new_node != nullptr);

        rbtree_side side;
        ExtractKey extractor;

        if (force_to_left || (parent == &this->anchor_) || this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(parent)->value)))
        {
            side = rbtree_side::left;
        }
        else
        {
            side = rbtree_side::right;
        }

        eastl::rbtree_insert(new_node, parent, &this->anchor_, side);
        
        ++this->size_;

        return iterator(new_node);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> template <class... Args> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value(std::true_type, Args&&... args) -> std::pair<iterator, bool>
    {
        // this is the pathway for insertion of unique keys (map and set, but not multimap and multiset).
        // note that we return a pair and not an iterator. this is because the C++ standard for map
        // and set is to return a pair and not just an iterator.

        rbtree_node<Value>* new_node = this->create_node(std::forward<Args>(args)...);

        const Key& key = ExtractKey{}(new_node->value);

        bool can_insert;

        rbtree_node_base* position = this->get_key_insertion_position_unique_keys(can_insert, key);

        if (can_insert)
        {
            iterator result(this->insert_value_impl(position, false, key, new_node));

            return std::pair<iterator, bool>(result, true);
        }

        this->free_node(new_node);

        return std::pair<iterator, bool>(iterator(position), false);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> template <class... Args> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value(std::false_type, Args&&... args) -> iterator
    {
        // we have a problem here if sizeof(value_type) is too big for the stack. We may want to consider having a specialization for large value_types.
        // todo: change this so that we call create_node(eastl::forward<Args>(args)...) here and use the value from the resulting node to get the 
        // key, and make insert_value_impl take that node as an argument. That way there is no value created on the stack.

        rbtree_node<Value>* const new_node = this->create_node(std::forward<Args>(args)...);

        const Key& key = ExtractKey{}(new_node->value);

        rbtree_node_base* position = this->get_key_insertion_position_non_unique_keys(key);

        return this->insert_value_impl(position, false, key, new_node);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> template <class... Args> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_value_impl(rbtree_node_base* parent, bool force_to_left, const Key& key, Args&&... args) -> iterator
    {
        rbtree_node<Value>* const new_node = this->create_node(std::forward<Args>(args)...);

        return this->insert_value_impl(parent, force_to_left, key, new_node);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_key(std::true_type, const Key& key) -> std::pair<iterator, bool>
    {
        // this is the pathway for insertion of unique keys (map and set, but not multimap and multiset).
        // note that we return a pair and not an iterator. this is because the C++ standard for map
        // and set is to return a pair and not just an iterator.
        bool can_insert;

        rbtree_node_base* position = this->get_key_insertion_position_unique_keys(can_insert, key);

        if (can_insert)
        {
            const iterator result(this->insert_key_impl(position, false, key));

            return std::pair<iterator, bool>(result, true);
        }

        return std::pair<iterator, bool>(iterator(position), false);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_key(std::false_type, const Key& key) -> iterator
    {
        rbtree_node_base* position = this->get_key_insertion_position_non_unique_keys(key);

        return this->insert_key_impl(position, false, key);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_key(std::true_type, const_iterator position, const Key& key) -> iterator
    {
        bool force_to_left;

        rbtree_node_base* position = this->get_key_insertion_position_unique_keys_hint(position, force_to_left, key);

        if (position != nullptr)
        {
            return this->insert_key_impl(position, force_to_left, key);
        }
        else
        {
            return this->insert_key(has_unique_keys_type(), key).first;
        }
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_key(std::false_type, const_iterator position, const Key& key) -> iterator
    {
        bool force_to_left;

        rbtree_node_base* position = this->get_key_insertion_position_non_unique_keys_hint(position, force_to_left, key);

        if (position != nullptr)
        {
            return this->insert_key_impl(position, force_to_left, key);
        }
        else
        {
            return this->insert_key(has_unique_keys_type(), key);
        }
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::insert_key_impl(rbtree_node_base* parent, bool force_to_left, const Key& key)
    {
        rbtree_side side;
        ExtractKey extractor;

        if (force_to_left || (parent == &this->anchor_) || this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(parent)->value)))
        {
            side = rbtree_side::left;
        }
        else
        {
            side = rbtree_side::right;
        }

        rbtree_node<Value>* const new_node = this->create_node_from_key(key);

        eastl::rbtree_insert(new_node, parent, &this->anchor_, side);
        
        ++this->size_++;

        return iterator(new_node);
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::get_key_insertion_position_unique_keys(bool& can_insert, const Key& key) -> rbtree_node_base*
    {
        // this code is essentially a slightly modified copy of the the rbtree::insert
        // function whereby this version takes a key and not a full value_type.
        ExtractKey extractor;

        rbtree_node_base* current = this->anchor_.parent; // start with the root node.
        rbtree_node_base* lower_bound = &this->anchor_;   // Set it to the container end for now.
        rbtree_node_base* parent;                         // This will be where we insert the new node.

        bool value_less_than_node = true; // if the tree is empty, this will result in an insertion at the front.

        // find insertion position of the value. this will either be a position which 
        // already contains the value, a position which is greater than the value or
        // end(), which we treat like a position which is greater than the value.
        while (current != nullptr) // do a walk down the tree.
        {
            value_less_than_node = this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(current)->value));

            lower_bound = current;

            if (value_less_than_node)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        parent = lower_bound; // lower_bound is actually upper bound right now (i.e. it is > value instead of <=), but we will make it the lower bound below.

        if (value_less_than_node) // if we ended up on the left side of the last parent node...
        {
            if (lower_bound != this->anchor_.left) // if the tree was empty or if we otherwise need to insert at the very front of the tree...
            {
                // at this point, pLowerBound points to a node which is > than value.
                // move it back by one, so that it points to a node which is <= value.

                lower_bound = eastl::rbtree_decrement(lower_bound);
            }
            else
            {
                can_insert = true;

                return lower_bound;
            }
        }

        // since here we require values to be unique, we will do nothing if the value already exists.
        
        rbtree_node<Value>* const lower_bound_full_node = reinterpret_cast<rbtree_node<Value>*>(lower_bound);

        if (this->comparer_(extractor(lower_bound_full_node->value), key)) // if the node is < the value (i.e. if value is >= the node)...
        {
            can_insert = true;

            return parent;
        }

        // the item already exists (as found by the compare directly above), so return false.
        can_insert = false;

        return lower_bound;
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::get_key_insertion_position_non_unique_keys(const Key& key) -> rbtree_node_base*
    {
        ExtractKey extractor;

        rbtree_node_base* current = this->anchor_.parent; // start with the root node.
        rbtree_node_base* range_end = &this->anchor_;     // set it to the container end for now.

        while (current != nullptr)
        {
            range_end = current;

            if (this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(current)->value)))
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        return range_end;
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::get_key_insertion_position_unique_keys_hint(const_iterator position, bool& force_to_left, const Key& key) -> rbtree_node_base*
    {
        ExtractKey extractor;

        if ((position.node != this->anchor_.right) && (position.node != &this->anchor_)) // if the user specified a specific insertion position...
        {
            iterator it_next(position.node);
            
            ++it_next;

            const bool position_less_than_value = this->comparer_(extractor(*position), key);

            if (position_less_than_value) // If (value > *position)...
            {
                const bool value_less_than_next = this->comparer_(key, extractor(*it_next));

                if (value_less_than_next) // If value < *it_next...
                {
                    if (position.node->right)
                    {
                        force_to_left = true; // specifically insert in front of (to the left of) it_next (and thus after 'position').

                        return it_next.node;
                    }

                    force_to_left = false;

                    return position.node;
                }
            }

            force_to_left = false;
            
            return nullptr;  // the above specified hint was not useful, then we do a regular insertion.
        }

        if (this->size_ != 0u && this->comparer_(extractor(reinterpret_cast<rbtree_node<Value>*>(this->anchor_.right)->value), key))
        {
            force_to_left = false;

            return this->anchor_.right;
        }

        force_to_left = false;

        return nullptr; // the caller can do a default insert.
    }

    template <typename Key, typename Value, typename C, typename A, typename ExtractKey, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, ExtractKey, MI, UK>::get_key_insertion_position_non_unique_keys_hint(const_iterator position, bool& force_to_left, const Key& key) -> rbtree_node_base*
    {
        ExtractKey extractor;

        if ((position.node != this->anchor_.right) && (position.node != &this->anchor_)) // if the user specified a specific insertion position...
        {
            iterator it_next(position.node);

            ++it_next;

            if (!this->comparer_(key, extractor(*position)) && !this->comparer_(extractor(*it_next), key))
            {
                if (position.node->right) // if there are any nodes to the right... [this expression will always be true as long as we aren't at the end()]
                {
                    force_to_left = true; // specifically insert in front of (to the left of) it_next (and thus after 'position').

                    return it_next.node;
                }

                force_to_left = false;

                return position.node;
            }

            force_to_left = false;

            return nullptr; // the above specified hint was not useful, then we do a regular insertion.
        }

        // this pathway shouldn't be commonly executed, as the user shouldn't be calling 
        // this hinted version of insert if the user isn't providing a useful hint.

        if (this->size_ != 0u && !this->comparer_(key, extractor(reinterpret_cast<rbtree_node<Value>*>(this->anchor_.right)->value)))
        {
            force_to_left = false;

            return this->anchor_.right;
        }

        force_to_left = false;

        return nullptr;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<K, Value, C, A, E, MI, UK>::allocate_node() -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = reinterpret_cast<rbtree_node<Value>*>(this->allocator_.allocate(sizeof(rbtree_node<Value>), 0u));

        assert(node != nullptr);

        return node;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> void eastl::rbtree<K, Value, C, A, E, MI, UK>::free_node(rbtree_node<Value>* node)
    {
        node->~rbtree_node();

        this->allocator_.deallocate(node, sizeof(rbtree_node<Value>));
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::create_node_from_key(const Key& key) -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = this->allocate_node();

        new (reinterpret_cast<Key*>(&node->value)) Key(key);
#if defined(_DEBUG)
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        node->color = rbtree_color::black;
#endif
        return node;
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::create_node(const Value& value) -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = this->allocate_node();

        new (&node->value) Value(value);
#if defined(_DEBUG)
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        node->color = rbtree_color::black;
#endif
        return node;
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::create_node(Value&& value) -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = this->allocate_node();

        new (&node->value) Value(std::move(value));
#if defined(_DEBUG)
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        node->color = rbtree_color::black;
#endif
        return node;
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::create_node(const rbtree_node<Value>* node_src, rbtree_node_base* node_parent) -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = this->create_node(node_src->value);

        node->left = nullptr;
        node->right = nullptr;
        node->parent = node_parent;
        node->color = node_src->color;

        return node;
    }

    template <typename Key, typename Value, typename C, typename A, typename E, bool MI, bool UK> template <typename... Args> auto eastl::rbtree<Key, Value, C, A, E, MI, UK>::create_node(Args&&... args) -> rbtree_node<Value>*
    {
        rbtree_node<Value>* node = this->allocate_node();

        new (&node->value) Value(std::forward<Args>(args)...);
#if defined(_DEBUG)
        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        node->color = rbtree_color::black;
#endif
        return node;
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> void eastl::rbtree<K, V, C, A, E, MI, UK>::reset_lose_memory()
    {
        this->anchor_.right = &this->anchor_;
        this->anchor_.left = &this->anchor_;
        this->anchor_.parent = nullptr;
        this->anchor_.color = rbtree_color::red;
        this->size_ = 0u;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> auto eastl::rbtree<K, Value, C, A, E, MI, UK>::copy_subtree(const rbtree_node<Value>* node_src, rbtree_node_base* node_dst) -> rbtree_node_base*
    {
        rbtree_node<Value>* const new_node_root = this->create_node(node_src, node_dst);

        // copy the right side of the tree recursively.
        if (node_src->right != nullptr)
        {
            new_node_root->right = this->copy_subtree(reinterpret_cast<const rbtree_node<Value>*>(node_src->right), new_node_root);
        }

        rbtree_node_base* new_node_left;

        for (node_src = reinterpret_cast<const rbtree_node<Value>*>(node_src->left), node_dst = new_node_root;
             node_src != nullptr;
             node_src = reinterpret_cast<const rbtree_node<Value>*>(node_src->left), node_dst = new_node_left
            )
        {
            new_node_left = this->create_node(node_src, node_dst);

            node_dst->left = new_node_left;

            // copy the right side of the tree recursively.
            if (node_src->right)
            {
                new_node_left->right = this->copy_subtree(reinterpret_cast<const rbtree_node<Value>*>(node_src->right), new_node_left);
            }
        }

        return new_node_root;
    }

    template <typename K, typename Value, typename C, typename A, typename E, bool MI, bool UK> void eastl::rbtree<K, Value, C, A, E, MI, UK>::nuke_subtree(rbtree_node_base* node)
    {
        while (node != nullptr) // Recursively traverse the tree and destroy items as we go.
        {
            this->nuke_subtree(node->right);

            rbtree_node<Value>* left = static_cast<rbtree_node<Value>*>(node->left);

            this->free_node(reinterpret_cast<rbtree_node<Value>*>(node));

            node = left;
        }
    }

    template <typename K, typename V, typename C, typename A, typename E, bool MI, bool UK> void eastl::rbtree<K, V, C, A, E, MI, UK>::swap(rbtree& other)
    {
        if (this->allocator_ == other.allocator_) // if allocators are equivalent...
        {
            // most of our members can be exchaged by a basic swap:
            std::swap(this->size_, other.size_);
            std::swap(this->comparer_, other.comparer_);
            std::swap(this->allocator_, other.allocator_);

            // however, because our anchor node is a part of our class instance and not 
            // dynamically allocated, we can't do a swap of it but must do a more elaborate
            // procedure. This is the downside to having the mAnchor be like this, but 
            // otherwise we consider it a good idea to avoid allocating memory for a 
            // nominal container instance.

            // we optimize for the expected most common case: both pointers being non-null.
            if (this->anchor_.parent && other.anchor_.parent) // if both pointers are non-null...
            {
                std::swap(this->anchor_.right, other.anchor_.right);
                std::swap(this->anchor_.left, other.anchor_.left);
                std::swap(this->anchor_.parent, other.anchor_.parent);

                // we need to fix up the anchors to point to themselves (we can't just swap them).
                this->anchor_.parent->parent = &this->anchor_;
                other.anchor_.parent->parent = &other.anchor_;
            }
            else if (this->anchor_.parent)
            {
                other.anchor_.right = this->anchor_.right;
                other.anchor_.left = this->anchor_.left;
                other.anchor_.parent = this->anchor_.parent;
                other.anchor_.parent->parent = &other.anchor_;

                // we need to fix up our anchor to point it itself (we can't have it swap with x).
                this->anchor_.right = &this->anchor_;
                this->anchor_.left = &this->anchor_;
                this->anchor_.parent = nullptr;
            }
            else if (other.mAnchor.parent)
            {
                this->anchor_.right = other.anchor_.right;
                this->anchor_.left = other.anchor_.left;
                this->anchor_.parent = other.anchor_.parent;
                this->anchor_.parent->parent = &this->anchor_;

                // we need to fix up x's anchor to point it itself (we can't have it swap with us).
                other.anchor_.right = &other.anchor_;
                other.anchor_.left = &other.anchor_;
                other.anchor_.parent = nullptr;
            } // else both are NULL and there is nothing to do.
        }
        else
        {
            const rbtree temp(*this); // can't call eastl::swap because that would
            *this = other;            // itself call this member swap function.
            other = temp;
        }
    }



    template <typename Key, typename Value, typename Comparer, typename Allocator> auto eastl::map<Key, Value, Comparer, Allocator>::operator[](const Key& key) -> Value&
    {
        auto it = this->lower_bound(key);

        if ((it == this->end()) || this->comparer_(key, (*it).first))
        {
            it = this->insert_key(std::true_type(), it, key);
        }

        return it->second;
    }

    template <typename Key, typename Value, typename Comparer, typename Allocator> auto eastl::map<Key, Value, Comparer, Allocator>::at(const Key& key) -> Value*
    {
        auto it = this->lower_bound(key);

        if ((it == this->end()) || this->comparer_(key, (*it).first))
        {
            return nullptr;
        }

        return &it->second;
    }



    ASSERT_SIZE(eastl::rbtree_node_base, 0x10);
}
