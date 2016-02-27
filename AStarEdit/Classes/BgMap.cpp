//
//  BgMap.cpp
//  cppgame
//
//  Created by Richard on 16/2/24.
//
//

#include <cmath>
#include "BgMap.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/stringbuffer.h"

using namespace cocos2d;

static int DirArray[8] =
{
    0,-1, 1,0, 0,1, -1, 0
};
static int DirNum = 8;

static const float MAX_VALUE = 1000000.0;
const std::string NAME_START = "START";
const std::string NAME_END = "END";
const std::string NAME_WALL = "WALL";
const std::string NAME_RELATE = "RELATE";
const std::string NAME_PATH = "PATH";

typedef std::unordered_map<std::string, PathCell*> PathMap;
typedef std::unordered_map<std::string, PathCell*>::iterator PathMapIter;
typedef std::pair<std::string, PathCell*> PathMapPair;

BgMap::BgMap() :
    _nSizeX(0),
    _nSizeY(0),
    _fCellSize(0),
    _hasStart(false),
    _hasEnd(false),
    _isClear(true)
{
    _cellWalls.clear();
}

BgMap::~BgMap()
{

}

BgMap* BgMap::createWithFile(const std::string &filename)
{
    auto node = new BgMap();
    if (node && node->initWithFile(filename) && node->init())
    {
        node->autorelease();
        return node;
    }
    else
    {
        delete node;
        node = nullptr;
        return nullptr;
    }
}

bool BgMap::init()
{
    if (!Node::init()) return false;

    // bg
    auto drawNode = DrawNode::create();
    drawNode->setLineWidth(1);
    addChild(drawNode);

    auto colorLine = Color4F(1.0, 1.0, 1.0, 0.5);

    for (int i = 0; i < _nSizeY+1; ++i)
    {
        drawNode->drawLine(Vec2(_fCellSize*i, 0), Vec2(_fCellSize*i, _fCellSize*_nSizeY), colorLine);
    }
    for (int i = 0; i < _nSizeX+1; ++i)
    {
        drawNode->drawLine(Vec2(0, _fCellSize*i), Vec2(_fCellSize*_nSizeX, _fCellSize*i), colorLine);
    }

    // wall
    for (std::unordered_map<std::string, CellPos>::iterator iter = _cellWalls.begin(); iter != _cellWalls.end(); ++iter)
    {
        std::string sId = iter->first;
        auto cellPos = this->getCellPosById(sId);
        this->drawWall(cellPos.x, cellPos.y);
    }
    CHECK_GL_ERROR_DEBUG();

    return true;
}

bool BgMap::initData(const std::string &jsonString)
{
    if (jsonString.empty()) return false;

    rapidjson::Document doc;
    if (doc.Parse<0>(jsonString.c_str()).HasParseError())
        return false;

    if (!doc.IsObject())
        return false;

    if (!doc.HasMember("map_size_x") || !doc.HasMember("map_size_y") || 
        !doc.HasMember("cell_size") || !doc.HasMember("cell_data"))
    {
        log("json file is not complete");
        return false;
    }

    _nSizeX = doc["map_size_x"].GetInt();
    _nSizeY = doc["map_size_y"].GetInt();
    _fCellSize = doc["cell_size"].GetInt();

    const rapidjson::Value &dataArray = doc["cell_data"];
    if (!dataArray.IsArray())
    {
        log("[json read] cell_data member is not array");
        return false;
    }
    for (rapidjson::SizeType i = 0; i < dataArray.Size(); i++)
    {
        const rapidjson::Value &temp = dataArray[i];
        if (temp.HasMember("x") && temp.HasMember("y") && temp.HasMember("type"))
        {
            CellPos cellpos;
            cellpos.x = temp["x"].GetInt();
            cellpos.y = temp["y"].GetInt();
            if (cellpos.x < 0 || cellpos.x >= _nSizeX || cellpos.y < 0 || cellpos.y >= _nSizeY)
            {
                log("[json read] cell_data pos x or y error!");
                return false;
            }

            std::string sId = this->getCellId(cellpos);
            std::string sType(temp["type"].GetString());
            if (sType == "wall")
            {
                _cellWalls.insert(std::pair<std::string, CellPos>(sId, cellpos));
            }
            else if (sType == "start")
            {
                _startCellPos = cellpos;
                this->drawStart(cellpos.x, cellpos.y);
                _hasStart = true;
            }
            else if (sType == "end")
            {
                _endCellPos = cellpos;
                this->drawEnd(cellpos.x, cellpos.y);
                _hasEnd = true;
            }
        }
    }

    return true;
}

