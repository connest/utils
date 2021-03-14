#ifndef IN_OTHER_THREAD_H
#define IN_OTHER_THREAD_H

#include <QObject>
#include <QThread>
#include <type_traits>


/**
 * @brief The InOtherThread class
 * Базовый класс семантики "один объект - один поток".
 * Корректно завершает QThread в деструкторе.
 * Имеет реализацию invokeIfOtherThread - вызов слота, если поток выполнения
 * не тот, которым он владеет
 */
class InOtherThread
{
    QThread* m_thread;
    QObject* m_obj;

public:
    /**
     * @brief InOtherThread
     *
     * Конструктор класса InOtherThread
     *
     * @param object                указатель на объект,
     *                              который будет помещен в отдельный поток
     *
     * @param startInConstructor    запускать ли QThread сразу
     */
    InOtherThread(QObject* object, bool startInConstructor = false)
        : m_thread {new QThread()}
        , m_obj {object}
    {
        m_obj->moveToThread(m_thread);

        if(startInConstructor)
            m_thread->start();

    }
    ~InOtherThread() {
        m_thread->quit();
        m_thread->requestInterruption();

        if(QThread::currentThread() == m_thread) {
            m_thread->deleteLater();

        } else {
            // Если завершаем соседний
            // (кто-то все таки вызвал delete, а не deleteLater() у объекта)
            // 3 раза сделать quit с интервалом 200 мс.
            // Если не вышло - terminate

            for(int i = 0; i < 3; ++i) {
                if(m_thread->isFinished()) {
                    m_thread->deleteLater();
                    return;
                }

                m_thread->wait(200);
            }
            m_thread->terminate();
            m_thread->wait(); // согласно документации необходимо ожидание
        }

    }

protected:
    /**
     * @brief getThread Получить указатель на поток выполения
     * @return указатель на поток выполения
     */
    QThread* getThread() {
        return m_thread;
    }

    /**
     * @brief invokeIfOtherThread
     *
     * Вызов слота в потоке, которым владеет InOtherThread,
     * если текущий не совпадает с ним
     *
     * @param member    название слота
     * @param args      параметры слота
     * @return          флаг необходимости завершить выполнение.
     *                  (То есть текущий поток выполнения не равен нашему)
     */
    template<typename ... Args>
    bool invokeIfOtherThread(const char* member, Args&&... args){
        if(QThread::currentThread() != m_thread) {

            QMetaObject::invokeMethod(m_obj,
                                      member,
                                      Qt::QueuedConnection,
                                      QArgument<typename std::decay<Args>::type>
                                            (qtypeToString<typename std::decay<Args>::type>(),
                                            std::forward<Args>(args))...);
            return true;
        }

        return false;
    }

private:

    /**
     * @brief qtypeToString Получить строковое название метатипа
     * @return строковое название метатипа
     */
    template<typename QtType>
    const char* qtypeToString() {
        const int id = qMetaTypeId<QtType>();
        return QMetaType::typeName(id);
    }
};


#endif // IN_OTHER_THREAD_H
