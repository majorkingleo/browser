#ifndef BUILD_TREE_H
#define BUILD_TREE_H

#include "ref.h"

class Node;
class FXHTML;
class HTMLObject;

Ref<HTMLObject> build_object_tree( Ref<Node> root, FXHTML *window );

#endif
