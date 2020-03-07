#pragma once

#include <QtCharts/QChartView>

class DonutBreakdownChart;

class ChartPerStory : public QtCharts::QChartView
{
    Q_OBJECT

public:
    explicit ChartPerStory(QWidget *parent = nullptr);

    void setPoints(const QMap<QString, QMap<QString, int>> &points);

private:
    DonutBreakdownChart *m_chart{};
};
