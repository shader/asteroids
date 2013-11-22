#include "object.h"
#include "scene.h"
#include "utils.h"

ScoreBoard::ScoreBoard(Scene *scene) : Object(scene) {
	score = 0;
}

void ScoreBoard::Draw(mat4 model, mat4 projection) {}
