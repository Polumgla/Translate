#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QSettings>
#include <QMessageBox>
#include "aboutdlg.h"
#include "settingdlg.h"
#include "xlsxdocument.h"
#include "showbarcodetoled.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_barcodeRows = 0;
    QSettings settings("config", QSettings::IniFormat);
    m_ledIp = settings.value("LED/ip").toString();
    m_excelPath = settings.value("BARCODE/path").toString();
    ui->label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    InitTimer();
    InitMenu();
    checkXlsx();

    m_showBarcodeToLed = new ShowBarcodeToLed();

}

MainWindow::~MainWindow()
{
    if(m_showBarcodeToLed) {
        delete m_showBarcodeToLed;
        m_showBarcodeToLed= NULL;
    }
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    m_timer->stop();
    if(event->text() == "\r" && !m_barcode.isEmpty()) {
        ui->label->setText(m_barcode);
        if(m_barcode.length() < 15) {
            m_showBarcodeToLed->sendCollectData(m_barcode);
            m_showBarcodeToLed->sendCollectData2(" ");
        } else if(m_barcode.length() >= 15) {
            m_showBarcodeToLed->sendCollectData(m_barcode.left(15));
            m_showBarcodeToLed->sendCollectData2(m_barcode.mid(15));
        }

        writeXlxs(m_barcode);
        m_barcode.clear();
        m_timerShow->start();
    } else {
        m_barcode.append(event->text());
    }

}

bool MainWindow::writeXlxs(QString barcode)
{
    QString path = QString("%1/%2.xlsx").arg(m_excelPath).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QXlsx::Document xlsx(path);
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:mm");
    xlsx.write(m_barcodeRows+1, 1, barcode);
    xlsx.write(m_barcodeRows+1, 2, time);
    xlsx.save();
    m_barcodeRows++;
    return true;
}

void MainWindow::InitMenu()
{
    ui->menuBar->setStyleSheet("background-color: rgb(208, 208, 209);");

    ui->menuBar->addAction("Setting", this, SLOT(on_setting_Trigger()));
    ui->menuBar->addAction("Tips", this, SLOT(on_tips_Trigger()));
}
void MainWindow::on_setting_Trigger()
{
    SettingDlg setDlg;
    if(setDlg.exec() != 0) {
        int nRet = QMessageBox::information(this, "Translate", "重新启动软件,设置生效.请点击 OK ,关闭软件.", QMessageBox::Yes, QMessageBox::No);
        if(nRet == QMessageBox::Ok)
            this->close();
    }
}
void MainWindow::on_tips_Trigger()
{
    AboutDlg aboutDlg;
    aboutDlg.exec();
}

void MainWindow::checkXlsx()
{
    QString path = QString("%1/%2.xlsx").arg(m_excelPath).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QDir dir(m_excelPath);
    if(dir.exists()) {
        QFile file(path);
        if(file.exists()) {
            QXlsx::Document xlsx(path);
            int row = 1;
            while (1) {
                QString barcode = xlsx.read(row,1).toString();
                if(barcode.isEmpty()) {
                    barcode = xlsx.read(row,2).toString();
                    if(barcode.isEmpty())
                        break;
                }
                m_barcodeRows++;
                row++;
                barcode.clear();
            }
        } else {
            QXlsx::Document xlsx;
            xlsx.write(1, 1, "Barcode");
            xlsx.write(1, 2, "Time");
            xlsx.saveAs(path);
            m_barcodeRows++;
        }
    } else {
        dir.mkdir(m_excelPath);
        QXlsx::Document xlsx;
        xlsx.write(1, 1, "Barcode");
        xlsx.write(1, 2, "Time");
        xlsx.saveAs(path);
        m_barcodeRows++;
    }
    qDebug()<<__FUNCTION__<<m_barcodeRows;
}

void MainWindow::InitTimer()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [=](){
        ui->label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
    m_timer->setInterval(1000);
    m_timer->start();
    m_timerShow = new QTimer(this);
    connect(m_timerShow, &QTimer::timeout, [=](){
        m_timer->start();
    });
    m_timerShow->setInterval(1000*60*3);
}
