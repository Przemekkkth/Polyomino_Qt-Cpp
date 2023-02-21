#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "gamescene.h"
#include "titlescene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View();

signals:

private:
    GameScene* m_gameScene;
    TitleScene* m_titleScene;
    // QWidget interface
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
