/* -*- tab-width: 4; indent-tabs-mode: t -*- */ 
// gensys - Generic plugin functionality

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

// TODO: Split file up - source file per plugin needed.

#include <openmedialib/ml/openmedialib_plugin.hpp>

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif // WIN32

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <cmath>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/cstdint.hpp>
#include <openpluginlib/pl/pcos/observer.hpp>

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4251 )
#endif

namespace opl = olib::openpluginlib;
namespace plugin = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;

namespace olib { namespace openmedialib { namespace ml { 

// Utility functionality common to the plugin
inline void fill( image_type_ptr img, size_t plane, unsigned char val )
{
	unsigned char *ptr = img->data( plane );
	int width = img->width( plane );
	int height = img->height( plane );
	int diff = img->pitch( plane );
	if ( ptr && val >= 16 && val <= 240 )
	{
		while( height -- )
		{
			memset( ptr, val, width );
			ptr += diff;
		}
	}
}

// A frame generator which provides a fixed colour
//
// NB: Decided to expose the user specified colour at a sample level rather 
// then a string (like 0xrrggbbaa) mainly to simplify the interface and to avoid
// parsing errors. 
//
// Since properties are primitive types only (int, double, string etc), a structure 
// isn't currently an option (though an array of primitives is arguably better for 
// the tuples required here).
//
// TODO: Provide additional properties to allow specification of the colour in 
// different colour spaces (RGBA, YIQ, YCbCr etc). 
//
// TODO: Provide support for additional image outputs (yuv planar formats supported)

class ML_PLUGIN_DECLSPEC colour_input : public input_type
{
	public:
		// Constructor and destructor
		colour_input( ) 
			: input_type( ) 
			, prop_colourspace_( pcos::key::from_string( "colourspace" ) )
			, prop_r_( pcos::key::from_string( "r" ) )
			, prop_g_( pcos::key::from_string( "g" ) )
			, prop_b_( pcos::key::from_string( "b" ) )
			, prop_a_( pcos::key::from_string( "a" ) )
			, prop_width_( pcos::key::from_string( "width" ) )
			, prop_height_( pcos::key::from_string( "height" ) )
			, prop_fps_num_( pcos::key::from_string( "fps_num" ) )
			, prop_fps_den_( pcos::key::from_string( "fps_den" ) )
			, prop_sar_num_( pcos::key::from_string( "sar_num" ) )
			, prop_sar_den_( pcos::key::from_string( "sar_den" ) )
			, prop_out_( pcos::key::from_string( "out" ) )
		{
			// Default to a black PAL video
			properties( ).append( prop_colourspace_ = opl::wstring( L"yuv420p" ) );
			properties( ).append( prop_r_ = 0x00 );
			properties( ).append( prop_g_ = 0x00 );
			properties( ).append( prop_b_ = 0x00 );
			properties( ).append( prop_a_ = 0xff );
			properties( ).append( prop_width_ = 720 );
			properties( ).append( prop_height_ = 576 );
			properties( ).append( prop_fps_num_ = 25 );
			properties( ).append( prop_fps_den_ = 1 );
			properties( ).append( prop_sar_num_ = 59 );
			properties( ).append( prop_sar_den_ = 54 );
			properties( ).append( prop_out_ = 25 );
		}

		virtual ~colour_input( ) { }

		// Basic information
		virtual const opl::wstring get_uri( ) const { return L"colour:"; }
		virtual const opl::wstring get_mime_type( ) const { return L""; }

		// Audio/Visual
		virtual int get_frames( ) const { return prop_out_.value< int >( ); }
		virtual bool is_seekable( ) const { return true; }

		// Visual
		virtual void get_fps( int &num, int &den ) const 
		{
			num = prop_fps_num_.value< int >( );
			den = prop_fps_den_.value< int >( );
		}

		virtual double fps( ) const
		{
			int num, den;
			get_fps( num, den );
			return den != 0 ? double( num ) / double( den ) : 1;
		}

		virtual void get_sar( int &num, int &den ) const 
		{
			num = prop_sar_num_.value< int >( );
			den = prop_sar_den_.value< int >( );
		}

		virtual int get_video_streams( ) const { return 1; }

		virtual int get_width( ) const { return prop_width_.value< int >( ); }
		virtual int get_height( ) const { return prop_height_.value< int >( ); }

		// Audio
		virtual int get_audio_streams( ) const { return 0; }

		// Fetch method
		virtual frame_type_ptr fetch( )
		{
			// Obtain property values
			acquire_values( );

			// Create and populate the image
			image_type_ptr image = il::allocate( prop_colourspace_.value< opl::wstring >( ).c_str(), get_width( ), get_height( ) );
			if ( image )
			{
				image->set_writable( true );
				populate( image );
			}

			image_type_ptr alpha;
			if ( prop_a_.value< int >( ) != 0xff )
			{
				alpha = il::allocate( L"l8", get_width( ), get_height( ) );
				if ( alpha )
				{
					alpha->set_writable( true );
					fill( alpha, 0, ( unsigned char )( prop_a_.value< int >( ) ) );
				}
			}
			
			// Construct a frame and populate with basic information
			frame_type *frame = new frame_type( );
			frame->set_sar( prop_sar_num_.value< int >( ), prop_sar_den_.value< int >( ) );
			frame->set_fps( prop_fps_num_.value< int >( ), prop_fps_den_.value< int >( ) );
			frame->set_pts( get_position( ) * 1.0 / fps( ) );
			frame->set_duration( 1.0 / fps( ) );
			frame->set_position( get_position( ) );

			// Set the image
			frame->set_image( image );
			frame->set_alpha( alpha );

			// Return a frame
			return frame_type_ptr( frame );
		}

		virtual bool reuse( ) { return false; }

	protected:
		void populate( il::image_type_ptr image )
		{
			if ( il::is_yuv_planar( image ) )
			{
				int y, u, v;
				il::rgb24_to_yuv444( y, u, v, ( unsigned char )prop_r_.value< int >( ), ( unsigned char )prop_g_.value< int >( ), ( unsigned char )prop_b_.value< int >( ) );
				fill( image, 0, ( unsigned char )y );
				fill( image, 1, ( unsigned char )u );
				fill( image, 2, ( unsigned char )v );
			}
		}

	private:
		pcos::property prop_colourspace_;
		pcos::property prop_r_;
		pcos::property prop_g_;
		pcos::property prop_b_;
		pcos::property prop_a_;
		pcos::property prop_width_;
		pcos::property prop_height_;
		pcos::property prop_fps_num_;
		pcos::property prop_fps_den_;
		pcos::property prop_sar_num_;
		pcos::property prop_sar_den_;
		pcos::property prop_out_;
};

// A generic frame pusher input
//
// Doesn't do much - frames that are pushed in are retrieved on the fetch. Seeks
// are ignored.
//
// TODO: Impose a property driven max size.
//
// TODO: Introduce thread safety

class ML_PLUGIN_DECLSPEC pusher_input : public input_type
{
	public:
		// Constructor and destructor
		pusher_input( ) : input_type( ) { }
		virtual ~pusher_input( ) { }

		// Basic information
		virtual const opl::wstring get_uri( ) const { return L"pusher:"; }
		virtual const opl::wstring get_mime_type( ) const { return L""; }

		// Audio/Visual
		virtual int get_frames( ) const { return int( queue_.size( ) ); }
		virtual bool is_seekable( ) const { return false; }

		// Visual
		virtual void get_fps( int& /*num*/, int& /*den*/ ) const { }
		virtual void get_sar( int& /*num*/, int& /*den*/ ) const { }
		virtual int get_video_streams( ) const { return 1; }
		virtual int get_width( ) const { return 0; }
		virtual int get_height( ) const { return 0; }

		// Audio
		virtual int get_audio_streams( ) const { return 1; }

		// Fetch method
		virtual frame_type_ptr fetch( )
		{
			// Obtain property values
			acquire_values( );

			// Acquire frame
			frame_type_ptr frame;
			if ( queue_.size( ) > 0 )
			{
				frame = *( queue_.begin( ) );
				queue_.pop_front( );
			}

			// Return a frame
			return frame;
		}

		// Push method
		virtual bool push( frame_type_ptr frame )
		{
			queue_.push_back( frame );
			return true;
		}

	private:
		std::deque< frame_type_ptr > queue_;
};

// Chroma replacer.
//
// Replaces all chroma with the u and v values specified. All output is
// yuv planar only - if the input is not yuv planar, yuv420p is output.
//
// Default behaviour is greyscale.
//
// Properties:
//
//	u, v = chroma values [default: 128, 128]
//		Values should be in the range 16 to 240 - not change occurs when the
//		value is out of range.

class ML_PLUGIN_DECLSPEC chroma_filter : public filter_type
{
	public:
		chroma_filter( )
			: filter_type( )
			, prop_enable_( pcos::key::from_string( "enable" ) )
			, prop_u_( pcos::key::from_string( "u" ) )
			, prop_v_( pcos::key::from_string( "v" ) )
		{
			properties( ).append( prop_enable_ = 1 );
			properties( ).append( prop_u_ = 128 );
			properties( ).append( prop_v_ = 128 );
		}

