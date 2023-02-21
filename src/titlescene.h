#ifndef TITLESCENE_H
#define TITLESCENE_H
#include <QGraphicsScene>

class QGraphicsPixmapItem;
class QGraphicsSimpleTextItem;
class QTimer;

class TitleScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TitleScene(QObject* parent = 0);
signals:
    void menuActivated();

private slots:
    void updateText();
private:
    QGraphicsPixmapItem* qtLogo, *titleItem;
    QGraphicsSimpleTextItem* pressAnyKeyText;
    QTimer* timer;
    bool isShow;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // TITLESCENE_H
