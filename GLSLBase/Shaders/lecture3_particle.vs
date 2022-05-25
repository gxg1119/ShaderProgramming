#version 450

in vec3 a_Position;
in vec3 a_Velocity;

uniform float u_Time;
uniform vec3 u_Accel;

void main()
{
	vec3 newPos;
	float t = u_Time;
	float tt = u_Time * u_Time;

	newPos = a_Position + a_Velocity * t + 0.5 * u_Accel * tt;
	gl_Position = vec4(newPos, 1);
}
