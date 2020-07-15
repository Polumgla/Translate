#include "showbarcodetoled.h"
#include <QLibrary>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>

ShowBarcodeToLed::ShowBarcodeToLed(QObject *parent) : QObject(parent)
{
    init();
    addShowPage();
    addCollectDataTmp();
    sendTemplate();
}

void ShowBarcodeToLed::init()
{
    QSettings setting("config", QSettings::IniFormat);
    LedIP = setting.value("LED/ip").toString();
    if(LedIP.isEmpty()) {
        setting.setValue("LED/ip", "192.168.0.142");
        LedIP = "192.168.0.142";
    }

}

void ShowBarcodeToLed::addShowPage()
{
    QLibrary m_lib("QYLED");
    if(m_lib.load()) {
        addShowPageFunc = (AddShowPage)m_lib.resolve("AddShowPage");
        if(addShowPageFunc) {
            int i = addShowPageFunc("00:00", "23:59", 0);
            if(i != 0)
                qDebug()<<__FUNCTION__<<" | add show page failed.";
        } else {
            qDebug()<<__FUNCTION__<<" | load interface failed.";
        }
    } else
        qDebug()<<__FUNCTION__<<" | load dll failed.";
}

void ShowBarcodeToLed::addCollectDataTmp()
{
    QLibrary m_lib("QYLED");
    if(m_lib.load()) {
        addAreaFunc = (AddArea)m_lib.resolve("AddArea");
        if(addAreaFunc) {
            int i = addAreaFunc(0, 0, 128, 16);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | add area failed .";
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface:AddArea  failed .";
        }
        addTemplate_CollectDataFunc = (AddTemplate_CollectData)m_lib.resolve("AddTemplate_CollectData");
        if(addTemplate_CollectDataFunc) {
            int i = addTemplate_CollectDataFunc("  ", 1, 1, 1, 1, 1);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | add tmp collect data failed .";
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface:AddTmp_CollectData failed .";
        }

        if(addAreaFunc) {
            int i = addAreaFunc(0, 16, 128, 16);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | add area failed 2 .";
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface:AddArea  failed 2 .";
        }
        if(addTemplate_CollectDataFunc) {
            int i = addTemplate_CollectDataFunc("  ", 2, 1, 1, 1, 1);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | add tmp collect data failed 2 .";
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface:AddTmp_CollectData failed 2 .";
        }
    } else {
        qDebug()<<__FUNCTION__<<" | load dll failed.";
    }
}

void ShowBarcodeToLed::sendTemplate()
{
    QLibrary m_lib("QYLED");
    if(m_lib.load()) {
        sendTemplateDataFunc = (SendTemplateData)m_lib.resolve("SendTemplateData_Net");
        if(sendTemplateDataFunc) {
            int i = sendTemplateDataFunc(LedIP.toStdString(), 1);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | "<<LedIP;
            }

        } else {
            qDebug()<<__FUNCTION__<<" | load interface failed.";
        }
    } else {
        qDebug()<<__FUNCTION__<<" | load dll failed.";
    }
}

void ShowBarcodeToLed::sendCollectData(QString data)
{
    QLibrary m_lib("QYLED");
    if(m_lib.load()) {
        sendCollectionDataFunc = (SendCollectionData)m_lib.resolve("SendCollectionData_Net");
        if(sendCollectionDataFunc) {
            int i = sendCollectionDataFunc(data.toStdString(), LedIP.toStdString(), 1, 1, 1, 1, 1);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | "<<LedIP;
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface failed.";
        }
    } else {
        qDebug()<<__FUNCTION__<<" | load dll failed.";
    }
}

void ShowBarcodeToLed::sendCollectData2(QString data)
{
    QLibrary m_lib("QYLED");
    if(m_lib.load()) {
        sendCollectionDataFunc = (SendCollectionData)m_lib.resolve("SendCollectionData_Net");
        if(sendCollectionDataFunc) {
            int i = sendCollectionDataFunc(data.toStdString(), LedIP.toStdString(), 1, 2, 1, 1, 1);
            if(i != 0) {
                qDebug()<<__FUNCTION__<<" | "<<LedIP;
            }
        } else {
            qDebug()<<__FUNCTION__<<" | load interface failed.";
        }
    } else {
        qDebug()<<__FUNCTION__<<" | load dll failed.";
    }
}

void ShowBarcodeToLed::dealBarcode(QString barcode)
{
    if(barcode.length() < 15) {
        sendCollectData(barcode);
    } else {
        sendCollectData2(barcode.left(15));
    }
}
