#include "daduda.h"
#include "ui_daduda.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFileDialog>
#include <QDebug>
#include <QProcess>
#include <QObject>
#include <QStringList>
#include <QMap>
#include <QFile>

//Глобальные переменные
QString fPath, eePath;
QString mainStr;
QString text;

QProcess *proc = new QProcess();

QMap<QString, QStringList> devices;
QMap<QString, QString> sck_rate;

bool checkSign = false;


DaDuDa::DaDuDa(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DaDuDa)
{
    ui->setupUi(this);

    ui->portCB->clear();
    ui->portCB->addItem("usb");
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
 #ifdef Q_OS_LINUX
        ui->portCB->addItem("/dev/" + serialPortInfo.portName());
 #else
        ui->portCB->addItem(serialPortInfo.portName());
 #endif
    }

    connect( proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdout()) );
    connect( proc, SIGNAL(readyReadStandardError()), this, SLOT(readFromStderr()) );

    devices["AT90CAN128"] << "c128" << "62" << "99" << "FF";
    devices["AT90CAN32"] << "c32" << "62" << "99" << "FF";
    devices["AT90CAN64"] << "c64" << "62" << "99" << "FF"; //
    devices["AT90PWM2"] << "pwm2" << "41" << "DF" << "F9"; // = "pwm2";
    devices["AT90PWM216"] << "pwm216" << "41" << "DF" << "F9"; // = "pwm216";
    devices["AT90PWM2B"] << "pwm2b" << "41" << "DF" << "F9"; // = "pwm2b";
    devices["AT90PWM3"] << "pwm3" << "41" << "DF" << "F9"; // = "pwm3"; ;
    devices["AT90PWM316"] << "pwm316" << "41" << "DF" << "F9"; // = "pwm316";
    devices["AT90PWM3B"] << "pwm3b" << "41" << "DF" << "F9"; // = "pwm3b";
    devices["AT90S1200"] << "1200" << "EF" << "none" << "none"; // = "1200";
    devices["AT90S2313"] << "2313" << "EF" << "none" << "none"; // = "2313";
    devices["AT90S2333"] << "2333" << "EF" << "none" << "none"; // = "2323";
    devices["AT90S2343"] << "2343" << "EF" << "none" << "none"; // = "2343";
    devices["AT90S4414"] << "4414" << "EF" << "none" << "none"; // = "4414";
    devices["AT90S4433"] << "4433" << "EF" << "none" << "none"; // = "4433";
    devices["AT90S4434"] << "4434" << "EF" << "none" << "none"; // = "4434";
    devices["AT90S8515"] << "8515" << "EF" << "none" << "none"; // = "8515";
    devices["AT90S8535"] << "8535" << "EF" << "none" << "none"; // = "8535";
    devices["AT90USB1286"] << "usb1286" << "EF" << "C9" << "FF"; // = "usb1286";
    devices["AT90USB1287"] << "usb1287" << "EF" << "C9" << "FF"; // = "usb1287";
    devices["AT90USB162"] << "usb162" << "EF" << "C9" << "FF"; // = "usb162";
    devices["AT90USB646"] << "usb646" << "EF" << "C9" << "FF"; // = "usb646";
    devices["AT90USB647"] << "usb647" << "EF" << "C9" << "FF"; // = "usb647";
    devices["AT90USB82"] << "usb82" << "EF" << "C9" << "FF"; // = "usb82";
    devices["ATmega103"] << "m103" << "EF" << "none" << "none"; // = "m103";
    devices["ATmega128"] << "m128" << "EF" << "C9" << "FF"; // = "m128";
    devices["ATmega1280"] << "m1280" << "EF" << "C9" << "FF"; // = "m1280";
    devices["ATmega1281"] << "m1281" << "EF" << "C9" << "FF";
    devices["ATmega1284"] << "m1284" << "EF" << "C9" << "FF";
    devices["ATmega1284P"] << "m1284p" << "EF" << "C9" << "FF";
    devices["ATmega128A"] << "m128a" << "EF" << "C9" << "FF";
    devices["ATmega16"] << "m1280" << "EF" << "C9" << "none";
    devices["ATmega1608"] << "m1608" << "none" << "none" << "none";
    devices["ATmega1609"] << "m1608" << "none" << "none" << "none";
    devices["ATmega161"] << "m161" << "EF" << "none" << "none";
    devices["ATmega161comp"] << "m161" << "EF" << "C9" << "FF";
    devices["ATmega162"] << "m162" << "EF" << "C9" << "FF";
    devices["ATmega163"] << "m163" << "EF" << "C9" << "none";
    devices["ATmega164A"] << "m164a" << "EF" << "C9" << "FF";
    devices["ATmega164P"] << "m164p" << "EF" << "C9" << "FF";
    devices["ATmega164PA"] << "m161" << "EF" << "C9" << "FF";
    devices["ATmega165"] << "m165" << "EF" << "C9" << "FF";
    devices["ATmega165A"] << "m165a" << "EF" << "C9" << "FF";
    devices["ATmega165P"] << "m165p" << "EF" << "C9" << "FF";
    devices["ATmega165PA"] << "m165pa" << "EF" << "C9" << "FF";
    devices["ATmega168"] << "m168" << "EF" << "C9" << "FF";
    devices["ATmega168A"] << "m168a" << "EF" << "C9" << "FF";
    devices["ATmega168P"] << "m168p" << "EF" << "C9" << "FF";
    devices["ATmega168PA"] << "m168pa" << "EF" << "C9" << "FF";
    devices["ATmega168PB"] << "m168pb" << "EF" << "C9" << "FF";
    devices["ATmega169"] << "m169" << "EF" << "C9" << "FF";
    devices["ATmega169A"] << "m169a" << "EF" << "C9" << "FF";
    devices["ATmega169P"] << "m169p" << "EF" << "C9" << "FF";
    devices["ATmega169PA"] << "m169pa" << "EF" << "C9" << "FF";
    devices["ATmega169"] << "m16a" << "EF" << "C9" << "none";
    devices["ATmega16U2"] << "m16u2" << "EF" << "C9" << "FF";
    devices["ATmega16U4"] << "m16u4" << "EF" << "C9" << "FF";
    devices["ATmega2560"] << "m2560" << "EF" << "C9" << "FF";
    devices["ATmega2561"] << "m2561" << "EF" << "C9" << "FF";
    devices["ATmega32"] << "m32" << "EF" << "C9" << "none";
    devices["ATmega3208"] << "m3208" << "none" << "none" << "none";
    devices["ATmega3209"] << "m3208" << "none" << "none" << "none";
    devices["ATmega324A"] << "m324a" << "EF" << "C9" << "FF";
    devices["ATmega324P"] << "m324p" << "EF" << "C9" << "FF";
    devices["ATmega324PA"] << "m324pa" << "EF" << "C9" << "FF";
    devices["ATmega324PB"] << "m324pb" << "EF" << "C9" << "FF";
    devices["ATmega325"] << "m325" << "EF" << "C9" << "FF";
    devices["ATmega3250"] << "m3250" << "EF" << "C9" << "FF";
    devices["ATmega3250A"] << "m3250a" << "EF" << "C9" << "FF";
    devices["ATmega3250P"] << "m3250p" << "EF" << "C9" << "FF";
    devices["ATmega3250PA"] << "m3250pa" << "EF" << "C9" << "FF";
    devices["ATmega328"] << "m328" << "EF" << "C9" << "FF";
    devices["ATmega328P"] << "m328p" << "EF" << "C9" << "FF";
    devices["ATmega328PB"] << "m328pb" << "EF" << "C9" << "FF";
    devices["ATmega329"] << "m329" << "EF" << "C9" << "FF";
    devices["ATmega3290"] << "m3290" << "EF" << "C9" << "FF";
    devices["ATmega3290A"] << "m3290a" << "EF" << "C9" << "FF";
    devices["ATmega3290P"] << "m3290p" << "EF" << "C9" << "FF";
    devices["ATmega3290PA"] << "m3290pa" << "EF" << "C9" << "FF";
    devices["ATmega329A"] << "m329a" << "EF" << "C9" << "FF";
    devices["ATmega329P"] << "m329p" << "EF" << "C9" << "FF";
    devices["ATmega329PA"] << "m329pa" << "EF" << "C9" << "FF";
    devices["ATmega32A"] << "m32a" << "EF" << "C9" << "none";
    devices["ATmega32M1"] << "m32m1" << "EF" << "C9" << "FF";
    devices["ATmega32U2"] << "m32u2" << "EF" << "C9" << "FF";
    devices["ATmega32U4"] << "m32u4" << "EF" << "C9" << "FF";
    devices["ATmega406"] << "m406" << "EF" << "C9" << "none";
    devices["ATmega48"] << "m48" << "EF" << "C9" << "FF";
    devices["ATmega4808"] << "m4808" << "none" << "none" << "none";
    devices["ATmega4809"] << "m4809" << "none" << "none" << "none";
    devices["ATmega48A"] << "m48a" << "EF" << "C9" << "FF";
    devices["ATmega48P"] << "m48p" << "EF" << "C9" << "FF";
    devices["ATmega48PA"] << "m48pa" << "EF" << "C9" << "FF";
    devices["ATmega48PB"] << "m48pb" << "EF" << "C9" << "FF";
    devices["ATmega64"] << "m64" << "EF" << "C9" << "FF";
    devices["ATmega640"] << "m640" << "EF" << "C9" << "FF";
    devices["ATmega644"] << "m644" << "EF" << "C9" << "FF";
    devices["ATmega644A"] << "m644a" << "EF" << "C9" << "FF";
    devices["ATmega644P"] << "m644p" << "EF" << "C9" << "FF";
    devices["ATmega644PA"] << "m644pa" << "EF" << "C9" << "FF";
    devices["ATmega645"] << "m645" << "EF" << "C9" << "FF";
    devices["ATmega6450"] << "m6450" << "EF" << "C9" << "FF";
    devices["ATmega6450A"] << "m6450a" << "EF" << "C9" << "FF";
    devices["ATmega6450P"] << "m6450p" << "EF" << "C9" << "FF";
    devices["ATmega645A"] << "m645a" << "EF" << "C9" << "FF";
    devices["ATmega645P"] << "m645p" << "EF" << "C9" << "FF";
    devices["ATmega649"] << "m649" << "EF" << "C9" << "FF";
    devices["ATmega6490"] << "m6490" << "EF" << "C9" << "FF";
    devices["ATmega6490A"] << "m6490a" << "EF" << "C9" << "FF";
    devices["ATmega6490P"] << "m6490p" << "EF" << "C9" << "FF";
    devices["ATmega649A"] << "m649a" << "EF" << "C9" << "FF";
    devices["ATmega649P"] << "m649p" << "EF" << "C9" << "FF";
    devices["ATmega64A"] << "m64a" << "EF" << "C9" << "FF";
    devices["ATmega64M1"] << "m64m1" << "EF" << "C9" << "FF";
    devices["ATmega8"] << "m8" << "EF" << "C9" << "none";
    devices["ATmega808"] << "m808" << "none" << "none" << "none";
    devices["ATmega809"] << "m809" << "none" << "none" << "none";
    devices["ATmega8515"] << "m8515" << "EF" << "C9" << "none";
    devices["ATmega8535"] << "m8535" << "EF" << "C9" << "none";
    devices["ATmega88"] << "m88" << "EF" << "C9" << "FF";
    devices["ATmega88A"] << "m88a" << "EF" << "C9" << "FF";
    devices["ATmega88P"] << "m88p" << "EF" << "C9" << "FF";
    devices["ATmega88PA"] << "m88pa" << "EF" << "C9" << "FF";
    devices["ATmega88PB"] << "m88pb" << "EF" << "C9" << "FF";
    devices["ATmega8A"] << "m8a" << "EF" << "C9" << "none";
    devices["ATmega8U2"] << "m8u2" << "EF" << "C9" << "FF";
    devices["ATtiny10"] << "t10" << "EF" << "none" << "none";
    devices["ATtiny102"] << "t102" << "EF" << "none" << "none";
    devices["ATtiny104"] << "t104" << "EF" << "none" << "none";
    devices["ATtiny11"] << "t11" << "EF" << "none" << "none";
    devices["ATtiny12"] << "t12" << "EF" << "none" << "none";
    devices["ATtiny13"] << "t13" << "EF" << "C9" << "none";
    devices["ATtiny13a"] << "t13a" << "EF" << "C9" << "none";
    devices["ATtiny15"] << "t12" << "EF" << "none" << "none";
    devices["ATtiny1604"] << "t1604" << "none" << "none" << "none";
    devices["ATtiny1606"] << "t1606" << "none" << "none" << "none";
    devices["ATtiny1607"] << "t1607" << "none" << "none" << "none";
    devices["ATtiny1614"] << "t1614" << "none" << "none" << "none";
    devices["ATtiny1616"] << "t1616" << "none" << "none" << "none";
    devices["ATtiny1617"] << "t1617" << "none" << "none" << "none";
    devices["ATtiny1624"] << "t1624" << "none" << "none" << "none";
    devices["ATtiny1626"] << "t1626" << "none" << "none" << "none";
    devices["ATtiny1627"] << "t1627" << "none" << "none" << "none";
    devices["ATtiny1634"] << "t1634" << "EF" << "C9" << "FF";
    devices["ATtiny1634R"] << "t1634r" << "EF" << "C9" << "FF";
    devices["ATtiny167"] << "t167" << "EF" << "C9" << "FF";
    devices["ATtiny20"] << "t20" << "EF" << "none" << "none";
    devices["ATtiny202"] << "t202" << "none" << "none" << "none";
    devices["ATtiny204"] << "t204" << "none" << "none" << "none";
    devices["ATtiny212"] << "t212" << "none" << "none" << "none";
    devices["ATtiny214"] << "t214" << "none" << "none" << "none";
    devices["ATtiny22"] << "t22" << "EF" << "none" << "none";
    devices["ATtiny2313"] << "t2313" << "EF" << "C9" << "FF";
    devices["ATtiny2313A"] << "2313a" << "EF" << "C9" << "FF";
    devices["ATtiny24"] << "24" << "EF" << "C9" << "FF";
    devices["ATtiny24a"] << "24a" << "EF" << "C9" << "FF";
    devices["ATtiny25"] << "25" << "EF" << "C9" << "FF";
    devices["ATtiny26"] << "26" << "EF" << "C9" << "none";
    devices["ATtiny261"] << "261" << "EF" << "C9" << "FF";
    devices["ATtiny261A"] << "261a" << "EF" << "C9" << "FF";
    devices["ATtiny28"] << "t28" << "EF" << "none" << "none";
    devices["ATtiny3216"] << "t3216" << "none" << "none" << "none";
    devices["ATtiny3217"] << "t3217" << "none" << "none" << "none";
    devices["ATtiny3224"] << "t3224" << "none" << "none" << "none";
    devices["ATtiny3226"] << "t3226" << "none" << "none" << "none";
    devices["ATtiny3227"] << "3227" << "none" << "none" << "none";
    devices["ATtiny4"] << "t4" << "EF" << "none" << "none";
    devices["ATtiny40"] << "t40" << "EF" << "none" << "none";
    devices["ATtiny402"] << "t402" << "none" << "none" << "none";
    devices["ATtiny404"] << "t404" << "none" << "none" << "none";
    devices["ATtiny406"] << "t406" << "none" << "none" << "none";
    devices["ATtiny412"] << "t412" << "none" << "none" << "none";
    devices["ATtiny414"] << "t414" << "none" << "none" << "none";
    devices["ATtiny416"] << "t416" << "none" << "none" << "none";
    devices["ATtiny417"] << "t417" << "none" << "none" << "none";
    devices["ATtiny424"] << "t424" << "none" << "none" << "none";
    devices["ATtiny426"] << "t426" << "none" << "none" << "none";
    devices["ATtiny427"] << "t427" << "none" << "none" << "none";
    devices["ATtiny4313"] << "t4313" << "EF" << "C9" << "FF";
    devices["ATtiny43U"] << "t43u" << "EF" << "C9" << "FF";
    devices["ATtiny44"] << "t44" << "EF" << "C9" << "FF";
    devices["ATtiny441"] << "t441" << "EF" << "C9" << "FF";
    devices["ATtiny44A"] << "t44a" << "EF" << "C9" << "FF";
    devices["ATtiny45"] << "t45" << "EF" << "C9" << "FF";
    devices["ATtiny461"] << "t461" << "EF" << "C9" << "FF";
    devices["ATtiny461A"] << "t461a" << "EF" << "C9" << "FF";
    devices["ATtiny48"] << "t48" << "EF" << "C9" << "FF";
    devices["ATtiny5"] << "t5" << "EF" << "none" << "none";
    devices["ATtiny804"] << "t804" << "none" << "none" << "none";
    devices["ATtiny806"] << "t806" << "none" << "none" << "none";
    devices["ATtiny807"] << "t807" << "none" << "none" << "none";
    devices["ATtiny814"] << "t814" << "none" << "none" << "none";
    devices["ATtiny816"] << "t816" << "none" << "none" << "none";
    devices["ATtiny817"] << "t817" << "none" << "none" << "none";
    devices["ATtiny824"] << "t824" << "none" << "none" << "none";
    devices["ATtiny826"] << "t826" << "none" << "none" << "none";
    devices["ATtiny827"] << "t827" << "none" << "none" << "none";
    devices["ATtiny828"] << "t828" << "EF" << "C9" << "FF";
    devices["ATtiny828R"] << "828r" << "EF" << "C9" << "FF";
    devices["ATtiny84"] << "t84" << "EF" << "C9" << "FF";
    devices["ATtiny841"] << "t841" << "EF" << "C9" << "FF";
    devices["ATtiny84a"] << "t84a" << "EF" << "C9" << "FF";
    devices["ATtiny85"] << "t85" << "EF" << "C9" << "FF";
    devices["ATtiny861"] << "t861" << "EF" << "C9" << "FF";
    devices["ATtiny861A"] << "t861a" << "EF" << "C9" << "FF";
    devices["ATtiny87"] << "t87" << "EF" << "C9" << "FF";
    devices["ATtiny88"] << "t88" << "EF" << "C9" << "FF";
    devices["ATtiny9"] << "t9" << "EF" << "none" << "none";

    foreach (QString key, devices.keys()) ui->deviceCB->addItem(key.arg(0));

    if(devices[ui->deviceCB->currentText()].at(1) == "none")
    {
        ui->lFuseTB->clear();
        ui->lFuseTB->setDisabled(true);
    }
    else
    {
        ui->lFuseTB->setEnabled(true);
        ui->lFuseTB->setText(devices[ui->deviceCB->currentText()].at(1));
    }

    if(devices[ui->deviceCB->currentText()].at(2) == "none")
    {
        ui->hFuseTB->clear();
        ui->hFuseTB->setDisabled(true);
    }
    else
    {
        ui->hFuseTB->setEnabled(true);
        ui->hFuseTB->setText(devices[ui->deviceCB->currentText()].at(2));
    }

    if(devices[ui->deviceCB->currentText()].at(3) == "none")
    {
        ui->eFuseTB->clear();
        ui->eFuseTB->setDisabled(true);
    }
    else
    {
        ui->eFuseTB->setEnabled(true);
        ui->eFuseTB->setText(devices[ui->deviceCB->currentText()].at(3));
    }

    sck_rate["375 KHz"] = "2.0";
    sck_rate["1.5 MHz"] = "0.5";
    sck_rate["750 KHz"] = "1.0";
    sck_rate["187.5 KHz"] = "4.0";
    sck_rate["93.75 KHz"] = "8.0";
    sck_rate["32 KHz"] = "20.96";
    sck_rate["16 KHz"] = "46.88";
    sck_rate["8 KHz"] = "93.75";
    sck_rate["4 KHz"] = "187.5";
    sck_rate["2 KHz"] = "375.0";
    sck_rate["1 KHz"] = "750.0";
    sck_rate["500 Hz"] = "1500.0";

}

