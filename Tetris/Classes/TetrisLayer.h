#ifndef _TERISLAYER_H_
#define _TERISLAYER_H_

#include <map>
#include "cocos2d.h"
#include "Block.h"
#include "Shape.h"

typedef std::function<void(std::string)> scoreCallFunc;
typedef std::function<void(void)> gameOverCallFunc;

class TetrisLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(TetrisLayer);

    void createNextShape();

    void putNextToCurrent();

    void dropDown(float dt);

    /*
     * check whether the current shape is cover with the blocks
     * @return bool: covered is true
     */
    bool checkCover();
    bool checkCoverByPos(int row, int col);

    /*
     * remove current shape, add shape's block to the `_blocks`
     */
    void removeCurShape();

    /*
     * check the `_blocks` and remove the full rows
     */
    void checkAndRemoveRows();

    /*
     * move and rotate shape.
     * @return bool: moving whether suceess
     */
    bool rotateShape();
    bool moveShapeLeft();
    bool moveShapeRight();
    void moveShapeDown();

    void setScoreCallback(scoreCallFunc func) { _callScoreFunc = func; }
    void setGameOverCallback(gameOverCallFunc func) { _callGameOverFunc = func; }

    /*
     * the left-down Block is row=0,col=0; the right-up Block is row=12,col=20
     */
    cocos2d::Vec2 getVec2Pos(int row, int col);

    /*
     * get ID on _blocks position
     * key(ID) = row * _nColNums + col
     */
    int getRowColumnID(int row, int col) { return row * _nColNums + col; }
    cocos2d::Vec2 getPositionByRowCol(int row, int col);

    /*
     * set current shape to (row,col) position
     */
    void setCurShapePosition(int row, int col);

    /*
     * looking for the below position for current shape
     */
    cocos2d::Vec2 getBelowRowCol();

    /*
     * get max row in blocks, can be -1 (bottom) 
     */
    int getMaxRowInBlocks(int row, int col);

    /*
     * whether Tetris has this ID block
     */
    bool hasBlockById(int id);

private:
    void getNextShape(float dt);

    void initBg();
    void initBlock();

    void addScoreByCount(int count);

    int _nRowNums;
    int _nColNums;
    int _fBlockWid;

    Shape *_curShape;
    Shape *_nextShape;

    float _curDownSpeed;

    int _score;
    scoreCallFunc _callScoreFunc;
    gameOverCallFunc _callGameOverFunc;
};


#endif /* _TERISLAYER_H_ */
