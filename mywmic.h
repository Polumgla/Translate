#ifndef MYWMIC_H
#define MYWMIC_H

#include <QObject>

class MyWMIC : public QObject
{
    Q_OBJECT
public:
    explicit MyWMIC(QObject *parent = nullptr);

    QString getWMIC(const QString &cmd);

    QString encryptionCPUID(QString cpuId);
    QString decodeCPUID(QString cpuId);
};

#endif // MYWMIC_H
