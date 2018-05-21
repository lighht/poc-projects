#include "datareader.h"
#include <iostream>
#include <vector>
#include <QTimer>

#include <QFile>
#include <QTextStream>
#include <QtCharts/QXYSeries>
DataReader::DataReader(QObject *parent) : QObject(parent),
    filename("scan")
{

}
void DataReader::setFileName(QString fn){
    filename = fn;
}
void DataReader::updateData(QAbstractSeries *pwm, QAbstractSeries *ms){
    isReading = true;
    m_pwm = pwm;
    m_ms = ms;
    QTimer::singleShot(100, this, &DataReader::readData);
    return;
}

void DataReader::readData(){
    QFile scan_file(filename);
    if (!scan_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr<<"file "<<filename.toStdString()<<" not found";
        return;
    }

    QTextStream stream(&scan_file);
    QXYSeries *msSeries = static_cast<QXYSeries *>(m_ms);
    QXYSeries *pwmSeries = static_cast<QXYSeries *>(m_pwm);
    QVector<QPointF> ms;
    QVector<QPointF> pwm;
    ms.reserve(10000);
    pwm.reserve(10000);
    std::vector<int> errors;
    {
        int i = 0;
        int m_current = 0;
        int m_previous = 0;
        int p_current;
        bool motor_high = false;
        bool pwm_peaked = false;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.startsWith("#") || line.startsWith(":"))
                continue;
            QStringList values = line.split(",", QString::SkipEmptyParts);
            m_current = values[0].toInt();
            p_current = values[1].toDouble();
            if(m_previous < m_current){
                motor_high = true;
            }
            if(m_previous > m_current){
                if(!pwm_peaked){
                    errors.push_back(i);
                }
                motor_high = false;
                pwm_peaked = false;
            }
            if(motor_high & !pwm_peaked){
                if(p_current>150){
                    pwm_peaked = true;
                }
            }
            ms.append(QPointF(i, m_current));
            pwm.append(QPointF(i, p_current));
            i++;
            m_previous = m_current;
        }
        scan_file.close();
    }
    isReading = false;
    msSeries->replace(ms);
    pwmSeries->replace(pwm);
    for(auto const& i : errors){
        std::cout<<i<<"\n";
    }
    emit readingChanged();
}