		virtual const opl::wstring get_uri( ) const { return L"chroma"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( prop_enable_.value< int >( ) && result && result->get_image( ) )
			{
				if ( !is_yuv_planar( result ) )
					result = frame_convert( result, L"yuv420p" );
				image_type_ptr img = result->get_image( );
				img = il::conform( img, il::writable );
				if ( img )
				{
					fill( img, 1, ( unsigned char )prop_u_.value< int >( ) );
					fill( img, 2, ( unsigned char )prop_v_.value< int >( ) );
				}
				result->set_image( img );
			}
			
			return result;
		}

	private:
		pcos::property prop_enable_;
		pcos::property prop_u_;
		pcos::property prop_v_;
};

// Conform filter
//
// Ensures all frames which pass through have image and/or audio components
// as requested.
//
// Properties:
//
//	image = 0, 1 [default] or 2
//		if 0: remove the image if present
//		if 1: if an image is not present, create a black image
//		if 2: do nothing
//
//	audio = 0, 1 [default] or 2
//		if 0: remove the audio if present
//		if 1: if an audio is not present, create silence at frame size
//		if 2: do nothing
//
//	NB: Currently, there's no control over the image or audio provided.
//	Downstream filters should be used to conform things further.
//

class ML_PLUGIN_DECLSPEC conform_filter : public filter_type
{
	public:
		conform_filter( )
			: filter_type( )
			, prop_image_( pcos::key::from_string( "image" ) )
			, prop_audio_( pcos::key::from_string( "audio" ) )
			, prop_frequency_( pcos::key::from_string( "frequency" ) )
			, prop_channels_( pcos::key::from_string( "channels" ) )
			, prop_pf_( pcos::key::from_string( "pf" ) )
		{
			properties( ).append( prop_image_ = 1 );
			properties( ).append( prop_audio_ = 1 );
			properties( ).append( prop_frequency_ = 48000 );
			properties( ).append( prop_channels_ = 2 );
			properties( ).append( prop_pf_ = opl::wstring( L"yuv420p" ) );
		}

		virtual const opl::wstring get_uri( ) const { return L"conform"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( result )
			{
				if ( prop_image_.value< int >( ) == 1 )
				{
					if ( !result->get_image( ) )
					{
						image_type_ptr image = il::allocate( prop_pf_.value< opl::wstring >( ).c_str( ), 720, 576 );
						fill( image, 0, ( unsigned char )16 );
						fill( image, 1, ( unsigned char )128 );
						fill( image, 2, ( unsigned char )128 );
						result->set_image( image );
						result->set_sar( 59, 54 );
					}
				}
				else if ( prop_image_.value< int >( ) == 0 )
				{
					result->set_image( image_type_ptr( ) );
				}

				if ( prop_audio_.value< int >( ) == 1 )
				{
					if ( !result->get_audio( ) )
					{
						int frequency = prop_frequency_.value< int >( );
						int channels = prop_channels_.value< int >( );
						int samples = audio_samples_for_frame( get_position( ), frequency, result->get_fps_num( ), result->get_fps_den( ) );
						audio_type_ptr aud = audio_type_ptr( new audio_type( audio<unsigned char, pcm16>( frequency, channels, samples ) ) );
						memset( aud->data( ), 0, channels * aud->samples( ) * sizeof( short ) );
						result->set_audio( aud );
					}
				}
				else if ( prop_audio_.value< int >( ) == 0 )
				{
					result->set_audio( audio_type_ptr( ) );
				}
			}
			
			return result;
		}

	private:
		pcos::property prop_image_;
		pcos::property prop_audio_;
		pcos::property prop_frequency_;
		pcos::property prop_channels_;
		pcos::property prop_pf_;
};

// Crop filter
//
// Requires a single input - crops an image to the specified relative 
// coordinates.
//
// Note: that if you have multiple crop filters in sequence, the default 
// behaviour is to reduce the image relative to the currently reported 
// dimensions.
//
// Property usage:
//
//	absolute = 0 [default] or 1
//		crop the reported image dimensions or the full uncropped image
//
//	rx, ry, rw, rh = double, double, double, double (0,0,1,1 is default)
//		the visible area of the background is specified in the range of 0.0 to 1.0

class ML_PLUGIN_DECLSPEC crop_filter : public filter_type
{
	public:
		crop_filter( )
			: filter_type( )
			, prop_enable_( pcos::key::from_string( "enable" ) )
			, prop_absolute_( pcos::key::from_string( "absolute" ) )
			, prop_rx_( pcos::key::from_string( "rx" ) )
			, prop_ry_( pcos::key::from_string( "ry" ) )
			, prop_rw_( pcos::key::from_string( "rw" ) )
			, prop_rh_( pcos::key::from_string( "rh" ) )
		{
			properties( ).append( prop_enable_ = 1 );
			properties( ).append( prop_absolute_ = 0 );
			properties( ).append( prop_rx_ = 0.0 );
			properties( ).append( prop_ry_ = 0.0 );
			properties( ).append( prop_rw_ = 1.0 );
			properties( ).append( prop_rh_ = 1.0 );
		}

		virtual const opl::wstring get_uri( ) const { return L"crop"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( prop_enable_.value< int >( ) && result && result->get_image( ) )
			{
				if ( !is_yuv_planar( result ) )
					result = frame_convert( result, L"yuv420p" );
				image_type_ptr img = result->get_image( );

				// Ensure the image component is writable
				img = il::conform( img, il::writable );
				result->set_image( img );

				if ( img )
				{
					if ( prop_absolute_.value< int >( ) == 1 )
						result = frame_crop_clear( result );

					int px = int( img->width( ) * prop_rx_.value< double >( ) );
					int py = int( img->height( ) * prop_ry_.value< double >( ) );
					int pw = int( img->width( ) * prop_rw_.value< double >( ) );
					int ph = int( img->height( ) * prop_rh_.value< double >( ) );

					if ( px < 0 )
					{
						pw += px;
						px = 0;
					}

					if ( py < 0 )
					{
						ph += py;
						py = 0;
					}

					if ( ( px + pw ) > img->width( ) )
					{
						pw = img->width( ) - px;
					}

					if ( ( py + ph ) > img->height( ) )
					{
						ph = img->height( ) - py;
					}

					result = frame_crop( result, px, py, pw, ph );
				}
			}

			return result;
		}

	private:
		pcos::property prop_enable_;
		pcos::property prop_absolute_;
		pcos::property prop_rx_;
		pcos::property prop_ry_;
		pcos::property prop_rw_;
		pcos::property prop_rh_;
};

// Composite filter
//
// Requires 2 connected inputs - by default, it composites the second input
// on to the first - this is mainly seen as a mechanism to provide letter/pillar
// boxing type of functionality.
//
// It provides a number of modes - these are mainly for the purposes of simplifying
// the usage from the applications point of view. 
//
// When the application provides control over the geometry of the overlay, it can 
// specify the physical pixel geometry (which needs to be background aware) or it can
// specify relative position (which typically gives more flexibility in terms of the
// background resolution and aspect ratio).
//
// Coordinates of 0,0 in both pixel and relative mode specify the top left hand corner.
//
// Property usage:
//
//	pixel = 0 [default] or 1
//		use pixel or relative pixel coordinates
//
//	rx, ry, rw, rh = double, double, double, double (0,0,1,1 is default)
//		the visible area of the background is specified in the range of 0.0 to 1.0
//
//	px, py, pw, ph = int, int, int, int (0,0,720,576 is default)
//		coordinates specified should be specified relative to background hence
//		the onus is pushed to the assignment functionality
//
//	mix = 0.0 to 1.0 [default]
//		level of video mixing for the requested frame
//
//	mix_in, mix_mid, mix_out = double, double, double (1,1,1)
//		Proposed/unimplemented: 3 point audio mixing for the current frame 
//
//	mode = fill [default], distort, letterbox, pillarbox, native
//		fill ensures that the overlay is scaled to fit in the specied geometry 
//		whilst maintaining aspect ratio
//		distort ensures the overlay is scaled to fit the specified geometry
//		ignoring aspect ratio
//		letterbox scales the image to fit horizontally whilst maintaining
//		aspect ratio (cropping may occur)
//		pillarbox scales the image to fit vertically whilst maintaining
//		aspect ratio (cropping may occur)
//		native scales the image to honour sample aspect ratio - cropping or
//		banding may occur
//
//	TODO: Provide support for non yuv planar formats.
//
//	TODO: Fix cropping/offsets to specified geometry.
//
//	TODO: Vertical and horizontal alignment.
//
//	TODO: Additional alpha compositing modes.

struct geometry { int x, y, w, h; };

class ML_PLUGIN_DECLSPEC composite_filter : public filter_type
{
	public:
		typedef frame_type_ptr ( *frame_rescale_type )( frame_type_ptr, int, int, il::rescale_filter );
		typedef il::image_type_ptr ( *image_rescale_type )( const il::image_type_ptr &, int, int, int, il::rescale_filter );

