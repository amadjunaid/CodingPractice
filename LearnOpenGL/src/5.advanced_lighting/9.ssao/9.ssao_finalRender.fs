#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D rColor;
uniform sampler2D ssaoColorBuffer;
uniform sampler2D ssaoColorBufferBlur;

uniform int showPass; //1, 2, 3 (Final color, ssao, ssao with blur)

void main()
{             
    // retrieve data from gbuffer
	vec3 color = vec3(1.f);
	if (showPass == 1)
	{		
		color = texture(rColor, TexCoords).rgb;		
	}

	else if (showPass == 2)
	{
		float r_ssao = texture(ssaoColorBuffer, TexCoords).r;	
		color = vec3(r_ssao, r_ssao, r_ssao);		
	}

	else if (showPass == 3)
	{
		float r_ssaoBlur = texture(ssaoColorBufferBlur, TexCoords).r;	
		color = vec3(r_ssaoBlur, r_ssaoBlur, r_ssaoBlur);		
	}

    FragColor = vec4(color, 1.0);
}
