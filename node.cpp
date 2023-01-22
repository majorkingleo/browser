#include "node.h"

#ifdef USE_LIBSGML
#include <sgml/libsgml.h>
#endif

#ifdef USE_LIBTIDY
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#endif

bool Node::getAttribute( const FXString & name, FXString & value ) const
{
    for( unsigned i = 0; i < attributes.size(); ++i )
    {
	if( attributes[i].name == name )
	{
	    value = attributes[i].value;
	    return true;
	}
    }

    return false;
}

void dump_tree( Ref<Node> node )
{
    switch( node->getType() )
    {
	case Node::TYPE_TEXT:
	    printf( "%s\n", node->getValue().text() );
	    break;
	case Node::TYPE_COMMENT:
	    printf( "<!-- %s -->\n", node->getValue().text() );
	    break;
	case Node::TYPE_ELEMENT:
	{
	    printf( "<%s ", node->getName().text() );

	    for( int i = 0; i < node->getAttributeNum(); ++i )
		printf( "%s=\"%s\" ", node->getAttribute( i ).name.text(), node->getAttribute( i ).value.text() );

	    if( node->getChildNum() )
	    {
		printf( ">\n" );

		for( int i = 0; i < node->getChildNum(); ++i )
		    dump_tree( node->getChild( i ) );

		printf( "</%s>\n", node->getName().text() );
	    } else {
		printf( "/>\n" );
	    }
	}
    }
}


#ifdef USE_LIBSGML

namespace {

void build_tree( Ref<Node> r_node, DOM_NODE *node )
{
    if( !node )
	return;

    switch( node->type )
    {
	case DOM_NODE_TYPE_TEXT:
	    if( node->value )
		r_node->addChield( new Node( r_node, Node::TYPE_TEXT, "", node->value ) );
	    break;

	case DOM_NODE_TYPE_ELEMENT:
	{
	    Ref<Node> curr = new Node( r_node, Node::TYPE_ELEMENT, node->name );

	    for( DOM_NODE *c = node->attributes; c; c = c->nextSibling )
		curr->addAttribute( Node::Attribute( c->name, c->value ) );

	    r_node->addChield( curr );

	    for( DOM_NODE *c = node->firstChild; c; c = c->nextSibling )
		build_tree( curr, c );

	    break;
	}

	case DOM_NODE_TYPE_COMMENT:
	    r_node->addChield( new Node( r_node, Node::TYPE_COMMENT, "", node->value ) );
	    break;	    

	default:
	    for( DOM_NODE *c = node->firstChild; c; c = c->nextSibling )
		build_tree( r_node, c );
		
    }
}

} // namespace 

Ref<Node> parse_file( const FXString & file )
{
    SGML_PARSER sgml;
    unsigned long flags= ;

    sgmlParserInitialize( &sgml, SGML_EXTENSION_TYPE_HTML, NULL, NULL );
    sgmlParserExtensionSetParam(&sgml, SGML_EXTENSION_HTML_PARAM_FLAG, (void *)flags);
    if( !sgmlParserParseFile( &sgml, file.text() ) )
    {
	sgmlParserDestroy( &sgml, 0 );
	printf( "error parsing file %s\n", file.text() );
	return Ref<Node>();
    }

    DOM_NODE *curr = sgmlExtensionHtmlGetDocument(&sgml);


    Ref<Node> node = new Node( Ref<Node>(), Node::TYPE_ELEMENT, "root" );

    build_tree( node, curr );

    sgmlParserDestroy( &sgml, 0 );

    return node;
}

#endif

#ifdef USE_LIBTIDY

namespace {

void build_tree( Ref<Node> r_node, TidyNode node, TidyDoc tdoc )
{
    switch( tidyNodeGetType( node ) )
    {
	case TidyNode_Text:
	{
	    TidyBuffer buffer = {0};

	    tidyNodeGetText( tdoc, node, &buffer );

	    r_node->addChield( new Node( r_node, Node::TYPE_TEXT, "", FXString( (FXchar*)buffer.bp, buffer.size ) ) );

	    tidyBufFree( &buffer );
	    break;
	}

	case TidyNode_Start:
	case TidyNode_StartEnd:
	{
	    Ref<Node> curr = new Node( r_node, Node::TYPE_ELEMENT, tidyNodeGetName( node ) );

	    for( TidyAttr attr = tidyAttrFirst( node ); attr; attr = tidyAttrNext( attr ) )
		curr->addAttribute( Node::Attribute( tidyAttrName( attr ), tidyAttrValue( attr ) ) );

	    r_node->addChield( curr );

	    for( TidyNode child = tidyGetChild( node ); child; child = tidyGetNext( child ) )
		build_tree( curr, child, tdoc );

	    break;
	}

	default:
	    for( TidyNode child = tidyGetChild( node ); child; child = tidyGetNext( child ) )
		build_tree( r_node, child, tdoc );
	    
    }    
}

}

Ref<Node> parse_file( const FXString & file )
{
    TidyDoc tdoc = tidyCreate();

    if( tidyParseFile( tdoc, file.text() ) < 0 )
    {
	tidyRelease( tdoc );
	return Ref<Node>();
    }
    
    Ref<Node> node = new Node( Ref<Node>(), Node::TYPE_ELEMENT, "root" );

    build_tree( node, tidyGetRoot( tdoc ), tdoc );

    tidyRelease( tdoc );

    return node;
}

#endif
