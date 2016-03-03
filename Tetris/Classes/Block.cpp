#include "Block.h"

using namespace cocos2d;

bool Block::init()
{
    if (!Sprite::init()) return false;
    
    _color = BlockColor::kUnkown;
    _row = 0;
    _col = 0;

    return true;
}

std::string Block::getColorName() const
{
    switch (_color)
    {
        case kUnkown:
            return "";
        case kBlue:
            return "blue";
        case kOrange:
            return "orange";
        case kPurple:
            return "purple";
        case kRed:
            return "red";
        case kTeal:
            return "teal";
        case kYellow:
            return "yellow";
    }
}

void Block::setBlockColor(Block::BlockColor color)
{
    if (color == kUnkown || _color == color) return ;
    _color = color;
    auto colorName = getColorName();
    this->setTexture(colorName+".png");
}

bool Block::operator== (const Block &otherBlock)
{
    return (_row == otherBlock._row && _col == otherBlock._col);
}

