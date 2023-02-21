#include "gamescene.h"
#include "utils.h"
#include "pixmapmanager.h"
#include "fontmanager.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>


GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    initGame(5);

    setBackgroundBrush(Qt::black);
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i] = new KeyStatus();
    }
    m_mouse = new MouseStatus();
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);

}

void GameScene::initGame(int level)
{
    mGame.initBoard(level);
    srand(time(0));
    nCurrentX = mGame.FIELD_WIDTH / 2;
    nCurrentY = 0;
    nCurrentRotation = 0;
    nCurrentPiece = rand() % mGame.COUNT_OF_PIECES;
    nNextPiece = rand() % mGame.COUNT_OF_PIECES;
    bMoveLeft = bMoveRight = bRotate = false;
    isGenerateNewPiece = true;
}

void GameScene::startGame()
{
    m_timer.start(int(1000.0f/FPS));
    m_elapsedTimer.start();
    initGame(5);
}

void GameScene::stopGame()
{
    m_timer.stop();
}

void GameScene::renderGameScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    while( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        handlePlayerInput();
        if(!bGameOver)
        {
            nSpeedCount++;
            bForceDown = (nSpeedCount == nSpeed);
            if (bForceDown)
            {
                // Update difficulty every 50 pieces
                nSpeedCount = 0;
                nPieceCount++;
                if (nPieceCount % 50 == 0)
                {
                    if (nSpeed >= 10)
                    {
                        nSpeed--;
                    }
                }


                // Test if piece can be moved down
                if (mGame.doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                {
                    nCurrentY++; // It can, so do it!
                }
                else
                {
                    // It can't! Lock the piece in place
                    for (int px = 0; px < mGame.COUNT_OF_BLOCKS; px++)
                    {
                        for (int py = 0; py < mGame.COUNT_OF_BLOCKS; py++)
                        {
                            if(mGame.getElementOfPiece(nCurrentPiece, mGame.rotate(px, py, nCurrentRotation)) != '.')
                            {
                                mGame.field()[(nCurrentY + py) * mGame.FIELD_WIDTH + (nCurrentX + px)] = nCurrentPiece+1;
                            }

                        }

                    }

                    // Check for lines
                    for (int py = 0; py < mGame.COUNT_OF_BLOCKS; py++)
                    {
                        if(nCurrentY + py < mGame.FIELD_HEIGHT - 1)
                        {
                            bool bLine = true;
                            for (int px = 1; px < mGame.FIELD_WIDTH - 1; px++)
                            {
                                bLine &= (mGame.field()[(nCurrentY + py) * mGame.FIELD_WIDTH + px]) != 0;
                            }


                            if (bLine)
                            {
                                // Remove Line, set to =
                                for (int px = 1; px < mGame.FIELD_WIDTH - 1; px++)
                                {
                                    mGame.field()[(nCurrentY + py) * mGame.FIELD_WIDTH + px] = Polyomino::ANIM_BLOCK;
                                }

                                vLines.push_back(nCurrentY + py);
                            }
                        }
                    }



                    nScore += 25;


                    if(!vLines.empty())	nScore += (1 << vLines.size()) * 100;

                    isGenerateNewPiece = true;



                }

            }
        }
        bMoveLeft = bMoveRight = bRotate = false;

        if (!vLines.empty())
        {
            for (auto &v : vLines)
                for (int px = 1; px < mGame.FIELD_WIDTH - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                        mGame.field()[py * mGame.FIELD_WIDTH + px] = mGame.field()[(py - 1) * mGame.FIELD_WIDTH + px];
                    mGame.field()[px] = 0;
                }

            vLines.clear();
        }


        if(isGenerateNewPiece)
        {
            isGenerateNewPiece = false;
            bMoveDown = false;
            //mTarget.setKeyRepeatEnabled(false);
            nCurrentX = mGame.FIELD_WIDTH / 2;
            nCurrentY = 0;
            nCurrentPiece = nNextPiece;
            nNextPiece = rand() % mGame.COUNT_OF_PIECES;

            bGameOver = !mGame.doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
        }


        clear();
        renderAll();
        resetStatus();
    }
}

void GameScene::renderAll()
{
    drawField();
    drawCurrentPiece();
    drawNextPiece();
    drawScore();
    if(bGameOver)
    {
        drawGameOverText();
    }
}

void GameScene::drawField()
{
    for (int x = 0; x < mGame.FIELD_WIDTH; x++)
    {
        for (int y = 0; y < mGame.FIELD_HEIGHT; y++)
        {
            QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem;
            sprite->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Blocks).copy(int(mGame.pField[y*mGame.FIELD_WIDTH + x])*Polyomino::GRID_SIZE, 0, Polyomino::GRID_SIZE, Polyomino::GRID_SIZE));
            sprite->setPos(x*Polyomino::GRID_SIZE + mGame.OFFSET_X/2, y*Polyomino::GRID_SIZE + mGame.OFFSET_Y);
            addItem(sprite);
        }
    }
}

