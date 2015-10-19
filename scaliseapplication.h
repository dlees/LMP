#ifndef SCALISEAPPLICATION_H
#define SCALISEAPPLICATION_H

#include <QApplication>

class ScaliseApplication : public QApplication
{
public:
    ScaliseApplication(int &argc, char ** argv);
private:
    bool notify(QObject *receiver_, QEvent *event_);
};
#endif // SCALISEAPPLICATION_H
