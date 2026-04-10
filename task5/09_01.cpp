#include <iostream>
#include <source_location>

class Tracer
{
public:
    explicit Tracer(std::source_location location = std::source_location::current())
        : m_location(location)
    {
        ++s_enter_count;
        std::cout << "enter: " << m_location.function_name()
                  << " at line " << m_location.line() << '\n';
    }

    ~Tracer()
    {
        ++s_leave_count;
        std::cout << "leave: " << m_location.function_name()
                  << " at line " << m_location.line() << '\n';
    }

    static int enter_count()
    {
        return s_enter_count;
    }

    static int leave_count()
    {
        return s_leave_count;
    }

private:
    std::source_location m_location;

    static inline int s_enter_count = 0;
    static inline int s_leave_count = 0;
};

#ifdef NDEBUG
#define trace() ((void)0)
#else
#define TRACE_JOIN_IMPL(lhs, rhs) lhs##rhs
#define TRACE_JOIN(lhs, rhs) TRACE_JOIN(lhs, rhs)
#define trace() Tracer TRACE_JOIN(local_tracer_, __LINE__)
#endif

void inner()
{
    trace();
}

void outer()
{
    trace();
    inner();
}

int main()
{
#ifdef NDEBUG
    outer();
    std::cout << "09.01: tracing disabled because NDEBUG is defined\n";
#else
    outer();
    if (Tracer::enter_count() != 2 || Tracer::leave_count() != 2)
    {
        std::cerr << "Unexpected tracer counters\n";
        return 1;
    }
    std::cout << "09.01: tracing works\n";
#endif

    return 0;
}
