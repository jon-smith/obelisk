#include "catch.hpp"

#include "ObserverPatternCallbackTests.h"
#include "obelisk/FormatString.hpp"

using namespace obelisk;
using namespace ObserverTestHelpers;

enum class ObsCallPrint
{
	PRINT,
	NO_PRINT
};

template <ObsCallPrint print, typename... Args> ObserverWithSingleCallback<Args...> createBasicObserver()
{
	return ObserverWithSingleCallback<Args...>([](Args... args) {
		if (print == ObsCallPrint::PRINT)
		{
			if (sizeof...(Args) > 0)
			{
				std::wstringstream ss;
				ss << L"In callback with args: ";
				printArgsToStream(ss, args...);
				UNSCOPED_INFO(ss.str().c_str());
			}
			else
			{
				UNSCOPED_INFO("In callback (no args)");
			}
		}
	});
}

template <class ObsT, class EventT> void waitUntilAllEventsProcessed(const ObsT &o, const EventT &e, std::chrono::milliseconds maxWaitTimePerEvent)
{
	auto lastReceivedCount = o.getCallCount();
	std::this_thread::sleep_for(maxWaitTimePerEvent);
	while (lastReceivedCount < o.getCallCount())
	{
		UNSCOPED_INFO(obelisk::formatString("Sent events %d, received events %d", e.getInvokeCount(), o.getCallCount()).c_str());
		lastReceivedCount = o.getCallCount();
		std::this_thread::sleep_for(maxWaitTimePerEvent);
	}
}

template <bool useSharedEventHandler> void ThrottledEvent_EventFirst_TestImpl()
{
	SingleAsyncEventObject<useSharedEventHandler, 0, false> singleEvents;
	ObserverWithSingleCallback<> obs = {[]() {}};
	obs.registerToEvent(singleEvents.testEvent, [&]() {
		obs.callFunc();
		// Delay here so events get sent in the meantime (and ignored)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	// Start, wait, stop
	UNSCOPED_INFO("Starting event send...");
	singleEvents.startSendingEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	singleEvents.stopSendingEvents();
	UNSCOPED_INFO("Stopped event sending");

	// Some time to let things process
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	// Get counts of send and received
	auto nSent = singleEvents.getInvokeCount();
	auto nReceived = obs.getCallCount();

	UNSCOPED_INFO(obelisk::formatString("Invoked events %d, received events %d", nSent, nReceived).c_str());
	// Received events should be less than sent events
	REQUIRE(nSent > nReceived);
}

template <bool useSharedEventHandler> void ThrottledEvent_ObserverFirst_TestImpl()
{
	// Check that the event can be destroyed before the observer
	auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
	SingleAsyncEventObject<useSharedEventHandler, 5, false> singleEvents;
	obs.registerToEvent(singleEvents.testEvent, [&]() {
		obs.callFunc();
		// Delay here so events get sent in the meantime (and ignored)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	// Start, wait, (destruct)
	UNSCOPED_INFO("Starting event send...");
	singleEvents.startSendingEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	UNSCOPED_INFO("Cleaning up...");
}

template <bool useSharedEventHandler> void ProcessAllEvents_EventFirst_TestImpl()
{
	SingleAsyncEventObject<useSharedEventHandler, 0, true> singleEvents;
	auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
	obs.registerToEvent(singleEvents.testEvent, [&]() { obs.callFunc(); });

	// Start, wait, stop
	UNSCOPED_INFO("Starting event send...");
	singleEvents.startSendingEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	singleEvents.stopSendingEvents();
	UNSCOPED_INFO("Stopped event sending");

	// While we're still receiving events, wait
	waitUntilAllEventsProcessed(obs, singleEvents, std::chrono::milliseconds(100));

	// Compare counts of send and received
	UNSCOPED_INFO(obelisk::formatString("Sent events %d, received events %d", singleEvents.getInvokeCount(), obs.getCallCount()).c_str());
	// Received events should be equal to sent events

	REQUIRE(singleEvents.getInvokeCount() == obs.getCallCount());
}

template <bool useSharedEventHandler> void ProcessAllEvents_ObserverFirst_TestImpl()
{
	auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
	SingleAsyncEventObject<useSharedEventHandler, 0, true> singleEvents;
	obs.registerToEvent(singleEvents.testEvent, [&]() { obs.callFunc(); });

	// Start, wait, stop
	UNSCOPED_INFO("Starting event send...");
	singleEvents.startSendingEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	singleEvents.stopSendingEvents();
	UNSCOPED_INFO("Stopped event sending");

	// While we're still receiving events, wait
	waitUntilAllEventsProcessed(obs, singleEvents, std::chrono::milliseconds(100));

	// Compare counts of send and received
	UNSCOPED_INFO(obelisk::formatString("Sent events %d, received events %d", singleEvents.getInvokeCount(), obs.getCallCount()).c_str());
	// Received events should be equal to sent events
	REQUIRE(singleEvents.getInvokeCount() == obs.getCallCount());
}

template <bool useSharedEventHandler> void MultipleArguments_TestImpl()
{
	// Check that multiple arguments compile (and will print something)
	SingleAsyncEventObject<useSharedEventHandler, 0, true, int, std::wstring> singleEvents(5, L"string");
	auto obs = createBasicObserver<ObsCallPrint::PRINT, int, std::wstring>();
	obs.registerToEvent(singleEvents.testEvent, [&](int i, std::wstring s) { obs.callFunc(i, s); });

	UNSCOPED_INFO("Starting event send...");
	singleEvents.startSendingEvents();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	UNSCOPED_INFO("Cleaning up...");
}

template <bool useSharedEventHandler> void MemFunc_Bind_TestImpl()
{
	class Internal : public obelisk::EventObserver
	{
	  public:
		void nonMod() const {};
		void mod(){};
		Internal() = delete;
		Internal(SingleAsyncEventObject<useSharedEventHandler, 0, false> &subject)
		{
			registerToEvent(subject.testEvent, &Internal::nonMod);
			registerToEvent(subject.testEvent, &Internal::mod);
		}
	};

	SingleAsyncEventObject<useSharedEventHandler, 0, false> singleEvents;
	Internal obj(singleEvents);
}

TEST_CASE("ThrottledEvent_EventFirst")
{
	ThrottledEvent_EventFirst_TestImpl<true>();
	ThrottledEvent_EventFirst_TestImpl<false>();
}

TEST_CASE("ThrottledEvent_ObserverFirst")
{
	ThrottledEvent_ObserverFirst_TestImpl<true>();
	ThrottledEvent_ObserverFirst_TestImpl<false>();
}

TEST_CASE("ProcessAllEvents_EventFirst")
{
	ProcessAllEvents_EventFirst_TestImpl<true>();
	ProcessAllEvents_EventFirst_TestImpl<false>();
}

TEST_CASE("ProcessAllEvents_ObserverFirst")
{
	ProcessAllEvents_ObserverFirst_TestImpl<true>();
	ProcessAllEvents_ObserverFirst_TestImpl<false>();
}

TEST_CASE("MultipleArguments")
{
	MultipleArguments_TestImpl<true>();
	MultipleArguments_TestImpl<false>();
}

// Static test - testing if we can register to non-const and const member functions
// If compiles, then passes
TEST_CASE("MemFunc_Bind")
{
	MemFunc_Bind_TestImpl<false>();
	MemFunc_Bind_TestImpl<true>();
}