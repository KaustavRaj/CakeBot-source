import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import QtQuick.Window 2.12

import bot.speak 1.0
import cake.bot 1.0
import receipt.print 1.0
import cancel.order 1.0
import sql.conversation 1.0

Page {
    id: chat
    property string inConversationWith
    property string me

    function changeHeight(height){
        return (Screen.height/768)*height;
    }

    function changeWidth(width){
        return (Screen.width/1366)*width;
    }

    header: ToolBar {
        height: chat.changeHeight(64)
        position: ToolBar.Header
        clip: false
        opacity: 1
        contentWidth: 0

        State {
            name: "fontloaded"
            when: delius.status === FontLoader.Ready
        }

        FontLoader {
            id: delius
            source: "../font/DeliusSwashCaps-Regular.ttf"
            onStatusChanged: if (delius.status !== FontLoader.Ready) {
                                 cakebottext.font.family = "helvetica"
                                 cakebottext.font.pixelSize = 30
                             }
                             else {
                                 cakebottext.font.family = delius.name
                                 cakebottext.font.pixelSize = 35
                             }
        }

        Text {
            id: cakebottext
            color: "#fffafa"
            text: "Cake Bot"
            anchors.centerIn: parent
            font.wordSpacing: 4
            font.family: delius.name
            font.pixelSize: 35
        }

        DropShadow {
            anchors.fill: cakebottext
            horizontalOffset: 2
            verticalOffset: 2
            radius: 2.0
            samples: 10
            color: "#dda0dd"
            source: cakebottext
        }

        Image {
            id: logo
            source: "qrc:/pic/logo.png"
            anchors {
                left: parent.left
                leftMargin: chat.changeWidth(10)
                verticalCenter: parent.verticalCenter
            }
        }

        background: Rectangle {
            width: parent.width
            height: chat.changeHeight(64)
            color: "#ba55d3"
            opacity: 1
        }

        Glow {
            anchors.fill: logo
            radius: 4
            samples: 8
            color: "#ee82ee"
            source: logo
        }

        Button {
            id: silent
            background : Image {
                id: voice_disable
                source: "qrc:/pic/voice_disable.png"
                height: chat.changeHeight(30)
                width: chat.changeWidth(30)
            }
            anchors {
                right: parent.right
                rightMargin: chat.changeWidth(10)
                verticalCenter: parent.verticalCenter
            }
            visible: true
            enabled: true

            onClicked: {
                silent.enabled = false
                silent.visible = false
                speak.enabled = true
                speak.visible = true
            }
        }

        Button {
            id: speak
            background: Image {
                id: voice_enable
                source: "qrc:/pic/voice_enable.png"
                height: chat.changeHeight(31)
                width: chat.changeWidth(31)

            }
            anchors {
                right: parent.right
                rightMargin: chat.changeWidth(10)
                verticalCenter: parent.verticalCenter
            }
            visible: false
            enabled: false

            onClicked: {
                silent.enabled = true
                silent.visible = true
                speak.enabled = false
                speak.visible = false
                botSpeak.stop()
            }
        }
    }

    WakeUpTheBot {
        id: botwelcome
    }

    ColumnLayout {

        id: colLayout
        anchors.fill: parent
        Rectangle {
            Image {
                source: "qrc:/pic/wallpaper.jpg"
                opacity: 0.3
                height: chat.changeHeight(1500)
                width: chat.changeWidth(1500)
            }
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 60
            displayMarginEnd: 60
            bottomMargin: 60
            topMargin: 60
            verticalLayoutDirection: ListView.TopToBottom
            spacing: 1
            model: SqlConversationModel { }
            delegate: Column {
                anchors.right: sentByMe ? parent.right : undefined
                spacing: 6

                readonly property bool sentByMe: model.recipient === inConversationWith

                Row {
                    id: messageRow
                    spacing: 6
                    anchors.right: sentByMe ? parent.right : undefined

                    Image {
                        id: avatar
                        source: !sentByMe ? "../pic/logo_48x48.ico" : ""
                    }

                    Rectangle {
                        width: Math.min(messageText.implicitWidth + 24, listView.width - avatar.width - messageRow.spacing)
                        height: messageText.implicitHeight + 24
                        color: sentByMe ? "#da70d6" : "#1e90ff"
                        radius: 20

                        Label {
                            id: messageText
                            text: model.message
                            font.pixelSize: 13
                            color: sentByMe ? "white" : "white"
                            anchors.fill: parent
                            anchors.margins: 12
                            wrapMode: Label.Wrap
                        }
                    }
                }

                Label {
                    id: timestampText
                    text: Qt.formatDateTime(model.timestamp, "dd MMM h:mm a")
                    color: "#da70d6"
                    opacity: 1
                    anchors.right: sentByMe ? parent.right : undefined
                }
            }

            Connections {
                target: bots
                onBotreply: {
                    listView.model.sendMessage(me, reply);
                    if (speak.enabled == true) {
                        botSpeak.speak(reply);
                        //botSpeak.stop();
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                rightPadding: 5
            }

            onCountChanged: {
                listView.currentIndex = count - 1
            }

        }
    }

    Pane {
        id: pane
        Layout.fillWidth: true
        width: parent.width
        anchors {
            bottom: parent.bottom
        }
        background: Rectangle {
            id: footer
            color: "#ba55d3"
            width: parent.width
            anchors.bottom: parent.bottom
            height: chat.changeHeight(75)
        }

        RowLayout {
            id: layout
            width: parent.width
            height: chat.changeHeight(75)
            anchors.verticalCenter: parent.verticalCenter

            TextArea {
                id: messageField
                Layout.fillWidth: true
                placeholderTextColor: "#800080"
                placeholderText: qsTr("<i>Type message here ...</i>")
                wrapMode: TextArea.Wrap
                selectByKeyboard: true
                selectByMouse: true
                background: Rectangle {
                    id: messageFieldbg
                    color: "white"
                    opacity: 1
                    radius: 20
                    implicitWidth: chat.changeWidth(200)
                    implicitHeight: chat.changeHeight(38)
                }
                font.pixelSize: 12
            }

            WakeUpTheBot {
                id: sendUserMsgToBot
            }


            //---------- CANCEL ORDER BUTTON -------

            Button {
                id: cancelOrderButton
                //anchors.centerIn: parent.Center
                text: qsTr("<b>CANCEL ORDER</b>")
                focusPolicy: Qt.ClickFocus
                enabled: false
                visible: false

                onClicked: {
                    cancelOrder.cancel();
                }

                onHoveredChanged: hovered ? cancelorderbuttonbg.opacity = 0.65 : cancelorderbuttonbg.opacity = 1

                contentItem: Text {
                    id: cancelText
                    text: cancelOrderButton.text
                    font: cancelOrderButton.font
                    opacity: enabled ? 1.0 : 0.6
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    id: cancelorderbuttonbg
                    implicitWidth: chat.changeWidth(75)
                    implicitHeight: chat.changeHeight(40)
                    opacity: enabled ? 1 : 0.3
                    color: "#f08080"
                    border.width: 0
                    radius: 20
                }
            }


            //---------- SEND BUTTON ---------------

            function sendToPrintButton(str) {
                if (str === "quit") {
                    sendButton.text = "<b>PRINT</b>"
                    sendButton.printerButton = true
                    sendbuttonbg.color = "#dc143c"

                    messageField.enabled = false
                    messageField.visible = false

                    cancelOrderButton.enabled = true
                    cancelOrderButton.visible = true
                }
            }

            PrintReceipt {
                id: printout
            }

            Button {
                id: sendButton
                property bool printerButton: false
                text: qsTr("<b>SEND</b>")
                focusPolicy: Qt.ClickFocus
                enabled: printerButton == false ? (messageField.length > 0) : true
                visible: true

                onClicked: {
                    if (printerButton == false) {
                        listView.model.sendMessage(inConversationWith, messageField.text);
                        bots.startTheBot(messageField.text);



                        layout.sendToPrintButton(messageField.text);
                        messageField.text = "";
                    }

                    else {
                        bots.startTheBot("printTheFile321");
                    }
                }

                onHoveredChanged: enabled ? (hovered ? sendbuttonbg.opacity = 0.65 : sendbuttonbg.opacity = 1) : sendbuttonbg.opacity = 0.3;

                contentItem: Text {
                    id: test
                    text: sendButton.text
                    font: sendButton.font
                    opacity: enabled ? 1.0 : 0.6
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    id: sendbuttonbg
                    implicitWidth: chat.changeWidth(100)
                    implicitHeight: chat.changeHeight(40)
                    opacity: enabled ? 1 : 0.3
                    color: "#dda0dd"
                    border.width: 0
                    radius: 20
                }
                Component.onCompleted: {
                    listView.model.sendMessage(me, "What type of cake do you want - birthday , wedding/anniversary ceremony or just for casual purpose ? ");
                    botwelcome.startTheBot("start");
                }
            }
        }
    }
}
