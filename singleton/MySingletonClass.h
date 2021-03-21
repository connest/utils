#ifndef MYSINGLETONCLASS_H
#define MYSINGLETONCLASS_H

#include <string>
#include <iostream>

/**
 * @brief The MySingletonClass class
 * Класс, который хотим сделать singleton.
 * ВАЖНО:   у класса есть _несколько_ конструкторов
 *          в том числе по умолчанию.
 */
struct MySingletonClass {
    std::string str{};

    MySingletonClass() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        str = "default";
    }
    MySingletonClass(std::string s)
        : str{ std::move(s) }
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ~MySingletonClass() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

};

#endif // MYSINGLETONCLASS_H
