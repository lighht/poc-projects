#pragma once
#include <QByteArray>
#include <QSerialPort>
#include <QTimer>
class MotorStateScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool motorActive READ getIsMotorActive NOTIFY motorStateChanged)
public:
    explicit MotorStateScanner(QObject *parent = nullptr);
    ~MotorStateScanner();

public slots:
    bool getIsMotorActive();
    bool getMotorDir();

signals:
   void motorStateChanged();
   void motorDirectionChanged();
   void stopped();

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);
    void handleTimeout();

private:
    bool motorActive;
    bool motorDirection;
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QTimer m_timer;
    int rawData;

    void setIsMotorActive(bool isMotorActive);
    void setRawData(const int &data);
    void setMotorDir(const bool dir);

    signals:
       void rawDataUpdated();
};
