#define GAME_QUAD_SIZE	1.0f
#define MAX_SPHERE_RADIUS GAME_QUAD_SIZE
#define INF	(float4)(10000.0f, 10000.0f, 10000.0f, 1.0f)
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)

#define DIV 1 / MAX_SPHERE_RADIUS


typedef struct
{
	float4	position;
	float2  uv;
}			QuadParticle;

void kernel update_sphere(global QuadParticle * particles, float deltaTime) {
//return;
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

	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;

	float4 velocity = particle_0->position / parDist_1;
	velocity.x *= parDist_0;
	velocity.z *= parDist_0;

	if (parDist_0 > 0.17) {
		velocity.y = velocity.y >= 0.0 ? velocity.y + parDist_0 *  parDist_1  : -velocity.y /10 + 0.5f * parDist_1;
	} else {
		velocity.y = velocity.y + parDist_0 * parDist_1;
	}

	particle_0->position.xyz += velocity.xyz * deltaTime * 0.65f;
	particle_0->position.w += deltaTime;
	float4 deltaPos = particle_0->position  - particle_0_start.position;

	for (int j = 1; j < 6; ++j) {
		global QuadParticle * particle= particles + i + j;
		particle->position += deltaPos;
	}
};
