/***************************************************************************
 * 
 * $Id$
 * 
 **************************************************************************/

/**
 * @file $HeadURL$
 * @author $Author$(hoping@baimashi.com)
 * @date $Date$
 * @version $Revision$
 * @brief 
 *  
 **/

#include "Node.h"
#include "Selection.h"
#include "QueryUtil.h"
#include <iostream>

CNode::CNode(GumboNode* apNode)
{
	mpNode = apNode;
}

CNode::~CNode()
{
	mpNode = nullptr;
}

CNode CNode::parent()
{
	return CNode(mpNode->parent);
}

CNode CNode::nextSibling()
{
	return parent().childAt(mpNode->index_within_parent + 1);
}

CNode CNode::prevSibling()
{
	return parent().childAt(mpNode->index_within_parent - 1);
}

unsigned int CNode::childNum()
{
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return 0;
	}

	return mpNode->v.element.children.length;

}

std::string CNode::outerHTML()
{
	std::string htmlString = "";
	htmlString = outerHTML_converter(mpNode);
	return htmlString;
}

std::string CNode::outerHTML_converter(GumboNode* root)

{
	std::string temp = "";
	if (root == nullptr)
	{
		return temp;
	}
	switch (root->type)
	{
	case GUMBO_NODE_DOCUMENT:
		for (unsigned int i = 0; i < root->v.document.children.length; ++i) {
			temp += outerHTML_converter(static_cast<GumboNode*>(root->v.document.children.data[i]));
		}
		break;
	case GUMBO_NODE_ELEMENT:
		temp += "<" + std::string(gumbo_normalized_tagname(root->v.element.tag));
		GumboAttribute* attr;
		for (unsigned int i = 0; i < root->v.element.attributes.length; ++i) {
			attr = static_cast<GumboAttribute*>(root->v.element.attributes.data[i]);
			temp += " " + std::string(attr->name) + "=\"" + std::string(attr->value) + "\"";
		}
		temp += ">";
		for (unsigned int i = 0; i < root->v.element.children.length; ++i) {
			temp += outerHTML_converter(static_cast<GumboNode*>(root->v.element.children.data[i]));
		}
		temp += "</" + std::string(gumbo_normalized_tagname(root->v.element.tag)) + ">";
		break;
	case GUMBO_NODE_TEXT:
	case GUMBO_NODE_WHITESPACE:
		temp += std::string(root->v.text.text);
		break;
	case GUMBO_NODE_CDATA:
		temp += std::string(root->v.text.text);
		break;
	case GUMBO_NODE_COMMENT:
		temp += std::string(root->v.text.text);
		break;
	default:
		break;
	}

	return temp;
}

std::string CNode::innerHTML()
{
	std::string htmlString = "";
	htmlString = innerHTML_converter(mpNode);
	return htmlString;
}

std::string CNode::innerHTML_converter(GumboNode* root)

{
	recursCount++;
	std::string temp = "";
	if (root == nullptr)
	{
		recursCount--;
		return temp;
	}
	switch (root->type)
	{
	case GUMBO_NODE_DOCUMENT:
		for (unsigned int i = 0; i < root->v.document.children.length; ++i) {
			temp += innerHTML_converter(static_cast<GumboNode*>(root->v.document.children.data[i]));
		}
		break;
	case GUMBO_NODE_ELEMENT:
		if (recursCount > 0)
		{
			temp += "<" + std::string(gumbo_normalized_tagname(root->v.element.tag));
			GumboAttribute* attr;
			for (unsigned int i = 0; i < root->v.element.attributes.length; ++i) {
				attr = static_cast<GumboAttribute*>(root->v.element.attributes.data[i]);
				temp += " " + std::string(attr->name) + "=\"" + std::string(attr->value) + "\"";
			}
			temp += ">";
		}
		for (unsigned int i = 0; i < root->v.element.children.length; ++i) {
			temp += innerHTML_converter(static_cast<GumboNode*>(root->v.element.children.data[i]));
		}
		if (recursCount > 0)
		{
			temp += "</" + std::string(gumbo_normalized_tagname(root->v.element.tag)) + ">";
		}
		break;
	case GUMBO_NODE_TEXT:
	case GUMBO_NODE_WHITESPACE:
		temp += std::string(root->v.text.text);
		break;
	case GUMBO_NODE_CDATA:
		temp += std::string(root->v.text.text);
		break;
	case GUMBO_NODE_COMMENT:
		temp += std::string(root->v.text.text);
		break;
	default:
		break;
	}

	recursCount--;
	return temp;
}

