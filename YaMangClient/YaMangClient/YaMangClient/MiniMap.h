#pragma once
#include "UIObject.h"
class MiniMap : public UIObject
{
public:
	MiniMap(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible);
	virtual ~MiniMap();

	virtual void		Update();
	virtual void		Render() const;

private:
};

