#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE error

#include <airmap/error.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_CASE(error_value_default_ctor_yields_correct_type) {
  airmap::Error::Value value;
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::undefined);
  BOOST_CHECK(!value.boolean());
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_boolean_yields_correct_type) {
  airmap::Error::Value value{false};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::boolean);
  BOOST_CHECK(!value.boolean());
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_integer_yields_correct_type) {
  airmap::Error::Value value{std::int64_t{42}};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::integer);
  BOOST_CHECK(value.integer() == 42);
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_double_yields_correct_type) {
  airmap::Error::Value value{double{42.}};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::floating_point);
  BOOST_CHECK(!(value.floating_point() < 42.) && !(value.floating_point() > 42.));
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_string_yields_correct_type) {
  airmap::Error::Value value{std::string{"42"}};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::string);
  BOOST_CHECK(value.string() == "42");
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_blob_yields_correct_type) {
  std::vector<std::uint8_t> ref{'4', '2'};
  airmap::Error::Value value{ref};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::blob);
  BOOST_CHECK(value.blob() == ref);
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_dictionary_yields_correct_type) {
  std::map<airmap::Error::Value, airmap::Error::Value> dict = {
      {airmap::Error::Value{std::int64_t{42}}, airmap::Error::Value{std::string{"42"}}}};

  airmap::Error::Value value{dict};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::dictionary);
  BOOST_CHECK(value.dictionary() == dict);
}

BOOST_AUTO_TEST_CASE(error_value_ctor_with_vector_yields_correct_type) {
  std::vector<airmap::Error::Value> vec = {airmap::Error::Value{std::int64_t{42}},
                                           airmap::Error::Value{std::string{"42"}}};

  airmap::Error::Value value{vec};
  BOOST_CHECK(value.type() == airmap::Error::Value::Type::vector);
  BOOST_CHECK(value.vector() == vec);
}

BOOST_AUTO_TEST_CASE(error_value_copy_ctor_yields_correct_type) {
  {
    airmap::Error::Value v{false};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::boolean);
    BOOST_CHECK(w.boolean() == false);
  }

  {
    airmap::Error::Value v{std::int64_t{42}};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::integer);
    BOOST_CHECK(w.integer() == 42);
  }

  {
    airmap::Error::Value v{double{42}};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::floating_point);
    BOOST_CHECK(!(w.floating_point() < 42.f) && !(w.floating_point() > 42.f));
  }

  {
    airmap::Error::Value v{std::string{"42"}};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::string);
    BOOST_CHECK(w.string() == "42");
  }

  {
    auto ref = std::vector<std::uint8_t>{'4', '2'};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::blob);
    BOOST_CHECK(w.blob() == ref);
  }

  {
    auto ref = std::map<airmap::Error::Value, airmap::Error::Value>{
        {airmap::Error::Value{std::int64_t{42}}, airmap::Error::Value{std::string{"42"}}}};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::dictionary);
    BOOST_CHECK(w.dictionary() == ref);
  }

  {
    auto ref = std::vector<airmap::Error::Value>{airmap::Error::Value{std::int64_t{42}},
                                                 airmap::Error::Value{std::string{"42"}}};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{v};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::vector);
    BOOST_CHECK(w.vector() == ref);
  }
}

BOOST_AUTO_TEST_CASE(error_value_move_ctor_yields_correct_type) {
  {
    airmap::Error::Value v{false};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::boolean);
    BOOST_CHECK(w.boolean() == false);
  }

  {
    airmap::Error::Value v{std::int64_t{42}};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::integer);
    BOOST_CHECK(w.integer() == 42);
  }

  {
    airmap::Error::Value v{double{42}};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::floating_point);
    BOOST_CHECK(!(w.floating_point() < 42.f) && !(w.floating_point() > 42.f));
  }

  {
    airmap::Error::Value v{std::string{"42"}};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::string);
    BOOST_CHECK(w.string() == "42");
  }

  {
    auto ref = std::vector<std::uint8_t>{'4', '2'};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::blob);
    BOOST_CHECK(w.blob() == ref);
  }

  {
    auto ref = std::map<airmap::Error::Value, airmap::Error::Value>{
        {airmap::Error::Value{std::int64_t{42}}, airmap::Error::Value{std::string{"42"}}}};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::dictionary);
    BOOST_CHECK(w.dictionary() == ref);
  }

  {
    auto ref = std::vector<airmap::Error::Value>{airmap::Error::Value{std::int64_t{42}},
                                                 airmap::Error::Value{std::string{"42"}}};
    airmap::Error::Value v{ref};
    airmap::Error::Value w{std::move(v)};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::vector);
    BOOST_CHECK(w.vector() == ref);
  }
}

