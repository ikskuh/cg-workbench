/*

   (c) 2018 Felix Queißner <felix@ib-queissner.de>

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or (at
 your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335  USA

 */

#include "webcamwindow.hpp"

#include <windowregistry.hpp>
REGISTER_WINDOW_CLASS(WebcamWindow, Menu::Graphic, "graphic-webcam",  "Webcam")

WebcamWindow::WebcamWindow() :
    Window("Webcam", ImGuiWindowFlags_AlwaysAutoResize),
    texture(0),
    continious(false)
{
	this->size[0] = 640;
	this->size[1] = 480;
	this->Setup(640, 480);

	this->AddSource<CgDataType::Texture2D>("Image", &this->texture);

	this->takeImage = this->AddSink<CgDataType::Event>("Trigger", Sink::UnlimitedConnections);
}

void WebcamWindow::Setup(int width, int height)
{
	this->camera.reset(new Webcam("/dev/video0", width, height));

	if(this->texture != 0)
		glDeleteTextures(1, &this->texture);
	glCreateTextures(GL_TEXTURE_2D, 1, &this->texture);
	glTextureStorage2D(
		this->texture,
		1,
		GL_RGB8,
		width,
		height);

	this->buffer.resize(width * height * 3);
}

void WebcamWindow::OnRender()
{
	if(!this->camera)
		return;

	if(this->continious || Event::Any(this->takeImage))
	{
		RGBImage frame;
		try {
			frame = this->camera->frame(100);
		} catch(std::runtime_error const & ex) {
			// Timeout
			return;
		}

		size_t stride = frame.width * 3;
		for(size_t y = 0; y < frame.height; y++)
		{
			memcpy(
				&this->buffer[stride * (frame.height - y - 1)],
				frame.data + stride * y,
				stride);
		}

		glTextureSubImage2D(
			this->texture,
			0,
			0, 0,
			frame.width, frame.height,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			this->buffer.data());
	}
}

void WebcamWindow::OnUpdate()
{
	if(this->camera)
	{
		ImGui::Text("Resolution: %dx%d", this->camera->width(), this->camera->height());
	}

	ImGui::Checkbox("Continuous", &this->continious);

	ImGui::Separator();

	ImGui::DragInt2("Resolution", this->size, 1, 0, INT_MAX);

	if(ImGui::Button("Setup"))
		this->Setup(this->size[0], this->size[1]);
}