		composite_filter( )
			: filter_type( )
			, prop_enable_( pcos::key::from_string( "enable" ) )
			, prop_mode_( pcos::key::from_string( "mode" ) )
			, prop_swap_( pcos::key::from_string( "swap" ) )
			, prop_pixel_( pcos::key::from_string( "pixel" ) )
			, prop_px_( pcos::key::from_string( "px" ) )
			, prop_py_( pcos::key::from_string( "py" ) )
			, prop_pw_( pcos::key::from_string( "pw" ) )
			, prop_ph_( pcos::key::from_string( "ph" ) )
			, prop_rx_( pcos::key::from_string( "rx" ) )
			, prop_ry_( pcos::key::from_string( "ry" ) )
			, prop_rw_( pcos::key::from_string( "rw" ) )
			, prop_rh_( pcos::key::from_string( "rh" ) )
			, prop_mix_( pcos::key::from_string( "mix" ) )
			, prop_interp_( pcos::key::from_string( "interp" ) )
			, prop_slot_( pcos::key::from_string( "slot" ) )
			, prop_frame_rescale_cb_( pcos::key::from_string( "frame_rescale_cb" ) )
			, prop_image_rescale_cb_( pcos::key::from_string( "image_rescale_cb" ) )
		{
			properties( ).append( prop_enable_ = 1 );
			properties( ).append( prop_mode_ = opl::wstring( L"fill" ) );
			properties( ).append( prop_swap_ = 0 );
			properties( ).append( prop_pixel_ = 0 );
			properties( ).append( prop_px_ = 0 );
			properties( ).append( prop_py_ = 0 );
			properties( ).append( prop_pw_ = 720 );
			properties( ).append( prop_ph_ = 576 );
			properties( ).append( prop_rx_ = 0.0 );
			properties( ).append( prop_ry_ = 0.0 );
			properties( ).append( prop_rw_ = 1.0 );
			properties( ).append( prop_rh_ = 1.0 );
			properties( ).append( prop_mix_ = 1.0 );
			properties( ).append( prop_interp_ = opl::wstring( L"bilinear" ) );
			properties( ).append( prop_slot_ = 0 );
			properties( ).append( prop_frame_rescale_cb_ = ( void *) 0 );
			properties( ).append( prop_image_rescale_cb_ = ( void *) 0 );
		}

		virtual const opl::wstring get_uri( ) const { return L"composite"; }

		virtual const size_t slot_count( ) const { return 2; }

		virtual int get_frames( ) const
		{
			input_type_ptr input = fetch_slot( prop_slot_.value< int >( ) );
			return input ? input->get_frames( ) : 0;
		}

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			frame_type_ptr overlay;

			if ( prop_enable_.value< int >( ) == 0 )
			{
				result = fetch_from_slot( prop_slot_.value< int >( ) );
			}
			else
			{
				result = fetch_from_slot( prop_swap_.value< int >( ) ? 1 : 0, false );
				overlay = fetch_from_slot( prop_swap_.value< int >( ) ? 0 : 1 );
			}

			if ( result && overlay )
			{
				image_type_ptr dst = result->get_image( );
				image_type_ptr src = overlay->get_image( );

				if ( dst && src )
				{
					struct geometry geom = calculate_full( result, overlay );
					result = composite( result, overlay, geom );
				}

				if ( result->get_audio( ) && overlay->get_audio( ) )
				{
					audio_type_ptr mix = audio_mix( result->get_audio( ), overlay->get_audio( ) );
					result->set_audio( mix );
				}

				if ( !result->get_audio( ) )
				{
					result->set_audio( overlay->get_audio( ) );
				}
			}
			else if ( overlay )
			{
				// Incorrect, but better than nothing for now
				result = overlay;
			}

			if ( result )
				result->set_position( get_position( ) );

			return result;
		}

	protected:
		struct geometry calculate_full( frame_type_ptr dst, frame_type_ptr src )
		{
			struct geometry result;

			double src_sar_num = double( src->get_sar_num( ) ); 
			double src_sar_den = double( src->get_sar_den( ) );
			double dst_sar_num = double( dst->get_sar_num( ) );
			double dst_sar_den = double( dst->get_sar_den( ) );
			double src_w = double( src->get_image( )->width( ) );
			double src_h = double( src->get_image( )->height( ) );
			double dst_w = double( dst->get_image( )->width( ) );
			double dst_h = double( dst->get_image( )->height( ) );

			if ( src_sar_num == 0 ) src_sar_num = 1;
			if ( src_sar_den == 0 ) src_sar_den = 1;
			if ( dst_sar_num == 0 ) dst_sar_num = 1;
			if ( dst_sar_den == 0 ) dst_sar_den = 1;

			if ( prop_pixel_.value< int >( ) )
			{
				result.x = prop_px_.value< int >( );
				result.y = prop_py_.value< int >( );
				result.w = prop_pw_.value< int >( );
				result.h = prop_ph_.value< int >( );
			}
			else
			{
				result.x = int( dst_w * prop_rx_.value< double >( ) );
				result.y = int( dst_h * prop_ry_.value< double >( ) );
				result.w = int( dst_w * prop_rw_.value< double >( ) );
				result.h = int( dst_h * prop_rh_.value< double >( ) );
			}

			if ( prop_mode_.value< opl::wstring >( ) == L"fill" )
			{
				dst_w = double( result.w );
				dst_h = double( result.h );

				result.h = int( dst_h );
				result.w = int( ( dst_h * src_w * src_sar_num * dst_sar_den ) / ( src_h * src_sar_den * dst_sar_num ) );

				if ( result.w > int( dst_w ) )
				{
					result.w = int( dst_w );
					result.h = int( ( dst_w * src_h * src_sar_den * dst_sar_num ) / ( src_w * src_sar_num * dst_sar_den ) );
				}

				result.x += ( int( dst_w ) - result.w ) / 2;
				result.y += ( int( dst_h ) - result.h ) / 2;
			}
			else if ( prop_mode_.value< opl::wstring >( ) == L"letterbox" )
			{
				dst_w = double( result.w );
				dst_h = double( result.h );

				result.w = int( dst_w );
				result.h = int( ( dst_w * src_h * src_sar_den * dst_sar_num ) / ( src_w * src_sar_num * dst_sar_den ) );

				result.x += ( int( dst_w ) - result.w ) / 2;
				result.y += ( int( dst_h ) - result.h ) / 2;
			}
			else if ( prop_mode_.value< opl::wstring >( ) == L"pillarbox" )
			{
				dst_w = double( result.w );
				dst_h = double( result.h );

				result.h = int( dst_h );
				result.w = int( ( dst_h * src_w * src_sar_num * dst_sar_den ) / ( src_h * src_sar_den * dst_sar_num ) );

				result.x += ( int( dst_w ) - result.w ) / 2;
				result.y += ( int( dst_h ) - result.h ) / 2;
			}
			else if ( prop_mode_.value< opl::wstring >( ) == L"native" )
			{
				result.h = int( src_h );
				result.w = int( ( src_h * src_w * src_sar_num * dst_sar_den ) / ( src_h * src_sar_den * dst_sar_num ) );
				result.x += ( int( dst_w ) - result.w ) / 2;
				result.y += ( int( dst_h ) - result.h ) / 2;
			}

			return result;
		}