BOOST_AUTO_TEST_CASE(error_value_assignment_operator_yields_correct_type) {
  {
    auto w = airmap::Error::Value{false};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::boolean);
    BOOST_CHECK(w.boolean() == false);
  }

  {
    auto w = airmap::Error::Value{std::int64_t{42}};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::integer);
    BOOST_CHECK(w.integer() == 42);
  }

  {
    auto w = airmap::Error::Value{double{42}};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::floating_point);
    BOOST_CHECK(!(w.floating_point() < 42.f) && !(w.floating_point() > 42.f));
  }

  {
    auto w = airmap::Error::Value{std::string{"42"}};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::string);
    BOOST_CHECK(w.string() == "42");
  }

  {
    auto ref = std::vector<std::uint8_t>{'4', '2'};
    auto w   = airmap::Error::Value{ref};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::blob);
    BOOST_CHECK(w.blob() == ref);
  }

  {
    auto ref = std::map<airmap::Error::Value, airmap::Error::Value>{
        {airmap::Error::Value{std::int64_t{42}}, airmap::Error::Value{std::string{"42"}}}};
    auto w = airmap::Error::Value{ref};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::dictionary);
    BOOST_CHECK(w.dictionary() == ref);
  }

  {
    auto ref = std::vector<airmap::Error::Value>{airmap::Error::Value{std::int64_t{42}},
                                                 airmap::Error::Value{std::string{"42"}}};
    auto w = airmap::Error::Value{ref};
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::vector);
    BOOST_CHECK(w.vector() == ref);
  }
}

BOOST_AUTO_TEST_CASE(error_value_move_operator_yields_correct_type) {
  {
    auto w = std::move(airmap::Error::Value{false});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::boolean);
    BOOST_CHECK(w.boolean() == false);
  }

  {
    auto w = std::move(airmap::Error::Value{std::int64_t{42}});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::integer);
    BOOST_CHECK(w.integer() == 42);
  }

  {
    auto w = std::move(airmap::Error::Value{double{42}});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::floating_point);
    BOOST_CHECK(!(w.floating_point() < 42.f) && !(w.floating_point() > 42.f));
  }

  {
    auto w = std::move(airmap::Error::Value{std::string{"42"}});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::string);
    BOOST_CHECK(w.string() == "42");
  }

  {
    auto ref = std::vector<std::uint8_t>{'4', '2'};
    auto w   = std::move(airmap::Error::Value{ref});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::blob);
    BOOST_CHECK(w.blob() == ref);
  }

  {
    auto ref = std::map<airmap::Error::Value, airmap::Error::Value>{
        {airmap::Error::Value{std::int64_t{42}}, airmap::Error::Value{std::string{"42"}}}};
    auto w = std::move(airmap::Error::Value{ref});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::dictionary);
    BOOST_CHECK(w.dictionary() == ref);
  }

  {
    auto ref = std::vector<airmap::Error::Value>{airmap::Error::Value{std::int64_t{42}},
                                                 airmap::Error::Value{std::string{"42"}}};
    auto w = std::move(airmap::Error::Value{ref});
    BOOST_CHECK(w.type() == airmap::Error::Value::Type::vector);
    BOOST_CHECK(w.vector() == ref);
  }
}

BOOST_AUTO_TEST_CASE(error_value_operator_outputs_correct_string) {
  auto error =
      airmap::Error{"this is a test"}
          .description("with a description")
          .value(airmap::Error::Value{std::string{"boolean"}}, airmap::Error::Value{false})
          .value(airmap::Error::Value{std::string{"integer"}}, airmap::Error::Value{std::int64_t{42}})
          .value(airmap::Error::Value{std::string{"floating_point"}}, airmap::Error::Value{double{42}})
          .value(airmap::Error::Value{std::string{"string"}}, airmap::Error::Value{std::string{"42"}})
          .value(airmap::Error::Value{std::string{"blob"}}, airmap::Error::Value{std::vector<std::uint8_t>{'4', '2'}})
          .value(airmap::Error::Value{std::string{"dictionary"}},
                 airmap::Error::Value{std::map<airmap::Error::Value, airmap::Error::Value>{
                     {airmap::Error::Value{std::string{"boolean"}}, airmap::Error::Value{false}},
                     {airmap::Error::Value{std::string{"integer"}}, airmap::Error::Value{std::int64_t{42}}},
                     {airmap::Error::Value{std::string{"floating_point"}}, airmap::Error::Value{double{42.}}},
                     {airmap::Error::Value{std::string{"string"}}, airmap::Error::Value{std::string{"42"}}},
                     {airmap::Error::Value{std::string{"blob"}},
                      airmap::Error::Value{std::vector<std::uint8_t>{'4', '2'}}},
                 }})
          .value(airmap::Error::Value{std::string{"vector"}},
                 airmap::Error::Value{std::vector<airmap::Error::Value>{
                     airmap::Error::Value{false}, airmap::Error::Value{std::int64_t{42}},
                     airmap::Error::Value{double{42.}}, airmap::Error::Value{std::string{"42"}},
                     airmap::Error::Value{std::vector<std::uint8_t>{'4', '2'}},
                 }});

  std::stringstream ss;
  ss << error;
  std::cout << error;
}
