#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "controller/SensorController.h"
#include "model/SensorData.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    SensorData sensorData;
    SensorController sensorController(&sensorData);

    qmlRegisterSingletonInstance("SensorController", 1, 0, "SensorController", &sensorController);

    engine.rootContext()->setContextProperty("sensorController", &sensorController);

    const QUrl url(QStringLiteral("qrc:/src/view/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
                     {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
