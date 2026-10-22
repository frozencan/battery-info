import QtQuick
import QtQuick.Layouts
import BatteryApp

Rectangle {
    id: root

    property string label: ""
    property string value: ""
    property string subValue: ""

    Layout.fillWidth: true
    implicitHeight: 110
    radius: 0
    color: Theme.surfaceContainerLowest
    border.color: Theme.ghostBorder
    border.width: 1

    Behavior on color {
        ColorAnimation { duration: 150 }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24 // 1.5rem internal padding from DESIGN.md
        spacing: 6

        // Label: technical, all-caps, secondary, letter-spacing +0.05em
        Text {
            Layout.fillWidth: true
            text: root.label.toUpperCase()
            font.family: Theme.fontFamily
            font.pixelSize: 10
            font.weight: Font.DemiBold
            font.letterSpacing: Theme.labelLetterSpacing
            color: Theme.secondary
            elide: Text.ElideRight
        }

        // Primary Headline / Value (on-surface #1a1b21)
        Text {
            text: root.value
            font.family: Theme.fontFamily
            font.pixelSize: 20
            font.weight: Font.Bold
            color: Theme.textOnSurface
            elide: Text.ElideRight
        }

        // Subvalue (on-surface-variant #474747)
        Text {
            visible: root.subValue !== ""
            text: root.subValue
            font.family: Theme.fontFamily
            font.pixelSize: 12
            font.weight: Font.Normal
            color: Theme.textOnSurfaceVariant
            elide: Text.ElideRight
        }
    }
}
