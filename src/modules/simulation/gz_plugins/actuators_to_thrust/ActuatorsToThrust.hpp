

#pragma once

#include <gz/msgs/actuators.pb.h>
#include <gz/sim/System.hh>
#include <gz/transport/Node.hh>

namespace custom
{

class ActuatorsToThrust:
	public gz::sim::System,
	public gz::sim::ISystemConfigure
{
public:
	void Configure(const gz::sim::Entity &_entity,
		       const std::shared_ptr<const sdf::Element> &_sdf,
		       gz::sim::EntityComponentManager &_ecm,
		       gz::sim::EventManager &_eventMgr) final;

private:
	void WheelActuatorsCallback(const gz::msgs::Actuators &_msg);
	void EscActuatorsCallback(const gz::msgs::Actuators &_msg);
	void Publish(const gz::msgs::Actuators &_msg, int _rightIndex, int _leftIndex);

	gz::transport::Node _node;
	gz::transport::Node::Publisher _rightThrustPub;
	gz::transport::Node::Publisher _leftThrustPub;

	int _wheelRightIndex{0};
	int _wheelLeftIndex{1};

	int _escRightIndex{1};
	int _escLeftIndex{0};
};

} // namespace custom
