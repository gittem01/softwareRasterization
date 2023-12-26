#include "cglm/cglm.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


typedef struct Texture
{
	uint8_t* data;
	int width;
	int height;
} texture;

int numTris = 12;
vec4** triPositions;
vec2** triUVs;
char* textureIndices;
texture* textures;

void initData()
{
	textures = malloc(sizeof(texture) * 3);
	textures[0].data = stbi_load("../../../assets/smiley.png", &textures[0].width, &textures[0].height, NULL, 3);
	textures[1].data = stbi_load("../../../assets/cait.jpg", &textures[1].width, &textures[1].height, NULL, 3);
	textures[2].data = stbi_load("../../../assets/bard.jpg", &textures[2].width, &textures[2].height, NULL, 3);

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

	// back face
	glm_vec4_copy((vec4) { -1.0f, +1.0f, -1.0f, 1 }, triPositions[10][0]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[10][1]);
	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[10][2]);

	glm_vec4_copy((vec4) { -1.0f, +1.0f, +1.0f, 1 }, triPositions[11][0]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, -1.0f, 1 }, triPositions[11][1]);
	glm_vec4_copy((vec4) { +1.0f, +1.0f, +1.0f, 1 }, triPositions[11][2]);

	// -------------------------------------------------------------------------

	// top face
	glm_vec2_copy((vec2) { 0, 0.5f }, triUVs[0][0]);
	glm_vec2_copy((vec2) { 1, 0.5f }, triUVs[0][1]);
	glm_vec2_copy((vec2) { 0, 0.0f }, triUVs[0][2]);

	glm_vec2_copy((vec2) { 0, 0.0f }, triUVs[1][0]);
	glm_vec2_copy((vec2) { 1, 0.5f }, triUVs[1][1]);
	glm_vec2_copy((vec2) { 1, 0.0f }, triUVs[1][2]);

	// bottom face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[2][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[2][1]);
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[2][2]);

	glm_vec2_copy((vec2) { 0, 1 }, triUVs[3][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[3][1]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[3][2]);

	// right face
	glm_vec2_copy((vec2) { 0, 1 }, triUVs[4][0]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[4][1]);
	glm_vec2_copy((vec2) { 0, 2.0f / 3.0f }, triUVs[4][2]);

	glm_vec2_copy((vec2) { 0, 2.0f / 3.0f }, triUVs[5][0]);
	glm_vec2_copy((vec2) { 1, 1 }, triUVs[5][1]);
	glm_vec2_copy((vec2) { 1, 2.0f / 3.0f }, triUVs[5][2]);

	// left face
	glm_vec2_copy((vec2) { 0, 0 }, triUVs[6][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[6][1]);
	glm_vec2_copy((vec2) { 0, 1.0f / 3.0f }, triUVs[6][2]);

	glm_vec2_copy((vec2) { 0, 1.0f / 3.0f }, triUVs[7][0]);
	glm_vec2_copy((vec2) { 1, 0 }, triUVs[7][1]);
	glm_vec2_copy((vec2) { 1, 1.0f / 3.0f }, triUVs[7][2]);

	// front face
	glm_vec2_copy((vec2) { 0, 1.0f }, triUVs[8][0]);
	glm_vec2_copy((vec2) { 1, 1.0f }, triUVs[8][1]);
	glm_vec2_copy((vec2) { 0, 0.5f }, triUVs[8][2]);

	glm_vec2_copy((vec2) { 0, 0.5f }, triUVs[9][0]);
	glm_vec2_copy((vec2) { 1, 1.0f }, triUVs[9][1]);
	glm_vec2_copy((vec2) { 1, 0.5f }, triUVs[9][2]);

	// back face
	glm_vec2_copy((vec2) { 0, 1.0f / 3.0f }, triUVs[10][0]);
	glm_vec2_copy((vec2) { 0, 2.0f / 3.0f }, triUVs[10][1]);
	glm_vec2_copy((vec2) { 1, 1.0f / 3.0f }, triUVs[10][2]);

	glm_vec2_copy((vec2) { 1, 1.0f / 3.0f }, triUVs[11][0]);
	glm_vec2_copy((vec2) { 0, 2.0f / 3.0f }, triUVs[11][1]);
	glm_vec2_copy((vec2) { 1, 2.0f / 3.0f }, triUVs[11][2]);

	// -------------------------------------------------------------------------

	// texture indices
	textureIndices[0] = 2;
	textureIndices[1] = 2;
	textureIndices[2] = 0;
	textureIndices[3] = 0;
	textureIndices[4] = 1;
	textureIndices[5] = 1;
	textureIndices[6] = 1;
	textureIndices[7] = 1;
	textureIndices[8] = 2;
	textureIndices[9] = 2;
	textureIndices[10] = 1;
	textureIndices[11] = 1;
}