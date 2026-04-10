#include <benchmark/benchmark.h>

#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <new>
#include <vector>

enum class SearchPolicy
{
    first_fit,
    best_fit
};

class Allocator
{
public:
    explicit Allocator(std::size_t bytes, SearchPolicy policy)
        : m_storage((align_up(bytes) + sizeof(std::max_align_t) - 1) / sizeof(std::max_align_t))
        , m_policy(policy)
    {
        m_blocks.push_back(Block{0, capacity_bytes(), true});
    }

    void* allocate(std::size_t bytes)
    {
        std::size_t const size = align_up(bytes);
        auto block = (m_policy == SearchPolicy::first_fit) ? find_first(size) : find_best(size);
        if (block == m_blocks.end())
        {
            throw std::bad_alloc();
        }

        if (block->size > size)
        {
            m_blocks.insert(std::next(block), Block{block->offset + size, block->size - size, true});
            block->size = size;
        }

        block->free = false;
        return base() + block->offset;
    }

    void deallocate(void* pointer)
    {
        if (pointer == nullptr)
        {
            return;
        }

        std::size_t const offset = static_cast<std::size_t>(static_cast<std::byte*>(pointer) - base());
        for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            if (it->offset == offset && !it->free)
            {
                it->free = true;
                merge_neighbors(it);
                return;
            }
        }

        throw std::runtime_error("invalid pointer passed to deallocate");
    }

private:
    struct Block
    {
        std::size_t offset = 0;
        std::size_t size = 0;
        bool free = true;
    };

    using iterator_t = std::list<Block>::iterator;

    static std::size_t align_up(std::size_t value)
    {
        std::size_t const alignment = alignof(std::max_align_t);
        return (value + alignment - 1) / alignment * alignment;
    }

    std::size_t capacity_bytes() const
    {
        return m_storage.size() * sizeof(std::max_align_t);
    }

    std::byte* base()
    {
        return reinterpret_cast<std::byte*>(m_storage.data());
    }

    iterator_t find_first(std::size_t size)
    {
        for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            if (it->free && it->size >= size)
            {
                return it;
            }
        }
        return m_blocks.end();
    }

    iterator_t find_best(std::size_t size)
    {
        iterator_t best = m_blocks.end();
        for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            if (!it->free || it->size < size)
            {
                continue;
            }
            if (best == m_blocks.end() || it->size < best->size)
            {
                best = it;
            }
        }
        return best;
    }

    void merge_neighbors(iterator_t block)
    {
        if (block != m_blocks.begin())
        {
            auto previous = std::prev(block);
            if (previous->free)
            {
                previous->size += block->size;
                block = m_blocks.erase(block);
                block = previous;
            }
        }

        auto next = std::next(block);
        if (next != m_blocks.end() && next->free)
        {
            block->size += next->size;
            m_blocks.erase(next);
        }
    }

    std::vector<std::max_align_t> m_storage;
    std::list<Block> m_blocks;
    SearchPolicy m_policy;
};

void allocator_script(SearchPolicy policy)
{
    Allocator allocator(1U << 20, policy);
    std::vector<void*> pointers;
    pointers.reserve(256);
    std::vector<std::size_t> const sizes = {16, 24, 32, 48, 64, 80, 96, 128};

    for (int round = 0; round < 64; ++round)
    {
        for (std::size_t index = 0; index < 128; ++index)
        {
            pointers.push_back(allocator.allocate(sizes[index % sizes.size()] + static_cast<std::size_t>(round % 3) * 8));
        }

        for (std::size_t index = 0; index < pointers.size(); index += 2)
        {
            allocator.deallocate(pointers[index]);
            pointers[index] = nullptr;
        }

        std::vector<void*> extra;
        extra.reserve(64);
        for (std::size_t index = 0; index < 64; ++index)
        {
            extra.push_back(allocator.allocate(sizes[(index + 3) % sizes.size()]));
        }

        for (void* pointer : extra)
        {
            allocator.deallocate(pointer);
        }

        for (void* pointer : pointers)
        {
            if (pointer != nullptr)
            {
                allocator.deallocate(pointer);
            }
        }
        pointers.clear();
    }
}

void self_test()
{
    for (SearchPolicy policy : {SearchPolicy::first_fit, SearchPolicy::best_fit})
    {
        Allocator allocator(512, policy);
        void* a = allocator.allocate(32);
        void* b = allocator.allocate(64);
        void* c = allocator.allocate(32);
        allocator.deallocate(b);
        void* d = allocator.allocate(48);
        if (a == nullptr || c == nullptr || d == nullptr)
        {
            throw std::runtime_error("allocator returned nullptr");
        }
        allocator.deallocate(a);
        allocator.deallocate(c);
        allocator.deallocate(d);
    }
}

static void BM_FirstFit(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        allocator_script(SearchPolicy::first_fit);
    }
}

static void BM_BestFit(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        allocator_script(SearchPolicy::best_fit);
    }
}

BENCHMARK(BM_FirstFit);
BENCHMARK(BM_BestFit);

int main(int argc, char** argv)
{
    try
    {
        self_test();
    }
    catch (std::exception const& error)
    {
        std::cerr << "09.09 self-test failed: " << error.what() << '\n';
        return 1;
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
