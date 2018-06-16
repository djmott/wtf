# The Hierarchy Generator Pattern

## Introduction

As the name implies, the Hierarchy Generator Pattern (HGP) is a C++ pattern that generates class hierarchies similar to dependency injection. HGP combines various generic behavioral policy classes into a linear inheratence model which eleminates problems caused with multiple inheritance.

### Policy Classes

A policy takes the form of a *parameterized super-class* idiom which takes the following form:

~~~cpp
/* A simple policy */
template <typename T> struct policy : T
{
};
~~~

**T** is accepted as a parameter which the class inherits from.

### Hierarchy Construction

#### Generic form

The process of composing a class hierachy from a series of discrete policies involves an intermediary which takes the following generic form:

~~~cpp
/* Generic form hierarchy_generator */
template <template <typename> typename ... _policies>
struct hierarchy_generator;
~~~

The **hierarchy_generator** is known as a *template-template class* because it is a template which accepts a template as a parameter. It is also known as a *variadic template* because it accepts a variable number of template class parameters. The parameters in a variadic template are known as a *parameter pack* Constructing the hierarchy of policies requires a minimum of two specializations of this class. The first one introduced here is the *terminator* of the hierarchy.

#### Hierarchy Terminator

~~~cpp
/* terminator of hierarchy_generator */
template <>
struct hierarchy_generator<>
{
};
~~~

This is a specialization of the generic form which takes no parameters. As will be demonstrated, it is the super-most class in the hierarchy. If desirable, it could inherit from some other common class which all concrete instances of the hierarchy would inhert, as WTF does.

#### Hierarchy specialization

The real construction of the hierarchy takes place with the second specialization of the generic form hierarchy_generator:

~~~cpp
/* generator */
template <template <typename> typename _head_policy, template <typename> typename ... _policies> 
struct hierarchy_generator<_head_policy, _policies...> 
  : _head_policy< hierarchy_generator<_policies...> >
{
};
~~~

This is a specialization of the generic form which takes one or more parameters. It is a *template-template class* a *parameterized super-class* and a *variadic template class*. The hierarchy generation occurs as a result of inheriting a specialization of the first item of the parameter packer. It's specialized with *hierarchy_generator<_policies...>*. This recursively composes the policies in a linear inheritance hierarchy.