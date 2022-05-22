#ifndef ONEBITGE_H_
#define ONEBITGE_H_

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

class AssetID	// Used for map tiles and character sprites IDs
{
public:
    int id = 0, x = 0, y = 0;
};

class OneBitGE
{
public:
    OneBitGE();
    ~OneBitGE();
    void Init(int screenWidth, int screenHeight);
    void GetTileSheetFromFile(string fileName);
    void GetSpriteSheetFromFile(string fileName);
    void GetMapFromFile(string fileName);
    void AssignCodeToTiles(int tileWidth, int tileHeight, int tileSheetWidth, int tileSheetHeight);
    void AssignCodeToSprites(int spriteWidth, int spriteHeight, int spriteSheetWidth, int spriteSheetHeight);
    void LoadMapToBuffer();
    void LoadTileToBuffer(int xPosition, int yPosition, int tileID);
    void LoadSpriteToBuffer(int xPosition, int yPosition, int spriteID);
    void Animation();
    void Input();
    void Draw();
private:
    HANDLE m_screenBuffer;
    vector<AssetID> m_Tiles;
    vector<AssetID> m_Sprites;
    vector<int> m_Map;
    wstring m_tileSheet;
    wstring m_spriteSheet;
    wchar_t* m_pScreen = nullptr;
    unsigned __int64 m_screenWidth;
    unsigned __int64 m_screenHeight;
    int m_tileSheetWidth;
    int m_tileSheetHeight;
    int m_spriteSheetWidth;
    int m_spriteSheetHeight;
    int m_tileWidth;
    int m_tileHeight;
    int m_spriteWidth;
    int m_spriteHeight;
    int m_mapWidthInTiles;
    int m_mapHeightInTiles;
    int m_playerPositionX;
    int m_playerPositionY;
    int m_previousSprite;
    char m_keyInput;
    char m_previousKeyInput;
    float m_elapsedTime;
    float m_Speed;
};

#endif // ONEBITGE_H_