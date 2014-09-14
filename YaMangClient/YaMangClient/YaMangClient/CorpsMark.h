#pragma once
#include "UIObject.h"

class Corps;

class CorpsMark : public UIObject
{
public:
	CorpsMark(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible, Corps* corps, bool ourCorps);
	virtual ~CorpsMark();

	virtual void		Update();
	virtual void		Render() const;

	void				SetCorps(Corps* corps) { m_Corps = corps; }
	void				SetIsOurCorps(bool ourCorps) { m_IsOurCorps = ourCorps; }
	bool				GetIsOurCorps() { return m_IsOurCorps; }
	void				SetMarkPos();
	void				CheckSuicide();

private:
	Corps*				m_Corps;
	bool				m_IsOurCorps = false;
};

