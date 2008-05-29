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
// Module: tpl/regex/Find.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_FIND_H
#define TPL_REGEX_FIND_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_KMP_FINDER_H
#include "../../../../stdext/include/stdext/kmp/Finder.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class NotCh_

class NotCh_
{
private:
	int m_c;

public:
	NotCh_(int c) : m_c(c) {}

	bool TPL_CALL operator()(int c) const {
		return m_c != c;
	}
};

// -------------------------------------------------------------------------
// function ch_while

// Usage: ch_while(cond)
// Usage: ch_while<true>(cond)

template <bool bEat, class SourceT, class PredT>
inline bool TPL_CALL match_while(SourceT& ar, PredT pred)
{
	typename SourceT::iterator pos = ar.position();
	typename SourceT::int_type c;
	while (pred(c = ar.get()))
	{
		if (c == SourceT::endch)
		{
			ar.seek(pos);
			return false;
		}
	}
	if (!bEat)
		ar.unget();
	return true;
}

template <bool bEat, class SourceT>
inline bool TPL_CALL match_while_not_ch(SourceT& ar, typename SourceT::int_type c) {
	NotCh_ notCh(c);
	return match_while<bEat>(ar, notCh);
}

template <class PredT, bool bEat = false>
class While
{
private:
	PredT m_pred;

public:
	template <class T1>
	While(const T1& pred) : m_pred(pred) {}

public:
	enum { character = 0 };
	enum { vtype = 0 };

	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_while<bEat>(ar, m_pred);
	}
};

template <bool bEat, class PredT>
__forceinline Rule<While<PredT, bEat> > TPL_CALL ch_while(PredT pred) {
	return Rule<While<PredT, bEat> >(pred);
}

template <class PredT>
__forceinline Rule<While<PredT, false> > TPL_CALL ch_while(PredT pred) {
	return Rule<While<PredT, false> >(pred);
}

// -------------------------------------------------------------------------
// function find

// Usage: find('c')				--- means: find character 'c'. ('c' remains in the inputstream)
// Usage: find<true>('c')		--- means: find character 'c' and eat it.

template <bool bEat = false>
class FindCh : public While<NotCh_, bEat>
{
public:
	FindCh(int x) : While<NotCh_, bEat>(x) {}
};

// -------------------------------------------------------------------------
// function find(val)

template <class Type, bool bEat = false>
struct FindTraits {
};

template <bool bEat>
struct FindTraits<char, bEat> {
	typedef FindCh<bEat> find_type;
};

template <bool bEat>
struct FindTraits<wchar_t, bEat> {
	typedef FindCh<bEat> find_type;
};

template <bool bEat>
struct FindTraits<int, bEat> {
	typedef FindCh<bEat> find_type;
};

template <bool bEat, class Type> __forceinline
Rule<typename FindTraits<Type, bEat>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, bEat>::find_type>(val);
}

template <class Type> __forceinline
Rule<typename FindTraits<Type, false>::find_type> TPL_CALL find(const Type& val) {
	return Rule<typename FindTraits<Type, false>::find_type>(val);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_FIND_H */