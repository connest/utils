#ifndef WORKER_H
#define WORKER_H


#include "in_other_thread.h"
#include "can_used_in_slot.h"

#include <iostream>
class Worker  : public QObject
              , protected InOtherThread
{
    Q_OBJECT
public:
    Worker()
        : QObject(nullptr)
        , InOtherThread(this)
    {

        // Если нужна инициализация внутри потока - подключить слот
        connect(getThread(),    &QThread::started,
                this,           &Worker::initInStardedThread);

        getThread()->start();
    }
    ~Worker()
    {
    }
    void func(const QString& arr) {
        std::cout << " ";
    }

public slots:

    /**
     * @brief show Слот для примера
     * @param c параметр, зарегистрированного метатипа
     */
    void show(CanUsedInSlot c)
    {
        if(invokeIfOtherThread(&Worker::show, c))
            return ;

        // Выполняется _нужный_ поток
        assert(QThread::currentThread() == this->thread());

        //Какие-то действия в слоте...

        func(QString::number(c.a));
    }


    // Работа с перегружеными слотами

    /**
     * @brief func_for_overload Слот который имеет перегрузку по параметрам
     * @param a параметр тип 1
     */
    void func_for_overload(int a) {
        //                              vvvvv - перечисление типов параметров функции
        if(invokeIfOtherThread(QOverload<int>::of(&Worker::func_for_overload), a))
            return ;

        // Выполняется _нужный_ поток
        assert(QThread::currentThread() == this->thread());

        //действия 1
    }

    /**
     * @brief func_for_overload Слот который имеет перегрузку по параметрам
     * @param b параметр тип 2
     */
    void func_for_overload(QByteArray b)
    {

        //                              vvvvvvvvvvvv - перечисление типов параметров функции
        if(invokeIfOtherThread(QOverload<QByteArray>::of(&Worker::func_for_overload), b))
            return ;

        // Выполняется _нужный_ поток
        assert(QThread::currentThread() == this->thread());

        //действия 2
    }

    /**
     * @brief func_for_overload Слот который имеет перегрузку по параметрам
     * @param a параметр тип 1
     * @param b параметр тип 3
     */
    void func_for_overload(int a, double b)
    {

        //                              vvvvvvvvvvvv - перечисление типов параметров функции
        if(invokeIfOtherThread(QOverload<int, double>::of(&Worker::func_for_overload), a, b))
            return ;

        // Выполняется _нужный_ поток
        assert(QThread::currentThread() == this->thread());

        //действия 3
    }

private slots:
     /**
      * @brief initInStardedThread
      * Дополнительная инициализация _после_ перемещения в поток.
      * Например, для создания сокетов и т.п.
      */
     void initInStardedThread() {
     }
};

#endif // WORKER_H
