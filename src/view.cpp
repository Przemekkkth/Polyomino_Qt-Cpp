#include "view.h"
#include <QKeyEvent>
#include <QApplication>

View::View()
    : m_gameScene(new GameScene(this)),
    m_titleScene(new TitleScene(this)),
    m_menuScene(new MenuScene(this))
{
    setScene(m_titleScene);
    resize(m_gameScene->sceneRect().width()+2, m_gameScene->sceneRect().height()+2);

    setMouseTracking(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    connect(m_titleScene, &TitleScene::menuActivated, [this](){
        setScene(m_menuScene);
    });
    connect(m_menuScene, &MenuScene::gameActivated, [this](int level){
        m_gameScene->startGame(level);
        setScene(m_gameScene);
    });
    connect(m_gameScene, &GameScene::menuActivated, [this](){
        setScene(m_menuScene);
    });
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        QApplication::instance()->quit();
    }
    QGraphicsView::keyReleaseEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}