// Have to store this node, the one getting removed, before calling this function so that the pointer to the node doesn't get lost, so it can be used for appending later.
void CNode::removeNode()
{
	// Unwrap nodes will keep children, not remove, remove will fully remove a node and it's children.
	// Go up to parent and delete the current node.
	GumboNode* target = mpNode;
	GumboNode* parent = mpNode->parent;
	if (parent->type != GUMBO_NODE_ELEMENT)
	{
		return;
	}
	for (int i = 0; i < parent->v.document.children.length; i++)
	{
		if (parent->v.document.children.data[i] == target)
		{	// No need to remove equivalent child from parent->v.element.children, it appears both that GumboVector and parent->v.document.children are intrinsically linked, so length, capacity, and data indices are equivalent.
			for (int j = i; j < (parent->v.document.children.length - 1); j++)	// They are the same GumboVector stored in two different locations, just the document.children data is linked to the tags and the element.children data is not.
			{
				parent->v.document.children.data[j] = parent->v.document.children.data[j + 1];
			}
			parent->v.document.children.length--;
			break;
		}
	}
	target = nullptr;
	parent = nullptr;
}

void CNode::append(CNode* child)
{
	// Go up to parent and delete the current node.
	GumboNode* newChild = child->mpNode;
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return;
	} // Unless issues are encountered later, realloc() should work 100% of the time, since it will always append the node to the end, as is the purpose of this append() function.
	  // mpNode->v.document.children.data = (void**)realloc(mpNode->v.document.children.data, (mpNode->v.document.children.length + 1) * sizeof(GumboNode*));
	mpNode->v.document.children.data[mpNode->v.document.children.length] = newChild;
	mpNode->v.document.children.length++;
	newChild = nullptr;
}

void CNode::addAttri(std::string name, std::string val)
{ // Does account for empty input for both value and name fields.
	if (name.length() < 1)
	{
		return;
	}
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return;
	}
	for (int i = 0; i < mpNode->v.element.attributes.length; i++)
	{
		GumboAttribute* temp = (GumboAttribute*)mpNode->v.element.attributes.data[i];
		if (temp->name == name)
		{
			delete[] temp->value;
			temp->value = (char*)malloc(val.length() * sizeof(val.c_str()));
			strcpy(temp->value, val.c_str());
			return;
		}
		temp = nullptr;
	}
	GumboAttribute** newArr = (GumboAttribute**)malloc((mpNode->v.element.attributes.length + 1) * sizeof(GumboAttribute));
	for (int i = 0; i < mpNode->v.element.attributes.length; i++)
	{
		newArr[i] = (GumboAttribute*)mpNode->v.element.attributes.data[i];
	}
	GumboAttribute* newatt = (GumboAttribute*)malloc(sizeof(GumboAttribute));
	newatt->name = (char*)malloc(name.length() * sizeof(name.c_str()));
	strcpy(newatt->name, name.c_str()); // strcpy() browser.cc example
	newatt->value = (char*)malloc(name.length() * sizeof(val.c_str()));
	strcpy(newatt->value, val.c_str());
	newArr[mpNode->v.element.attributes.length] = newatt;
	free((GumboAttribute*)mpNode->v.element.attributes.data);
	mpNode->v.element.attributes.data = (void**)newArr;
	newatt->name_start = (GumboSourcePosition){ 0, 0, 0 };
	newatt->name_end = (GumboSourcePosition){ 0, 0, 0 };
	newatt->value_start = (GumboSourcePosition){ 0, 0, 0 };
	newatt->value_end = (GumboSourcePosition){ 0, 0, 0 };
	newatt->attr_namespace = GUMBO_ATTR_NAMESPACE_NONE;
	//mpNode->v.element.attributes.data = (void**)realloc(mpNode->v.element.attributes.data, (mpNode->v.element.attributes.length + 1) * sizeof(GumboAttribute*));
	mpNode->v.element.attributes.data[mpNode->v.element.attributes.length] =(void*) newatt;
	mpNode->v.element.attributes.length++;
	newArr = nullptr;
	newatt = nullptr;
}

