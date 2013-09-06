#include "scene.h"

class Scene {
public:
	Scene();
	~Scene();
	void Initialize();
	void Update();
	void Draw();
	void Mouse(int button, int state, int x, int y);
	void Keyboard(unsigned char key, int x, int y);
	void Resize(int w, int h);
};