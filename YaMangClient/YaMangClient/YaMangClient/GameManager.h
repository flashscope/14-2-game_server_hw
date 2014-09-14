#pragma once

#include "MacroSet.h"

struct MESHOBJECT;

class GameManager : public Singleton<GameManager>
{
public:
	GameManager();
	~GameManager();

	bool Init() const;
	bool Process() const;
	void Destroy() const;
	void Stop() { m_Continue = false;  }

private:
	bool			m_Continue = true;
};