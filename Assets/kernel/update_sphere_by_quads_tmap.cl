#define GAME_QUAD_SIZE	1.0f
#define LIVE_TIME	2.0f
#define TEXTURE_COUNT 16
#define TIME_FOR_TEXTURE	LIVE_TIME / TEXTURE_COUNT
#define MAX_SPHERE_RADIUS GAME_QUAD_SIZE
#define INF	(float4)(10000.0f, 10000.0f, 10000.0f, 1.0f)
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)

#define DIV 1 / MAX_SPHERE_RADIUS


#define T_MAP_STEP_X	(float)0.25
#define T_MAP_STEP_Y	(float)0.25
#define T_MAP_TO_2		(float2)(T_MAP_STEP_X, 0)
#define T_MAP_TO_3		(float2)(0, T_MAP_STEP_Y)
#define T_MAP_TO_4		(float2)(T_MAP_STEP_X, T_MAP_STEP_Y)

typedef struct
{
	float4	position;
	float2  uv;
}			QuadParticle;

typedef float2 textCoord;

void kernel update_sphere(global QuadParticle * particles, float deltaTime) {

	int		i = get_global_id(0);

	uint 	quadPoint = i % 6;
	if (quadPoint)
		return;

	global QuadParticle *	particle_0 = particles + i;
	QuadParticle particle_0_start = *particle_0;

	if (particle_0->position.x == 10000.0)
		return;

	float dist = length(particle_0->position);
	if (dist > MAX_SPHERE_RADIUS) {
		for (int j = 0; j < 6; ++j) {
			global QuadParticle * particle= particles + i + j;
			particle->position = INF;
		}
		return;
	}

	int texture_index =  floor(particle_0->position.w / TIME_FOR_TEXTURE);
	textCoord leftBottom = (float2)(0.0f + T_MAP_STEP_X * (texture_index % 4), 0.0f + T_MAP_STEP_Y * (texture_index / 4));
	textCoord U1 = leftBottom;
	textCoord U2 = leftBottom + T_MAP_TO_2;
	textCoord U3 = leftBottom + T_MAP_TO_3;
	textCoord U4 = leftBottom + T_MAP_TO_4;

	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;

	float4 velocity = particle_0->position /parDist_1;
	velocity.x *= parDist_0;
	velocity.z *= parDist_0;

	if (parDist_0 > 0.27) {
		velocity.y = velocity.y >= 0.0 ? velocity.y + parDist_0 *  parDist_1  : -velocity.y /5 + 0.5f * parDist_1;
	} else {
		velocity.y = velocity.y + parDist_0 *  parDist_1;
	}


	particle_0->position.xyz += velocity.xyz * deltaTime;
	particle_0->position.w += deltaTime;
	particle_0->uv = U1;

	float4 deltaPos = particle_0->position  - particle_0_start.position;

	for (int j = 1; j < 6; ++j) {
		global QuadParticle * particle= particles + i + j;
		particle->position += deltaPos;
		if (j == 1 || j == 3){
			particle->uv = U2;
		} else if (j == 2 || j == 5) {
			particle->uv = U3;
		} else {
			particle->uv = U4;
		}
	}
};
