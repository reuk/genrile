#pragma once

#include <string>
#include <vector>
#include <map>
#include <ostream>

namespace Genrile {

class Value;
class Integer;
class Real;
class Boolean;
class String;
class Array;
class Object;
class Null;
class Variadic;

//  TODO
//  initializer list constructors

class Serializable {
public:
    virtual std::ostream& write_json_to_stream(std::ostream& os) const = 0;
    virtual void read_json_from_stream(std::istream& is) = 0;
    std::string to_json() const;
};

class Value : public Serializable {
public:
    enum class Type {
        integer,
        real,
        boolean,
        string,
        array,
        object,
        null,
    };

    virtual Type get_type() const = 0;

    Value() = default;
    virtual ~Value() noexcept = default;
    Value(const Value&) noexcept = default;
    Value& operator=(const Value&) noexcept = default;
    Value(Value&&) noexcept = default;
    Value& operator=(Value&&) noexcept = default;

    virtual std::unique_ptr<Value> clone() const = 0;

    template <Type TYPE>
    struct upcast_trait {};

    template <Type TYPE>
    typename upcast_trait<TYPE>::type& get() {
        if (get_type() != TYPE)
            throw std::runtime_error("incorrect type");
        return dynamic_cast<typename upcast_trait<TYPE>::type&>(*this);
    }
    template <Type TYPE>
    const typename upcast_trait<TYPE>::type& get() const {
        if (get_type() != TYPE)
            throw std::runtime_error("incorrect type");
        return dynamic_cast<const typename upcast_trait<TYPE>::type&>(*this);
    }

    virtual Integer& get_integer();
    virtual const Integer& get_integer() const;
    virtual Real& get_real();
    virtual const Real& get_real() const;
    virtual Boolean& get_boolean();
    virtual const Boolean& get_boolean() const;
    virtual String& get_string();
    virtual const String& get_string() const;
    virtual Array& get_array();
    virtual const Array& get_array() const;
    virtual Object& get_object();
    virtual const Object& get_object() const;
    virtual Null& get_null();
    virtual const Null& get_null() const;
};

template <>
struct Value::upcast_trait<Value::Type::integer> {
    using type = Integer;
};

template <>
struct Value::upcast_trait<Value::Type::real> {
    using type = Real;
};

template <>
struct Value::upcast_trait<Value::Type::boolean> {
    using type = Boolean;
};

template <>
struct Value::upcast_trait<Value::Type::string> {
    using type = String;
};

template <>
struct Value::upcast_trait<Value::Type::array> {
    using type = Array;
};

template <>
struct Value::upcast_trait<Value::Type::object> {
    using type = Object;
};

template <>
struct Value::upcast_trait<Value::Type::null> {
    using type = Null;
};

template <typename Primitive, Value::Type T>
class PrimitiveJsonValue : public Value {
public:
    using value_type = Primitive;
    static constexpr auto type = T;

    Type get_type() const override {
        return type;
    }

    PrimitiveJsonValue() = default;

    PrimitiveJsonValue(const value_type& value)
            : value(value) {
    }

    PrimitiveJsonValue(value_type&& value)
            : value(std::move(value)) {
    }

    operator Primitive() const {
        return value;
    }

    const Primitive& get_value() const {
        return value;
    }

    bool operator==(const PrimitiveJsonValue& rhs) const {
        return value == rhs.value;
    }

    bool operator!=(const PrimitiveJsonValue& rhs) const {
        return value != rhs.value;
    }

    bool operator<(const PrimitiveJsonValue& rhs) const {
        return value < rhs.value;
    }

    bool operator<=(const PrimitiveJsonValue& rhs) const {
        return value <= rhs.value;
    }

    bool operator>(const PrimitiveJsonValue& rhs) const {
        return value > rhs.value;
    }

