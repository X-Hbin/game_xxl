#include <QApplication>
#include "serverwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用信息
    app.setApplicationName("Match3 Server");
    app.setApplicationVersion("1.0.0");

    ServerWindow window;
    window.show();

    return app.exec();
}
