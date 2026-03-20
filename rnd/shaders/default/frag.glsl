#version 420 core

in vec3 FragPos;
out vec4 FragColor;

uniform float time;
uniform ivec2 ScreenResolution;


float tol = 1e-2;
float inf = 1e3;

const uint SPHERE = 1;
const uint PLANE = 2;


struct descr
{
    vec3 center;
    float r;
    vec3 N;
    float D;
};


struct shape
{
    vec3 color;
    uint type;
    descr data;
};

int shapes_num = 3;
shape shapes[3];


float ExpSMin(float a, float b, float k)
{
    k *= 1.0;
    float r = exp2(-a / k) + exp2(-b / k);
    return -k * log2(r);
}


float sphere_sdf(shape S, vec3 p)
{
    return length(S.data.center - p) - S.data.r;
}


float plane_sdf(shape S, vec3 p)
{
    return dot(p, S.data.N) + S.data.D;
}


vec2 SDF(vec3 p)
{
    float mdist = inf;
    int obj = -1;
    for (int i; i < shapes_num; i++)
    {
        shape s = shapes[i];
        float prev = mdist;
        float dist;

        switch (s.type)
        {
        case SPHERE:
            dist = sphere_sdf(s, p);
            break;
        case PLANE:
            dist = plane_sdf(s, p);
            break;
        default:
            dist = inf;
            break;
        }
        mdist = ExpSMin(mdist, dist, 0.16);
        if (abs(mdist - prev) > abs(mdist - dist))
            obj = i;
    }
    
    return vec2(mdist, obj);
}


vec3 color_mix(vec3 p)
{
    vec3 res = vec3(0);
    for (int i = 0; i < shapes_num; i++)
    {
        shape s = shapes[i];
        float dist;
        switch (s.type)
        {
        case SPHERE:
            dist = sphere_sdf(s, p);
            break;
        case PLANE:
            dist = plane_sdf(s, p);
            break;
        default:
            dist = inf;
            break;
        }
        res += s.color / (1.0 + dist * 10.0);
    }
    return res;
}


vec3 Normal(vec3 pos)
{
    float delta = 0.01;
    vec3 norm = vec3(0);
    vec3 pdist, ndist;
    vec3 mpdist = vec3(inf), mndist = vec3(inf);
    
    pdist.x = SDF(pos + vec3(delta, 0, 0)).x;
    ndist.x = SDF(pos - vec3(delta, 0, 0)).x;
    pdist.y = SDF(pos + vec3(0, delta, 0)).x;
    ndist.y = SDF(pos - vec3(0, delta, 0)).x;
    pdist.z = SDF(pos + vec3(0, 0, delta)).x;
    ndist.z = SDF(pos - vec3(0, 0, delta)).x;
    
    norm = normalize(pdist - ndist);
    return norm;
}


vec3 shade(shape S, vec3 p, vec3 N)
{
    vec3 res = S.color;
    vec3 LightPos = normalize(vec3(0, 4, -10));
    vec3 LightColor = vec3(1);
    float diff = max(dot(N, LightPos), 0.0);
    vec3 diffuse = LightColor * diff;
    float amb = 0.1;
    vec3 ambient = amb * LightColor;
    res = (diffuse + ambient) * color_mix(p);
    return res;
}


void main()
{
    float w = ScreenResolution.x, h = ScreenResolution.y;
    vec2 pxlpos = vec2(FragPos.x, FragPos.y / w * h) * 1;

    shape S1, S2, P1;
    S1.data.r = 1;
    S1.data.center = vec3(0, 0, 2);
    S1.color = vec3(0, 1, 0);
    S1.type = SPHERE;

    S2.data.r = 0.5;
    S2.data.center = vec3(sin(time * 1.5), 1 + sin(time * 2) * 0.5, 2 + cos(time));
    S2.color = vec3(0, 0, 1);
    S2.type = SPHERE;

    P1.data.N = vec3(0, 1, 1);
    P1.data.D = 1.5;
    P1.type = PLANE;
    P1.color = vec3(0.8, 0.1, 0.1);
  
    shapes[0] = S1;
    shapes[1] = S2;
    shapes[2] = P1;
    
    vec3 ResColor = vec3(0);
    vec3 start = vec3(pxlpos, -1);
    vec3 dir = normalize(vec3(pxlpos, 1));
    int obj = -1;

    float rstep = (inf + tol) / 2.0, t = 0.0;
    vec3 pos = start;

    while (rstep > tol && rstep < inf && t < inf)
    {
        float mdist = inf;

        for (int i = 0; i < shapes_num; i++)
        {
            shape s = shapes[i];
            float prev = mdist;
            float dist;
            
            switch (s.type)
            {
            case SPHERE:
                dist = sphere_sdf(s, pos);
                break;
            case PLANE:
                dist = plane_sdf(s, pos);
                break;
            default:
                dist = inf;
                break;
            }
            mdist = ExpSMin(mdist, dist, 0.16);
            mdist = min(mdist, dist);
            if (abs(mdist - prev) > abs(mdist - dist)) {
                obj = i;
            }
        }
        
        rstep = mdist;
        t += rstep;
        pos += dir * rstep;
    }
  
    shape s = shapes[obj];
    vec3 norm = Normal(pos);

    if (rstep <= tol)
    {
        ResColor = shapes[obj].color;
        ResColor = norm;
        ResColor = shade(s, pos, norm);
        // ResColor = color_mix(pos);
    }

    FragColor = vec4(ResColor, 1.0);
}
