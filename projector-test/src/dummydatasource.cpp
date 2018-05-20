// Serious re-write necessary. Need to write it as interface-implementation
// in combination with datasource.cpp. Repetitive and unsafe code here

#include <projector_test/dummydatasource.h>

#include <QtCharts/QXYSeries>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>

#include <QFile>
#include <QVector>
#include <QDateTime>
#include <QTextStream>
QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)
const uint8_t POINTS = 255;


DummyDataSource::DummyDataSource(QObject *parent) :
    QObject(parent),
    m_index(0)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
    m_pwm_points.reserve(POINTS);
    m_ms_points.reserve(POINTS);
}

void DummyDataSource::updateData(QAbstractSeries *pwmSeries, QAbstractSeries *msSeries){
    QXYSeries *xySeries = static_cast<QXYSeries *>(pwmSeries);
    qreal x(0);
    qreal y(0);
    y = qSin(M_PI / 50 * m_index) + 0.5 + QRandomGenerator::global()->generateDouble();
    x = m_index;
    if(m_index < POINTS){
        m_pwm_points.append(QPointF(x, y));
     }
    else{
        std::rotate(m_pwm_points.begin(), m_pwm_points.begin()+1, m_pwm_points.end());
        m_pwm_points.back() = QPointF(x,y);
    }
     xySeries->replace(m_pwm_points);

    QXYSeries *xy2Series = static_cast<QXYSeries *>(msSeries);
    qreal y2(0);
    y2 = qCos(M_PI / 50 * m_index) + 0.5 + QRandomGenerator::global()->generateDouble();
    if(m_index < POINTS){
        m_ms_points.append(QPointF(x, y2));
    }
    else{
        std::rotate(m_ms_points.begin(), m_ms_points.begin()+1, m_ms_points.end());
        m_ms_points.back() = QPointF(x,y2);
    }
    xy2Series->replace(m_ms_points);
    m_data_points.append(QPointF(y2, y));
    m_index++;
}

void DummyDataSource::saveData(){
    QString filename = "scan"+QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QFile fileout(filename);
    if (fileout.open(QFile::ReadWrite | QFile::Text)){
     QTextStream out(&fileout);
     for (auto iter = m_data_points.begin(); iter != m_data_points.end(); iter++){
         out << QString::number(iter->x(), 'f', 0) <<"," <<QString::number(iter->y(), 'f', 2) <<"\n";
     }
     fileout.close();
    }
}

void DummyDataSource::reset(){
    m_index = 0;
    m_ms_points.clear();
    m_ms_points.reserve(POINTS);

    m_pwm_points.clear();
    m_pwm_points.reserve(POINTS);

    m_data_points.clear();
    m_data_points.reserve(POINTS);
}
