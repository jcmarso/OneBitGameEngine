#include "onebitge.h"
#include <iostream>

auto time1 = chrono::system_clock::now();
auto time2 = chrono::system_clock::now();

OneBitGE::OneBitGE() 
{
	m_screenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	m_screenWidth = 512;
	m_screenHeight = 288;
	m_tileSheetWidth = 320;
	m_tileSheetHeight = 416;
	m_spriteSheetWidth = 96;
	m_spriteSheetHeight = 192;
	m_tileWidth = 32;
	m_tileHeight = 32;
	m_spriteWidth = 32;
	m_spriteHeight = 32;
	m_mapWidthInTiles = 46;
	m_mapHeightInTiles = 27;
	m_elapsedTime = 0.0;
	m_Speed = 20.0;
	m_playerPositionX = 0;
	m_playerPositionY = 0;
	m_previousSprite = 0;
	m_keyInput = ' ';
	m_previousKeyInput = ' ';
	m_pScreen = new wchar_t[m_screenWidth * m_screenHeight];
}

OneBitGE::~OneBitGE()
{
	delete[] m_pScreen;
}

void OneBitGE::Init(int screenWidth, int screenHeight)
{
	_COORD coord = { static_cast<short>(screenWidth), static_cast<short>(screenHeight) };
	SetConsoleScreenBufferSize(m_screenBuffer, coord);

	_CONSOLE_FONT_INFOEX font = { 0 };
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.nFont;
	font.dwFontSize.X = 2;
	font.dwFontSize.Y = 2;
	font.FaceName;
	font.FontFamily = FF_DONTCARE;
	font.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(m_screenBuffer, false, &font);
	
	HWND console = GetConsoleWindow();
	RECT rect;
	GetWindowRect(console, &rect);
	MoveWindow(console, 0, 0, 0, 0, TRUE);
	MoveWindow(console, 0, 0, screenWidth, screenHeight, TRUE);
	SetConsoleActiveScreenBuffer(m_screenBuffer);
}

void OneBitGE::Draw()
{
	wstring appName = L"OneBitGE";
	wchar_t s[256];

	time2 = chrono::system_clock::now();
	chrono::duration<float> passedTime = time2 - time1;
	time1 = time2;
	m_elapsedTime = passedTime.count();
	swprintf_s(s, 256, L"%s - FPS: %3.2f", appName.c_str(), 1.0f / m_elapsedTime);
	SetConsoleTitle(s);

	DWORD dwScreenBufferData = 0;
	WriteConsoleOutputCharacter(m_screenBuffer, m_pScreen, m_screenWidth * m_screenHeight, { 0,0 }, &dwScreenBufferData);
}

void OneBitGE::LoadTileToBuffer(int xPosition, int yPosition, int tileID)
{
	AssetID temp = { tileID, 0, 0 };

	for (int i = 0; i < (m_screenWidth / m_tileWidth) * (m_screenHeight / m_tileHeight); i++) {
		if (m_Tiles[i].id == temp.id) {
			temp.x = m_Tiles[i].x;
			temp.y = m_Tiles[i].y;
			break;
		}
	}
	for (long long i = 0 + xPosition; i < (m_tileWidth + static_cast<long long>(xPosition)); i++) {
		for (long long j = 0 + yPosition; j < (m_tileHeight + static_cast<long long>(yPosition)); j++) {
			m_pScreen[j * m_screenWidth + i] = m_tileSheet[(temp.y + j - static_cast<long long>(yPosition)) * m_tileSheetWidth + (temp.x + i - static_cast<long long>(xPosition))];
		}
	}
}

void OneBitGE::LoadSpriteToBuffer(int xPosition, int yPosition, int spriteID)
{
	AssetID temp = { spriteID, 0, 0 };

	for (int i = 0; i < (m_spriteSheetWidth / m_spriteHeight) * (m_spriteSheetHeight / m_spriteHeight); i++) {
		if (m_Sprites[i].id == temp.id) {
			temp.x = m_Sprites[i].x;
			temp.y = m_Sprites[i].y;
			break;
		}
	}
	for (int i = 0 + xPosition; i < (m_spriteWidth + xPosition); i++) {
		for (int j = 0 + yPosition; j < (m_spriteHeight + yPosition); j++) {
			if (m_spriteSheet[(temp.y + static_cast<std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>::size_type>(j) - yPosition) * m_spriteSheetWidth + (temp.x + static_cast<unsigned long long>(i) - xPosition)] == L'\u2588' || m_spriteSheet[(temp.y + static_cast<std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>::size_type>(j) - yPosition) * m_spriteSheetWidth + (temp.x + static_cast<unsigned long long>(i) - xPosition)] == L' ') {
				m_pScreen[j * m_screenWidth + i] = m_spriteSheet[(temp.y + static_cast<std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>::size_type>(j) - yPosition) * m_spriteSheetWidth + (temp.x + static_cast<unsigned long long>(i) - xPosition)];
			}
		}
	}
}

