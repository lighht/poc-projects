#pragma once

#include <projector_test/powermeterscanner.h>
#include <projector_test/motorstatescanner.h>

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long index READ getIndex)
public:
    explicit DataSource(QObject *parent = nullptr);
    Q_INVOKABLE double getIndex(){
        return m_index;
    }
    Q_INVOKABLE void saveData();
    Q_INVOKABLE void reset();
Q_SIGNALS:

public slots:
    Q_INVOKABLE void updateData(QAbstractSeries *pwmSeries, QAbstractSeries *msSeries);

private:
    PowerMeterScanner *m_pwm;
    MotorStateScanner *m_ms;
    QVector<QPointF> m_pwm_points;
    QVector<QPointF> m_ms_points;
    double m_index;
    int8_t m_ms_value;
    double m_pwm_value;
    QVector<QPointF> m_data_points;

    void updatePowerMeterData(QAbstractSeries *series);
    void updateMotorScannerData(QAbstractSeries *series);
};
