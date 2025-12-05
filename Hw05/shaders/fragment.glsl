#version 330



uniform vec2 lightPosition;   // world-space player position
uniform sampler2D texture0;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

// Adjustable attenuation parameters
const float LINEAR_TERM    = 0.00004; // linear term
const float QUADRATIC_TERM = 0.00008; // quadratic term
const float MIN_BRIGHTNESS = 0.0;    // avoid total darkness

float attenuate(float distance, float linearTerm, float quadraticTerm)
{
    float attenuation = 1.0 / (1.0 + 
                               linearTerm * distance + 
                               quadraticTerm * distance * distance);

    return max(attenuation, MIN_BRIGHTNESS);
}

void main()
{
    float distance = distance(lightPosition, fragPosition);
    float brightness = attenuate(distance, LINEAR_TERM, QUADRATIC_TERM);
    vec4 color = texture(texture0, fragTexCoord);
    finalColor = vec4(color.rgb * brightness, color.a);
}