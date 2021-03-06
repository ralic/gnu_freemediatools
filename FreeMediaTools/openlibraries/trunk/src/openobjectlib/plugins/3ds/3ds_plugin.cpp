
// 3DS - An 3DS plugin to sg.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <boost/filesystem/fstream.hpp>

#include <openpluginlib/pl/geometry.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/calculate_tangent_space.hpp>
#include <openobjectlib/sg/spatial.hpp>
#include <openobjectlib/sg/meshes.hpp>

#include <openobjectlib/plugins/3ds/3ds_plugin.hpp>

namespace fs		= boost::filesystem;
namespace opl		= olib::openpluginlib;
namespace sg		= olib::openobjectlib::sg;

namespace olib { namespace openobjectlib { namespace plugins { namespace DS {

namespace
{
	static const unsigned short MAIN3DS				= 0x4D4D;
	static const unsigned short EDIT3DS				= 0x3D3D;

	static const unsigned short MAT_ENTRY			= 0xAFFF;
	static const unsigned short MAT_NAME			= 0xA000;
	static const unsigned short MAT_AMBIENT			= 0xA010;
	static const unsigned short MAT_DIFFUSE			= 0xA020;
	static const unsigned short MAT_SPECULAR		= 0xA030;
	static const unsigned short MAT_SHININESS		= 0xA040;
	static const unsigned short MAT_SHIN2PCT		= 0xA041;
	static const unsigned short MAT_TRANSPARENCY	= 0xA050;
	static const unsigned short MAT_SHADING			= 0xA100;
	static const unsigned short MAT_TWO_SIDE		= 0xA081;
	static const unsigned short MAT_ADDITIVE		= 0xA083;
	static const unsigned short MAT_WIRE			= 0xA085;
	static const unsigned short MAT_FACEMAP			= 0xA088;
	static const unsigned short MAT_WIRESIZE		= 0xA087;
	static const unsigned short MAT_DECAL			= 0xA082;
	static const unsigned short MAT_TEXMAP			= 0xA200;
	static const unsigned short MAT_MAPNAME			= 0xA300;
	static const unsigned short MAT_MAP_TILING		= 0xA351;
	static const unsigned short MAT_MAP_USCALE		= 0xA354;
	static const unsigned short MAT_MAP_VSCALE		= 0xA356;
	static const unsigned short MAT_MAP_UOFFSET		= 0xA358;
	static const unsigned short MAT_MAP_VOFFSET		= 0xA35A;
	static const unsigned short MAT_MAP_ANG			= 0xA35C;
	static const unsigned short MAT_TEX2MAP			= 0xA33A;
	static const unsigned short MAT_OPACMAP			= 0xA210;
	static const unsigned short MAT_BUMPMAP			= 0xA230;
	static const unsigned short MAT_SPECMAP			= 0xA204;
	static const unsigned short MAT_SHINMAP			= 0xA33C;
	static const unsigned short MAT_REFLMAP			= 0xA220;
	static const unsigned short MAT_ACUBIC			= 0xA310;

	static const unsigned short COLOR_F				= 0x0010;
	static const unsigned short COLOR_24			= 0x0011;
	static const unsigned short LIN_COLOR_24		= 0x0012;
	static const unsigned short LIN_COLOR_F			= 0x0013;

	static const unsigned short EDIT_OBJECT         = 0x4000;
	static const unsigned short OBJ_TRIMESH         = 0x4100;
	static const unsigned short OBJ_LIGHT           = 0x4600;
	static const unsigned short OBJ_CAMERA          = 0x4700;

	static const unsigned short CAM_RANGES			= 0x4720;

	static const unsigned short LIT_OFF             = 0x4620;
	static const unsigned short LIT_SPOT            = 0x4610;
	static const unsigned short LIT_INRANGE         = 0x4659;
	static const unsigned short LIT_OUTRANGE        = 0x465A;

	static const unsigned short TRI_VERTEXLIST      = 0x4110;
	static const unsigned short TRI_VERTEXOPTIONS   = 0x4111;
	static const unsigned short TRI_FACELIST        = 0x4120;
	static const unsigned short TRI_MAT_GROUP       = 0x4130;
	static const unsigned short TRI_SMOOTH_GROUP    = 0x4150;
	static const unsigned short TRI_FACEMAPPING     = 0x4140;
	static const unsigned short TRI_MATRIX          = 0x4160;

	static const unsigned short SPOTLIGHT           = 0x4610;

	struct chunk
	{
		unsigned short	id;
		unsigned int	size;
		std::streamsize offset;
	};
	
	template<class T>
	T convert( const unsigned char* buf, bool swap )
	{
		return *( ( T* ) buf );
	}

