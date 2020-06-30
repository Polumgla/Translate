#include "mainwindow.h"
#include <QApplication>
#include "mywmic.h"
#include <QSettings>
#include <QMessageBox>
#include "settingdlg.h"
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void customMessageHandler(QtMsgType, const QMessageLogContext &, const QString & str)
{
    QString txt=str;
#else
void customMessageHandler(QtMsgType, const char *msg)
{
    QString txt(msg);
#endif
    QString fileName = QString("%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile outFile(fileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "  " << txt << endl;
}


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qInstallMessageHandler(customMessageHandler); //注册qInstallMessageHandler回调函数
    QSettings setting("config", QSettings::IniFormat);
    MyWMIC myWMIC;

    if(setting.value("Base/key").toString().isEmpty()) {
        int nRet = QMessageBox::warning(NULL, "Translate", "是否绑定此设备？",
                             QMessageBox::Yes, QMessageBox::No);
        if(nRet == QMessageBox::Yes) {
            setting.setValue("Base/key", myWMIC.encryptionCPUID(myWMIC.getWMIC("wmic cpu get processorid")));
            SettingDlg setDlg;
            setDlg.exec();
        } else {
            return 0;
        }
    } else {
        if(myWMIC.decodeCPUID(setting.value("Base/key").toString()) != myWMIC.getWMIC("wmic cpu get processorid")) {
            QMessageBox::information(NULL, "Translate", "设备信息校验失败！", QMessageBox::Close);
            return 0;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
