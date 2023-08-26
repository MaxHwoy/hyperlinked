#include <hyperlib/bench.hpp>

namespace hyper
{
    auto bench::get_free_bench_id(const char* name) -> std::uint32_t
    {
        std::uint32_t id = static_cast<std::uint32_t>(bench::marks_.size());

        bench::marks_.emplace_back();

        bench::names_.push_back(name);

        return bench::marks_[id].id = id;
    }

    auto bench::get_current_average(std::uint32_t id) -> std::uint64_t
    {
        assert(id < bench::marks_.size());

        bench::mark& current = bench::marks_[id];

        if (current.count == 0u)
        {
            return 0u;
        }

        if (current.count < std::size(current.durations))
        {
            std::uint64_t sum = 0u;

            for (std::uint32_t i = 0u; i < current.count; ++i)
            {
                sum += current.durations[i];
            }

            return sum / current.count;
        }
        else
        {
            std::uint64_t sum = 0u;

            for (std::uint32_t i = 0u; i < std::size(current.durations); ++i)
            {
                sum += current.durations[i];
            }

            return sum / std::size(current.durations);
        }
    }

    auto bench::get_total_average(std::uint32_t id) -> std::uint64_t
    {
        assert(id < bench::marks_.size());

        bench::mark& current = bench::marks_[id];

        if (current.count == 0u)
        {
            return 0u;
        }
        else
        {
            return current.total / current.count;
        }
    }

    void bench::print()
    {
        ::printf("BenchID    TheCallCount    CurrAvg(ns)    CurrAvg(us)    TotalAvg(ns)    TotalAvg(us)    Maximum(ns)    Maximum(us)    TotalTime(ns)    TotalTime(us)    Function\n");
        ::printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for (std::uint32_t i = 0u; i < bench::marks_.size(); ++i)
        {
            const bench::mark& current = bench::marks_[i];

            std::uint64_t avg = bench::get_current_average(i);
            std::uint64_t tot = bench::get_total_average(i);
            const char* bname = bench::names_[i];

            ::printf("   %4d    %12llu    %11llu    %11llu    %12llu    %12llu    %11llu    %11llu    %13llu    %13llu    %s\n", 
                current.id, current.count, avg, avg / 1000u, tot, tot / 1000u, current.max, current.max / 1000u, current.total, current.total / 1000u, bname);
        }
    }

    void bench::reset()
    {
        size_t size = bench::marks_.size();

        if (size != 0u)
        {
            ::memset(&bench::marks_[0], 0, sizeof(bench::mark) * size);

            for (std::uint32_t i = 0u; i < size; ++i)
            {
                bench::marks_[i].id = i;
            }
        }
    }
}
