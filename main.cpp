#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include <VLCQtCore/Common.h>

#include "Player.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("P2PSP GUI");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication app(argc, argv);
    //VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");

    SimplePlayer mainWindow;
    mainWindow.show();

    return app.exec();
}