DaDuDa::~DaDuDa()
{
    delete ui;
}

void DaDuDa::checkFuses()
{
    QFile file("lock.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line = in.readLine();

        ui->lockTB->setText(line.mid(2, 2).toUpper());

        file.close();
        file.remove();
    }

    QFile file1("lfuse.txt");
    if(file1.open(QIODevice::ReadOnly))
    {
        QTextStream in1(&file1);
        QString line1 = in1.readLine();

        ui->lFuseTB->setText(line1.mid(2, 2).toUpper());

        file1.close();
        file1.remove();
    }

    QFile file2("hfuse.txt");
    if(file2.open(QIODevice::ReadOnly))
    {
        QTextStream in2(&file2);
        QString line2 = in2.readLine();

        ui->hFuseTB->setText(line2.mid(2, 2).toUpper());

        file2.close();
        file2.remove();
    }

    QFile file3("efuse.txt");
    if(file3.open(QIODevice::ReadOnly))
    {
        QTextStream in3(&file3);
        QString line3 = in3.readLine();

        ui->eFuseTB->setText(line3.mid(2, 2).toUpper());

        file3.close();
        file3.remove();
    }
}

void DaDuDa::writeSign()
{
    if(text.contains("Device signature") && checkSign)
    {
        int signStart = text.indexOf("Device signature") + 21;
        ui->signLB->setText(text.mid(signStart, 6));

        int modStart = text.indexOf("probably") + 9;
        int modStop = text.lastIndexOf(")") - 1;
        QString dev = text.mid(modStart, (modStop - (modStart - 1)));
        foreach (QString key, devices.keys())
        {
            QString curKey = devices[key].at(0);
            if(dev == curKey)
            {
                ui->deviceCB->setCurrentText(key);
                return;
            }
        }
        checkSign = false;
    }
}