		frame_type_ptr composite( frame_type_ptr background, frame_type_ptr foreground, struct geometry geom )
		{
			// Ensure conformance
			if ( !is_yuv_planar( background ) )
				background = frame_convert( background, L"yuv420p" );
			foreground = frame_convert( foreground, background->get_image( )->pf( ) );

			frame_rescale_type f_rescale = reinterpret_cast< frame_rescale_type >( prop_frame_rescale_cb_.value< void* >( ) );
			image_rescale_type i_rescale = reinterpret_cast< image_rescale_type >( prop_image_rescale_cb_.value< void* >( ) );

			if( !f_rescale )
				f_rescale = ml::frame_rescale;

			if( !i_rescale )
				i_rescale = il::rescale;

			il::rescale_filter filter = il::BILINEAR_SAMPLING;
			if ( prop_interp_.value< opl::wstring >( ) == L"point" )
				filter = il::POINT_SAMPLING;
			else if ( prop_interp_.value< opl::wstring >( ) == L"bicubic" )
				filter = il::BICUBIC_SAMPLING;
			foreground = f_rescale( foreground, geom.w, geom.h, filter );

			// We're going to update both image and alpha here
			background->set_image( il::conform( background->get_image( ), il::writable ) );
			background->set_alpha( il::conform( background->get_alpha( ), il::writable ) );

			// Extract images
			image_type_ptr dst = background->get_image( );
			image_type_ptr src = foreground->get_image( );

			// Obtain the src and dst image dimensions
			int src_width = src->width( );
			int src_height = src->height( );
			int dst_width = dst->width( );
			int dst_height = dst->height( );

			// Calculate the src x,y offset
			int dst_x = geom.x;
			int dst_y = geom.y;
			int src_x = 0;
			int src_y = 0;

			if ( dst_x < 0 )
			{
				src_width += dst_x;
				src_x -= dst_x;
				dst_x = 0;
			}

			if ( dst_y < 0 )
			{
				src_height += dst_y;
				src_y -= dst_y;
				dst_y = 0;
			}

			// Trim from below and to the right
			if ( dst_x + src_width > dst_width )
				src_width -= ( dst_x + src_width - dst_width );
		
			if ( dst_y + src_height > dst_height )
				src_height -= ( dst_y + src_height - dst_height );
		
			// Scale down the alphas to the size of the chroma planes 
			image_type_ptr half_src_alpha; 
			image_type_ptr half_dst_alpha;

			if ( background->get_alpha( ) )
				half_dst_alpha = i_rescale( background->get_alpha( ), background->get_image( )->width( 1 ), background->get_image( )->height( 1 ), 1, filter );
			if ( foreground->get_alpha( ) )
				half_src_alpha = i_rescale( foreground->get_alpha( ), foreground->get_image( )->width( 1 ), foreground->get_image( )->height( 1 ), 1, filter );

			// Determine the x and y factors for the chroma plane sizes
			int plane_x_factor = background->get_image( )->width( ) / background->get_image( )->width( 1 );
			int plane_y_factor = background->get_image( )->height( ) / background->get_image( )->height( 1 );

			// Composite the image if we have any image left...
			for ( size_t p = 0; p < size_t( dst->plane_count( ) ); p ++ )
			{
				if ( src_width > 0 && src_height > 0 )
				{
					// Calculate the byte offsets
					unsigned char *src_ptr = src->data( p ) + src_y * src->pitch( p ) + src_x;
					unsigned char *dst_ptr = dst->data( p ) + dst_y * dst->pitch( p ) + dst_x;

					// Obtain the pitches of both images and reduce by the distance travelled on each row
					int src_remainder = src->pitch( p ) - src_width;
					int dst_remainder = dst->pitch( p ) - src_width;
					int temp_h = src_height;
					int mix = int( ( 1 << 8 ) * prop_mix_.value< double >( ) );
					int xim = ( 1 << 8 ) - mix;

					if ( !background->get_alpha( ) && !foreground->get_alpha( ) )
					{
						while ( temp_h -- )
						{
							int temp_w = src_width;
							while( temp_w -- )
							{
								*dst_ptr = static_cast< unsigned char >( ( *dst_ptr * xim + *src_ptr * mix ) >> 8 );
								src_ptr ++;
								dst_ptr ++;
							}
							dst_ptr += dst_remainder;
							src_ptr += src_remainder;
						}
					}
					else if ( !background->get_alpha( ) && foreground->get_alpha( ) )
					{
						image_type_ptr src_alpha = p == 0 ? foreground->get_alpha( ) : half_src_alpha;
						unsigned char *src_alpha_ptr = src_alpha->data( ) + src_y * src_alpha->pitch( ) + src_x;
						int src_alpha_remainder = src_alpha->pitch( ) - src_width;
						int alpha = 0;
						while ( temp_h -- )
						{
							int temp_w = src_width;
							while( temp_w -- )
							{
								alpha = ( ( ( *src_alpha_ptr ++ << 8 ) / 255 ) * mix ) >> 8;
								*dst_ptr = static_cast< unsigned char >( ( *dst_ptr * ( ( 1 << 8 ) - alpha ) + *src_ptr * alpha ) >> 8 );
								src_ptr ++;
								dst_ptr ++;
							}
							dst_ptr += dst_remainder;
							src_ptr += src_remainder;
							src_alpha_ptr += src_alpha_remainder;
						}
					}
					else if ( background->get_alpha( ) && !foreground->get_alpha( ) )
					{
						image_type_ptr dst_alpha = p == 0 ? background->get_alpha( ) : half_dst_alpha;
						unsigned char *dst_alpha_ptr = dst_alpha->data( ) + dst_y * dst_alpha->pitch( ) + dst_x;
						int dst_alpha_remainder = dst_alpha->pitch( ) - src_width;
						while ( temp_h -- )
						{
							int temp_w = src_width;
							while( temp_w -- )
							{
								*dst_ptr = static_cast< unsigned char >( ( *dst_ptr * ( xim ) + *src_ptr * mix ) >> 8 );
								*dst_alpha_ptr = 255 + *dst_alpha_ptr - ( ( 255 * *dst_alpha_ptr ) >> 8 );
								src_ptr ++;
								dst_ptr ++;
								dst_alpha_ptr ++;
							}
							dst_ptr += dst_remainder;
							src_ptr += src_remainder;
							dst_alpha_ptr += dst_alpha_remainder;
						}
					}
					else
					{
						image_type_ptr dst_alpha = p == 0 ? background->get_alpha( ) : half_dst_alpha;
						unsigned char *dst_alpha_ptr = dst_alpha->data( ) + dst_y * dst_alpha->pitch( ) + dst_x;
						int dst_alpha_remainder = dst_alpha->pitch( ) - src_width;
						image_type_ptr src_alpha = p == 0 ? foreground->get_alpha( ) : half_src_alpha;
						unsigned char *src_alpha_ptr = src_alpha->data( ) + src_y * src_alpha->pitch( ) + src_x;
						int src_alpha_remainder = src_alpha->pitch( ) - src_width;
						int alpha = 0;
						while ( temp_h -- )
						{
							int temp_w = src_width;
							while( temp_w -- )
							{
								alpha = ( ( ( *src_alpha_ptr << 8 ) / 255 ) * mix ) >> 8;
								*dst_ptr = static_cast< unsigned char >( ( *dst_ptr * ( ( 1 << 8 ) - alpha ) + *src_ptr * alpha ) >> 8 );
								*dst_alpha_ptr = *src_alpha_ptr + *dst_alpha_ptr - ( ( *src_alpha_ptr * *dst_alpha_ptr ) >> 8 );
								src_alpha_ptr ++;
								dst_alpha_ptr ++;
								src_ptr ++;
								dst_ptr ++;
							}
							dst_ptr += dst_remainder;
							src_ptr += src_remainder;
							src_alpha_ptr += src_alpha_remainder;
							dst_alpha_ptr += dst_alpha_remainder;
						}
					}
				}

				// FIXME: This is the part which is yuv planar dependent
				if ( p == 0 )
				{
					src_x /= plane_x_factor;
					src_y /= plane_y_factor;
					dst_x /= plane_x_factor;
					dst_y /= plane_y_factor;
					src_width /= plane_x_factor;
					src_height /= plane_y_factor;
					dst_width /= plane_x_factor;
					dst_height /= plane_y_factor;
				}
			}

			return background;
		}

	private:
		pcos::property prop_enable_;
		pcos::property prop_mode_;
		pcos::property prop_swap_;
		pcos::property prop_pixel_;
		pcos::property prop_px_;
		pcos::property prop_py_;
		pcos::property prop_pw_;
		pcos::property prop_ph_;
		pcos::property prop_rx_;
		pcos::property prop_ry_;
		pcos::property prop_rw_;
		pcos::property prop_rh_;
		pcos::property prop_mix_;
		pcos::property prop_interp_;
		pcos::property prop_slot_;
		pcos::property prop_frame_rescale_cb_;
		pcos::property prop_image_rescale_cb_;
};

// Colour correction filter
//
// Requires a single connected input.
//
// Carries out typical colour correction operations (hue, contrast, brightness
// and saturation).
//
// Works in the YCbCr colour space only.
//
// Properties:
//
// 	contrast = 0.0 to 1.992 (default: 1.0)
// 		Specifies the contrast adjustment (multiplies Y, Cb and Cr)
//
//	brightness = -32 to 31 (default: 0)
//		Specifies the brightness (added to Y)
//
//	hue = -30 to 30 (default 0)
//		Specifies the hue (Cb = Cb cos hue + Cr sin hue, Cr = Cb cos hue - Cr sin hue)
//
//	saturation = 0.0 to 1.992 (default: 1.0)
//		Specifies the saturation (multiplies Cb and Cr)
//
// Notes:
//
// No range checks are carried out - garbage in, garbage out.
//
// TODO: Support additional colour images

class ML_PLUGIN_DECLSPEC correction_filter : public filter_type
{
	public:
		correction_filter( )
			: filter_type( )
			, prop_enable_( pcos::key::from_string( "enable" ) )
			, prop_contrast_( pcos::key::from_string( "contrast" ) )
			, prop_brightness_( pcos::key::from_string( "brightness" ) )
			, prop_hue_( pcos::key::from_string( "hue" ) )
			, prop_saturation_( pcos::key::from_string( "saturation" ) )
		{
			properties( ).append( prop_enable_ = 1 );
			properties( ).append( prop_contrast_ = 1.0 );
			properties( ).append( prop_brightness_ = 0.0 );
			properties( ).append( prop_hue_ = 0.0 );
			properties( ).append( prop_saturation_ = 1.0 );
		}

