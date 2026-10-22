#pragma once

#include <QObject>
#include <QList>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include "batterydevice.h"

class BatteryManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> batteries READ batteriesList NOTIFY batteriesChanged)
    Q_PROPERTY(BatteryDevice* primaryBattery READ primaryBattery NOTIFY primaryBatteryChanged)
    Q_PROPERTY(BatteryDevice* currentBattery READ currentBattery NOTIFY selectedIndexChanged)
    Q_PROPERTY(int batteryCount READ batteryCount NOTIFY batteriesChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(bool onBattery READ onBattery NOTIFY onBatteryChanged)
    Q_PROPERTY(bool acConnected READ acConnected NOTIFY onBatteryChanged)
    Q_PROPERTY(QString lastEventLog READ lastEventLog NOTIFY lastEventLogChanged)
    Q_PROPERTY(int eventCount READ eventCount NOTIFY lastEventLogChanged)
    Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged)

public:
    explicit BatteryManager(QObject *parent = nullptr);
    ~BatteryManager() override;

    bool darkMode() const { return m_darkMode; }
    void setDarkMode(bool dark);

    QList<QObject*> batteriesList() const;
    BatteryDevice* primaryBattery() const;
    BatteryDevice* currentBattery() const;
    int batteryCount() const { return m_devices.size(); }
    int selectedIndex() const { return m_selectedIndex; }
    void setSelectedIndex(int index);

    bool onBattery() const { return m_onBattery; }
    bool acConnected() const { return !m_onBattery; }
    QString lastEventLog() const { return m_lastEventLog; }
    int eventCount() const { return m_eventCount; }

    Q_INVOKABLE void copyDetailsToClipboard();
    Q_INVOKABLE QString getDiagnosticSummary() const;

public slots:
    void onDeviceAdded(const QDBusObjectPath &path);
    void onDeviceRemoved(const QDBusObjectPath &path);
    void onManagerPropertiesChanged(const QString &interfaceName,
                                   const QVariantMap &changedProperties,
                                   const QStringList &invalidatedProperties);
    void onBatteryEventLogged(const QString &devicePath, const QString &propertyName, const QString &valueStr);

signals:
    void batteriesChanged();
    void primaryBatteryChanged();
    void selectedIndexChanged();
    void onBatteryChanged();
    void lastEventLogChanged();
    void darkModeChanged();
    void toastRequested(const QString &message);

private:
    void enumerateDevices();
    void queryGlobalStatus();
    bool isBatteryDevice(const QString &path);

    QList<BatteryDevice*> m_devices;
    int m_selectedIndex = 0;
    bool m_onBattery = false;
    bool m_darkMode = false;
    QString m_lastEventLog = "Waiting for initial hardware event...";
    int m_eventCount = 0;
};
