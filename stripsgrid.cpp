#include "stripsgrid.h"

#include <QGridLayout>
#include <QMouseEvent>
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

    const auto stories = random.bounded(3, 6);

    int row{0};
    {
        int column{0};
        for (auto &header : m_tableHeader)
        {
            if (column)
            {
                auto line = new QFrame;
                line->setFrameShape(QFrame::VLine);
                line->setFrameShadow(QFrame::Sunken);
                m_layout->addWidget(line, row, column++, (stories*2)+1, 1);
            }

            header = new QLabel;
            {
                auto font = header->font();
                font.setPointSize(20);
                header->setFont(font);
            }
            header->setFixedHeight(40);
            m_layout->addWidget(header, row, column++);
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

        Story story;
        story.widget = new StripWidget{StripWidget::Story};
        story.widget->setTitle(QString("ATC-%0").arg(random.bounded(1000, 5000)));

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

                call_n_times(random.bounded(4), [&]()
                {
                    auto widget = new StripWidget{StripWidget::Subtask};
                    widget->setTitle(QString("ATC-%0").arg(random.bounded(1000, 5000)));
                    widget->setPoints(std::array<int, 5>{1,3,5,8,13}[random.bounded(5)]);
                    widget->setOwner(std::array<const char *, 5>{"DB", "KW", "BK", "MS", "AS"}[random.bounded(5)]);
                    connect(widget, &StripWidget::pointsChanged, this, &StripsGrid::updatePoints);
                    test.layout->addWidget(widget);
                    test.widgets.push_back(widget);
                });

                m_layout->addLayout(test.layout, row, column*2);
            }
        }

        m_stories.push_back(story);

        row++;
    });

    m_layout->addItem(new QSpacerItem{0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding}, ++row, 0, 1, 5);

    updatePoints();
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
                    if (subtaskWidget->isInDragArea(event->pos() - subtaskWidget->pos()))
                    {
                        m_draggedWidget = subtaskWidget;
                        m_draggedWidget->beginDrag();

                        m_dragWidget = new StripWidget{StripWidget::DraggingSubtask, *m_draggedWidget, this};
                        m_dragWidget->move(m_draggedWidget->pos());
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
        m_draggedWidget->endDrag();
        m_draggedWidget = nullptr;

        delete m_dragWidget;
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

            updatePoints();
        }

        m_dragWidget->move(event->pos() - m_dragOffset);
    }

    QWidget::mouseMoveEvent(event);
}

void StripsGrid::updatePoints()
{
    std::array<int, 5> sumsPerColumn{0,0,0,0,0};
    m_pointsPerStory.clear();

    for (const auto &story : m_stories)
    {
        int storyPoints = 0;

        auto &test = m_pointsPerStory[story.widget->title()];

        for (int i = 0; i < 4; i++)
        {
            int columnSum = 0;
            for (const auto *widget : story.columns[i].widgets)
            {
                columnSum += widget->points();
                test[widget->title()] = widget->points();
            }
            sumsPerColumn[i+1] += columnSum;
            storyPoints += columnSum;
        }
        sumsPerColumn[0] += storyPoints;
        story.widget->setPoints(storyPoints);
    }

    std::array<QString, 5> texts{
        tr("Stories (%0)"),
        tr("To Do (%0)"),
        tr("In Progress (%0)"),
        tr("In Review (%0)"),
        tr("Done (%0)")
    };

    for (int i = 0; i < 5; i++)
        m_tableHeader[i]->setText(texts[i].arg(sumsPerColumn[i]));

    emit pointsPerStoryChanged(m_pointsPerStory);
}
