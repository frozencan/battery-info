#include "batterydevice.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMessage>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <cmath>

BatteryDevice::BatteryDevice(const QString &dbusPath, QObject *parent)
    : QObject(parent), m_path(dbusPath)
{
    m_lastEventTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_lastChangedProperty = "Initialized";

    // 100% Event-Driven: Connect directly to PropertiesChanged D-Bus signal
    bool connected = QDBusConnection::systemBus().connect(
        "org.freedesktop.UPower",
        m_path,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onDbusPropertiesChanged(QString, QVariantMap, QStringList))
    );

    if (!connected) {
        qWarning() << "Failed to connect to PropertiesChanged D-Bus signal for:" << m_path;
    }

    refreshInitialProperties();
}

BatteryDevice::~BatteryDevice()
{
    QDBusConnection::systemBus().disconnect(
        "org.freedesktop.UPower",
        m_path,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onDbusPropertiesChanged(QString, QVariantMap, QStringList))
    );
}

void BatteryDevice::refreshInitialProperties()
{
    QDBusInterface iface("org.freedesktop.UPower", m_path,
                         "org.freedesktop.DBus.Properties",
                         QDBusConnection::systemBus());

    if (!iface.isValid()) {
        qWarning() << "Interface invalid for path:" << m_path;
        return;
    }

    QDBusReply<QVariantMap> reply = iface.call("GetAll", "org.freedesktop.UPower.Device");
    if (reply.isValid()) {
        parseProperties(reply.value());
        readSysfsExtras();
        emit propertiesChanged();
    } else {
        qWarning() << "GetAll failed for" << m_path << ":" << reply.error().message();
    }
}

void BatteryDevice::onDbusPropertiesChanged(const QString &interfaceName,
                                           const QVariantMap &changedProperties,
                                           const QStringList &invalidatedProperties)
{
    Q_UNUSED(invalidatedProperties);
    if (interfaceName != "org.freedesktop.UPower.Device") {
        return;
    }

    m_lastEventTime = QDateTime::currentDateTime().toString("hh:mm:ss");

    QStringList changedKeys;
    for (auto it = changedProperties.cbegin(); it != changedProperties.cend(); ++it) {
        changedKeys.append(it.key());
        // Emit log signal for the first/key changed property
        emit propertyEventLogged(m_nativePath.isEmpty() ? m_path : m_nativePath,
                                 it.key(),
                                 it.value().toString());
    }

    if (!changedKeys.isEmpty()) {
        m_lastChangedProperty = changedKeys.join(", ");
    }

    parseProperties(changedProperties);
    readSysfsExtras();

    emit propertiesChanged();
}

void BatteryDevice::parseProperties(const QVariantMap &props)
{
    if (props.contains("NativePath"))
        m_nativePath = props.value("NativePath").toString();
    if (props.contains("Vendor"))
        m_vendor = props.value("Vendor").toString();
    if (props.contains("Model"))
        m_model = props.value("Model").toString();
    if (props.contains("Serial"))
        m_serial = props.value("Serial").toString();
    if (props.contains("Technology"))
        m_technology = props.value("Technology").toUInt();
    if (props.contains("State"))
        m_state = props.value("State").toUInt();
    if (props.contains("Percentage"))
        m_percentage = props.value("Percentage").toDouble();
    if (props.contains("Energy"))
        m_energy = props.value("Energy").toDouble();
    if (props.contains("EnergyFull"))
        m_energyFull = props.value("EnergyFull").toDouble();
    if (props.contains("EnergyFullDesign"))
        m_energyFullDesign = props.value("EnergyFullDesign").toDouble();
    if (props.contains("EnergyRate"))
        m_energyRate = props.value("EnergyRate").toDouble();
    if (props.contains("Voltage"))
        m_voltage = props.value("Voltage").toDouble();
    if (props.contains("VoltageMinDesign"))
        m_voltageMinDesign = props.value("VoltageMinDesign").toDouble();
    if (props.contains("Capacity"))
        m_capacity = props.value("Capacity").toDouble();
    if (props.contains("ChargeCycles"))
        m_cycleCount = props.value("ChargeCycles").toInt();
    if (props.contains("TimeToEmpty"))
        m_timeToEmpty = props.value("TimeToEmpty").toLongLong();
    if (props.contains("TimeToFull"))
        m_timeToFull = props.value("TimeToFull").toLongLong();
    if (props.contains("Temperature"))
        m_temperature = props.value("Temperature").toDouble();
    if (props.contains("WarningLevel"))
        m_warningLevel = props.value("WarningLevel").toUInt();
    if (props.contains("IconName"))
        m_iconName = props.value("IconName").toString();
    if (props.contains("IsPresent"))
        m_isPresent = props.value("IsPresent").toBool();
    if (props.contains("IsRechargeable"))
        m_isRechargeable = props.value("IsRechargeable").toBool();
}

