#include <iostream>
#include <memory>
#include <string>

template <typename T>
class List
{
private:
    struct Node
    {
        explicit Node(T value)
            : value(std::move(value))
        {
        }

        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> previous;
    };

public:
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(std::shared_ptr<Node> node = nullptr, std::shared_ptr<Node> tail = nullptr)
            : m_node(std::move(node))
            , m_tail(std::move(tail))
        {
        }

        Iterator& operator++()
        {
            m_node = m_node->next;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator copy = *this;
            ++(*this);
            return copy;
        }

        Iterator& operator--()
        {
            if (m_node)
            {
                m_node = m_node->previous.lock();
            }
            else
            {
                m_node = m_tail.lock();
            }
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator copy = *this;
            --(*this);
            return copy;
        }

        T& operator*() const
        {
            return m_node->value;
        }

        T* operator->() const
        {
            return &m_node->value;
        }

        friend bool operator==(Iterator const& lhs, Iterator const& rhs)
        {
            return lhs.m_node == rhs.m_node;
        }

    private:
        std::shared_ptr<Node> m_node;
        std::weak_ptr<Node> m_tail;
    };

    Iterator begin() const
    {
        return Iterator(m_head, m_tail);
    }

    Iterator end() const
    {
        return Iterator(nullptr, m_tail);
    }

    void push_back(T value)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>(std::move(value));
        if (!m_head)
        {
            m_head = node;
            m_tail = node;
            return;
        }

        node->previous = m_tail;
        m_tail->next = node;
        m_tail = node;
    }

private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
};

int main()
{
    List<std::string> list;
    list.push_back("one");
    list.push_back("two");
    list.push_back("three");

    std::string joined;
    for (auto const& value : list)
    {
        joined += value;
    }
    if (joined != "onetwothree")
    {
        std::cerr << "Forward iteration failed\n";
        return 1;
    }

    auto iterator = list.end();
    --iterator;
    if (*iterator != "three")
    {
        std::cerr << "Decrement from end failed\n";
        return 2;
    }
    iterator--;
    if (*iterator != "two")
    {
        std::cerr << "Postfix decrement failed\n";
        return 3;
    }

    std::cout << "09.05: bidirectional list iterator works\n";
    return 0;
}
