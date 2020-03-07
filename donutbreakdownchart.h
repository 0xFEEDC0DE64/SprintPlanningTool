#pragma once

#include <QtCharts/QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QPieSeries;
QT_CHARTS_END_NAMESPACE

class DonutBreakdownChart : public QtCharts::QChart
{
public:
    explicit DonutBreakdownChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());

    void addBreakdownSeries(QtCharts::QPieSeries *series, QColor color);

private:
    void recalculateAngles();
    void updateLegendMarkers();

private:
    QtCharts::QPieSeries *m_mainSeries;
};
