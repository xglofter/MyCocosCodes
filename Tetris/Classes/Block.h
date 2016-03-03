#ifndef Block_h
#define Block_h

#include <string>
#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:
    enum BlockColor
    {
        kUnkown,
        kBlue,
        kOrange,
        kPurple,
        kRed,
        kTeal,
        kYellow
    };

    CREATE_FUNC(Block);
    virtual bool init();

    void setBlockColor(BlockColor color);
    BlockColor getBlockColor() const { return _color; }

    std::string getColorName() const;
    static int getColorNumber() { return 6; }

    void setRow(int row) { _row = row; }
    int getRow() const { return _row; }
    void setCol(int col) { _col = col; }
    int getCol() const { return _col; }

    bool operator== (const Block &otherBlock);

private:
    int _row;
    int _col;
    BlockColor _color;
};

#endif /* Block_h */