	template<>
	unsigned short convert( const unsigned char* buf, bool swap )
	{
		unsigned short v = *( unsigned short* ) buf;
		
		if( swap )
		{
			unsigned short t1 = *buf++;
			unsigned short t2 = *buf++;

			v = ( t1 << 8 ) | t2;
		}
		
		return v;
	}

	template<>
	unsigned int convert( const unsigned char* buf, bool swap )
	{
		unsigned int v = *( unsigned int* ) buf;
		
		if( swap )
		{
			unsigned int t1 = *buf++;
			unsigned int t2 = *buf++;
			unsigned int t3 = *buf++;
			unsigned int t4 = *buf++;

			v = ( t1 << 24 ) | ( t2 << 16 ) | ( t3 << 8 ) | t4;
		}
		
		return v;
	}

	template<>
	float convert( const unsigned char* buf, bool swap )
	{
		float v = *( float* ) buf;
		
		if( swap )
		{
			unsigned int t1 = *buf++;
			unsigned int t2 = *buf++;
			unsigned int t3 = *buf++;
			unsigned int t4 = *buf++;

			v = ( t1 << 24 ) | ( t2 << 16 ) | ( t3 << 8 ) | t4;
		}
		
		return v;
	}

	opl::string convert_asciiz( const unsigned char* buf, int i )
	{
		opl::string s;
		
		while( buf[ i ] )
			s += buf[ i ], ++i;
		
		return s;
	}

	bool find_chunk( unsigned short id, const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize filesize, bool swap, chunk& c )
	{
		while( i < filesize )
		{
			c.id   = convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
			c.size = convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );

			if( c.id == id )
			{
				c.offset = i + off;
				return true;
			}

			i += c.size - 6;
		}
		
		return false;
	}

	bool Read_s( fs::ifstream& file, char* s, std::streamsize size, std::streamsize max )
	{
#if _MSC_VER >= 1400
		file._Read_s( s, size, max );
#else
		file.read( s, size );
#endif // _MSC_VER >= 1400
			
		return !file.fail( );
	}

	opl::color3f read_color( const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize /*siz*/, bool swap )
	{
		opl::color3f v;
		
		chunk curr;
		curr.id		= convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
		curr.size	= convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );
		
		switch( curr.id )
		{
			case COLOR_F:
			case LIN_COLOR_F:
				v[ 0 ] = convert<float>( &buf[ i + 0 ], swap );
				v[ 1 ] = convert<float>( &buf[ i + sizeof( float ) ], swap );
				v[ 2 ] = convert<float>( &buf[ i + 2 * sizeof( float ) ], swap );
				break;

			case COLOR_24:
			case LIN_COLOR_24:
				v[ 0 ] = convert<unsigned char>( &buf[ i + 0 ], swap ) / 255.0f;
				v[ 1 ] = convert<unsigned char>( &buf[ i + sizeof( unsigned char ) ], swap ) / 255.0f;
				v[ 2 ] = convert<unsigned char>( &buf[ i + 2 * sizeof( unsigned char ) ], swap ) / 255.0f;
				break;

			default:
				break;
		}

		return v;
	}

	sg::texture_ptr read_tex_map( const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize siz, bool swap, const fs::path& branch )
	{
		opl::string name;
		
		sg::texture_ptr tex( new sg::texture( ) );
		
		while( i < siz )
		{
			chunk curr;
			
			curr.id		= convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
			curr.size	= convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );
			
			if( curr.id == MAT_MAPNAME )
				name = convert_asciiz( buf, i );
			else if( curr.id == MAT_MAP_USCALE )
				;
			else if( curr.id == MAT_MAP_VSCALE )
				;
			else if( curr.id == MAT_MAP_UOFFSET )
				;
			else if( curr.id == MAT_MAP_VOFFSET )
				;
			else if( curr.id == MAT_MAP_ANG )
				;
				
			i += curr.size - 6;
		}
		
		if( !name.empty( ) )
		{
			opl::m_url* u = tex->pointer<opl::m_url>( L"url" );
			u->push_back( fs::path( name.c_str( ), fs::native ) );
			u->push_back( branch / fs::path( name.c_str( ), fs::native ) );
		}
		
		return tex;
	}

	sg::v_appearance_ptr read_material( const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize siz, bool swap, const fs::path& branch )
	{
		opl::color3f amb, dif, spec;
		sg::texture_ptr tex;

		sg::appearance_ptr a( new sg::appearance( ) );
		
		while( i < siz )
		{
			chunk curr;
			
			curr.id		= convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
			curr.size	= convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );
			
			if( curr.id == MAT_NAME )
				opl::string name = convert_asciiz( buf, i );
			else if( curr.id == MAT_AMBIENT )
				amb = read_color( &buf[ i ], 0, i, curr.size, swap );
			else if( curr.id == MAT_DIFFUSE )
				dif = read_color( &buf[ i ], 0, i, curr.size, swap );
			else if( curr.id == MAT_SPECULAR )
				spec = read_color( &buf[ i ], 0, i, curr.size, swap );
			else if( curr.id == MAT_TEXMAP )
				tex = read_tex_map( &buf[ i ], 0, i, curr.size, swap, branch );
			
			i += curr.size - 6;
		}

