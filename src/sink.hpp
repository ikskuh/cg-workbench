#pragma once

#include <GL/gl3w.h>
#include "slot.hpp"
#include "source.hpp"

class Source;

class Sink :
	public Slot
{
private:
	std::vector<Source const *> sources;
	int limit;
public:
	Sink(CgDataType type, std::string const & name, int maxConnections = 1);
	virtual ~Sink();

	void Clear();

	//! Adds a source to the sink.
	//! @remarks Special Case: when the limit is 1, it allows replacement of the sources
	bool AddSource(Source const * source);

	void RemoveSource(Source const * source);

	int GetSourceCount() const { return int(this->sources.size()); }

	Source const * GetSource(bool withDefault = true, int index = 0) const
	{
		if(index < int(this->sources.size()))
			return this->sources[index];
		else if(withDefault)
			return Source::GetDefault(this->GetType());
		else
			return nullptr;
	}

	template<CgDataType _Type>
	typename UniformType<_Type>::type const & GetObject(int index = 0) const
	{
		static typename UniformType<_Type>::type _default;
		auto * src = this->GetSource(true, index);
		if(src != nullptr)
			return *static_cast<typename UniformType<_Type>::type const *>(src->GetObject());
		else
			return _default;
	}

	bool HasSource(int index = 0) const { return (this->GetSource(index) != nullptr); }

	bool HasSourceConnected() const { return (this->sources.size() > 0); }
};
