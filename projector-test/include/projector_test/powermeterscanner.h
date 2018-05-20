#pragma once
#include <QByteArray>
#include <QSerialPort>

#include <QTimer>

class PowerMeterScanner: public QObject
{
Q_OBJECT
Q_PROPERTY(double pwmData READ get_reading)
public:
    explicit PowerMeterScanner(QObject *parent = nullptr);
    ~PowerMeterScanner();
    double get_reading();
private:
    QTimer timer;
    char* hdrbuf, *buf;
    int fd;
    ssize_t sz;
    double reading;
    size_t dev_write(const char *cmd);
    char* dev_read();

private slots:
    void update_reading();
signals:
    void timeout();
};