void GameScene::drawCurrentPiece()
{
    for (int px = 0; px < mGame.COUNT_OF_BLOCKS; px++)
    {
        for (int py = 0; py < mGame.COUNT_OF_BLOCKS; py++)
        {
            if(mGame.getElementOfPiece(nCurrentPiece, mGame.rotate(px, py, nCurrentRotation))  != '.')
            {
                QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem;
                sprite->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Blocks).copy((nCurrentPiece+1)*Polyomino::GRID_SIZE, 0, Polyomino::GRID_SIZE, Polyomino::GRID_SIZE)) ;
                sprite->setPos((nCurrentX + px)*Polyomino::GRID_SIZE + mGame.OFFSET_X/2 , (nCurrentY + py)*Polyomino::GRID_SIZE + mGame.OFFSET_Y);
                addItem(sprite);
            }

        }
    }
}

void GameScene::drawNextPiece()
{
    QGraphicsSimpleTextItem* nextItem = new QGraphicsSimpleTextItem;
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::Main);
    font.setPointSize(20);
    font.setBold(true);
    nextItem->setFont(font);
    nextItem->setPos(SCREEN::PHYSICAL_SIZE.width() - (mGame.COUNT_OF_BLOCKS-1)*Polyomino::GRID_SIZE, mGame.OFFSET_Y+75);
    nextItem->setBrush(Qt::white);
    nextItem->setText("Next: ");
    addItem(nextItem);

    for (int px = 0; px < mGame.COUNT_OF_BLOCKS; px++)
    {
        for (int py = 0; py < mGame.COUNT_OF_BLOCKS; py++)
        {
            if( mGame.getElementOfPiece(nNextPiece, mGame.rotate(px, py, 0)) != '.' )
            {
                QGraphicsPixmapItem* sprite = new QGraphicsPixmapItem;
                sprite->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Blocks).copy((nNextPiece+1)*Polyomino::GRID_SIZE, 0, Polyomino::GRID_SIZE, Polyomino::GRID_SIZE));
                sprite->setPos(px*Polyomino::GRID_SIZE + SCREEN::PHYSICAL_SIZE.width() - (mGame.COUNT_OF_BLOCKS+1)*Polyomino::GRID_SIZE, (py)*Polyomino::GRID_SIZE + mGame.OFFSET_Y+100);
                addItem(sprite);
            }

        }
    }
}

void GameScene::drawScore()
{
    QGraphicsSimpleTextItem* scoreItem = new QGraphicsSimpleTextItem;
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::Main);
    font.setPointSize(20);
    font.setBold(true);
    scoreItem->setFont(font);
    scoreItem->setBrush(Qt::white);
    scoreItem->setText("Score: "+QString::number(nScore).leftJustified(5));
    scoreItem->setPos(SCREEN::PHYSICAL_SIZE.width() - scoreItem->boundingRect().width(), 0);
    addItem(scoreItem);
}

void GameScene::drawGameOverText()
{
    QGraphicsSimpleTextItem* goItem = new QGraphicsSimpleTextItem();
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::Main);
    font.setPointSize(50);
    font.setBold(true);
    goItem->setFont(font);
    goItem->setBrush(Qt::red);
    goItem->setText("     Game Over\n"
                    "r - restart\n"
                    "backspace - menu");
    goItem->setPos(SCREEN::PHYSICAL_SIZE.width()/2-goItem->boundingRect().width()/2,
                   SCREEN::PHYSICAL_SIZE.height()/2-goItem->boundingRect().height()/2);

    QGraphicsRectItem *rItem = new QGraphicsRectItem();
    rItem->setPos(goItem->pos());
    rItem->setRect(goItem->boundingRect());
    rItem->setBrush(Qt::black);
    rItem->setPen(QPen(Qt::white,2));

    addItem(rItem);
    addItem(goItem);
}

void GameScene::handlePlayerInput()
{
    if(m_keys[KEYBOARD::KEY_LEFT]->m_released)
    {
        bMoveLeft = true;
    }
    if(m_keys[KEYBOARD::KEY_RIGHT]->m_released)
    {
        bMoveRight = true;
    }
    if(m_keys[KEYBOARD::KEY_UP]->m_released)
    {
        bRotate = true;
    }
    if(m_keys[KEYBOARD::KEY_DOWN]->m_pressed)
    {
        bMoveDown = true;
    }
    if(m_keys[KEYBOARD::KEY_DOWN]->m_released || isGenerateNewPiece)
    {
        bMoveDown = false;
    }
    if(m_keys[KEYBOARD::KEY_R]->m_released)
    {
        if(bGameOver)
        {
            initGame(5);
        }
    }
    if(bMoveLeft)
    {
        if(mGame.doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
        {
            nCurrentX--;
        }
    }
    else if(bMoveRight)
    {
        if(mGame.doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
        {
            nCurrentX++;
        }
    }
    else if(bMoveDown)
    {
        if(mGame.doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
        {
            nCurrentY++;
        }
    }
    else if(bRotate)
    {
        if(mGame.doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY))
        {
            nCurrentRotation++;
        }
        bRotate = false;
    }
}

void GameScene::resetStatus()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_released = false;
    }
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_pressed = false;
    }
    m_mouse->m_released = false;
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
        }
        else
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(!event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
        }

    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = true;
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = false;
    m_mouse->m_released = true;
    QGraphicsScene::mouseReleaseEvent(event);
}
