#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_RamdomValue;

uniform float u_Time;
uniform vec3 u_Accel;

//bool bLoop = true; ����.. -> ������ ���� ���ɼ��� ����

float g_PI = 3.14;
mat3 g_RotMat = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;

	if(t > 0)
	{
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

		newPos = newPos + a_Velocity * t + 0.5 * u_Accel * tt;

		vec3 rotVec = normalize(a_Velocity * g_RotMat);
		newPos = newPos + amp * rotVec * sin(period * t * 2.0 * g_PI);
		newPos.z = 0;
	}
	else
	{
		newPos = vec3(-1000,-1000,-1000);
	}

	gl_Position = vec4(newPos, 1);
}
