#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QDBusConnection>
#include <QDateTime>

class BatteryDevice : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(QString nativePath READ nativePath NOTIFY propertiesChanged)
    Q_PROPERTY(QString vendor READ vendor NOTIFY propertiesChanged)
    Q_PROPERTY(QString model READ model NOTIFY propertiesChanged)
    Q_PROPERTY(QString serial READ serial NOTIFY propertiesChanged)
    Q_PROPERTY(QString technology READ technology NOTIFY propertiesChanged)
    Q_PROPERTY(int state READ state NOTIFY propertiesChanged)
    Q_PROPERTY(QString stateString READ stateString NOTIFY propertiesChanged)
    Q_PROPERTY(QString stateColor READ stateColor NOTIFY propertiesChanged)
    Q_PROPERTY(double percentage READ percentage NOTIFY propertiesChanged)
    Q_PROPERTY(double energy READ energy NOTIFY propertiesChanged)
    Q_PROPERTY(double energyFull READ energyFull NOTIFY propertiesChanged)
    Q_PROPERTY(double energyFullDesign READ energyFullDesign NOTIFY propertiesChanged)
    Q_PROPERTY(double energyRate READ energyRate NOTIFY propertiesChanged)
    Q_PROPERTY(double voltage READ voltage NOTIFY propertiesChanged)
    Q_PROPERTY(double voltageMinDesign READ voltageMinDesign NOTIFY propertiesChanged)
    Q_PROPERTY(double capacityHealth READ capacityHealth NOTIFY propertiesChanged)
    Q_PROPERTY(int cycleCount READ cycleCount NOTIFY propertiesChanged)
    Q_PROPERTY(qint64 timeToEmpty READ timeToEmpty NOTIFY propertiesChanged)
    Q_PROPERTY(qint64 timeToFull READ timeToFull NOTIFY propertiesChanged)
    Q_PROPERTY(QString timeRemainingString READ timeRemainingString NOTIFY propertiesChanged)
    Q_PROPERTY(double temperature READ temperature NOTIFY propertiesChanged)
    Q_PROPERTY(QString warningLevel READ warningLevel NOTIFY propertiesChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY propertiesChanged)
    Q_PROPERTY(bool isPresent READ isPresent NOTIFY propertiesChanged)
    Q_PROPERTY(bool isRechargeable READ isRechargeable NOTIFY propertiesChanged)
    Q_PROPERTY(QString lastEventTime READ lastEventTime NOTIFY propertiesChanged)
    Q_PROPERTY(QString lastChangedProperty READ lastChangedProperty NOTIFY propertiesChanged)

public:
    explicit BatteryDevice(const QString &dbusPath, QObject *parent = nullptr);
    ~BatteryDevice() override;

    QString path() const { return m_path; }
    QString nativePath() const { return m_nativePath; }
    QString vendor() const { return m_vendor; }
    QString model() const { return m_model; }
    QString serial() const { return m_serial; }
    QString technology() const;
    int state() const { return m_state; }
    QString stateString() const;
    QString stateColor() const;
    double percentage() const { return m_percentage; }
    double energy() const { return m_energy; }
    double energyFull() const { return m_energyFull; }
    double energyFullDesign() const { return m_energyFullDesign; }
    double energyRate() const { return m_energyRate; }
    double voltage() const { return m_voltage; }
    double voltageMinDesign() const { return m_voltageMinDesign; }
    double capacityHealth() const;
    int cycleCount() const { return m_cycleCount; }
    qint64 timeToEmpty() const { return m_timeToEmpty; }
    qint64 timeToFull() const { return m_timeToFull; }
    QString timeRemainingString() const;
    double temperature() const { return m_temperature; }
    QString warningLevel() const;
    QString iconName() const { return m_iconName; }
    bool isPresent() const { return m_isPresent; }
    bool isRechargeable() const { return m_isRechargeable; }
    QString lastEventTime() const { return m_lastEventTime; }
    QString lastChangedProperty() const { return m_lastChangedProperty; }

    void refreshInitialProperties();
    QString toMarkdownReport() const;

public slots:
    void onDbusPropertiesChanged(const QString &interfaceName,
                                 const QVariantMap &changedProperties,
                                 const QStringList &invalidatedProperties);

signals:
    void propertiesChanged();
    void propertyEventLogged(const QString &devicePath, const QString &propertyName, const QString &valueStr);

private:
    void parseProperties(const QVariantMap &props);
    void readSysfsExtras();

    QString m_path;
    QString m_nativePath;
    QString m_vendor;
    QString m_model;
    QString m_serial;
    uint m_technology = 0;
    uint m_state = 0;
    double m_percentage = 0.0;
    double m_energy = 0.0;
    double m_energyFull = 0.0;
    double m_energyFullDesign = 0.0;
    double m_energyRate = 0.0;
    double m_voltage = 0.0;
    double m_voltageMinDesign = 0.0;
    double m_capacity = 100.0;
    int m_cycleCount = -1;
    qint64 m_timeToEmpty = 0;
    qint64 m_timeToFull = 0;
    double m_temperature = 0.0;
    uint m_warningLevel = 0;
    QString m_iconName;
    bool m_isPresent = true;
    bool m_isRechargeable = true;
    QString m_lastEventTime;
    QString m_lastChangedProperty;
};
