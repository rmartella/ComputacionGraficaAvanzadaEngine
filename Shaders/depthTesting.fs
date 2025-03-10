#version 330 core
out vec4 FragColor;

/***uniform float near = 0.1; 
uniform float far = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}***/

void main()
{             
    /***float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far to get depth in range [0,1] for visualization purposes
    FragColor = vec4(vec3(depth), 1.0);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);***/
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}