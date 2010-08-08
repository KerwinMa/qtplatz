//////////////////////////////////////////
// Copyright (C) 2010 Toshinobu Hondo, Ph.D.
// Science Liaison / Advanced Instrumentation Project
//////////////////////////////////////////

#import <msxml4.dll> named_guids /* , raw_interfaces_only */
#include "xmldom.h"

static std::wstring __string_empty;

using namespace std;
using namespace xmlwrapper;
using namespace xmlwrapper::msxml;

void
intrusive_ptr_add_ref( xmldomnode_type * ptr )
{
   ptr->AddRef();
}

void
intrusive_ptr_release( xmldomnode_type * ptr )
{
    ptr->Release();
}

void
intrusive_ptr_add_ref( xmldomnodelist_type * ptr )
{
   ptr->AddRef();
}

void
intrusive_ptr_release( xmldomnodelist_type * ptr )
{
   ptr->Release();
}

XMLNode::~XMLNode()
{
}

XMLNode::XMLNode()
{
}

XMLNode::XMLNode( xmldomnode_type * p ) : pImpl_( p )
{

}

XMLNode::XMLNode( IDispatch * p )
{
    HRESULT hr;
    if ( p )
        hr = p->QueryInterface(MSXML2::IID_IXMLDOMNode, reinterpret_cast<void **>(&pImpl_));
    (void)hr;
}

XMLNode::XMLNode(const XMLNode & t) : pImpl_( t.pImpl_ )
{
}

XMLNode::operator bool () const
{
   return pImpl_ == 0 ? false : true;
}

XMLNode::operator xmldomnode_type * ()
{
   return pImpl_.get();  // no addref
}

std::wstring
XMLNode::nodeName() const
{
    if ( pImpl_ )
        return std::wstring ( pImpl_->nodeName );
    return __string_empty;
}

std::wstring
XMLNode::textValue() const
{
    if ( pImpl_ )
        return std::wstring( pImpl_->text );
    return __string_empty;
}

void
XMLNode::textValue(const std::wstring& text)
{
   if ( pImpl_ )
      pImpl_->text = text.c_str();
}

const XMLAttr
XMLNode::attributeNode( const std::wstring& attr ) const
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMNamedNodeMapPtr piMap = pImpl_->attributes;
      if ( piMap ) 
         return XMLAttr( piMap->getNamedItem( attr.c_str() ) );
   }
   return XMLAttr(); // empty attr
}

std::wstring
XMLNode::attribute( const std::wstring& attr ) const
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMNamedNodeMapPtr piMap = pImpl_->attributes;
      if ( piMap ) {
         MSXML2::IXMLDOMNodePtr pinode = piMap->getNamedItem( attr.c_str() );
         if ( pinode )
            return std::wstring( pinode->text );
      }
   }
   return __string_empty;
}

XMLNodeList
XMLNode::childNodes() const
{
   if ( pImpl_ )
      return XMLNodeList ( pImpl_->childNodes );
   return XMLNodeList();
}

XMLNode
XMLNode::appendChild( XMLNode& child )
{
   if ( pImpl_ )
      return XMLNode ( pImpl_->appendChild ( child ) );
   return XMLNode();
}

XMLNode
XMLNode::removeChild( XMLNode& child )
{
   if ( pImpl_ )
      return XMLNode ( pImpl_->removeChild ( child ) );
   return XMLNode();
}

XMLNodeList
XMLNode::selectNodes(const std::wstring& query ) const
{
   if ( pImpl_ )
      return XMLNodeList ( pImpl_->selectNodes( query.c_str() ) );
   return XMLNodeList();
}

XMLNode
XMLNode::selectSingleNode(const std::wstring& query) const
{
   if ( pImpl_ )
      return XMLNode ( pImpl_->selectSingleNode( query.c_str() ) );
   return XMLNode();
}

XMLNode
XMLNode::cloneNode(bool deep) const
{
   if ( pImpl_ )
      return XMLNode ( pImpl_->cloneNode( deep ? VARIANT_TRUE : VARIANT_FALSE ) );
   return XMLNode();
}

void 
XMLNode::appendText( msxml::XMLDocument& dom, XMLNode& node, const std::wstring& text )
{
	XMLText text_node ( dom.createTextNode( text ) );
   node.appendChild( text_node );
}

std::wstring
XMLNode::transformNode( XMLNode& stylesheet )
{
    if ( pImpl_ ) 
        return std::wstring( pImpl_->transformNode( stylesheet ) );
    return L"";
}

////////////////////////////////////////////////////////////////////////////

XMLNodeList::~XMLNodeList()
{
}

XMLNodeList::XMLNodeList()
{
}

XMLNodeList::XMLNodeList( xmldomnodelist_type * p ) : pImpl_(p)
{
}

size_t
XMLNodeList::size() const
{
  if ( pImpl_ )
    return pImpl_->length;
  return 0;
}

XMLNode
XMLNodeList::operator [] (int idx) const
{
   if ( pImpl_ )
		return XMLNode( pImpl_->item[idx] );
   return XMLNode();
}

////////////////////////////////////////////////////////////////////////////

msxml::XMLDocument::~XMLDocument()
{
}

