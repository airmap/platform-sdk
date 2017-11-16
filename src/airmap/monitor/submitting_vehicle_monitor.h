#ifndef AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_
#define AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_

#include <airmap/mavlink/vehicle.h>
#include <airmap/monitor/telemetry_submitter.h>

#include <memory>

namespace airmap {
namespace monitor {

class SubmittingVehicleMonitor : public mavlink::Vehicle::Monitor {
 public:
  explicit SubmittingVehicleMonitor(const std::shared_ptr<TelemetrySubmitter>& submitter);

  // From Vehicle::Monitor
  void on_system_status_changed(const Optional<mavlink::State>& old_state, mavlink::State new_state) override;
  void on_position_changed(const Optional<mavlink::GlobalPositionInt>& old_position,
                           const mavlink::GlobalPositionInt& new_position) override;

 private:
  std::shared_ptr<TelemetrySubmitter> submitter_;
};

}  // namespace monitor
}  // namespace airmap

#endif  // AIRMAP_MONITOR_SUBMITTING_VEHICLE_MONITOR_H_
