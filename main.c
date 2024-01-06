#include "SDL.h"
#include "dataInit.h"
#include <time.h>
#include <stdlib.h>

#undef main

typedef struct Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;

int width = 800;
int height = 800;
int persCorrect = 1;
int rotate = 1;

#define GET_PIXEL(x, y, aWidth, pixelSize, ptr) (((char*)ptr) + y * aWidth * pixelSize + x * pixelSize)

Pixel* frameBuffer;
float* depthBuffer;

mat2 triBarSolverMatrix;
mat4 viewMatrix;
mat4 modelMatrix;

void generateViewMatrix(vec3 camPos, vec3 lookPos, mat4* dstMatrix)
{
	vec3 forward;
	glm_vec3_sub(lookPos, camPos, forward);
	glm_normalize(forward);

	vec3 right;
	glm_vec3_cross(forward, (vec3){ 0, 0, 1 }, right);
	glm_normalize(right);

	vec3 up;
	glm_vec3_cross(right, forward, up);
	glm_normalize(up);

	glm_mat4_identity(*dstMatrix);
	(*dstMatrix)[0][0] = right[0];
	(*dstMatrix)[1][0] = right[1];
	(*dstMatrix)[2][0] = right[2];
	(*dstMatrix)[0][1] = up[0];
	(*dstMatrix)[1][1] = up[1];
	(*dstMatrix)[2][1] = up[2];
	(*dstMatrix)[0][2] = forward[0];
	(*dstMatrix)[1][2] = forward[1];
	(*dstMatrix)[2][2] = forward[2];
	(*dstMatrix)[3][0] = -glm_vec3_dot(right, camPos);
	(*dstMatrix)[3][1] = -glm_vec3_dot(up, camPos);
	(*dstMatrix)[3][2] = -glm_vec3_dot(forward, camPos);
}

// rotation arround Z axis
void generateRotationMatrix(float rot, mat4* dstMatrix)
{
	glm_mat4_identity(*dstMatrix);
	(*dstMatrix)[0][0] = cos(rot);
	(*dstMatrix)[0][1] = -sin(rot);
	(*dstMatrix)[1][0] = sin(rot);
	(*dstMatrix)[1][1] = cos(rot);
}

void drawLine(float x0, float x1, float y, vec4* vertices, vec2* uvs, char texIndex)
{
	if (x0 > x1)
	{
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
	}

	for (int i = (int)x0 + 1; i < (int)(x1 + 1); i++)
	{
		float l1 = triBarSolverMatrix[0][0] * (i - vertices[2][0]) + triBarSolverMatrix[0][1] * (y - vertices[2][1]);
		float l2 = triBarSolverMatrix[1][0] * (i - vertices[2][0]) + triBarSolverMatrix[1][1] * (y - vertices[2][1]);
		float l3 = 1 - l1 - l2;

		float interZ = l1 * vertices[0][2] + l2 * vertices[1][2] + l3 * vertices[2][2];
		float* depthPtr = (float*)GET_PIXEL(i, (int)y, width, sizeof(float), depthBuffer);
		if (interZ < *depthPtr)
		{
			float interS;
			float interT;

			if (!persCorrect)
			{
				interS = fmin(fmax(l1 * uvs[0][0] + l2 * uvs[1][0] + l3 * uvs[2][0], 0.0), 1.0);
				interT = fmin(fmax(l1 * uvs[0][1] + l2 * uvs[1][1] + l3 * uvs[2][1], 0.0), 1.0);
			}
			else
			{
				float G1S = l1 * uvs[0][0] / vertices[0][2] + l2 * uvs[1][0] / vertices[1][2] + l3 * uvs[2][0] / vertices[2][2];
				float G1T = l1 * uvs[0][1] / vertices[0][2] + l2 * uvs[1][1] / vertices[1][2] + l3 * uvs[2][1] / vertices[2][2];
				float G2 = l1 / vertices[0][2] + l2 / vertices[1][2] + l3 / vertices[2][2];
				
				interS = fmin(fmax(G1S / G2, 0.0f), 1.0f);
				interT = fmin(fmax(G1T / G2, 0.0f), 1.0f);
			}

			Pixel* color = (Pixel*)GET_PIXEL((int)(interS * (textures[texIndex].width - 1)), (int)(interT * (textures[texIndex].height - 1)),
				textures[texIndex].width, sizeof(Pixel), textures[texIndex].data);

			*depthPtr = interZ;
			Pixel* framePtr = (Pixel*)GET_PIXEL(i, (int)y, width, sizeof(Pixel), frameBuffer);
			*framePtr = *color;
		}
	}
}

void fillFlatBottomTriangle(vec4 v1, vec4 v2, vec4 v3, vec4* vertices, vec2* uvs, char texIndex)
{
	float invslp1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
	float invslp2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

	float curx1 = v1[0];
	float curx2 = v1[0];

	for (int scanlineY = (int)v1[1]; scanlineY < (int)v2[1] + 1; scanlineY++)
	{
		drawLine(curx1, curx2, scanlineY, vertices, uvs, texIndex);
		curx1 += invslp1;
		curx2 += invslp2;
	}
}

void fillFlatTopTriangle(vec4 v1, vec4 v2, vec4 v3, vec4* vertices, vec2* uvs, char texIndex)
{
	float invslp1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
	float invslp2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

	float curx1 = v3[0];
	float curx2 = v3[0];

	for (int scanlineY = (int)v3[1]; scanlineY > (int)v1[1] - 1; scanlineY--)
	{
		drawLine(curx1, curx2, scanlineY, vertices, uvs, texIndex);
		curx1 -= invslp1;
		curx2 -= invslp2;
	}
}

