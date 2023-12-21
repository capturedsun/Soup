#ifndef SELECTION_H_
#define SELECTION_H_

#include "Object.h"
#include <vector>
#include <string>
#include "html/gumbo/gumbo.h"

class CNode;

class CSelection: public CObject
{
	private:

		std::vector<GumboNode*> selectedNodes;

	public:

		CNode operator [](int i);
		CSelection(GumboNode* rootNode);
		CSelection(std::vector<GumboNode*> rootNodes);
		virtual ~CSelection();

	public:

		CSelection find(std::string selector);
		CNode nodeAt(size_t i);
		GumboNode* gumobonodeAt(size_t i);
		size_t length();
};

#endif
