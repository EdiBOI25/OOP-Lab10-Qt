#include "Lab10Qt.h"
#include <QtWidgets/QApplication>

#include "GUI.h"
#include "service.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RepoWithFile repo{ "data.txt" };
    Service serv{ repo };

    /* LA OBSERVER:
     * Service e cel care transmite notificarea (ObservableObject)
     * Tot service tine minte lista de abonati (de observeri)
     * CartGUI-urile sunt cele care primesc notificarile si se actualizeaza (Observer)
     */

    GUI w{ serv };
    w.show();


    return a.exec();
}
