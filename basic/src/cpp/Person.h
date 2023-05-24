//
// Created by binlee on 2020/9/8.
//

#ifndef BASIC_PERSON_H
#define BASIC_PERSON_H

#include <string>
#include <iostream>

#ifndef MALE
#define MALE 1
#endif

#ifndef FEMALE
#define FEMALE 0
#endif

class Person {
public:
    virtual void speak() = 0;

    virtual std::string toString() = 0;
};

class Ben : public Person {

private:
    std::string mName;
    int mAge;
    int mGender;

public:
    Ben(std::string name, int age, int gender);

    std::string getName() {
        return mName;
    }

    int getAge() const {
        return mAge;
    }

    int getGender() const {
        return mGender;
    }

    void speak() override;

    std::string toString() override;

    ~Ben() {
        mName = "";
        mAge = 0;
        mGender = 0;
        std::cout << "Ben's destructor is invoked." << std::endl;
    }
};

void person_main();

#endif //BASIC_PERSON_H
