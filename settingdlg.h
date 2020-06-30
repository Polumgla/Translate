#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>

namespace Ui {
class SettingDlg;
}

class SettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDlg(QWidget *parent = 0);
    ~SettingDlg();

private slots:
    void on_pushButton_clicked();

    void on_btn_Aplay_clicked();

private:
    Ui::SettingDlg *ui;
};

#endif // SETTINGDLG_H
