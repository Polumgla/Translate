#include "settingdlg.h"
#include "ui_settingdlg.h"
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

SettingDlg::SettingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDlg)
{
    ui->setupUi(this);
    QSettings settings("config", QSettings::IniFormat);
    ui->lineEdit_IP->setText(settings.value("LED/ip", "127.0.0.1").toString());
    ui->lineEdit_Path->setText(settings.value("BARCODE/path", "D:/barcode").toString());
}

SettingDlg::~SettingDlg()
{
    delete ui;
}

void SettingDlg::on_pushButton_clicked()
{
    QFileDialog fileDialog;
    QString filePath;
    filePath = fileDialog.getExistingDirectory(this, "", "D:/");
    ui->lineEdit_Path->setText(filePath+"/barcode");
}

void SettingDlg::on_btn_Aplay_clicked()
{
    QSettings settings("config", QSettings::IniFormat);
    if(ui->lineEdit_IP->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "IP can not be NULL.");
        return;
    }
    settings.setValue("LED/ip", ui->lineEdit_IP->text());
    settings.setValue("BARCODE/path", ui->lineEdit_Path->text());

//    close();
    this->accept();
}
