#ifndef HAPTICAPP_H
#define HAPTICAPP_H

class HapticApp {
public:
	HapticApp(int width=600, int height=600);
	virtual ~HapticApp();

	void run();

	virtual void tick();
	virtual void draw(bool haptic);
		
	static HapticApp* _happ;
protected:
	void initGlut(int width, int height);

	bool _running;
	int _windowWidth;
	int _windowHeight;

};

#endif /*SAMPLEHAPTICAPP_H*/