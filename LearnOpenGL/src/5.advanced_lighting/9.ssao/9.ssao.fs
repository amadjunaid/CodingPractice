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
uniform bool useLogDepth;
uniform float near; 
uniform float far;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

uniform mat4 projection;
uniform mat4 projectionInverse;
//TODO: Calculate proper Sample Depth.. also from reconstruction as in http://ogldev.atspace.co.uk/www/tutorial46/tutorial46.html
float GetViewZ(vec2 coords)
{
	float z = texture(gDepth, coords).r;
    z = z * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * ( far - near));	
	//return (projection[3][2] / ( z - projection[2][2]));
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

void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).xyz;

	//Reconstruct Position
	float z_view = GetViewZ(TexCoords);
	float x_view = z_view * ViewRay.x;
	float y_view = z_view * ViewRay.y;

	//Use Reconstructed Position
	if(useLogDepth){			
		fragPos = ReconstructPosition(texture(gDepth, TexCoords).r);		
	}

    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sample = TBN * samples[i]; // from tangent to view-space
        sample = fragPos + sample * ssaoRadius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
		if(useLogDepth){	
			sampleDepth = ReconstructPosition(texture(gDepth, offset.xy).r).z;
			//sampleDepth = GetViewZ(offset.xy);
		}
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, ssaoRadius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
	RePosition = vec3(x_view, y_view, z_view);
}