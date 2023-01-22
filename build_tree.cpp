#include "build_tree.h"
#include "node.h"
#include "html_object.h"
#include "object_root.h"
#include "object_body.h"
#include "object_text.h"
#include "object_layout.h"
#include "object_table.h"
#include "object_row.h"
#include "object_td.h"
#include "object_ul.h"
#include "object_image.h"
#include "object_a.h"

namespace {

FXString text_section( const FXString & str, const FXString & del, int start )
{
    FXString s;

    for( int i = start; i < str.length(); ++i )
	for( int j = 0; j < del.length(); ++j )
	    if( str[i] == del[j] )
	    {
		for( int k = start; k < i; ++k )
		    s += str[k];

		return s;
	    }

    for( int k = start; k < str.length(); ++k )
	s += str[k];

    return s;
}
    
void build_object_tree( Ref<Node> node, Ref<HTMLObject> obj, ObjectRoot *root )
{
    switch( node->getType() )
    {
	case Node::TYPE_TEXT:
	{
	    FXString text = node->getValue();

	    text.trim();
	    
//	    if( text.length() )
//		obj->addChield( new ObjectText( obj, node ) );

	    FXString delimiters( " \n\t" );

	    for( int start = 0; start < text.length();  )
	    {
		FXString str = text_section( text, delimiters, start );

		if( str.length() )
		{
		    obj->addChield( new ObjectText( obj, str ) );
		} else {
		    start++;
		}

		start += str.length();
	    }

	    break;
	}

	case Node::TYPE_ELEMENT:
	{
	    FXString name = node->getName().lower();

	    Ref<HTMLObject> o;

	    if( name == "body" )
	    {
		o = new ObjectBody( obj, node );
		obj->addChield( o );
		root->setBody( o );
	    }

	    if( name == "div"    ||
		name == "center" ||
		name == "code"   ||
		name == "br"     ||
		name == "h1"     ||
		name == "h2"     ||
		name == "h3"     ||
		name == "h4"     ||
		name == "h5"     ||
		name == "b"      ||
		name == "i"      ||
		name == "hr"     ||
		name == "br"     ||
		name == "li"     ||
		name == "pre"    )
	    {
		o = new ObjectLayout( obj, node );
		obj->addChield( o );
	    }

	    if( name == "a" )
	    {
		o = new ObjectA( obj, node );
		obj->addChield( o );
	    }

	    if( name == "table" )
	    {
		o = new ObjectTable( obj, node );
		obj->addChield( o );
	    }
	    
	    if( name == "tr" )
	    {
		o = new ObjectRow( obj, node );
		obj->addChield( o );
	    }

	    if( name == "td" )
	    {
		Ref<HTMLObject> td = new ObjectTD( obj, node );
		obj->addChield( td );
		o = new ObjectLayout( td, node );
		td->addChield( o );
	    }


	    if( name == "ul" )
	    {
		o = new ObjectUL( obj, node );
		obj->addChield( o );
	    }

	    if( name == "img" )
	    {
		o = new ObjectImage( obj, node );
		obj->addChield( o );
	    }

	    if( !o.valid() )
		o = obj;

	    for( int i = 0; i < node->getChildNum(); ++i )
		build_object_tree( node->getChild( i ), o, root );

	    break;
	}

	case Node::TYPE_COMMENT:
	    break;
    }
}

}

Ref<HTMLObject> build_object_tree( Ref<Node> root, FXHTML *window )
{
    printf( "HTMLObject size: %d\n", sizeof( HTMLObject ) );

    ObjectRoot *oroot = new ObjectRoot( window );
    Ref<HTMLObject> object_root = oroot;
    build_object_tree( root, object_root, oroot );
    return object_root;
}

