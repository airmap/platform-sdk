#include <airmap/cmds/airmap/cmd/test/laanc.phoenix.h>

#include <airmap/codec.h>

namespace laanc = airmap::cmds::airmap::cmd::test::laanc;

namespace {

constexpr const char* component{"laanc::Suite::test"};

}  // namespace

namespace ph = std::placeholders;

void laanc::Suite::run(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::airmap::Client>& client,
                       const std::shared_ptr<::airmap::Context>& context, const ::airmap::Token& token) {
  log_     = util::FormattingLogger{logger};
  client_  = client;
  context_ = context;
  token_   = token;

  query_pilot();
}

laanc::Suite::EvaluationResult laanc::Suite::evaluate_initial_flight_plan(const FlightPlan&) {
  return EvaluationResult::passed;
}

laanc::Suite::EvaluationResult laanc::Suite::evaluate_initial_briefing(const FlightPlan::Briefing& briefing) {
  std::size_t laanc_conflicted = 0;

  for (const auto& ruleset : briefing.evaluation.rulesets) {
    for (const auto& rule : ruleset.rules) {
      for (const auto& feature : rule.features) {
        if (feature.name == "flight_authorized" && feature.code &&
            feature.code.get() == "laanc_authorization_required" &&
            feature.status == RuleSet::Rule::Status::conflicting) {
          laanc_conflicted++;
        }
      }
    }
  }

  return laanc_conflicted == 0 ? EvaluationResult::error : EvaluationResult::passed;
}

laanc::Suite::EvaluationResult laanc::Suite::evaluate_submitted_flight_plan(const FlightPlan& fp) {
  return fp.flight_id ? EvaluationResult::passed : EvaluationResult::error;
}

laanc::Suite::EvaluationResult laanc::Suite::evaluate_submitted_briefing(const FlightPlan::Briefing&) {
  return EvaluationResult::passed;
}

laanc::Suite::EvaluationResult laanc::Suite::evaluate_final_briefing(const FlightPlan::Briefing& b) {
  if (b.evaluation.authorizations.empty()) {
    return EvaluationResult::error;
  }

  auto auth = b.evaluation.authorizations.front();

  if (auth.status != Evaluation::Authorization::Status::accepted || auth.authority.id != "faa-laanc") {
    return EvaluationResult::error;
  }

  return EvaluationResult::passed;
}

void laanc::Suite::query_pilot() {
  Pilots::Authenticated::Parameters parameters;
  parameters.authorization = token_.id();

  client_->pilots().authenticated(parameters, std::bind(&Suite::handle_query_pilot_finished, this, ph::_1));
}

