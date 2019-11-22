import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.12

import cake.bot 1.0
import sql.contact 1.0

Page {
    id: welcome

    property date currDate: new Date()

    function changeHeight(height){
        return (Screen.height/768)*height;
    }

    function changeWidth(width){
        return (Screen.width/1366)*width;
    }

    function dayCondition() {
        var h = currDate.getHours()
        var m = currDate.getMinutes()
        var t = h*60 + m
        if(t >= 240 && t < 720) {
            return "morning"
        }
        else if(t >= 720 && t < 960) {
            return "afternoon"
        }
        return "evening"
    }

    function getMinSideLength() {
        var x = Screen.width
        var y = Screen.height
        return Math.max(x,y)
    }

    background: Image {
        source: "qrc:/pic/bubble.jpeg"
        height: welcome.changeHeight(getMinSideLength())
        width: welcome.changeWidth(getMinSideLength())
        opacity: 0.7
        fillMode: Image.PreserveAspectFit
        anchors{
            horizontalCenter: parent.horizontalCenter
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: SqlContactModel {}

        Image {
            id: welcomepic
            source: "qrc:/pic/WelcomeFront.png"
            width: welcome.changeWidth(500)
            fillMode: Image.PreserveAspectFit
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            y: welcome.changeHeight((listView.height - implicitHeight)/5)
        }

        State {
            name: "welcomefontloaded"
            when: clickerScript.status === FontLoader.Ready
        }

        FontLoader {
            id: clickerScript
            source: "../font/ClickerScript-Regular.ttf"
            onStatusChanged: if (clickerScript.status !== FontLoader.Ready) {
                                 cakebottext.font.family = "helvetica"
                                 cakebottext.font.pixelSize = 20
                             }
                             else {
                                 cakebottext.font.family = clickerScript.name
                                 cakebottext.font.pixelSize = 17
                             }
        }

        Text {
            id: welcomemsg
            text: "                       Good " + dayCondition() + " !
    I'm Cake Bot and will be taking your order.
   To chat with me, please click on the cake below."
            font.family: clickerScript.name
            font.pointSize: 19.5
            renderType: Text.NativeRendering
            font.hintingPreference: Font.PreferVerticalHinting
            color: "black"  /*"#ffe4e1"*/
            anchors{
                horizontalCenter: parent.horizontalCenter
            }
            y: welcome.changeHeight((listView.height)*0.65)
        }

        Glow {
            anchors.fill: welcomepic
            radius: 13
            samples: 17
            color: "#ee82ee"
            source: welcomepic
        }

        DropShadow {
            anchors.fill: welcomemsg
            horizontalOffset: 2
            verticalOffset: 2
            radius: 5.0
            samples: 10
            color: "#c71585"
            source: welcomemsg
        }

        delegate: ItemDelegate {
            id: start

            background: Image {
                id: startpic
                source: "qrc:/pic/confectionery.png";
                width: welcome.changeWidth(64)
                height: welcome.changeHeight(64)
                fillMode: Image.PreserveAspectFit
                x: welcome.changeWidth((listView.width - width)/2)
                y: welcome.changeHeight(3*(listView.height + height)/4)

                MouseArea {
                    anchors.fill: parent
                    onClicked : {
                        welcome.StackView.view.push("qrc:/ChatZone.qml", {inConversationWith: model.display})
                    }
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }
}