void DaDuDa::readFromStdout()
{
    QByteArray data = proc->readAllStandardOutput();
    text = QString(data);
    ui->consoleTB->moveCursor(QTextCursor::End);
    ui->consoleTB->insertPlainText(text);

    checkFuses();
    writeSign();
}

void DaDuDa::readFromStderr()
{
    QByteArray data = proc->readAllStandardError();
    text = QString(data);
    ui->consoleTB->moveCursor(QTextCursor::End);
    ui->consoleTB->insertPlainText(text);

    checkFuses();
    writeSign();
}

//Обновление списка портов
void DaDuDa::on_scanBTN_clicked()
{
    ui->portCB->clear();
    ui->portCB->addItem("usb");
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->portCB->addItem(serialPortInfo.portName());
    }
}

//Путь к файлу для записи Flash
void DaDuDa::on_openBTN_clicked()
{
    fPath = QFileDialog :: getOpenFileName (NULL, "Title", ".", "*");
    ui->fPathTB->setText(fPath);
}

//Путь к файлу для записи EEprom
void DaDuDa::on_eeOpenBTN_clicked()
{
    eePath = QFileDialog :: getOpenFileName (NULL, "Title", ".", "*");
    ui->eePathTB->setText(eePath);
}

void DaDuDa::set_shareArgs(QStringList &sarg)
{
    sarg << "-c";
    sarg << ui->programmerCB->currentText();
    sarg << "-p";
    sarg << devices[ui->deviceCB->currentText()].at(0);
    sarg << "-P";
    sarg << ui->portCB->currentText();

    if(ui->baudTB->text() != "")
    {
        sarg << "-b";
        sarg << ui->baudTB->text();
    }
    sarg << "-B";
    sarg << sck_rate[ui->sckCB->currentText()];

    if(ui->noCheckSignCHB->isChecked()) sarg << "-F";
    if(ui->noVerifyCHB->isChecked()) sarg << "-V";
    if(ui->disErrCHB->isChecked()) sarg << "-D";
}

