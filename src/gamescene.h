#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QTimer>
#include <list>
#include <memory>
#include <QImage>
#include "polyomino.h"

struct KeyStatus
{
    bool m_pressed = false;
    bool m_held = false;
    bool m_released = false;
};

struct MouseStatus
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    bool m_released = false;
    bool m_pressed = false;
};


class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:

private slots:
    void loop();

private:
    void renderAll();
    void drawField();
    void drawCurrentPiece();
    void drawNextPiece();
    void drawScore();
    void drawGameOverText();

    Polyomino mGame;
    bool bMoveLeft, bMoveRight, bMoveDown, bRotate;
    int nCurrentPiece = 0;
    int nNextPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = mGame.FIELD_WIDTH / 2;
    int nCurrentY = 0;
    int nSpeed = 20;
    int nSpeedCount = 0;
    bool bForceDown = false;
    bool bRotateHold = true;
    int nPieceCount = 0;
    int nScore = 0;
    std::vector<int> vLines;
    bool bGameOver = false;
    bool isGenerateNewPiece = false;



    void renderGameScene();
    void handlePlayerInput();
    void resetStatus();
    KeyStatus* m_keys[256];
    MouseStatus* m_mouse;

    const int FPS = 60;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f, m_loopTime = 0.0f;
    const float m_loopSpeed = int(1000.0f/FPS);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
