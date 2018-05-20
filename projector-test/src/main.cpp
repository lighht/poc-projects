#include <iostream>

#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <projector_test/motorstatescanner.h>
#include <projector_test/powermeterscanner.h>
#include <projector_test/datasource.h>
#include <projector_test/dummydatasource.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    #ifdef QT_DEBUG
        std::cout<<"Running in Debug mode with dummy data";
        DummyDataSource dataSource(&app);
        engine.rootContext()->setContextProperty("dataSource", &dataSource);
    #else
        DataSource dataSource(&app);
        engine.rootContext()->setContextProperty("dataSource", &dataSource);

    #endif
    engine.load(QUrl(QLatin1String("qrc:/src/main.qml")));
    return app.exec();
}
