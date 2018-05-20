#include <projector_test/motorstatescanner.h>
#include <QMetaEnum>
#include <exception>
#include <iostream>

//const uint motorResetTime = 10000; //10 seconds
// The motor status is reset to active after 10 milliseconds.
const uint motorResetTime = 100;
MotorStateScanner::MotorStateScanner(QObject *parent) :
    QObject(parent),
    motorActive(false),
    motorDirection(false),
    m_serialPort(new QSerialPort),
    rawData(0x00)
{
    const QString serialPortName = "/dev/ttyUSB0";
    m_serialPort->setPortName(serialPortName);

    const int serialPortBaudRate = QSerialPort::Baud115200;
    m_serialPort->setBaudRate(serialPortBaudRate);
    if (!m_serialPort->open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open port with error:" +m_serialPort->errorString().toStdString());
    }
    connect(m_serialPort, &QSerialPort::readyRead, this, &MotorStateScanner::handleReadyRead);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &MotorStateScanner::handleError);
    connect(&m_timer, &QTimer::timeout, this, &MotorStateScanner::handleTimeout);
}

MotorStateScanner::~MotorStateScanner(){
    m_serialPort->close();
    emit stopped();
}

void MotorStateScanner::handleReadyRead()
{
   m_timer.start(motorResetTime);
   setRawData(std::atoi(m_serialPort->readAll().data()) & 0x09);
   if(motorActive == false)
       return;
   motorActive = false;
//   emit motorStateChanged();

}

//If for a duration of motorResetTime, no input: motor is moving
void MotorStateScanner::handleTimeout()
{
    if(motorActive == true)
        return;
    motorActive = true;
//    emit motorStateChanged();

}

void MotorStateScanner::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        throw std::runtime_error("An I/O error occurred while reading the data from port "+
                                 m_serialPort->portName().toStdString()+
                                 "\n Error:"+
                                 m_serialPort->errorString().toStdString());
    }
    else{
        QMetaEnum serialPortError2String = QMetaEnum::fromType<QSerialPort::SerialPortError>();
        throw std::runtime_error(serialPortError2String.valueToKey(serialPortError));
    }
}

void MotorStateScanner::setRawData(const int &data){
    if(rawData==data){
        return;
    }
    rawData = data;
    setMotorDir(data & 0x08);
}

bool MotorStateScanner::getIsMotorActive(){
    return motorActive;
}

void MotorStateScanner::setMotorDir(bool dir){
    if(motorActive==dir)
        return;
   motorDirection = dir;
//   emit motorDirectionChanged();
}

bool MotorStateScanner::getMotorDir(){
    return motorDirection;
}
