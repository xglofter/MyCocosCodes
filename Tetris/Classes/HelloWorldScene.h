#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <string>
#include "cocos2d.h"
#include "TetrisLayer.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void onRotateShape();
    void onMoveLeftShape();
    void onMoveRightShape();
    void onMoveDownShape();

    void onQuit(Ref* pSender);

    void onGetScore(std::string score);
    void onGameOver();

private:
    TetrisLayer *_tetrisLayer;
    cocos2d::Label *_scoreLabel;
};

#endif // __HELLOWORLD_SCENE_H__
