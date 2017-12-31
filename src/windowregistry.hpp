#pragma once

#include <functional>
#include <vector>

class Window;

class WindowCategory;

class WindowClass
{
private:
	std::function<Window*()> ctor;
	std::string id, name;
	WindowCategory * category;
	WindowClass * next;
public:
	WindowClass(std::string id, std::string name, WindowCategory * category, std::function<Window*()> const & ctor);
	WindowClass(WindowClass const &) = delete;
	WindowClass(WindowClass &&) = delete;
	~WindowClass();

	std::string GetID() const { return this->id; }

	std::string GetName() const { return this->name; }

	Window * CreateInstance() const { return this->ctor(); }

	WindowClass * Next() const { return this->next; }

	static WindowClass * First();
};

class WindowCategory
{
	friend class WindowClass;
private:
	mutable std::vector<WindowCategory const *> * children;
	std::string name;
	mutable std::vector<WindowClass const *> * classes;
private:
	std::vector<WindowCategory const *> & InitChildren() const;
	std::vector<WindowClass const *> & InitClasses() const;
public:
	WindowCategory(WindowCategory *parent, std::string name);
	WindowCategory(WindowCategory const &) = delete;
	WindowCategory(WindowCategory &&) = delete;
	~WindowCategory();

	std::string GetName() const {
		return this->name;
	}

	std::vector<WindowCategory const *> const & GetChildren() const {
		return this->InitChildren();
	}

	std::vector<WindowClass const *> const & GetClasses() const {
		return this->InitClasses();
	}
};

namespace Menu
{
	extern WindowCategory Instance;
	namespace Values
	{
		extern WindowCategory Instance;
	}
	namespace Arithmetic
	{
		extern WindowCategory Instance;
	}
	namespace Transforms
	{
		extern WindowCategory Instance;
	}
	namespace Buffer
	{
		extern WindowCategory Instance;
	}
	namespace Tools
	{
		extern WindowCategory Instance;
	}
	namespace Noise
	{
		extern WindowCategory Instance;
	}
	namespace Audio
	{
		extern WindowCategory Instance;
	}
	namespace Event
	{
		extern WindowCategory Instance;
	}
}

#define REGISTER_WINDOW_CLASS(_Class, _Category, _ID, _DisplayName) \
	WindowClass const __##_Class##__registration(_ID, _DisplayName, &_Category::Instance, []() -> Window * { return new _Class(); }); \
	std::string _Class::GetTypeID() const { return _ID; }

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define REGISTER_GENERIC_WINDOW_CLASS(_Class, _Arg, _Category, _ID, _DisplayName) \
	WindowClass const MACRO_CONCAT(__##_Class##__registration_,__COUNTER__)(_ID, _DisplayName, &_Category::Instance, []() -> Window * { return new _Class _Arg(); }); \
	template<> std::string _Class _Arg :: GetTypeID() const { return _ID; }
