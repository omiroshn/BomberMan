#define GAME_QUAD_SIZE	1.5f
#define SPHERE_RADIUS	GAME_QUAD_SIZE / 5
#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)
#define INF	(float4)(10000.0f, 10000.0f, 10000.0f, 1.0f)

typedef struct
{
	float4	position;
	float4	velocity;
	float4	color;
}			Particle;

void kernel initialize_sphere(global Particle * particles, int particleCount)
{
	int				i = get_global_id(0);
	global float4 *	position = &(particles[i].position);


	uint	subDivCount = cbrt((float)particleCount);
	float2	delta = (float2)(M_PI * 2 / subDivCount, M_PI / subDivCount);
	float	radiusDelta = (SPHERE_RADIUS )/ subDivCount;

	uint	x = fmod(i, (float)subDivCount);
	uint	y = fmod((float)i / subDivCount, (float)subDivCount);
	uint	r = i / (subDivCount * subDivCount);

	float radius = radiusDelta * r;

	float2	offset = (r % 2 == 0)
		? delta / 2
		: (float2)(0);

	position->x = radius * sin(delta.x * y + offset.x) * sin(delta.y * x + offset.y) / 1.f;
	position->y = radius * cos(delta.x * y + offset.x)/ 1.f;
	position->z = radius * sin(delta.x * y + offset.x) * cos(delta.y * x + offset.y)/ 1.f;
	position->w = 0.0f;

	particles[i].velocity = (float4)(0.0f, 0.0f, 0.0f, 0.0f);
	particles[i].color = (float4)(1.0f, 0.8f, 0.0f, 1.0f);
}
