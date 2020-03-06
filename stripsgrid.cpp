#include "stripsgrid.h"

#include <QDebug>
#include <QGridLayout>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QSpacerItem>
#include <QRandomGenerator>
#include <QDateTime>

#include <algorithm>

#include "stripwidget.h"
#include "flowlayout.h"

namespace {
template<typename T>
void call_n_times(int count, T func)
{
    for (int i = 0; i < count; i++)
        func();
}
}

StripsGrid::StripsGrid(QWidget *parent) :
    QWidget{parent}
{
    m_layout = new QGridLayout{this};

    QRandomGenerator random((qint32)(QDateTime::currentMSecsSinceEpoch()));

    const auto stories = random.bounded(3, 10);

    int row{0};
    {
        int column{0};
        for (const auto text : {"Story", "To Do:", "In Progress:", "In Review:", "Done:"})
        {
            if (column)
            {
                auto line = new QFrame;
                line->setFrameShape(QFrame::VLine);
                line->setFrameShadow(QFrame::Sunken);
                m_layout->addWidget(line, row, column++, (stories*2)+1, 1);
            }

            auto label = new QLabel{text};
            {
                auto font = label->font();
                font.setPointSize(24);
                label->setFont(font);
            }
            label->setFixedHeight(40);
            m_layout->addWidget(label, row, column++);
        }
    }

    row++;

    call_n_times(stories, [&]()
    {
        {
            auto line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            m_layout->addWidget(line, row, 0, 1, 9);
        }

        row++;

        int storyPoints{0};

        Story story;
        story.widget = new StripWidget;
        story.widget->setTitle(QString("ATC-%0").arg(random.bounded(1000, 5000)));
        story.widget->setStyleSheet("StripWidget { background-color: #AFA; }");

        for (int column = 0; column < 5; column++)
        {
            if (column == 0)
            {
                auto layout = new QVBoxLayout;
                layout->addSpacing(0);
                layout->addWidget(story.widget);
                layout->addStretch(1);
                m_layout->addLayout(layout, row, column*2);
            }
            else
            {
                auto &test = story.columns[column-1];
                test.layout = new FlowLayout;

                call_n_times(random.bounded(6), [&]()
                {
                    auto widget = new StripWidget;
                    widget->setTitle(QString("ATC-%0").arg(random.bounded(1000, 5000)));
                    {
                        const auto points = std::array<int, 5>{1,3,5,8,13}[random.bounded(5)];
                        widget->setPoints(points);
                        storyPoints += points;
                    }
                    widget->setOwner(std::array<const char *, 5>{"DB", "KW", "BK", "MS", "AS"}[random.bounded(5)]);
                    widget->setStyleSheet("StripWidget { background-color: #FCC; }");
                    test.layout->addWidget(widget);
                    test.widgets.push_back(widget);
                });

                m_layout->addLayout(test.layout, row, column*2);
            }
        }

        story.widget->setPoints(storyPoints);
        m_stories.push_back(story);

        row++;
    });

    m_layout->addItem(new QSpacerItem{0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding}, ++row, 0, 1, 5);
}

void StripsGrid::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        !m_dragWidget)
    {
        for (const auto &story : m_stories)
        {
            for (const auto &column : story.columns)
            {
                for (auto *subtaskWidget : column.widgets)
                {
                    if (subtaskWidget->startDragging(event->pos() - subtaskWidget->pos()))
                    {
                        m_draggedWidget = subtaskWidget;
                        m_draggedWidget->setGraphicsEffect(new QGraphicsBlurEffect);

                        m_dragWidget = new StripWidget{this};
                        m_dragWidget->setTitle(m_draggedWidget->title());
                        m_dragWidget->setDescription(m_draggedWidget->description());
                        m_dragWidget->setPoints(m_draggedWidget->points());
                        m_dragWidget->setOwner(m_draggedWidget->owner());
                        m_dragWidget->setStyleSheet(m_draggedWidget->styleSheet());
                        m_dragWidget->move(m_draggedWidget->pos());
                        m_dragWidget->setGraphicsEffect(new QGraphicsDropShadowEffect);
                        m_dragWidget->show();

                        m_dragOffset = event->pos() - m_draggedWidget->pos();
                        goto after;
                    }
                }
            }
        }
    }

    after:

    QWidget::mousePressEvent(event);
}

void StripsGrid::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        m_dragWidget)
    {
        m_draggedWidget->setGraphicsEffect(nullptr);
        m_draggedWidget = nullptr;
        m_dragWidget->deleteLater();
        m_dragWidget = nullptr;
    }

    QWidget::mouseReleaseEvent(event);
}

void StripsGrid::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragWidget)
    {
        Story::Column *currentColumn{};

        for (auto &story : m_stories)
        {
            for (auto &column : story.columns)
            {
                if (std::find(column.widgets.begin(), column.widgets.end(), m_draggedWidget) != column.widgets.end())
                    currentColumn = &column;
            }
        }

        Story::Column *newColumn{};
        for (auto &story : m_stories)
        {
            for (auto &column : story.columns)
            {
                if (column.layout->geometry().contains(event->pos()))
                    newColumn = &column;
            }
        }

        if (currentColumn != newColumn && newColumn != nullptr)
        {
            if (currentColumn)
            {
                currentColumn->layout->removeWidget(m_draggedWidget);
                currentColumn->widgets.erase(std::remove(currentColumn->widgets.begin(), currentColumn->widgets.end(), m_draggedWidget), currentColumn->widgets.end());

                currentColumn->layout->update();
            }

            newColumn->layout->addWidget(m_draggedWidget);
            newColumn->widgets.push_back(m_draggedWidget);
            newColumn->layout->update();
        }

        m_dragWidget->move(event->pos() - m_dragOffset);
    }

    QWidget::mouseMoveEvent(event);
}
