#pragma once

#include <QWidget>

#include <vector>

class QGridLayout;
class QLabel;

class StripWidget;
class FlowLayout;

class StripsGrid : public QWidget
{
    Q_OBJECT

public:
    explicit StripsGrid(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void updatePoints();

    QGridLayout *m_layout;

    StripWidget *m_draggedWidget{};
    StripWidget *m_dragWidget{};
    QPoint m_dragOffset;

    std::array<QLabel*, 5> m_tableHeader;

    struct Story {
        StripWidget *widget;

        struct Column {
            FlowLayout *layout;

            std::vector<StripWidget*> widgets;
        };

        std::array<Column, 4> columns;
    };

    std::vector<Story> m_stories;
};
