#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float EnemyTime;

// output values to fragment shader

out vec3 color;
out vec3 position;


	float aleatoriu(vec3 co)
{
    return fract(sin(dot(co, vec3(12.9898, 78.233, 11))) * 43758.5453);
}
void main()
{

	position	= v_position;
	color		= v_color;

	float pozitie_random=aleatoriu(v_position);
	float timp=EnemyTime;
	timp=sin(timp);
	timp=abs(timp);

	vec3 whereTo;

	whereTo=v_position*(1+timp*pozitie_random*pozitie_random);

	gl_Position	= Projection * View * Model*vec4(whereTo,1.0);
}

