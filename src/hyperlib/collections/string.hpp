#pragma once

#include <hyperlib/shared.hpp>
#include <hyperlib/collections/bstl.hpp>
#include <hyperlib/collections/eastl.hpp>

namespace hyper
{
    class vector_string final : public eastl::vector<char>
    {
    public:
        vector_string();
        vector_string(const vector_string& other);
        vector_string(vector_string&& other);
        vector_string(const char* str);
        vector_string& operator=(const vector_string& other);
        vector_string& operator=(vector_string&& other);
        ~vector_string();

        operator const char*();

        auto length() const -> size_t;

        static auto empty() -> const vector_string&;

    private:
        static char empty_array_[1];

        static vector_string empty_;
    };
}
