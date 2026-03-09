//////////////////////////////////////////////////////////////////

#include <cassert>
#include <string>

//////////////////////////////////////////////////////////////////
// Person — составной объект
//////////////////////////////////////////////////////////////////

class Person
{
public:
    Person() = default;

    Person(std::string name, int age, int grade)
        : m_name(std::move(name)), m_age(age), m_grade(grade)
    {}

    const std::string& name() const { return m_name; }
    int age()   const { return m_age; }
    int grade() const { return m_grade; }

private:
    std::string m_name;
    int m_age   = 0;
    int m_grade = 0;
};

//////////////////////////////////////////////////////////////////
// Builder — пошаговое создание Person
//////////////////////////////////////////////////////////////////

class Builder
{
public:
    // начальное состояние
    Builder() = default;

    // этапы построения
    Builder& name(std::string value)
    {
        m_person_name = std::move(value);
        return *this;
    }

    Builder& age(int value)
    {
        m_person_age = value;
        return *this;
    }

    Builder& grade(int value)
    {
        m_person_grade = value;
        return *this;
    }

    // получение готового объекта
    Person get() const
    {
        return Person(m_person_name, m_person_age, m_person_grade);
    }

private:
    std::string m_person_name;
    int m_person_age   = 0;
    int m_person_grade = 0;
};

//////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////

int main()
{
    Builder builder;

    auto person = builder
        .name("Ivan")
        .age(25)
        .grade(10)
        .get();

    assert(person.name()  == "Ivan");
    assert(person.age()   == 25);
    assert(person.grade() == 10);
}

//////////////////////////////////////////////////////////////////
