# Singleton

Класс-wrapper, делающий внутренний класс singleton'ом.

## Мотивация

Часто используется паттерн singleton.

Это повторяющаяся функциональность.
Нужно вынести её в отдельный класс.

Требования:

- предусмотреть возможность вызова разных конструкторов класса;
- добавить визуальной семантики.

## Пример

```
auto s1 = Singleton<MySingletonClass>::instance("hello");
auto s  = Singleton<MySingletonClass>::instance();

assert(s == s1); // true
```

## Вывод на экран

```
MySingletonClass::MySingletonClass(std::string)
```

Замечания:

- вызван _только_ один конструктор
- _не_ вызван деструктор (баг?)

## TODO

Сейчас C++17, так как используются inline variables - попытаться понизить требования до C++11.