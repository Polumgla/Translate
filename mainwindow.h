#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class ShowBarcodeToLed;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool writeXlxs(QString barcode);
    void InitMenu();
    void checkXlsx();
    void InitTimer();

private slots:
    void on_setting_Trigger();
    void on_tips_Trigger();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void sendBarcodeToLed(QString barcode);

private:
    Ui::MainWindow *ui;

    QString m_barcode;
    int m_barcodeRows;

    QTimer *m_timer;
    QTimer *m_timerShow;
    int time;

    QString m_excelPath;
    QString m_ledIp;

    ShowBarcodeToLed *m_showBarcodeToLed;
    QThread *m_threadLed;
};

#endif // MAINWINDOW_H
