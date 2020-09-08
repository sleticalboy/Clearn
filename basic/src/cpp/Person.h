//
// Created by binlee on 2020/9/8.
//

#ifndef BASIC_PERSON_H
#define BASIC_PERSON_H

#include <string>

#ifndef MALE
#define MALE 1
#endif

#ifndef FEMALE
#define FEMALE 0
#endif

class Person {
private:
    std::string name;
    int age;
    int gender;

public:
    Person(std::string name, int age, int gender);

    ~Person();

    void speak() const;
};

void person_main();

#endif //BASIC_PERSON_H
