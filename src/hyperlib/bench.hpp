#pragma once

#include <cstdint>
#include <vector>
#include <chrono>
#include <cassert>

namespace hyper
{
    class bench final
    {
    private:
        struct __declspec(align(0x10)) mark
        {
            std::uint64_t durations[0x20];
            std::uint64_t total;
            std::uint64_t max;
            std::uint32_t count;
            std::uint32_t id;
        };

    public:
        inline bench(std::uint32_t id);

        inline ~bench();

    public:
        static auto get_free_bench_id(const char* name) -> std::uint32_t;

        static auto get_current_average(std::uint32_t id) -> std::uint64_t;

        static auto get_total_average(std::uint32_t id) -> std::uint64_t;

        static void print();

        static void reset();

    private:
        std::chrono::system_clock::time_point start_;
        std::uint32_t id_;

    private:
        static inline std::vector<mark> marks_{};
        static inline std::vector<const char*> names_{};
    };
}

namespace hyper
{
    bench::bench(std::uint32_t id) : id_(id), start_(std::chrono::system_clock::now())
    {
        assert(id < bench::marks_.size());
    }

    bench::~bench()
    {
        bench::mark& current = bench::marks_[this->id_];

        std::chrono::system_clock::duration duration = std::chrono::system_clock::now() - this->start_;

        std::uint64_t elapsed = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());

        current.durations[current.count++ & 0x1F] = elapsed;

        current.total += elapsed;

        if (current.max < elapsed)
        {
            current.max = elapsed;
        }
    }
}

#if defined(_DEBUG)
#define BENCHMARK()                                                                           \
    static std::uint32_t __s_bench_id = hyper::bench::get_free_bench_id(__FUNCSIG__); \
    hyper::bench __func_bench(__s_bench_id)
#else
#define BENCHMARK()
#endif
