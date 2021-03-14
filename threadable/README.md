# InOtherThread

Базовый класс семантики "один объект - один поток".
Корректно завершает QThread в деструкторе.
Имеет реализацию invokeIfOtherThread - вызов слота, если поток выполнения
не тот, которым он владеет

## Мотивация

В Qt иногда необходимо делать некоторые вещи в отдельном потоке. При этом очень
важно, чтобы слот был вызван именно через Qt::QueuedConnection. Например,
при работе с сокетами.

Однако, в реальном мире клиентский код так не поступает, поэтому нужна защита.

Вариант создавать для этого отдельный сигнал на каждый слот - не красив. Он не
только создает лишний копирующийся из раза в раз код (при копировании велик
шанс ошибки), но и способствует "протиканию" абстракции
(сигналы видны клиентскому коду). 

Решение: вынести такой код выше по иерархии так, чтобы его не нужны было писать
каждый раз заново и он был бы невидим клиентскому коду.

## Пример

### Worker

```
class Worker  : public QObject
              , protected InOtherThread <---- наследуемся  от InOtherThread
{
    Q_OBJECT
public:
    Worker()
        : QObject(nullptr)
        , InOtherThread(this /* , false */) <---- передаем себя в поток выполения
                                 ^^^^^^ - не стартовать поток пока мы не попросим
    {
        std::cout << QThread::currentThreadId() << " Init in main thread..." << std::endl;;

        // Если нужна инициализация внутри потока - подключить слот
        connect(getThread(),    &QThread::started,
                this,           &Worker::initInStardedThread);

        // И начать выполнение
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

        <---- один поток ...
        
                                      vvv - параметры слота
        if(invokeIfOtherThread("show", c))
                               ^^^^^ - название слота
            return ;

        <---- уже другой поток
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
```

### Использование 

```
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
```

### Вывод на экран

```

 ПОТОК (адрес)               СООБЩЕНИЕ
0x7eff71845840 - main thread 
0x7eff71845840 Init in main thread...
0x7eff7155c640 Init in thread...

0x7eff71845840 call slot ... <---- вызов в _неправильном_ потоке

0x7eff7155c640 call slot ... <---- вызвали еще раз, но уже в правильном
0x7eff7155c640 call slot in right thread

0x7eff7155c640 190

0x7eff7155c640 die <---- вызвали deleteLater() - удаление в _нужном_ потоке

```