bool BgMap::initWithFile(const std::string &jsonFileName)
{
    auto jsonContent = FileUtils::getInstance()->getStringFromFile(jsonFileName);
    log("[initWithFile] filename : %s",jsonFileName.c_str());
    if (this->initData(jsonContent))
        return true;
    return false;
}

bool BgMap::addStartPos(const Vec2& pos)
{
    CellPos cellPos;
    if (this->getCellPos(pos, cellPos))
    {
        if (_cellWalls.find(getCellId(cellPos)) != _cellWalls.end())
            return false;
        if (_hasEnd && _endCellPos.x == cellPos.x && _endCellPos.y == cellPos.y)
            return false;
        if (_hasStart)
            this->removeChildByName(NAME_START);
        this->drawStart(cellPos.x, cellPos.y);
        _startCellPos.x = cellPos.x;
        _startCellPos.y = cellPos.y;
        _hasStart = true;
        return true;
    }
    return false;
}

bool BgMap::addEndPos(const Vec2& pos)
{
    CellPos cellPos;
    if (this->getCellPos(pos, cellPos))
    {
        if (_cellWalls.find(getCellId(cellPos)) != _cellWalls.end())
            return false;
        if (_hasStart && _startCellPos.x == cellPos.x && _startCellPos.y == cellPos.y)
            return false;
        if (_hasEnd)
            this->removeChildByName(NAME_END);
        this->drawEnd(cellPos.x, cellPos.y);
        _endCellPos.x = cellPos.x;
        _endCellPos.y = cellPos.y;
        _hasEnd = true;
        return true;
    }
    return false;
}

bool BgMap::addWallPos(const Vec2& pos)
{
    CellPos cellPos;
    if (this->getCellPos(pos, cellPos))
    {
        if (_hasStart && _startCellPos.x == cellPos.x && _startCellPos.y == cellPos.y)
            return false;
        if (_hasEnd && _endCellPos.x == cellPos.x && _endCellPos.y == cellPos.y)
            return false;
        auto id = this->getCellId(cellPos);
        if (_cellWalls.find(id) == _cellWalls.end())
        {
            this->drawWall(cellPos.x, cellPos.y);
            _cellWalls[id] = cellPos;
        }
    }
    return false;
}

bool BgMap::removePos(const cocos2d::Vec2& pos)
{
    CellPos cellPos;
    if (this->getCellPos(pos, cellPos))
    {
        if (_hasStart && _startCellPos.x == cellPos.x && _startCellPos.y == cellPos.y)
        {
            this->removeChildByName(NAME_START);
            _hasStart = false;
            return true;
        }
        else if (_hasEnd && _endCellPos.x == cellPos.x && _endCellPos.y == cellPos.y)
        {
            this->removeChildByName(NAME_END);
            _hasEnd = false;
            return true;
        }
        else
        {
            auto id = this->getCellId(cellPos);
            auto iterWall = _cellWalls.find(id);
            if (iterWall != _cellWalls.end())
            {
                this->removeChildByName(NAME_WALL+id);
                _cellWalls.erase(iterWall);
                return true;
            }
        }
    }
    return false;
}

