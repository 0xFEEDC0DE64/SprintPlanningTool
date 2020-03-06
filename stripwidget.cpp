#include "stripwidget.h"
#include "ui_stripwidget.h"

StripWidget::StripWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::StripWidget>()}
{
    m_ui->setupUi(this);

    setMinimumSize(size());
    setMaximumSize(size());
    setFixedSize(size());
}

StripWidget::~StripWidget() = default;

bool StripWidget::startDragging(const QPoint &pos) const
{
    return m_ui->title->geometry().contains(pos);
}

QString StripWidget::title() const
{
    return m_ui->title->text();
}

void StripWidget::setTitle(const QString &title)
{
    m_ui->title->setText(title);
}

QString StripWidget::description() const
{
    return m_ui->description->text();
}

void StripWidget::setDescription(const QString &description)
{
    m_ui->description->setText(description);
}

int StripWidget::points() const
{
    return m_ui->points->text().toInt();
}

void StripWidget::setPoints(int points)
{
    m_ui->points->setText(QString::number(points));
}

QString StripWidget::owner() const
{
    return m_ui->owner->text();
}

void StripWidget::setOwner(const QString &owner)
{
    m_ui->owner->setText(owner);
}
