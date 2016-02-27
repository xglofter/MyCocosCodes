#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    _editState = EditState::kAdd_Start;
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // left menus
    auto sLabel = Label::create();
    sLabel->setString("ADD START");
    auto eLabel = Label::create();
    eLabel->setString("ADD END");
    auto wLabel = Label::create();
    wLabel->setString("ADD WALL");
    auto rLabel = Label::create();
    rLabel->setString("REMOVE CELL");
    auto raLabel = Label::create();
    raLabel->setString("REMOVE ALL");

    auto sMenuItem = MenuItemLabel::create(sLabel, std::bind(&HelloWorld::onSetStartPos, this));
    sMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto eMenuItem = MenuItemLabel::create(eLabel, std::bind(&HelloWorld::onSetEndPos, this));
    eMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto wMenuItem = MenuItemLabel::create(wLabel, std::bind(&HelloWorld::onSetWallPos, this));
    wMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto rMenuItem = MenuItemLabel::create(rLabel, std::bind(&HelloWorld::onRemovePos, this));
    rMenuItem->setAnchorPoint(Vec2(0, 0.5));

    auto menu = Menu::create(sMenuItem, eMenuItem, wMenuItem, rMenuItem, nullptr);
    menu->alignItemsVerticallyWithPadding(10);
    menu->setAnchorPoint(Vec2(0, 0));
    menu->setPosition(Vec2(origin.x + 10, origin.y + 230));
    this->addChild(menu);

    // right menu
    auto clearLabel = Label::create();
    clearLabel->setString("CLEAR PATH");
    auto getLabel = Label::create();
    getLabel->setString("GET PATH");
    auto saveLabel = Label::create();
    saveLabel->setString("SAVE MAP");
    auto quitLabel = Label::create();
    quitLabel->setString("QUIT");

    auto clearMenuItem = MenuItemLabel::create(clearLabel, std::bind(&HelloWorld::onClearPath, this));
    clearMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto getMenuItem = MenuItemLabel::create(getLabel, std::bind(&HelloWorld::onGetPath, this));
    getMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto saveMenuItem = MenuItemLabel::create(saveLabel, std::bind(&HelloWorld::onSaveThisMap, this));
    saveMenuItem->setAnchorPoint(Vec2(0, 0.5));
    auto quitMenuItem = MenuItemLabel::create(quitLabel, std::bind(&HelloWorld::onQuit, this));
    quitMenuItem->setAnchorPoint(Vec2(0, 0.5));

    auto menu2 = Menu::create(clearMenuItem, getMenuItem, saveMenuItem, quitMenuItem, nullptr);
    menu2->alignItemsVerticallyWithPadding(10);
    menu2->setAnchorPoint(Vec2(0, 0));
    menu2->setPosition(Vec2(origin.x + 10, origin.y + 80));
    this->addChild(menu2);

    // touch
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ready json file
    auto targetFile = FileUtils::getInstance()->getWritablePath() + "target.json";
    if (!FileUtils::getInstance()->isFileExist(targetFile))
    {
        auto jsonStr = FileUtils::getInstance()->getStringFromFile("mapbg.json");
        FileUtils::getInstance()->writeStringToFile(jsonStr, targetFile);
    }

    // init map
    _bgMap = BgMap::createWithFile(targetFile);
    if (!_bgMap)
    {
        FileUtils::getInstance()->removeFile(targetFile);
        _bgMap = BgMap::createWithFile("mapbg.json");
    }
    _bgMap->setPosition(Vec2(origin.x + 130, 5));
    this->addChild(_bgMap);

    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    auto pos = touch->getLocation();
    auto posOnMap = _bgMap->convertToNodeSpace(pos);
    log("[touch] pos on Map: (%f,%f)", posOnMap.x, posOnMap.y);

    switch (_editState) {
        case kAdd_Start:
            _bgMap->addStartPos(posOnMap);
            break;
        case kAdd_End:
            _bgMap->addEndPos(posOnMap);
            break;
        case kAdd_Wall:
            _bgMap->addWallPos(posOnMap);
            break;
        case kRemove_Pos:
            _bgMap->removePos(posOnMap);
            break;
        default:
            break;
    }
    return false;
}

void HelloWorld::onSetStartPos()
{
    _bgMap->clearPath();
    _editState = kAdd_Start;
}
void HelloWorld::onSetEndPos()
{
    _bgMap->clearPath();
    _editState = kAdd_End;
}
void HelloWorld::onSetWallPos()
{
    _bgMap->clearPath();
    _editState = kAdd_Wall;
}
void HelloWorld::onRemovePos()
{
    _bgMap->clearPath();
    _editState = kRemove_Pos;
}

void HelloWorld::onClearPath()
{
    _editState = kUse_Map;
    _bgMap->clearPath();
}
void HelloWorld::onSaveThisMap()
{
    _editState = kUse_Map;
    _bgMap->saveCurrentMap();
}
void HelloWorld::onGetPath()
{
    _editState = kUse_Map;
    _bgMap->getPath();
}

void HelloWorld::onQuit()
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