void BatteryDevice::readSysfsExtras()
{
    if (m_nativePath.isEmpty()) return;

    QString basePath = QString("/sys/class/power_supply/%1/").arg(m_nativePath);

    // If cycle count is unknown (-1 or 0), attempt reading directly from sysfs
    if (m_cycleCount <= 0) {
        QFile cycleFile(basePath + "cycle_count");
        if (cycleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&cycleFile);
            bool ok = false;
            int c = in.readAll().trimmed().toInt(&ok);
            if (ok && c >= 0) {
                m_cycleCount = c;
            }
        }
    }

    // If temperature is reported as 0 by UPower, check sysfs temp
    if (std::abs(m_temperature) < 0.001) {
        QFile tempFile(basePath + "temp");
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&tempFile);
            bool ok = false;
            double t = in.readAll().trimmed().toDouble(&ok);
            if (ok && t > 0) {
                // sysfs temp is usually tenths of degree Celsius (e.g. 300 = 30.0 C)
                if (t > 100) t /= 10.0;
                m_temperature = t;
            }
        }
    }
}

QString BatteryDevice::technology() const
{
    switch (m_technology) {
    case 1: return "Lithium-ion (Li-ion)";
    case 2: return "Lithium Polymer (Li-poly)";
    case 3: return "Lithium Iron Phosphate (LiFePO4)";
    case 4: return "Lead Acid";
    case 5: return "Nickel Cadmium (NiCd)";
    case 6: return "Nickel Metal Hydride (NiMH)";
    default: return "Unknown";
    }
}

QString BatteryDevice::stateString() const
{
    switch (m_state) {
    case 1: return "Charging";
    case 2: return "Discharging";
    case 3: return "Empty";
    case 4: return "Fully Charged";
    case 5: return "Plugged In (Pending Charge)";
    case 6: return "Pending Discharge";
    default: return "Idle / Connected";
    }
}

QString BatteryDevice::stateColor() const
{
    switch (m_state) {
    case 1: return "#10b981"; // Emerald Green
    case 2:
        if (m_percentage <= 15.0) return "#ef4444"; // Rose Red
        if (m_percentage <= 30.0) return "#f59e0b"; // Amber
        return "#38bdf8"; // Sky Blue
    case 3: return "#ef4444"; // Red
    case 4: return "#059669"; // Deep Emerald
    case 5: return "#a855f7"; // Purple / Plugged In
    case 6: return "#f59e0b"; // Amber
    default: return "#94a3b8"; // Slate Gray
    }
}

double BatteryDevice::capacityHealth() const
{
    if (m_energyFullDesign > 0.0 && m_energyFull > 0.0) {
        return std::min(100.0, (m_energyFull / m_energyFullDesign) * 100.0);
    }
    return m_capacity > 0.0 ? m_capacity : 100.0;
}

QString BatteryDevice::timeRemainingString() const
{
    if (m_state == 1 && m_timeToFull > 0) {
        qint64 hours = m_timeToFull / 3600;
        qint64 mins = (m_timeToFull % 3600) / 60;
        if (hours > 0)
            return QString("%1h %2m until full").arg(hours).arg(mins);
        return QString("%1m until full").arg(mins);
    }

    if (m_state == 2 && m_timeToEmpty > 0) {
        qint64 hours = m_timeToEmpty / 3600;
        qint64 mins = (m_timeToEmpty % 3600) / 60;
        if (hours > 0)
            return QString("%1h %2m remaining").arg(hours).arg(mins);
        return QString("%1m remaining").arg(mins);
    }

    if (m_state == 4) {
        return "Fully Charged";
    }

    if (m_state == 5) {
        return "Plugged in (Conservation Mode or Full)";
    }

    if (m_state == 2 && m_timeToEmpty == 0) {
        return "Calculating remaining time...";
    }

    return "AC Connected";
}

QString BatteryDevice::warningLevel() const
{
    switch (m_warningLevel) {
    case 1: return "None";
    case 2: return "Discharging";
    case 3: return "Low";
    case 4: return "Critical";
    case 5: return "Action Required";
    default: return "Normal";
    }
}

QString BatteryDevice::toMarkdownReport() const
{
    QString report;
    QTextStream out(&report);
    out << "### Battery Details (" << (m_nativePath.isEmpty() ? m_path : m_nativePath) << ")\n\n"
        << "| Metric | Value |\n"
        << "| :--- | :--- |\n"
        << "| **Model** | " << m_model << " |\n"
        << "| **Vendor** | " << m_vendor << " |\n"
        << "| **Serial** | " << m_serial << " |\n"
        << "| **Technology** | " << technology() << " |\n"
        << "| **State** | " << stateString() << " |\n"
        << "| **Percentage** | " << QString::number(m_percentage, 'f', 1) << "% |\n"
        << "| **Health** | " << QString::number(capacityHealth(), 'f', 1) << "% |\n"
        << "| **Energy Remaining** | " << QString::number(m_energy, 'f', 2) << " Wh |\n"
        << "| **Energy Full** | " << QString::number(m_energyFull, 'f', 2) << " Wh |\n"
        << "| **Design Capacity** | " << QString::number(m_energyFullDesign, 'f', 2) << " Wh |\n"
        << "| **Voltage** | " << QString::number(m_voltage, 'f', 2) << " V |\n"
        << "| **Min Design Voltage** | " << QString::number(m_voltageMinDesign, 'f', 2) << " V |\n"
        << "| **Power Rate** | " << QString::number(m_energyRate, 'f', 2) << " W |\n"
        << "| **Cycle Count** | " << (m_cycleCount >= 0 ? QString::number(m_cycleCount) : "N/A") << " |\n"
        << "| **Time Remaining** | " << timeRemainingString() << " |\n"
        << "| **Monitoring Mode** | Event-Driven (UPower D-Bus, Zero Polling) |\n";
    return report;
}
