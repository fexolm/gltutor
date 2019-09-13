#version 330

out vec4 outputColor;

uniform float fragLoopDuration;
uniform float loopTime;

smooth in vec4 theColor;

const vec4 secondColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
void main()
{
   float currTime = mod(loopTime, fragLoopDuration);
   float currLerp = currTime / fragLoopDuration;

   outputColor = mix(theColor, secondColor, currLerp);
}
