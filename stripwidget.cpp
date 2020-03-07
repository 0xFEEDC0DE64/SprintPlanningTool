#include "stripwidget.h"
#include "ui_stripwidget.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>

StripWidget::StripWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::StripWidget>()}
{
    m_ui->setupUi(this);

    setMinimumSize(size());
    setMaximumSize(size());
    setFixedSize(size());
}

StripWidget::StripWidget(StripWidget::Story_t, QWidget *parent) :
    StripWidget{parent}
{
    m_ui->points->setReadOnly(true);
    m_ui->points->setMaximum(9999);

    setStyleSheet("StripWidget { background-color: #AFA; }");
}

StripWidget::StripWidget(StripWidget::Subtask_t, QWidget *parent) :
    StripWidget{parent}
{
    setStyleSheet("StripWidget { background-color: #FCC; }");
}

StripWidget::StripWidget(StripWidget::DraggedSubtask_t, QWidget *parent) :
    StripWidget{Subtask, parent}
{
    setGraphicsEffect(new QGraphicsDropShadowEffect);
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
    return m_ui->points->value();
}

void StripWidget::setPoints(int points)
{
    m_ui->points->setValue(points);
}

QString StripWidget::owner() const
{
    return m_ui->owner->currentText();
}

void StripWidget::setOwner(const QString &owner)
{
    m_ui->owner->setCurrentIndex(m_ui->owner->findText(owner));
}

void StripWidget::beginDrag()
{
    setGraphicsEffect(new QGraphicsOpacityEffect);
}

void StripWidget::endDrag()
{
    setGraphicsEffect(nullptr);
}
