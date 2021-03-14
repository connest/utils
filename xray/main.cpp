/**
  * Проверка работы std::tie
  */
#include <iostream>
#include <tuple>
#include "xray.h"
using namespace std;

/**
 * @brief retTuple Создать std::tuple и вернуть его
 * @return std::tuple содержищий xray
 */
std::tuple<xray, int, double> retTuple() {

    std::cout << "======= make_tuple =======" << std::endl;
    auto t = std::make_tuple(44, 34, 3.9);

    std::cout << "======= return =======" << std::endl;
    return t;
}

int main()
{
    xray x{0};
    int i;
    double d;

    // Получение возвращаемого кортежа по ссылкам
    std::tie(x, i, d) = retTuple();

    /*
        Вывод на консоль:
        xray(int) 0
        ======= make_tuple =======
        ======= return =======
        xray(int) 44
        xray& operator=(xray&&) 44 to 0 <-- Перемещение
        ~xray() 0
        ~xray() 44

        Вывод:
        std::tie создает кортеж _ссылок_ на созданные объекты.
        После чего происходит поэлементное присваивание rvalue в lvalue&
        (т.е. перемещением)
    */

    return 0;
}
