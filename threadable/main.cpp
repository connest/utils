#include <QCoreApplication>
#include <QTimer>
#include "can_used_in_slot.h"
#include "in_other_thread.h"
#include "worker.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << QThread::currentThreadId() << " - main thread " << std::endl;


    // Зарегистрировали свой тип для передачи его в слоты
    qRegisterMetaType<CanUsedInSlot>("CanUsedInSlot");

    // Собственно параметр
    CanUsedInSlot cuis;
    cuis.a = 190;



    // Объект, который будет в _отдельном_ потоке.
    Worker* wp = new Worker();

    // Вызов слота.
    // ВАЖНО: вызов происходит из этого потока, а _не_ в потоке объекта.
    // direct call
    // В обычной ситуации - такой вызов слота может привести к гонке данных
    // и хуже...
    wp->show(cuis);


    // Корректное удаление объекта: в _его_ потоке, _после_ всех принятых событий
    wp->deleteLater();

    // НЕВЕРНО!!! Нельзя делать delete над QObject в другом потоке
    // delete /*don't do it*/ wp;



    // Через секунду завершаем программу
    QTimer::singleShot(1000, [&](){
        std::cout << "exit ..." << std::endl;
        a.exit();
    });

    return a.exec();
}
