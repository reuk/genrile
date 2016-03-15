# genrile
c++14 json generator/parser with schema support

## disclaimer
work-in-progress, currently just a bad json generator

# interface ideas

Compile-time schema

* parse a schema at compile time and turn it into a parser for an istream
    * this will just return generic json 'Value' objects which pass the schema

Run-time schema

* parse a schema at run time and produce an istream parser
    * build a parser hierarchy at runtime
    * returns generic json 'Value' objects

Built-in schema

* define a class hierarchy directly in C++ code
    * use some kind of reflection to generate a parser for this hierarchy
