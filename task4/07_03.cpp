#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum class Status
{
    success,
    failure
};

struct Recorder
{
    void save(Status status, int id)
    {
        if (throw_on_save)
        {
            throw std::runtime_error("save() failed");
        }
        entries.emplace_back(status, id);
    }

    bool throw_on_save = false;
    std::vector<std::pair<Status, int>> entries;
};

class Person
{
public:
    Person(std::string name, int grade, int salary, int id)
        : m_name(std::move(name))
        , m_grade(grade)
        , m_salary(salary)
        , m_id(id)
    {
    }

    std::string const& name() const
    {
        if (throw_on_name)
        {
            throw std::runtime_error("name() failed");
        }
        return m_name;
    }

    int grade() const
    {
        if (throw_on_grade)
        {
            throw std::runtime_error("grade() failed");
        }
        return m_grade;
    }

    int salary() const
    {
        if (throw_on_salary)
        {
            throw std::runtime_error("salary() failed");
        }
        return m_salary;
    }

    int id() const
    {
        if (throw_on_id)
        {
            throw std::runtime_error("id() failed");
        }
        return m_id;
    }

    bool throw_on_name = false;
    bool throw_on_grade = false;
    bool throw_on_salary = false;
    bool throw_on_id = false;

private:
    std::string m_name;
    int m_grade = 0;
    int m_salary = 0;
    int m_id = 0;
};

void test(Person const& person, Recorder& recorder)
{
    // Potential throw points:
    // 1) stream output, 2) person.name(),
    // 3) person.grade(), 4) person.salary(),
    // 5) person.id(), 6) recorder.save(...).
    std::cout << "test: " << person.name() << '\n';

    if (person.grade() == 10 || person.salary() > 1'000'000)
    {
        recorder.save(Status::success, person.id());
    }
    else
    {
        recorder.save(Status::failure, person.id());
    }
}

int main()
{
    Recorder recorder;

    Person alice("Alice", 10, 50'000, 1);
    test(alice, recorder);
    assert(recorder.entries.size() == 1);
    assert(recorder.entries.back().first == Status::success);

    Person bob("Bob", 8, 50'000, 2);
    test(bob, recorder);
    assert(recorder.entries.size() == 2);
    assert(recorder.entries.back().first == Status::failure);

    bool caught = false;
    Person carol("Carol", 9, 1'500'000, 3);
    carol.throw_on_id = true;
    try
    {
        test(carol, recorder);
    }
    catch (std::runtime_error const& error)
    {
        caught = true;
        std::cout << "Caught runtime_error: " << error.what() << '\n';
    }
    if (!caught)
    {
        std::cerr << "Expected exception was not thrown\n";
        return 1;
    }

    std::cout << "07.03: analysis demo completed\n";
    return 0;
}
