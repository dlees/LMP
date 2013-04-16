#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QMessageBox>

struct Error
{
    Error(QString msg_) : msg(msg_) {}

    QString msg;

    void print_error_msg()
    {
        QMessageBox error_msg;
        error_msg.setText(msg);
        error_msg.exec();
    }

    void print_error_msg_str(QString message)
    {
        QMessageBox error_msg;
        error_msg.setText(message);
        error_msg.exec();
    }
};

#endif
