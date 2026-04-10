#include <iostream>
#include <stack>
#include <utility>

template <typename T>
class Stack_v1
{
public:
    void push(T value)
    {
        T current_min = m_stack.empty() ? value : std::min(value, m_stack.top().second);
        m_stack.emplace(value, current_min);
    }

    T top() const
    {
        return m_stack.top().first;
    }

    void pop()
    {
        m_stack.pop();
    }

    T min() const
    {
        return m_stack.top().second;
    }

private:
    std::stack<std::pair<T, T>> m_stack;
};

template <typename T>
class Stack_v2
{
public:
    void push(T value)
    {
        if (m_stack.empty())
        {
            m_stack.push(value);
            m_min = value;
        }
        else if (value < m_min)
        {
            m_stack.push(2 * value - m_min);
            m_min = value;
        }
        else
        {
            m_stack.push(value);
        }
    }

    T top() const
    {
        return m_stack.top() < m_min ? m_min : m_stack.top();
    }

    void pop()
    {
        T const top_value = m_stack.top();
        m_stack.pop();
        if (top_value < m_min)
        {
            m_min = 2 * m_min - top_value;
        }
    }

    T min() const
    {
        return m_min;
    }

private:
    std::stack<T> m_stack;
    T m_min = T();
};

template <typename Stack>
bool run_stack_checks()
{
    Stack stack;
    stack.push(5);
    if (stack.top() != 5 || stack.min() != 5)
    {
        return false;
    }
    stack.push(2);
    if (stack.top() != 2 || stack.min() != 2)
    {
        return false;
    }
    stack.push(4);
    if (stack.top() != 4 || stack.min() != 2)
    {
        return false;
    }
    stack.pop();
    if (stack.top() != 2 || stack.min() != 2)
    {
        return false;
    }
    stack.pop();
    return stack.top() == 5 && stack.min() == 5;
}

int main()
{
    if (!run_stack_checks<Stack_v1<int>>())
    {
        std::cerr << "Stack_v1 checks failed\n";
        return 1;
    }
    if (!run_stack_checks<Stack_v2<int>>())
    {
        std::cerr << "Stack_v2 checks failed\n";
        return 2;
    }

    std::cout << "10.03: min-stack implementations work\n";
    return 0;
}
