#include "chartperuser.h"

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QGraphicsLayout>

QT_CHARTS_USE_NAMESPACE

ChartPerUser::ChartPerUser(QWidget *parent) :
    QChartView{parent}
{
    setRenderHint(QPainter::Antialiasing);
}

void ChartPerUser::setPoints(const QMap<QString, int> &points)
{
    auto chart = new QChart;
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);
    chart->setTitle("Story points per owner");
    chart->legend()->setAlignment(Qt::AlignBottom);

    auto series = new QBarSeries();
    for (auto iter = std::begin(points); iter != std::end(points); iter++)
    {
        auto set = new QBarSet(iter.key());
        *set << iter.value();
        series->append(set);
    }
    chart->addSeries(series);

    setChart(chart);
    delete m_chart;
    m_chart = chart;
}
