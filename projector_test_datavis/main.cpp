#include <memory>
#include <datareader.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include <QtCharts/QLineSeries>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE
int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QQmlApplicationEngine engine;
    DataReader dataReader(&app);
    dataReader.setFileName(QString(argv[1]));
    engine.rootContext()->setContextProperty("dataReader", &dataReader);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
