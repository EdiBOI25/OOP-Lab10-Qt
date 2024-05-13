#include "Lab10Qt.h"
#include <QtWidgets/QApplication>
#include "service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RepoWithFile repo{ "data.txt" };
    Service serv{ repo };

    Lab10Qt w;
    w.show();
    return a.exec();
}
