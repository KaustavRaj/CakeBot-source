import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

import sql.contact 1.0

ApplicationWindow {
    id: frame
    visible: true
    width: changeWidth(400)
    height: changeHeight(600)
    title: qsTr("CakeBot")

    function changeHeight(height){
        return (Screen.height/768)*height;
    }

    function changeWidth(width){
        return (Screen.width/1366)*width;
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: CakeBotWelcome {}
    }
}
