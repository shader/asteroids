#include "model.h"
#include "timer.h"

class Object {
public:
	vec3 position, size;
	quat orientation;
	vector<Model> models;

	Object();
	~Object();
	void Initialize();
	void Draw(mat4 view_projection);
	void Update(Time time);
};