		virtual const opl::wstring get_uri( ) const { return L"correction"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( result && prop_enable_.value< int >( ) )
				result = process_image( result );
			
			return result;
		}

	private:
		frame_type_ptr process_image( frame_type_ptr result )
		{
			if ( !is_yuv_planar( result ) )
				result = frame_convert( result, L"yuv420p" );
			result->set_image( il::conform( result->get_image( ), il::writable ) );
			image_type_ptr img = result->get_image( );
			if ( img )
			{
				// Interpret properties
				int contrast = int( 256 * prop_contrast_.value< double >( ) );
				int brightness = int( 256 * prop_brightness_.value< double >( ) );
				int sin_hue = int( 256 * ( std::sin( M_PI * prop_hue_.value< double >( ) / 180.0 ) ) );
				int cos_hue = int( 256 * ( std::cos( M_PI * prop_hue_.value< double >( ) / 180.0 ) ) );
				int contrast_saturation = int( 256 * prop_contrast_.value< double >( ) * prop_saturation_.value< double >( ) );

				unsigned char *y = img->data( 0 );
				unsigned char *cb = img->data( 1 );
				unsigned char *cr = img->data( 2 );

				int w = img->width( 0 );
				int h = img->height( 0 );
				int y_rem = img->pitch( 0 ) - img->linesize( 0 );
				int cb_rem = img->pitch( 1 ) - img->linesize( 1 );
				int cr_rem = img->pitch( 2 ) - img->linesize( 2 );
				int t;

				while( h -- )
				{
					t = w;
					while( t -- )
					{
						*y = limit( ( ( ( *y - 16 ) * contrast + brightness ) >> 8 ) + 16, 16, 235 );
						y ++;
					}
					y += y_rem;
				}

				w = img->width( 1 );
				h = img->height( 1 );

				while( h -- )
				{
					t = w;
					while( t -- )
					{
						*cb = limit( ( ( ( ( ( *cb - 128 ) * cos_hue + ( *cr - 128 ) * sin_hue ) >> 8 ) * contrast_saturation ) >> 8 ) + 128, 16, 240 );
						cb ++;
						*cr = limit( ( ( ( ( ( *cr - 128 ) * cos_hue - ( *cb - 128 ) * sin_hue ) >> 8 ) * contrast_saturation ) >> 8 ) + 128, 16, 240 );
						cr ++;
					}
					cb += cb_rem;
					cr += cr_rem;
				}
			}

			return result;
		}

		inline int limit( int v, int l, int u )
		{
			return v < l ? l : v > u ? u : v;
		}

		pcos::property prop_enable_;
		pcos::property prop_contrast_;
		pcos::property prop_brightness_;
		pcos::property prop_hue_;
		pcos::property prop_saturation_;
};

// Threading filter
//
// Requires a single connected input. 
//
// The basic premise of this filter is to provide a threaded read ahead filter
// which can be placed anywhere in a filter graph (see caveats below). 
//
// It has a state of inactive or active - when it's inactive, a fetch is a
// straightforward call into the the connected inputs fetch method. When it's
// active, a thread is employed which pulls frames from the connected input and
// places them in a size limit imposed queue. A fetch from the right hand side of
// the graph will simply obtain the requested frame from the queue and remove it.
// When the readahead queue is full, the thread blocks until a remove occurs
// thus freeing an entry for the thread to populate.
//
// Property usage:
//
// 	active = 0 [default] or 1
// 		start or stop the thread
//
// 	queue = max size of read ahead queue
// 		only modifiable when the filter is inactive and must be >= 1
//
// Caveats:
//
// Can only be used when the left hand node is fully thread safe. 

template < typename C > class fn_observer : public pcos::observer
{
	public:
		fn_observer( C *instance, void ( C::*fn )( ) )
			: instance_( instance )
			, fn_( fn )
		{ }

		virtual void updated( pcos::isubject * )
		{
			( instance_->*fn_ )( );
		}

	private:
		C *instance_;
		void ( C::*fn_ )( );
};

class ML_PLUGIN_DECLSPEC threader_filter : public filter_type
{
	typedef boost::mutex::scoped_lock scoped_lock;

	struct reader_thread
	{
		reader_thread( threader_filter *filter ) : filter_( filter ) { }
		void operator( )( ) { filter_->run( ); }
		threader_filter *filter_;
	};

	public:
		threader_filter( )
			: filter_type( )
			, start_( this )
			, obs_active_( new fn_observer< threader_filter >( const_cast< threader_filter * >( this ), &threader_filter::update_active ) )
			, prop_active_( pcos::key::from_string( "active" ) )
			, running_( false )
			, prop_queue_( pcos::key::from_string( "queue" ) )
			, last_position_( -1 )
			, reader_( 0 )
		{
			properties( ).append( prop_active_ = 0 );
			properties( ).append( prop_queue_ = 25 );
			prop_active_.attach( obs_active_ );
		}

		virtual ~threader_filter( )
		{
			prop_active_.set( 0 );
		}

		virtual const opl::wstring get_uri( ) const { return L"threader"; }

		void update_active( )
		{
			if ( is_thread_safe( ) )
			{
				if ( prop_active_.value< int >( ) == 1 && !running_ )
				{
					running_ = true;
					reader_ = new boost::thread( start_ );
				}
				else if ( prop_active_.value< int >( ) == 0 && running_ )
				{
					running_ = false;
					{
						scoped_lock lock( mutex_ );
						cond_.notify_all( );
					}
					reader_->join( );
					delete reader_;
				}
			}
			else
			{
				fprintf( stderr, "Not thread safe\n" );
			}
		}

		void run( )
		{
			int position = get_position( );
			int max_size = prop_queue_.value< int >( );
			input_type_ptr input = fetch_slot( );

			while( input && running_ && position < input->get_frames( ) )
			{
				input->seek( position );
				frame_type_ptr frame = input->fetch( );
				if ( frame )
				{
					scoped_lock lock( mutex_ );

					if ( int( queue_.size( ) ) >= max_size )
						while ( running_ && int( queue_.size( ) ) >= max_size )
							cond_.wait( lock );

					if ( frame )
						frame->set_position( position );
					queue_.push_back( frame );
					cond_.notify_one( );
				}
				else
				{
					break;
				}

				position += 1;
			}
		}

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			input_type_ptr input = fetch_slot( );

			if ( input )
			{
				if ( !running_ )
				{
					if ( debug_level( ) )
						fprintf( stderr, "requesting %d from dormant\n", get_position( ) );
					input->seek( get_position( ) );
					result = input->fetch( );
					last_position_ = get_position( );
				}
				else
				{
					// This could be a little more intelligent - disposing of
					// the entire queue is a bit agressive
					if ( get_position( ) != last_position_ && get_position( ) != last_position_ + 1 )
					{
						if ( debug_level( ) )
							fprintf( stderr, "restarting the thread %d %d\n", get_position( ), last_position_ );
						prop_active_.set( 0 );
						queue_.clear( );
						input->seek( get_position( ) );
						prop_active_.set( 1 );
					}
					else
					{
						if ( debug_level( ) )
							fprintf( stderr, "requesting %d from active %d\n", get_position( ), int( queue_.size( ) ) );
					}

					last_position_ = get_position( );

					{
						scoped_lock lock( mutex_ );

						if ( queue_.size( ) == 0 )
							cond_.wait( lock );

						if ( queue_.size( ) != 0 )
						{
							result = *( queue_.begin( ) );
							while ( result && get_position( ) > result->get_position( ) )
							{
								queue_.pop_front( );
								if ( queue_.size( ) == 0 )
									cond_.wait( lock );
								result = *( queue_.begin( ) );
							}
						}

						cond_.notify_one( );
					}
				}

				result = frame_type::deep_copy( result );
			}
			
