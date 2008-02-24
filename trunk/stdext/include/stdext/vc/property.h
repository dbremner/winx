/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/vc/property.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_VC_PROPERTY_H__
#define __STDEXT_VC_PROPERTY_H__

#ifndef __STDEXT_PLATFORM_H__
#include "../Platform.h"
#endif

// =========================================================================
// parent_class_ptr - for general use

#ifndef _offsetof
#	if defined(X_CC_GCC)
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)64)->member - 64 )
#	else
#		define _offsetof(coclass, member)	( (size_t)&((coclass*)0)->member )
#	endif
#endif

#ifndef parent_class_ptr
#define parent_class_ptr(ParentClass, member)	\
	( (ParentClass*)((const char*)(this) - _offsetof(ParentClass, member)) )
#endif

// =========================================================================
// WINX_THISCLASS

#define WINX_THISCLASS(Type)												\
private:																	\
	typedef Type ThisClass;													\

// =========================================================================
// Boolean operations

#define _WINX_PROPERTY_BOOLOP(Type, Var)									\
	template <class T2>														\
	inline bool operator==(const T2& b) const {								\
		return (Type)*this == b;											\
	}																		\
	template <class T2>														\
	inline bool operator!=(const T2& b) const {								\
		return (Type)*this != b;											\
	}																		\
	template <class T2>														\
	inline bool operator<(const T2& b) const {								\
		return (Type)*this < b;												\
	}																		\
	template <class T2>														\
	inline bool operator<=(const T2& b) const {								\
		return (Type)*this <= b;											\
	}																		\
	template <class T2>														\
	inline bool operator>(const T2& b) const {								\
		return (Type)*this > b;												\
	}																		\
	template <class T2>														\
	inline bool operator>=(const T2& b) const {								\
		return (Type)*this >= b;											\
	}

// =========================================================================
// WINX_DEFINE_PROPERTY_RW, WINX_DEFINE_PROPERTY_RO, WINX_DEFINE_PROPERTY_WO
// WINX_DEFINE_PROPERTY

#define _WINX_PROPERTY_READ(Type, Var, get)									\
	inline operator Type() const                							\
	{																		\
		ThisClass* pThis = parent_class_ptr(ThisClass, Var);				\
		return pThis->get();												\
	}																		\
	_WINX_PROPERTY_BOOLOP(Type, Var)

#define _WINX_PROPERTY_WRITE(Type, Var, put)								\
	inline void operator=(const Type& _winx_value) const					\
	{																		\
		ThisClass* pThis = parent_class_ptr(ThisClass, Var);				\
		pThis->put(_winx_value);											\
	}

#define _WINX_PROPERTY(Var)													\
	friend struct _winxpr_##Var;											\
	_winxpr_##Var Var

#define WINX_DEFINE_PROPERTY_RW(Type, Var, get, put)						\
	struct _winxpr_##Var {													\
		_WINX_PROPERTY_READ(Type, Var, get)									\
		_WINX_PROPERTY_WRITE(Type, Var, put)								\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_PROPERTY_RO(Type, Var, get)								\
	struct _winxpr_##Var {													\
		_WINX_PROPERTY_READ(Type, Var, get)									\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_PROPERTY_WO(Type, Var, put)								\
	struct _winxpr_##Var {													\
		_WINX_PROPERTY_WRITE(Type, Var, put)								\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_PROPERTY(Type, Name, Get, Put) 							\
	WINX_DEFINE_PROPERTY_RW(Type, Name, Get, Put)

// =========================================================================
// DEFINE_PROPERTY_RW, DEFINE_PROPERTY_RO, DEFINE_PROPERTY_WO
// DEFINE_PROPERTY

#if !defined(_DEBUG) && defined(X_CC_VC)

#define DEFINE_PROPERTY_RO(Type, Name, Method)								\
	Type __declspec(property(get=Method)) Name

#define DEFINE_PROPERTY_WO(Type, Name, Method)								\
	Type __declspec(property(put=Method)) Name

#define DEFINE_PROPERTY_RW(Type, Name, Get, Put)							\
	Type __declspec(property(get=Get,put=Put)) Name

#elif !defined(_DEBUG) && defined(X_CC_BCB)

#define DEFINE_PROPERTY_RO(Type, Name, Method)								\
	__property Type Name = {read=Method}

#define DEFINE_PROPERTY_WO(Type, Name, Method)								\
	__property Type Name = {write=Method}

#define DEFINE_PROPERTY_RW(Type, Name, Get, Put)							\
	__property Type Name = {read=Get, write=Put}

#else

#define DEFINE_PROPERTY_RO(Type, Name, Method)								\
	WINX_DEFINE_PROPERTY_RO(Type, Name, Method)

