#pragma once

#include <QtCharts/QPieSlice>

QT_CHARTS_BEGIN_NAMESPACE
class QPieSeries;
QT_CHARTS_END_NAMESPACE

class MainSlice : public QtCharts::QPieSlice
{
    Q_OBJECT

public:
    explicit MainSlice(QtCharts::QPieSeries *breakdownSeries, QObject *parent = 0);

    QtCharts::QPieSeries *breakdownSeries() const;

    void setName(QString name);
    QString name() const;

public Q_SLOTS:
    void updateLabel();

private:
    QtCharts::QPieSeries *m_breakdownSeries;
    QString m_name;
};