msxml::XMLDocument::XMLDocument()
{
   MSXML2::IXMLDOMDocument2Ptr pidoc2;

   HRESULT hr = CoCreateInstance( MSXML2::CLSID_FreeThreadedDOMDocument40
                                , NULL
                                , CLSCTX_INPROC_SERVER
                                , MSXML2::IID_IXMLDOMDocument2
                                , reinterpret_cast<void**>( &pidoc2 ));
   (void)hr;
   if ( pidoc2 )
       pidoc2->setProperty(_bstr_t(L"SelectionLanguage"), _variant_t("XPath"));
   pImpl_ = pidoc2;
}

msxml::XMLDocument::XMLDocument( const XMLDocument& t ) : XMLNode( t )
{
	MSXML2::IXMLDOMDocument2Ptr pidoc2 = pImpl_.get();
	if ( pidoc2 )
		pidoc2->setProperty(_bstr_t(L"SelectionLanguage"), _variant_t("XPath"));
	else 
		pImpl_ = 0;
}

msxml::XMLDocument::XMLDocument( IDispatch * p )
{
	MSXML2::IXMLDOMDocument2Ptr pidoc2 = p;
	if ( pidoc2 ) {
		pidoc2->setProperty(_bstr_t(L"SelectionLanguage"), _variant_t("XPath"));
		pImpl_ = pidoc2;
	}
}

bool
msxml::XMLDocument::save( const std::wstring& filename ) const
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      if ( pidoc )
			return pidoc->save( filename.c_str() ) == S_OK ? true : false;
   }
   return false;
}

bool
msxml::XMLDocument::load( const std::wstring& filename )
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      if ( pidoc )
			return pidoc->load( filename.c_str() ) ? true : false;
   }
   return false;
}


bool
msxml::XMLDocument::loadXML( const std::wstring& xml )
{
   MSXML2::IXMLDOMDocumentPtr pdom = pImpl_.get();
	if ( pdom ) {
		if ( pdom->loadXML( _bstr_t( xml.c_str() ) ) )
			return true;
	}
	return false;
}

bool
msxml::XMLDocument::xml( std::wstring& xml ) const
{
   MSXML2::IXMLDOMDocumentPtr pdom = pImpl_.get();
   if ( pdom ) {
      xml = pdom->xml;
      return true;
   }
   return false;
}

XMLProcessingInstruction
msxml::XMLDocument::createProcessingInstruction( const std::wstring& target, const std::wstring& data )
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocument2Ptr pidoc = pImpl_.get();
      return XMLProcessingInstruction(
         pidoc->createProcessingInstruction( target.c_str(), data.c_str() ) );
   }
   return XMLProcessingInstruction();
}

XMLComment
msxml::XMLDocument::createComment( const std::wstring& text )
{
	if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      return XMLComment( pidoc->createComment( text.c_str() ) );
   }
   return XMLComment();
}

XMLElement
msxml::XMLDocument::documentElement()
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      if ( pidoc )
         return XMLElement ( pidoc->documentElement );
   }
   return XMLElement();
}

XMLElement
msxml::XMLDocument::createElement( const std::wstring& tag )
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      if ( pidoc )
         return XMLElement ( pidoc->createElement( tag.c_str() ) );
   }
   return XMLElement();
}

XMLText
msxml::XMLDocument::createTextNode( const std::wstring& data )
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMDocumentPtr pidoc = pImpl_.get();
      if ( pidoc )
         return XMLText ( pidoc->createTextNode( data.c_str() ) );
   }
   return XMLText();
}

XMLNode
msxml::XMLDocument::importNode(XMLNode& node, bool deep)
{
    (void)deep;
   // this is workaround since MSXML4 does not has 'importNode'.
   return node.cloneNode(true);
}

///////////////

msxml::XMLElement::~XMLElement()
{
}

msxml::XMLElement::XMLElement()
{
}

msxml::XMLElement::XMLElement( const XMLNode& t ) : XMLNode( t )
{
}

msxml::XMLElement::XMLElement( xmldomnode_type * p ) : XMLNode( p )
{
}

void
msxml::XMLElement::setAttribute(const std::wstring& attr, const std::wstring& value)
{
   if ( pImpl_ ) {
      MSXML2::IXMLDOMElementPtr piElement = pImpl_.get();
      if ( piElement )
         piElement->setAttribute( attr.c_str(), value.c_str() );
   }
}

///////////////

msxml::XMLAttr::~XMLAttr()
{
}

msxml::XMLAttr::XMLAttr()
{
}

msxml::XMLAttr::XMLAttr( const XMLNode& t ) : XMLNode( t )
{
}

msxml::XMLAttr::XMLAttr( xmldomnode_type * p ) : XMLNode( p )
{
}
/////////////////////

msxml::XMLProcessingInstruction::~XMLProcessingInstruction()
{
}

msxml::XMLProcessingInstruction::XMLProcessingInstruction()
{
}

msxml::XMLProcessingInstruction::XMLProcessingInstruction( const XMLNode& t ) : XMLNode( t )
{
}

msxml::XMLProcessingInstruction::XMLProcessingInstruction( xmldomnode_type * p ) : XMLNode( p )
{
}

/////////////////////

msxml::XMLText::~XMLText()
{
}

msxml::XMLText::XMLText()
{
}

msxml::XMLText::XMLText( const XMLNode& t ) : XMLNode( t )
{
}

msxml::XMLText::XMLText( xmldomnode_type * p ) : XMLNode( p )
{
}

//////////////////////////////

msxml::XMLComment::~XMLComment()
{
}

msxml::XMLComment::XMLComment()
{
}

msxml::XMLComment::XMLComment( xmldomnode_type * p ) : XMLNode( p )
{
}