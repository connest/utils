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
        std::cout << QThread::currentThreadId() << " Init in main thread..." << std::endl;;

        // Если нужна инициализация внутри потока - подключить слот
        connect(getThread(),    &QThread::started,
                this,           &Worker::initInStardedThread);

        getThread()->start();
    }
    ~Worker()
    {
        std::cout << QThread::currentThreadId() << " die" << std::endl;
    }
    void func(const QString& arr) {
        std::cout << QThread::currentThreadId() << " " << arr.toStdString() << std::endl;
    }

public slots:

    /**
     * @brief show Слот для примера
     * @param c параметр, зарегистрированного метатипа
     */
    void show(CanUsedInSlot c){

        std::cout << QThread::currentThreadId() << " call slot ..." << std::endl;

        if(invokeIfOtherThread("show", c))
            return ;

        std::cout << QThread::currentThreadId() << " call slot in right thread" << std::endl;


        //Какие-то действия в слоте...

        func(QString::number(c.a));
    }


private slots:
     /**
      * @brief initInStardedThread
      * Дополнительная инициализация _после_ перемещения в поток.
      * Например, для создания сокетов и т.п.
      */
     void initInStardedThread() {
         std::cout << QThread::currentThreadId() << " Init in thread..." << std::endl;
     }
};

#endif // WORKER_H
