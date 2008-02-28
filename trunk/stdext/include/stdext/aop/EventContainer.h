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
// Module: stdext/aop/EventContainer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_AOP_EVENTCONTAINER_H__
#define __STDEXT_AOP_EVENTCONTAINER_H__

#ifndef __STDEXT_AOP_INTERFACE_H__
#include "Interface.h"
#endif

#ifndef __STDEXT_ARRAY_H__
#include "../Array.h"
#endif

// -------------------------------------------------------------------------
// class EventContainer

__NS_STD_BEGIN

template <int EventCount>
class EventContainer : public IEventContainer
{
private:
	typedef IEvent EventBase;
	struct ValueType {
		const EventID* m_name;
		void* m_event;
		ValueType() 
			: m_name(NULL) {
		}
		ValueType(EventIDRef name, EventBase* event)
			: m_name(&name), m_event(event) {}
		int winx_call operator==(EventIDRef name) const {
			return *m_name == name;
		}
	};
	typedef std::Array<ValueType, EventCount> ContainerType;

private:
	ContainerType m_events;

public:
	void winx_call addEvent(EventIDRef name, EventBase& event)
	{
		m_events.push_back(ValueType(name, &event));
	}

	void winx_call addEvent(EventIDRef name, EventBase* event)
	{
		m_events.push_back(ValueType(name, event));
	}

	HRESULT __stdcall findEvent(EventIDRef name, void** event)
	{
		typedef typename ContainerType::iterator Iterator;
		Iterator it = std::find(m_events.begin(), m_events.end(), name);
		if (it != m_events.end())
		{
			*event = (*it).m_event;
			return S_OK;
		}
		return E_FAIL;
	}
};

__NS_STD_END

// -------------------------------------------------------------------------
// class TestEventContainer

#if defined(__STDEXT_TESTCASE_H__)

#ifndef __STDEXT_AOP_EVENT_H__
#include "Event.h"
#endif

//
// Interface part
// 

namespace test_event_container {

typedef int Text;

DEFINE_EVENT(TextChanged, (Text text)); // define event type 'TextChanged'

}; // namespace test_event_container

DEFINE_EID(test_event_container::TextChanged, "FDA4D6CB-91E1-47a0-9D85-E200110EA0E3", 
		0xfda4d6cb, 0x91e1, 0x47a0, 0x9d, 0x85, 0xe2, 0x0, 0x11, 0xe, 0xa0, 0xe3);

// 
// Implementation part
//

namespace test_event_container {

//
// Edit (v2.0) component
//

class EditV2 : public std::EventContainer<1>
{
private:
	typedef EVENT_TYPE(TextChanged, (Text text), (text)) TextChangedEvent;

	Text m_text;
	TextChangedEvent m_changed;

public:
	EditV2(std::ScopeAlloc& alloc) : m_changed(alloc)
	{
		addEvent(EID(TextChanged), m_changed);
	}

	void putText(Text text)
	{
		m_text = text;		
		m_changed.fire(text); // fire 'changed' event when the content of Edit is altered
	}
};

//
// DialogV2 component
//

class DialogV2
{
	std::BlockPool m_recycle;
	std::ScopeAlloc m_alloc;

	EditV2 m_edit;
	std::Connection m_editChanged;
	std::Connection m_editChanged2;
	// NOTE: even you don't need to disconnect, you must hold the connection handle.

public:
	DialogV2() 
		: m_alloc(m_recycle), m_edit(m_alloc)
	{
		TextChanged* textChanged = NULL;
		m_edit.findEvent(EID(TextChanged), (void**)&textChanged);
		// NOTE: I doesn't do failure handling.

		m_editChanged = textChanged->addListener(this, &DialogV2::onEditChanged);
		m_editChanged2 = textChanged->addListener(this, &DialogV2::onEditChanged2);
	}

	void __stdcall onEditChanged(Text text)
	{
		printf("onEditChanged: text=%d\n", text);
	}

	void __stdcall onEditChanged2(Text text)
	{
		printf("onEditChanged2: text=%d\n", text);
	}

	void input(Text text)
	{
		m_edit.putText(text);
	}

	void disconnectEditChanged()
	{
		m_editChanged.disconnect();
	}
};

} // namespace test_event_container

__NS_STD_BEGIN

template <class LogT>
class TestEventContainer : public TestCase
{
	WINX_TEST_SUITE(TestEventContainer);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void test(LogT& log)
	{
		using namespace test_event_container;

		DialogV2 dlg;
		dlg.input(11);
		dlg.input(12);
		dlg.disconnectEditChanged();
		dlg.input(13);
	}
};

__NS_STD_END

#endif // defined(__STDEXT_TESTCASE_H__)

// -------------------------------------------------------------------------
// $Log: $

#endif /* __STDEXT_AOP_EVENTCONTAINER_H__ */
