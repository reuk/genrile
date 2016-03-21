#pragma once

#include "types.h"

#include <string>
#include <vector>
#include <map>
#include <ostream>

namespace genrile {

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
//  eof checking

class Serializable {
public:
    virtual std::ostream& write_json_to_stream(std::ostream& os) const = 0;
    virtual void read_json_from_stream(std::istream& is) = 0;
    std::string to_json() const;
};

class Value : public Serializable {
public:
    virtual JsonType get_type() const = 0;

    Value() = default;
    virtual ~Value() noexcept = default;
    Value(const Value&) noexcept = default;
    Value& operator=(const Value&) noexcept = default;
    Value(Value&&) noexcept = default;
    Value& operator=(Value&&) noexcept = default;

    virtual std::unique_ptr<Value> clone() const = 0;

    template <JsonType TYPE>
    typename JsonCastTrait<TYPE>::type& get() {
        if (get_type() != TYPE)
            throw std::runtime_error("incorrect type");
        return dynamic_cast<typename JsonCastTrait<TYPE>::type&>(*this);
    }
    template <JsonType TYPE>
    const typename JsonCastTrait<TYPE>::type& get() const {
        if (get_type() != TYPE)
            throw std::runtime_error("incorrect type");
        return dynamic_cast<const typename JsonCastTrait<TYPE>::type&>(*this);
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

template <typename Primitive, JsonType T>
class PrimitiveJsonValue : public Value {
public:
    using value_type = Primitive;
    static constexpr auto type = T;

    JsonType get_type() const override {
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

class Integer final : public PrimitiveJsonValue<int, JsonType::integer> {
public:
    Integer() = default;
    Integer(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class Real final : public PrimitiveJsonValue<double, JsonType::real> {
public:
    Real() = default;
    Real(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class Boolean final : public PrimitiveJsonValue<bool, JsonType::boolean> {
public:
    Boolean() = default;
    Boolean(std::istream& is);
    std::unique_ptr<Value> clone() const override;
    std::ostream& write_json_to_stream(std::ostream& os) const override;
    void read_json_from_stream(std::istream& is) override;
    using PrimitiveJsonValue::PrimitiveJsonValue;
};

class String final : public PrimitiveJsonValue<std::string, JsonType::string> {
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
    : public PrimitiveJsonValue<std::vector<Variadic>, JsonType::array> {
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

class Object final
    : public PrimitiveJsonValue<std::map<String, Variadic>, JsonType::object> {
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

    static constexpr auto type = JsonType::null;
    JsonType get_type() const override;
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
