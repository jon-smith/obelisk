#pragma once

#include <string>

namespace obelisk
{
class EventObserver;

class EventBase
{
  public:
	virtual void unregisterHandler(const EventObserver &observer) = 0;
	virtual void unregisterAllHandlers() = 0;
	std::wstring getName() const { return name; }

  protected:
	explicit EventBase(std::wstring name) : name(name){};
	~EventBase() {}

	const std::wstring name;
};
}