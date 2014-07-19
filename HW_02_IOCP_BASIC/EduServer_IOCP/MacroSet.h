#pragma once

template <typename ... Types >
inline void Log(const Types& ...args)
{
#ifdef _DEBUG
	//printf_s(args...);
#endif // _DEBUG
}
