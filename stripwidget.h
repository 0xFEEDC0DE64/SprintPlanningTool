#pragma once

#include <QFrame>

#include <memory>

namespace Ui { class StripWidget; }

class StripWidget : public QFrame
{
    Q_OBJECT

public:
    explicit StripWidget(QWidget *parent = nullptr);
    ~StripWidget() override;

    bool startDragging(const QPoint &pos) const;

    void setTitle(const QString &title);
    void setDescription(const QString &description);
    void setPoints(int points);
    void setOwner(const QString &owner);

private:
    std::unique_ptr<Ui::StripWidget> m_ui;
};
