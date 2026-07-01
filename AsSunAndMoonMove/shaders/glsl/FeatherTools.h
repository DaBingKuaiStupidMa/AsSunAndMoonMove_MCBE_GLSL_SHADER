#define getGray(color) (0.299*color.r+0.587*color.g+0.114*color.b)
#define getDayClear(fogFar) ((fogFar-0.69921875)*3.413333333)
#define useDayClear(a) (a*dayClear+(1.0-a))
#define getEnd1Hell2(fogColor, fogNear) (\
	fogColor.g<fogColor.r &&\
	fogColor.r<fogColor.b &&\
	fogColor.b<0.05078125 && fogNear>0.6 ?\
	1.0 :(fogNear>0.01 && fogNear<0.11 ?2.0 :0.0)\
)
#define DawnDusk(focus, SUN_DIR) (\
	clamp(focus *(SUN_DIR.y -(\
		SUN_DIR.w>0.5 ?\
		0.0 :5733.0/65536.0\
	)), 0.0, 1.0)\
)
#define maxOfRGB(color) (max(max(color.r, color.g), color.b))
#define sumOfXYZ(XYZ) (XYZ.x +XYZ.y +XYZ.z)
float getDiffuse(
    vec3 sunDir, vec3 normal, bool isDay, bool notWorld, float slowFade, float fastFade, float shadow
){//notWorld: Not in The Overworld.
    float diffuse=dot(sunDir, normal);
	diffuse=0.5 +0.5 *diffuse; // [-1.0, 1.0] to [0.0, 1.0]
	diffuse=((
        isDay || notWorld
	)?
		diffuse *slowFade: // Lyt fades at dawn/dusk.
		diffuse *0.3 *fastFade // moonlyt
	);
	diffuse *=notWorld ?1.0 :shadow; // 地底不应受日月影响
	float base =0.45 +shadow *(isDay ?0.1 :0.1*(1.0-slowFade));
	diffuse =mix(diffuse, 1.0, base); // 提亮
    return diffuse;
}

#define calcDawnDusk4_8_16 {\
	DawnDusk_4=DawnDusk(4.0,SUN_DIR);\
	DawnDusk_8=DawnDusk(8.0,SUN_DIR);\
	DawnDusk_16=DawnDusk(16.0,SUN_DIR);\
}