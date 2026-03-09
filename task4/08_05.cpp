#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

class Timer
{
public:
    using duration_t = std::chrono::duration<double>;
    using clock_t = std::chrono::steady_clock;

    void start()
    {
        if (m_running)
        {
            throw std::logic_error("Timer::start called while timer is already running");
        }
        m_running = true;
        m_begin = clock_t::now();
    }

    void stop()
    {
        if (!m_running)
        {
            throw std::logic_error("Timer::stop called while timer is not running");
        }
        m_measurements.push_back(clock_t::now() - m_begin);
        m_running = false;
    }

    double average() const
    {
        if (m_measurements.empty())
        {
            return 0.0;
        }

        double sum = 0.0;
        for (duration_t const duration : m_measurements)
        {
            sum += duration.count();
        }
        return sum / static_cast<double>(m_measurements.size());
    }

    std::size_t samples() const
    {
        return m_measurements.size();
    }

private:
    bool m_running = false;
    clock_t::time_point m_begin{};
    std::vector<duration_t> m_measurements;
};

double calculate(std::size_t iterations)
{
    double x = 0.0;
    for (std::size_t i = 0; i < iterations; ++i)
    {
        x += std::sin(static_cast<double>(i)) * std::cos(static_cast<double>(i));
    }
    return x;
}

int main()
{
    Timer timer;

    bool caught = false;
    try
    {
        timer.stop();
    }
    catch (std::logic_error const&)
    {
        caught = true;
    }
    if (!caught)
    {
        std::cerr << "Expected logic_error from stop() before start()\n";
        return 1;
    }

    for (int i = 0; i < 5; ++i)
    {
        timer.start();
        (void)calculate(200'000);
        timer.stop();
    }

    double const average = timer.average();
    assert(timer.samples() == 5);
    assert(average > 0.0);

    std::cout << "08.05: average over " << timer.samples()
              << " measurements = " << average << " sec\n";
    return 0;
}