void OneBitGE::Animation()
{	
	if (GetAsyncKeyState(0x53)) {	// S
		m_keyInput = 'S';
		if (m_playerPositionY < m_screenHeight - m_spriteHeight) {
			m_playerPositionY += 4 * m_Speed * m_elapsedTime;
			LoadMapToBuffer();
			if (m_previousSprite == 0)
				LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 1);

			else if (m_previousSprite == 1)
				LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 2);

			else if (m_previousSprite == 2)
				LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 3);

			else if (m_previousSprite == 3)
				LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 2);
		}
		else 
		m_previousKeyInput = m_keyInput;
		if (m_previousSprite < 3)
			m_previousSprite += 1;
		else
			m_previousSprite = 0;
		
	}
	else if (GetAsyncKeyState(0x41)) {	// A
		m_keyInput = 'A';
			if (m_playerPositionX > 0) {
				m_playerPositionX -= 4 * m_Speed * m_elapsedTime;
				LoadMapToBuffer();
				if (m_previousSprite == 0)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 4);

				else if (m_previousSprite == 1)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 5);

				else if (m_previousSprite == 2)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 6);

				else if (m_previousSprite == 3)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 4);
			}
		m_previousKeyInput = m_keyInput;
		if (m_previousSprite < 3)
			m_previousSprite += 1;
		else
			m_previousSprite = 0;
	}
	else if (GetAsyncKeyState(0x44)) {	// D
		m_keyInput = 'D';
			if (m_playerPositionX < m_screenWidth - m_spriteWidth) {
				m_playerPositionX += 4 * m_Speed * m_elapsedTime;
				LoadMapToBuffer();
				if (m_previousSprite == 0)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 7);

				else if (m_previousSprite == 1)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 8);

				else if (m_previousSprite == 2)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 9);

				else if (m_previousSprite == 3)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 7);
			}
		m_previousKeyInput = m_keyInput;
		if (m_previousSprite < 3)
			m_previousSprite += 1;
		else
			m_previousSprite = 0;
	}
	else if (GetAsyncKeyState(0x57)) {	// W
		m_keyInput = 'W';
			if (m_playerPositionY > 0) {
				m_playerPositionY -= 4 * m_Speed * m_elapsedTime;
				LoadMapToBuffer();
				if (m_previousSprite == 0)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 10);

				else if (m_previousSprite == 1)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 11);

				else if (m_previousSprite == 2)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 12);

				else if (m_previousSprite == 3)
					LoadSpriteToBuffer(m_playerPositionX, m_playerPositionY, 10);
			}
		m_previousKeyInput = m_keyInput;
		if (m_previousSprite < 3)
			m_previousSprite += 1;
		else
			m_previousSprite = 0;
	}
}

void OneBitGE::Input()
{
	
}

void OneBitGE::GetMapFromFile(string fileName)
{
	string temp;
	ifstream file3(fileName);
	if (file3.is_open())
	{
		while (getline(file3, temp))
		{
			stringstream linestream(temp);
			string token;
			while (getline(linestream, token, ',')) {
				m_Map.push_back(stoi(token));
			}
		}
	}
}

void OneBitGE::LoadMapToBuffer()
{
	for (unsigned __int64 i = 0; i < m_screenWidth / m_tileWidth; i++) {
		for (unsigned __int64 j = 0; j < m_screenHeight / m_tileHeight; j++) {
			LoadTileToBuffer(i * m_tileWidth, j * m_tileHeight, m_Map.at(j * m_mapWidthInTiles + i));
		}
	}
}

void OneBitGE::AssignCodeToTiles(int tileWidth, int tileHeight, int tileSheetWidth, int tileSheetHeight)
{
	AssetID temp;
	int id = 1;
	int x = 0;
	int y = 0;

	for (int i = 0; i < (m_tileSheetWidth / m_tileWidth); i++) 
	{
		for (int j = 0; j < (m_tileSheetHeight / m_tileHeight); j++) 
		{
			id = (j * (m_tileSheetWidth / m_tileWidth)) + i + 1;
			temp = { id, x * m_tileWidth, y * m_tileHeight };
			m_Tiles.push_back(temp);
			y++;
		}
		x++;
		y = 0;
	}
}

void OneBitGE::AssignCodeToSprites(int spriteWidth, int spriteHeight, int spriteSheetWidth, int spriteSheetHeight)
{
	AssetID temp;
	int id = 1;
	int x = 0;
	int y = 0;

	for (int i = 0; i < (m_spriteSheetWidth / m_spriteWidth); i++) 
	{
		for (int j = 0; j < (m_spriteSheetHeight / m_spriteHeight); j++) 
		{
			id = (j * (m_spriteSheetWidth / m_spriteWidth)) + i + 1;
			temp = { id, x * m_spriteWidth, y * m_spriteHeight };
			m_Sprites.push_back(temp);
			y++;
		}
		x++;
		y = 0;
	}
}

void OneBitGE::GetTileSheetFromFile(string fileName)
{
	string temp;
	ifstream file(fileName);
	if (file.is_open())
	{
		while (getline(file, temp))
		{
			for (int i = 0; i < temp.size(); i++) 
			{
				if (temp[i] == '1')
					m_tileSheet += L'\u2588';
				else if (temp[i] == '0')
					m_tileSheet += L' ';
			}
		}
	}
}

void OneBitGE::GetSpriteSheetFromFile(string fileName)
{
	string temp;
	ifstream file(fileName);
	if (file.is_open())
	{
		while (getline(file, temp))
		{
			for (int i = 0; i < temp.size(); i++)
			{
				if (temp[i] == '1')
					m_spriteSheet += L'\u2588';
				else if (temp[i] == '0')
					m_spriteSheet += L' ';
				else
					m_spriteSheet += L'X'; // "Alpha" channel
			}
		}
	}
}
