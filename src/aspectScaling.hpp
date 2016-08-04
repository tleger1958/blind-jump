//========================================================================//
// Copyright (C) 2016 Evan Bowman                                         //
//                                                                        //
// This program is free software: you can redistribute it and/or modify   //
// it under the terms of the GNU General Public License as published by   //
// the Free Software Foundation, either version 3 of the License, or      //
// (at your option) any later version.                                    //
//                                                                        //
// This program is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
// GNU General Public License for more details.                           //
//                                                                        //
// You should have received a copy of the GNU General Public License      //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.  //
//========================================================================//

#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

inline sf::Vector2f getDrawableRegionSize() {
	sf::Vector2f drawableRegionSize;
	float aspectRatio = static_cast<float>(sf::VideoMode::getDesktopMode().width) /
		static_cast<float>(sf::VideoMode::getDesktopMode().height);
	drawableRegionSize.x = 450.f;
	drawableRegionSize.y = 450.f;
	float windowAspect;
	// TODO: Binary search
	if (aspectRatio > 1.f) {
		do {
			drawableRegionSize.x += 0.025f;
			drawableRegionSize.y -= 0.025f;
			windowAspect = drawableRegionSize.x / drawableRegionSize.y;
		}
		while (std::fabs(aspectRatio - windowAspect) > 0.005f);
	} else {
		do {
			drawableRegionSize.x -= 0.025f;
			drawableRegionSize.y += 0.025f;
			windowAspect = drawableRegionSize.x / drawableRegionSize.y;
		}
		while (std::fabs(aspectRatio - windowAspect) > 0.005f);
	}
	return drawableRegionSize;
}
