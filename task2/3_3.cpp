///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <memory>

///////////////////////////////////////////////////////////////////////////////////////////////

class List
{
private:
    struct Node
    {
        int value = 0;
        Node *next = nullptr;

        Node(int v, Node *n = nullptr) : value(v), next(n) {}
    };

    Node *m_head = nullptr;
    Node *m_tail = nullptr;

public:
    // -------------------------------------------------------------------------
    List() = default;

    // Запрещаем копирование (для простоты)
    List(List const &) = delete;
    List &operator=(List const &) = delete;

    // -------------------------------------------------------------------------
    ~List()
    {
        while (!empty())
            pop_front();
    }

    // -------------------------------------------------------------------------
    bool empty() const
    {
        return m_head == nullptr;
    }

    // -------------------------------------------------------------------------
    void push_front(int value)
    {
        Node *node = new Node(value, m_head);
        m_head = node;
        if (m_tail == nullptr)
            m_tail = node;
    }

    // -------------------------------------------------------------------------
    void push_back(int value)
    {
        Node *node = new Node(value);
        if (m_tail)
            m_tail->next = node;
        else
            m_head = node;
        m_tail = node;
    }

    // -------------------------------------------------------------------------
    void pop_front()
    {
        if (empty())
            return;
        Node *tmp = m_head;
        m_head = m_head->next;
        delete tmp;
        if (m_head == nullptr)
            m_tail = nullptr;
    }

    // -------------------------------------------------------------------------
    void pop_back()
    {
        if (empty())
            return;

        if (m_head == m_tail)
        {
            delete m_head;
            m_head = m_tail = nullptr;
            return;
        }

        Node *prev = m_head;
        while (prev->next != m_tail)
            prev = prev->next;

        delete m_tail;
        m_tail = prev;
        m_tail->next = nullptr;
    }

    // -------------------------------------------------------------------------
    void show(std::ostream &os = std::cout) const
    {
        Node *curr = m_head;
        while (curr)
        {
            os << curr->value;
            if (curr->next)
                os << ' ';
            curr = curr->next;
        }
    }

    // -------------------------------------------------------------------------
    int middle() const
    {
        if (empty())
            throw std::runtime_error("List is empty");

        Node *slow = m_head;
        Node *fast = m_head;

        while (fast && fast->next && fast->next->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow->value;
    }

    int front() const
    {
        if (empty())
            throw std::runtime_error("List is empty");
        return m_head->value;
    }
};

void test_reverse()
{
    List list;
    for (int i = 1; i <= 5; ++i)
        list.push_back(i);

    List reversed;
    while (!list.empty())
    {
        int val = list.front(); // теперь берём первый элемент
        list.pop_front();
        reversed.push_front(val);
    }

    std::cout << "Reversed list: ";
    reversed.show();
    std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    List list;

    assert(list.empty());

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    assert(!list.empty());

    // Проверим вывод
    std::cout << "List contents: ";
    list.show();
    std::cout << '\n';

    // Проверка среднего элемента
    assert(list.middle() == 3);

    // Проверка pop_front
    list.pop_front(); // удалили 1
    assert(list.middle() == 3);

    // Проверка pop_back
    list.pop_back(); // удалили 5
    assert(list.middle() == 3);

    // Добавим в начало
    list.push_front(0);
    assert(list.middle() == 2);

    // Освобождение памяти проверится деструктором
    std::cout << "All tests passed!\n";

    test_reverse();
}

///////////////////////////////////////////////////////////////////////////////////////////////
