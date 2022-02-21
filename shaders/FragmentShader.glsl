#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int type_of_light;


// Output
layout(location = 0) out vec4 out_color;


void main()
{ 
       float lumina = 2;


    //vectorul normalei
    vec3 N=world_normal;

    //vectorul directiei luminii
    vec3 L = normalize(light_position - world_position);

    //componenta speculara
    vec3 R = normalize (reflect(-L,N));

    //vectorul directiei privirii observatorului
	vec3 V = normalize (eye_position - world_position);

    //vectorul median
	vec3 H = normalize (L + V);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light =material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(world_normal,L), 0);
    float specular_light = 0;


    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
         specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.


      

	if (type_of_light == 1)
	{   
        float pi=22/7;
        float cut_off = 30* (pi/180) ;
        float spot_light = dot(-L, light_direction);
        
		
		if (spot_light > cos(cut_off))
		{	 
            float linear_att = (spot_light - cos(cut_off)) / (1.0f - cos(cut_off));
            float light_att_factor = pow(linear_att, 2);
			lumina = (specular_light+diffuse_light) * light_att_factor + ambient_light;
		} 

	} else
	{  
            float aux1=distance(light_position, world_position);
            float factorAtenuare= aux1 * 0.14 + aux1 * aux1 * 0.07+1;
            factorAtenuare=1/factorAtenuare;
	        lumina=ambient_light + factorAtenuare * (specular_light + diffuse_light);
            
	}


    // TODO(student): Write pixel out color
    vec3 culoare = object_color* lumina;


    out_color = vec4(culoare,1);
    
}