			return result;
		}

		virtual void on_slot_change( input_type_ptr input, int )
		{
			if ( !input )
				prop_active_.set( 0 );
		}

	private:
		reader_thread start_;
		boost::shared_ptr< pcos::observer > obs_active_;
		pcos::property prop_active_;
		bool running_;
		pcos::property prop_queue_;
		int last_position_;
		boost::mutex mutex_;
		boost::condition cond_;
		std::deque< frame_type_ptr > queue_;
		boost::thread *reader_;
};

// Frame rate filter
//
// Changes the frame rate to the requested value.
//
// Properties:
//
// 	fps_num = numerator [default: 25]
// 	fps_den = denominator [default: 1]
//

class ML_PLUGIN_DECLSPEC frame_rate_filter : public filter_type
{
	public:
		frame_rate_filter( )
			: filter_type( )
			, position_( 0 )
			, src_frames_( 0 )
			, src_frequency_( 0 )
			, src_fps_num_( -1 )
			, src_fps_den_( -1 )
			, prop_fps_num_( pcos::key::from_string( "fps_num" ) )
			, prop_fps_den_( pcos::key::from_string( "fps_den" ) )
			, reseat_( )
			, map_( )
		{
			properties( ).append( prop_fps_num_ = 25 );
			properties( ).append( prop_fps_den_ = 1 );
			reseat_ = create_audio_reseat( );
		}

		virtual int get_frames( ) const
		{
			return map_source_to_dest( src_frames_ );
		}

		virtual const opl::wstring get_uri( ) const { return L"frame_rate"; }

		virtual void seek( const int position, const bool relative = false )
		{
			int expected = position_ + 1;

			if ( relative )
				position_ += position;
			else
				position_ = position;

			if ( position_ < 0 )
				position_ = 0;

			if ( position_ != expected )
			{
				reseat_->clear( );
				map_.clear( );
			}
		}

		virtual int get_position( ) const
		{
			return position_;
		}

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			input_type_ptr input = fetch_slot( );

			if ( input )
			{
				src_frames_ = input->get_frames( );

				int requested = map_dest_to_source( position_ );
				int next = requested;

				const int fps_num = prop_fps_num_.value< int >( );
				const int fps_den = prop_fps_den_.value< int >( );

				if ( src_frequency_ == 0 || ( fps_num == src_fps_num_ && fps_den == src_fps_den_ ) )
				{
					input->seek( requested );
					result = input->fetch( );
					src_frames_ = input->get_frames( );
				}
				else
				{
					int samples = audio_samples_for_frame( position_, src_frequency_, fps_num, fps_den );

					while ( map_.find( next ) != map_.end( ) ) 
						next ++;

					while ( next < src_frames_ && ( !reseat_->has( samples ) || map_.find( requested ) == map_.end( ) ) )
					{
						input->seek( next );
						frame_type_ptr frame = input->fetch( );
						src_frames_ = input->get_frames( );
						if ( frame )
						{
							map_[ next ] = frame;
							reseat_->append( frame->get_audio( ) );
							next ++;
						}
						else
						{
							break;
						}
					}

					result = frame_type::deep_copy( map_.find( requested )->second );
					if ( result && reseat_->has( samples ) )
						result->set_audio( reseat_->retrieve( samples ) );

					std::map< int, frame_type_ptr >::iterator iter = map_.begin( );
					while( iter != map_.end( ) )
					{
						if ( iter->first < requested || iter->first > requested + 25 )
						{
							int index = iter->first;
							iter ++;
							map_.erase( index );
						}
						else
						{
							iter ++;
						}
					}
				}

				if ( result )
				{
					result->set_position( position_ );
					result->set_fps( fps_num, fps_den );
				}
			}

			return result;
		}

		virtual void on_slot_change( input_type_ptr input, int )
		{
			if ( input )
			{
				frame_type_ptr frame = input->fetch( );

				src_frames_ = input->get_frames( );
				src_fps_num_ = -1;
				src_fps_den_ = -1;
				src_frequency_ = 0;

				if ( frame )
				{
					src_fps_num_ = frame->get_fps_num( );
					src_fps_den_ = frame->get_fps_den( );
					if ( frame->get_audio( ) )
						src_frequency_ = frame->get_audio( )->frequency( );
				}
			}
		}

	private:
		inline int map_source_to_dest( int position ) const
		{
			return int( position * ( double( prop_fps_num_.value< int >( ) * src_fps_den_ ) / double( prop_fps_den_.value< int >( ) * src_fps_num_ ) ) );
		}

		inline int map_dest_to_source( int position ) const
		{
			return int( position * ( double( src_fps_num_ * prop_fps_den_.value< int >( ) ) / double( src_fps_den_ * prop_fps_num_.value< int >( ) ) ) );
		}

		int position_;
		int src_frames_;
		int src_frequency_;
		int src_fps_num_;
		int src_fps_den_;
		pcos::property prop_fps_num_;
		pcos::property prop_fps_den_;
		audio_reseat_ptr reseat_;
		std::map < int, frame_type_ptr > map_;
};

//
// Clip filter
//
// Clip the connected input to the specified in/out points.
//
// Properties:
//
// 	in = -n to n [default: 0]
// 		The first frame expressed in the source frame rate.
// 		Negative offsets are treated as relative to the length of the
// 		connected input.
//
// 	out = -n to n [default: inherited from connected input]
// 		The last frame expressed in the source frame rate.
// 		Negative offsets are treated as relative to the length of the
// 		connected input.
//
// Caveat: Properties should be set prior to the first fetch.

class ML_PLUGIN_DECLSPEC clip_filter : public filter_type
{
	public:
		clip_filter( )
			: filter_type( )
			, position_( 0 )
			, last_position_( -1 )
			, prop_in_( pcos::key::from_string( "in" ) )
			, prop_out_( pcos::key::from_string( "out" ) )
		{
			properties( ).append( prop_in_ = 0 );
			properties( ).append( prop_out_ = -1 );
		}

		virtual int get_frames( ) const
		{
			int frames = get_out( ) - get_in( );
			return frames < 0 ? - frames : frames;
		}

		virtual const opl::wstring get_uri( ) const { return L"clip"; }

		virtual void seek( const int position, const bool relative = false )
		{
			if( relative )
				position_ += position;
			else
				position_ = position;
		}

		virtual int get_position( ) const
		{
			return position_;
		}

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			input_type_ptr input = fetch_slot( );

			if ( input )
			{
				if ( get_in( ) < get_out( ) )
				{
					input->seek( get_in( ) + get_position( ) );
					result = input->fetch( );
				}
				else
				{
					input->seek( get_in( ) - get_position( ) );
					result = input->fetch( );
					if ( result && result->get_audio( ) )
						audio_reverse( result->get_audio( ) );
				}

				if ( result )
					result->set_position( get_position( ) );
			}
			
			return result;
		}

	private:
		inline int get_in( ) const
		{
			int in = prop_in_.value< int >( );
			input_type_ptr input = fetch_slot( );
			if ( input )
			{
				if ( in >= input->get_frames() )
					in = 0;
				if ( in < 0 )
					in = input->get_frames( ) + in;
				if ( in < 0 )
					in = 0;
			}
			return in;
		}

		inline int get_out( ) const
		{
			int out = prop_out_.value< int >( );
			input_type_ptr input = fetch_slot( );
			if ( input )
			{
				if ( out >= input->get_frames() )
					out = input->get_frames()-1;
				if ( out < 0 )
					out = input->get_frames( ) + out + 1;
				if ( out < 0 )
					out = 0;
			}
			return out;
		}

		int position_;
		int last_position_;
		pcos::property prop_in_;
		pcos::property prop_out_;
};

// Deinterlace
//
// Deinterlaces an image on demand.

class ML_PLUGIN_DECLSPEC deinterlace_filter : public filter_type
{
	public:
		deinterlace_filter( )
			: filter_type( )
			, prop_enable_( pcos::key::from_string( "enable" ) )
		{
			properties( ).append( prop_enable_ = 1 );
		}

		virtual const opl::wstring get_uri( ) const { return L"deinterlace"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( prop_enable_.value< int >( ) && result && result->get_image( ) )
			{
				result->set_image( il::conform( result->get_image( ), il::writable ) );
				result->set_image( il::deinterlace( result->get_image( ) ) );
			}
			
			return result;
		}

	private:
		pcos::property prop_enable_;
};

// Lerp
//
// Experimental linear interpolation

class ML_PLUGIN_DECLSPEC lerp_filter : public filter_type
{
	public:
		lerp_filter( )
			: filter_type( )
			, prop_in_( pcos::key::from_string( "in" ) )
			, prop_out_( pcos::key::from_string( "out" ) )
		{
			properties( ).append( prop_in_ = 0 );
			properties( ).append( prop_out_ = -1 );
		}

