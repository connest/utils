#include <QCoreApplication>
#include <QTimer>
#include "can_used_in_slot.h"
#include "in_other_thread.h"
#include "worker.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Зарегистрировали свой тип для передачи его в слоты
    qRegisterMetaType<CanUsedInSlot>("CanUsedInSlot");

    // Собственно параметр
    CanUsedInSlot cuis;
    cuis.a = 190;



    // Объект, который будет в _отдельном_ потоке.
    Worker* wp = new Worker();

    // Вызов слота.
    // ВАЖНО: вызов происходит из этого потока, а _не_ в потоке объекта.
    // direct call -> queued call
    // В обычной ситуации - такой вызов слота может привести к гонке данных
    // и хуже...

    for(int i=0;i<10000;i++)
        wp->show(cuis);




    QTimer::singleShot(200, [&]() {

        // TODO: Fix "QThread: Destroyed while thread is still running"
        // Корректное удаление объекта: в _его_ потоке, _после_ всех принятых событий
        wp->deleteLater();

        QTimer::singleShot(200, [&]() {
            qDebug() << "exit";

            a.exit();

        });
    });

    return a.exec();
}
