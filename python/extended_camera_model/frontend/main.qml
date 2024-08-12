import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "./items"

Window {
    id: window

    color: "white"
    width: 1280
    height: 720

    minimumWidth: 852
    minimumHeight: 480

    visible: true
    title: "3D - Engine"

    Header {
        id: header
        width: parent.width
        height: 50
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        z: 1
    }

    SliderPanel {
        id: sliderPanel
        visible: false
        width: visible ? 200 : 0
        anchors.left: parent.left
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        z: 1
    }

    Rectangle {
        id: mainContent
        anchors.top: header.bottom
        anchors.left: sliderPanel.visible ? sliderPanel.right : parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "transparent"
        z: 0

        Image {
            id: cameraImage
            source: "image://imageprovider/image"
        }
    }

    Connections {
    target: engineInstance
    onImageChanged: {
        console.log("Image changed. New source: ", cameraImage.source);
        cameraImage.source = "image://imageprovider/image";
    }
}

}
