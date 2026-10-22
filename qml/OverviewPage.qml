import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import BatteryApp

Item {
    id: root

    property var currentDev: batteryManager.currentBattery

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        Item {
            width: parent.width
            implicitHeight: contentColumn.implicitHeight + 44

            ColumnLayout {
                id: contentColumn
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 20
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 28

                // ================= HERO SPREAD =================
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: heroLayout.implicitHeight + 48
                    radius: 0
                    color: Theme.surfaceContainerLowest
                    border.color: Theme.ghostBorder
                    border.width: 1

                    ColumnLayout {
                        id: heroLayout
                        anchors.fill: parent
                        anchors.margins: 24
                        spacing: 18

                        // Hero Status Header
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            ColumnLayout {
                                spacing: 2
                                Text {
                                    text: currentDev ? (currentDev.vendor + " " + currentDev.model) : "Primary Battery"
                                    font.family: Theme.fontFamily
                                    font.pixelSize: 15
                                    font.weight: Font.Bold
                                    color: Theme.textOnSurface
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            // Power Mode Badge (Clean rectangular badge)
                            Rectangle {
                                implicitHeight: 26
                                implicitWidth: pwrText.implicitWidth + 20
                                radius: 0
                                color: Theme.surfaceContainerHighest

                                Text {
                                    id: pwrText
                                    anchors.centerIn: parent
                                    text: batteryManager.acConnected ? "AC" : "ON BATTERY"
                                    font.family: Theme.fontFamily
                                    font.pixelSize: 10
                                    font.weight: Font.Bold
                                    font.letterSpacing: 0.8
                                    color: Theme.textOnSurface
                                }
                            }
                        }

                        // Centered Circular Metric Gauge
                        BatteryGauge {
                            Layout.alignment: Qt.AlignHCenter
                            percentage: currentDev ? currentDev.percentage : 0.0
                            stateString: currentDev ? currentDev.stateString : "Idle"
                            energyRate: currentDev ? currentDev.energyRate : 0.0
                            timeRemaining: currentDev ? currentDev.timeRemainingString : ""
                            isCharging: currentDev ? (currentDev.state === 1) : false
                        }

                        // Editorial Capacity & Health Precision Bar
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    text: "HEALTH & CAPACITY"
                                    font.family: Theme.fontFamily
                                    font.pixelSize: 10
                                    font.weight: Font.Bold
                                    font.letterSpacing: Theme.labelLetterSpacing
                                    color: Theme.secondary
                                }
                                Item {
                                    Layout.fillWidth: true
                                }
                                Text {
                                    text: currentDev ? (currentDev.capacityHealth.toFixed(1) + "% HEALTH  •  " + currentDev.energyFull.toFixed(1) + " / " + currentDev.energyFullDesign.toFixed(1) + " WH") : "N/A"
                                    font.family: Theme.fontFamily
                                    font.pixelSize: 10
                                    font.weight: Font.DemiBold
                                    font.letterSpacing: 0.6
                                    color: Theme.textOnSurfaceVariant
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 4
                                radius: 0
                                color: Theme.surfaceContainerHighest

                                Rectangle {
                                    width: parent.width * (currentDev ? Math.min(1.0, currentDev.capacityHealth / 100.0) : 1.0)
                                    height: parent.height
                                    radius: 0
                                    color: Theme.primary
                                }
                            }
                        }
                    }
                }

                // ================= SECTION: THE GALLERY =================
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    Text {
                        text: "ELECTRICAL & HARDWARE SPECIFICATIONS"
                        font.family: Theme.fontFamily
                        font.pixelSize: 11
                        font.weight: Font.Bold
                        font.letterSpacing: 1.2
                        color: Theme.secondary
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 16
                        rowSpacing: 16

                        EditorialCard {
                            label: "Capacity & Health"
                            value: currentDev ? (currentDev.capacityHealth.toFixed(1) + "%") : "100.0%"
                            subValue: currentDev ? (currentDev.energyFull.toFixed(2) + " Wh of " + currentDev.energyFullDesign.toFixed(2) + " Wh design") : ""
                        }

                        EditorialCard {
                            label: "Energy Remaining"
                            value: currentDev ? (currentDev.energy.toFixed(2) + " Wh") : "0.00 Wh"
                            subValue: currentDev ? (currentDev.percentage.toFixed(1) + "% current level") : ""
                        }

                        EditorialCard {
                            label: "Voltage & Potential"
                            value: currentDev ? (currentDev.voltage.toFixed(2) + " V") : "0.00 V"
                            subValue: currentDev ? (currentDev.voltageMinDesign.toFixed(2) + " V minimum design limit") : ""
                        }

                        EditorialCard {
                            label: "Power"
                            value: currentDev ? (currentDev.energyRate.toFixed(2) + " W") : "0.00 W"
                            subValue: currentDev ? currentDev.stateString : "Idle"
                        }

                        EditorialCard {
                            label: "Hardware Model"
                            value: currentDev ? currentDev.model : "Unknown"
                            subValue: currentDev ? (currentDev.vendor + " • " + currentDev.nativePath + " • S/N " + currentDev.serial) : ""
                        }

                        EditorialCard {
                            label: "Chemistry & Cycles"
                            value: currentDev ? currentDev.technology : "Unknown"
                            subValue: currentDev ? ((currentDev.cycleCount >= 0 ? (currentDev.cycleCount + " cycles") : "0 cycles") + " • Warning: " + currentDev.warningLevel) : ""
                        }
                    }
                }

                // Bottom spacer
                Item {
                    Layout.preferredHeight: 20
                }
            }
        }
    }
}
