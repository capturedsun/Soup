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

#ifndef NODE_H_
#define NODE_H_

#include "html/gumbo/gumbo.h"
#include <string>
#include <cstring>

class CSelection;

class CNode
{
	public:

		CNode(GumboNode* apNode = NULL);

		virtual ~CNode();

	public:

		bool valid();
		CNode parent();
		CNode nextSibling();
		CNode prevSibling();
		unsigned int childNum();
		CNode childAt(size_t i);

		std::string attribute(std::string key);
		std::string text();
		std::string ownText();
		GumboNode* getGumboNode();

		size_t startTagLeft();
		size_t startTagRight();
		size_t endTagLeft();
		size_t endTagRight();
		size_t length();

		std::string outerHTML();
		std::string innerHTML();
		// add add/remove node
		void removeNode();
		void append(CNode* child);
		void addAttri(std::string name, std::string val);
		void removeAttri(std::string name);
		// Add/remove style attributes

		std::string tag();
		CSelection find(std::string aSelector);

		bool equals(CNode* rhs);

	private:

		std::string outerHTML_converter(GumboNode* root);

		std::string innerHTML_converter(GumboNode* root);

		int recursCount = -1;

		GumboNode* mpNode = nullptr;
};

#endif /* NODE_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
