

#include "ActuatorsToThrust.hpp"

#include <gz/msgs/double.pb.h>
#include <gz/plugin/Register.hh>
#include <gz/sim/Model.hh>

using namespace custom;

GZ_ADD_PLUGIN(
	ActuatorsToThrust,
	gz::sim::System,
	ActuatorsToThrust::ISystemConfigure
)

void ActuatorsToThrust::Configure(const gz::sim::Entity &_entity,
				   const std::shared_ptr<const sdf::Element> &_sdf,
				   gz::sim::EntityComponentManager &_ecm,
				   gz::sim::EventManager &/*_eventMgr*/)
{
	gz::sim::Model model(_entity);
	std::string modelName = model.Name(_ecm);

	if (_sdf->HasElement("wheel_right_actuator_number")) {
		_wheelRightIndex = _sdf->Get<int>("wheel_right_actuator_number");
	}

	if (_sdf->HasElement("wheel_left_actuator_number")) {
		_wheelLeftIndex = _sdf->Get<int>("wheel_left_actuator_number");
	}

	if (_sdf->HasElement("esc_right_actuator_number")) {
		_escRightIndex = _sdf->Get<int>("esc_right_actuator_number");
	}

	if (_sdf->HasElement("esc_left_actuator_number")) {
		_escLeftIndex = _sdf->Get<int>("esc_left_actuator_number");
	}


	std::string wheelTopic = "/model/" + modelName + "/command/motor_speed";
	std::string escTopic = "/" + modelName + "/command/motor_speed";


	std::string rightThrustTopic = "/wamv/thrusters/right/thrust";
	std::string leftThrustTopic = "/wamv/thrusters/left/thrust";

	_rightThrustPub = _node.Advertise<gz::msgs::Double>(rightThrustTopic);
	_leftThrustPub = _node.Advertise<gz::msgs::Double>(leftThrustTopic);

	if (!_node.Subscribe(wheelTopic, &ActuatorsToThrust::WheelActuatorsCallback, this)) {
		gzerr << "ActuatorsToThrust: failed to subscribe to " << wheelTopic << std::endl;
	}

	if (!_node.Subscribe(escTopic, &ActuatorsToThrust::EscActuatorsCallback, this)) {
		gzerr << "ActuatorsToThrust: failed to subscribe to " << escTopic << std::endl;
	}
}

void ActuatorsToThrust::WheelActuatorsCallback(const gz::msgs::Actuators &_msg)
{
	Publish(_msg, _wheelRightIndex, _wheelLeftIndex);
}

void ActuatorsToThrust::EscActuatorsCallback(const gz::msgs::Actuators &_msg)
{
	Publish(_msg, _escRightIndex, _escLeftIndex);
}

void ActuatorsToThrust::Publish(const gz::msgs::Actuators &_msg, int _rightIndex, int _leftIndex)
{
	if (_msg.velocity_size() <= _rightIndex || _msg.velocity_size() <= _leftIndex) {
		return;
	}

	gz::msgs::Double rightCmd;
	rightCmd.set_data(_msg.velocity(_rightIndex));
	_rightThrustPub.Publish(rightCmd);

	gz::msgs::Double leftCmd;
	leftCmd.set_data(_msg.velocity(_leftIndex));
	_leftThrustPub.Publish(leftCmd);
}
