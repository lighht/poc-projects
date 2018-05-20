#pragma once

#include <projector_test/powermeterscanner.h>
#include <projector_test/motorstatescanner.h>

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DummyDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long index READ getIndex)
public:
    explicit DummyDataSource(QObject *parent = nullptr);
    Q_INVOKABLE double getIndex(){
        return m_index;
    }
    Q_INVOKABLE void reset();
    Q_INVOKABLE void saveData();
Q_SIGNALS:

public slots:
    Q_INVOKABLE void updateData(QAbstractSeries *pwmSeries, QAbstractSeries *msSeries);

private:
    double m_index;
    QVector<QPointF> m_ms_points;
    QVector<QPointF> m_pwm_points;
    QVector<QPointF> m_data_points;

};
