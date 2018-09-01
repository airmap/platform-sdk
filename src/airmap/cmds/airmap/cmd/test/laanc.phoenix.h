//
//  laanc.phoenix.h
//  Airmap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#ifndef AIRMAP_CMDS_AIRMAP_CMD_TEST_LAANC_PHOENIX_H_
#define AIRMAP_CMDS_AIRMAP_CMD_TEST_LAANC_PHOENIX_H_

#include <airmap/cmds/airmap/cmd/test.h>

#include <airmap/evaluation.h>
#include <airmap/flight_plans.h>
#include <airmap/flights.h>
#include <airmap/pilots.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {
namespace test {
namespace laanc {

class Suite : public Test::Suite {
 public:
  void run(const std::shared_ptr<Logger>& logger, const std::shared_ptr<::airmap::Client>& client,
           const std::shared_ptr<::airmap::Context>& context, const ::airmap::Token& token) override;

 protected:
  enum class EvaluationResult {
    error,
    passed,
    finished,
  };

  virtual FlightPlans::Create::Parameters parameters() = 0;
  virtual EvaluationResult evaluate_initial_flight_plan(const FlightPlan& fp);
  virtual EvaluationResult evaluate_initial_briefing(const FlightPlan::Briefing& briefing);
  virtual EvaluationResult evaluate_submitted_flight_plan(const FlightPlan& fp);
  virtual EvaluationResult evaluate_submitted_briefing(const FlightPlan::Briefing& briefing);
  virtual EvaluationResult evaluate_final_briefing(const FlightPlan::Briefing& briefing);

  void query_pilot();
  void handle_query_pilot_finished(const Pilots::Authenticated::Result& result);

  void query_aircrafts();
  void handle_query_aircrafts_finished(const Pilots::Aircrafts::Result& result);

  void plan_flight();
  void handle_plan_flight_finished(const FlightPlans::Create::Result& result);

  void render_briefing();
  void handle_render_briefing_finished(const FlightPlans::RenderBriefing::Result& result);

  void submit_flight_plan();
  void handle_submit_flight_plan_finished(const FlightPlans::Submit::Result& result);

  void rerender_briefing();
  void handle_rerender_briefing_finished(const FlightPlans::RenderBriefing::Result& result);

  void render_final_briefing();
  void handle_render_final_briefing_finished(const FlightPlans::RenderBriefing::Result& result);

  void delete_flight_plan();
  void handle_delete_flight_plan_finished(const FlightPlans::Delete::Result& result);

  void end_flight();
  void handle_end_flight_finished(const Flights::EndFlight::Result& result);

  void delete_flight();
  void handle_delete_flight_finished(const Flights::DeleteFlight::Result& result);

  util::FormattingLogger log_{create_null_logger()};
  std::shared_ptr<::airmap::Client> client_;
  std::shared_ptr<::airmap::Context> context_;
  Token token_;
  Optional<Pilot> pilot_;
  Optional<Pilot::Aircraft> aircraft_;
  Optional<FlightPlan> flight_plan_;
  Optional<Flight::Id> flight_id_;
};

class PhoenixZoo : public Suite {
 public:
  static constexpr const char* name{"laanc.phoenix.zoo"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class PhoenixManual : public Suite {
 public:
  static constexpr const char* name{"laanc.phoenix.manual"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class PhoenixSchwegg : public Suite {
 public:
  static constexpr const char* name{"laanc.phoenix.schwegg"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class PhoenixUniversity : public Suite {
 public:
  static constexpr const char* name{"laanc.phoenix.university"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class KentuckyFlorence : public Suite {
 public:
  static constexpr const char* name{"laanc.kentucky.florence"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class NevadaReno : public Suite {
 public:
  static constexpr const char* name{"laanc.nevada.reno"};

 private:
  EvaluationResult evaluate_final_briefing(const FlightPlan::Briefing& briefing) override;
  FlightPlans::Create::Parameters parameters() override;
};

class ArkansasPineBluff : public Suite {
 public:
  static constexpr const char* name{"laanc.arkansas.pinebluff"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

class WyomingTetonVillage : public Suite {
 public:
  static constexpr const char* name{"laanc.wyoming.tetonvillage"};

 private:
  FlightPlans::Create::Parameters parameters() override;
};

}  // namespace laanc
}  // namespace test
}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_TEST_LAANC_PHOENIX_H_
