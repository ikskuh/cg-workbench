#include "windowregistry.hpp"

static WindowClass * first = nullptr;

WindowClass::WindowClass(std::string id, std::string name, WindowCategory * category, std::function<Window*()> const & ctor) :
    ctor(ctor),
    id(id), name(name),
    category(category),
    next(first)
{
	::first = this;
	this->category->InitClasses().emplace_back(this);
}

WindowClass::~WindowClass()
{
	// Nah, fuck this shit, we're not gonna remove ourselves
}

WindowClass * WindowClass::First()
{
	return ::first;
}

WindowCategory::WindowCategory(WindowCategory *parent, std::string name) :
    name(name)
{
	if(parent)
		parent->InitChildren().emplace_back(this);
}

WindowCategory::~WindowCategory()
{

}

std::vector<WindowCategory const *> & WindowCategory::InitChildren() const
{
	if(this->children)
		return *this->children;
	else
		return *(this->children = new std::vector<WindowCategory const *>());
}

std::vector<WindowClass const *> & WindowCategory::InitClasses() const
{
	if(this->classes)
		return *this->classes;
	else
		return *(this->classes = new std::vector<WindowClass const *>());
}

namespace Menu
{
	WindowCategory Instance(nullptr, "~");
	namespace Values
	{
		WindowCategory Instance(&Menu::Instance, "Values");
	}
	namespace Arithmetic
	{
		WindowCategory Instance(&Menu::Instance, "Arithmetic");
	}
	namespace Transforms
	{
		WindowCategory Instance(&Menu::Instance, "Transforms");
	}
	namespace Buffer
	{
		WindowCategory Instance(&Menu::Instance, "Buffer");
	}
	namespace Tools
	{
		WindowCategory Instance(&Menu::Instance, "Tools");
	}
	namespace Noise
	{
		WindowCategory Instance(&Menu::Instance, "Noise");
	}
	namespace Audio
	{
		WindowCategory Instance(&Menu::Instance, "Audio");
	}
	namespace Event
	{
		WindowCategory Instance(&Menu::Instance, "Event");
	}
}
