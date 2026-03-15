
// --- fragment shader core shader
@block fs_block

layout(binding = 1) uniform fs_params{
    float min_dist;
    float max_dist;
    float focal_length;
    int frac_iterations; // maximum fractal iterstions 
    int max_march_steps;

    vec3 ambient_color;
    vec3 light_color;
    vec3 light_direction;
    vec3 spec_color;
    float ambient_strength;
    float spec_strength;
    float spec_power;
};
in vec2 pos;
in vec3 eye;
in vec3 up;
in vec3 right;
in vec3 fwd;
//in float i_time;
out vec4 frag_color;



// --- fragment shader core shader

float d_scene(vec3 p) {
    return de(p, frac_iterations);
}

// surface normal estimation
vec3 surface_normal(vec3 p, float dp, float t) {
    const vec2 d = vec2(min_dist * t, 0);
    float x = d_scene(p + d.xyy) - dp;
    float y = d_scene(p + d.yxy) - dp;
    float z = d_scene(p + d.yyx) - dp;
    return normalize(vec3(x, y, z));
}

vec3 get_light(vec3 p, vec3 obj_color, vec3 norm, vec3 ray_direction) {
    //vec3 spec_color = light_color; // vec3(0.05, 0.05, 0.05); // jiná barva? možná cool!
    //float spec_strength = 0.8f;
    //float ambient_strength = 0.4f;

    vec3 L = normalize(light_direction);//normalize(light_position - p);
    vec3 V = -(p - eye); // -(p - eye);
    vec3 R = reflect(light_direction, norm);
    
    vec3 ambient = ambient_strength * ambient_color;
    vec3 diffuse = clamp(dot(L, norm), 0.0, 1.0) * light_color;
    // vec3 specular = spec_color * pow(clamp(dot(R, V), 0.0, 1.0), 10.0);
    vec3 specular = spec_color * spec_strength * pow(clamp(dot(R, ray_direction), 0.0, 1.0), spec_power); // 10.0
    return (ambient + diffuse) * obj_color + specular;
}

vec3 refVector(in vec3 v, in vec3 n)
{
    return v;
    float k = dot(v, n);
    //return (k>0.0) ? v : -v;
    return (k > 0.0) ? v : v - 2.0 * n * k;
}

vec3 get_light2(vec3 p, vec3 obj_color, vec3 norm, vec3 rd) {
    // color
    //col = vec3(0.01);
    //col = mix(col, vec3(0.10, 0.20, 0.30), clamp(tra.y, 0.0, 1.0));
    //col = mix(col, vec3(0.02, 0.10, 0.30), clamp(tra.z * tra.z, 0.0, 1.0));
    //col = mix(col, vec3(0.30, 0.10, 0.02), clamp(pow(tra.w, 6.0), 0.0, 1.0));
    //col *= 0.5;
    vec3 col = vec3(0.3, 0.2, 0.1);

    vec3 light1 = vec3(0.9, 0.8, 0.8);
    vec3 light2 = vec3(0.2, 0.01, 0.01);

    // lighting terms
    //vec3 pos = ro + t * rd;
    vec3 pos = p;// ro + t * rd;
    //vec3 nor = calcNormal(pos, t, px);
    vec3 nor = norm;// calcNormal(pos, t, px);

    //nor = refVector(nor, -normalize(rd));

    vec3  hal = normalize(light1 - rd);
    vec3  ref = reflect(rd, nor);
    float occ = clamp(0.05 * log(1.0), 0.0, 1.0); // float occ = clamp(0.05 * log(tra.x), 0.0, 1.0);
    float fac = clamp(1.0 + dot(rd, nor), 0.0, 1.0);

    // sun
    //float sha1 = softshadow(pos + 0.001 * nor, light1, 32.0);
    float dif1 = clamp(dot(light1, nor), 0.0, 1.0);// * sha1;
    float spe1 = pow(clamp(dot(nor, hal), 0.0, 1.0), 32.0) * dif1 * (0.04 + 0.96 * pow(clamp(1.0 - dot(hal, light1), 0.0, 1.0), 5.0));
    // bounce
    float dif2 = clamp(0.5 + 0.5 * dot(light2, nor), 0.0, 1.0) * occ;
    // sky
    float dif3 = (0.7 + 0.3 * nor.y) * (0.2 + 0.8 * occ);

    vec3 lin = vec3(0.0);
    lin += 12.0 * vec3(1.50, 1.10, 0.70) * dif1;
    lin += 4.0 * vec3(0.25, 0.20, 0.15) * dif2;
    lin += 1.5 * vec3(0.10, 0.20, 0.30) * dif3;
    lin += 2.5 * vec3(0.35, 0.30, 0.25) * (0.05 + 0.95 * occ);
    lin += 4.0 * fac * occ;
    col *= lin;
    //col = pow(col, vec3(0.7, 0.9, 1.0));
    //col += spe1 * 15.0;

    // gamma
    col = pow(col, vec3(0.4545));

    // vignette
    //col *= 1.0 - 0.05 * length(sp);

    return col;
}

float linearize_depth(float d, float zNear, float zFar) {
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main() {
    vec3 ray_origin = eye;
    vec3 ray_direction = normalize(fwd * focal_length + (right * pos.x + up * pos.y));
    float ambient_strength = 0.2f;
    vec3 object_color = vec3(1.0);
    vec4 color = vec4(ambient_color, 1.0);
    float t = 0.0;
    for (int i = 0; i < max_march_steps; i++) {
        vec3 p = ray_origin + ray_direction * t;
        float d = d_scene(p);
        if (d < min_dist * t) {
            vec3 norm = surface_normal(p, d, t);
            float ambient_occlusion = 1.0f - float(i) / float(max_march_steps);
            float fog = 1.0 - length(eye - p) / max_dist;
            color.xyz = get_light(p, object_color, norm, ray_direction) * ambient_occlusion * fog + ambient_color * (1.0f - fog);


            //color.xyz = get_light2(p, object_color, norm, ray_direction);// * ambient_occlusion * fog + ambient_color * (1.0f - fog);
            break;
        }
        if (t > max_dist) break;
        t += d;
    }
    frag_color = color;
}
@end

// lepší lighting model:
// https://www.shadertoy.com/view/ltfSWn