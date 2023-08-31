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
            /*
            typedef rbtree_iterator<T, Pointer, Reference>      this_type;
            typedef rbtree_iterator<T, T*, T&>                  iterator;
            typedef rbtree_iterator<T, const T*, const T&>      const_iterator;
            typedef eastl_size_t                                size_type;     // See config.h for the definition of eastl_size_t, which defaults to size_t.
            typedef ptrdiff_t                                   difference_type;
            typedef T                                           value_type;
            typedef rbtree_node_base                            base_node_type;
            typedef rbtree_node<T>                              node_type;
            typedef Pointer                                     pointer;
            typedef Reference                                   reference;
            typedef EASTL_ITC_NS::bidirectional_iterator_tag    iterator_category;
            */

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
            /*
            typedef ptrdiff_t                                                                       difference_type;
            typedef eastl_size_t                                                                    size_type;     // See config.h for the definition of eastl_size_t, which defaults to size_t.
            typedef Key                                                                             key_type;
            typedef Value                                                                           value_type;
            typedef rbtree_node<value_type>                                                         node_type;
            typedef value_type&                                                                     reference;
            typedef const value_type&                                                               const_reference;
            typedef value_type*                                                                     pointer;
            typedef const value_type*                                                               const_pointer;

            typedef typename conditional<bMutableIterators,
                rbtree_iterator<value_type, value_type*, value_type&>,
                rbtree_iterator<value_type, const value_type*, const value_type&> >::type           iterator;

            typedef rbtree_iterator<value_type, const value_type*, const value_type&>               const_iterator;
            typedef eastl::reverse_iterator<iterator>                                               reverse_iterator;
            typedef eastl::reverse_iterator<const_iterator>                                         const_reverse_iterator;

            typedef Allocator                                                                       allocator_type;
            typedef Compare                                                                         key_compare;
            typedef typename conditional<bUniqueKeys, eastl::pair<iterator, bool>, iterator>::type  insert_return_type;  // map/set::insert return a pair, multimap/multiset::iterator return an iterator.
            
            typedef rbtree<Key, Value, Compare, Allocator,
                ExtractKey, bMutableIterators, bUniqueKeys>                                         this_type;

            typedef rb_base<Key, Value, Compare, ExtractKey, bUniqueKeys, this_type>                base_type;
            typedef integral_constant<bool, bUniqueKeys>                                            has_unique_keys_type;
            typedef typename base_type::extract_key                                                 extract_key;
            */

        public:
            typedef typename std::conditional<MutableIterators,
                rbtree_iterator<Value, Value*, Value&>,
                rbtree_iterator<Value, const Value*, const Value&>>::type   iterator;

            typedef rbtree_iterator<Value, const Value*, const Value&>      const_iterator;

        public:
            rbtree();

            rbtree(const Allocator& allocator);

            rbtree(const Comparer& comparer, const Allocator& allocator);
            
            rbtree(const rbtree& other);
            
            rbtree(rbtree&& other);
            
            rbtree(rbtree&& other, const Allocator& allocator);

            //rbtree& operator=(const rbtree& x);

            //rbtree& operator=(std::initializer_list<Value> ilist);

            //rbtree& operator=(rbtree&& x);

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

    template <typename T, typename P, typename R> eastl::rbtree_iterator<T, P, R>::rbtree_iterator(const rbtree_node_base* node) : node_(node)
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

    ASSERT_SIZE(eastl::rbtree_node_base, 0x10);
}
