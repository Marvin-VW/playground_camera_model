import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: header
    height: 50
    color: "#ffffff"

    RowLayout {
        anchors.fill: parent
        spacing: 10

        Rectangle {
            width: 10
            height: parent.height
            color: "#ffffff"
        }

        Button {
            text: "\u2630"
            onClicked: {
                sliderPanel.visible = !sliderPanel.visible
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Row {
            spacing: 75
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Switch {
                id: switch1
                text: "Points"
                checked: false
                onCheckedChanged: console.log("Switch 1 ist jetzt: " + (checked ? "An" : "Aus"))
            }

            Switch {
                id: switch2
                text: "Normals"
                checked: false
                onCheckedChanged: console.log("Switch 2 ist jetzt: " + (checked ? "An" : "Aus"))
            }

            Switch {
                id: switch3
                text: "Faces"
                checked: false
                onCheckedChanged: console.log("Switch 3 ist jetzt: " + (checked ? "An" : "Aus"))
            }

            Switch {
                id: switch4
                text: "Textures"
                checked: false
                onCheckedChanged: console.log("Switch 4 ist jetzt: " + (checked ? "An" : "Aus"))
            }

            Switch {
                id: switch5
                text: "Clipping Space"
                checked: false
                onCheckedChanged: console.log("Switch 5 ist jetzt: " + (checked ? "An" : "Aus"))
            }

            Switch {
                id: switch6
                text: "Backface Culling"
                checked: false
                onCheckedChanged: console.log("Switch 6 ist jetzt: " + (checked ? "An" : "Aus"))
            }
        }

        Rectangle {
            width: 10
            height: parent.height
            color: "#ffffff"
        }
    }

    Rectangle {
        height: 1
        width: parent.width
        anchors.bottom: parent.bottom

        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 0.2; color: "#c2c2c2" }
            GradientStop { position: 0.8; color: "#c2c2c2" }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }
}
