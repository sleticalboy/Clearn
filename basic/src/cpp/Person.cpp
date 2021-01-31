//
// Created by binlee on 2020/9/8.
//

#include "Person.h"

Ben::Ben(std::string name, int age, int gender) {
    mName = move(name);
    mAge = age;
    mGender = gender;
    std::cout << "Person's constructor is invoked." << std::endl;
}

void Ben::speak() {
    std::cout << "I'm " << getName() << " and I'm " << getAge() << " years old.\n" << std::endl;
}

std::string Ben::toString() {
    return "name: " + getName() + ", age: " + std::to_string(getAge()) + ", gender: " + std::to_string(getGender());
}

void person_main() {

    std::cout << "\n>>>>>This is my first cpp class: Person<<<<<" << std::endl;

    Ben ben("Ben", 26, MALE);

    ben.speak();

    std::cout << ben.toString() << std::endl;
}
