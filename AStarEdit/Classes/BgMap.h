//
//  BgMap.hpp
//  cppgame
//
//  Created by Richard on 16/2/24.
//
//

#ifndef BgMap_h
#define BgMap_h

#include <map>
#include <string>
#include "cocos2d.h"

enum CellType
{
    kCell_Unkown,
    kCell_Empty,
    kCell_Wall,
};

struct CellPos
{
    CellPos() {}
    CellPos(int x, int y) : x(x), y(y) {}
    int x;
    int y;
};

struct PathCell
{
    PathCell() : gN(0), hN(0), parent(nullptr) {}
    CellPos pos;
    float gN;
    float hN;
    PathCell *parent;
};


class BgMap : public cocos2d::Node
{
public:
    BgMap();
    ~BgMap();

    static BgMap* createWithFile(const std::string &filename);
    virtual bool init();

    /*
     * init Background Map data by json format String
     */
    bool initData(const std::string &jsonString);

    /*
     * init Background Map data by json file.
     */
    bool initWithFile(const std::string &jsonFileName);

    /*
     * add a Cell on pos
     */
    bool addStartPos(const cocos2d::Vec2& pos);
    bool addEndPos(const cocos2d::Vec2& pos);
    bool addWallPos(const cocos2d::Vec2& pos);
    /*
     * remove a Cell
     */
    bool removePos(const cocos2d::Vec2& pos);
    void removeAll();

    /*
     * get short path with start position and end position.
     */
    void getPath();
    void clearPath();

    /*
     * save current map
     */
    bool saveCurrentMap();

    /*
     * get Cell on map position with CellPos's x and y
     * @Note: Cell's Anchor is on center
     */
    cocos2d::Vec2 getCellVecPos(int x, int y);

    /*
     * get map CellPos with position on map
     * @param pos: the Vec2 position on map
     * @param cellPos[out]: get the cellpos
     * @return true: input pos can get a right CellPos, if not return false.
     */
    bool getCellPos(const cocos2d::Vec2 &pos, CellPos &cellPos);
    
    /*
     * get map CellPos by Cell id
     */
    CellPos getCellPosById(std::string &id);

    /*
     * get map Cell id by CellPos
     * @return "" or "x:y"
     */
    std::string getCellId(const CellPos &pos);
    std::string getCellId(int x, int y);

private:
    void drawSolidRect(int x, int y, cocos2d::Color4F color, std::string tagName);
    void drawWall(int x, int y);
    void drawStart(int x, int y);
    void drawEnd(int x, int y);
    void drawRelateCell(int x, int y);
    void drawPathCell(int x, int y);

    int _nSizeX;
    int _nSizeY;
    float _fCellSize;
    bool _hasStart;
    bool _hasEnd;
    CellPos _startCellPos;
    CellPos _endCellPos;
    std::unordered_map<std::string, CellPos> _cellWalls;

    bool _isClear;
};

#endif /* BgMap_h */
