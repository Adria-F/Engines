#ifndef __PANELCINFIGURATION_H__
#define __PANELCINFIGURATION_H__

#include "Panel.h"
#include <vector>

class Module;
class Application;

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(const char* name);
	~PanelConfiguration();

	void Draw();

	void addFPS(float fps, float ms);
	void windowConfig();

public:

	std::vector<float> fps_log;
	std::vector<float> ms_log;

private:

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool full_desktop = false;
	
};

#endif // !__PANELCINFIGURATION_H__