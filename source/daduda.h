#ifndef DADUDA_H
#define DADUDA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class DaDuDa; }
QT_END_NAMESPACE

class DaDuDa : public QMainWindow
{
    Q_OBJECT

public:
    DaDuDa(QWidget *parent = nullptr);
    ~DaDuDa();

    void set_shareArgs(QStringList &sarg);
    void checkFuses();
    void writeSign();

private slots:
    void on_scanBTN_clicked();

    void on_openBTN_clicked();

    void on_eeOpenBTN_clicked();

    void on_checkBTN_clicked();

    //Console
    void readFromStdout();
    void readFromStderr();

    void on_fReadBTN_clicked();

    void on_eeReadBTN_clicked();

    void on_fWriteBTN_clicked();

    void on_eeWriteBTN_clicked();

    void on_fVerifyBTN_clicked();

    void on_eeVerifyBTN_clicked();

    void on_pushButton_3_clicked();

    void on_eraseBTN_clicked();

    void on_rLockBTN_clicked();

    void on_wLockBTN_clicked();

    void on_rFuseBTN_clicked();

    void on_deviceCB_currentIndexChanged(int index);

    void on_wFuseBTN_clicked(bool checked);

private:
    Ui::DaDuDa *ui;
};
#endif // DADUDA_H
