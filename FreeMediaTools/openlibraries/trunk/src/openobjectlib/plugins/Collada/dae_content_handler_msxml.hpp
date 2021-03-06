
// sg - A scene graph representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DAE_CONTENT_HANDLER_MSXML_INC_
#define DAE_CONTENT_HANDLER_MSXML_INC_

#ifdef WANT_PAIN_USE_WINDOWS

#import <msxml4.dll> raw_interfaces_only

#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/plugins/Collada/actions/dae_parser_action.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace dae {

//
class dae_content_handler_msxml : public MSXML2::ISAXContentHandler
{
public:
    explicit dae_content_handler_msxml( );
    virtual ~dae_content_handler_msxml( );

    long            __stdcall QueryInterface( const struct _GUID&, void** );
    unsigned long   __stdcall AddRef( void );
    unsigned long   __stdcall Release( void );

    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( MSXML2::ISAXLocator __RPC_FAR* locator );

    virtual HRESULT STDMETHODCALLTYPE startDocument( void );
    virtual HRESULT STDMETHODCALLTYPE endDocument( void );

    virtual HRESULT STDMETHODCALLTYPE startPrefixMapping( unsigned short __RPC_FAR* pwchPrefix,
                                                          int cchPrefix,
                                                          unsigned short __RPC_FAR* pwchUri,
                                                          int cchUri );

    virtual HRESULT STDMETHODCALLTYPE endPrefixMapping( unsigned short __RPC_FAR* pwchPrefix,
                                                        int cchPrefix );

    virtual HRESULT STDMETHODCALLTYPE startElement( unsigned short __RPC_FAR* pwchNamespaceUri,
                                                    int cchNamespaceUri,
                                                    unsigned short __RPC_FAR* pwchLocalName,
                                                    int cchLocalName,
                                                    unsigned short __RPC_FAR* pwchRawName,
                                                    int cchRawName,
                                                    MSXML2::ISAXAttributes __RPC_FAR* pAttributes );

    virtual HRESULT STDMETHODCALLTYPE endElement( unsigned short __RPC_FAR* pwchNamespaceUri,
                                                  int cchNamespaceUri,
                                                  unsigned short __RPC_FAR* pwchLocalName,
                                                  int cchLocalName,
                                                  unsigned short __RPC_FAR* pwchRawName,
                                                  int cchRawName );

    virtual HRESULT STDMETHODCALLTYPE characters( unsigned short __RPC_FAR* pwchChars, int cchChars );
    virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace( unsigned short __RPC_FAR* pwchChars, int cchChars );
    virtual HRESULT STDMETHODCALLTYPE processingInstruction( unsigned short __RPC_FAR* pwchTarget,
                                                             int cchTarget,
                                                             unsigned short __RPC_FAR* pwchData,
                                                             int cchData );

    virtual HRESULT STDMETHODCALLTYPE skippedEntity( unsigned short __RPC_FAR* pwchName, int cchName );

    void            set_base_url( const boost::filesystem::path& base_url );
    sg::scene_ptr   root( );

private:
    sg::actions::dae_parser_action action_;
    ULONG refcnt_;
    openpluginlib::wstring characters_;
};

inline void dae_content_handler_msxml::set_base_url( const boost::filesystem::path& base_url )
{ action_.set_base_url( base_url ); }

} } } }

#endif
#endif
