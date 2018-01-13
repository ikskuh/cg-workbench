#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <imgui.h>
#include <GL/gl3w.h>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <json.hpp>
#include <SDL.h>

#include "source.hpp"
#include "sink.hpp"

class Window
{
	friend void load(std::string const & fileName);
	friend void save(std::string const & fileName);
	friend int main(int argc, char ** argv);
private:
	int id;
	bool isOpen;
	std::string title;
	ImGuiWindowFlags flags;
	bool wantsResize;

	ImVec2 pos, size;

	std::vector<std::unique_ptr<Source>> sources;
	std::vector<std::unique_ptr<Sink>> sinks;
	std::vector<std::unique_ptr<Event>> events;

	char titleEditBuffer[256];

	ImVec2 dupSpawnPos;
protected:
	Event * CreateEvent();

	Event * AddEventSource(std::string name);

	void AddSource(Source * source);
	void AddSink(Sink * sink);

	void RemoveSource(Source * source, bool free = true);
	void RemoveSink(Sink * sink, bool free = true);

	template<CgDataType _Type>
	Sink * AddSink(std::string name, int maxConnections = 1)
	{
		Sink * s = new Sink(_Type, name, maxConnections);
		this->AddSink(s);
		return s;
	}

	template<CgDataType _Type>
	Source * AddSource(std::string name, typename UniformType<_Type>::type const * data)
	{
		Source * s = new Source(_Type, name, data);
		this->AddSource(s);
		return s;
	}
protected:
	explicit Window(std::string const & title, ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar);
	virtual void OnUpdate() = 0;
	virtual void OnRender();
	virtual void OnSetup();
	virtual std::string GetTypeID() const = 0;
public:
	virtual ~Window();

	void Update();

	void Render();

	void Close();

	ImVec2 GetPosition() const { return this->pos; }

	ImVec2 GetSize() const { return this->size; }

    int GetSinkCount() const { return int(this->sinks.size()); }

    int GetSourceCount() const { return int(this->sources.size()); }

	Source * GetSource(int idx) { return this->sources[idx].get(); }

	Source const * GetSource(int idx) const { return this->sources[idx].get(); }

	Source * GetSource(std::string name);
	Source const * GetSource(std::string name) const;

	Sink * GetSink(int idx) { return this->sinks[idx].get(); }
	Sink const * GetSink(int idx) const { return this->sinks[idx].get(); }

	Sink * GetSink(std::string name);
	Sink const * GetSink(std::string name) const;

	int GetID() const { return this->id; }

	std::string GetTitle() const { return this->title; }

	virtual nlohmann::json Serialize() const;

	virtual void Deserialize(nlohmann::json const & value);

public:
	static void Register(Window * window);

	static void Unregister(Window * window);

	static void UpdateAll();

	//! Creates buttons for each window sink/source
	//! and draws interconnection lines
	static void UpdateNodes();

	static void RenderAll();

	static void DestroyAll();

	static void RenderAudio(void*  userdata, Uint8* stream, int len);

	static Window * CreateFromJSON(nlohmann::json const & window);

public:
	static std::vector<std::unique_ptr<Window>>::iterator Begin();
	static std::vector<std::unique_ptr<Window>>::iterator End();
};

#define WINDOW_PREAMBLE \
	protected: \
		virtual std::string GetTypeID() const;

#define WINDOW_CUSTOM_SERIALIZE \
	public: \
		nlohmann::json Serialize() const override; \
		void Deserialize(nlohmann::json const & value) override;

#define WINDOW_SERIALIZE_IMPL(_Type) \
	nlohmann::json _Type::Serialize() const

#define WINDOW_DESERIALIZE_IMPL(_Type) \
	void _Type::Deserialize(nlohmann::json const & data)

#endif // WINDOW_HPP
