/*
  Copyright 2019 Equinor ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UDA_VALUE_HPP
#define UDA_VALUE_HPP

#include <string>
#include <iosfwd>
#include <optional>

#include <opm/input/eclipse/Units/Dimension.hpp>

namespace Opm {

class UDAValue {
public:
    UDAValue();
    explicit UDAValue(double);
    explicit UDAValue(const std::string&);
    explicit UDAValue(const Dimension& dim);
    UDAValue(double data, const Dimension& dim);
    UDAValue(const std::string& data, const Dimension& dim);

    /*
      The assignment operators have been explicitly deleted, that is to prevent
      people from adding them at a later stage. It seems very tempting/natural
      to implement these assignment operators, but the problem is that the
      resulting UDA object will typically have the wrong dimension member, and
      subtle dimension related bugs will arise.
    */
    UDAValue& operator=(double value) = delete;
    UDAValue& operator=(const std::string& value) = delete;
    void update(double d);
    void update(const std::string& s);
    void update_value(const UDAValue& other);

    static UDAValue serializationTestObject();

    /*
     * The *_value_or functions will throw an exception in case of non-zero string value
     */
    double raw_value_or(const double raw_default_value) const;
    double SI_value_or(const double SI_default_value) const;

    /*
      The get<double>() and get<std::string>() methods will throw an
      exception if the internal type and the template parameter disagree.
    */

    template<typename T>
    T get() const;

    /*
      The getSI() can only be called for numerical values.
    */
    double getSI() const;
    bool zero() const;

    //epsilon limit  = 1.E-20  (~= 0.)
    double epsilonLimit() const;

    bool is_defined() const;
    template<typename T>
    bool is() const;

    void assert_numeric() const;
    void assert_numeric(const std::string& error_msg) const;
    void assert_maybe_numeric() const;
    const Dimension& get_dim() const;
    void set_dim(const Dimension& new_dim);

    bool operator==(const UDAValue& other) const;
    bool operator!=(const UDAValue& other) const;

    bool is_numeric() const { return double_value.has_value(); }

    template<class Serializer>
    void serializeOp(Serializer& serializer)
    {
        serializer(double_value);
        serializer(string_value);
        serializer(dim);
    }

    void operator*=(double rhs);


private:
    std::optional<double> double_value;
    std::string string_value;

    Dimension dim;
};

std::ostream& operator<<( std::ostream& stream, const UDAValue& uda_value );
}



#endif
