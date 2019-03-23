#define GAME_QUAD_SIZE	1.5f
#define HALF_QUAD_SIZE	GAME_QUAD_SIZE / 2
#define MAX_SPHERE_RADIUS  1.5 * GAME_QUAD_SIZE
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)

#define DIV 1 / MAX_SPHERE_RADIUS

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
	float4	velocity;
	float4	color;
}			Particle;

typedef struct
{
	float4	position;
	float4	velocity;
	float4	color;
	float2  uv;
}			QuadParticle;

typedef struct
{
 	QuadParticle 	particle[6];
}					Quad;

typedef  struct
{
 	Quad 		quad[6];
}			Brick;

typedef float2 textCoord;

void kernel update_brick_block(global Brick * brick, float deltaTime)
{
	int		i = get_global_id(0);

	global Brick * brickParticle = brick + i;
	global QuadParticle * cornerParticle = &(brickParticle->quad[0].particle[0]);
	QuadParticle start = *cornerParticle;

	float dist = length(cornerParticle->position);
	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;

	cornerParticle->velocity =   (cornerParticle->position) /parDist_1;//centerPosition/parDist_1;//
	if(cornerParticle->position.y > GAME_QUAD_SIZE / 2) {
		cornerParticle->velocity.x *=  5 * parDist_0;
		cornerParticle->velocity.z *=  5 * parDist_0;
	} else {
		cornerParticle->velocity.x = 0;
		cornerParticle->velocity.z = 0;
		cornerParticle->velocity.y *= 4;
	}
	if (parDist_0 > 0.27) {
		cornerParticle->velocity.y = cornerParticle->velocity.y >= 0.0 ? cornerParticle->velocity.y + parDist_0 *  parDist_1  : -cornerParticle->velocity.y /5 + 0.5f * parDist_1;
	} else {
		cornerParticle->velocity.y = cornerParticle->velocity.y + parDist_0 *  parDist_1;
	}

	cornerParticle->color.x /= (1 +  0.02f * squareParDist_1);
	cornerParticle->color.y /= (1 +  0.04f * squareParDist_1);
	cornerParticle->color.z /= (1 +  0.04f * squareParDist_1);
	cornerParticle->color.w /= (1 +  0.01f * squareParDist_1);
	cornerParticle->position.xyz += cornerParticle->velocity.xyz * deltaTime;

	float4 deltaPos = cornerParticle->position  - start.position;
	float4 deltaColor = cornerParticle->color - start.color;

	for (int j = 1; j < 36; ++j) {
		global QuadParticle * particle= cornerParticle + j;
		particle->position += deltaPos;
		particle->color += deltaColor;
	}
}
