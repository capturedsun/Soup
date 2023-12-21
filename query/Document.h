#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "html/gumbo/gumbo.h"
#include <string>
#include "Selection.h"
#include "Node.h"
#include <iostream>

class CDocument: public CObject
{
	public:

		CNode* head;

		CNode* body;

		CDocument();

		void parse(const std::string& htmlString);

		virtual ~CDocument();

		GumboNode* getParsedRoot();

		CSelection find(std::string selector);

		std::string outer_text();

		std::string outer_text_converter(GumboNode* root);

		std::string body_text();

		std::string body_text_converter(GumboNode* root);

		void remove_gumbo_nodes(GumboNode* node);

		bool gumbo_node_equals(GumboNode* lhs, GumboNode* rhs);

	private:

		void reset();

		void findHead(GumboNode* root);

		void findBody(GumboNode* root);

	private:

		int recursCount = -1;

		GumboOutput* parsed;
};

#endif /* DOCUMENT_H_ */

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
