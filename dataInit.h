#include "cglm/cglm.h"

int numTris = 12;
vec4** triPositions;
vec2** triUVs;
char* textureIndices;

void initData()
{
	triPositions = malloc(sizeof(vec4*) * numTris);
	triUVs = malloc(sizeof(vec2*) * numTris);
	textureIndices = malloc(sizeof(char) * numTris);

	for (int i = 0; i < numTris; i++)
	{
		triPositions[i] = malloc(sizeof(vec4) * 3);
		triUVs[i] = malloc(sizeof(vec2) * 3);
	}

	// top face
	glm_vec4_copy((vec4) { -1.0f, -1.0f, +1.0f, 1 }, triPositions[0][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, +1.0f, 1 }, triPositions[0][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[0][2]);

	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[1][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, +1.0f, 1 }, triPositions[1][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, +1.0f, 1 }, triPositions[1][2]);

	// bottom face
	glm_vec4_copy((vec4) { -1.0f, -1.0f, -1.0f, 1 }, triPositions[2][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, -1.0f, 1 }, triPositions[2][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[2][2]);

	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[3][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, -1.0f, 1 }, triPositions[3][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[3][2]);

	// right face
	glm_vec4_copy((vec4) { +1.0f, -1.0f, -1.0f, 1 }, triPositions[4][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, +1.0f, 1 }, triPositions[4][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[4][2]);

	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[5][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, +1.0f, 1 }, triPositions[5][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, +1.0f, 1 }, triPositions[5][2]);
	
	// left face
	glm_vec4_copy((vec4) { -1.0f, -1.0f, -1.0f, 1 }, triPositions[6][0]);
	glm_vec4_copy((vec4) { -1.0f, -1.0f, +1.0f, 1 }, triPositions[6][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[6][2]);

	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[7][0]);
	glm_vec4_copy((vec4) { -1.0f, -1.0f, +1.0f, 1 }, triPositions[7][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[7][2]);

	// front face
	glm_vec4_copy((vec4) { -1.0f, -1.0f, -1.0f, 1 }, triPositions[8][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, -1.0f, 1 }, triPositions[8][1]);
	glm_vec4_copy((vec4) { -1.0f, -1.0f, +1.0f, 1 }, triPositions[8][2]);

	glm_vec4_copy((vec4) { -1.0f, -1.0f, +1.0f, 1 }, triPositions[9][0]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, -1.0f, 1 }, triPositions[9][1]);
	glm_vec4_copy((vec4) { +1.0f, -1.0f, +1.0f, 1 }, triPositions[9][2]);

	// bottom face
	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[10][0]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[10][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[10][2]);

	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[11][0]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[11][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, +1.0f, 1 }, triPositions[11][2]);


	// -------------------------------------------------------------------------

	// top face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[0][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[0][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[0][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[1][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[1][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[1][2]);

	// bottom face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[2][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[2][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[2][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[3][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[3][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[3][2]);

	// right face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[4][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[4][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[4][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[5][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[5][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[5][2]);

	// left face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[6][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[6][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[6][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[7][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[7][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[7][2]);

	// front face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[8][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[8][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[8][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[9][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[9][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[9][2]);

	// bottom face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[10][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[10][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[10][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[11][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[11][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[11][2]);

	// -------------------------------------------------------------------------
	textureIndices[0] = 0;
	textureIndices[1] = 0;
	textureIndices[2] = 0;
	textureIndices[3] = 0;
	textureIndices[4] = 0;
	textureIndices[5] = 0;
	textureIndices[6] = 0;
	textureIndices[7] = 0;
	textureIndices[8] = 0;
	textureIndices[9] = 0;
	textureIndices[10] = 0;
	textureIndices[11] = 0;
}