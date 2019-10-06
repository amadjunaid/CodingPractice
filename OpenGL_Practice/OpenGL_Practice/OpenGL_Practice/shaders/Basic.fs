#version 330 core
  
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;
 

void main()
{
    //FragColor = vec4(ourColor, 1.0);
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0)	;
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
