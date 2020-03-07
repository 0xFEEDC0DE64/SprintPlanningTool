#pragma once

#include <QtCharts/QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

class ChartPerUser : public QtCharts::QChartView
{
    Q_OBJECT

public:
    explicit ChartPerUser(QWidget *parent = nullptr);

    void setPoints(const QMap<QString, int> &points);

private:
    QtCharts::QChart *m_chart{};
};
