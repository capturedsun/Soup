#include "Document.h"

CDocument::CDocument()
{
	parsed = NULL;
    head = NULL;
    body = NULL;
}

void CDocument::parse(const std::string& htmlString)
{
	reset();
	parsed = gumbo_parse(htmlString.c_str());
    findBody(parsed->root);
    findHead(parsed->root);
}

void CDocument::findBody(GumboNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    if (root->type == GUMBO_NODE_ELEMENT)
    {
        if (std::string(gumbo_normalized_tagname(root->v.element.tag)).compare("body") == 0)
        {
            body = new CNode(root);
            root = nullptr;
        }
        else
        {
            for (unsigned int i = 0; i < root->v.element.children.length; ++i)
            {
                findBody(static_cast<GumboNode*>(root->v.element.children.data[i]));
            }
        }
    }
    return;
}

void CDocument::findHead(GumboNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    if (root->type == GUMBO_NODE_ELEMENT)
    {
        if (std::string(gumbo_normalized_tagname(root->v.element.tag)).compare("head") == 0)
        {
            head = new CNode(root);
            root = nullptr;
        }
        else
        {
            for (unsigned int i = 0; i < root->v.element.children.length; ++i)
            {
                findHead(static_cast<GumboNode*>(root->v.element.children.data[i]));
            }
        }
    }
    return;
}

CDocument::~CDocument()
{
	reset();
}

GumboNode* CDocument::getParsedRoot()
{
    return parsed->root;
}

std::string CDocument::outer_text()
{
    std::string htmlString = "";
    htmlString = outer_text_converter(parsed->root);
    return htmlString;
}

std::string CDocument::outer_text_converter(GumboNode* root)
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
            temp += outer_text_converter(static_cast<GumboNode*>(root->v.document.children.data[i]));
        }
        break;
    case GUMBO_NODE_ELEMENT:
        temp += "<" + std::string(gumbo_normalized_tagname(root->v.element.tag));
        GumboAttribute* attr;
        for (unsigned int i = 0; i < root->v.element.attributes.length; ++i)
        {
            attr = static_cast<GumboAttribute*>(root->v.element.attributes.data[i]);
            temp += " " + std::string(attr->name) + "=\"" + std::string(attr->value) + "\"";
        }
        temp += ">";
        for (unsigned int i = 0; i < root->v.element.children.length; ++i) {
            temp += outer_text_converter(static_cast<GumboNode*>(root->v.element.children.data[i]));
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

std::string CDocument::body_text()
{
    std::string htmlString = "";
    htmlString = body_text_converter(parsed->root);
    return htmlString;
}

std::string CDocument::body_text_converter(GumboNode* root)
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
            temp += body_text_converter(static_cast<GumboNode*>(root->v.document.children.data[i]));
        }
        break;
    case GUMBO_NODE_ELEMENT:
        if (recursCount >= 2)
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
            temp += body_text_converter(static_cast<GumboNode*>(root->v.element.children.data[i]));
        }
        if (recursCount >= 2)
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


CSelection CDocument::find(std::string selector)
{
	if (parsed == NULL)
	{
		throw "document not initialized";
	}

	CSelection sel(parsed->root);
	return sel.find(selector);
}

void CDocument::reset()
{
	if (parsed != NULL)
	{
		gumbo_destroy_output(&kGumboDefaultOptions, parsed);
		parsed = NULL;
	}
}
