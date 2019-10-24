#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 ViewRay;

uniform sampler2D rColor;
uniform sampler2D ssaoColorBuffer;
uniform sampler2D ssaoColorBufferBlur;
uniform sampler2D gDepth;
uniform sampler2D rePosition;
uniform sampler2D gPosition;

uniform int showPass; //1, 2, 3 (Final color, ssao, ssao with blur)

uniform float near; 
uniform float far;
uniform mat4 projection;
uniform mat4 projectionInverse;

float GetViewZ(vec2 coords)
{
	float z = texture(gDepth, coords).r;
    //return (projection[3][2] / (2.f * z - 1.f - projection[2][2]));

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
	else if (showPass == 9)
	{
		vec3 viewPosition = texture(gPosition, TexCoords).rgb;
		float depth =  viewPosition.z;
		color = vec3(depth);		
	}
	else if (showPass == 4)
	{
		//Reconstruct 3D point by Inverse of projection
		float z = texture(gDepth, TexCoords).r*2.f -1.f;
		float x = TexCoords.x * 2.f - 1.f;
		float y = TexCoords.y * 2.f - 1.f;
		vec4 projectedPos = vec4(x, y ,z, 1.f);
		vec4 PositionVS = projectionInverse * projectedPos;
		color = vec3(PositionVS / PositionVS.w);

		//Reconstruct 3D point by View Ray projeciton
		float z_view = GetViewZ(TexCoords);//color.z;
		float x_view = z_view * ViewRay.x*-1.f;
		float y_view = z_view * ViewRay.y*-1.f;
		color = vec3(-1.f*projection[3][2], -1.f*projection[3][2], -1.f*projection[3][2]);
	}
	else if (showPass == 5)
	{
		color = texture(gPosition, TexCoords).rgb;		
	}

    FragColor = vec4(color, 1.0);
}
