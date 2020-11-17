#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__

struct DirectionalLight
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 direction;
};

struct PointLight
{
	float3 color;
	float3 position;
};

// - You don’t necessarily have to keep all the comments; they’re here for your reference
// The fresnel value for non-metals (dielectrics)
// Page 9: "F0 of nonmetals is now a constant 0.04"
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
static const float F0_NON_METAL = 0.04f;
// Minimum roughness for when spec distribution function denominator goes to zero
static const float MIN_ROUGHNESS = 0.0000001f; // 6 zeros after decimal
// Handy to have this as a constant
static const float PI = 3.14159265359f;

// =============| PBR Methods

// Lambert diffuse BRDF - Same as the basic lighting diffuse calculation!
// - NOTE: this function assumes the vectors are already NORMALIZED!
float DiffusePBR(float3 normal, float3 dirToLight)
{
	return saturate(dot(normal, dirToLight));
}

// Calculates diffuse amount based on energy conservation
//
// diffuse - Diffuse amount
// specular - Specular color (including light color)
// metalness - surface metalness amount
//
// Metals should have an albedo of (0,0,0)...mostly
// See slide 65: http://blog.selfshadow.com/publications/s2014-shading-course/hoffman/s2014_pbs_physics_math_slides.pdf
float3 DiffuseEnergyConserve(float diffuse, float3 specular, float metalness)
{
	return diffuse * ((1 - saturate(specular)) * (1 - metalness));
}

// GGX (Trowbridge-Reitz)
//
// a - Roughness
// h - Half vector
// n - Normal
// 
// D(h, n) = a^2 / pi * ((n dot h)^2 * (a^2 - 1) + 1)^2
float SpecDistribution(float3 n, float3 h, float roughness)
{
	// Pre-calculations
	float NdotH = saturate(dot(n, h));
	float NdotH2 = NdotH * NdotH;
	float a = roughness * roughness;
	float a2 = max(a * a, MIN_ROUGHNESS); // Applied after remap!
	// ((n dot h)^2 * (a^2 - 1) + 1)
	float denomToSquare = NdotH2 * (a2 - 1) + 1;
	// Can go to zero if roughness is 0 and NdotH is 1
	// Final value
	return a2 / (PI * denomToSquare * denomToSquare);
}

// Fresnel term - Schlick approx.
// 
// v - View vector
// h - Half vector
// f0 - Value when l = n (full specular color)
//
// F(v,h,f0) = f0 + (1-f0)(1 - (v dot h))^5
float3 Fresnel(float3 v, float3 h, float3 f0)
{
	// Pre-calculations
	float VdotH = saturate(dot(v, h));
	// Final value
	return f0 + (1 - f0) * pow(1 - VdotH, 5);
}
// Geometric Shadowing - Schlick-GGX (based on Schlick-Beckmann)
// - k is remapped to a / 2, roughness remapped to (r+1)/2
//
// n - Normal
// v - View vector
//
// G(l,v,h)
float GeometricShadowing(float3 n, float3 v, float3 h, float roughness)
{
	// End result of remapping:
	float k = pow(roughness + 1, 2) / 8.0f;
	float NdotV = saturate(dot(n, v));
	// Final value
	return NdotV / (NdotV * (1 - k) + k);
}

// Microfacet BRDF (Specular)
//
// f(l,v) = D(h)F(v,h)G(l,v,h) / 4(n dot l)(n dot v)
// - part of the denominator are canceled out by numerator (see below)
//
// D() - Spec Dist - Trowbridge-Reitz (GGX)
// F() - Fresnel - Schlick approx
// G() - Geometric Shadowing - Schlick-GGX
float3 MicrofacetBRDF(float3 n, float3 l, float3 v, float roughness, float metalness, float3 specColor)
{
	// Other vectors
	float3 h = normalize(v + l);
	// Grab various functions
	float D = SpecDistribution(n, h, roughness);
	float3 F = Fresnel(v, h, specColor);
	float G = GeometricShadowing(n, v, h, roughness) * GeometricShadowing(n, l, h, roughness);
	// Final formula
	// Denominator dot products partially canceled by G()!
	// See page 16: http://blog.selfshadow.com/publications/s2012-shading-course/hoffman/s2012_pbs_physics_math_notes.pdf
	return (D * F * G) / (4 * max(dot(n, v), dot(n, l)));
}

