#ifndef RENAME_H
#define RENAME_H

#include "network/networkmanager.h"
#include "operation.h"
#include "network/queries.h"
#include "gui/items.h"

class Rename : public NetworkManager, public Operation
{
    Q_OBJECT
public:
    Rename(QObject *parent = 0);

protected slots:
    void slotPutFinished(void);

public:
    void item(const Items::Data &source, const QString &newName);

private:
    Queries queries;
};

#endif // RENAME_H