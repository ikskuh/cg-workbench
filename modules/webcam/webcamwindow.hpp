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

#ifndef WEBCAM_HPP
#define WEBCAM_HPP

#include <window.hpp>
#include <webcam.h>

class WebcamWindow :
	public Window
{
	WINDOW_PREAMBLE
private:
	GLuint texture;
	std::unique_ptr<Webcam> camera;
	std::vector<uint8_t> buffer;

	bool continious;

	int size[2];

	Sink * takeImage;

	void Setup(int width, int height);

protected:
	void OnRender() override;
	void OnUpdate() override;
public:
	WebcamWindow();
};

#endif // WEBCAM_HPP
