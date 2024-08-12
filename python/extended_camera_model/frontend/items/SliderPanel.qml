import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: sliderPanel
    width: 200
    color: "#f0f0f0"
    border.color: "#cccccc"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 10
        anchors.topMargin: 10

        Text { text: "Camera Settings:" }

        GridLayout {
            columns: 2
            columnSpacing: 20
            rowSpacing: 10
            anchors.margins: 10

            Text { text: "X Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Y Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Z Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Roll:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }

            Text { text: "Pitch:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }

            Text { text: "Yaw:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }
        }

        Text { text: " " }

        Text { text: "Cube Settings:" }

        GridLayout {
            columns: 2
            columnSpacing: 20
            rowSpacing: 10
            anchors.margins: 10

            Text { text: "X Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Y Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Z Rotation:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 20000; Layout.alignment: Qt.AlignRight }

            Text { text: "Roll:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }

            Text { text: "Pitch:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }

            Text { text: "Yaw:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 0; to: 3600; Layout.alignment: Qt.AlignRight }

            Text { text: "Scale:"; Layout.alignment: Qt.AlignLeft }
            Slider { from: 1; to: 10; Layout.alignment: Qt.AlignRight }
        }

        Item { Layout.fillWidth: true }
    }
}
