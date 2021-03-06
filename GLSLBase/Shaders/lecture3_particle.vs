#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_RamdomValue;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Accel;

//bool bLoop = true; 숙제.. -> 시험이 나올 가능성이 있음

float g_PI = 3.14;
mat3 g_RotMat = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);
const vec3 g_Gravity = vec3(0, -0.5, 0);

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;

	if(t > 0)
	{
		vec3 newAccel = g_Gravity + a_Velocity;
		newPos.x = sin(a_RamdomValue * 2 * g_PI);
		newPos.y = cos(a_RamdomValue * 2 * g_PI);
		newPos.z = 0;
		newPos = a_Position + newPos;

		float temp = t/a_LifeTime;
		float fractional = fract(temp);
		t = fractional * a_LifeTime;
		tt = t * t;

		float period = a_Period;
		float amp = a_Amp;

		newPos = newPos + newAccel * t + 0.5 * u_Accel * tt;

		vec3 rotVec = normalize(newAccel * g_RotMat);
		newPos = newPos + 0.1 * amp * rotVec * sin(period * t * 2.0 * g_PI);
		newPos.z = 0;
		v_Color = a_Color * (1.0 - fractional);
	}
	else
	{
		newPos = vec3(-1000,-1000,-1000);
		v_Color = vec4(0, 0, 0, 0);
	}

	gl_Position = vec4(newPos, 1);
	
}
