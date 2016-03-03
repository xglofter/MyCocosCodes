#ifndef Shape_h
#define Shape_h

#include <vector>
#include "Block.h"
#include "cocos2d.h"


class Shape : public cocos2d::Node
{
public:

    virtual bool init();

    enum Orientation
    {
        kUp,
        kRight,
        kDown,
        kLeft
    };
    Orientation getOrientation() const { return _orientation; }

    enum ShapeType
    {
        kTypeI,
        kTypeJ,
        kTypeL,
        kTypeO,
        kTypeS,
        kTypeZ,
        kTypeT
    };
    ShapeType getShapeType() const { return _type; }


    void setBlockColor(Block::BlockColor color) { _color = color; }
    Block::BlockColor getBlockColor() { return _color; }

    void setOrientation(Orientation orient) { _orientation = orient; }
    Orientation getOrientation() { return _orientation; }
    Orientation getNextOrientation();

    ShapeType getShapeType() { return _type; }
    static int getShapeNumber() { return 7; }

    void setRowInTetris(int row) { _nRowInTetris = row; }
    int getRowInTetris() { return _nRowInTetris; }
    void setColInTetris(int col) { _nColInTetris = col; }
    int getColInTetris() { return _nColInTetris; }

    /*
     * get blocks relative position
     */
    std::vector<cocos2d::Vec2> getBlockPos();
    std::vector<cocos2d::Vec2> getBlockPosByOrientation(Orientation orient);

    /*
     * get blocks
     */
    std::vector<Block*> getBlocks() { return _blocks; }

    void initBlocks();

    /*
     * rotate the Orientation: Up->Right->Down->Left->Up...
     */
    void rotate();

protected:
    /*
     * load the blocks array data
     */
    void loadBlocksPos(int array[32]);

    void updateBlockPos();

private:

    Orientation _orientation;
    ShapeType _type;
    Block::BlockColor _color;

    int _nRowInTetris;
    int _nColInTetris;
    bool _bCurrent;

    // to store Blocks
    std::vector<Block*> _blocks;

    // all orientation blocks' position
    // order: up->right->down->left
    std::vector<cocos2d::Vec2> _blockPos;
};

////////////////////////////////////////////////////////////////////////////////
//// all kinds of Shape

class IShape : public Shape
{
public:
    CREATE_FUNC(IShape);
    virtual bool init();
};

class JShape : public Shape
{
public:
    CREATE_FUNC(JShape);
    virtual bool init();
};

class LShape : public Shape
{
public:
    CREATE_FUNC(LShape);
    virtual bool init();
};

class OShape : public Shape
{
public:
    CREATE_FUNC(OShape);
    virtual bool init();
};

class SShape : public Shape
{
public:
    CREATE_FUNC(SShape);
    virtual bool init();
};

class ZShape : public Shape
{
public:
    CREATE_FUNC(ZShape);
    virtual bool init();
};

class TShape : public Shape
{
public:
    CREATE_FUNC(TShape);
    virtual bool init();
};


#endif /* Shape_h */
