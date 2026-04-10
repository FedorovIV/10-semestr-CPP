#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <utility>
#include <vector>

class Allocator
{
public:
    virtual ~Allocator() = default;

    virtual void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;
    virtual void deallocate(void* pointer) = 0;

protected:
    template <typename T>
    T* get(void* pointer) const
    {
        return static_cast<T*>(pointer);
    }

    template <typename T>
    T const* get(void const* pointer) const
    {
        return static_cast<T const*>(pointer);
    }
};

class LinearAllocator final : public Allocator
{
public:
    explicit LinearAllocator(std::size_t size)
        : m_size(size)
        , m_begin(::operator new(size, std::align_val_t(alignof(std::max_align_t))))
    {
    }

    ~LinearAllocator() override
    {
        ::operator delete(m_begin, m_size, std::align_val_t(alignof(std::max_align_t)));
    }

    void* allocate(std::size_t size, std::size_t alignment) override
    {
        void* current = get<std::byte>(m_begin) + m_offset;
        std::size_t free = m_size - m_offset;
        if (std::align(alignment, size, current, free) == nullptr)
        {
            return nullptr;
        }

        m_offset = m_size - free + size;
        return current;
    }

    void deallocate(void* /*pointer*/) override
    {
        // Linear allocator frees the whole buffer at once, so single deallocation is a no-op.
    }

private:
    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void* m_begin = nullptr;
};

class StackAllocator final : public Allocator
{
public:
    explicit StackAllocator(std::size_t size)
        : m_size(size)
        , m_begin(::operator new(size, std::align_val_t(alignof(std::max_align_t))))
    {
    }

    ~StackAllocator() override
    {
        ::operator delete(m_begin, m_size, std::align_val_t(alignof(std::max_align_t)));
    }

    void* allocate(std::size_t size, std::size_t alignment) override
    {
        void* current = get<std::byte>(m_begin) + m_offset + sizeof(Header);
        std::size_t free = m_size - m_offset - sizeof(Header);
        if (std::align(alignment, size, current, free) == nullptr)
        {
            return nullptr;
        }

        Header* header = get<Header>(get<std::byte>(current) - sizeof(Header));
        header->padding = static_cast<std::uint8_t>(
            std::distance(get<std::byte>(m_begin) + m_offset, get<std::byte>(current)));
        m_offset = static_cast<std::size_t>(get<std::byte>(current) - get<std::byte>(m_begin)) + size;
        return current;
    }

    void deallocate(void* pointer) override
    {
        Header const* header = get<Header const>(get<std::byte>(pointer) - sizeof(Header));
        m_offset = static_cast<std::size_t>(get<std::byte>(pointer) - get<std::byte>(m_begin)) - header->padding;
    }

private:
    struct Header
    {
        std::uint8_t padding = 0;
    };

    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void* m_begin = nullptr;
};

class PoolAllocator final : public Allocator
{
public:
    PoolAllocator(std::size_t size, std::size_t step)
        : m_size(size)
        , m_step(step)
        , m_begin(::operator new(size, std::align_val_t(alignof(std::max_align_t))))
    {
        reset();
    }

    ~PoolAllocator() override
    {
        ::operator delete(m_begin, m_size, std::align_val_t(alignof(std::max_align_t)));
    }

    void* allocate(std::size_t size, std::size_t /*alignment*/) override
    {
        if (size > m_step || m_head == nullptr)
        {
            return nullptr;
        }

        Node* node = m_head;
        m_head = m_head->next;
        return node;
    }

    void deallocate(void* pointer) override
    {
        Node* node = get<Node>(pointer);
        node->next = m_head;
        m_head = node;
    }

private:
    struct Node
    {
        Node* next = nullptr;
    };

    void reset()
    {
        m_head = get<Node>(m_begin);
        std::byte* current = get<std::byte>(m_begin);
        std::byte* end = current + m_size;
        while (current + m_step < end)
        {
            Node* node = get<Node>(current);
            node->next = get<Node>(current + m_step);
            current += m_step;
        }
        get<Node>(current)->next = nullptr;
    }

    std::size_t m_size = 0;
    std::size_t m_step = 0;
    void* m_begin = nullptr;
    Node* m_head = nullptr;
};

class FreeListAllocator final : public Allocator
{
public:
    explicit FreeListAllocator(std::size_t size)
        : m_size(size)
        , m_begin(::operator new(size, std::align_val_t(alignof(std::max_align_t))))
    {
        m_head = get<Node>(m_begin);
        m_head->size = size - sizeof(Header);
        m_head->next = nullptr;
    }

    ~FreeListAllocator() override
    {
        ::operator delete(m_begin, m_size, std::align_val_t(alignof(std::max_align_t)));
    }

