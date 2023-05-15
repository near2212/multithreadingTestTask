
#pragma once
#include <vector>
#include <atomic>
#include "Points.h"

namespace platform
{
	void drawPoint(float x, float y, float r, float g, float b, float a);
};


namespace test
{
	const float SCREEN_WIDTH = 1024;
	const float SCREEN_HEIGHT = 768;
	
	

	void render(void); // Only platform::drawPoint should be used
	void update(int dt); // dt in milliseconds
	void on_click(int x, int y); // x, y - in pixels



	void init(void);
	void term(void);

};
