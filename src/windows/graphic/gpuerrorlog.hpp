#pragma once

#include "window.hpp"

class GpuErrorLog :
	public Window
{
	WINDOW_PREAMBLE
protected:
	void OnUpdate() override;
public:
	GpuErrorLog();
	GpuErrorLog(GpuErrorLog const &) = delete;
	GpuErrorLog(GpuErrorLog &&) = delete;
	virtual ~GpuErrorLog();

	static void APIENTRY LogMessage(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
};
