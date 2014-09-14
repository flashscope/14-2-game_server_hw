#include "stdafx.h"
#include "TextManager.h"
#include "TextObject.h"
#include "SceneManager.h"

TextManager::TextManager()
{
	m_TextList.fill( nullptr );

	for ( auto& it : m_TextList )
	{
		TextObject* textOBJ = new TextObject( L"", -9999.0f, -9999.0f, 255, 255, 255, 0, 0, SCENE_NONE );
		it = textOBJ;
	}
}


TextManager::~TextManager()
{
	for ( auto& it : m_TextList )
	{
		TextObject* toBeDelete = it;
		SafeDelete( toBeDelete );
	}
	m_TextList.fill( nullptr );
}

void TextManager::RegistText(TextType key, LPCWSTR text, float left, float top, SceneType sceneType, int RGB_R /*= 255*/, int RGB_G /*= 255*/, int RGB_B /*= 255*/, float right /*= 0*/, float bottom /*= 0*/)
{
	TextObject* textOBJ = m_TextList[key];
	textOBJ->SetText(text);
	textOBJ->SetLeft(left);
	textOBJ->SetTop(top);
	textOBJ->SetColorR(RGB_R);
	textOBJ->SetColorG(RGB_G);
	textOBJ->SetColorB(RGB_B);
	textOBJ->SetRight(right);
	textOBJ->SetBottom(bottom);
	textOBJ->SetSceneType(sceneType);
}

void TextManager::DeleteText( TextType key )
{
	TextObject* textOBJ = m_TextList[key];
	textOBJ->SetText( L"" );
	textOBJ->SetLeft( -9999.0f );
	textOBJ->SetTop( -9999.0f );
}

void TextManager::DrawTexts() const
{
	for ( auto& it : m_TextList )
	{
		TextObject* textOBJ = it;
		if (it->GetSceneType() == SceneManager::GetInstance()->GetNowSceneType())
		{
			textOBJ->DrawText();
		}
	}
}

