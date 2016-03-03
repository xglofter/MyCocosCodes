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

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::onQuit, this));
    closeItem->setAnchorPoint(Vec2(1,1));

    // create menu, it's an autorelease object
    auto mainMenu = Menu::create(closeItem, NULL);
    mainMenu->setAnchorPoint(Vec2(1,1));
    mainMenu->setPosition(Vec2(origin.x + visibleSize.width - 20, origin.y + visibleSize.height - 20));
    this->addChild(mainMenu, 10);

    // Tetris Main UI
    _tetrisLayer = TetrisLayer::create();
    _tetrisLayer->setPosition(Vec2(10, 10));
    _tetrisLayer->setScoreCallback(std::bind(&HelloWorld::onGetScore, this, std::placeholders::_1));
    _tetrisLayer->setGameOverCallback(std::bind(&HelloWorld::onGameOver, this));
    this->addChild(_tetrisLayer);

    // left menus
    auto rotateLabel = Label::createWithTTF("Rotate", "arial.ttf", 40);
    auto downLabel = Label::createWithTTF("Down", "arial.ttf", 40);
    auto leftLabel = Label::createWithTTF("Left", "arial.ttf", 40);
    auto rightLabel = Label::createWithTTF("Right", "arial.ttf", 40);

    auto rotateMenuItem = MenuItemLabel::create(rotateLabel, std::bind(&HelloWorld::onRotateShape, this));
    auto downMenuItem = MenuItemLabel::create(downLabel, std::bind(&HelloWorld::onMoveDownShape, this));
    auto leftMenuItem = MenuItemLabel::create(leftLabel, std::bind(&HelloWorld::onMoveLeftShape, this));
    auto rightMenuItem = MenuItemLabel::create(rightLabel, std::bind(&HelloWorld::onMoveRightShape, this));

    rotateMenuItem->setPosition(Vec2(0, 70));
    downMenuItem->setPosition(Vec2(0, -70));
    leftMenuItem->setPosition(Vec2(-90, 0));
    rightMenuItem->setPosition(Vec2(90, 0));

    auto menu = Menu::create(rotateMenuItem, downMenuItem, leftMenuItem, rightMenuItem, nullptr);
    menu->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + 150));
    this->addChild(menu);

    // score label
    _scoreLabel = Label::createWithTTF("Score: 0", "arial.ttf", 40);
    _scoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2 - 130, origin.y + visibleSize.height - 50));
    _scoreLabel->setAnchorPoint(Vec2(0, 0.5));
    addChild(_scoreLabel);

    return true;
}

void HelloWorld::onRotateShape()
{
    _tetrisLayer->rotateShape();
}

void HelloWorld::onMoveLeftShape()
{
    _tetrisLayer->moveShapeLeft();
}

void HelloWorld::onMoveRightShape()
{
    _tetrisLayer->moveShapeRight();
}

void HelloWorld::onMoveDownShape()
{
    _tetrisLayer->moveShapeDown();
}

void HelloWorld::onQuit(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onGetScore(std::string score)
{
    if (score == "") return ;
    log("onGetScore %s", score.c_str());
    _scoreLabel->setString("Score: "+score);
}

void HelloWorld::onGameOver()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Game Over", "arial.ttf", 60);
    label->setColor(Color3B(255, 0, 0));
    label->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    addChild(label);
}
