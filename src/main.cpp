#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QCommandLineParser>
#include <iostream>
#include "batterymanager.h"
#include "batterydevice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("LinuxPower");
    app.setApplicationName("battery-info");
    app.setApplicationDisplayName("Battery Info");
    app.setDesktopFileName("battery-info");

    QCommandLineParser parser;
    parser.setApplicationDescription("Lightweight, event-driven Linux battery information tool");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption cliOption(QStringList() << "c" << "cli" << "test",
                                 "Print battery diagnostics to stdout and exit.");
    parser.addOption(cliOption);

    QCommandLineOption themeOption(QStringList() << "t" << "theme",
                                   "Set persistent theme mode ('dark' or 'light').", "mode");
    parser.addOption(themeOption);
    parser.process(app);

    BatteryManager batteryManager;

    if (parser.isSet(themeOption)) {
        QString mode = parser.value(themeOption).trimmed().toLower();
        bool isDark = (mode == "dark" || mode == "true" || mode == "1");
        batteryManager.setDarkMode(isDark);
        std::cout << "Persistent theme set to: " << (isDark ? "Dark" : "Light") << std::endl;
        return 0;
    }

    if (parser.isSet(cliOption)) {
        std::cout << batteryManager.getDiagnosticSummary().toStdString() << std::endl;
        std::cout << "Power Supply: " << (batteryManager.onBattery() ? "On Battery" : "AC Connected") << std::endl;
        std::cout << "Total Devices Detected: " << batteryManager.batteryCount() << std::endl;
        std::cout << "Pure Event-Driven Monitoring: Active (0 timer wakeups)" << std::endl;
        return 0;
    }

    QQmlApplicationEngine engine;

    // Register BatteryDevice type so QML can work with its properties
    qmlRegisterUncreatableType<BatteryDevice>("BatteryApp", 1, 0, "BatteryDevice",
                                             "BatteryDevice cannot be created directly in QML");

    engine.rootContext()->setContextProperty("batteryManager", &batteryManager);

    const QUrl url(QStringLiteral("qrc:/qt/qml/BatteryApp/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(url);

    return app.exec();
}
