#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D rColor;
uniform sampler2D ssaoColorBuffer;
uniform sampler2D ssaoColorBufferBlur;
uniform sampler2D gDepth;
uniform sampler2D rePosition;

uniform int showPass; //1, 2, 3 (Final color, ssao, ssao with blur)

uniform float near; 
uniform float far;

float GetViewZ(vec2 coords)
{
	float z = texture(gDepth, coords).r;
    z = z * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

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

	else if (showPass == 0)
	{
		float depth = GetViewZ(TexCoords) / far;
		color = vec3(depth);		
	}
	else if (showPass == 4)
	{
		color = texture(rePosition, TexCoords).rgb;		
	}

    FragColor = vec4(color, 1.0);
}
