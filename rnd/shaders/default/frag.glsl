#version 430 core

in vec3 FragPos;
out vec4 FragColor;

uniform float time;
uniform ivec2 ScreenResolution;

uniform vec3 cameraPos;
uniform vec3 cameraDir;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform vec3 bgColor;
uniform bool colorCorrection;
uniform float fog_coeff;

uniform uint shapes_num;


float tol = 1e-3;
float inf = 3e2;
uint MAX_STEPS = 1000;

const uint SPHERE = 1;
const uint PLANE = 2;
const uint BOX = 3;
const uint TORUS = 4;
const uint CONE = 5;
const uint CYLINDER = 6;
const uint ELLIPSOID = 7;

const uint ADD = 0;
const uint SUB = 1;
const uint INT = 2;

const uint MIN = 0;
const uint CUB = 1;
const uint EXP = 2;


struct descr
{
    vec3 center;
    float r;
    vec3 N;
    float D;
    vec3 sides;
	float R;
	vec3 cap;
	float padding;
};


struct shape
{
    vec4 color;
    uint type;
    uint min_type;
    float min_coef;
    uint mode;
    vec4 rotation;

    // uint padding;

    descr data;
};

layout(binding = 3) coherent restrict buffer mySSBO
{
    shape shapes[];
};

mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 rotate(vec3 v, vec3 axis, float angle) {
	mat4 m = rotationMatrix(axis, angle);
	return (m * vec4(v, 1.0)).xyz;
}


float ExpSMin(float a, float b, float k)
{
    if (k == 0) return min(a, b);

    k *= 1.0;
    float r = exp2(-a / k) + exp2(-b / k);
    return -k * log2(r);
}


float CubicSMin(float a, float b, float k)
{
    float h = 1.0 - min(abs(a - b) / (6.0 * k), 1.0);
    float w = h * h * h;

    return (a < b) ? a - w*k : b - w*k;
}


// General minimum function
float GenMin(float a, float b, uint min_type, float min_coef)
{
    // return min(a, b);
    if (min_type == MIN)
        return (a < b) ? a : b;
    if (min_type == EXP)
        return ExpSMin(a, b, min_coef);
    if (min_type == CUB)
        return CubicSMin(a, b, min_coef);
}


float sphere_sdf(shape S, vec3 p)
{
    return length(S.data.center - p) - S.data.r;
}


float plane_sdf(shape S, vec3 p)
{
    return dot(p, S.data.N) + S.data.D;
}


float box_sdf(shape S, vec3 p)
{
    vec3 pos = p - S.data.center;
    if (S.rotation.w != 0) pos = rotate(pos, S.rotation.xyz, S.rotation.w);
    vec3 q = abs(pos) - S.data.sides;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}


float torus_sdf(shape S, vec3 p)
{
    vec3 pos = p - S.data.center;
    if (S.rotation.w != 0) pos = rotate(pos, S.rotation.xyz, S.rotation.w);
    vec2 q = vec2(length(pos.xz) - S.data.r, pos.y);
    return length(q) - S.data.R;
}


float ellipsoid_sdf(shape S, vec3 p)
{
    vec3 pos = p - S.data.center;
    if (S.rotation.w != 0) pos = rotate(pos, S.rotation.xyz, S.rotation.w);
    float k0 = length(pos / S.data.sides);
    float k1 = length(pos / (S.data.sides * S.data.sides));
    return k0 * (k0 - 1.0) / k1;
}


float cylinder_sdf(shape S, vec3 p)
{
    vec3 pos = p - S.data.center;
    if (S.rotation.w != 0) pos = rotate(pos, S.rotation.xyz, S.rotation.w);
    return inf;
}


float cone_sdf(shape S, vec3 p)
{
    vec3 pos = p - S.data.center;
    if (S.rotation.w != 0) pos = rotate(pos, S.rotation.xyz, S.rotation.w);
    return inf;
}


// general sdf function
float GenSDF(shape S, vec3 p)
{
    float dist;

    switch (S.type)
    {
    case SPHERE:
        dist = sphere_sdf(S, p);
        break;
    case PLANE:
        dist = plane_sdf(S, p);
        break;
    case BOX:
        dist = box_sdf(S, p);
        break;
    case TORUS:
        dist = torus_sdf(S, p);
        break;
    case ELLIPSOID:
        dist = ellipsoid_sdf(S, p);
        break;
    case CONE:
        dist = cone_sdf(S, p);
        break;
    case CYLINDER:
        dist = cylinder_sdf(S, p);
        break;
    default:
        dist = inf;
        break;
    }
    return dist;
}


