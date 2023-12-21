#include "Selection.h"
#include "CParser.h"
#include "QueryUtil.h"
#include "Node.h"

CSelection::CSelection(GumboNode* rootNode)
{
	selectedNodes.push_back(rootNode);
}

CSelection::CSelection(std::vector<GumboNode*> rootNodes)
{
	this->selectedNodes = rootNodes;
}

CSelection::~CSelection()
{
}

CNode CSelection::operator [](int i)
{
	return this->nodeAt(i);
}

CSelection CSelection::find(std::string selector)
{
	CSelector* sel = CParser::create(selector);
	std::vector<GumboNode*> ret;
	for (std::vector<GumboNode*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); it++)
	{
		GumboNode* pNode = *it;
		std::vector<GumboNode*> matched = sel->matchAll(pNode);
		ret = CQueryUtil::unionNodes(ret, matched);
	}
	sel->release();
	return CSelection(ret);
}

CNode CSelection::nodeAt(size_t i)
{
	if (i >= selectedNodes.size())
	{
		return CNode();
	}

	return CNode(selectedNodes[i]);
}

GumboNode* CSelection::gumobonodeAt(size_t i)
{
	if (i >= selectedNodes.size())
	{
		return nullptr;
	}

	return selectedNodes[i];
}

size_t CSelection::length()
{
	return selectedNodes.size();
}

/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

