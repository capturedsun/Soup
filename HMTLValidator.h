#ifndef HTMLVALIDATOR
#define HTMLVALIDATOR
#include "html/query/Document.h"
#include "html/query/Selection.h"
#include "html/query/Node.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

using std::string;

class HTMLValidator 
{
    private:
        std::vector<std::string> toolTags, selectTags;
        std::string indexHTML;
    
    public:
        HTMLValidator() {}
        HTMLValidator(std::string boilerplatePath);

        static std::string HTML_file_to_string(std::string filename);

        int checkComplete(string html);
        std::string cleanup_HTML(std::string html, std::string spaceNameFromURL);
        std::string get_HTML_from_file(std::string htmlfilepath, std::string foldername);
        std::string create_new_html(std::string foldername);
        bool is_valid_html(std::string html);
        void getSpaceFiles(std::string& html, std::vector<string>& folders, std::vector<string>& files);
        static void remove(CNode node, std::string& html);
    
    private:
        static void unwrap(CNode node, std::string& html);
        static void unwrap_all_instances(CSelection selection, std::string& html);
        static void remove_class(CNode node, std::string classname, std::string& html);
        static void replace_backround_image_url(CNode node, std::string newUrl, std::string& html);
        static void remove_content(CNode node, std::string& html);
        static void replace_content(CNode element, std::string& html, std::vector<CNode> newContent);
        static void remove_all_instances(CSelection selection, std::string& html);
        static void split_string(std::string s, std::vector<std::string>& v);
        static void add_attribute(CNode node, std::string& html, std::string attributeName, std::string attributeValue);
        
        static std::string get_wrapper(CDocument& doc, std::string html, std::string spaceNameFromURL);
        static std::string get_element_string(CNode node, std::string html);
        static std::string get_element_contents(CNode node, std::string html);
};

#endif