import QtQuick
import QtQuick.Layouts
import BatteryApp

Item {
    id: root

    property double percentage: 0.0
    property string stateString: "Idle"
    property double energyRate: 0.0
    property string timeRemaining: ""
    property bool isCharging: false

    property double animatedPercentage: percentage

    Behavior on animatedPercentage {
        NumberAnimation {
            duration: 600
            easing.type: Easing.OutCubic
        }
    }

    onAnimatedPercentageChanged: canvas.requestPaint()

    Connections {
        target: Theme
        function onDarkModeChanged() {
            canvas.requestPaint();
        }
    }

    implicitWidth: 240
    implicitHeight: 240

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var centerX = width / 2;
            var centerY = height / 2;
            var radius = Math.min(centerX, centerY) - 18;
            var lineWidth = 10;

            var startAngle = 0.75 * Math.PI; // 135 deg
            var totalAngle = 1.5 * Math.PI;  // 270 deg span

            // Background Track: surface-container-highest
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, startAngle, startAngle + totalAngle, false);
            ctx.lineWidth = lineWidth;
            ctx.strokeStyle = "" + Theme.surfaceContainerHighest;
            ctx.lineCap = "butt";
            ctx.stroke();

            // Progress Arc: Disciplined primary
            var pct = Math.max(0.0, Math.min(100.0, root.animatedPercentage)) / 100.0;
            if (pct > 0) {
                var progressAngle = startAngle + (totalAngle * pct);
                ctx.beginPath();
                ctx.arc(centerX, centerY, radius, startAngle, progressAngle, false);
                ctx.lineWidth = lineWidth;
                ctx.strokeStyle = "" + Theme.primary;
                ctx.lineCap = "butt";
                ctx.stroke();
            }
        }
    }

    // Center Core Metrics (Optically centered within the arc)
    ColumnLayout {
        id: centerMetrics
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -10
        spacing: 4

        // Status Chip (sharp rectangular badge)
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            implicitWidth: stateText.implicitWidth + 20
            implicitHeight: 24
            radius: 0
            color: Theme.surfaceContainerHighest

            Text {
                id: stateText
                anchors.centerIn: parent
                text: {
                    var s = root.stateString.toLowerCase();
                    if (s.indexOf("pending") !== -1 || s.indexOf("conservation") !== -1)
                        return "PLUGGED IN";
                    return root.stateString.toUpperCase();
                }
                font.family: Theme.fontFamily
                font.pixelSize: 10
                font.weight: Font.Bold
                font.letterSpacing: Theme.labelLetterSpacing
                color: Theme.textOnSurface
            }
        }

        // Display Percentage (lg, -0.02em letter spacing)
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 2

            Text {
                text: Math.round(root.animatedPercentage)
                font.family: Theme.fontFamily
                font.pixelSize: 52
                font.weight: Font.Black
                font.letterSpacing: -1.0
                color: Theme.textOnSurface
            }

            Text {
                text: "%"
                font.family: Theme.fontFamily
                font.pixelSize: 22
                font.weight: Font.DemiBold
                color: Theme.secondary
                Layout.alignment: Qt.AlignBottom
                Layout.bottomMargin: 8
            }
        }

        // Instantaneous Power Draw
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: {
                if (Math.abs(root.energyRate) < 0.01) return "0.0 W DRAW";
                var sign = root.isCharging ? "+" : "-";
                return sign + Math.abs(root.energyRate).toFixed(1) + " W DRAW";
            }
            font.family: Theme.fontFamily
            font.pixelSize: 11
            font.weight: Font.DemiBold
            font.letterSpacing: 0.8
            color: Theme.secondary
        }
    }

    // Time Remaining Subtitle (Positioned down in the opening below the circular arc)
    Text {
        id: subtitleText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: root.timeRemaining
        font.family: Theme.fontFamily
        font.pixelSize: 11
        font.weight: Font.Normal
        color: Theme.textOnSurfaceVariant
        visible: text !== ""
        elide: Text.ElideMiddle
    }
}