void sortVerticesY(vec4* vertices, vec2* uvs)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2 - i; j++)
		{
			if (vertices[j][1] > vertices[j + 1][1])
			{
				vec4 tmp;
				glm_vec4_copy(vertices[j], tmp);
				glm_vec4_copy(vertices[j + 1], vertices[j]);
				glm_vec4_copy(tmp, vertices[j + 1]);

				vec2 tmp2;
				glm_vec2_copy(uvs[j], tmp2);
				glm_vec2_copy(uvs[j + 1], uvs[j]);
				glm_vec2_copy(tmp2, uvs[j + 1]);
			}
		}
	}
}

void drawTriangle(int index)
{
	vec4 newVertices[3];
	vec2 newUVs[3];
	int texIndex = textureIndices[index];

	for (int i = 0; i < 3; i++)
	{
		glm_mat4_mulv(modelMatrix, triPositions[index][i], newVertices[i]);
		glm_mat4_mulv(viewMatrix, newVertices[i], newVertices[i]);
		newVertices[i][0] = (newVertices[i][0] / newVertices[i][2] + 0.5f) * width;
		newVertices[i][1] = (newVertices[i][1] / newVertices[i][2] + 0.5f) * height;

		glm_vec2_copy(triUVs[index][i], newUVs[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		newVertices[i][0] = (int)newVertices[i][0];
		newVertices[i][1] = (int)newVertices[i][1];
	}

	sortVerticesY(newVertices, newUVs);

	mat2 newMat =
	{
		{ newVertices[0][0] - newVertices[2][0], newVertices[1][0] - newVertices[2][0] },
		{ newVertices[0][1] - newVertices[2][1], newVertices[1][1] - newVertices[2][1] },
	};

	glm_mat2_inv(newMat, triBarSolverMatrix);

	if (newVertices[1][1] == newVertices[2][1])
	{
		fillFlatBottomTriangle(newVertices[0], newVertices[1], newVertices[2], newVertices, newUVs, texIndex);
	}
	else if (newVertices[0][1] == newVertices[1][1])
	{
		fillFlatTopTriangle(newVertices[0], newVertices[1], newVertices[2], newVertices, newUVs, texIndex);
	}
	else
	{
		vec4 v4 = {
			newVertices[0][0] + (newVertices[2][0] - newVertices[0][0]) / (newVertices[2][1] - newVertices[0][1]) * (newVertices[1][1] - newVertices[0][1]), newVertices[1][1]
		};

		fillFlatBottomTriangle(newVertices[0], newVertices[1], v4, newVertices, newUVs, texIndex);
		fillFlatTopTriangle(newVertices[1], v4, newVertices[2], newVertices, newUVs, texIndex);
	}
}

int main(int argc, char* argv[])
{
	initData();

	// enable vsync
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* framebufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
	depthBuffer = malloc(sizeof(float) * width * height);

	float camRot[2] = { 0.0f, 0.0f };
	int lastMouse[2] = { 0, 0 };
	int diff[2];

	int dt = 0;
	float rot = 0.0f;
	int running = 1;
	int click = 0;
    float dist = 5.0f;
	while (running)
	{
		int time = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_s)
				{
					persCorrect ^= 1;
				}
				else if (event.key.keysym.sym == SDLK_r)
				{
					rotate ^= 1;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					click = 1;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					click = 0;
				}
				break;
			case SDL_MOUSEMOTION:
				diff[0] = event.motion.x - lastMouse[0];
				diff[1] = event.motion.y - lastMouse[1];
				lastMouse[0] = event.motion.x;
				lastMouse[1] = event.motion.y;
				if (click)
				{
					camRot[0] += fmod(diff[1] * 0.01f, GLM_PI);
					camRot[1] += fmod(diff[0] * 0.01f, GLM_PI);

					if (camRot[0] > GLM_PI / 2.0f)
					{
						camRot[0] = GLM_PI / 2.0f - 0.0001f;
					}
					else if (camRot[0] < -GLM_PI / 2.0f)
					{
						camRot[0] = -GLM_PI / 2.0f + 0.0001f;
					}
				}
				break;
            case SDL_MOUSEWHEEL:
                dist -= event.wheel.y * 0.5f; 
				if (dist < 4.0f)
				{
					dist = 4.0f;
				}
                else if (dist > 20.0f)
                {
                    dist = 20.0f;
                }
                break;
			default:
				break;
			}
		}

		int pitch;
		SDL_LockTexture(framebufferTexture, NULL, &frameBuffer, &pitch);

		// clear
		memset(frameBuffer, 0, height * pitch);
		for (int i = 0; i < width * height; i++)
		{
			depthBuffer[i] = (float)1e20;
		}

		vec3 camPos = { 0.0f, 1.0f, 0.0f };
		glm_vec3_rotate(camPos, -camRot[0], (vec3){ 1, 0, 0 });
		glm_vec3_rotate(camPos, -camRot[1], (vec3){ 0, 0, 1 });

		glm_vec3_normalize(camPos);
		glm_vec3_mul(camPos, (vec3){dist, dist, dist}, camPos);

		generateViewMatrix(camPos, (vec3){ 0, 0, 0 }, &viewMatrix);
		generateRotationMatrix(rot, &modelMatrix);

		if (rotate)
		{
			rot += 0.001f * dt;
		}

		for (int i = 0; i < numTris; i++)
		{
			drawTriangle(i);
		}

		SDL_UnlockTexture(framebufferTexture);
		SDL_RenderCopy(renderer, framebufferTexture, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		dt = SDL_GetTicks() - time;
		//printf("Frame time: %d\n", dt);
	}

	return 0;
}