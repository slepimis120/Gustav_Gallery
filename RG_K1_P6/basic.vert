#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex; //Koordinate texture, propustamo ih u FS kao boje 
uniform bool mirrorEnabled;
uniform bool flipEnabled;
out vec2 chTex;

void main()
{
	if (mirrorEnabled) {
        chTex = vec2(1.0 - inTex.x, inTex.y);
    } 
	else if(flipEnabled){
		chTex = vec2(inTex.x, 1.0 - inTex.y);
	}
	else{
        chTex = inTex;
    }
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
}