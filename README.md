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

# wish list

* serializer and deserializer should be enabled through free functions
    * so that class definitions don't have to be altered
    * either register
        * a data member
        * a getter
        * a setter
        * a getter and setter
    * maybe there's a way of doing this with type traits

# todo

* better error reporting

* build runtime structures from compile time ones

* parser generator from compile time schema
