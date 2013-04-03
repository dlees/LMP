#ifndef ERROR_H
#define ERROR_H

#include <QString>

struct Error
{
    Error(QString msg_) : msg(msg_) {}

    QString msg;
};

#endif
