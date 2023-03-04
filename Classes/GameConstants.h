//
//  GameConstants.h
//  KittyTouch
//
//  Created by 王小兵 on 16/3/15.
//
//

#ifndef GameConstants_h
#define GameConstants_h


template <class T>
std::string ConvertToString(T);

struct GameConstants
{
public:
    constexpr static const float MONSTER_MOVE_SPEED = 1960.0f / 10.0f;
    constexpr static const float NOMARL_SKILL_MOVE_SPEED = 1960.0f / 1.0f;
    constexpr static const float SCENE_HEIGHT = 1920.0f;
};

struct GameZOrder
{
public:
    constexpr static const int BG_ZORDER = 0;
    constexpr static const int MONSTER_ZORDER = 1;
    constexpr static const int PLAYER_ZORDER = 2;
    constexpr static const int SUMMARIZE_UI_ZORDER = 3;
    
};


template <class T>
std::string ConvertToString(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}
#endif /* GameConstants_h */
