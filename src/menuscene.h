#ifndef MENUSCENE_H
#define MENUSCENE_H
#include <QGraphicsScene>

class QGraphicsPixmapItem;
class MenuScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MenuScene(QObject* parent = nullptr);
signals:
    void gameActivated(int level);
private:
    QGraphicsPixmapItem* getActiveButton();
    void addTextToButton(QGraphicsPixmapItem* button, QString text);
    QGraphicsPixmapItem *monominoButton, *dominoButton, *triominoButton,
    *tetrominoButton, *pentominoButton, *exitButton;
    QGraphicsPixmapItem *title;
    int currentIndex;

    // QGraphicsScene interface
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // MENUSCENE_H
