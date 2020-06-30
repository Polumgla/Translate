#ifndef SHOWBARCODETOLED_H
#define SHOWBARCODETOLED_H

#include <QObject>
#include <iostream>

//添加显示页
typedef int(*AddShowPage)(std::string startTime, std::string stopTime, int weekDay);

//添加区域
typedef int(*AddArea)(int Xpos, int Ypos, int AreaWidth, int AreaHigth);

//添加实时采集模板
typedef int(*AddTemplate_CollectData)(std::string ShowContent, int TypeNo,
                               int ScreenColor, int FontColor, int FontBody, int FontSize);

//添加内码文字模板
typedef int(*AddTemplate_InternalText)(std::string ShowContent, int UID, int ScreenColor, int ShowStyle,
                                       int ShowSpeed, int StopTime, int FontColor, int FontBody, int FontSize, int PowerOffSave);
//发送素材模板
typedef int(*SendTemplateData)(std::string CardIP, int NetProtocol);

//发送实时采集
typedef int(*SendCollectionData)(std::string ShowContent, std::string CardIP, int NetProtocol,
                               int UID, int FontColor, int FontBody, int FontSize);

//发送内码文字
typedef int(*SendInternalText)(std::string ShowContent, std::string CardIP, int NetProtocol, int AreaWidth, int AreaHigth,
                               int UID, int ScreenColor, int ShowStyle, int ShowSpeed, int StopTime, int FontColor, int FontBody,
                               int FontSize, int UpdateStyle, bool PowerOffSave);

class ShowBarcodeToLed : public QObject
{
    Q_OBJECT
public:
    explicit ShowBarcodeToLed(QObject *parent = 0);
private:
    void init();

public:
    void addShowPage();
    void addCollectDataTmp();
    void sendTemplate();
    void sendCollectData(QString data);
    void sendCollectData2(QString data);
    void dealBarcode(QString barcode);

private:
    AddShowPage addShowPageFunc;
    AddArea addAreaFunc;
    AddTemplate_CollectData addTemplate_CollectDataFunc;
    SendTemplateData sendTemplateDataFunc;
    SendCollectionData sendCollectionDataFunc;

    QString LedIP;

};

#endif // SHOWBARCODETOLED_H