//==============| Helper Methods

// Calculates the specular reflection using the Phong lighting
// model, given a surface normal, a direction FROM the light,
// a direction TO the camera (from the surface) and a specular 
// exponent value (the shininess of the surface)
float SpecularPhong(float3 normal, float3 lightDir, float3 toCamera, float specExp)
{
	// Calculate light reflection vector
	float3 refl = reflect(normalize(lightDir), normal);

	// 1. Get the dot product between the perfect reflection of the light
	//    and the vector to the camera, as that tells us how "close" we 
	//    are to seeing the perfect reflection.  
	// 2. Saturate to ensure it doesn't go negative on the back of the object
	// 3. Raise to a power to create a very quick "falloff" (so the highlight
	//    appears smaller if the object is shinier)
	return pow(saturate(dot(refl, toCamera)), specExp);
}

float CalculateSpecularForLight(float3 _pixelWorldPosition, float3 _pixelNormal, float3 _lightPosition, float3 cameraPos, float roughness, float metalness, float3 specColor)
{
	_pixelNormal = normalize(_pixelNormal);
	float3 incomingLightDirection = normalize(_pixelWorldPosition - _lightPosition);
	float3 viewVec = normalize(cameraPos - _pixelWorldPosition);
	float3 reflectionVec = reflect(incomingLightDirection, _pixelNormal);
	//return pow(saturate(dot(reflectionVec, viewVec)), 64);

	//return SpecularPhong(_pixelNormal, incomingLightDirection, viewVec, 64);
	return MicrofacetBRDF(_pixelNormal, incomingLightDirection, viewVec, roughness, metalness, specColor);
}

float4 ComputeDirectionalLightColor(
	float3 _pixelPosition, 
	float3 _normal, 
	DirectionalLight _light, 
	float3 cameraPos, 
	float roughness, 
	float metalness, 
	float3 specColor, 
	float3 surfaceColor)
{
	float3 normalizedLight = normalize(_light.direction * -1.0);
	float lightingAmount = saturate(dot(normalizedLight, _normal));
	float3 newColor = lightingAmount * _light.diffuseColor;
	float3 otherNewColor = _light.ambientColor;
	return float4(newColor + otherNewColor, 1);
}

float4 ComputePointLightColor(float3 _pixelPosition, float3 _normal, PointLight _light, float3 cameraPos, float roughness, float metalness, float3 specColor, float3 surfaceColor)
{
	// for the record, I dont think point lights are working quite as they should - they seem to be treated as if they're in local space for the object theyre shining on
	float3 normalizedLight = normalize(_pixelPosition - _light.position );
	float lightingAmount = saturate(dot(normalizedLight, _normal));
	float3 newColor = lightingAmount * _light.color + CalculateSpecularForLight(_pixelPosition, _normal, _light.position, cameraPos, roughness, metalness, specColor);	
	
	float3 toCam = normalize(cameraPos - _pixelPosition);

	return float4(newColor, 1);

	// Chris' stuff

	// Calculate the light amounts
	float diff = DiffusePBR(_normal, normalizedLight);
	float3 spec = MicrofacetBRDF(_normal, normalizedLight, toCam, roughness, metalness, specColor);
	//float3 spec = CalculateSpecularForLight(_pixelPosition, _normal, _light.position, cameraPos, roughness, metalness, specColor);

	// Calculate diffuse with energy conservation
	// (Reflected light doesn't get diffused)
	float3 balancedDiff = DiffuseEnergyConserve(diff, spec, metalness);

	// Combine the final diffuse and specular values for this light
	float3 total = (balancedDiff * surfaceColor + spec) /* * _light.intensity */ *  _light.color;

	return float4(total, 1);
}

//==========| Pipeline structs

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;        // RGBA color
	float2 uv			: TEXCOORD;        // RGBA color
	float3 tangent		: TANGENT;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VertexToPixelSky
{
	float4 position : SV_POSITION;
	float3 sampleDir : DIRECTION;
};


#endif