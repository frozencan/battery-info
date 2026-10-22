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
                text: "📈"
                font.pixelSize: 20
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "DISCHARGE & CHARGE HISTORY"
            font.family: Theme.fontFamily
            font.pixelSize: 11
            font.weight: Font.Bold
            font.letterSpacing: Theme.labelLetterSpacing
            color: Theme.secondary
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Historical Power Analytics"
            font.family: Theme.fontFamily
            font.pixelSize: 22
            font.weight: Font.Bold
            color: Theme.textOnSurface
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            text: "Historical rate curves, voltage variance intervals, and thermal degradation logs will be curated in this gallery spread."
            font.family: Theme.fontFamily
            font.pixelSize: 13
            color: Theme.textOnSurfaceVariant
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }
}
