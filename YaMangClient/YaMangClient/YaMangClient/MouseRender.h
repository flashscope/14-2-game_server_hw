#pragma once
#include "MacroSet.h"
#include "Interface3D.h"
#include "EnumSet.h"

class MouseRender : public Interface3D
{
public:
	MouseRender();
	~MouseRender();

public:
	void				Init();
	virtual void		Update();
	virtual void		Render() const;
	
	void				SetGameCursorPos( int PosX, int PosY );

private:
	bool				m_IsCursorReady = false;
	int					m_CursorHotspotX = 4;
	int					m_CursorHotspotY = 3;
};