#define DEFINE_PROPERTY_WO(Type, Name, Method)								\
	WINX_DEFINE_PROPERTY_WO(Type, Name, Method)

#define DEFINE_PROPERTY_RW(Type, Name, Get, Put)							\
	WINX_DEFINE_PROPERTY_RW(Type, Name, Get, Put)

#endif

#define DEFINE_PROPERTY(Type, Name, Get, Put) DEFINE_PROPERTY_RW(Type, Name, Get, Put)

// =========================================================================
// WINX_DEFINE_ALIAS_RW, WINX_DEFINE_ALIAS_RO, WINX_DEFINE_ALIAS_RO
// WINX_DEFINE_ALIAS

#define _WINX_ALIAS_READ(Type, Var, Member)									\
	inline operator const Type&() const                						\
	{																		\
		ThisClass* pThis = parent_class_ptr(ThisClass, Var);				\
		return pThis->Member;												\
	}																		\
	_WINX_PROPERTY_BOOLOP(const Type&, Var)

#define _WINX_ALIAS_WRITE(Type, Var, Member)								\
	inline void operator=(const Type& _winx_value) const					\
	{																		\
		ThisClass* pThis = parent_class_ptr(ThisClass, Var);				\
		pThis->Member = _winx_value;										\
	}

#define WINX_DEFINE_ALIAS_RW(Type, Var, Member)								\
	struct _winxpr_##Var {													\
		_WINX_ALIAS_READ(Type, Var, Member)									\
		_WINX_ALIAS_WRITE(Type, Var, Member)								\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_ALIAS_RO(Type, Var, Member)								\
	struct _winxpr_##Var {													\
		_WINX_ALIAS_READ(Type, Var, Member)									\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_ALIAS_WO(Type, Var, Member)								\
	struct _winxpr_##Var {													\
		_WINX_ALIAS_WRITE(Type, Var, Member)								\
	};																		\
	_WINX_PROPERTY(Var)

#define WINX_DEFINE_ALIAS(Type, Var, Member)								\
	WINX_DEFINE_ALIAS_RO(Type, Var, Member)

// =========================================================================
// DEFINE_ALIAS_RW/DEFINE_ALIAS_RO/DEFINE_ALIAS_WO
// DEFINE_ALIAS

#if !defined(_DEBUG) && defined(X_CC_BCB)

#define DEFINE_ALIAS_RO(Type, Name, Member)
	__property Type Name = {read=Member}

#define DEFINE_ALIAS_WO(Type, Name, Member)									\
	__property Type Name = {write=Member}

#define DEFINE_ALIAS_RW(Type, Name, Member)									\
	__property Type Name = {read=Member, write=Member}

#else

#define DEFINE_ALIAS_RO(Type, Name, Member)									\
	WINX_DEFINE_ALIAS_RO(Type, Name, Member)

#define DEFINE_ALIAS_WO(Type, Name, Member)									\
	WINX_DEFINE_ALIAS_WO(Type, Name, Member)

#define DEFINE_ALIAS_RW(Type, Name, Member)									\
	WINX_DEFINE_ALIAS_RW(Type, Name, Member)

#endif

#define DEFINE_ALIAS(Type, Var, Member)	DEFINE_ALIAS_RO(Type, Var, Member)

// =========================================================================
// Example Code

#if (0)

class FooProp
{
	WINX_THISCLASS(FooProp);

private:
	typedef std::string string;
	
	string m_strText;
	string m_strToAlias;

public:
	DEFINE_PROPERTY_RW(string, Text, get_Text, put_Text);
	DEFINE_PROPERTY_RO(string, TextRO, get_Text);
	DEFINE_PROPERTY_WO(string, TextWO, put_Text);

	DEFINE_ALIAS_RW(string, Alias, m_strToAlias);
	DEFINE_ALIAS_RO(string, AliasRO, m_strToAlias);
	DEFINE_ALIAS_WO(string, AliasWO, m_strToAlias);

	void put_Text(const std::string& value) {
		m_strText = value;
	}
	std::string get_Text() const {
		return m_strText;
	}
};

void testProperty()
{
	FooProp foo;

	foo.Text = "ABC";
	AssertExp(foo.Text == "ABC");
	AssertExp(foo.Text == (std::string)foo.TextRO);

	foo.TextWO = "123";
	AssertExp(foo.Text == "123");
	AssertExp(foo.TextRO == (std::string)foo.Text);

	foo.Alias = "ABC";
	AssertExp(foo.Alias == "ABC");
	AssertExp(foo.Alias == (std::string)foo.AliasRO);

	foo.AliasWO = "123";
	AssertExp(foo.Alias == "123");
	AssertExp(foo.AliasRO == (std::string)foo.Alias);
}

#endif

// =========================================================================
// $Log: $

#endif /* __STDEXT_VC_PROPERTY_H__ */
