import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import BatteryApp

Item {
    id: root

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16
        width: Math.min(parent.width - 48, 480)

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 48
            height: 48
            radius: 0
            color: Theme.surfaceContainerHighest

            Text {
                anchors.centerIn: parent
                text: "⚙️"
                font.pixelSize: 20
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "LOW-LEVEL D-BUS & SYSFS"
            font.family: Theme.fontFamily
            font.pixelSize: 11
            font.weight: Font.Bold
            font.letterSpacing: Theme.labelLetterSpacing
            color: Theme.secondary
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Raw Diagnostics Inspector"
            font.family: Theme.fontFamily
            font.pixelSize: 22
            font.weight: Font.Bold
            color: Theme.textOnSurface
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            text: "Live UPower netlink events, ACPI hardware descriptors, and direct `/sys/class/power_supply` attribute telemetry."
            font.family: Theme.fontFamily
            font.pixelSize: 13
            color: Theme.textOnSurfaceVariant
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }
}
