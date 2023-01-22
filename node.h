#ifndef NODE_H
#define NODE_H

#include "ref.h"
#include <fox/fx.h>
#include <vector>

class Node
{
 public:

    enum Type {
	TYPE_TEXT,
	TYPE_ELEMENT,
	TYPE_COMMENT
    };

    struct Attribute {
	FXString name;
	FXString value;
	
	Attribute( const FXString & name, const FXString & value ) : name( name ), value( value ) {}
    };

 private:
    typedef Ref<Node> r_node;

    r_node parent;
    std::vector<r_node> children;    
    Type type;
    FXString name;
    FXString value;

    std::vector<Attribute> attributes;

 public:
    Node( r_node parent = r_node(), Type type = TYPE_TEXT, FXString name = "", FXString value = "" ) : 
	parent( parent ), type( type ), name(name), value( value ) {}

    r_node    getParent() const { return parent; }
    int       getChildNum() const { return children.size(); }
    r_node    getChild( int i ) { return children[i]; }
    Type      getType() const { return type; }
    FXString  getName() const { return name; }
    FXString  getValue() const { return value; }
    int       getAttributeNum() const { return attributes.size(); }
    Attribute getAttribute( int i ) const { return attributes[i]; }
    bool      getAttribute( const FXString & name, FXString & value ) const ;

    void setParent( r_node p ) { parent = p; }
    void addChield( r_node c ) { children.push_back( c ); }
    void addAttribute( Attribute attr ) { attributes.push_back( attr ); }
};

Ref<Node> parse_file( const FXString & file );

void dump_tree( Ref<Node> node );

#endif
