/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <projector_test/datasource.h>

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>


#include <QFile>
#include <QVector>
#include <QDateTime>
#include <QTextStream>

#include <iostream>
#include <limits>
QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)
const uint8_t POINTS = 255;
DataSource::DataSource(QObject *parent) :
    QObject(parent),
    m_index(0),
    m_ms_value(0),
    m_pwm_value(0)
{
    try {
        m_pwm = std::make_unique<PowerMeterScanner>(this);
    } catch (std::exception) {
        m_pwm = nullptr;
    }
    try {
        m_ms = std::make_unique<MotorStateScanner>(this);
    } catch (std::exception) {
        m_ms = nullptr;
    }
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
    m_pwm_points.reserve(POINTS);
    m_ms_points.reserve(POINTS);
    m_data_points.reserve(POINTS);
}
void DataSource::updateData(QAbstractSeries *pwmSeries, QAbstractSeries *msSeries){
    updatePowerMeterData(pwmSeries);
    updateMotorScannerData(msSeries);
    m_data_points.append(QPointF(m_ms_value, m_pwm_value));
    m_index++;
}
void DataSource::updatePowerMeterData(QAbstractSeries *series)
{
    QXYSeries *xySeries = static_cast<QXYSeries *>(series);
    qreal x(0);
    qreal y(0);

    if(m_pwm!=nullptr){
        try{
            m_pwm_value = m_pwm->get_reading();//qSin(M_PI / 50 * m_index) + 0.5 + QRandomGenerator::global()->generateDouble();
        } catch(std::exception){
            m_pwm_value = 0;
        }
    }
    else{
        m_pwm_value = 0;
    }
    y = m_pwm_value;
    x = m_index;

    if(m_index < POINTS){
        m_pwm_points.append(QPointF(x, y));
    }
    else {
        std::rotate(m_pwm_points.begin(), m_pwm_points.begin()+1, m_pwm_points.end());
        m_pwm_points.back() = QPointF(x,y);
    }
    xySeries->replace(m_pwm_points);
}

void DataSource::updateMotorScannerData(QAbstractSeries *series){
    QXYSeries *xySeries = static_cast<QXYSeries *>(series);
    qreal x(0);
    qreal y(0);
    //This if else can be optimized
    if(m_ms!=nullptr){
        try {
            if(m_ms->getIsMotorActive()){
                m_ms_value = m_ms->getMotorDir()?1:-1;
            }
            else{
                m_ms_value = 0;
            }
        } catch (std::exception) {
            m_ms_value = -2;
        }
    }
    else{
        m_ms_value = -2;
    }


    x = m_index;
    y = m_ms_value;
    if(m_index < POINTS){
        m_ms_points.append(QPointF(x, y));
    }
    else {
        std::rotate(m_ms_points.begin(), m_ms_points.begin()+1, m_ms_points.end());
        m_ms_points.back() = QPointF(x,y);
    }
    xySeries->replace(m_ms_points);
}

void DataSource::saveData(){
    QString filename = "scan"+QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QFile fileout(filename);
    if (fileout.open(QFile::ReadWrite | QFile::Text)){
        QTextStream out(&fileout);
        for (auto iter = m_data_points.begin(); iter != m_data_points.end(); iter++){
         out << QString::number(iter->x(), 'f', 0) <<"," <<QString::number(iter->y(), 'f', 4) <<"\n";
        }
        fileout.close();
    }
}

void DataSource::reset(){
    m_index = 0;
    m_ms_points.clear();
    m_ms_points.reserve(POINTS);

    m_pwm_points.clear();
    m_pwm_points.reserve(POINTS);

    m_data_points.clear();
    m_data_points.reserve(POINTS);
}
