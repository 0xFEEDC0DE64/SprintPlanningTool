#pragma once

#include <QWidget>

class QGridLayout;

class StripWidget;

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
    QGridLayout *m_layout;

    bool m_isDragging{false};
    QPoint m_dragOffset;
    StripWidget *m_widget;
};
