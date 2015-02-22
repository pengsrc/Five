#include "Five.h"
#include <QApplication>

int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    Five w;
    w.show();
    
    return a.exec();
}
