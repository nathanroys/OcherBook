#include <QtGui>

#include "ocher/ux/Controller.h"
#include "ocher/ux/BootActivityFb.h"
#include "ocher/ux/HomeActivityFb.h"
#include "ocher/ux/LibraryActivityFb.h"
#include "ocher/ux/NavBar.h"
#include "ocher/ux/ReadActivityFb.h"
#include "ocher/ux/SettingsActivityFb.h"
#include "ocher/ux/SleepActivityFb.h"
#include "ocher/ux/SyncActivityFb.h"
#include "ocher/ux/SystemBar.h"
#include "ocher/ux/MainWindow.h"

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;

    setCentralWidget(widget);

    m_view = new QStackedWidget;
    m_view->addWidget(new BootActivity());
    m_view->addWidget(new HomeActivity());
    m_view->addWidget(new LibraryActivity());
    m_view->addWidget(new ReadActivity());
    m_view->addWidget(new SettingsActivity());
    m_view->addWidget(new SleepActivity());
    m_view->addWidget(new SyncActivity());

    QWidget *topFiller = m_view;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    widget->setLayout(layout);

    createActions();
    createMenus();

    setWindowTitle(tr("OcherBook"));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(newOrgAct);
    menu.exec(event->globalPos());
}

void MainWindow::newFile()
{
}

void MainWindow::open()
{
}

void MainWindow::save()
{
}

void MainWindow::newOrg()
{
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About OcherBook"),
            tr("blah blah blah"));
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    newOrgAct = new QAction(tr("New organism"), this);
    newOrgAct->setStatusTip(tr("Place a new organism here"));
    connect(newOrgAct, SIGNAL(triggered()), this, SLOT(newOrg()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addSeparator();
    editMenu->addAction(newOrgAct);
    editMenu->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));
//	viewMenu->addAction(

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::setNextActivity(enum ActivityType a)
{
    if (a == ACTIVITY_QUIT) {
        // TODO
    } else {

        switch (a) {
        case ACTIVITY_BOOT:
            m_view->setCurrentIndex(0);
            break;
        case ACTIVITY_SLEEP:
            m_view->setCurrentIndex(5);
            break;
        case ACTIVITY_SYNC:
            m_view->setCurrentIndex(6);
            break;
        case ACTIVITY_HOME:
            m_view->setCurrentIndex(1);
            break;
        case ACTIVITY_READ:
            m_view->setCurrentIndex(3);
            break;
        case ACTIVITY_LIBRARY:
            m_view->setCurrentIndex(2);
            break;
        case ACTIVITY_SETTINGS:
            m_view->setCurrentIndex(4);
            break;
        }
    }
}