		sg::material_ptr m( new sg::material( ) );
		opl::v_color* d  = m->pointer<opl::v_color>( L"diffuseColor" );
		opl::v_color* s  = m->pointer<opl::v_color>( L"specularColor" );
		d->get( ) = dif;
		s->get( ) = spec;

		sg::v_material_ptr* mat = a->pointer<sg::v_material_ptr>( L"material" );
		mat->get( ) = m;
		
		if( tex )
		{
			sg::mv_texture_ptr* vtex = a->pointer<sg::mv_texture_ptr>( L"texture" );
			vtex->push_back( sg::v_texture_ptr( tex ) );
		}
		
		/*
./openlibraries/trunk/src/openobjectlib/plugins/3ds/3ds_plugin.cpp:307: 
error: no matching function for call to 'olib::openobjectlib::sg::v_appearance_ptr::v_appearance_ptr(
olib::openobjectlib::sg::appearance_ptr&)'

./openlibraries/trunk/src/openobjectlib/sg/appearance.hpp:108: note: candidates are: olib::openobjectlib::sg::v_appearance_ptr::v_appearance_ptr()
./openlibraries/trunk/src/openobjectlib/sg/appearance.hpp:108: note:                 olib::openobjectlib::sg::v_appearance_ptr::v_appearance_ptr(const olib::openobjectlib::sg::v_appearance_ptr&)
*/

		/*

./openlibraries/trunk/src/openobjectlib/plugins/3ds/3ds_plugin.cpp:315: 
error: invalid initialization of reference of type '
const olib::openobjectlib::sg::v_appearance_ptr&' from expression of type '
      olib::openobjectlib::sg::appearance_ptr'
*/

		olib::openobjectlib::sg::v_appearance_ptr toPass(a);

		//		const olib::openobjectlib::sg::v_appearance_ptr& toPass=a;

		return sg::v_appearance_ptr( toPass );
	}
	
