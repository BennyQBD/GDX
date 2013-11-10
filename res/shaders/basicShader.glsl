struct VSOutput
{
	vec2 texCoord;
};

uniform mat4 transform;
uniform sampler2D diffuse;

shader VSmain(in vec3 position, in vec2 texCoord, out VSOutput VSout)
{
	gl_Position = transform * vec4(position, 1.0);
	VSout.texCoord = texCoord;
}

shader FSmain(in VSOutput FSin, out vec4 fragColor)
{
	fragColor = texture2D(diffuse, texCoord0.xy);
}

program BasicShader
{
	vs(120)=VSmain();
	fs(120)=FSmain();
};
