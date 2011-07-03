import QtQuick 1.0

Rectangle {
    id: title
    width: parent.width
    height: 28
    color: "#141166"
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#141166";
        }
        GradientStop {
            position: 0.30;
            color: "#ffffff";
        }
        GradientStop {
            position: 0.66;
            color: "#ffffff";
        }
        GradientStop {
            position: 1.00;
            color: "#141166";
        }
    }
    property string title: "title..."
    Text {
        color: "#131c47"
        text: parent.title; font.family: "Handwriting - Dakota";font.pointSize: 22
        anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
    }
}
