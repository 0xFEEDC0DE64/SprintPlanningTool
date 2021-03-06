#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    m_ui{std::make_unique<Ui::MainWindow>()}
{
    m_ui->setupUi(this);

    connect(m_ui->stripsGrid, &StripsGrid::pointsPerStoryChanged, m_ui->chartPerStory, &ChartPerStory::setPoints);
    connect(m_ui->stripsGrid, &StripsGrid::pointsPerUserChanged, m_ui->chartPerUser, &ChartPerUser::setPoints);

    m_ui->chartPerStory->setPoints(m_ui->stripsGrid->pointsPerStory());
    m_ui->chartPerUser->setPoints(m_ui->stripsGrid->pointsPerUser());

    for (QDockWidget *dockWidget : findChildren<QDockWidget*>())
    {
        m_ui->menu_View->addAction(dockWidget->toggleViewAction());
        dockWidget->close();
    }
}

MainWindow::~MainWindow() = default;
