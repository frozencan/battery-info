import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import BatteryApp

Rectangle {
    id: root

    property int currentTab: 0
    signal tabSelected(int index)
    signal copyClicked

    implicitHeight: 68
    color: Theme.surfaceContainerLow
    border.color: Theme.ghostBorder
    border.width: 1
    radius: 0

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 16
        spacing: 16

        // Editorial Brand Stamp
        ColumnLayout {
            spacing: 2

            Text {
                text: "BATTERY"
                font.family: Theme.fontFamily
                font.pixelSize: 14
                font.weight: Font.Black
                font.letterSpacing: 1.5
                color: Theme.textOnSurface
            }

            Text {
                text: ":3"
                font.family: Theme.fontFamily
                font.pixelSize: 9
                font.weight: Font.DemiBold
                font.letterSpacing: 1.2
                color: Theme.secondary
            }
        }

        Item {
            Layout.fillWidth: true
        }

        // Selection Tabs (Clean Rectangular Editorial Style)
        Rectangle {
            implicitHeight: 38
            implicitWidth: tabRow.implicitWidth + 8
            radius: 0
            color: Theme.surfaceContainerHighest

            RowLayout {
                id: tabRow
                anchors.centerIn: parent
                spacing: 2

                Repeater {
                    model: ["Overview", "History", "Health", "Diagnostics"]
                    delegate: Rectangle {
                        required property int index
                        required property string modelData

                        implicitHeight: 32
                        implicitWidth: tabLabel.implicitWidth + 24
                        radius: 0
                        color: root.currentTab === index ? Theme.primary : "transparent"

                        Behavior on color {
                            ColorAnimation {
                                duration: 180
                            }
                        }

                        Text {
                            id: tabLabel
                            anchors.centerIn: parent
                            text: modelData
                            font.family: Theme.fontFamily
                            font.pixelSize: 12
                            font.weight: root.currentTab === index ? Font.Bold : Font.Medium
                            font.letterSpacing: 0.2
                            color: root.currentTab === index ? Theme.textOnPrimary : Theme.textOnSurfaceVariant
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                root.currentTab = index;
                                root.tabSelected(index);
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }

        // Action Controls (Theme Toggle & Primary CTA)
        RowLayout {
            spacing: 8

            // Dark Mode Toggle Button (Sharp rectangular, zero rounded corners)
            Rectangle {
                id: themeToggleBtn
                implicitHeight: 36
                implicitWidth: 36
                radius: 0
                color: toggleMouse.containsMouse ? Theme.surfaceContainerHighest : Theme.surfaceContainerLowest
                border.color: Theme.ghostBorder
                border.width: 1

                Behavior on color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Text {
                    anchors.centerIn: parent
                    text: Theme.darkMode ? "☀" : "☾"
                    font.family: Theme.fontFamily
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: Theme.textOnSurface
                }

                MouseArea {
                    id: toggleMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: Theme.toggleDarkMode()
                }

                ToolTip.visible: toggleMouse.containsMouse
                ToolTip.text: Theme.darkMode ? "Switch to Light Mode" : "Switch to Dark Mode"
                ToolTip.delay: 300
            }

            // Primary CTA Button (Solid primary with on-primary text, sharp corners)
            Rectangle {
                implicitHeight: 36
                implicitWidth: ctaLabel.implicitWidth + 28
                radius: 0
                color: ctaMouse.containsMouse ? Theme.primaryContainer : Theme.primary

                Behavior on color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Text {
                    id: ctaLabel
                    anchors.centerIn: parent
                    text: "Copy Specs"
                    font.family: Theme.fontFamily
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    font.letterSpacing: 0.4
                    color: Theme.textOnPrimary
                }

                MouseArea {
                    id: ctaMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.copyClicked()
                }
            }
        }
    }
}
