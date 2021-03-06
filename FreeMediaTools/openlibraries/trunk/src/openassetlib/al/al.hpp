// al.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef AL_INC_
#define AL_INC_

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openpluginlib/pl/string.hpp>
#include <openimagelib/il/il.hpp>
#include <openimagelib/il/openimagelib_plugin.hpp>
#include <openmedialib/ml/ml.hpp>
#include <openmedialib/ml/openmedialib_plugin.hpp>
#include <openobjectlib/sg/sg.hpp>
#include <openobjectlib/sg/openobjectlib_plugin.hpp>
#include <openassetlib/al/config.hpp>
#include <openassetlib/al/al_plugin.hpp>

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4251 )
#endif

namespace opl = olib::openpluginlib;
namespace oil = olib::openimagelib;
namespace oml = olib::openmedialib;
namespace ool = olib::openobjectlib;

namespace olib { namespace openassetlib {

	AL_DECLSPEC bool				init(guid_generator_ptr guid_gen);
	AL_DECLSPEC bool				uninit();
	AL_DECLSPEC const al::guid_ptr	generate_guid();


	namespace al {

		//namespace
		//{
			struct query_traits
			{
			  query_traits( const opl::wstring& filename );

			  opl::wstring libname( ) const;

			  opl::wstring to_match( ) const;

			  opl::wstring type( ) const;

			  int merit( ) const;

				opl::wstring filename_;
			};

			struct oil_query_traits
			{
			  oil_query_traits( const opl::wstring& filename );
			  opl::wstring libname( ) const;

			  opl::wstring to_match( ) const;

			  opl::wstring type( ) const;

			  int merit( ) const;

				opl::wstring filename_;
			};

			struct oml_query_traits
			{
			  oml_query_traits( const opl::wstring& filename );

			  opl::wstring libname( ) const;

			  opl::wstring to_match( ) const;

			  opl::wstring type( ) const;

			  int merit( ) const;

				opl::wstring filename_;
			};

			struct ool_query_traits
			{
			  ool_query_traits( const opl::wstring& filename );
			  opl::wstring libname( ) const;

			  opl::wstring to_match( ) const;

			  opl::wstring type( ) const;

			  int merit( ) const;

				opl::wstring filename_;
			};

		struct AL_DECLSPEC predicate : public std::unary_function<al::asset_ptr, bool>
		{
			virtual ~predicate() {};
			virtual bool operator()(const al::asset_ptr& asset) const = 0;
		};

			struct set_search_predicate
			{

			  set_search_predicate(const predicate& pred);
			  ~set_search_predicate();
			  void operator()(const asset_ptr& asset);


			  set_ptr result();
			private:
				const predicate&	predicate_;
				set_ptr				result_set_;
			};



		/////////////////////////////////////
		// Filesystem plugin helper functions
		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse);
		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize);
		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& datetime_comparitor, const std::tm& comparison_datetime);
		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize, const opl::string& datetime_comparitor,  const std::tm& comparison_datetime);

		///////////////////////////////////
		// Database plugin helper functions
		// stores/restores assets with metadata to the database supplied
		AL_DECLSPEC set_ptr store(const al_plugin_ptr& database_plugin, const set_ptr& set);
		AL_DECLSPEC set_ptr restore(const al_plugin_ptr& database_plugin, const set_ptr& set);

		// Saves/loads set information (asset guids) to xml file and stores/restores asset metadata from database supplied
		AL_DECLSPEC bool	save_set(const al_plugin_ptr& database_plugin, const opl::string& xmlfile, const set_ptr& set);
		AL_DECLSPEC set_ptr	load_set(const al_plugin_ptr& database_plugin, const opl::string& xmlfile);

		///////////////
		// Set searches


		class AL_DECLSPEC key_value_regex_search_predicate : public predicate
		{
		public:
			key_value_regex_search_predicate(const opl::string& key_regex, const opl::string& value_regex);
			~key_value_regex_search_predicate();
			
			bool operator()(const asset_ptr& asset) const;

		private:
			key_value_regex_search_predicate& operator=( const key_value_regex_search_predicate& );

			const opl::string	key_regex_;
			const opl::string	value_regex_;
		};

		class AL_DECLSPEC location_filename_regex_predicate : public predicate
		{
		public:
			location_filename_regex_predicate(const opl::string& location_regex, const opl::string& filename_regex);
			~location_filename_regex_predicate();

			bool operator()(const asset_ptr& asset) const;

		private:
			location_filename_regex_predicate& operator=( const location_filename_regex_predicate& );

			const opl::string location_regex_;
			const opl::string filename_regex_;
		};

		AL_DECLSPEC set_ptr search(const set_ptr& set, const predicate& pred);

		/////////////////////////////////////////////////////
		// Helper functions to convert assets to olib objects
		AL_DECLSPEC bool                    is_convertible(asset_ptr asset);
		AL_DECLSPEC oil::il::image_type_ptr convert_to_image(asset_ptr);
		AL_DECLSPEC oml::ml::input_type_ptr convert_to_media(asset_ptr);
		AL_DECLSPEC ool::sg::scene_ptr      convert_to_object(asset_ptr);

		/////////////////////////////////////////////////////////////////////////////
		// Helper functions to serialize/deserialize image_type data to/from metadata
		AL_DECLSPEC bool					store_image(asset_ptr asset, oil::il::image_type_ptr image, const opl::string& image_property_name_prefix);
		AL_DECLSPEC oil::il::image_type_ptr restore_image(asset_ptr asset, const opl::string& image_property_name_prefix);

		////////////////////////////////////////////////////////////////////
		// Helper functions to calulate set unions/intersections/differences
		AL_DECLSPEC set_ptr calculate_union(const set_ptr& set1, const set_ptr& set2);
		AL_DECLSPEC set_ptr calculate_intersection(const set_ptr& set1, const set_ptr& set2);
		AL_DECLSPEC set_ptr calculate_difference(const set_ptr& set1, const set_ptr& set2);
	}

} }

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#endif