    void* allocate(std::size_t size, std::size_t /*alignment*/) override
    {
        std::pair<Node*, Node*> const found = find(size);
        Node* current = found.first;
        Node* previous = found.second;
        if (current == nullptr)
        {
            return nullptr;
        }

        if (current->size >= size + sizeof(Node) + 1)
        {
            std::size_t const step = sizeof(Header) + size;
            Node* next = get<Node>(get<std::byte>(current) + step);
            next->size = current->size - step;
            next->next = current->next;
            current->next = next;
        }
        else
        {
            size = current->size;
        }

        if (previous == nullptr)
        {
            m_head = current->next;
        }
        else
        {
            previous->next = current->next;
        }

        Header* header = get<Header>(current);
        header->size = size;
        return get<std::byte>(current) + sizeof(Header);
    }

    void deallocate(void* pointer) override
    {
        Node* node = get<Node>(get<std::byte>(pointer) - sizeof(Header));
        Node* previous = nullptr;
        Node* current = m_head;

        while (current != nullptr && node > current)
        {
            previous = current;
            current = current->next;
        }

        node->next = current;
        if (previous == nullptr)
        {
            m_head = node;
        }
        else
        {
            previous->next = node;
        }

        merge(previous, node);
    }

private:
    struct alignas(std::max_align_t) Header
    {
        std::size_t size = 0;
    };

    struct Node
    {
        std::size_t size = 0;
        Node* next = nullptr;
    };

    std::pair<Node*, Node*> find(std::size_t size) const
    {
        Node* current = m_head;
        Node* previous = nullptr;
        while (current != nullptr && current->size < size)
        {
            previous = current;
            current = current->next;
        }
        return {current, previous};
    }

    void merge(Node* previous, Node* node)
    {
        if (node->next != nullptr &&
            get<std::byte>(node) + sizeof(Header) + node->size == get<std::byte>(node->next))
        {
            node->size += sizeof(Header) + node->next->size;
            node->next = node->next->next;
        }

        if (previous != nullptr &&
            get<std::byte>(previous) + sizeof(Header) + previous->size == get<std::byte>(node))
        {
            previous->size += sizeof(Header) + node->size;
            previous->next = node->next;
        }
    }

    std::size_t m_size = 0;
    void* m_begin = nullptr;
    Node* m_head = nullptr;
};

bool check_linear(Allocator& allocator)
{
    void* first = allocator.allocate(16, 8);
    void* second = allocator.allocate(16, 8);
    allocator.deallocate(first);
    return first != nullptr && second != nullptr && first != second;
}

bool check_stack(Allocator& allocator)
{
    void* first = allocator.allocate(16, 8);
    void* second = allocator.allocate(24, 8);
    allocator.deallocate(second);
    void* third = allocator.allocate(24, 8);
    return first != nullptr && second != nullptr && third == second;
}

bool check_pool(Allocator& allocator)
{
    void* first = allocator.allocate(16, alignof(std::max_align_t));
    void* second = allocator.allocate(16, alignof(std::max_align_t));
    allocator.deallocate(first);
    void* third = allocator.allocate(16, alignof(std::max_align_t));
    return second != nullptr && third == first;
}

bool check_free_list(Allocator& allocator)
{
    void* first = allocator.allocate(16, alignof(std::max_align_t));
    void* second = allocator.allocate(16, alignof(std::max_align_t));
    void* third = allocator.allocate(16, alignof(std::max_align_t));
    allocator.deallocate(second);
    allocator.deallocate(first);
    void* merged = allocator.allocate(32, alignof(std::max_align_t));
    return third != nullptr && merged == first;
}

int main()
{
    std::vector<std::unique_ptr<Allocator>> allocators;
    allocators.push_back(std::make_unique<LinearAllocator>(256));
    allocators.push_back(std::make_unique<StackAllocator>(256));
    allocators.push_back(std::make_unique<PoolAllocator>(256, 32));
    allocators.push_back(std::make_unique<FreeListAllocator>(256));

    if (!check_linear(*allocators[0]))
    {
        std::cerr << "Linear allocator check failed\n";
        return 1;
    }
    if (!check_stack(*allocators[1]))
    {
        std::cerr << "Stack allocator check failed\n";
        return 2;
    }
    if (!check_pool(*allocators[2]))
    {
        std::cerr << "Pool allocator check failed\n";
        return 3;
    }
    if (!check_free_list(*allocators[3]))
    {
        std::cerr << "Free-list allocator check failed\n";
        return 4;
    }

    std::cout << "09.10: polymorphic allocators work\n";
    return 0;
}
