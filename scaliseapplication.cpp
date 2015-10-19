#include "scaliseapplication.h"
#include "Error.h"

#include <Qt>
#include <iostream>

ScaliseApplication::ScaliseApplication(int &argc, char ** argv) :
    QApplication(argc, argv)
{
}

bool ScaliseApplication::notify(QObject *receiver, QEvent *event)
{/*
    try {
        return QApplication::notify( receiver, event );
    } catch ( std::exception& e ) {
        Error::print_error_msg_str( e.what() );
        std::cerr << e.what() ;
        return false;
    }*/
    bool done = true;
    try {
      done = QApplication::notify(receiver, event);
    } catch (const std::exception& e) {
        Error::print_error_msg_str( e.what() );
        std::cerr << e.what() << endl;

    } catch (Error &e) {
        e.print_error_msg();

    }
    return done;
}