void CNode::removeAttri(std::string name)
{
	for (int i = 0; i < mpNode->v.element.attributes.length; i++)
	{
		GumboAttribute* temp = (GumboAttribute*)mpNode->v.element.attributes.data[i];
		if (temp->name == name)
		{
			free((char*)temp->name);
			free((char*)temp->value);
			free((GumboAttribute*)temp);
			for (int j = i; j < (mpNode->v.element.attributes.length - 1); j++)
			{
				mpNode->v.element.attributes.data[j] = mpNode->v.element.attributes.data[j + 1];
			}
			mpNode->v.element.attributes.length--;
			return;
		}

		temp = nullptr;
	}
}

bool CNode::valid()
{
	return mpNode != NULL;
}

CNode CNode::childAt(size_t i)
{
	if (mpNode->type != GUMBO_NODE_ELEMENT || i >= mpNode->v.element.children.length)
	{
		return CNode();
	}

	return CNode((GumboNode*) mpNode->v.element.children.data[i]);
}

std::string CNode::attribute(std::string key)
{
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	GumboVector attributes = mpNode->v.element.attributes;
	for (unsigned int i = 0; i < attributes.length; i++)
	{
		GumboAttribute* attr = (GumboAttribute*) attributes.data[i];
		if (key == attr->name)
		{
			return attr->value;
		}
	}

	return "";
}

std::string CNode::text()
{
	return CQueryUtil::nodeText(mpNode);
}

std::string CNode::ownText() // returns mpNode->v.text.text
{
	return CQueryUtil::nodeOwnText(mpNode);
}

GumboNode* CNode::getGumboNode()
{
	return mpNode;
}

size_t CNode::startTagRight()
{
	switch(mpNode->type)
	{
	  case GUMBO_NODE_ELEMENT:
		  return mpNode->v.element.start_pos.offset + mpNode->v.element.original_tag.length;
	  case GUMBO_NODE_TEXT:
		  return mpNode->v.text.start_pos.offset;
	  default:
		  return 0;
  }
}

size_t CNode::endTagLeft()
{
	switch(mpNode->type)
	{
	  case GUMBO_NODE_ELEMENT:
		  return mpNode->v.element.end_pos.offset;
	  case GUMBO_NODE_TEXT:
		  return mpNode->v.text.original_text.length + startTagRight();
	  default:
		  return 0;
	}
}

size_t CNode::startTagLeft()
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.start_pos.offset;
	case GUMBO_NODE_TEXT:
		return mpNode->v.text.start_pos.offset;
	default:
		return 0;
	}
}

size_t CNode::endTagRight()
{
	switch(mpNode->type)
	{
	case GUMBO_NODE_ELEMENT:
		return mpNode->v.element.end_pos.offset + mpNode->v.element.original_end_tag.length;
	case GUMBO_NODE_TEXT:
		return mpNode->v.text.original_text.length + startTagRight();
	default:
		return 0;
	}
}

size_t CNode::length()
{
	return endTagRight() - startTagLeft();
}

std::string CNode::tag()
{
	if (mpNode->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}

	return gumbo_normalized_tagname(mpNode->v.element.tag);
}

CSelection CNode::find(std::string aSelector)
{
	CSelection c(mpNode);
	return c.find(aSelector);
}

bool CNode::equals(CNode* rhs)
{
	if (mpNode == rhs->mpNode)
	{
		return true;
	}
	return false;
}