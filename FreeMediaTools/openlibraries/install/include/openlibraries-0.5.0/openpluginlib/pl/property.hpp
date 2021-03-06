
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#pragma warn "mike"
#ifndef PROPERTY_INC_
#pragma warn "mikewas"
#define PROPERTY_INC_
#pragma warning "mikewashere"

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable:4251 )
#endif

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <vector>

#pragma warning "mikewashere2"
#if defined ( HAVE_BOOST_FILESYSTEM )
#include <boost/filesystem/path.hpp>
#endif

#pragma warning "mikewashere3"
#if defined( HAVE_CG_RUNTIME )
#include <openpluginlib/pl/cg.hpp>
#endif

#pragma warning "mikewashere4"
#include <openpluginlib/pl/bind_info.hpp>
#include <openpluginlib/pl/geometry.hpp>
#include <openpluginlib/pl/string.hpp>

#pragma warning "mikewashere5"

#ifndef HAVE_GL_GLEW_H
#pragma warning "dont have GLEW_H, dont car"
#endif

//#ifdef HAVE_GL_GLEW_H

#pragma warning "mikewashere6"
namespace olib { namespace openpluginlib {

#pragma warning "mikewashere openpluginlib"
// parameters can be of one of 4 types...
typedef int update_flags;

const update_flags input_only		= 0x1;
const update_flags output_only		= 0x2;
const update_flags input_output		= 0x4;
const update_flags input_once_only	= 0x8;

template<class T>
class value_property
{
public:
	typedef T 			value_type;
	typedef T&			reference;
	typedef const T& 	const_reference;

	explicit value_property( const_reference v = T( ), update_flags flags = input_once_only )
		: v_( v )
		, flags_( flags )
	{ }

	value_property( const value_property<T>& v )
		: v_( v.v_ )
		, flags_( v.flags_ )
	{ }

	value_property<T>& operator=( const value_property<T>& v )
	{
		value_property<T>( v ).swap( *this );
		return *this;
	}

	value_type 		value( ) const	{ return v_; }
	reference 		get( )			{ return v_; }
	const_reference	get( ) const	{ return v_; }
	
	void			set_value( const_reference v )	{ v_ = v; }

	update_flags flags( ) const 
	{ return flags_; }

	void swap( value_property<T>& v )
	{
		std::swap( v_, v.v_ );
		std::swap( flags_, v.flags_ );
	}

public:
	const bind_info& bi( ) const { return bi_; }
	bind_info&		 bi( )		 { return bi_; }

private:
	T v_;
	update_flags flags_;
	
private:
	bind_info bi_;
};

template<class T>
void swap( value_property<T>& lhs, value_property<T>& rhs )
{ lhs.swap( rhs ); }
			
template<class T>
class multi_value_property
{
public:
	typedef std::vector<T>						container;
	typedef typename container::reference		reference;
	typedef typename container::const_reference	const_reference;
	typedef typename container::pointer			pointer;
	typedef typename container::const_pointer	const_pointer;
	typedef typename container::iterator		iterator;
	typedef typename container::const_iterator	const_iterator;
	typedef typename container::size_type		size_type;
	typedef typename container::value_type		value_type;
				
	explicit multi_value_property( update_flags flags = input_once_only )
		: v_( )
		, flags_( flags )
	{ }

	explicit multi_value_property( const container& v )
		: v_( v )
		, flags_( v.flags_ )
	{ }

	multi_value_property( const multi_value_property<T>& v )
		: v_( v.v_ )
		, flags_( v.flags_ )
	{ }

	multi_value_property<T>& operator=( const multi_value_property<T>& v )
	{
		multi_value_property<T>( v ).swap( *this );
		return *this;
	}

#if !defined( PYTHON_BINDING )
	const_iterator begin( ) const
	{ return v_.begin( ); }
	const_iterator end( ) const
	{ return v_.end( ); }
#endif

	iterator begin( )
	{ return v_.begin( ); }
	iterator end( )
	{ return v_.end( ); }

	bool empty( ) const
	{ return v_.empty( ); }

	size_type size( ) const
	{ return v_.size( ); }

	const_pointer data( ) const
	{ return &v_[ 0 ]; }
	pointer data( )
	{ return &v_[ 0 ]; }

	void clear( )
	{ v_.clear( ); }
	void push_back( const_reference v )
	{ v_.push_back( v ); }

	void resize( size_type n )
	{ v_.resize( n ); }
	void reserve( size_type n )
	{ v_.reserve( n ); }

	const_reference operator[ ]( size_type i ) const
	{ return v_[ i ]; }
	reference operator[ ]( size_type i )
	{ return v_[ i ]; }

	update_flags flags( ) const 
	{ return flags_; }