		virtual const opl::wstring get_uri( ) const { return L"lerp"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			input_type_ptr input = fetch_slot( );

			if ( input )
			{
				input->seek( get_position( ) );
				result = input->fetch( );
				pcos::key_vector props = properties( ).get_keys( );
				for( pcos::key_vector::iterator it = props.begin( ); result && it != props.end( ); it ++ )
					evaluate( result, *it );
			}
			
			return result;
		}

	protected:
		void correct_in_out( int &in, int &out )
		{
			if ( in < 0 )
				in = get_frames( ) + in;
			if ( in < 0 )
				in = 0;
			if ( in >= get_frames( ) )
				in = get_frames( ) - 1;

			if ( out < 0 )
				out = get_frames( ) + out;
			if ( out <= 0 )
				out = 0;
			if ( out >= get_frames( ) )
				out = get_frames( ) - 1;
		}

		void assign_property( frame_type_ptr frame, std::string name, double result )
		{
			if ( name.find( ":" ) != std::string::npos )
				name = name.substr( 0, name.find( ":" ) );

			if ( !frame->properties( ).get_property_with_string( name.c_str( ) ).valid( ) )
			{
				pcos::property final( pcos::key::from_string( name.c_str( ) ) );
				frame->properties( ).append( final = result );
			}
			else
			{
				frame->properties( ).get_property_with_string( name.c_str( ) ) = result;
			}
		}

		virtual void evaluate( frame_type_ptr frame, pcos::key &key )
		{
			std::string name( key.as_string( ) );

			if ( name.substr( 0, 2 ) == "@@" && get_frames( ) )
			{
				pcos::property prop = properties( ).get_property_with_key( key );
				opl::wstring value = prop.value< opl::wstring >( );

				double lower, upper, result;
				int in = prop_in_.value< int >( );
				int out = prop_out_.value< int >( );
				int position = get_position( ) % get_frames( );

				int count = sscanf( opl::to_string( value ).c_str( ), "%lf:%lf:%d:%d", &lower, &upper, &in, &out );

				correct_in_out( in, out );

				if ( count > 0 && position >= in && position <= out )
				{
					if ( count >= 2 )
						result = lower + ( double( position - in ) / double( out - in + 1 ) ) * ( upper - lower );
					else
						result = double( lower );

					assign_property( frame, name.substr( 2 ), result );
				}
			}
		}

		pcos::property prop_in_;
		pcos::property prop_out_;
};

// Bezier
//
// Experimental Bezier interpolation
//
// Horrifically inefficient. Some code originating from:
//
// http://en.wikipedia.org/wiki/B%C3%A9zier_curve

class ML_PLUGIN_DECLSPEC bezier_filter : public lerp_filter
{
	public:
		bezier_filter( ) : lerp_filter( ) { }
		virtual const opl::wstring get_uri( ) const { return L"bezier"; }

	protected:
		typedef struct
		{
    		double x;
    		double y;
		}
		point;

		void calculate( double &x, double &y, point points[ 4 ], double t )
		{
    		double ax, bx, cx;
    		double ay, by, cy;
    		double t_squared, t_cubed;

    		// calculate the polynomial coefficients
    		cx = 3.0 * ( points[ 1 ].x - points[ 0 ].x );
    		bx = 3.0 * ( points[ 2 ].x - points[ 1 ].x ) - cx;
    		ax = points[ 3 ].x - points[ 0 ].x - cx - bx;
        
    		cy = 3.0 * ( points[ 1 ].y - points[ 0 ].y );
    		by = 3.0 * ( points[ 2 ].y - points[ 1 ].y ) - cy;
    		ay = points[ 3 ].y - points[ 0 ].y - cy - by;
        
    		// calculate the curve point at parameter value t
    		t_squared = t * t;
    		t_cubed = t_squared * t;
        
    		x = ( ax * t_cubed ) + ( bx * t_squared ) + ( cx * t ) + points[ 0 ].x;
    		y = ( ay * t_cubed ) + ( by * t_squared ) + ( cy * t ) + points[ 0 ].y;
		}

		void evaluate( frame_type_ptr frame, pcos::key &key )
		{
			std::string name( key.as_string( ) );

			if ( name.substr( 0, 2 ) == "@@" && get_frames( ) )
			{
				int position = get_position( ) % get_frames( );
				pcos::property prop = properties( ).get_property_with_key( key );
				opl::wstring value = prop.value< opl::wstring >( );

				point points[ 4 ];
				int in = prop_in_.value< int >( );
				int out = prop_out_.value< int >( );

				int count = sscanf( opl::to_string( value ).c_str( ), "%lf,%lf:%lf,%lf:%lf,%lf:%lf,%lf:%d:%d", 
																	  &points[ 0 ].x, &points[ 0 ].y,
																	  &points[ 1 ].x, &points[ 1 ].y,
																	  &points[ 2 ].x, &points[ 2 ].y,
																	  &points[ 3 ].x, &points[ 3 ].y,
																	  &in, &out );

				correct_in_out( in, out );

				double x = 0.0;
				double y = 0.0;

				if ( count > 0 && position >= in && position <= out )
				{
					calculate( x, y, points, double( position - in ) / double( out - in ) );

					size_t index = name.rfind( "," );
					if ( index !=  std::string::npos )
					{
						assign_property( frame, name.substr( 2, index - 2 ), x );
						assign_property( frame, name.substr( index + 1 ), y );
					}
					else
					{
						assign_property( frame, name.substr( 2 ), x );
					}
				}
			}
		}
};

// Wave Visualisation
//
// Provides a wave visualisation of the audio.
//
// Expects a single input.
//
// Properties:
//
//	force = 0 [default], 1 or 2
//		if 0: only provide visualisation when no image exists
//		if 1: destroy existing image and provide a new one
//		if 2: overlay visualisation on the existing image (needs geometry)
//
//	width, height = 640, 480 [default]
//		dimensions to generate image with (ignored in overlay case)
//
//	colourspace = "r8g8b8" [default] or "yuv420p"
//		colourspace to generate (only used in overlay case if input is unsupported)
//
//	type = 0 [default]
//		reserved for future multiple visualisation type (wave form only for now)
//
// TODO: Geometry of overlay
//
// TODO: Better visualisations

class ML_PLUGIN_DECLSPEC visualise_filter : public filter_type
{
	public:
		visualise_filter( )
			: filter_type( )
			, prop_force_( pcos::key::from_string( "force" ) )
			, prop_width_( pcos::key::from_string( "width" ) )
			, prop_height_( pcos::key::from_string( "height" ) )
			, prop_sar_num_(  pcos::key::from_string( "sar_num" ) )
			, prop_sar_den_(  pcos::key::from_string( "sar_den" ) )
			, prop_type_( pcos::key::from_string( "type" ) )
			, prop_colourspace_( pcos::key::from_string( "colourspace" ) )
			, previous_( )
		{
			properties( ).append( prop_force_ = 0 );
			properties( ).append( prop_width_ = 640 );
			properties( ).append( prop_height_ = 480 );
			properties( ).append( prop_sar_num_ = 1 );
			properties( ).append( prop_sar_den_ = 1 );
			properties( ).append( prop_type_ = 0 );
			properties( ).append( prop_colourspace_ = opl::wstring( L"r8g8b8" ) );
		}

		virtual const opl::wstring get_uri( ) const { return L"visualise"; }

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result = fetch_from_slot( );

			if ( result && ( ( previous_ == 0 && result->get_image( ) == 0 ) || prop_force_.value< int >( ) ) )
				visualise( result );
			else if ( result && result->get_image( ) == 0 )
				result->set_image( previous_ );
			else if ( result )
				previous_ = result->get_image( );

