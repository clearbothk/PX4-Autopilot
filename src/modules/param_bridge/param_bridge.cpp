#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <drivers/drv_hrt.h>
#include <parameters/param.h>
#include <uORB/Subscription.hpp>
#include <uORB/Publication.hpp>
#include <uORB/topics/param_bridge_request.h>
#include <uORB/topics/param_bridge_response.h>
#include <string.h>

using namespace time_literals;

class ParamBridge : public ModuleBase<ParamBridge>, public ModuleParams, public px4::ScheduledWorkItem
{
public:
	ParamBridge() : ModuleParams(nullptr), ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::lp_default) {}
	~ParamBridge() { ScheduleClear(); }

	static int task_spawn(int argc, char *argv[]);
	static int custom_command(int argc, char *argv[]);
	static int print_usage(const char *reason = nullptr);

	bool init();

private:
	void Run() override;

	uORB::Subscription _request_sub{ORB_ID(param_bridge_request)};
	uORB::Publication<param_bridge_response_s> _response_pub{ORB_ID(param_bridge_response)};
};

bool ParamBridge::init()
{
	ScheduleOnInterval(10_ms);
	return true;
}

void ParamBridge::Run()
{
	if (should_exit()) {
		ScheduleClear();
		exit_and_cleanup();
		return;
	}

	param_bridge_request_s req;

	while (_request_sub.update(&req)) {
		param_bridge_response_s rsp{};
		rsp.timestamp = hrt_absolute_time();
		memcpy(rsp.param_name, req.param_name, sizeof(rsp.param_name));
		rsp.param_name[sizeof(rsp.param_name) - 1] = '\0';

		param_t p = param_find_no_notification(req.param_name);

		if (p == PARAM_INVALID) {
			PX4_WARN("param not found: %.16s", req.param_name);
			rsp.status = param_bridge_response_s::STATUS_NOT_FOUND;

		} else if (req.operation == param_bridge_request_s::OP_GET) {
			if (param_type(p) == PARAM_TYPE_INT32) {
				param_get(p, &rsp.int_value);
				rsp.param_type = param_bridge_response_s::TYPE_INT;
				rsp.status = param_bridge_response_s::STATUS_OK;

			} else if (param_type(p) == PARAM_TYPE_FLOAT) {
				param_get(p, &rsp.float_value);
				rsp.param_type = param_bridge_response_s::TYPE_FLOAT;
				rsp.status = param_bridge_response_s::STATUS_OK;

			} else {
				rsp.status = param_bridge_response_s::STATUS_ERROR;
			}

		} else if (req.operation == param_bridge_request_s::OP_SET) {
			int set_ret = -1;

			if (param_type(p) == PARAM_TYPE_INT32) {
				set_ret = param_set(p, &req.int_value);

			} else if (param_type(p) == PARAM_TYPE_FLOAT) {
				set_ret = param_set(p, &req.float_value);
			}

			if (set_ret == 0) {
				param_save_default(false);
				rsp.status = param_bridge_response_s::STATUS_OK;

			} else {
				PX4_ERR("SET %.16s failed (%d)", req.param_name, set_ret);
				rsp.status = param_bridge_response_s::STATUS_ERROR;
			}

		} else {
			rsp.status = param_bridge_response_s::STATUS_ERROR;
		}

		_response_pub.publish(rsp);
	}
}

int ParamBridge::task_spawn(int argc, char *argv[])
{
	ParamBridge *instance = new ParamBridge();

	if (instance) {
		_object.store(instance);
		_task_id = task_id_is_work_queue;

		if (instance->init()) {
			return PX4_OK;
		}

	} else {
		PX4_ERR("alloc failed");
	}

	delete instance;
	_object.store(nullptr);
	_task_id = -1;

	return PX4_ERROR;
}

int ParamBridge::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int ParamBridge::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION("Parameter bridge: GET/SET PX4 params by name over uXRCE-DDS");
	PRINT_MODULE_USAGE_NAME("param_bridge", "system");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
	return 0;
}

extern "C" __EXPORT int param_bridge_main(int argc, char *argv[])
{
	return ParamBridge::main(argc, argv);
}
