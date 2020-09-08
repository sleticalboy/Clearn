//
// Created by binlee on 2020/9/8.
//

#include <iostream>
#include "Person.h"

Person::Person(std::string name, int age, int gender) {
    this->name = std::move(name);
    this->age = age;
    this->gender = gender;
    std::cout << "Person's constructor is invoked." << std::endl;
}

Person::~Person() {
    std::cout << "Person's destructor is invoked." << std::endl;
}

void Person::speak() const {
    std::cout << "I'm " << name << " and I'm " << age << " years old.\n";
}

void person_main() {

    std::cout << "\n>>>>>This is my first cpp class: Person<<<<<" << std::endl;

    Person ben("Ben", 26, MALE);
    ben.speak();
}
