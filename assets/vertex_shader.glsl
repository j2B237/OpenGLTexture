#version 330 core
layout (location = 0) in vec3 aPos;			// Input vertex in the vertex shader


void main()
{
	gl_position = vec4(aPos.x, aPos.y, aPos.z, 1.0);		// 1.0 correspond to aPos.w which is called perspective division
}