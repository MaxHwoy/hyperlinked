#include <hyperlib/collections/string.hpp>

#pragma warning (disable : 26439)

namespace hyper
{
    char vector_string::empty_array_[1] { 0 };

    vector_string vector_string::empty_{};

    vector_string::vector_string()
    {
        this->begin_ = vector_string::empty_array_;
        this->end_ = vector_string::empty_array_;
        this->capacity_ = vector_string::empty_array_ + 1u;
    }

    vector_string::vector_string(const vector_string& other) : vector<char, bstl::allocator>()
    {
        if (other.length() == 0u)
        {
            this->begin_ = vector_string::empty_array_;
            this->end_ = vector_string::empty_array_;
            this->capacity_ = vector_string::empty_array_ + 1u;
        }
        else
        {
            this->reserve(other.capacity());

            ::memcpy(this->begin_, other.begin_, other.capacity());

            this->end_ = this->begin_ + other.length();
        }
    }

    vector_string::vector_string(vector_string&& other)
    {
        this->begin_ = other.begin_;
        this->end_ = other.end_;
        this->capacity_ = other.capacity_;

        this->begin_ = vector_string::empty_array_;
        this->end_ = vector_string::empty_array_;
        this->capacity_ = vector_string::empty_array_ + 1u;
    }

    vector_string::vector_string(const char* str)
    {
        size_t length = str == nullptr ? 0u : string::length(str);

        if (length == 0u)
        {
            this->begin_ = vector_string::empty_array_;
            this->end_ = vector_string::empty_array_;
            this->capacity_ = vector_string::empty_array_ + 1u;
        }
        else
        {
            this->reserve(length + 1u);

            ::memcpy(this->begin_, str, length);

            this->end_ = this->begin_ + length;

            *this->end_ = 0;
        }
    }

    auto vector_string::operator=(const vector_string& other) -> vector_string&
    {
        if (this != &other)
        {
            if (other.length() == 0u)
            {
                this->begin_ = vector_string::empty_array_;
                this->end_ = vector_string::empty_array_;
                this->capacity_ = vector_string::empty_array_ + 1u;
            }
            else
            {
                this->reserve(other.capacity());

                ::memcpy(this->begin_, other.begin_, other.capacity());

                this->end_ = this->begin_ + other.length();
            }
        }

        return *this;
    }

    auto vector_string::operator=(vector_string&& other) -> vector_string&
    {
        if (this != &other)
        {
            if (this->length() != 0u)
            {
                this->free(this->begin_, this->capacity());
            }

            this->begin_ = other.begin_;
            this->end_ = other.end_;
            this->capacity_ = other.capacity_;

            other.begin_ = vector_string::empty_array_;
            other.end_ = vector_string::empty_array_;
            other.capacity_ = vector_string::empty_array_ + 1u;
        }

        return *this;
    }

    vector_string::~vector_string()
    {
        if (this->begin_ == vector_string::empty_array_)
        {
            this->begin_ = nullptr;
            this->end_ = nullptr;
            this->capacity_ = nullptr;
        }
    }

    vector_string::operator const char*()
    {
        return this->begin_;
    }

    auto vector_string::length() const -> size_t
    {
        return static_cast<size_t>(this->end_ - this->begin_);
    }

    auto vector_string::empty() -> const vector_string&
    {
        return vector_string::empty_;
    }
}
