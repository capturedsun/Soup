![soup 1 (1)](https://github.com/capturedsun/Soup/assets/61888181/4014845c-3cdf-44d0-8854-0e6595179e0d)
# Soup
Soup is an open-source HTML parser and query language for C/C++. It uses Google's Gumbo Parser and a wrapper library, Query, to make DOM manipulations to the Gumbo parse tree.
## Installation
To download and use Soup:
1. Clone the repository to your machine.
2. Move the Soup files into your project files.
3. Use the appropriate include statement to use in your project. Exact wording of include statement may vary depending on the file structure of your project.

## Use
Example of usage:
```
string html = "<p id="horse"></p>"
CDoc doc = new CDocument()
doc.parse(html)
CNode paragraph = doc.find("#horse")
paragraph = doc.body.children.at(0)
```
Functions that are available for DOM manipulation:
```
parse()
CNode.remove()
CNode.appendChild(CNode child)
...
```

## How to Contribute
We welcome any additions to the project via pull request. We ask that you thoroughly test your code beforehand. In your PR, make the title concise and relevant, and your description while we would prefer it not be too long, be descriptive of the changes or additions being made to the code. It will be reviewed and merged after review, so long as no issues were found.
## Credits
### Google 
They provided the open-source [Gumbo library](https://github.com/google/gumbo-parser/tree/master) that we ue for parse tree generation.
### Lazytiger
They provided the open-source [Query library](https://github.com/lazytiger/gumbo-query/tree/master) that we use to manipulate Gumbo parse data.
### Captured Sun
We made the validator file, and made updates to the Query library for direct parse tree manipulation.
Individual contributors from Captured Sun are as follows:

Derek Wright: Wrote the original version of the Validator file.

Reece McDonald: Worked on updating Validator and Query Library.

Daniel Camejo: Worked on updating the Query Library.