//Проверка наличия программатора и чипа
void DaDuDa::on_checkBTN_clicked()
{
    checkSign = true;
    ui->signLB->setText("");

    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Чтение flash
void DaDuDa::on_fReadBTN_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Title", ".", "*.hex");
    mainStr = "flash:r:" + fileName + ":i";

    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Чтение Eeprom
void DaDuDa::on_eeReadBTN_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Title", ".", "*.eep");
    mainStr = "eeprom:r:" + fileName + ":i";

    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Запись Flash
void DaDuDa::on_fWriteBTN_clicked()
{
    if(ui->fPathTB->text() == "")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Title", ".", "*");
        mainStr = "flash:w:" + fileName + ":a";
        ui->fPathTB->setText(fileName);
    }
    else
    {
        mainStr = "flash:w:" + ui->fPathTB->text() + ":a";
    }

    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Записать Eeprom
void DaDuDa::on_eeWriteBTN_clicked()
{
    if(ui->eePathTB->text() == "")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Title", ".", "*");
        mainStr = "eeprom:w:" + fileName + ":a";
        ui->eePathTB->setText(fileName);
    }
    else
    {
        mainStr = "eeprom:w:" + ui->eePathTB->text() + ":a";
    }

    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Проверка Flash
void DaDuDa::on_fVerifyBTN_clicked()
{
    if(ui->fPathTB->text() == "")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Title", ".", "*");
        mainStr = "flash:v:" + fileName + ":a";
    }
    else
    {
        mainStr = "flash:v:" + ui->fPathTB->text() + ":a";
    }
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//
void DaDuDa::on_eeVerifyBTN_clicked()
{
    if(ui->eePathTB->text() == "")
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Title", ".", "*");
        mainStr = "eeprom:v:" + fileName + ":a";
    }
    else
    {
        mainStr = "eeprom:v:" + ui->eePathTB->text() + ":a";
    }
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg << mainStr;


    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Erase
void DaDuDa::on_eraseBTN_clicked()
{
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-e";

    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

void DaDuDa::on_pushButton_3_clicked(){}

//Read lock bits
void DaDuDa::on_rLockBTN_clicked()
{
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    arg <<"lock:r:lock.txt:h";

    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Write Lock
void DaDuDa::on_wLockBTN_clicked()
{
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    arg << "-U";
    QString lk = "lock:w:0x" + ui->lockTB->text() + ":m";
    arg << lk;

    mainStr = "avrdude ";
    for(const auto& i : arg )
    {
        mainStr += i;
        mainStr += " ";
    }
    mainStr += "\r\n";

    ui->consoleTB->setText(mainStr);
    mainStr = "";

    proc->start("avrdude", arg);
}

//Read fuse
void DaDuDa::on_rFuseBTN_clicked()
{
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    if(ui->eFuseTB->isEnabled())
    {
        arg << "-U";
        arg <<"efuse:r:efuse.txt:h";
    }
    if(ui->lFuseTB->isEnabled())
    {
        arg << "-U";
        arg <<"lfuse:r:lfuse.txt:h";
    }
    if(ui->hFuseTB->isEnabled())
    {
        arg << "-U";
        arg <<"hfuse:r:hfuse.txt:h";
    }

    if(ui->eFuseTB->isEnabled() || ui->lFuseTB->isEnabled()  || ui->hFuseTB->isEnabled())
    {
        mainStr = "avrdude ";
        for(const auto& i : arg )
        {
            mainStr += i;
            mainStr += " ";
        }
        mainStr += "\r\n";

        ui->consoleTB->setText(mainStr);
        mainStr = "";

        proc->start("avrdude", arg);
    }
}

//Write fuse
void DaDuDa::on_wFuseBTN_clicked(bool checked)
{
    ui->consoleTB->clear();

    QStringList arg;

    set_shareArgs(arg);

    if(ui->eFuseTB->isEnabled() || ui->lFuseTB->isEnabled()  || ui->hFuseTB->isEnabled())
    {
        if(ui->eFuseTB->isEnabled())
        {
            arg << "-U";
            QString lk = "efuse:w:0x" + ui->eFuseTB->text() + ":m";
            arg << lk;
        }

        if(ui->lFuseTB->isEnabled())
        {
            arg << "-U";
            QString lk = "lfuse:w:0x" + ui->lFuseTB->text() + ":m";
            arg << lk;
        }

        if(ui->hFuseTB->isEnabled())
        {
            arg << "-U";
            QString lk = "hfuse:w:0x" + ui->hFuseTB->text() + ":m";
            arg << lk;
        }

        mainStr = "avrdude ";
        for(const auto& i : arg )
        {
            mainStr += i;
            mainStr += " ";
        }
        mainStr += "\r\n";

        ui->consoleTB->setText(mainStr);
        mainStr = "";

        proc->start("avrdude", arg);
    }
}

//Выбор модели
void DaDuDa::on_deviceCB_currentIndexChanged(int index)
{
    if(devices[ui->deviceCB->currentText()].at(1) == "none")
    {
        ui->lFuseTB->clear();
        ui->lFuseTB->setDisabled(true);
    }
    else
    {
        ui->lFuseTB->setEnabled(true);
        ui->lFuseTB->setText(devices[ui->deviceCB->currentText()].at(1));
    }

    if(devices[ui->deviceCB->currentText()].at(2) == "none")
    {
        ui->hFuseTB->clear();
        ui->hFuseTB->setDisabled(true);
    }
    else
    {
        ui->hFuseTB->setEnabled(true);
        ui->hFuseTB->setText(devices[ui->deviceCB->currentText()].at(2));
    }

    if(devices[ui->deviceCB->currentText()].at(3) == "none")
    {
        ui->eFuseTB->clear();
        ui->eFuseTB->setDisabled(true);
    }
    else
    {
        ui->eFuseTB->setEnabled(true);
        ui->eFuseTB->setText(devices[ui->deviceCB->currentText()].at(3));
    }
}
