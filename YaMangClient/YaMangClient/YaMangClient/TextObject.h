#pragma once
#include "Interface3D.h"
#include "EnumSet.h"

class TextObject//: public Interface3D
{
public:
	TextObject( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom, SceneType sceneType );
	~TextObject();

	void	DrawText() const;

	void	SetText( LPCWSTR text ) { m_Text.clear(); m_Text.append( text ); }
	void	SetLeft( float left ) { m_Left = left; }
	void	SetTop( float top ) { m_Top = top; }
	void	SetRight( float right ) { m_Right = right; }
	void	SetBottom( float bottom ) { m_Bottom = bottom; }

	void	SetColorR( int red ) { m_RGB_R = red; }
	void	SetColorG( int green ) { m_RGB_G = green; }
	void	SetColorB( int blue ) { m_RGB_B = blue; }
	void	SetSceneType( SceneType sceneType ) { m_SceneType = sceneType; }

	SceneType		GetSceneType() { return m_SceneType; }

private:
	std::wstring	m_Text;
	float			m_Left = 0;
	float			m_Top = 0;
	int				m_RGB_R = 255;
	int				m_RGB_G = 255;
	int				m_RGB_B = 255;
	float			m_Right = 0;
	float			m_Bottom = 0;

	SceneType		m_SceneType = SCENE_NONE;

};

