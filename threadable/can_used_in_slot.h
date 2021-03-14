#ifndef CAN_USED_IN_SLOT_H
#define CAN_USED_IN_SLOT_H

#include <QMetaType>

/**
 * @brief The CanUsedInSlot class
 * Некий класс, способный передаваться в слоты
 */
class CanUsedInSlot
{
public:
    int a{7};
};

// Его нужно зарегистировать
Q_DECLARE_METATYPE(CanUsedInSlot)



#endif // CAN_USED_IN_SLOT_H
