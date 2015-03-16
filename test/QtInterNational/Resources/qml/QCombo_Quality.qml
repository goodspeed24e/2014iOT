import QtQuick 1.1

Rectangle {
    id: combo_Box;
    smooth: true;
    radius: 4;

    property real items_Counter: 3
    property variant items: "";
    property alias selected_Item: chosen_Item_Text.text;
    property alias selected_Index: list_View.currentIndex;
    property string title_text;
    property real margin: 5;

    signal clicked();

    Rectangle {
        // the First one , show in starting
        id: chosen_Item;
        anchors.fill: combo_Box;
        radius: 4;
        color: "lightgray";
        smooth: true;

        gradient: Gradient {
            GradientStop { position: 0.0; color: "gray" }
            GradientStop { position: 1.0; color: "white" }
        }

        Text {
            anchors.centerIn: parent;
            id: chosen_Item_Text;
            text: combo_Box.items[0];
            font.pointSize: 14;
            color: "Black";
            smooth: true;
        }

        MouseArea {
            anchors.fill: chosen_Item;
            onClicked: {
                    combo_Box.state = combo_Box.state==="dropDown"?"":"dropDown"
            }
        }
    }

    Rectangle {
        id: dropDown;
        width: combo_Box.width;
        clip: true;
        radius: 4;
        anchors.bottom: chosen_Item.top;
        anchors.margins: 0;

        gradient: Gradient {
            GradientStop { position: 0.1; color: "gray" }
            GradientStop { position: 1.0; color: "white" }
        }

        Rectangle {
            id: combo_title;
            width: combo_Box.width;
            height: combo_Box.height + margin;
            anchors.top: dropDown.top;
            opacity: 0;
            visible: opacity > 0;
            color: "#00000000"
            border.color: "black"
            border.width: 1;
            z: 9;

            gradient: Gradient {
                GradientStop { position: 0.0; color: "darkgray" }
                GradientStop { position: 1.0; color: "gray" }
            }

            Text{
                id: combo_title_Text;
                text: title_text;
                font.pointSize: 16;
                style: Text.Raised; styleColor: "White";
                color: "Black";
                smooth:true;
                anchors.centerIn: combo_title;
            }
        }

        ListView {
            id: list_View;
            width: dropDown.width;
            height: dropDown.height - chosen_Item.height;
            anchors.top: combo_title.bottom;
            anchors.topMargin: margin;
            model: combo_Box.items;  cacheBuffer: 200
            currentIndex: list_View.currentIndex
            highlightFollowsCurrentItem: true
            delegate:
                Rectangle{
                    width: combo_Box.width;
                    height: combo_Box.height;
                    color: ListView.isCurrentItem ? "#AA0000" : "#00000000"
                    radius: 4;
                    Text {
                        id: text_combo;
                        text: modelData
                        font.pointSize: 14;
                        anchors.centerIn: parent;
                        anchors.margins: 1;
                        color: "Black";
                    }

                    function todo()
                    {
                        combo_Box.state = ""
                        var prevSelection = chosen_Item_Text.text
                        chosen_Item_Text.text = modelData
                        if(chosen_Item_Text.text != prevSelection)
                        {
                            combo_Box.clicked();
                        }
                        list_View.currentIndex = index;
                        chosen_Item_Text.text = combo_Box.items[list_View.currentIndex]
                    }

                MouseArea {
                    anchors.fill: parent;
                    hoverEnabled: true;
                    z: 10;
                    onClicked: todo();
                    onDoubleClicked: todo();
                    onEntered: {list_View.currentIndex = index; text_combo.color = "White";}
                    onExited: {text_combo.color = "Black";}
                }
            }
        }
    }

    states: State {
        name: "dropDown";
        PropertyChanges { target: dropDown; height: (items_Counter + 1) * chosen_Item.height + margin + margin}
        PropertyChanges { target: combo_title; opacity: 1}
    }

    transitions: Transition {
        NumberAnimation { target: dropDown; properties: "height"; easing.type: Easing.OutExpo; duration: 500 }
    }
}