    bool operator>=(const PrimitiveJsonValue& rhs) const {
        return value >= rhs.value;
    }

protected:
    value_type value;
};

class Integer final : public PrimitiveJsonValue<int, Value::Type::integer> {
public:
    Integer() = default;
    Integer(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class Real final : public PrimitiveJsonValue<double, Value::Type::real> {
public:
    Real() = default;
    Real(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class Boolean final : public PrimitiveJsonValue<bool, Value::Type::boolean> {
public:
    Boolean() = default;
    Boolean(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class String final
    : public PrimitiveJsonValue<std::string, Value::Type::string> {
public:
    String() = default;
    String(std::istream& is);
    String(const char*);
    String(const std::string&);
    String(std::string&&);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
};

class Array final
    : public PrimitiveJsonValue<std::vector<Variadic>, Value::Type::array> {
public:
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;

    Array() = default;
    Array(std::istream& is);

    Array(std::vector<Variadic>&& v);
    Array(std::initializer_list<Variadic> il);

    value_type::reference at(value_type::size_type pos);
    value_type::const_reference at(value_type::size_type pos) const;

    value_type::reference operator[](value_type::size_type pos);
    value_type::const_reference operator[](value_type::size_type pos) const;

    value_type::reference front();
    value_type::const_reference front() const;

    value_type::reference back();
    value_type::const_reference back() const;

    value_type::iterator begin();
    value_type::const_iterator begin() const;
    value_type::const_iterator cbegin() const;
    value_type::reverse_iterator rbegin();
    value_type::const_reverse_iterator rbegin() const;
    value_type::const_reverse_iterator crbegin() const;

    value_type::iterator end();
    value_type::const_iterator end() const;
    value_type::const_iterator cend() const;
    value_type::reverse_iterator rend();
    value_type::const_reverse_iterator rend() const;
    value_type::const_reverse_iterator crend() const;

    bool empty() const;
    value_type::size_type size() const;

    void clear();

    value_type::iterator insert(value_type::const_iterator pos,
                                const Variadic& value);

    value_type::iterator erase(value_type::const_iterator pos);
    value_type::iterator erase(value_type::const_iterator first,
                               value_type::const_iterator last);

    void push_back(const Variadic& value);
    void pop_back();
};

class Object final : public PrimitiveJsonValue<std::map<String, Variadic>,
                                               Value::Type::object> {
public:
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;

    Object() = default;
    Object(std::istream& is);

    Object(std::initializer_list<std::pair<const String, Variadic>> il);

    value_type::iterator begin();
    value_type::const_iterator begin() const;
    value_type::iterator end();
    value_type::const_iterator end() const;
    value_type::reverse_iterator rbegin();
    value_type::const_reverse_iterator rbegin() const;
    value_type::reverse_iterator rend();
    value_type::const_reverse_iterator rend() const;
    value_type::const_iterator cbegin() const;
    value_type::const_iterator cend() const;
    value_type::const_reverse_iterator crbegin() const;
    value_type::const_reverse_iterator crend() const;

    bool empty() const;
    value_type::size_type size() const;

    void clear();

    Variadic& at(const String& k);
    const Variadic& at(const String& k) const;
    Variadic& operator[](const String& k);

    void erase(value_type::iterator position);
    void erase(value_type::iterator first, value_type::iterator last);

    value_type::iterator find(const value_type::key_type& v);
    value_type::const_iterator find(const value_type::key_type& v) const;
};

class Null final : public Value {
public:
    Null() = default;
    Null(std::istream& is);

    static constexpr auto type = Type::null;
    Type get_type() const override;
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
};

class Variadic : public Serializable {
public:
    Variadic() = default;
    Variadic(std::istream& is);

    Variadic(const Value&);

    Variadic(const Integer&);
    Variadic(const Real&);
    Variadic(const Boolean&);
    Variadic(const String&);
    Variadic(const Array&);
    Variadic(const Object&);
    Variadic(const Null&);

    Variadic(Integer::value_type);
    Variadic(Real::value_type);
    Variadic(Boolean::value_type);
    Variadic(const String::value_type&);
    Variadic(const char*);

    Variadic(const Variadic& rhs);
    Variadic& operator=(const Variadic& rhs);

    Variadic(Variadic&& rhs) noexcept = default;
    Variadic& operator=(Variadic&& rhs) noexcept = default;

    bool operator==(const Variadic& v) const;
    bool operator!=(const Variadic& v) const;

    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;

    Value* operator->() const;

private:
    std::unique_ptr<Value> value;
};
}

namespace std {
template <typename Primitive, Genrile::Value::Type T>
void swap(Genrile::PrimitiveJsonValue<Primitive, T>& a,
          Genrile::PrimitiveJsonValue<Primitive, T>& b) {
    std::swap(a.value, b.value);
}
}
