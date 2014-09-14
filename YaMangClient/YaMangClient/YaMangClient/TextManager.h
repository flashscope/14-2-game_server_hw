#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

enum TextType
{
	TEXT_NONE,
	TEXT_ROOM_NUMBER,
	TEXT_FPS,
	TEXT_GAME_RESULT,
	TEXT_REGEN_TIME,
	TEXT_CORPS_NUM,
	TEXT_BASE_NUM,
	TEXT_TEST_BASE,
	TEXT_TEST_KILLED_CORPS_NUM,
	TEXT_TEST_DEATH_CORPS_NUM,
	TEXT_TEST_TOTAL_MY_CORPS_NUM,
	TEXT_MAX,
};

class TextObject;
class TextManager:public Singleton<TextManager>
{
public:
	TextManager();
	~TextManager();

	void	RegistText(TextType key, LPCWSTR text, float left, float top, SceneType sceneType, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0);
	void	DeleteText( TextType key );

	void	DrawTexts() const;

private:
	std::array<TextObject*, TEXT_MAX> m_TextList;
};

