#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;
uniform vec3 u_Accel;

//bool bLoop = true; 숙제.. -> 시험이 나올 가능성이 있음

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;

	if(t > 0)
	{
		float temp = t/a_LifeTime;
		float fractional = fract(temp);
		t = fractional * a_LifeTime;
		tt = t * t;
		newPos = a_Position + a_Velocity * t + 0.5 * u_Accel * tt;
	}
	else
	{
		newPos = vec3(-1000,-1000,-1000);
	}

	gl_Position = vec4(newPos, 1);
}
