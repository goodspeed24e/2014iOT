import QtQuick 1.0

Rectangle {
    id: mainPage
    width: 800; height: 600

    // rootItem.emptyString is a feature for C++ to identify
    property string lang_english: qsTr("English") + rootItem.emptyString;
    property string lang_zh_CN: qsTr("Chinese Simplified") + rootItem.emptyString;
    property string lang_zh_TW: qsTr("Chinese Traditional") + rootItem.emptyString;

    function getLanguageName(number)
    {
        switch(number)
        {
        case 0:
            return "EN";
        case 1:
            return "zh_CN";
        case 2:
            return "zh_TW";
        default:
            return null;
        }
    }

    Column {
        id: column;
        anchors.centerIn: mainPage; spacing: 0.1*mainPage.height;

        Text {
            text: qsTr("Hello") + rootItem.emptyString
            font.pointSize: 25; anchors.horizontalCenter: column.horizontalCenter
        }
    }

    Row {
        id: row;
        anchors.top: column.bottom;
        anchors.topMargin: 10;
        anchors.horizontalCenter: mainPage.horizontalCenter
        Button { label: qsTr("English") + rootItem.emptyString; onClicked: rootItem.selectLanguage("EN"); }
        Button { label: qsTr("Chinese Traditional") + rootItem.emptyString; onClicked: rootItem.selectLanguage("zh_TW"); }
        Button { label: qsTr("Chinese Simplified") + rootItem.emptyString; onClicked: rootItem.selectLanguage("zh_CN"); }
    }

    QCombo_Quality{
        id: combo;
        width: 150;
        height: 30;
        anchors.bottom: mainPage.bottom;
        anchors.bottomMargin: 10;
        anchors.horizontalCenter: mainPage.horizontalCenter;
        opacity: 1;
        visible: combo.opacity > 0;

        onClicked: rootItem.selectLanguage(mainPage.getLanguageName(combo.selected_Index));
        // String for combo box list
        items: {[lang_english, lang_zh_CN, lang_zh_TW]}
        // Title of combo box
        title_text: qsTr("Language") + rootItem.emptyString;
    }
}
