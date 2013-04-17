/*
 *一个sceneContainer，包含了一个Default Effect(默认，可以替换)，包含了一个effect列表（map），以effect名来索引。
 *包含了一个Entity列表（在遍历结点的时候，可以根据结点从Entity列表中寻找结点中对应的Entity）,当然还包括场景树的根节点，以及整棵树。
 *
 *在container的结点列表（单独放到一个列表里）保存上对应的Entity的指针。
 *这样以后遍历结点，可以通过这个列表查找是否有对应的Entity，有几个，然后逐一创建= =！
*/
#ifndef MYGAMESCENECONTAINER_H
#define MYGAMESCENECONTAINER_H

#include <map>
#include <vector>
#include <string>
using std::map;
using std::vector;
using std::string;

#include "MyGameNode.h"
#include "MyGame3DEffect.h"
#include "MyGameScene.h"
using namespace MyGameScene;

class MyGameSceneContainer
{
private:
    map< string, MyGame3DEffect* >  effectMap;
    map< MyGameSceneEntity*, MyGame3DEffect* > entityMap;
    MyGameSceneNode*                rootNode;

    unsigned int                    clonedCount;


    //void cloneNodesWithEntities(MyGameSceneManager *mgr, vector<MyGameSceneEntity *> &entityVec, MyGameSceneNode *&root);

    void cloneNodesWithEntities(MyGameSceneManager *mgr,
                                vector<MyGameSceneEntity *> &entityVec,
                                MyGameNode *originNode,
                                MyGameNode *newParentNode,
                                unsigned int num );

    static MyGame3DEffect*      defaultEffect;

    static const string         defaultEffectName;
public:
    MyGameSceneContainer( MyGameSceneManager* mgr, const string& sceneName );
    ~MyGameSceneContainer();

    static void loadDefaultEffect( const string& fileName );

    static void setDefaultEffect( MyGame3DEffect* );

    MyGameNode* clone(MyGameSceneManager* mgr,
                vector<MyGameSceneEntity*>& entityVec);

    MyGameSceneNode*    getRootNode();

    void addEffect( const string& name, MyGame3DEffect* effect );
    void addEntity( MyGameSceneEntity* entity, const string effectName = defaultEffectName );
    void setVisible( bool );
};

#endif // MYGAMESCENECONTAINER_H
