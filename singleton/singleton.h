#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <atomic>
#include <type_traits>

/**
 * @brief The Singleton class
 * Класс-wrapper, делающий внутренний класс singleton'ом
 */
template<typename T>
class Singleton
{
    static_assert(std::is_default_constructible<T>::value,
                  "T must have default constructor");

    // TODO: inline variable - C++17 only
    inline static std::atomic<T*> m_instance;
    inline static std::mutex m_mutex;

public:
    /// Disable construct
    Singleton()  = delete;
    ~Singleton() = delete;

    /**
     * @brief instance
     *
     * Создание экземпляра класса.
     *
     * Можно использовать конструктор _не_ по умолчанию. Будет создан
     * единственный экзепляр.
     * Далее можно получать указатель на объект с помощью instance()
     *
     * @param args параметры конструктора
     * @return единственный глобальный объект класса T
     */
    template<typename ... Args>
    static T* instance(Args&& ... args)
    {
        T* tmp = m_instance.load(std::memory_order_acquire);

        if(tmp == nullptr) {
            std::lock_guard<std::mutex> locker(m_mutex);
            tmp = m_instance.load(std::memory_order_acquire);

            if(tmp == nullptr) {
                // strong exception guarantee
                // если конструктор вызовет исключение - не устанавливать
                // указатель на объект

                tmp = new T{std::forward<Args>(args)...};
                m_instance.store(tmp, std::memory_order_release);
            }
        }

        return tmp;
    }
};

#endif // SINGLETON_H