	sg::v_mesh_ptr read_mesh( const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize siz, bool swap )
	{
		sg::mesh_ptr m( new sg::mesh( ) );
		m->prim_type( ) = sg::mesh::TRIANGLES;

		sg::v_mesh_ptr geo;
		geo.get( ) = m;
		
		sg::v_attribute_array_ptr*	coord = m->pointer<sg::v_attribute_array_ptr>( L"coord" );
		sg::mv_uv_set_ptr*			uv	  = m->pointer<sg::mv_uv_set_ptr>( L"uv" );
		
		opl::matrixf mat = opl::make_identity<float>( );
		
		while( i < siz )
		{
			chunk curr;

			curr.id		= convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
			curr.size	= convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );

			if( curr.id == TRI_VERTEXLIST )
			{
				int j = i;
				unsigned short count = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
				
				sg::attribute_array_ptr arr( new sg::attribute_array( ) );
				coord->get( ) = arr;

				opl::m_float* value	= coord->get( )->pointer<opl::m_float>( L"value" );
				opl::v_int32* comp	= coord->get( )->pointer<opl::v_int32>( L"components" );
				
				value->reserve( count );
				comp->get( ) = 3;
				
				for( size_t k = 0; k < count; ++k )
				{
					float v0 = convert<float>( &buf[ j ], swap ); j += sizeof( float );
					float v1 = convert<float>( &buf[ j ], swap ); j += sizeof( float );
					float v2 = convert<float>( &buf[ j ], swap ); j += sizeof( float );
					
					value->push_back( v0 );
					value->push_back( v1 );
					value->push_back( v2 );
				}
			}
			else if( curr.id == TRI_FACEMAPPING )
			{
				int j = i;
				unsigned short count = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
				
				sg::uv_set_ptr uv0( new sg::uv_set( ) );
				
				sg::attribute_array_ptr arr( new sg::attribute_array( ) );
				sg::v_attribute_array_ptr* uvv = uv0->pointer<sg::v_attribute_array_ptr>( L"uv" );
				uvv->get( ) = arr;
				
				opl::m_float* value	= uvv->get( )->pointer<opl::m_float>( L"value" );
				opl::v_int32* comp	= uvv->get( )->pointer<opl::v_int32>( L"components" );

				value->reserve( count );
				comp->get( ) = 2;
				
				for( size_t k = 0; k < count; ++k )
				{
					float v0 = convert<float>( &buf[ j ], swap ); j += sizeof( float );
					float v1 = convert<float>( &buf[ j ], swap ); j += sizeof( float );
					
					value->push_back( v0 );
					value->push_back( v1 );
				}
				
				uv->push_back( sg::v_uv_set_ptr( uv0 ) );
			}
			else if( curr.id == TRI_FACELIST )
			{
				int j = i;
				unsigned short count = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
				
				opl::m_int32* index = m->pointer<opl::m_int32>( L"index" );
				index->reserve( count );

				for( size_t k = 0; k < count; ++k )
				{
					unsigned short f0 = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
					unsigned short f1 = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
					unsigned short f2 = convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
					
					convert<unsigned short>( &buf[ j ], swap ); j += sizeof( unsigned short );
					
					index->push_back( f0 );
					index->push_back( f1 );
					index->push_back( f2 );
				}
			}
			else if( curr.id == TRI_MATRIX )
			{
				int j = i;
				
				mat.set( 0, 0, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 0, 1, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 0, 2, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				
				mat.set( 1, 0, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 1, 1, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 1, 2, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				
				mat.set( 2, 0, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 2, 1, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 2, 2, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				
				mat.set( 3, 0, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 3, 1, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
				mat.set( 3, 2, convert<float>( &buf[ j ], swap ) ); j += sizeof( float );
			}

			i += curr.size - 6;
		}

		return geo;
	}
	
	sg::v_mesh_ptr read_scene( const unsigned char* buf, std::streamsize i, std::streamsize off, std::streamsize siz, bool swap )
	{
		sg::v_mesh_ptr m;
		
		opl::string s = convert_asciiz( buf, i ); i += s.size( ) + 1;
		while( i < siz )
		{
			chunk curr;
			
			curr.id		= convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
			curr.size	= convert<unsigned int>(   &buf[ i ], swap ); i += sizeof( unsigned int );

			if( curr.id == OBJ_TRIMESH )
				m = read_mesh( &buf[ i ], 0, i, curr.size, swap );

			i += curr.size - 6;
		}

		return m;
	}

	sg::scene_ptr load_3ds( const fs::path& path )
	{
		sg::scene_ptr sc( new sg::scene( ) );

		union { int i; char c[ 4 ]; } big_endian_test;
		big_endian_test.i = 1;
		bool swap = big_endian_test.c[ 0 ] != 1;

		fs::ifstream file( path, std::ios::in | std::ios::binary );
		if( !file.is_open( ) )
			return sc;

		std::streamsize siz = file.rdbuf( )->pubseekoff( 0, std::ios::end );
		file.rdbuf( )->pubseekoff( 0, std::ios::beg );

		std::vector<unsigned char> buf( siz );
		if( !Read_s( file, reinterpret_cast<char*>( &buf[ 0 ] ), siz, siz ) )
			return sc;

		std::streamsize i = 0;
		unsigned int csize;
		unsigned short cid;

		// header chunk
		cid = convert<unsigned short>( &buf[ i ], swap ); i += sizeof( unsigned short );
		csize = convert<unsigned int>( &buf[ i ], swap ); i += sizeof( unsigned int );

		if( cid != MAIN3DS )
			return sc;

		sg::v_appearance_ptr app;

		chunk edit;
		if( !find_chunk( EDIT3DS, &buf[ i ], 0, i, siz, swap, edit ) )
			return sc;

		chunk material;
		if( find_chunk( MAT_ENTRY, &buf[ edit.offset ], 0, edit.offset, siz, swap, material ) )
			app = read_material( &buf[ material.offset ], 0, material.offset, material.size, swap, path.branch_path( ) );

		sg::group_ptr grp( new sg::group( ) );
		sg::m_spatial_ptr* children = grp->pointer<sg::m_spatial_ptr>( L"children" );

		i = edit.offset;
		while( i < siz )
		{
			chunk scene;
			if( !find_chunk( EDIT_OBJECT, &buf[ 0 ], i, 0, siz, swap, scene ) )
				break;

			sg::shape_ptr s( new sg::shape( ) );

			sg::v_mesh_ptr*		  m = s->pointer<sg::v_mesh_ptr>( L"geometry" );
			sg::v_appearance_ptr* a = s->pointer<sg::v_appearance_ptr>( L"appearance" );

			sg::v_mesh_ptr mesh = read_scene( &buf[ scene.offset ], 0, scene.offset, scene.size, swap );
			
			*a = app;
			*m = mesh;
			
			children->push_back( s );
				
			i = scene.offset + scene.size - 6;
		}
		
		sc->set_root( grp );

		sg::calculate_tangent_space ts;
		ts.calculate( grp );

		return sc;
	}
}

sg::scene_ptr max3ds_plugin::load( const fs::path& path, const opl::string& /*options*/ )
{ return load_3ds( path ); }

bool max3ds_plugin::store( const fs::path& /*path*/, const sg::scene_ptr& /*root*/ )
{ return false; }

} } } }