void BgMap::getPath()
{
    if (!_isClear) return ;
    if (!_hasStart || !_hasEnd) return ;

    auto startCellId = this->getCellId(_startCellPos);
    auto endCellId = this->getCellId(_endCellPos);

    log("[get path] start : %s end : %s", startCellId.c_str(), endCellId.c_str());

    if (startCellId == "" || endCellId == "")
    {
        log("!!! getPath paramter start or end pos error !!!");
        return ;
    }   

    // use map, CellId as key of map
    PathMap openSheet;
    PathMap closeSheet;
    PathCell* startCell = new PathCell();
    startCell->pos = _startCellPos;
    closeSheet.insert(PathMapPair(startCellId, startCell));

    // to find the short way
    bool isFindWay = false;
    float minFn = MAX_VALUE;
    std::string minId;
    PathCell *curPathCell = startCell;
    while (true)
    {
        // get new Cell, add to open sheet
        for (int i = 0; i < DirNum; i = i+2)  // all direction cell
        {
            int newNeighborX = curPathCell->pos.x + DirArray[i];
            int newNeighborY = curPathCell->pos.y + DirArray[i+1];
            std::string newCellId = getCellId(newNeighborX, newNeighborY);

            if (newCellId == "") // not in map
                continue;
            if (_cellWalls.find(newCellId) != _cellWalls.end()) // is wall
                continue;
            if (closeSheet.find(newCellId) != closeSheet.end()) // in close sheet
                continue;

            if (openSheet.find(newCellId) == openSheet.end())   // not in open sheet
            {
                PathCell* newPathCell = new PathCell();
                newPathCell->pos = CellPos(newNeighborX, newNeighborY);
                newPathCell->parent = curPathCell;
                newPathCell->gN = curPathCell->gN + 10;
                newPathCell->hN = 10*abs(_endCellPos.x - newNeighborX) + 10*abs(_endCellPos.y - newNeighborY);
                openSheet.insert(PathMapPair(newCellId, newPathCell));
                if (newCellId == endCellId)  // find end Cell
                {
                    isFindWay = true;
                    break;
                }
            }
            else  // already in open sheet. if find better path, change path
            {
                auto curGn = openSheet.at(newCellId)->gN;
                auto tempGn = curPathCell->gN + 10;
                if (tempGn < curGn)  // from currPathCell go here is better
                {
                    openSheet.at(newCellId)->gN = tempGn;
                    openSheet.at(newCellId)->parent = curPathCell;
                }
            }
        }
        if (isFindWay) break;

        // get min Fn PathCell in open sheet, move min Cell in close sheet
        minFn = MAX_VALUE;
        for (auto iter = openSheet.begin(); iter != openSheet.end(); ++iter)
        {
            auto newCellId = iter->first;
            auto newFn = iter->second->gN + iter->second->hN;
            if (newFn < minFn)
            {
                minFn = newFn;
                minId = iter->first;
                curPathCell = iter->second;
            }
        }
        if (minFn != MAX_VALUE)
        {
            openSheet.erase(minId);
            closeSheet.insert(PathMapPair(minId, curPathCell));
        }
        else
        {
            // open sheet is empty, can't find a short way
            if (openSheet.empty())
            {
                isFindWay = false;
                break;
            }
        }
    } // end while

    // draw close sheet
    for (auto iter = closeSheet.begin(); iter != closeSheet.end(); ++iter)
    {
        auto id = iter->first;
        auto cellPos = this->getCellPosById(id);
        drawRelateCell(cellPos.x, cellPos.y);  // draw all close sheet
    }

    if (!isFindWay)
    {
        log("[get path] can't find a right way!");
        return ;
    }

    // get the path
    std::stack<CellPos> pathStack;
    pathStack.push(_endCellPos);
    PathCell *temp = openSheet.at(endCellId);
    while (temp->parent != nullptr)
    {
        pathStack.push(temp->pos);
        temp = temp->parent;
    }
    // draw path
    while (!pathStack.empty())
    {
        auto cellPos = pathStack.top();
        pathStack.pop();
        this->drawPathCell(cellPos.x, cellPos.y);
    }
    
    _isClear = false;

    // clear new
    for (PathMapIter iter = openSheet.begin(); iter != openSheet.end(); ++iter)
        delete (iter->second);
    for (PathMapIter iter = closeSheet.begin(); iter != closeSheet.end(); ++iter)
        delete (iter->second);
}

