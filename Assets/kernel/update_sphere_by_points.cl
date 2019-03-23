#define GAME_QUAD_SIZE	1.5f
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

void kernel update_sphere(global Particle * particles, float deltaTime) {
	int				i = get_global_id(0);

	global Particle *	particle = particles + i;

	if (particle->position.x == 10000.0f)
		return;
	float dist = length(particle->position);

	if (dist > MAX_SPHERE_RADIUS) {
		particle->position = INF;
		particles->velocity = NULITY;
		return;
	}

	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;
	particle->velocity = particle->position /parDist_1;
	particle->velocity.x *= parDist_0;
	particle->velocity.z *= parDist_0;
	particle->velocity.y = particle->velocity.y >= 0.0 ? particle->velocity.y + parDist_0 *  parDist_1  : -particle->velocity.y /10 + 1.5f * parDist_1;
	particle->color.x /= (1 + 0.4f * squareParDist_1);
	particle->color.y /= (1 + 1.5f * squareParDist_1);
	particle->color.w /= (1 +  0.4f * squareParDist_1);
	particle->position += particle->velocity * deltaTime ;
};
