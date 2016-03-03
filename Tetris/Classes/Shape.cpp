#include "Shape.h"

using namespace cocos2d;

bool Shape::init()
{
    // is next one, not current
    _bCurrent = false;
    _nColInTetris = -1;
    _nColInTetris = -1;

    for (int i = 0; i < 4; i++)
    {
        Block* block = Block::create();
        _blocks.push_back(block);
        addChild(block);
    }
    return true;
}

void Shape::loadBlocksPos(int array[32])
{
    for (int i = 0; i < 32; i += 2)
    {
        _blockPos.push_back(Vec2(array[i], array[i+1]));
    }
}

void Shape::initBlocks()
{
    auto posStartIdx = _orientation;
    for (int i = 0; i < 4; i++)
    {
        auto posIdx = 4*posStartIdx + i;
        _blocks[i]->setPosition(Vec2(40*_blockPos[posIdx].x, 40*_blockPos[posIdx].y));
        _blocks[i]->setBlockColor(_color);
    }
}

Shape::Orientation Shape::getNextOrientation()
{
    if (_orientation == Orientation::kUp)
        return Orientation::kRight;
    else if (_orientation == Orientation::kRight)
        return Orientation::kDown;
    else if (_orientation == Orientation::kDown)
        return Orientation::kLeft;
    else if (_orientation == Orientation::kLeft)
        return Orientation::kUp;
}

std::vector<Vec2> Shape::getBlockPos()
{
    return getBlockPosByOrientation(_orientation);
}

std::vector<cocos2d::Vec2> Shape::getBlockPosByOrientation(Orientation orient)
{
    std::vector<Vec2> temp;
    auto posIdx = orient * 4;
    for (int i = posIdx; i < posIdx+4; i++)
    {
        temp.push_back(_blockPos[i]);
    }
    return temp;
}

void Shape::rotate()
{
    if (_orientation == Orientation::kUp)
        _orientation = Orientation::kRight;
    else if (_orientation == Orientation::kRight)
        _orientation = Orientation::kDown;
    else if (_orientation == Orientation::kDown)
        _orientation = Orientation::kLeft;
    else if (_orientation == Orientation::kLeft)
        _orientation = Orientation::kUp;

    updateBlockPos();
}

void Shape::updateBlockPos()
{
    auto posStartIdx = _orientation;
    for (int i = 0; i < 4; i++)
    {
        auto posIdx = 4*posStartIdx + i;
        _blocks[i]->setPosition(Vec2(40*_blockPos[posIdx].x, 40*_blockPos[posIdx].y));
    }
}


////////////////////////////////////////////////////////////////////////////////
//// IShape

/*
 
 up/down:   /     right/left

 | 0, 1 |
 | 0, 0 |      | -1,0 | 0,0 | 1,0 | 2,0 |
 | 0,-1 |
 | 0,-2 |
 */

bool IShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,-2, 0,1, 0,0, 0,-1,
        -1,0, 0,0, 1,0, 2,0,
        0,-2, 0,1, 0,0, 0,-1,
        -1,0, 0,0, 1,0, 2,0
    };
    loadBlocksPos(array);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//// JShape

/*

     up
        | 0,1 |
        | 0,0 |
| -1,-1 | 0,-1|

 */

bool JShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,1, 0,0, 0,-1, -1,-1,
        -1,1, -1,0, 0,0, 1,0,
        1,1, 0,1, 0,0, 0,-1,
        -1,0, 0,0, 1,0, 1,-1
    };
    loadBlocksPos(array);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//// LShape

/*

      up
 | 0, 1  |
 | 0, 0  |
 | 0,-1  | 1,-1|

 */

bool LShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,1, 0,0, 0,-1, 1,-1,
        -1,0, 0,0, 1,0, -1,-1,
        -1,1, 0,1, 0,0, 0,-1,
        -1,0, 0,0, 1,0, 1,1
    };
    loadBlocksPos(array);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//// OShape

/*

 up
 | 0, 1 | 1, 1 |
 | 0, 0 | 1, 0  |

 */

bool OShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,1, 1,1, 0,0, 1,0,
        0,1, 1,1, 0,0, 1,0,
        0,1, 1,1, 0,0, 1,0,
        0,1, 1,1, 0,0, 1,0
    };
    loadBlocksPos(array);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//// SShape

/*

 up
 | 0, 1 |
 | 0, 0 | 1, 0 |
        | 1, -1|
 */

bool SShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,1, 0,0, 1,0, 1,-1,
        0,0, 1,0, -1,-1, 0,-1,
        0,1, 0,0, 1,0, 1,-1,
        0,0, 1,0, -1,-1, 0,-1
    };
    loadBlocksPos(array);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//// ZShape

/*

 up
        | 1, 1 |
 | 0, 0 | 1, 0 |
 | 0, -1|
 */

bool ZShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        1,1, 0,0, 1,0, 0,-1,
        -1,0, 0,0, 0,-1, 1,-1,
        1,1, 0,0, 1,0, 0,-1,
        -1,0, 0,0, 0,-1, 1,-1
    };
    loadBlocksPos(array);

    return true;
}


////////////////////////////////////////////////////////////////////////////////
//// TShape

/*
         up
 
        | 0, 1 |
 | -1,0 | 0, 0 | 1, 0 |

 */

bool TShape::init()
{
    if (!Shape::init()) return false;

    int array[32] =
    {
        0,1, -1,0, 0,0, 1,0,
        0,1, 0,0, 1,0, 0,-1,
        -1,0, 0,0, 1,0, 0,-1,
        0,1, -1,0, 0,0, 0,-1
    };
    loadBlocksPos(array);

    return true;
}




