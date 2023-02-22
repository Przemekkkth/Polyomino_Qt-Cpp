#include "menuscene.h"
#include "pixmapmanager.h"
#include "utils.h"
#include "fontmanager.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QApplication>

MenuScene::MenuScene(QObject *parent)
    : QGraphicsScene(parent), currentIndex(0)
{
    title           = new QGraphicsPixmapItem();
    title->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Title));
    title->setPos(30, 30);

    monominoButton  = new QGraphicsPixmapItem();
    monominoButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonSelected));
    monominoButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-monominoButton->boundingRect().width()/2, 200);
    addTextToButton(monominoButton, "Monomino");

    dominoButton    = new QGraphicsPixmapItem();
    dominoButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
    dominoButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-dominoButton->boundingRect().width()/2, 275);
    addTextToButton(dominoButton, "Domino");

    triominoButton  = new QGraphicsPixmapItem();
    triominoButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
    triominoButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-triominoButton->boundingRect().width()/2, 350);
    addTextToButton(triominoButton, "Triomiono");

    tetrominoButton = new QGraphicsPixmapItem();
    tetrominoButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
    tetrominoButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-tetrominoButton->boundingRect().width()/2, 425);
    addTextToButton(tetrominoButton, "Tetromino");

    pentominoButton = new QGraphicsPixmapItem();
    pentominoButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
    pentominoButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-pentominoButton->boundingRect().width()/2, 500);
    addTextToButton(pentominoButton, "Pentomino");

    exitButton      = new QGraphicsPixmapItem();
    exitButton->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
    exitButton->setPos(SCREEN::PHYSICAL_SIZE.width()/2-exitButton->boundingRect().width()/2, 575);
    addTextToButton(exitButton, "Exit");

    addItem(title);
    addItem(monominoButton);
    addItem(dominoButton);
    addItem(triominoButton);
    addItem(tetrominoButton);
    addItem(pentominoButton);
    addItem(exitButton);

    setBackgroundBrush(Qt::black);
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
}

QGraphicsPixmapItem *MenuScene::getActiveButton()
{
    if(currentIndex == 0)
    {
        return monominoButton;
    }
    else if(currentIndex == 1)
    {
        return dominoButton;
    }
    else if(currentIndex == 2)
    {
        return triominoButton;
    }
    else if(currentIndex == 3)
    {
        return tetrominoButton;
    }
    else if(currentIndex == 4)
    {
        return pentominoButton;
    }
    else
    {
        return exitButton;
    }
}

void MenuScene::addTextToButton(QGraphicsPixmapItem *button, QString text)
{
    QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(button);
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::Main);
    font.setPointSize(20);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setBrush(Qt::white);
    textItem->setText(text);
    textItem->setPos(button->boundingRect().width()/2-textItem->boundingRect().width()/2,
                     button->boundingRect().height()/2-textItem->boundingRect().height()/2);
}

void MenuScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        getActiveButton()->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
        currentIndex--;
        if(currentIndex < 0)
        {
            currentIndex = 5;
        }
        getActiveButton()->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonSelected));
    }
    else if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
    {
        getActiveButton()->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonUnselected));
        currentIndex++;
        if(currentIndex > 5)
        {
            currentIndex = 0;
        }
        getActiveButton()->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::ButtonSelected));
    }
    else if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Space)
    {
        if(currentIndex < 6)
        {
            emit gameActivated(currentIndex+1);
        }
        else
        {
            QApplication::instance()->quit();
        }
    }
    QGraphicsScene::keyReleaseEvent(event);
}
