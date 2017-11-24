#include "stdafx.h"
#include "CppUnitTest.h"
#include "ObserverPatternCallbackTests.h"
#include "FormatString.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ObeliskTest
{
	enum class ObsCallPrint { PRINT, NO_PRINT };

	template <ObsCallPrint print, typename...Args>
	ObserverWithSingleCallback<Args...> createBasicObserver()
	{
		return ObserverWithSingleCallback<Args...>([](Args...args) {
			if (print == ObsCallPrint::PRINT)
			{
				if (sizeof...(Args) > 0)
				{
					std::wstringstream ss;
					ss << L"In callback with args: ";
					printArgsToStream(ss, args...);
					Logger::WriteMessage(ss.str().c_str());
				}
				else
				{
					Logger::WriteMessage("In callback (no args)");
				}
			}
		});
	}

	TEST_CLASS(ObserverPatternCallbackTest)
	{
	private:
		template <class ObsT, class EventT>
		void waitUntilAllEventsProcessed(const ObsT &o, const EventT &e, std::chrono::milliseconds maxWaitTimePerEvent)
		{
			auto lastReceivedCount = o.getCallCount();
			std::this_thread::sleep_for(maxWaitTimePerEvent);
			while (lastReceivedCount < o.getCallCount())
			{
				Logger::WriteMessage(obelisk::formatString("Sent events %d, received events %d", e.getInvokeCount(), o.getCallCount()).c_str());
				lastReceivedCount = o.getCallCount();
				std::this_thread::sleep_for(maxWaitTimePerEvent);
			}
		}

	public:   

		TEST_METHOD(ThrottledEvent_EventFirst)
		{
			SingleAsyncEventObject<0, false> singleEvents;
			ObserverWithSingleCallback<> obs = { []() {} };
			obs.registerToEvent(singleEvents.testEvent, [&]()
			{
				obs.callFunc();
				// Delay here so events get sent in the meantime (and ignored)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			});

			// Start, wait, stop
			Logger::WriteMessage("Starting event send...");
			singleEvents.startSendingEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			singleEvents.stopSendingEvents();
			Logger::WriteMessage("Stopped event sending");

			// Some time to let things process
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			// Get counts of send and received
			auto nSent = singleEvents.getInvokeCount();
			auto nReceived = obs.getCallCount();

			Logger::WriteMessage(obelisk::formatString("Invoked events %d, received events %d", nSent, nReceived).c_str());
			Assert::IsTrue(nSent > nReceived, L"Received events should be less than sent events");
		}

		TEST_METHOD(ThrottledEvent_ObserverFirst)
		{
			// Check that the event can be destroyed before the observer
			auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
			SingleAsyncEventObject<5, false> singleEvents;
			obs.registerToEvent(singleEvents.testEvent, [&]()
			{
				obs.callFunc();
				// Delay here so events get sent in the meantime (and ignored)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			});

			// Start, wait, (destruct)
			Logger::WriteMessage("Starting event send...");
			singleEvents.startSendingEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			Logger::WriteMessage("Cleaning up...");
		}

		TEST_METHOD(ProcessAllEvents_EventFirst)
		{
			SingleAsyncEventObject<0, true> singleEvents;
			auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
			obs.registerToEvent(singleEvents.testEvent, [&]()
			{
				obs.callFunc();
			});

			// Start, wait, stop
			Logger::WriteMessage("Starting event send...");
			singleEvents.startSendingEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			singleEvents.stopSendingEvents();
			Logger::WriteMessage("Stopped event sending");

			// While we're still receiving events, wait
			waitUntilAllEventsProcessed(obs, singleEvents, std::chrono::milliseconds(100));

			// Compare counts of send and received
			Logger::WriteMessage(obelisk::formatString("Sent events %d, received events %d", singleEvents.getInvokeCount(), obs.getCallCount()).c_str());
			Assert::IsTrue(singleEvents.getInvokeCount() == obs.getCallCount(), L"Received events should be equal to sent events");
		}

		TEST_METHOD(ProcessAllEvents_ObserverFirst)
		{
			auto obs = createBasicObserver<ObsCallPrint::NO_PRINT>();
			SingleAsyncEventObject<0, true> singleEvents;
			obs.registerToEvent(singleEvents.testEvent, [&]()
			{
				obs.callFunc();
			});

			// Start, wait, stop
			Logger::WriteMessage("Starting event send...");
			singleEvents.startSendingEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			singleEvents.stopSendingEvents();
			Logger::WriteMessage("Stopped event sending");

			// While we're still receiving events, wait
			waitUntilAllEventsProcessed(obs, singleEvents, std::chrono::milliseconds(100));

			// Compare counts of send and received
			Logger::WriteMessage(obelisk::formatString("Sent events %d, received events %d", singleEvents.getInvokeCount(), obs.getCallCount()).c_str());
			Assert::IsTrue(singleEvents.getInvokeCount() == obs.getCallCount(), L"Received events should be equal to sent events");
		}

		TEST_METHOD(MultipleArguments)
		{
			// Check that multiple arguments compile (and will print something)
			SingleAsyncEventObject<0, true, int, std::wstring> singleEvents(5, L"string");
			auto obs = createBasicObserver<ObsCallPrint::PRINT, int, std::wstring>();
			obs.registerToEvent(singleEvents.testEvent, [&](int i, std::wstring s)
			{
				obs.callFunc(i, s);
			});

			Logger::WriteMessage("Starting event send...");
			singleEvents.startSendingEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			Logger::WriteMessage("Cleaning up...");
		}

    // Static test - testing if we can register to non-const and const member functions
    // If compiles, then passes
    TEST_METHOD(MemFunc_Bind)
    {
      class Internal : public obelisk::EventObserver
      {
      public:
        void nonMod() const {};
        void mod() {};
        Internal() = delete;
        Internal(SingleAsyncEventObject<0, false> &subject)
        {
          registerToEvent(subject.testEvent, &Internal::nonMod);
          registerToEvent(subject.testEvent, &Internal::mod);
        }
      };

      SingleAsyncEventObject<0, false> singleEvents;
      Internal obj(singleEvents);
    }
	};
}