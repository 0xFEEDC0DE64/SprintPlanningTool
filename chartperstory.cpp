#include "chartperstory.h"

#include <QPieSeries>
#include <QGraphicsLayout>

#include "donutbreakdownchart.h"

QT_CHARTS_USE_NAMESPACE

ChartPerStory::ChartPerStory(QWidget *parent) :
    QChartView{parent}
{
    setRenderHint(QPainter::Antialiasing);
}

void ChartPerStory::setPoints(const QMap<QString, QMap<QString, int> > &points)
{
    auto chart = new DonutBreakdownChart;
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    chart->setTitle("Story points with their subtasks");
    chart->legend()->setAlignment(Qt::AlignBottom);
    for (auto iter = std::begin(points); iter != std::end(points); iter++)
    {
        auto series = new QtCharts::QPieSeries();
        series->setName(iter.key());

        for (auto iter_ = std::begin(iter.value()); iter_ != std::end(iter.value()); iter_++)
            series->append(iter_.key(), iter_.value());

        chart->addBreakdownSeries(series, Qt::red);
    }
    setChart(chart);
    delete m_chart;
    m_chart = chart;
}
