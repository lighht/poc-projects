#pragma once
#include <QObject>
#include <QtCharts/QAbstractSeries>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool reading READ reading NOTIFY readingChanged)
public:
    explicit DataReader(QObject *parent = nullptr);
    void setFileName(QString fn);
    Q_INVOKABLE bool reading(){
        return isReading;
    }
signals:
   void readingChanged();
public slots:
    Q_INVOKABLE void updateData(QAbstractSeries *pwmSeries, QAbstractSeries *msSeries);
private:
   bool isReading = false;
   QAbstractSeries *m_pwm;
   QAbstractSeries *m_ms;
   QString filename;
private slots:
   void readData();

};
