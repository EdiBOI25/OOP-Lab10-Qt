#include "Lab10Qt.h"
#include <QtWidgets/QApplication>

#include "GUI.h"
#include "service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RepoWithFile repo{ "data.txt" };
    Service serv{ repo };

    GUI w{ serv };
    w.show();


    return a.exec();
}
