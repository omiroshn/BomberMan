#define GAME_QUAD_SIZE	1.0f
#define MAX_SPHERE_RADIUS GAME_QUAD_SIZE
#define INF	(float4)(10000.0f, 10000.0f, 10000.0f, 1.0f)
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)

#define DIV 1 / MAX_SPHERE_RADIUS

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
			particles->velocity = NULITY;
		}
		return;
	}

	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;

	particle_0->velocity = particle_0->position /parDist_1;
	particle_0->velocity.x *= parDist_0;
	particle_0->velocity.z *= parDist_0;

	if (parDist_0 > 0.17) {
		particle_0->velocity.y = particle_0->velocity.y >= 0.0 ? particle_0->velocity.y + parDist_0 *  parDist_1  : -particle_0->velocity.y /10 + 0.5f * parDist_1;
	} else {
		particle_0->velocity.y = particle_0->velocity.y + parDist_0 *  parDist_1;
	}

	particle_0->color.x /= (1 +  0.02f * squareParDist_1);
	particle_0->color.y /= (1 +  0.05f * squareParDist_1);
	particle_0->color.w /= (1 +  0.1f * squareParDist_1);
	particle_0->position += particle_0->velocity * deltaTime * 0.5f;

	float4 deltaPos = particle_0->position  - particle_0_start.position;
	float4 deltaColor = particle_0->color - particle_0_start.color;

	for (int j = 1; j < 6; ++j) {
		global QuadParticle * particle= particles + i + j;
		particle->position += deltaPos;
		particle->color += deltaColor;
	}
};
