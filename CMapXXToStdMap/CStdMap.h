#pragma once
#include <Windows.h> //include for DWORD, INT_PTR etc
#include <unordered_map>
#include <string>

//uncomment the following header if you want to use the other MFC features such as CString
//#include <afxwin.h>

//if MFC is not used, define POSITION
#ifndef __AFXWIN_H__
struct __POSITION {};
typedef __POSITION* POSITION;
#endif // !__AFXWIN_H__

/// <summary>
/// templated class to use std::unordered map and provide MFC CMapXXX container like functionality
/// please inherit or specialize if you see missing function/modify the behavior.
/// </summary>
/// <typeparam name="__KEY">typename for the container Key</typeparam>
/// <typeparam name="VAL">typename for the container Val</typeparam>
/// <typeparam name="KEY_VALTYPE">argument type to be passed in the function for key</typeparam>
/// <typeparam name="VAL_VALTYPE">argument type to passed in function for value </typeparam>

template<typename __KEY, typename VAL, typename KEY_VALTYPE, typename VAL_VALTYPE>
class CStdMap
{
	__POSITION m_pos = {};
	POSITION __pos = &m_pos;
public:
	
//Handling for CMapCStringToCString, in this case key will be LPCTSTR
#ifdef __AFXWIN_H__
	using KEY = typename std::conditional<std::is_same_v<__KEY, CString>, LPCTSTR, __KEY>::type;
#else
	using KEY = typename __KEY;
#endif
	std::unordered_map<KEY, VAL> m_map;
	INT_PTR GetCount() const
	{
		return 0;
	}
	INT_PTR GetSize() const
	{
		return m_map.size();
	}
	BOOL IsEmpty() const
	{
		return m_map.empty();
	}

	// Lookup
	BOOL Lookup(KEY key, VAL_VALTYPE& rValue) const
	{
		auto itr = m_map.find(key);
		if (itr == m_map.end())
			return FALSE;
		rValue = &itr.second;
		return TRUE;
	}
	BOOL LookupKey(KEY_VALTYPE key, KEY_VALTYPE& rKey) const
	{
		for (auto itr = m_map.begin(); itr != m_map.end(); ++itr)
		{
			if (itr->second == key)
			{
				rKey = itr->first;
				return TRUE;
			}
		}
		return FALSE;
	}

	// Operations
		// Lookup and add if not there
	VAL_VALTYPE& operator[](KEY_VALTYPE key)
	{
		if (m_map.find(key) == m_map.end())
		{
			m_map.emplace(T(key), {});
		}
		return &m_map[key];
	}

	// add a new (key, value) pair
	void SetAt(KEY_VALTYPE key, VAL_VALTYPE newValue)
	{
		//KEY _key = { key };
		//auto x = std::pair<KEY_VALTYPE, VAL_VALTYPE>(key, newValue);
		//m_map.insert(x);
		m_map.insert({ key, newValue });
	}

	// removing existing (key, ?) pair
	BOOL RemoveKey(KEY_VALTYPE key)
	{
		return m_map.erase(key) > 0 ? TRUE : FALSE;
	}
	void RemoveAll()
	{
		m_map.clear();
	}

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const
	{
		if (m_map.size() <= 0)
			return NULL;
		*(int*)__pos = 0;
		return __pos;
	}
	void GetNextAssoc(POSITION& rNextPosition, __KEY& rKey, VAL& rValue)
	{
		unsigned int*& pos = reinterpret_cast<unsigned int*&>(rNextPosition);
		if (pos == NULL || *pos >= m_map.size())
		{
			pos = NULL;
			return;
		}
		auto itr = m_map.begin();
		auto nxt = std::next(itr, *pos);
		rKey = nxt->first;
		rValue = nxt->second;
		if (*pos == m_map.size() - 1)
		{
			pos = NULL;
		}
		else
		{
			*pos += 1;
		}
	}

	// advanced features for derived classes
	//UINT GetHashTableSize() const;
	//void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

	//// Overridables: special non-virtual (see map implementation for details)
	//	// Routine used to user-provided hash keys
	//UINT HashKey(LPCTSTR key) const;

};