	void swap( multi_value_property<T>& v )
	{
		v_.swap( v.v_ );
		std::swap( flags_, v_.flags_ );
	}

public:
	const bind_info& bi( ) const { return bi_; }
	bind_info&		 bi( )		 { return bi_; }

private:
	container v_;
	update_flags flags_;
	
private:
	bind_info bi_;
};
			
template<class T>
void swap( multi_value_property<T>& lhs, multi_value_property<T>& rhs )
{ lhs.swap( rhs ); }

typedef value_property<bool>							v_bool;
typedef value_property<color<float> >					v_color;
typedef value_property<color_rgba<float> >				v_color_rgba;
typedef value_property<double>							v_double;
typedef value_property<float>							v_float;
typedef value_property<int>								v_int32;
typedef value_property<matrixd>							v_matrix4d;
typedef value_property<matrixf>							v_matrix4f;
typedef value_property<axis_angle<float> >				v_rotation;
typedef value_property<openpluginlib::wstring>			v_string;
typedef v_float											v_time;
typedef value_property<unsigned int>					v_uint32;
#if defined ( HAVE_BOOST_FILESYSTEM )
typedef value_property<boost::filesystem::path>			v_url;
#endif
typedef value_property<vec2d>							v_vec2d;
typedef value_property<vec2f>							v_vec2f;
typedef value_property<vec3d>							v_vec3d;
typedef value_property<vec3f>							v_vec3f;
typedef value_property<vec4d>							v_vec4d;
typedef value_property<vec4f>							v_vec4f;

typedef multi_value_property<bool>						m_bool;
typedef multi_value_property<color<float> >				m_color;
typedef multi_value_property<color_rgba<float> >		m_color_rgba;
typedef multi_value_property<float>						m_float;
typedef multi_value_property<double>					m_double;
typedef multi_value_property<int>						m_int32;
typedef multi_value_property<matrixd>					m_matrix4d;
typedef multi_value_property<matrixf>					m_matrix4f;
typedef multi_value_property<axis_angle<float> >		m_rotation;
typedef multi_value_property<openpluginlib::wstring>	m_string;
typedef m_float											m_time;
#if defined ( HAVE_BOOST_FILESYSTEM )
typedef multi_value_property<boost::filesystem::path>	m_url;
#endif
typedef multi_value_property<vec2d>						m_vec2d;
typedef multi_value_property<vec2f>						m_vec2f;
typedef multi_value_property<vec3d>						m_vec3d;
typedef multi_value_property<vec3f>						m_vec3f;
typedef multi_value_property<vec4d>						m_vec4d;
typedef multi_value_property<vec4f>						m_vec4f;

#if defined( HAVE_CG_RUNTIME )
typedef value_property<CGparameter>						v_CGparameter;
typedef value_property<CGprogram>						v_CGprogram;
#endif


//olib::openpluginlib::
class property_typename : public
openpluginlib::wstring
{
public:
  property_typename(const wchar_t *);
};


const property_typename v_bool_typename					= L"SFBool";
const property_typename v_color_typename				= L"SFColor";
const property_typename v_color_rgba_typename			= L"SFColorRGBA";
const property_typename v_double_typename				= L"SFDouble";
const property_typename v_float_typename				= L"SFFloat";
const property_typename v_frame_position_typename		= L"SFFramePosition";
const property_typename v_int32_typename				= L"SFInt32";
const property_typename v_matrix4d_typename				= L"SFMatrix4d";
const property_typename v_matrix4f_typename				= L"SFMatrix4f";
const property_typename v_rotation_typename				= L"SFRotation";
const property_typename v_string_typename				= L"SFString";
const property_typename v_time_typename					= L"SFTime";
const property_typename v_uint32_typename				= L"SFUint32";
const property_typename v_url_typename					= L"SFUrl";
const property_typename v_vec2f_typename				= L"SFVec2f";
const property_typename v_vec3f_typename				= L"SFVec3f";
const property_typename v_vec4f_typename				= L"SFVec4f";

const property_typename m_bool_typename					= L"MFBool";
const property_typename m_color_typename				= L"MFColor";
const property_typename m_color_rgba_typename			= L"MFColorRGBA";
const property_typename m_double_typename				= L"MFDouble";
const property_typename m_float_typename				= L"MFFloat";
const property_typename m_frame_position_typename		= L"MFFramePosition";
const property_typename m_int32_typename				= L"MFInt32";
const property_typename m_matrix4d_typename				= L"MFMatrix4d";
const property_typename m_matrix4f_typename				= L"MFMatrix4f";
const property_typename m_string_typename				= L"MFString";
const property_typename m_time_typename					= L"MFTime";
const property_typename m_uint32_typename				= L"MFUint32";
const property_typename m_url_typename					= L"MFUrl";
const property_typename m_vec2f_typename				= L"MFVec2f";
const property_typename m_vec3f_typename				= L"MFVec3f";
const property_typename m_vec4f_typename				= L"MFVec4f";

} }

//#endif

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif
