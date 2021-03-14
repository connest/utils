#ifndef XRAY_H
#define XRAY_H

#include <iostream>

/**
 * @brief The xray class
 * Класс, показывающий поведение при копировании/перемещении.
 * Используется для устранения неоднозначностей в lvalue/rvalue выражениях
 * и конструкциях.
 */

class xray {
    /// Параметр, чтобы отличать объекты друг от друга
    int m_x;
public:
    /**
     * @brief xray Конструктор по умолчанию
     */
    xray()
        : m_x{0}
    {
        std::cout << "xray()" << std::endl;
    }

    /**
     * @brief xray Конструктор с параметрами
     * @param x параметр
     */
    xray(int x)
        : m_x{x}
    {
        std::cout << "xray(int) " << x << std::endl;
    }

    /**
     * @brief xray Конструктор копирования
     * @param other копируемый объект
     */
    xray(const xray& other)
        // Намерено нет initialization list, так как значение m_x
        // понадобится в выводе на экран
    {
        std::cout   << "xray(const xray&) "
                    << other.m_x << " to " << m_x << std::endl;

        m_x =  other.m_x;
    }

    /**
     * @brief xray Конструктор перемещения
     * @param other перемещаемый объект
     */
    xray(xray&& other) noexcept
        // Намерено нет initialization list, так как значение m_x
        // понадобится в выводе на экран
    {
        std::cout << "xray(xray&&) " << other.m_x << " to " << m_x << std::endl;

        std::swap(m_x, other.m_x);
    }

    /**
      * @brief ~xray() Деструктор
      */
    ~xray() {
        std::cout << "~xray() " << m_x << std::endl;
    }

    /**
     * @brief operator = Оператор присваивания (копирующий)
     * @param other копируемый объект
     * @return ссылка на экземпляр
     */
    xray& operator=(const xray& other) {
        std::cout   << "xray& operator=(const xray&) "
                    << other.m_x << " to " << m_x << std::endl;

        m_x =  other.m_x;
        return *this;
    }

    /**
     * @brief operator = Оператор присваивания (перемещающий)
     * @param other перемещаемый объект
     * @return ссылка на экземпляр
     */
    xray& operator=(xray&& other) {
        std::cout   << "xray& operator=(xray&&) "
                    << other.m_x << " to " << m_x << std::endl;

        std::swap(m_x, other.m_x);
        return *this;
    }
};

#endif // XRAY_H
