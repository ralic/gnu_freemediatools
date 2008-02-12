
// fixed_function.vert - A fixed function emulator vertex shader to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

// Modified from the automatically generated GLSL ShaderGen from 3Dlabs.

vec4 Ambient;
vec4 Diffuse;
vec4 Specular;

void pointLight( in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3 )
{
	float nDotVP;       // normal . light direction
	float nDotHV;       // normal . light half vector
	float pf;           // power factor
	float attenuation;  // computed attenuation factor
	float d;            // distance from surface to light source
	vec3  VP;           // direction from surface to light position
	vec3  halfVector;   // direction of maximum highlights

	// Compute vector from surface to light position
	VP = vec3( gl_LightSource[ i ].position ) - ecPosition3;

	// Compute distance between surface and light position
	d = length( VP );

	// Normalize the vector from surface to light position
	VP = normalize( VP );

	// Compute attenuation
	attenuation = 1.0 / ( gl_LightSource[ i ].constantAttenuation + gl_LightSource[ i ].linearAttenuation * d + gl_LightSource[ i ].quadraticAttenuation * d * d );

	halfVector = normalize( VP + eye );

	nDotVP = max( 0.0, dot( normal, VP ) );
	nDotHV = max( 0.0, dot( normal, halfVector ) );

	if( nDotVP == 0.0 )
	{
		pf = 0.0;
	}
	else
	{
		pf = pow( nDotHV, gl_FrontMaterial.shininess );
	}

	Ambient  += gl_LightSource[ i ].ambient * attenuation;
	Diffuse  += gl_LightSource[ i ].diffuse * nDotVP * attenuation;
	Specular += gl_LightSource[ i ].specular * pf * attenuation;
}

void spotLight( in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3 )
{
	float nDotVP;            // normal . light direction
	float nDotHV;            // normal . light half vector
	float pf;                // power factor
	float spotDot;           // cosine of angle between spotlight
	float spotAttenuation;   // spotlight attenuation factor
	float attenuation;       // computed attenuation factor
	float d;                 // distance from surface to light source
	vec3  VP;                // direction from surface to light position
	vec3  halfVector;        // direction of maximum highlights

	// Compute vector from surface to light position
	VP = vec3( gl_LightSource[ i ].position ) - ecPosition3;

	// Compute distance between surface and light position
	d = length( VP );

	// Normalize the vector from surface to light position
	VP = normalize( VP );

	// Compute attenuation
	attenuation = 1.0 / ( gl_LightSource[ i ].constantAttenuation + gl_LightSource[ i ].linearAttenuation * d + gl_LightSource[ i ].quadraticAttenuation * d * d );

	// See if point on surface is inside cone of illumination
	spotDot = dot( -VP, normalize( gl_LightSource[ i ].spotDirection ) );

	if( spotDot < gl_LightSource[ i ].spotCosCutoff )
	{
		spotAttenuation = 0.0; // light adds no contribution
	}
	else
	{
		spotAttenuation = pow( spotDot, gl_LightSource[ i ].spotExponent );
	}

	// Combine the spotlight and distance attenuation.
	attenuation *= spotAttenuation;

	halfVector = normalize( VP + eye );

	nDotVP = max( 0.0, dot( normal, VP ) );
	nDotHV = max( 0.0, dot( normal, halfVector ) );

	if( nDotVP == 0.0 )
	{
		pf = 0.0;
	}
	else
	{	
		pf = pow( nDotHV, gl_FrontMaterial.shininess );
	}

	Ambient  += gl_LightSource[ i ].ambient * attenuation;
	Diffuse  += gl_LightSource[ i ].diffuse * nDotVP * attenuation;
	Specular += gl_LightSource[ i ].specular * pf * attenuation;
}

void directionalLight( in int i, in vec3 normal )
{
	float nDotVP; // normal . light direction
	float nDotHV; // normal . light half vector
	float pf;     // power factor

	nDotVP = max( 0.0, dot( normal, normalize( vec3( gl_LightSource[ i ].position ) ) ) );
	nDotHV = max( 0.0, dot( normal, vec3( gl_LightSource[ i ].halfVector ) ) );

	if( nDotVP == 0.0 )
	{
		pf = 0.0;
	}
	else
	{
		pf = pow( nDotHV, gl_FrontMaterial.shininess );
	}
 
	Ambient  += gl_LightSource[ i ].ambient;
	Diffuse  += gl_LightSource[ i ].diffuse * nDotVP;
	Specular += gl_LightSource[ i ].specular * pf;
}

vec3 fnormal( void )
{
	//Compute the normal 
	vec3 normal = gl_NormalMatrix * gl_Normal;
	normal = normalize( normal );
	return normal;
}

void flight( in vec3 normal, in vec4 ecPosition, float alphaFade )
{
	vec4 color;
	vec3 ecPosition3;
	vec3 eye;

	ecPosition3 = ( vec3( ecPosition ) ) / ecPosition.w;
	eye = vec3( 0.0, 0.0, 1.0 );

    // Clear the light intensity accumulators
	Ambient  = vec4( 0.0 );
	Diffuse  = vec4( 0.0 );
	Specular = vec4( 0.0 );

	pointLight( 0, normal, eye, ecPosition3 );

	color = gl_FrontLightModelProduct.sceneColor + Ambient  * gl_FrontMaterial.ambient + Diffuse  * gl_FrontMaterial.diffuse;
	color += Specular * gl_FrontMaterial.specular;
	color = clamp( color, 0.0, 1.0 );
	gl_FrontColor = color;
 
	gl_FrontColor.a *= alphaFade;
}

void main( void )
{
	vec3  transformedNormal;
	float alphaFade = 1.0;

	// Eye-coordinate position of vertex, needed in various calculations
	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;

	// Do fixed functionality vertex transform
	gl_Position = ftransform( );
	transformedNormal = fnormal( );
	flight( transformedNormal, ecPosition, alphaFade );
	
	gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
}