			return result;
		}

	private:
		void visualise( frame_type_ptr frame )
		{
			if ( frame->get_audio( ) != 0 )
			{
				int type = prop_type_.value< int >( );
				opl::wstring colourspace = prop_colourspace_.value< opl::wstring >( );
				int width = prop_width_.value< int >( );
				int height = prop_height_.value< int >( );
				image_type_ptr image = frame->get_image( );

				if ( image == 0 || prop_force_.value< int >( ) == 1 )
				{
					if ( colourspace == L"yuv420p" )
					{
						int by, bu, bv;
						il::rgb24_to_yuv444( by, bu, bv, 0, 0, 0 );
						image = il::allocate( L"yuv420p", width, height );
						fill( image, 0, by );
						fill( image, 1, bu );
						fill( image, 2, bv );
					}
					else
					{
						image = il::allocate( L"r8g8b8", width, height );
						memset( image->data( ), 0, image->size( ) );
					}

					frame->set_image( image );
					frame->set_sar( prop_sar_num_.value< int >( ), prop_sar_den_.value< int >( ) );
				}
				else
				{
					if ( image->pf( ) == L"yuv420p" || image->pf( ) == L"r8g8b8" )
						colourspace = image->pf( );
					else
						frame_convert( frame, colourspace.c_str() );
				}

				if ( type == 0 && colourspace == L"r8g8b8" )
					wave_rgb( frame );
				else if ( type == 0 && colourspace == L"yuv420p" )
					wave_yuv( frame );
				else
					wave_rgb( frame );
			}
		}

		void wave_rgb( frame_type_ptr frame )
		{
			frame->set_image( il::conform( frame->get_image( ), il::writable ) );

			audio_type_ptr audio = frame->get_audio( );
			image_type_ptr image = frame->get_image( );

			int width = image->width( );
			int height = image->height( );

			short *buff = ( short * )audio->data( );
			int samples = audio->samples( );
			int channels = audio->channels( );

			double sample_offset = double( samples ) / width;
			int pitch = image->pitch( );
			unsigned char *middle = image->data( ) + pitch * image->height( ) / 2;

			short sample;
			unsigned char *p;
			int i, j;
			int offset;

			for ( i = 0; i < width; i ++ )
			{
				offset = i * 3;
				for ( j = 0; j < channels; j ++ )
				{
					sample = int( ( double( ( buff + int( sample_offset * i ) * channels )[ j ] ) / 32767.0 ) * ( height / 2 ) );
					p = middle - sample * pitch + offset;
					*p ++ = j == 0 ? 255 : 0;
					*p ++ = j == 0 ? 0 : 255;
					*p ++ = 0;
				}
			}
		}

		void line( unsigned char *p, unsigned char *m, int pitch, unsigned char v )
		{
			if ( p < m )
			{
				while( p < m + pitch )
				{
					*p = 128 + ( *p + v - 256 ) / 2;
					p += pitch;
				}
			}
			else if ( p > m + pitch )
			{
				while( p > m )
				{
					*p = 128 + ( *p + v - 256 ) / 2;
					p -= pitch;
				}
			}
		}

		void wave_yuv( frame_type_ptr frame )
		{
			frame->set_image( il::conform( frame->get_image( ), il::writable ) );

			int ry, ru, rv;
			il::rgb24_to_yuv444( ry, ru, rv, 255, 0, 0 );

			int gy, gu, gv;
			il::rgb24_to_yuv444( gy, gu, gv, 255, 255, 255 );

			audio_type_ptr audio = frame->get_audio( );
			image_type_ptr image = frame->get_image( );
			int width = image->width( );
			int height = image->height( );

			short *buff = ( short * )audio->data( );
			int samples = audio->samples( );
			int channels = audio->channels( );

			double sample_offset = double( samples ) / width;
			int pitch_y = image->pitch( 0 );
			int pitch_u = image->pitch( 1 );
			int pitch_v = image->pitch( 2 );
			unsigned char *middle_y = image->data( 0 ) + pitch_y * image->height( ) / 2;
			unsigned char *middle_u = image->data( 1 ) + pitch_u * image->height( ) / 4;
			unsigned char *middle_v = image->data( 2 ) + pitch_v * image->height( ) / 4;

			short sample;
			unsigned char *p;
			int i, j;

			for ( i = 0; i < width; i ++ )
			{
				for ( j = 0; j < channels; j ++ )
				{
					sample = int( ( double( ( buff + int( sample_offset * i ) * channels )[ j ] ) / 32767.0 ) * ( height / 2 ) );
					p = middle_y - sample * pitch_y + i;
					line( p, middle_y, pitch_y, j == 0 ? ry : gy );
					sample /= 2;
					p = middle_u - sample * pitch_u + i / 2;
					line( p, middle_u, pitch_u, j == 0 ? ru : gu );
					p = middle_v - sample * pitch_v + i / 2;
					line( p, middle_v, pitch_v, j == 0 ? rv : gv );
				}
			}
		}

	private:
		pcos::property prop_force_;
		pcos::property prop_width_;
		pcos::property prop_height_;
		pcos::property prop_sar_num_;
		pcos::property prop_sar_den_;
		pcos::property prop_type_;
		pcos::property prop_colourspace_;
		image_type_ptr previous_;
};

// Playlist filter
//
// Experimental n-ary filter - the number of possibly connected slots is
// defined by the 'slots' property. The number of frames reported by the filter
// is defined as the sum of the frames in all the connected slots. 
// 
// It is not this filters responsibility to ensure that all connected slots 
// have the same frame rate/sample information - that is left as an exercise
// for the filter graph builder to deal with.
//
// Properties:
//
// 	slots = n (default: 2)
// 		The maximum number of connected slots

class ML_PLUGIN_DECLSPEC playlist_filter : public filter_type
{
	public:
		playlist_filter( )
			: filter_type( )
			, prop_slots_( pcos::key::from_string( "slots" ) )
		{
			properties( ).append( prop_slots_ = 2 );
		}

		virtual const size_t slot_count( ) const { return size_t( prop_slots_.value< int >( ) ); }

		virtual const opl::wstring get_uri( ) const { return L"playlist"; }

		virtual int get_frames( ) const 
		{
			int result = 0;
			for ( size_t i = 0; i < slot_count( ); i ++ )
				if ( fetch_slot( i ) )
					result += fetch_slot( i )->get_frames( );
			return result;
		}

		virtual frame_type_ptr fetch( )
		{
			acquire_values( );

			frame_type_ptr result;
			size_t slot = slot_for_position( get_position( ) );
			input_type_ptr input = fetch_slot( slot );

			if ( input )
			{
				input->seek( get_position( ) - slot_offset( slot ) );
				result = input->fetch( );
				if ( result )
					result->set_position( get_position( ) );
			}
			
			return result;
		}

	private:
		size_t slot_for_position( int position )
		{
			size_t result = 0;
			for ( size_t i = 0; i < slot_count( ); i ++ )
			{
				if ( fetch_slot( i ) )
				{
					result = i;
					if ( position < fetch_slot( i )->get_frames( ) )
						break;
					position -= fetch_slot( i )->get_frames( );
				}
			}
			return result;
		}

		int slot_offset( size_t slot )
		{
			int result = 0;
			for ( size_t i = 0; i < slot; i ++ )
			{
				if ( fetch_slot( i ) )
					result += fetch_slot( i )->get_frames( );
			}
			return result;
		}

		pcos::property prop_slots_;
};

//
// Plugin object
//

class ML_PLUGIN_DECLSPEC gensys_plugin : public openmedialib_plugin
{
public:
	virtual input_type_ptr input( const opl::wstring &request )
	{
		if ( request == L"pusher:" )
			return input_type_ptr( new pusher_input( ) );
		else
			return input_type_ptr( new colour_input( ) );
	}

	virtual filter_type_ptr filter( const opl::wstring &request )
	{
		if ( request == L"chroma" )
			return filter_type_ptr( new chroma_filter( ) );
		else if ( request == L"clip" )
			return filter_type_ptr( new clip_filter( ) );
		else if ( request == L"composite" )
			return filter_type_ptr( new composite_filter( ) );
		else if ( request == L"conform" )
			return filter_type_ptr( new conform_filter( ) );
		else if ( request == L"correction" )
			return filter_type_ptr( new correction_filter( ) );
		else if ( request == L"crop" )
			return filter_type_ptr( new crop_filter( ) );
		else if ( request == L"deinterlace" )
			return filter_type_ptr( new deinterlace_filter( ) );
		else if ( request == L"frame_rate" )
			return filter_type_ptr( new frame_rate_filter( ) );
		else if ( request == L"lerp" )
			return filter_type_ptr( new lerp_filter( ) );
		else if ( request == L"bezier" )
			return filter_type_ptr( new bezier_filter( ) );
		else if ( request == L"playlist" )
			return filter_type_ptr( new playlist_filter( ) );
		else if ( request == L"temporal" )
			return filter_type_ptr( new clip_filter( ) );
		else if ( request == L"threader" )
			return filter_type_ptr( new threader_filter( ) );
		else if ( request == L"visualise" )
			return filter_type_ptr( new visualise_filter( ) );
		return filter_type_ptr( );
	}
};

} } }

//
// Access methods for openpluginlib
//

extern "C"
{
	ML_PLUGIN_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}
	
	ML_PLUGIN_DECLSPEC bool openplugin_create_plugin( const char*, opl::openplugin** plug )
	{
		*plug = new plugin::gensys_plugin;
		return true;
	}
	
	ML_PLUGIN_DECLSPEC void openplugin_destroy_plugin( opl::openplugin* plug )
	{ 
		delete static_cast< plugin::gensys_plugin * >( plug ); 
	}
}

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

