#define GAME_QUAD_SIZE	1.5f
#define MAX_SPHERE_RADIUS GAME_QUAD_SIZE
#define INF	(float4)(10000.0f, 10000.0f, 10000.0f, 2.0f)
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)

#define DIV 1 / MAX_SPHERE_RADIUS

typedef struct
{
	float4	position;
}			Particle;

void kernel update_sphere(global Particle * particles, float deltaTime) {
	int				i = get_global_id(0);

	global Particle *	particle = particles + i;

	if (particle->position.x == 10000.0f)
		return;
	float dist = length(particle->position);

	if (dist > MAX_SPHERE_RADIUS) {
		particle->position = INF;
		return;
	}

	float parDist_0 =  (MAX_SPHERE_RADIUS - dist) * DIV;
	float parDist_1 =  dist * DIV;
	float squareParDist_1 = parDist_1 * parDist_1;
	float4	velocity = particle->position /parDist_1;

	velocity.x *= parDist_0;
	velocity.z *= parDist_0;
	velocity.y =velocity.y >= 0.0 ? velocity.y + parDist_0 *  parDist_1  : -velocity.y /10 + 1.5f * parDist_1;

	particle->position.xyz += velocity.xyz * deltaTime ;
	particle->position.w += deltaTime;
};
