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

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    int points() const;
    void setPoints(int points);

    QString owner() const;
    void setOwner(const QString &owner);

private:
    std::unique_ptr<Ui::StripWidget> m_ui;
};
