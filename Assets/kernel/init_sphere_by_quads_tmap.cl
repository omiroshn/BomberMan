#define GAME_QUAD_SIZE	0.5f
#define SPHERE_RADIUS	GAME_QUAD_SIZE / 3
#define MIN_SPHERE_RADIUS SPHERE_RADIUS * 0.65f
#define QUAD_SIZE	 SPHERE_RADIUS * 2

#define T_MAP_STEP_X	(float)0.25
#define T_MAP_STEP_Y	(float)0.25
#define T_MAP_TO_2		(float2)(T_MAP_STEP_X, 0)
#define T_MAP_TO_3		(float2)(0, T_MAP_STEP_Y)
#define T_MAP_TO_4		(float2)(T_MAP_STEP_X, T_MAP_STEP_Y)

#define NULITY	(float4)(0.0f, 0.0f, 0.0f, 0.0f)
#define Z (float4)(0.0f, 0.0f, 1.0f, 0.0f)
#define Y (float4)(0.0f, 1.0f, 0.0f, 0.0f)
#define WHITE (float4)(1.0f, 1.0f, 1.0f, 1.0f);

typedef float2 textCoord;

typedef struct
{
	float4	position;
	float2  uv;
}			QuadParticle;

void kernel initialize_sphere(global QuadParticle * particles, int particleCount)
{
	int				i = get_global_id(0);
	uint quadPoint = i % 6;
	if (quadPoint)
		return;

	global QuadParticle *	particle_0 = particles + i;
	global QuadParticle *	particle_1 = particles + i + 1;
	global QuadParticle *	particle_2 = particles + i + 2;
	global QuadParticle *	particle_3 = particles + i + 3;
	global QuadParticle *	particle_4 = particles + i + 4;
	global QuadParticle *	particle_5 = particles + i + 5;

	textCoord leftBottom = (float2)(0.0f, 0.0f);
	textCoord U1 = leftBottom;
	textCoord U2 = leftBottom + T_MAP_TO_2;
	textCoord U3 = leftBottom + T_MAP_TO_3;
	textCoord U4 = leftBottom + T_MAP_TO_4;

	global float4 *	position = &(particle_0->position);

	uint	subDivCount = cbrt((float)particleCount);
	float2	delta = (float2)(M_PI * 2 / subDivCount, M_PI / subDivCount);
	float	radiusDelta = SPHERE_RADIUS / subDivCount;

	uint	x = fmod(i, (float)subDivCount);
	uint	y = fmod((float)i / subDivCount, (float)subDivCount);
	uint	r = i / (subDivCount * subDivCount);

	float radius = radiusDelta * r;
	float2	offset = (r % 2 == 0)
		? delta / 2
		: (float2)(0);

	position->x = radius * sin(delta.x * y + offset.x) * sin(delta.y * x + offset.y) / 3.f;
	position->y = radius * cos(delta.x * y + offset.x)/ 3.f;
	position->z = radius * sin(delta.x * y + offset.x) * cos(delta.y * x + offset.y)/ 3.f;
	position->w = 0.0f;

	particle_0->uv = U1;

	float dist = length(*position);
	if (dist < MIN_SPHERE_RADIUS) {
		*position = normalize(*position) * MIN_SPHERE_RADIUS;
	}

	float4 tangent_1 = cross(*position, Z);
	float4 tangent_1_norm = length(tangent_1) > MIN_SPHERE_RADIUS ?  normalize(tangent_1) : Z;
	float4 tangent_2 = cross(*position, tangent_1);
	float4 tangent_2_norm = length(tangent_2) > MIN_SPHERE_RADIUS ?  normalize(tangent_2) : Y;

	particle_1->position = *position + tangent_1_norm * QUAD_SIZE;
	particle_1->uv = U2;

	particle_3->position = *position + tangent_1_norm * QUAD_SIZE;
	particle_3->uv = U2;

	particle_2->position = *position + tangent_2_norm * QUAD_SIZE;
	particle_2->uv = U3;

	particle_5->position = *position + tangent_2_norm * QUAD_SIZE;
	particle_5->uv = U3;

	particle_4->position = *position +  tangent_1_norm * QUAD_SIZE + tangent_2_norm * QUAD_SIZE;
	particle_4->uv = U4;
}
