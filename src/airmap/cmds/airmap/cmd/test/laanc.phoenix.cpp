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
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query pilot profile: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query pilot profile");
    }
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
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to query pilot profile for aircrafts: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to query pilot profile for aircrafts");
    }
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
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to create flight: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to create flight");
    }
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
    log_.infof(component, "successfully rendered flight brief");

    std::size_t laanc_conflicted = 0;

    for (const auto& ruleset : result.value().rulesets) {
      for (const auto& rule : ruleset.rules) {
        for (const auto& feature : rule.features) {
          if (feature.name == "flight_authorized" && feature.code &&
              feature.code.get() == "laanc_authorization_required" &&
              feature.status == FlightPlan::Briefing::RuleSet::Rule::Status::conflicting) {
            laanc_conflicted++;
          }
        }
      }
    }

    if (laanc_conflicted == 0) {
      log_.errorf(component, "expected laanc authorization to be conflicting");
      context_->stop(::airmap::Context::ReturnCode::error);
    } else {
      submit_flight_plan();
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to render flight briefing: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to render flight briefing");
    }
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
    if (result.value().flight_id) {
      log_.infof(component, "successfully submitted flight plan and received flight id");
      log_.infof(component, "scheduling rendering of flight plan");

      flight_id_ = result.value().flight_id;
      rerender_briefing();
    } else {
      log_.errorf(component, "successfully submitted flight plan but did not receive flight id");
      context_->stop(::airmap::Context::ReturnCode::error);
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to submit flight plan: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to submit flight plan");
    }
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
    static const Microseconds timeout{5 * 1000 * 1000};
    log_.infof(component, "successfully rerendered flight briefing");
    context_->schedule_in(timeout, [this]() { render_final_briefing(); });
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to rerender flight briefing: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to rerender flight briefing");
    }
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
    log_.infof(component, "successfully render final flight briefing");
    delete_flight_plan();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to render final flight briefing: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to render final flight briefing");
    }
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
    delete_flight();
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to delete flight plan: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to delete flight plan");
    }
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
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      log_.errorf(component, "failed to delete flight: %s", e.what());
    } catch (...) {
      log_.errorf(component, "failed to delete flight");
    }
    context_->stop(::airmap::Context::ReturnCode::error);
  }
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixZoo::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -118.364180570977,
        "takeoff_latitude": 34.0168307437243,
        "max_altitude_agl": 100,
        "min_altitude_agl": 1,
        "geometry": {
            "type": "Polygon",
            "coordinates": [
                [
                    [
                        -118.364180570977,
                        34.0168307437243
                    ],
                    [
                        -118.365628044777,
                        34.0026580160048
                    ],
                    [
                        -118.370366665279,
                        33.9889926626396
                    ],
                    [
                        -118.378212913785,
                        33.9763595096654
                    ],
                    [
                        -118.388864275762,
                        33.9652435148409
                    ],
                    [
                        -118.401911015266,
                        33.9560712113537
                    ],
                    [
                        -118.416851982014,
                        33.9491944147928
                    ],
                    [
                        -118.433113834775,
                        33.9448768046356
                    ],
                    [
                        -118.450072947987,
                        33.9432838806463
                    ],
                    [
                        -118.467079178993,
                        33.9444766667622
                    ],
                    [
                        -118.483480612439,
                        33.9484093948682
                    ],
                    [
                        -118.498648367065,
                        33.954931253147
                    ],
                    [
                        -118.512000548545,
                        33.9637921333616
                    ],
                    [
                        -118.523024460256,
                        33.9746521633944
                    ],
                    [
                        -118.531296241358,
                        33.9870946705171
                    ],
                    [
                        -118.536497187531,
                        34.0006420919084
                    ],
                    [
                        -118.538426122695,
                        34.0147742363959
                    ],
                    [
                        -118.537007327878,
                        34.0289482094587
                    ],
                    [
                        -118.532293692779,
                        34.0426192459108
                    ],
                    [
                        -118.524464932309,
                        34.055261654524
                    ],
                    [
                        -118.513820898597,
                        34.0663890684601
                    ],
                    [
                        -118.500770211903,
                        34.0755732160792
                    ],
                    [
                        -118.48581462341,
                        34.0824604786282
                    ],
                    [
                        -118.469529700366,
                        34.086785583271
                    ],
                    [
                        -118.45254258058,
                        34.0883818892682
                    ],
                    [
                        -118.435507670481,
                        34.0871878577985
                    ],
                    [
                        -118.419081251578,
                        34.0832494465733
                    ],
                    [
                        -118.403896008951,
                        34.0767183326201
                    ],
                    [
                        -118.390536499309,
                        34.0678460332763
                    ],
                    [
                        -118.379516534899,
                        34.056974159122
                    ],
                    [
                        -118.371259375595,
                        34.0445211860435
                    ],
                    [
                        -118.366081499672,
                        34.030966270202
                    ],
                    [
                        -118.364180570977,
                        34.0168307437243
                    ]
                ]
            ]
        },
        "buffer": 100,
        "rulesets": ["usa_part_107", "usa_sec_91"],
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
  parameters.start_time                      = Clock::universal_time();
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixSchwegg::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -111.89334869384766,
        "takeoff_latitude": 33.42943568280503,
        "max_altitude_agl": 100,
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
        "rulesets": ["usa_part_107", "usa_sec_91"],
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
  parameters.start_time                      = Clock::universal_time();
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}

airmap::FlightPlans::Create::Parameters laanc::PhoenixUniversity::parameters() {
  static constexpr const char* json          = R"_(
    {
        "takeoff_longitude": -111.92922592163086,
        "takeoff_latitude": 33.412672344081756,
        "max_altitude_agl": 100,
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
        "rulesets": ["usa_part_107", "usa_sec_91"],
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
  parameters.start_time                      = Clock::universal_time();
  parameters.end_time                        = parameters.start_time + Minutes{5};
  return parameters;
}
