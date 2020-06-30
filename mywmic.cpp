#include "mywmic.h"
#include <QProcess>
#include <QCryptographicHash>

MyWMIC::MyWMIC(QObject *parent) : QObject(parent)
{

}

QString MyWMIC::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

QString MyWMIC::encryptionCPUID(QString cpuId)
{
    return (QString)cpuId.toUtf8().toBase64();
}
QString MyWMIC::decodeCPUID(QString cpuId)
{
    return (QString)QByteArray::fromBase64(cpuId.toUtf8());
}
