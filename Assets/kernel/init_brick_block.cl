#define GAME_QUAD_SIZE	1.0f
#define HALF_QUAD_SIZE	GAME_QUAD_SIZE / 2

#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)
#define WHITE (float4)(1.0f, 1.0f, 1.0f, 1.0f);
#define BRICK_COLOR (float4)(1.0f, 0.2f, 0.2f, 1.0f);

#define U_1  (float2)(0.0f, 0.0f)
#define U_2  (float2)(1.0f, 0.0f)
#define U_3  (float2)(0.0f, 1.0f)
#define U_4  (float2)(1.0f, 1.0f);

#define Y_COUNT 4
#define Z_COUNT 2
#define X_COUNT 6

#define ROW_COUNT X_COUNT *  Z_COUNT
#define HEIGHT (float)GAME_QUAD_SIZE / Y_COUNT
#define LENGTH (float)GAME_QUAD_SIZE / X_COUNT
#define WIDTH (float)GAME_QUAD_SIZE / Z_COUNT

typedef struct
{
	float4	position;
	float2  uv;
}			QuadParticle;

typedef struct
{
 	QuadParticle 	particle[6];
}					Quad;

typedef  struct
{
 	Quad 	quad[6];
}			Brick;


void initQuad(global Quad * quad, float4 position, int quadIndex)
{
	for (int i = 0; i < 6; ++i) {
		quad->particle[i].position = position;
		if (i == 0) {
			quad->particle[i].uv = U_1;
		} else if (i == 1 || i == 3) {
			quad->particle[i].uv = U_2;
		} else if (i == 2 || i == 5) {
			quad->particle[i].uv = U_3;
		} else if (i == 4) {
			quad->particle[i].uv = U_4;
		}
	}

	if (quadIndex == 0) {
		quad->particle[1].position.y += HEIGHT;
		quad->particle[3].position.y += HEIGHT;
		quad->particle[2].position.x += LENGTH;
		quad->particle[5].position.x += LENGTH;
		quad->particle[4].position.x += LENGTH;
		quad->particle[4].position.y += HEIGHT;
	} else if (quadIndex == 1) {
		for (int i = 0; i < 6; ++i) {
			quad->particle[i].position.y += HEIGHT;
		}
		quad->particle[1].position.z += WIDTH;
		quad->particle[3].position.z += WIDTH;
		quad->particle[2].position.x += LENGTH;
		quad->particle[5].position.x += LENGTH;
		quad->particle[4].position.x += LENGTH;
		quad->particle[4].position.z += WIDTH;
	} else if (quadIndex == 2) {
		for (int i = 0; i < 6; ++i) {
			quad->particle[i].position.z += WIDTH;
		}
		quad->particle[1].position.y += HEIGHT;
		quad->particle[3].position.y += HEIGHT;
		quad->particle[2].position.x += LENGTH;
		quad->particle[5].position.x += LENGTH;
		quad->particle[4].position.x += LENGTH;
		quad->particle[4].position.y += HEIGHT;
	} else if (quadIndex == 3) {
		quad->particle[1].position.z += WIDTH;
		quad->particle[3].position.z += WIDTH;
		quad->particle[2].position.x += LENGTH;
		quad->particle[5].position.x += LENGTH;
		quad->particle[4].position.x += LENGTH;
		quad->particle[4].position.z += WIDTH;
	} else if (quadIndex == 4) {
		quad->particle[1].position.y += HEIGHT;
		quad->particle[3].position.y += HEIGHT;
		quad->particle[2].position.z += WIDTH;
		quad->particle[5].position.z += WIDTH;
		quad->particle[4].position.z += WIDTH;
		quad->particle[4].position.y += HEIGHT;
	} else if (quadIndex == 5) {
		for (int i = 0; i < 6; ++i) {
			quad->particle[i].position.x += LENGTH;
		}
		quad->particle[1].position.y += HEIGHT;
		quad->particle[3].position.y += HEIGHT;
		quad->particle[2].position.z += WIDTH;
		quad->particle[5].position.z += WIDTH;
		quad->particle[4].position.z += WIDTH;
		quad->particle[4].position.y += HEIGHT;
	}
}

void kernel initialize_brick_block(global Brick * brick)
{
	int				i = get_global_id(0);
	global Brick * brickParticle = brick + i;

	float x_start = LENGTH * (i % X_COUNT) - HALF_QUAD_SIZE + LENGTH / 2;
	float y_start = HEIGHT * (i / (ROW_COUNT)) -  HALF_QUAD_SIZE  + HEIGHT / 2;
	float z_start = WIDTH * ((i % (ROW_COUNT))/ X_COUNT) - HALF_QUAD_SIZE  + WIDTH / 2;

	float4 position = (float4)(x_start, y_start, z_start, 0.0f);
	for (int j = 0; j < 6; ++j) {
		global Quad *quad = &(brickParticle->quad[j]);
		initQuad(quad, position, j);
	}
}
