#include <algorithm>
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

    auto bench::get_bench_id(std::uint32_t id) -> std::uint64_t
    {
        return static_cast<std::uint64_t>(id);
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

    auto bench::get_max_execution(std::uint32_t id) -> std::uint64_t
    {
        assert(id < bench::marks_.size());

        return bench::marks_[id].max;
    }

    void bench::print(sort_type sort)
    {
        assert(sort >= sort_type::first && sort < sort_type::count);

        size_t count = bench::marks_.size();

        sort_entry* sort_table = new sort_entry[count];

        if (sort_table)
        {
            std::uint64_t(*func)(std::uint32_t) = nullptr;

            switch (sort)
            {
                case sort_type::bench_id:
                    func = bench::get_bench_id;
                    break;

                case sort_type::avg_time:
                    func = bench::get_current_average;
                    break;

                case sort_type::max_time:
                    func = bench::get_max_execution;
                    break;

                case sort_type::total_time:
                    func = bench::get_total_average;
                    break;
            }

            assert(func != nullptr);

            for (std::uint32_t i = 0u; i < count; ++i)
            {
                sort_table[i] = { func(i), i };
            }

            std::sort(sort_table, sort_table + count, [](const sort_entry& lhs, const sort_entry& rhs) -> bool
            {
                return rhs.value < lhs.value;
            });

            ::printf("BenchID    TheCallCount    CurrAvg(ns)    CurrAvg(us)    TotalAvg(ns)    TotalAvg(us)    Maximum(ns)    Maximum(us)    TotalTime(ns)    TotalTime(us)    Function\n");
            ::printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

            for (std::uint32_t i = 0u; i < count; ++i)
            {
                const bench::mark& current = bench::marks_[sort_table[i].id];

                std::uint64_t avg = bench::get_current_average(current.id);
                std::uint64_t tot = bench::get_total_average(current.id);
                const char* bname = bench::names_[current.id];

                ::printf("   %4d    %12llu    %11llu    %11llu    %12llu    %12llu    %11llu    %11llu    %13llu    %13llu    %s\n",
                    current.id, current.count, avg, avg / 1000u, tot, tot / 1000u, current.max, current.max / 1000u, current.total, current.total / 1000u, bname);
            }

            delete[] sort_table;
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
