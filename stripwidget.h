#pragma once

#include <QFrame>

#include <memory>

namespace Ui { class StripWidget; }

class StripWidget : public QFrame
{
    Q_OBJECT

    explicit StripWidget(QWidget *parent = nullptr);

public:
    struct Story_t{};
    static constexpr const Story_t Story{};

    struct Subtask_t{};
    static constexpr const Subtask_t Subtask{};

    struct DraggingSubtask_t{};
    static constexpr const DraggingSubtask_t DraggingSubtask{};

    explicit StripWidget(Story_t, QWidget *parent = nullptr);
    explicit StripWidget(Subtask_t, QWidget *parent = nullptr);
    explicit StripWidget(DraggingSubtask_t, const StripWidget &draggedSubtask, QWidget *parent = nullptr);
    ~StripWidget() override;

    bool isInDragArea(const QPoint &pos) const;

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    int points() const;
    void setPoints(int points);

    QString owner() const;
    void setOwner(const QString &owner);

    void beginDrag();
    void endDrag();

signals:
    void pointsChanged(int points);

private:
    std::unique_ptr<Ui::StripWidget> m_ui;
};
