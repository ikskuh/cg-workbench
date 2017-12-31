#pragma once

#include <GL/gl3w.h>
#include "slot.hpp"
#include "source.hpp"

class Source;

class Sink :
	public Slot
{
private:
	Source const * source;
public:
	Sink(CgDataType type, std::string const & name);
	virtual ~Sink();

	void SetSource(Source const * source);

	Source const * GetSource(bool withDefault = true) const
	{
		if(this->source != nullptr)
			return this->source;
		else if(withDefault)
			return Source::GetDefault(this->GetType());
		else
			return nullptr;
	}

	template<CgDataType _Type>
	typename UniformType<_Type>::type const & GetObject() const
	{
		static typename UniformType<_Type>::type _default;
		auto * src = this->GetSource();
		if(src != nullptr)
			return *static_cast<typename UniformType<_Type>::type const *>(src->GetObject());
		else
			return _default;
	}

	bool HasSource() const { return (this->GetSource() != nullptr); }

	bool HasSourceConnected() const { return (this->source != nullptr); }
};
