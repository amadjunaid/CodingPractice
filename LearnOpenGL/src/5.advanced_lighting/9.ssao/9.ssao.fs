#version 330 core
layout (location = 0) out float FragColor;
layout (location = 1) out vec3 RePosition;

in vec2 TexCoords;
in vec2 ViewRay;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform sampler2D gDepth;

uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
uniform float ssaoRadius;
uniform float ssao_DR_falloff;
uniform float ssao_DR_area;
uniform bool useDepthReconstruction;
uniform bool usePureDepth;
uniform bool useRadiusScaling;
uniform bool fallOffMulti;
uniform bool useNormalReconstruction;
uniform bool useGBufferDepth;
uniform float near; 
uniform float far;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

uniform mat4 projection;
uniform mat4 projectionInverse;


float ConvertToViewZ(float depth)
{	
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * ( far - near));		
}

float GetLogZ(vec2 coords)
{
	float z = texture(gDepth, coords).r;    
    return z;// (2.0 * near * far) / (far + near - z * ( far - near));		
}

vec3 ReconstructPosition(float z_view)
{		
	float z = z_view*2.f -1.f;
	float x = TexCoords.x * 2.f - 1.f;
	float y = TexCoords.y * 2.f - 1.f;
	vec4 projectedPos = vec4(x, y ,z, 1.f);
	vec4 PositionVS = projectionInverse * projectedPos;
	return vec3(PositionVS / PositionVS.w);
}


vec3 normal_from_depth(float depth, vec2 texcoords) {
  
	vec2 offset1 = vec2(0.0,0.001);
	vec2 offset2 = vec2(0.001,0.0);
  
	float depth1 = texture(gDepth, texcoords + offset1).r;
	float depth2 = texture(gDepth, texcoords + offset2).r;
  
	vec3 p1 = vec3(offset1, depth1 - depth);
	vec3 p2 = vec3(offset2, depth2 - depth);
  
	vec3 normal = cross(p1, p2);
	normal.z = -normal.z;
  
	return normalize(normal);
}


void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = vec3(0.f);
	float ssaoRadius_Depth = ssaoRadius;
	float depth_log = GetLogZ(TexCoords);
	vec3 normal = normalize(texture(gNormal, TexCoords).rgb);

	//Position reconstruction
	if(useGBufferDepth)
		fragPos = texture(gPosition, TexCoords).xyz;
	else if(useDepthReconstruction){
		
		fragPos = ReconstructPosition(texture(gDepth, TexCoords).r);		
	}	
	else if(usePureDepth){		

		fragPos = vec3(TexCoords, depth_log);

		if(useNormalReconstruction)
		{
			normal = normal_from_depth(depth_log, TexCoords);	
		}
	
		
		if (useRadiusScaling){
			ssaoRadius_Depth = fallOffMulti ? ssaoRadius * depth_log : ssaoRadius / depth_log;
			//ssaoRadius_Depth = ssaoRadius;	
		}
	}		

    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;		

	for(int i = 0; i < kernelSize; ++i)
    {
		if (useGBufferDepth || useDepthReconstruction){
			// get sample position
			vec3 sample = TBN * samples[i]; // from tangent to view-space
			sample = fragPos + sample * ssaoRadius_Depth; 
        
			// project sample position (to sample texture) (to get position on screen/texture)
			vec4 offset = vec4(sample, 1.0);
			offset = projection * offset; // from view to clip-space
			offset.xyz /= offset.w; // perspective divide
			offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
			// get sample depth
			float depth_sample = 0.f;
			if(useGBufferDepth){
				depth_sample = texture(gPosition, offset.xy).z; // get depth value of kernel sample
			}
			if(useDepthReconstruction){	
				depth_sample = ReconstructPosition(texture(gDepth, offset.xy).r).z;			
			}
			float rangeCheck = smoothstep(0.0, 1.0, ssaoRadius_Depth / abs(fragPos.z - depth_sample));
			occlusion += (depth_sample >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
		}
		else if(usePureDepth){
			//ssaoRadius_Depth -= 0.4f;
			vec3 ray = ssaoRadius_Depth * reflect(samples[i], randomVec);
			vec3 offset = fragPos + sign(dot(ray,normal)) * ray;
			float depth_sample = GetLogZ(offset.xy);		
			
			//float view_depth = ConvertToViewZ(depth_log);
			//float view_depth_sample = ConvertToViewZ(depth_sample);
			float falloff_d = ssao_DR_falloff;
			if (useRadiusScaling)
				float falloff_d = fallOffMulti?ssao_DR_falloff*depth_log:ssao_DR_falloff/depth_log;
			float area_d = ssao_DR_area;
			float difference = depth_log - depth_sample;
    
			occlusion += step(falloff_d, difference) * (1.0-smoothstep(falloff_d, area_d, difference));			
		}
		
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
	FragColor = occlusion;
	if (usePureDepth)
		FragColor = FragColor + 0.2;
    //FragColor = ssaoRadius_Depth;
	

	//RePosition = vec3(x_view, y_view, z_view);
}