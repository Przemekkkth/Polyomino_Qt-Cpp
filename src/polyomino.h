#ifndef POLYOMINO_H
#define POLYOMINO_H
#include <QString>

class Polyomino
{
public:
    static QString pentomino[16];
    static QString tetromino[7];
    static QString triomino[3];
    static QString domino[1];
    static QString monomino[1];
    constexpr static const int ANIM_BLOCK = 17;
    constexpr static const int BOUNDARY_BLOCK = 18;
    constexpr static const int GRID_SIZE = 32;

    Polyomino();
    void initBoard(int level);
    void setPentomino();
    void setTetromino();
    void setTriomino();
    void setDomino();
    void setMonomino();
    QString getElementOfPiece(int x, int y);
    int rotate(int px, int py, int r);
    bool doesPieceFit(int nBlock, int nRotation, int nPosX, int nPosY);

    unsigned char* field() const;

    int OFFSET_X;
    int OFFSET_Y;
    int COUNT_OF_PIECES;
    int FIELD_WIDTH;
    int FIELD_HEIGHT;
    int COUNT_OF_BLOCKS;
    unsigned char *pField;
};

#endif // POLYOMINO_H
