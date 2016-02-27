#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BgMap.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    enum EditState
    {
        kAdd_Start,
        kAdd_End,
        kAdd_Wall,
        kRemove_Pos,
        kUse_Map,
    };
    EditState _editState;

    void onSetStartPos();
    void onSetEndPos();
    void onSetWallPos();
    void onRemoveAll();
    void onRemovePos();

    void onClearPath();
    void onGetPath();
    void onSaveThisMap();
    void onQuit();

private:
    BgMap *_bgMap;
};

#endif // __HELLOWORLD_SCENE_H__
