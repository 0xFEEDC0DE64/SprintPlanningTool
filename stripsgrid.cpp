#include "stripsgrid.h"

#include <QDebug>
#include <QGridLayout>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QSpacerItem>
#include <QRandomGenerator>
#include <QDateTime>

#include "stripwidget.h"
#include "flowlayout.h"

namespace {
QDebug &operator<<(QDebug &debug, const QMouseEvent &event)
{
    return debug << "QMouseEvent(" << event.pos() << event.source() << event.flags() << ")";
}

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
            auto label = new QLabel{text};
            {
                auto font = label->font();
                font.setPointSize(24);
                label->setFont(font);
            }
            label->setFixedHeight(40);
            m_layout->addWidget(label, row, column++);

            {
                auto line = new QFrame;
                line->setFrameShape(QFrame::VLine);
                line->setFrameShadow(QFrame::Sunken);
                m_layout->addWidget(line, row, column++, (stories*2)+1, 1);
            }
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

        auto storyWidget = new StripWidget;
        storyWidget->setTitle(QString("ATC-%0").arg(random.bounded(1000, 5000)));
        storyWidget->setStyleSheet("StripWidget { background-color: #AFA; }");

        for (int column = 0; column < 5; column++)
        {
            if (column == 0)
            {
                auto layout = new QVBoxLayout;
                layout->addSpacing(0);
                layout->addWidget(storyWidget);
                layout->addStretch(1);
                m_layout->addLayout(layout, row, column*2);
            }
            else
            {
                int count = random.bounded(0, 5);
                if (!count)
                    continue;

                auto layout = new FlowLayout;

                call_n_times(count, [&]()
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
                    layout->addWidget(widget);
                });

                m_layout->addLayout(layout, row, column*2);
            }
        }

        storyWidget->setPoints(storyPoints);

        row++;
    });

    m_layout->addItem(new QSpacerItem{0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding}, ++row, 0, 1, 5);

    m_widget = new StripWidget{this};
    m_widget->setStyleSheet("StripWidget { background-color: #88FFFF; }");
    m_widget->move(100, 100);

//    auto widget = new QWidget;
//    setCentralWidget(widget);

//    m_layout = new QGridLayout{widget};
//    const auto addColumn = [&](const QString &name)
//    {
//        auto label = new QLabel{name};
//        {
//            auto font = label->font();
//            font.setPointSize(30);
//            label->setFont(font);
//        }
//        m_layout->addWidget(label, 0, m_layout->columnCount());
//    };
//    addColumn("To Do");
//    addColumn("In Progress");
//    addColumn("In Review");
//    addColumn("Done");

//    m_layout->addWidget(new QWidget, 1, 0);
}

void StripsGrid::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent" << *event;

    if (event->button() == Qt::LeftButton &&
        !m_isDragging &&
        m_widget->startDragging(event->pos() - m_widget->pos()))
    {
        m_widget->setGraphicsEffect(new QGraphicsOpacityEffect);
        m_isDragging = true;
        m_dragOffset = event->pos() - m_widget->pos();
    }

    QWidget::mousePressEvent(event);
}

void StripsGrid::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent" << *event;

    if (event->button() == Qt::LeftButton &&
        m_isDragging)
    {
        m_widget->setGraphicsEffect(nullptr);
        m_isDragging = false;
    }

    QWidget::mouseReleaseEvent(event);
}

void StripsGrid::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoveEvent" << *event;

    if (m_isDragging)
        m_widget->move(event->pos() - m_dragOffset);

    QWidget::mouseMoveEvent(event);
}
