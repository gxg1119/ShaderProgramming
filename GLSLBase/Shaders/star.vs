#version 450

in vec3 a_Position;

uniform float u_Time;

void main()
{
	vec3 newPos = a_Position;

	if(abs(a_Position.x) == 0.25 && a_Position.y > 0)
	{
		newPos.x = a_Position.x * u_Time;
		newPos.y = a_Position.y * u_Time;
	}

	if(abs(a_Position.x) == 0.37 && a_Position.y > 0)
	{
		newPos.x = a_Position.x * u_Time;
		newPos.y = a_Position.y * u_Time;
	}

	if(abs(a_Position.x) == 0.37 && a_Position.y < 0)
	{
		newPos.x = a_Position.x * u_Time;
		newPos.y = a_Position.y * u_Time;
	}

	if(abs(a_Position.x) == 0 && a_Position.y < 0)
	{
		//newPos.x = a_Position.x * u_Time;
		newPos.y = a_Position.y * u_Time;
	}

	gl_Position = vec4(newPos, 1);
}
