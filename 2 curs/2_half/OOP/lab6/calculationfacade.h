#ifndef CALCULATIONFACADE_H
#define CALCULATIONFACADE_H

#include <QObject>
#include <estate.h>

class calculationfacade : public QObject
{
    Q_OBJECT
public:
    explicit calculationfacade(QObject *parent = nullptr);

    static int getCost(Estate *value);
signals:
};

#endif // CALCULATIONFACADE_H