vec2 SDF(vec3 p)
{
    float mdist = inf;
    int obj = -1;
    for (int i = 0; i < shapes_num; i++)
    {
        shape s = shapes[i];
        float prev = mdist;
        float dist = GenSDF(s, p);

        if (s.mode == ADD)
            mdist = GenMin(mdist, dist, s.min_type, s.min_coef);
        else if (s.mode == SUB)
            mdist = -GenMin(-mdist, dist, s.min_type, s.min_coef);
        else
            mdist = -GenMin(-mdist, -dist, s.min_type, s.min_coef);

        if (abs(mdist - prev) > abs(mdist - dist))
            obj = i;
    }
    
    return vec2(mdist, obj);
}


vec3 color_mix(vec3 p)
{
    vec3 res = vec3(0);
    float div = 0.0;
    float mdist_zero = inf, mdist = inf;
    vec3 color_zero;

    for (int i = 0; i < shapes_num; i++)
    {
        shape s = shapes[i];
        float dist = GenSDF(s, p);

        if (s.color.w == 0.0 && dist < mdist_zero) {
            mdist_zero = dist;
            color_zero = s.color.xyz;
            continue;
        }

        if (dist < mdist) mdist = dist;

        float coef = exp(-dist * 6.0 / s.color.w * 0.16);
        res += s.color.xyz * coef;
        div += coef;
    }

    if (mdist_zero < mdist)
        return color_zero;

    div = max(div, 1.0);
    return (res / div);
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


vec3 reflVec(vec3 v, vec3 r)
{
    float k = dot(v, r);
    return (k > 0) ? v : v - 2.f*r*k;
}


vec3 shade(shape S, vec3 p, vec3 N)
{
    vec3 res = S.color.xyz;
    vec3 LightPos = normalize(vec3(0, 4, 10));
    vec3 LightColor = vec3(1);
    float diff = max(dot(N, LightPos), 0.0);
    vec3 diffuse = LightColor * diff;
    float amb = 0.1;
    vec3 ambient = amb * LightColor;
    if (S.color.w <= 0) return (diffuse + ambient) * S.color.xyz;
    res = (diffuse + ambient) * color_mix(p);
    return res;
}


void main()
{
    float w = ScreenResolution.y, h = ScreenResolution.x;
    vec2 pxlpos = vec2(FragPos.x, FragPos.y / h * w) * 1;
    int pxlX = int((FragPos.x + 1) * 0.5 * w);
    int pxlY = int((FragPos.y + 1) * 0.5 * h);
    float pxlSize = FragPos.x / w;
  
    vec3 ResColor = bgColor;
    vec3 start = cameraPos;
    vec3 dir = normalize(pxlpos.x * cameraRight + pxlpos.y * cameraUp + cameraDir * 1.0);
    int obj = -1;

    float rstep = (inf + tol) / 2.0, t = 0.0;
    vec3 pos = start;
    uint steps = 0;

    while (abs(rstep) > tol && t < inf && t >= 0 && steps < MAX_STEPS)
    {
        vec2 sdf_res = SDF(pos);
        obj = int(sdf_res.y);

        rstep = sdf_res.x;
        t += rstep;
        pos += dir * rstep;
        steps += 1;
    }

    shape s = shapes[obj];
    vec3 norm = Normal(pos);
    
    if (rstep <= tol)
    {
        ResColor = s.color.xyz;
        ResColor = norm;
        ResColor = shade(s, pos, norm);
        // float fog = 1.0 - exp(-t*0.0125);
        float fog = 1.0 - exp(-t*fog_coeff);
        if (t > 0) ResColor = mix(ResColor, bgColor, fog);
    }


    if (colorCorrection) {
        ResColor = pow(ResColor, vec3(0.4545));
        ResColor *= 0.9;
        ResColor = clamp(ResColor, 0, 1);
        ResColor = ResColor * ResColor * (3.0 - 2.0*ResColor);
    }

    FragColor = vec4(ResColor, 0.0);
}