bool BgMap::saveCurrentMap()
{
    rapidjson::Document writeDoc;
    writeDoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = writeDoc.GetAllocator();

    writeDoc.AddMember("map_size_x", 19, allocator);
    writeDoc.AddMember("map_size_y", 19, allocator);
    writeDoc.AddMember("cell_size", 16, allocator);

    rapidjson::Value arrayData(rapidjson::kArrayType);
    for (std::unordered_map<std::string, CellPos>::iterator iter = _cellWalls.begin();
         iter != _cellWalls.end(); ++iter)
    {
        CellPos cellPos = iter->second;
        rapidjson::Value tempObj(rapidjson::kObjectType);
        tempObj.AddMember("x", cellPos.x, allocator);
        tempObj.AddMember("y", cellPos.y, allocator);
        tempObj.AddMember("type", "wall", allocator);
        arrayData.PushBack(tempObj, allocator);
    }

    if (_hasStart)
    {
        rapidjson::Value startObj(rapidjson::kObjectType);
        startObj.AddMember("x", _startCellPos.x, allocator);
        startObj.AddMember("y", _startCellPos.y, allocator);
        startObj.AddMember("type", "start", allocator);
        arrayData.PushBack(startObj, allocator);
    }

    if (_hasEnd)
    {
        rapidjson::Value endObj(rapidjson::kObjectType);
        endObj.AddMember("x", _endCellPos.x, allocator);
        endObj.AddMember("y", _endCellPos.y, allocator);
        endObj.AddMember("type", "end", allocator);
        arrayData.PushBack(endObj, allocator);
    }
    writeDoc.AddMember("cell_data", arrayData, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writeDoc.Accept(writer);

    auto targetFile = FileUtils::getInstance()->getWritablePath() + "target.json";
    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), targetFile);

    log("[save json] over");
    return true;
}

void BgMap::clearPath()
{
    if (_isClear) return ;

    auto &children = this->getChildren();
    for (Vector<Node*>::iterator iter = children.begin(); iter != children.end();)
    {
        if ((*iter)->getName() == NAME_RELATE || (*iter)->getName() == NAME_PATH)
            (*iter)->removeFromParent();
        else
            ++iter;
    }
    _isClear = true;
}

Vec2 BgMap::getCellVecPos(int x, int y)
{
    return Vec2(_fCellSize * (x + 0.5), _fCellSize * (_nSizeY - y - 0.5));
}

bool BgMap::getCellPos(const Vec2 &pos, CellPos &cellPos)
{
    if (pos.x < 0 || pos.x > _fCellSize * _nSizeX ||
        pos.y < 0 || pos.y > _fCellSize * _nSizeY)
    {
        // not a right pos
        return false;
    }

    // get this pos
    cellPos.x = pos.x/_fCellSize;
    cellPos.y = (_fCellSize*_nSizeY - pos.y)/_fCellSize;
    return true;
}

CellPos BgMap::getCellPosById(std::string &id)
{
    auto splitTagIdx = id.find(":");
    auto x = atoi(id.substr(0, splitTagIdx).c_str());
    auto y = atoi(id.substr(splitTagIdx+1, id.length()).c_str());
    return CellPos(x, y);
}

std::string BgMap::getCellId(const CellPos &pos)
{
    return this->getCellId(pos.x, pos.y);
}

std::string BgMap::getCellId(int x, int y)
{
    if (x < 0 || x > _nSizeX-1 || y < 0 || y > _nSizeY-1)
        return "";
    return StringUtils::format("%d:%d", x, y);
}

void BgMap::drawSolidRect(int x, int y, Color4F color, std::string tagName)
{
    auto fStartX = x * _fCellSize;
    auto fStartY = (_nSizeY - y - 1) * _fCellSize;

    auto drawNode = DrawNode::create();
    drawNode->drawSolidRect(Vec2(fStartX, fStartY),
                             Vec2(fStartX + _fCellSize, fStartY + _fCellSize),
                             color);
    this->addChild(drawNode, 10, tagName);
    CHECK_GL_ERROR_DEBUG();
}

void BgMap::drawWall(int x, int y)
{
    this->drawSolidRect(x, y, Color4F(1,1,1,1), NAME_WALL+this->getCellId(x, y));
}

void BgMap::drawStart(int x, int y)
{
    this->drawSolidRect(x, y, Color4F(0,1,0,1), NAME_START);
}

void BgMap::drawEnd(int x, int y)
{
    this->drawSolidRect(x, y, Color4F(1,0,0,1), NAME_END);
}

void BgMap::drawRelateCell(int x, int y)
{
    this->drawSolidRect(x, y, Color4F(1,1,0,0.5), NAME_RELATE);
}

void BgMap::drawPathCell(int x, int y)
{
    this->drawSolidRect(x, y, Color4F(0,1,1,0.5), NAME_PATH);
}


