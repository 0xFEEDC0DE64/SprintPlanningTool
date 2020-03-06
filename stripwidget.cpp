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

void StripWidget::setTitle(const QString &title)
{
    m_ui->title->setText(title);
}

void StripWidget::setDescription(const QString &description)
{
    m_ui->description->setText(description);
}

void StripWidget::setPoints(int points)
{
    m_ui->points->setText(QString::number(points));
}

void StripWidget::setOwner(const QString &owner)
{
    m_ui->owner->setText(owner);
}