void laanc::Suite::handle_query_pilot_finished(const Pilots::Authenticated::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile");
    pilot_ = result.value();
    query_aircrafts();
  } else {
    log_.errorf(component, "failed to query pilot profile: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::query_aircrafts() {
  Pilots::Aircrafts::Parameters parameters;
  parameters.authorization = token_.id();
  parameters.id            = pilot_.get().id;

  client_->pilots().aircrafts(parameters, std::bind(&Suite::handle_query_aircrafts_finished, this, ph::_1));
}

void laanc::Suite::handle_query_aircrafts_finished(const Pilots::Aircrafts::Result& result) {
  if (result) {
    log_.infof(component, "successfully queried pilot profile for aircrafts");
    if (result.value().empty()) {
      log_.errorf(component, "cannot plan flight, no aircraft associated with pilot");
      context_->stop(::airmap::Context::ReturnCode::error);
    } else {
      aircraft_ = result.value().front();
      plan_flight();
    }
  } else {
    log_.errorf(component, "failed to query pilot profile for aircrafts: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::plan_flight() {
  client_->flight_plans().create_by_polygon(parameters(), std::bind(&Suite::handle_plan_flight_finished, this, ph::_1));
}

void laanc::Suite::handle_plan_flight_finished(const FlightPlans::Create::Result& result) {
  if (result) {
    log_.infof(component, "successfully created flight plan");
    flight_plan_ = result.value();
    render_briefing();
  } else {
    log_.errorf(component, "failed to create flight: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::render_briefing() {
  FlightPlans::RenderBriefing::Parameters parameters;
  parameters.id            = flight_plan_.get().id;
  parameters.authorization = token_.id();

  client_->flight_plans().render_briefing(parameters, std::bind(&Suite::handle_render_briefing_finished, this, ph::_1));
}

void laanc::Suite::handle_render_briefing_finished(const FlightPlans::RenderBriefing::Result& result) {
  if (result) {
    if (evaluate_initial_briefing(result.value()) == EvaluationResult::passed) {
      log_.infof(component, "successfully rendered flight brief");
      submit_flight_plan();
    } else {
      log_.errorf(component, "expected laanc authorization to be conflicting");
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to render flight briefing: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::submit_flight_plan() {
  FlightPlans::Submit::Parameters parameters;
  parameters.id            = flight_plan_.get().id;
  parameters.authorization = token_.id();

  client_->flight_plans().submit(parameters, std::bind(&Suite::handle_submit_flight_plan_finished, this, ph::_1));
}

void laanc::Suite::handle_submit_flight_plan_finished(const FlightPlans::Submit::Result& result) {
  if (result) {
    auto er = evaluate_submitted_flight_plan(result.value());

    if (er == EvaluationResult::passed) {
      log_.infof(component, "successfully submitted flight plan and received flight id");
      flight_id_ = result.value().flight_id;
      rerender_briefing();
    } else {
      log_.errorf(component, "successfully submitted flight plan but did not receive flight id");
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to submit flight plan: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::rerender_briefing() {
  FlightPlans::RenderBriefing::Parameters parameters;
  parameters.id            = flight_plan_.get().id;
  parameters.authorization = token_.id();

  client_->flight_plans().render_briefing(parameters,
                                          std::bind(&Suite::handle_rerender_briefing_finished, this, ph::_1));
}

void laanc::Suite::handle_rerender_briefing_finished(const FlightPlans::RenderBriefing::Result& result) {
  if (result) {
    auto er = evaluate_submitted_briefing(result.value());

    if (er == EvaluationResult::passed) {
      static const Microseconds timeout{5 * 1000 * 1000};
      log_.infof(component, "successfully rerendered flight briefing");
      log_.infof(component, "scheduling final rendering of flight plan");
      context_->schedule_in(timeout, [this]() { render_final_briefing(); });
    } else {
      log_.errorf(component, "successfully rerendered flight briefing but evluation failed");
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    log_.errorf(component, "failed to rerender flight briefing: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::render_final_briefing() {
  FlightPlans::RenderBriefing::Parameters parameters;
  parameters.id            = flight_plan_.get().id;
  parameters.authorization = token_.id();

  client_->flight_plans().render_briefing(parameters,
                                          std::bind(&Suite::handle_render_final_briefing_finished, this, ph::_1));
}

void laanc::Suite::handle_render_final_briefing_finished(const FlightPlans::RenderBriefing::Result& result) {
  if (result) {
    auto er = evaluate_final_briefing(result.value());

    if (er == EvaluationResult::passed) {
      log_.infof(component, "successfully render final flight briefing");

      delete_flight_plan();
    } else {
      log_.errorf(component, "missing laanc authorization in flight briefing");
      context_->stop(::airmap::Context::ReturnCode::error);
      return;
    }
  } else {
    log_.errorf(component, "failed to render final flight briefing: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::delete_flight_plan() {
  FlightPlans::Delete::Parameters parameters;
  parameters.id            = flight_plan_.get().id;
  parameters.authorization = token_.id();

  client_->flight_plans().delete_(parameters, std::bind(&Suite::handle_delete_flight_plan_finished, this, ph::_1));
}

void laanc::Suite::handle_delete_flight_plan_finished(const FlightPlans::Delete::Result& result) {
  if (result) {
    log_.infof(component, "successfully deleted flight plan");
    end_flight();
  } else {
    log_.errorf(component, "failed to delete flight plan: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::end_flight() {
  Flights::EndFlight::Parameters parameters;
  parameters.id            = flight_id_.get();
  parameters.authorization = token_.id();

  client_->flights().end_flight(parameters, std::bind(&Suite::handle_end_flight_finished, this, ph::_1));
}

void laanc::Suite::handle_end_flight_finished(const Flights::EndFlight::Result& result) {
  if (result) {
    log_.infof(component, "successfully ended flight");
    delete_flight();
  } else {
    log_.errorf(component, "failed to end flight: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

void laanc::Suite::delete_flight() {
  Flights::DeleteFlight::Parameters parameters;
  parameters.id            = flight_id_.get();
  parameters.authorization = token_.id();

  client_->flights().delete_flight(parameters, std::bind(&Suite::handle_delete_flight_finished, this, ph::_1));
}

void laanc::Suite::handle_delete_flight_finished(const Flights::DeleteFlight::Result& result) {
  if (result) {
    log_.infof(component, "successfully deleted flight");
    context_->stop();
  } else {
    log_.errorf(component, "failed to delete flight: %s", result.error());
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixManual::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -118.364180570977,
        "takeoff_latitude": 34.0168307437243,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -112.10620880126953,
                33.431011556740536
              ],
              [
                -112.1000289916992,
                33.42793141281223
              ],
              [
                -112.09402084350586,
                33.42914915719729
              ],
              [
                -112.09917068481445,
                33.431011556740536
              ],
              [
                -112.10620880126953,
                33.431011556740536
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = DateTime(Clock::universal_time().date()) + Hours{40};
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixZoo::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -111.95188522338867,
        "takeoff_latitude": 33.45135208763854,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -111.95188522338867,
                33.45135208763854
              ],
              [
                -111.95265769958495,
                33.449132050292846
              ],
              [
                -111.95059776306152,
                33.446840339233134
              ],
              [
                -111.94896697998047,
                33.44691195612031
              ],
              [
                -111.94888114929199,
                33.44791458633172
              ],
              [
                -111.94673538208006,
                33.44798620233191
              ],
              [
                -111.94493293762207,
                33.448129434154886
              ],
              [
                -111.94398880004883,
                33.44949012467619
              ],
              [
                -111.94441795349121,
                33.451065634400095
              ],
              [
                -111.94699287414551,
                33.451065634400095
              ],
              [
                -111.94862365722656,
                33.45213982916561
              ],
              [
                -111.95094108581543,
                33.45199660396564
              ],
              [
                -111.95188522338867,
                33.45135208763854
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = move_to_hour(Clock::universal_time(), 16);
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixSchwegg::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -111.89334869384766,
        "takeoff_latitude": 33.42943568280503,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [
                    [
                        -111.89334869384766,
                        33.42943568280503
                    ],
                    [
                        -111.89317703247069,
                        33.48027899035629
                    ],
                    [
                        -111.9070816040039,
                        33.48027899035629
                    ],
                    [
                        -111.90485000610352,
                        33.430581775777526
                    ],
                    [
                        -111.89334869384766,
                        33.42943568280503
                    ]
                ]
            ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = move_to_hour(Clock::universal_time(), 16);
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixUniversity::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -111.92922592163086,
        "takeoff_latitude": 33.412672344081756,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [
                    [
                        -111.92922592163086,
                        33.412672344081756
                    ],
                    [
                        -111.9257926940918,
                        33.412529053733444
                    ],
                    [
                        -111.92699432373047,
                        33.415681386786396
                    ],
                    [
                        -111.92682266235352,
                        33.41955000278094
                    ],
                    [
                        -111.92682266235352,
                        33.4216991594148
                    ],
                    [
                        -111.92253112792969,
                        33.4216991594148
                    ],
                    [
                        -111.9206428527832,
                        33.42485115958141
                    ],
                    [
                        -111.92253112792969,
                        33.42671365134311
                    ],
                    [
                        -111.92544937133789,
                        33.427859780257556
                    ],
                    [
                        -111.92682266235352,
                        33.427859780257556
                    ],
                    [
                        -111.92819595336914,
                        33.42671365134311
                    ],
                    [
                        -111.9290542602539,
                        33.42528096891931
                    ],
                    [
                        -111.93248748779297,
                        33.42341844641943
                    ],
                    [
                        -111.93780899047852,
                        33.42284535453425
                    ],
                    [
                        -111.93780899047852,
                        33.42040967181813
                    ],
                    [
                        -111.93763732910156,
                        33.41725751040402
                    ],
                    [
                        -111.93489074707031,
                        33.416397810158834
                    ],
                    [
                        -111.93180084228516,
                        33.4142485223105
                    ],
                    [
                        -111.92922592163086,
                        33.412672344081756
                    ]
                ]
            ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = DateTime(Clock::universal_time().date()) + Hours{16};
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::KentuckyFlorence::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -84.61800813674927,
        "takeoff_latitude": 39.00109310191585,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -84.61800813674927,
                39.00109310191585
              ],
              [
                -84.61790084838867,
                39.000759591417825
              ],
              [
                -84.62045431137085,
                38.999025311485944
              ],
              [
                -84.61693525314331,
                38.998058098444474
              ],
              [
                -84.6162486076355,
                38.99812480321662
              ],
              [
                -84.61627006530762,
                38.999125367252894
              ],
              [
                -84.61740732192993,
                39.00089299580567
              ],
              [
                -84.6162486076355,
                39.001526663213
              ],
              [
                -84.61652755737305,
                39.00172676753097
              ],
              [
                -84.61800813674927,
                39.00109310191585
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = DateTime(Clock::universal_time().date()) + Hours{16};
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

laanc::Suite::EvaluationResult laanc::NevadaReno::evaluate_final_briefing(const FlightPlan::Briefing& b) {
  if (b.evaluation.authorizations.empty()) {
    return EvaluationResult::error;
  }

  auto auth = b.evaluation.authorizations.front();

  if (auth.status != Evaluation::Authorization::Status::rejected || auth.authority.id != "faa-laanc") {
    return EvaluationResult::error;
  }

  return EvaluationResult::passed;
}

airmap::FlightPlans::Create::Parameters laanc::NevadaReno::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -119.79702472686768,
        "takeoff_latitude": 39.51084501367582,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -119.79702472686768,
                39.51084501367582
              ],
              [
                -119.78983640670778,
                39.51084501367582
              ],
              [
                -119.78983640670778,
                39.51134165240756
              ],
              [
                -119.79702472686768,
                39.51134165240756
              ],
              [
                -119.79702472686768,
                39.51084501367582
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = DateTime(Clock::universal_time().date()) + Hours{16};
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::ArkansasPineBluff::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -91.97863340377808,
        "takeoff_latitude": 34.219946599274,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -91.97863340377808,
                34.219946599274
              ],
              [
                -91.97835445404053,
                34.218101304583485
              ],
              [
                -91.97726011276245,
                34.217249616479045
              ],
              [
                -91.97569370269775,
                34.21865134857525
              ],
              [
                -91.97863340377808,
                34.219946599274
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = DateTime(Clock::universal_time().date()) + Hours{16};
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::WyomingTetonVillage::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -110.82299709320068,
        "takeoff_latitude": 43.58135458245754,
        "max_altitude_agl": 20,
        "min_altitude_agl": 1,
        "geometry": {
          "type": "Polygon",
          "coordinates": [
            [
              [
                -110.82299709320068,
                43.58135458245754
              ],
              [
                -110.82359790802002,
                43.581820896362856
              ],
              [
                -110.83806037902832,
                43.577002145173076
              ],
              [
                -110.83600044250488,
                43.56876274176711
              ],
              [
                -110.82286834716797,
                43.56988212311368
              ],
              [
                -110.82299709320068,
                43.58135458245754
              ]
            ]
          ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107"],
        "flight_features": {
          "environment_visibility": 5000.0,
          "flight_max_speed": 3.0,
          "flight_vlos": true,
          "flight_authorized": false,
          "flight_carries_property_for_hire": false,
          "flight_crosses_us_state_border": false,
          "pilot_first_name": "Thomas",
          "pilot_last_name": "Voß",
          "pilot_phone_number": "+491621074430",
          "pilot_in_command_part107_cert": true,
          "uav_nav_lights": true,
          "uav_preflight_check": true,
          "uav_registered": true,
          "uav_weight" : 1.0
        }
    }
  )_";
  FlightPlans::Create::Parameters parameters = nlohmann::json::parse(json);
  parameters.authorization                   = token_.id();
  parameters.pilot                           = pilot_.get();
  parameters.aircraft                        = aircraft_.get();
  parameters.start_time                      = move_to_hour(Clock::universal_time(), 16);
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}
