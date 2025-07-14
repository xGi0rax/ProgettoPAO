#include <QApplication>
#include <QIcon>
#include "Codice/GUI/Headers/mainWindow.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/Immagini/icon1.png"));

    MainWindow window;
    window.show();
    
    return app.exec();
}