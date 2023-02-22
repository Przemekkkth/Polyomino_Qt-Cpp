#include "view.h"
#include <QKeyEvent>
#include <QApplication>
#include <QDir>
#include <QPainter>

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
    if(event->key() == Qt::Key_Z)
    {
        //Uncomment below if you want to make screenshots
//        static int index = 0;
//        QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
//        QRect rect = sceneRect().toAlignedRect();
//        QImage image(rect.size(), QImage::Format_ARGB32);
//        image.fill(Qt::transparent);
//        QPainter painter(&image);
//        render(&painter);
//        image.save(fileName);
//        qDebug() << "saved " << fileName;
    }
    QGraphicsView::keyReleaseEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}
